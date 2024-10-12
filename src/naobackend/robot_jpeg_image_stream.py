#!/usr/bin/env python3

import socket
import struct
import threading
import sys
from queue import SimpleQueue


class RobotJpegImageStream(threading.Thread):
    images_upper_cam: SimpleQueue[bytes]
    images_lower_cam: SimpleQueue[bytes]

    max_images_in_queue = 1

    def __init__(self, ipaddr: str, port: int = 9999):
        threading.Thread.__init__(self)
        self.images_upper_cam = SimpleQueue()
        self.images_lower_cam = SimpleQueue()
        threading.Thread(target=self._receive_image_thread, name="RobotJpegReceiver", args=(ipaddr, port),
                         daemon=True).start()

    def _discard_old_images(self, q: SimpleQueue[bytes]):
        while q.qsize() > self.max_images_in_queue:
            q.get()

    def _receive_image_thread(self, ipaddr: str, port: int):
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
        sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        sock.bind((ipaddr, port))  # Bind to all available interfaces

        print(f"UDP server listening on port {port}")

        while True:
            try:
                data, addr = sock.recvfrom(65536)  # Max UDP packet size
                if not data:
                    continue

                # Unpack the header (size and camera ID)
                size = struct.unpack(">I", data[:4])[0]
                cam_id = data[4]

                # Extract the image data
                buf = data[5:]

                if len(buf) != size:
                    print(f"Warning: Received {len(buf)} bytes, expected {size} bytes", file=sys.stderr)
                    continue

                if cam_id != 0:
                    self.images_lower_cam.put(buf)
                    self._discard_old_images(self.images_lower_cam)
                else:
                    self.images_upper_cam.put(buf)
                    self._discard_old_images(self.images_upper_cam)

            except Exception as e:
                print(f"Error receiving UDP packet: {e}", file=sys.stderr)

        sock.close()