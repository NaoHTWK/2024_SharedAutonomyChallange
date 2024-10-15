# HTWK Robots Shared Autonomy Challenge for RoboCup SPL 2024 World Championship

In this publication you will find the code for the Shared Autonomy Challenge (SAC) by team [HTWK Robots](https://htwk.bot/).

The SAC Code is divided into 2 Parts: the GUI running on the operators laptop and the firmware running on the Nao robot.

## Part 1: GUI

The GUI can be found in `Python-GUI`. It was tested to run under Linux and Windows and requires Python 3 to run. The Python dependencies are defined in `requirements.txt` and can be installed with `pip`.

The program can be run on Linux via `src/main.sh` or using the IDE PyCharm. The GUI connects to a Nao robot running the firmware. The IP address of the robot can be adjusted in `src/main.py`.

## Part 2: Firmware/Robot code

The firmware is located in `Cpp-Robot-Code`. It requires a C++ 17 compiler (GCC and clang were tested), [Protocol Buffers](https://protobuf.dev/) and CMake.

The relase includes all code related to the SAC and can be intergated into your robot code. If desired the [HTWK Vision](https://github.com/NaoHTWK/HTWKVision.git) and [HTWK Motion](https://github.com/NaoHTWK/HTWKMotion.git) releases can be used with the SAC code, or your own modules can be substituted.

Communication between GUI and firmware is defined with the provided `.proto` files. You can substitute your own communication if desired.

To build on the Linux commandline:
```bash
mkdir sacbuild
cd sacbuild
cmake .. 
cmake --build .
```
