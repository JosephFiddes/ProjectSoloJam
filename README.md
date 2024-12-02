# ProjectSoloJam
 Files and info for my solo jam project.

## Setup
### Dependencies
Currently, the solution is only designed for Windows.

The following dependencies are required:
 - [oscpp](https://github.com/kaoskorobase/oscpp): library for constructing and parsing OpenSoundControl packets.
 - windows.h

### Compilation

#### Serial2OSC
This project makes use of the winsock2.h library. With MinGW it is compiled as follows:

`g++ -o serial2osc.exe serial2osc.cpp connection.cpp -lws2_32`

#### Controller
The code for the controller is run on an Arduino UNO, and can be compiled in the Arduino IDE.

### Required REAPER Settings
REAPER needs to have OSC control enabled. 

1. Go to Preferences -> Control/OSC/Web. 
2. Add "OSC (Open Sound Control)" Control surface, with mode "Local port"
3. Set Local listen port to `2345`.

## Operation

With REAPER open, run serial2osc.exe. Track 2 will be armed for recording, and track 3 will not. The other tracks will not be affected.

## TODO
 - Set up device which captures physical input (so tracks can be armed using buttons rather than mouse).
 - Read serial input, and convert to OSC.
 - Have REAPER react to the OSC.
 - In OSCconnection.h, find a way to force "buffer" to be aligned to 32 bits.