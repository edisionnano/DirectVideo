# Direct Video
## What is Direct Video?
Direct Video is a custom [MediaStream](https://developer.mozilla.org/en-US/docs/Web/API/MediaStream) type introduced by Discord on their open source Electron fork. [Here](https://github.com/discord/electron/commit/e1098e5c9c7bf6282ea29013bb95b02e0d4a7c01) is the commit which applies the patch on their, currently, latest Electron version. It basically adds a way of presenting frames on the Electron renderer directly instead of sending them through JavaScript callbacks which perform worse.
## Getting Frames
### As a client
The file stub.cpp takes the role of Electron, it contains the function DeliverDiscordFrame needed to receive a frame from a native module that can post them. After you clone the reposity compile the code using:
```console
g++ -fPIC -c -o stub.o stub.cpp
gcc -shared -o stub.so stub.o
```
This will create the shared object stub.so which you can then preload to capture a frame.<br>
In order to capture the frame you will also need a native module that can post such frames, Discord's Media Engine (AKA Voice Engine). The location of this module is `~/.config/discord(ptb,canary,development)/0.0.(version_number_here)/modules/discord_voice/discord_voice.node`. Simply copy the file to your working directory (next to stub.so) and then run node using:
```console
LD_PRELOAD="$PWD/stub.so" node
```
Then import the Media Engine module as voice and initialize using:
```JavaScript
const voice = require('./discord_voice.node');
voice.initialize({});
```
List Video Input devices(webcams) using:
```Javascript
voice.getVideoInputDevices((devices) => console.log(devices))
```
Copy your webcam's guid, mine is `usb-0000:03:00.0-8`, and use the command bellow to enable the webcam replacing the guid with yours (even if you don't see `>` you can still type):
```Javascript
voice.addDirectVideoOutputSink("usb-0000:03:00.0-8");
```
Let it run for 2-3 seconds and type `.exit` to stop it.
