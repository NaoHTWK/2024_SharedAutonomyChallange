import socket
import struct
import threading
import sys
from queue import SimpleQueue
import pygame
import io

class RobotImageDisplay:
    MAX_IMAGES_IN_QUEUE = 5

    def __init__(self):
        self.images_upper_cam = SimpleQueue()
        self.images_lower_cam = SimpleQueue()
        self.ipaddr = None
        self.port = 9999
        self.running = False
        self.screen = None  # Add a screen attribute

    def _discard_old_images(self, q: SimpleQueue):
        while q.qsize() > self.MAX_IMAGES_IN_QUEUE:
            q.get()

    def _receive_image_thread(self):
        try:
            sock = socket.create_connection((self.ipaddr, self.port))
        except (ConnectionRefusedError, OSError):
            print('''
Error: could not connect to robot {0} port {1}.
Please check: Is the robot turned on and reachable via network and fw_sydney started?
Re-raising exception roboJpegStream...
'''.format(self.ipaddr, self.port), file=sys.stderr)
            raise

        fp = sock.makefile("rb")
        while self.running:
            length, is_lower_cam = struct.unpack(">ib", fp.read(5))
            buf = fp.read(length)
            if is_lower_cam != 0:
                self.images_lower_cam.put(buf)
                self._discard_old_images(self.images_lower_cam)
            else:
                self.images_upper_cam.put(buf)
                self._discard_old_images(self.images_upper_cam)

    def start_receiving(self, ipaddr: str):
        self.ipaddr = ipaddr
        self.running = True
        threading.Thread(target=self._receive_image_thread, name="RobotJpegReceiver", daemon=True).start()

    def draw_images(self):
        if self.screen is None:
            return

        if not self.images_upper_cam.empty():
            image_data = self.images_upper_cam.get()
            image_upper = pygame.image.load(io.BytesIO(image_data))
            self.screen.blit(image_upper, (0, 0))

        if not self.images_lower_cam.empty():
            image_data = self.images_lower_cam.get()
            image_lower = pygame.image.load(io.BytesIO(image_data))
            self.screen.blit(image_lower, (0, 0))
