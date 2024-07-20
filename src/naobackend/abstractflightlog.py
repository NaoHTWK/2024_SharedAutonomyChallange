#!/usr/bin/env python3

from __future__ import annotations

import threading
from typing import Callable

from google.protobuf.message import DecodeError

from src.naobackend.logentryadapter import LogEntryAdapter
from src.naobackend.lola_status import LolaStatus
from src.naobackend.lola_status import convert_from_proto as lola_status_convert_from_proto
from src.naobackend.proto import LolaStatus as LolaStatusPB
from src.naobackend.proto import VisualizerTransaction
from src.naobackend.visualizer_parameter import VisualizerParameter


class AbstractFlightLog:
    _parameter_subscribers: dict[str, list[Callable[[VisualizerParameter], None]]]
    _parameter_flat_subscribers: list[Callable[[dict[str, any]], None]]
    _parameter_waiter: dict[str, threading.Event]

    _log_entry_subscribers: dict[tuple[str, str], list[Callable[["LogEntryAdapter"], None]]]
    _lola_debug_frames_subscribers: list[Callable[[LolaStatus], None]]
    _lola_debug_frames_flat_subscribers: list[Callable[[dict[str, float]], None]]
    parameter: dict[str, VisualizerParameter]  # Contain all currently known parameter with values

    def __init__(self):
        self._parameter_subscribers = dict()
        self._parameter_flat_subscribers = list()
        self._parameter_waiter = dict()
        self._lola_debug_frames_subscribers = list()
        self._lola_debug_frames_flat_subscribers = list()
        self._log_entry_subscribers = dict()
        self.parameter = dict()

    def wait_for_parameter(self, subsystem: str, timeout: float = None) -> bool:
        """ Waits until a parameter from a visual transaction is available. You can
        specify a timeout in seconds as float. If the timeout is None your call returns only when the
        parameter is available. If a timeout is given the return value is true if the parameter is
        available or false if the timeout was reacheed.
        """
        if subsystem in self.parameter:
            return True

        w = threading.Event()
        self._parameter_waiter[subsystem] = w
        return w.wait(timeout)

    def subscribe_parameter(self, subsystem: str, callback: Callable[[VisualizerParameter], None]):
        """ Subscribe to a parameter in a visual transaction. The callback is called when a new
        parameter with the given subsystem is available. Please note that the callback is maybe called by another thread,"""
        if subsystem not in self._parameter_subscribers:
            self._parameter_subscribers[subsystem] = [callback]
        else:
            self._parameter_subscribers[subsystem].append(callback)

    def subscribe_parameter_flat(self, callback: Callable[[dict[str, any]], None]):
        """
        Subscribe to all parameter, The callable get a dictionary where the contents is flattened.
        For examplay subsystem 'body' parameter 'angle'  and value 1.123 is given as {'body.angle':  1.123}.
        """
        self._parameter_flat_subscribers.append(callback)

    def subscribe_lola_debug(self, callback: Callable[[LolaStatus], None]):
        """
        If a new LolaStatus frame is available the callback is called. Attention: The call can be in another thread.
        """
        if callable not in self._lola_debug_frames_subscribers:
            self._lola_debug_frames_subscribers.append(callback)

    def subscribe_lola_debug_flat(self, callback: Callable[[dict[str, float]], None]):
        """
        Similar to 'subscribe_parameter_flat' the LolaStatus object is flattended to a map with values.
        """
        if callable not in self._lola_debug_frames_flat_subscribers:
            self._lola_debug_frames_flat_subscribers.append(callback)

    def subscribe_log_entry(self, subsystem: str, typeinfo: str, callback: Callable[[LogEntryAdapter], None]):
        """
        All functions above are syntactic sugar to this function. You register here a callback to a LogEntry.
        You have to specify the 'subsystem' and the 'typeinfo' to the LogEntry. A LogEntry only contains bytes so
        you have to unserialize / interpret the contents yourself. The callback can be called from another thread.
        """
        if (subsystem, typeinfo) not in self._log_entry_subscribers:
            self._log_entry_subscribers[(subsystem, typeinfo)] = [callback]
        else:
            self._log_entry_subscribers[(subsystem, typeinfo)].append(callback)

    def _process_subscriber_updates_visualizer_processor(self, entry: LogEntryAdapter):
        """ This is a helper function which notifies all waiter for parameter and
         call all necessary callbacks """
        vis = VisualizerTransaction()

        try:
            vis.ParseFromString(entry.logEntry()[4:])
        except DecodeError:
            return

        updated_parameter = False
        for p in vis.parameter:
            if vis.subsystem not in self.parameter.keys():
                self.parameter[vis.subsystem] = VisualizerParameter(vis.subsystem)

            self.parameter[vis.subsystem].recv_time = vis.time
            if len(p.floatParams) > 0:
                self.parameter[vis.subsystem].entries[p.name] = p.floatParams
            else:
                self.parameter[vis.subsystem].entries[p.name] = p.intParams

            updated_parameter = True

        if vis.subsystem in self._parameter_waiter:
            self._parameter_waiter[vis.subsystem].set()
            self._parameter_waiter.pop(vis.subsystem)

        if updated_parameter and vis.subsystem in self._parameter_subscribers:
            for subscriber in self._parameter_subscribers[vis.subsystem]:
                subscriber(self.parameter[vis.subsystem])

        if updated_parameter and len(self._parameter_flat_subscribers) > 0:
            for subscriber in self._parameter_flat_subscribers:
                subscriber(self.parameter[vis.subsystem].flatten())

    def _process_subscriber_updates_lola_connector(self, entry: LogEntryAdapter):

        if len(self._lola_debug_frames_subscribers) == 0 and len(self._lola_debug_frames_flat_subscribers) == 0:
            return

        lola_status_pb = LolaStatusPB()

        try:
            lola_status_pb.ParseFromString(entry.logEntry())
        except DecodeError:
            return

        lola_status = lola_status_convert_from_proto(lola_status_pb)

        for subscriber in self._lola_debug_frames_subscribers:
            subscriber(lola_status)

        if len(self._lola_debug_frames_flat_subscribers) > 0:
            lola_status_dict = lola_status.flatten()
            for subscriber in self._lola_debug_frames_flat_subscribers:
                subscriber(lola_status_dict)

    def _process_subscriber_updates_log_entries(self, entry: LogEntryAdapter):

        if len(self._log_entry_subscribers) == 0:
            return

        subscribers = self._log_entry_subscribers.get((entry.subsystem(), entry.typeinfo()), [])
        if len(subscribers) == 0:
            return

        for subscriber in subscribers:
            subscriber(entry)
