#!/usr/bin/env python3

from io import BytesIO
import sys
import pygame
from PIL import Image
from src.naobackend.robot_jpeg_image_stream import RobotJpegImageStream

# Initialize Pygame
pygame.init()

img_upper = None
img_lower = None


def main() -> None:
    if len(sys.argv) > 1:
        robot_addr = sys.argv[1]
    else:
        robot_addr = "127.0.0.1"

    global img_upper
    global img_lower

    # Connect to the robot
    stream = RobotJpegImageStream(robot_addr)


    # Set up the Pygame window (resizable)
    screen = pygame.display.set_mode((640, 960), pygame.RESIZABLE)
    pygame.display.set_caption('Robot Camera Stream')

    clock = pygame.time.Clock()
    running = True

    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False

        # Fetch images from the robot
        try:
            img_upper = Image.open(BytesIO(stream.images_upper_cam.get_nowait()))
        except:
            pass

        try:
            img_lower = Image.open(BytesIO(stream.images_lower_cam.get_nowait()))
        except:
            pass

        if img_upper is None and img_lower is None:
            continue

        # Create a new image to stitch both images vertically
        stitched_image = Image.new('RGB', (img_upper.width, img_upper.height + img_lower.height))
        stitched_image.paste(img_upper, (0, 0))
        stitched_image.paste(img_lower, (0, img_upper.height))

        # Convert the stitched image to Pygame format
        mode = stitched_image.mode
        size = stitched_image.size
        data = stitched_image.tobytes()
        pygame_image = pygame.image.fromstring(data, size, mode)

        # Get the current window size
        window_size = pygame.display.get_surface().get_size()
        window_width, window_height = window_size

        # Calculate the new size while maintaining the aspect ratio
        img_width, img_height = stitched_image.size
        aspect_ratio = img_width / img_height

        if window_width / window_height > aspect_ratio:
            new_width = window_height * aspect_ratio
            new_height = window_height
        else:
            new_width = window_width
            new_height = window_width / aspect_ratio

        # Resize the image while maintaining aspect ratio
        resized_image = pygame.transform.scale(pygame_image, (int(new_width), int(new_height)))

        # Clear the screen and draw the new image
        screen.fill((0, 0, 0))
        screen.blit(resized_image, (0, 0))
        pygame.display.flip()

        # Cap the frame rate
        clock.tick(30)  # Adjust frame rate as needed

    pygame.quit()


if __name__ == "__main__":
    main()
