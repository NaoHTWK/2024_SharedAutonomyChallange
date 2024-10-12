#!/usr/bin/env python3

from io import BytesIO
import sys

import matplotlib.pyplot as plt
from PIL import Image
from matplotlib.animation import FuncAnimation
from naobackend.robot_jpeg_image_stream import RobotJpegImageStream

# I have to say it: I created this code with Chat GPT... awesome


img_upper = None
img_lower = None


def main() -> None:
    if len(sys.argv) > 1:
        robot_addr = sys.argv[1]
    else:
        robot_addr = "10.0.13.6"

    # Connect to the robot
    stream = RobotJpegImageStream(robot_addr)

    def update_image(frame):
        global img_upper
        global img_lower
        try:
            img_upper = Image.open(BytesIO(stream.images_upper_cam.get_nowait()))
        except:
            pass
        try:
            img_lower = Image.open(BytesIO(stream.images_lower_cam.get_nowait()))
        except:
            pass

        if img_upper is None or img_lower is None:
            return
        stitched_image = Image.new('RGB', (img_upper.width, img_upper.height + img_lower.height))
        stitched_image.paste(img_upper, (0, 0))
        stitched_image.paste(img_lower, (0, img_upper.height))

        plt.clf()
        plt.imshow(stitched_image)
        plt.axis('off')

    # Create a figure and axis
    fig, ax = plt.subplots()
    ax.autoscale(False)
    plt.rcParams['toolbar'] = 'None'

    # Set the initial image as blank
    blank_image = Image.new('RGB', (1, 1))
    ax.imshow(blank_image)
    ax.axis('off')

    # Update the image continuously using FuncAnimation
    animation = FuncAnimation(fig, update_image, frames=None, interval=33)  # Adjust the interval as needed

    # Show the plot
    plt.show()


if __name__ == "__main__":
    main()
