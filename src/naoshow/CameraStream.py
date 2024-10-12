import threading
from io import BytesIO
from PIL import Image
import pygame
import time

from src.naobackend.robot_jpeg_image_stream import RobotJpegImageStream

class CameraStream(threading.Thread):
    def __init__(self, running, fps, ip_addr):
        threading.Thread.__init__(self)
        self.running = running
        self.fps = fps
        self.stream = RobotJpegImageStream(ip_addr, port=20013)
        self.image = None
        self.img_upper = None
        self.img_lower = None

    def run(self):
        while self.running:
            # Fetch images from the robot
            try:
                self.img_upper = Image.open(BytesIO(self.stream.images_upper_cam.get_nowait()))
                print("upper")
            except:
                pass
            
            try:
                self.img_lower = Image.open(BytesIO(self.stream.images_lower_cam.get_nowait()))
                print("lower")
            except:
                pass

            if (self.img_upper is None) or (self.img_lower is None):
                continue
            
            # Create a new image to stitch both images vertically
            stitched_image = Image.new('RGB', (self.img_upper.width, self.img_upper.height + self.img_lower.height))
            stitched_image.paste(self.img_upper, (0, 0))
            stitched_image.paste(self.img_lower, (0, self.img_upper.height))

            # Convert the stitched image to Pygame format
            mode = stitched_image.mode
            size = stitched_image.size
            data = stitched_image.tobytes()
            self.image = pygame.image.fromstring(data, size, mode)
            
            time.sleep(self.fps)
