import time

import pygame
import pygame_gui

from src.pingmanger.PingManger import PingManager
from src.robotjpegimageStream.robot_jpeg_image_stream import RobotImageDisplay


class App:
    def __init__(self, ips):
        pygame.init()
        self.SCREEN_WIDTH = 1000
        self.SCREEN_HEIGHT = 600

        # Constants for colors
        self.WHITE = (255, 255, 255)
        self.BLACK = (0, 0, 0)

        self.ip_addresses = ips
        self.screen = pygame.display.set_mode((self.SCREEN_WIDTH, self.SCREEN_HEIGHT))
        self.manager = pygame_gui.UIManager((self.SCREEN_WIDTH, self.SCREEN_HEIGHT))
        self.screen.fill(self.WHITE)
        self.font = pygame.font.Font(None, 24)

        self.draw_text("Pinging IP Addresses...", self.SCREEN_WIDTH // 2, self.SCREEN_HEIGHT // 2 - 20)
        pygame.display.flip()
        pygame.time.wait(2000)

        self.buttons = []
        self.ip_button_map = {}

        self.ping_manager = PingManager(self.ip_addresses)
        self.create_buttons()

        print("SET KAMERA STREAM")
        time.sleep(2)
        #self.robot_image_display = RobotImageDisplay()
        #self.robot_image_display.screen = self.screen  # Set the screen for RobotImageDisplay

    def draw_text(self, text, x, y):
        self.font = pygame.font.Font(None, 54)
        text_obj = self.font.render(text, True, self.BLACK)
        text_rect = text_obj.get_rect(center=(x, y))
        self.screen.blit(text_obj, text_rect)

    def create_buttons(self):
        reachable_ips = self.ping_manager.get_reachable_ips()

        for ip in reachable_ips:
            button = pygame_gui.elements.UIButton(
                relative_rect=pygame.Rect((self.SCREEN_WIDTH // 2 - 100, len(self.buttons) * 40 + 100, 200, 30)),
                text=ip,
                manager=self.manager
            )
            self.buttons.append(button)
            self.ip_button_map[ip] = button

    def run(self):
        clock = pygame.time.Clock()
        is_running = True
        clicked_ip = None

        while is_running:
            time_delta = clock.tick(60) / 1000.0

            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    is_running = False
                elif event.type == pygame.USEREVENT:
                    if event.ui_element in self.ip_button_map.values() and event.user_type == pygame_gui.UI_BUTTON_PRESSED:
                        for ip, button in self.ip_button_map.items():
                            if event.ui_element == button:
                                clicked_ip = ip
                                print(clicked_ip, "clicked")

                                self.manager.clear_and_reset()
                                print("[WARNING]: before Break in app.py")

                                is_running = False
                                break  # Exit the inner loop

                self.manager.process_events(event)

            self.manager.update(time_delta)
            self.screen.fill((255, 255, 255))

            self.manager.draw_ui(self.screen)

            pygame.display.flip()

        print("[INFO]: return clicked_ip")
        return clicked_ip