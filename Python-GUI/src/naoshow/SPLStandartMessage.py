import struct

SPL_STANDARD_MESSAGE_STRUCT_HEADER = "SPL "
SPL_STANDARD_MESSAGE_STRUCT_VERSION = 7
SPL_STANDARD_MESSAGE_DATA_SIZE = 474


class SPLStandardMessage:
    def __init__(self, data):
        self.unpack(data)

    def unpack(self, data):
        # Convert the data to a list of bytes
        bytes_data = list(data)

        # Extract header
        self.header = data[:4].decode('ascii')

        # Extract version, playerNumber, teamNumber, and isFallen
        self.version = bytes_data[4]
        self.playerNumber = bytes_data[5]
        self.teamNumber = bytes_data[6]
        self.isFallen = bytes_data[7] == 1

        # Extract position
        self.position = {
            'x': struct.unpack('<f', bytes(data[8:12]))[0],
            'y': struct.unpack('<f', bytes(data[12:16]))[0],
            'a': struct.unpack('<f', bytes(data[16:20]))[0]
        }

        # Extract ballAge
        self.ballAge = struct.unpack('<f', bytes(data[20:24]))[0]

        # Extract ball position
        self.ball = {
            'x': struct.unpack('<f', bytes(data[24:28]))[0],
            'y': struct.unpack('<f', bytes(data[28:32]))[0]
        }

        # Extract data
        self.data = bytes(data[34:])

    def __str__(self):
        return f"SPLStandardMessage(header={self.header}, version={self.version}, playerNumber={self.playerNumber}, " \
               f"teamNumber={self.teamNumber}, isFallen={self.isFallen}, position={self.position}, " \
               f"ballAge={self.ballAge}, ball={self.ball}, data={self.data})"

#
