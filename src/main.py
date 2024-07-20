from app.app import *
from joystickhandler.joystick_handler import *



def main(joystick):
    #app = App(['10.0.13.17'])
    #clicked_ip = app.run()
    #robot_addr = clicked_ip

    #robot_display = RobotImageDisplay()
    #robot_display.run(robot_addr)
    robot_addr = "10.0.13.6"

    print(f"[INFO]: Connecting to Robot... {robot_addr}")

    walk_control = WalkControl(robot_addr)
    print("[INFO]: After Walk_control; Before JoystickHandler")
    handler = JoystickHandler(walk_control)
    handler.get_joystick_values(joystick)

if __name__ == "__main__":
    print("Systems start...")

    pygame.init()
    # Versuchen, das Joystick-Modul zu initialisieren
    try:
        pygame.joystick.init()
        print("Joystick-Modul erfolgreich initialisiert.")
    except pygame.error as e:
        print(f"Fehler bei der Initialisierung des Joystick-Moduls: {e}")
        pygame.quit()
        exit()

    # Überprüfen, ob Joysticks angeschlossen sind
    joystick = None

    joystick_count = pygame.joystick.get_count()
    if joystick_count > 0:
        print(f"{joystick_count} Joystick(s) gefunden.")
        try:
            joystick = pygame.joystick.Joystick(0)
            joystick.init()
            print(f"Joystick '{joystick.get_name()}' erfolgreich initialisiert.")
        except pygame.error as e:
            print(f"Fehler bei der Erstellung des Joystick-Objekts: {e}")
        except Exception as e:
            print(f"Ein unerwarteter Fehler ist aufgetreten: {e}")
    else:
        print("Kein Joystick gefunden.")


    main(joystick)
