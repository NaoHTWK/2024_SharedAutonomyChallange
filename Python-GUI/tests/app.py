#!/usr/bin/env python3
import socket
import struct
import threading
import sys
from queue import SimpleQueue
import pygame
import io

class RobotImageReceiver:
    MAX_IMAGES_IN_QUEUE = 5

    def __init__(self, ipaddr, port=9999):
        self.images_upper_cam = SimpleQueue()
        self.images_lower_cam = SimpleQueue()
        self.ipaddr = ipaddr
        self.port = port
        self.running = False

    def _discard_old_images(self, q: SimpleQueue[bytes]):
        while q.qsize() > self.MAX_IMAGES_IN_QUEUE:
            q.get()

    def _receive_image_thread(self):
        try:
            sock = socket.create_connection((self.ipaddr, self.port))
        except (ConnectionRefusedError, OSError):
            print(f'''
Error: could not connect to robot {self.ipaddr} port {self.port}.
Please check: Is the robot turned on and reachable via network and fw_sydney started?
Re-raising exception llllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllll...
''', file=sys.stderr)
            self.running = False
            raise

        fp = sock.makefile("rb")
        while self.running:
            try:
                length, is_lower_cam = struct.unpack(">ib", fp.read(5))
                buf = fp.read(length)
                if is_lower_cam != 0:
                    self.images_lower_cam.put(buf)
                    self._discard_old_images(self.images_lower_cam)
                else:
                    self.images_upper_cam.put(buf)
                    self._discard_old_images(self.images_upper_cam)
            except Exception as e:
                print(f"Error receiving image: {e}", file=sys.stderr)
                self.running = False

    def start_receiving(self):
        self.running = True
        threading.Thread(target=self._receive_image_thread, name="RobotJpegReceiver", daemon=True).start()

    def stop_receiving(self):
        self.running = False


class RobotImageDisplay:
    def __init__(self, receiver, window_size=(1000, 600)):
        self.receiver = receiver
        self.window_size = window_size
        self.running = False

    def run(self):
        self.running = True
        self.receiver.start_receiving()

        # Initialize pygame
        pygame.init()
        screen = pygame.display.set_mode(self.window_size)
        pygame.display.set_caption("Upper and Lower Camera")
        clock = pygame.time.Clock()

        while self.running:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    self.running = False

            if not self.receiver.images_upper_cam.empty():
                image_data = self.receiver.images_upper_cam.get()
                image_upper = pygame.image.load(io.BytesIO(image_data))
                screen.blit(image_upper, (0, 0))

            if not self.receiver.images_lower_cam.empty():
                image_data = self.receiver.images_lower_cam.get()
                image_lower = pygame.image.load(io.BytesIO(image_data))
                screen.blit(image_lower, (0, self.window_size[1] // 2))

            pygame.display.update()
            clock.tick(30)

        pygame.quit()
        self.receiver.stop_receiving()


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python3 robot_image_display.py <robot_ip>")
        sys.exit(1)

    robot_ip = sys.argv[1]
    receiver = RobotImageReceiver(robot_ip)
    display = RobotImageDisplay(receiver)
    display.run()