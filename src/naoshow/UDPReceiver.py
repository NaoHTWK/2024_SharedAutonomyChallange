import socket
import time
import threading
import src.naoshow.SPLStandartMessage as SPL
class UDPReceiver(threading.Thread):

    def __init__(self, running, fps):

        threading.Thread.__init__(self)
        self.running = running
        self.infocast_port = 20013

        self.fps = fps
        self.robots = {}
        
    def run(self):
        return
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        sock.bind(("", self.infocast_port))

        while self.running:
            data, addr = sock.recvfrom(1024)
            robot_id = addr[0].split(".")[3]
            msg = SPL.SPLStandardMessage(data)
            if msg.position != {'x': 0, 'y': 0, 'a': 0}:
                self.robots[robot_id] = msg
            time.sleep(self.fps)
