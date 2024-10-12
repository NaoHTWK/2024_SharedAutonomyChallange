import pygame
import math
import sys
from PIL import Image
import threading

import src.naoshow.SPLStandartMessage as SPL
import src.naoshow.UDPReceiver as UDP
import src.naoshow.CameraStream as CS

class Gui(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)
        # Field dimensions and margins
        self.field_width = 900
        self.field_height = 600
        self.margin = 70

        self.img_width = 640
        self.img_height = 960

    # Draw the field with robots and the ball
    def draw_field(self, window, width, height, robots):
        color_green = (0, 255, 0)
        color_blue = (0, 0, 255)
        color_orange = (255, 165, 0)
        color_black = (0, 0, 0)
        color_white = (255, 255, 255)

        # Calculate total dimensions including margins
        total_width = self.field_width + 2 * self.margin
        total_height = self.field_height + 2 * self.margin

        # Calculate scaling factors
        scale = min((width - self.img_width) / total_width, height / total_height)
        scaled_field_width = self.field_width * scale
        scaled_field_height = self.field_height * scale
        scaled_margin = self.margin * scale
        margin_x = self.img_width + (width - self.img_width - scaled_field_width - 2 * scaled_margin) // 2
        margin_y = (height - scaled_field_height - 2 * scaled_margin) // 2

        # Draw the field
        window.fill(color_green)
        pygame.draw.rect(window, color_white, [margin_x + scaled_margin, margin_y + scaled_margin, scaled_field_width, scaled_field_height], 5)
        pygame.draw.line(window, color_white, [margin_x + scaled_margin + scaled_field_width / 2, margin_y + scaled_margin], [margin_x + scaled_margin + scaled_field_width / 2, margin_y + scaled_margin + scaled_field_height], 5)
        pygame.draw.line(window, color_white, [margin_x + scaled_margin + scaled_field_width / 2 - 5, margin_y + scaled_margin + scaled_field_height / 2], [margin_x + scaled_margin + scaled_field_width / 2 + 5, margin_y + scaled_margin + scaled_field_height / 2], 5)
        pygame.draw.circle(window, color_white, [int(margin_x + scaled_margin + scaled_field_width / 2), int(margin_y + scaled_margin + scaled_field_height / 2)], int(75 * scale), 5)
        pygame.draw.rect(window, color_white, [margin_x + scaled_margin, margin_y + scaled_margin + scaled_field_height / 6, 165 * scale, 400 * scale], 5)
        pygame.draw.rect(window, color_white, [margin_x + scaled_margin, margin_y + scaled_margin + scaled_field_height / 3, 60 * scale, 220 * scale], 5)
        pygame.draw.line(window, color_white, [margin_x + scaled_margin + 125 * scale, margin_y + scaled_margin + scaled_field_height / 2], [margin_x + scaled_margin + 135 * scale, margin_y + scaled_margin + scaled_field_height / 2], 5)
        pygame.draw.line(window, color_white, [margin_x + scaled_margin + 130 * scale, margin_y + scaled_margin + scaled_field_height / 2 - 5], [margin_x + scaled_margin + 130 * scale, margin_y + scaled_margin + scaled_field_height / 2 + 5], 5)
        pygame.draw.rect(window, color_white, [margin_x + scaled_margin + scaled_field_width - 165 * scale, margin_y + scaled_margin + scaled_field_height / 6, 165 * scale, 400 * scale], 5)
        pygame.draw.rect(window, color_white, [margin_x + scaled_margin + scaled_field_width - 60 * scale, margin_y + scaled_margin + scaled_field_height / 3, 60 * scale, 220 * scale], 5)
        pygame.draw.line(window, color_white, [margin_x + scaled_margin + scaled_field_width - 135 * scale, margin_y + scaled_margin + scaled_field_height / 2], [margin_x + scaled_margin + scaled_field_width - 125 * scale, margin_y + scaled_margin + scaled_field_height / 2], 5)
        pygame.draw.line(window, color_white, [margin_x + scaled_margin + scaled_field_width - 130 * scale, margin_y + scaled_margin + scaled_field_height / 2 - 5], [margin_x + scaled_margin + scaled_field_width - 130 * scale, margin_y + scaled_margin + scaled_field_height / 2 + 5], 5)

        # Draw Robots with direction and IDs
        for id, msg in robots.items():
            x = margin_x + scaled_margin + (msg.position['x'] / 10) * scale + scaled_field_width / 2
            y = margin_y + scaled_margin + scaled_field_height / 2 - (msg.position['y'] / 10) * scale

            angle = msg.position['a']
            length = 15 * scale
            end_x = x + length * math.cos(angle)
            end_y = y - length * math.sin(angle)

            pygame.draw.line(window, color_white, (x, y), (end_x, end_y), 5)

            pygame.draw.circle(window, color_blue, [int(x), int(y)], 10, 0)
            pygame.draw.circle(window, color_black, [int(x), int(y)], 10, 1)

            font = pygame.font.SysFont("Arial", 15)
            text = font.render(str(id), True, color_white)
            textRect = text.get_rect()
            textRect.center = (int(x), int(y))
            window.blit(text, textRect)

            # Draw the ball
            ball_x = x + (msg.ball['x'] / 10) * scale * math.cos(angle)
            ball_y = y + (msg.ball['y'] / 10) * scale * math.sin(angle)
            pygame.draw.circle(window, color_orange, [int(ball_x), int(ball_y)], 5, 0)
            pygame.draw.circle(window, color_black, [int(ball_x), int(ball_y)], 5, 1)

    def run(self, ip_addr):

        robots = {}
        running = True
        fps = 1 / 30

        camera_thread = CS.CameraStream(running, fps, ip_addr)
        camera_thread.start()

        udp_thread = UDP.UDPReceiver(running, fps)
        udp_thread.start()

        pygame.init()
        pygame.display.set_caption("NaoControl")
        window = pygame.display.set_mode((1040, 740), pygame.RESIZABLE)

        while running:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    running = False

            # Draw the field and robots
            width, height = pygame.display.get_window_size()
            self.img_width = 0
            resized_image = None
            
            if camera_thread.image:
                # Calculate the new size while maintaining the aspect ratio
                self.img_width, self.img_height = camera_thread.image.get_size()
                aspect_ratio = self.img_width / self.img_height

                if width / height > aspect_ratio:
                    new_width = height * aspect_ratio
                    new_height = height
                else:
                    new_width = width
                    new_height = width / aspect_ratio

                # Resize the image while maintaining aspect ratio
                resized_image = pygame.transform.scale(camera_thread.image, (int(new_width), int(new_height)))
                
            robots = udp_thread.robots

            self.draw_field(window, width, height, robots)
            if resized_image:
                window.blit(resized_image, (0, 0))


            pygame.display.flip()

            camera_thread.join()
            udp_thread.join()




        pygame.quit()
