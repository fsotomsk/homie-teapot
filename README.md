IoT Teapot based on Homie
=================

[![Latest Release](https://img.shields.io/badge/release-v2.0.0-yellow.svg?style=flat-square)](https://github.com/marvinroger/homie-esp8266/releases)

An Arduino for ESP8266 implementation of [Homie](https://github.com/marvinroger/homie), an MQTT convention for the IoT.

Currently Homie for ESP8266 implements [Homie 2.0.1](https://github.com/marvinroger/homie/releases/tag/v2.0.1)

## Using with PlatformIO

[PlatformIO](http://platformio.org) is an open source ecosystem for IoT development with cross platform build system, library manager and full support for Espressif ESP8266 development. It works on the popular host OS: Mac OS X, Windows, Linux 32/64, Linux ARM (like Raspberry Pi, BeagleBone, CubieBoard).

1. Install [PlatformIO IDE](http://platformio.org/platformio-ide)
2. Create new project using "PlatformIO Home > New Project"
3. Open [Project Configuration File `platformio.ini`](http://docs.platformio.org/page/projectconf.html)

### Stable version

4. Add "Homie" to project using `platformio.ini` and [lib_deps](http://docs.platformio.org/page/projectconf/section_env_library.html#lib-deps) option:
```ini
[env:myboard]
platform = espressif8266
board = ...
framework = arduino
lib_deps = Homie
```

### Development version

4. Update dev/platform to staging version:
   - [Instruction for Espressif 8266](http://docs.platformio.org/en/latest/platforms/espressif8266.html#using-arduino-framework-with-staging-version)
5. Add development version of "Homie" to project using `platformio.ini` and [lib_deps](http://docs.platformio.org/page/projectconf/section_env_library.html#lib-deps) option:
```ini
[env:myboard]
platform = ...
board = ...
framework = arduino

; the latest development branch
lib_deps = https://github.com/marvinroger/homie-esp8266.git

; or tagged version
lib_deps = https://github.com/marvinroger/homie-esp8266.git#v2.0.0-beta.2
```
-----
Happy coding with PlatformIO!

## Features

* Automatic connection/reconnection to Wi-Fi/MQTT
* [JSON configuration file](http://marvinroger.github.io/homie-esp8266/docs/develop/configuration/json-configuration-file) to configure the device
* [Cute HTTP API / Web UI / App](http://marvinroger.github.io/homie-esp8266/docs/develop/configuration/http-json-api) to remotely send the configuration to the device and get information about it
* [Custom settings](http://marvinroger.github.io/homie-esp8266/docs/develop/advanced-usage/custom-settings)
* [OTA over MQTT](http://marvinroger.github.io/homie-esp8266/docs/develop/others/ota-configuration-updates)
* [Magic bytes](http://marvinroger.github.io/homie-esp8266/docs/develop/advanced-usage/magic-bytes)
* Available in the [PlatformIO registry](http://platformio.org/#!/lib/show/555/Homie)
* Pretty straightforward sketches, a simple light for example:


## Requirements, installation and usage

The project is documented on http://marvinroger.github.io/homie-esp8266/ with a *Getting started* guide and every piece of information you will need.

## Donate for Marvin

I am a student and maintaining Homie for ESP8266 takes time. **I am not in need and I will continue to maintain this project as much as I can even without donations**. Consider this as a way to tip the project if you like it. :wink:

[![Donate button](https://www.paypal.com/en_US/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=JSGTYJPMNRC74)
