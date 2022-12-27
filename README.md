# Direct Video
## What is Direct Video?
Direct Video is a custom [MediaStream](https://developer.mozilla.org/en-US/docs/Web/API/MediaStream) type introduced by Discord on their open source Electron fork. [Here](https://github.com/discord/electron/commit/e1098e5c9c7bf6282ea29013bb95b02e0d4a7c01) is the commit which applies the patch on their, currently, latest Electron version. It basically adds a way of presenting frames on the Electron renderer directly instead of sending them through JavaScript callbacks which perform worse.
