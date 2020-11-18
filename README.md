# sonde-decoder

QT application to decode and display M10 messages.

## How to build

This project uses Qt 5, if you want to build from source, download Qt creator at https://www.qt.io/.

### Windows

Windows 64bit users : you can use the provided binary (check page _releases_). No installation required !

If you want to build from sources, download Qt creator from the Qt website, open SondeDecoder.pro and click build !

### Linux

Tested on ubuntu 18.04.

Download Qt creator from Qt website (do not used the version shipped with ubuntu, it is too old). Open SondeDecoder.pro and build project.
At the moment you also have to manually copy all resources files/folders (Map.qml, helpers.js, resources, menus, map and forms) to the build folder.

## How to use

- Compile or use released binary.
- Use any SDR and demodulate in FM with 32kHz or more bandwidth.
- Redirect audio to this program (using virtual cable for instance).

## Code quality

- Every rule of the book has been broken to write this program as fast as possible. Do not take any of it as good practice example.


![GUI](resources/GUI.png)

