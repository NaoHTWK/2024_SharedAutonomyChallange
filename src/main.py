from app.app import *
from src.joystickhandler.joystick_handler import *
from src.walkcontrol.walk_control import *
import src.naoshow.Gui as GUI

def main(joystick):
    #app = App(['10.0.13.17'])
    #clicked_ip = app.run()
    #robot_addr = clicked_ip
    isrunning = True

    #robot_display = RobotImageDisplay()
    #robot_display.run(robot_addr)
    print("[WARNING]: Before Walk_control")

    ip_addr = "192.168.13.6"  # roboter abhängig
    walk_control = WalkControl(ip_addr)

    gui = GUI.Gui()
    gui.run("127.0.0.1")

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
