import src.naobackend.robot_option_connection as roptions
from enum import Enum
import sys


class WalkModes(Enum):
    WALK_STAND = 0
    WALK_RELATIVE = 1
    WALK_ABSOLUTE = 2
    WALK_SIT = 3
    SHOOT_RIGHT = 4
    SHOOT_LEFT = 5

class WalkControl:
    def __init__(self, ip):
        # Connect to robot
        self.robot = roptions.RobotOptionConnection()
        self.robot.connect(ip)
        try:
            self.strategy = self.robot.firmware_options.option_sets["manual_strategy"]
            self.walk_tuner = self.robot.firmware_options.option_sets["walktuner"]
            print("")
            self.param_tuner = self.robot.bridge_options.option_sets["walkingengine"]
        except KeyError:
            print('''
Error: could not connect to walk tuner on robot {0}.
Please check: Is the walktuner agent started in fw_sydney?
Re-raising exception ...
'''.format(ip), file=sys.stderr)
            raise
        # self.param_tuner2 = self.robot.bridge_options.option_sets["anklebalancer"]
        print("ALLE ALLE")
    def set_velocity(self, vx, vy, va):
        if vx == 0 and vy == 0 and va == 0:
            self.walk_tuner.options["mode"].value = WalkModes.WALK_STAND.value
            # self.walk_tuner.options["timeout_ms"].value = 0
        else:
            self.walk_tuner.options["vx"].value = vx
            self.walk_tuner.options["vy"].value = vy
            self.walk_tuner.options["va"].value = va
            self.walk_tuner.options["mode"].value = WalkModes.WALK_RELATIVE.value
            # self.walk_tuner.options["timeout_ms"].value = 60000

