# BluetoothAudioTransmitter_KCX_BT_EMITTER
# This is the master branch; code is original KCX_BT_EMITTER_V1.7
Utilities to help program the KCX_BT_EMITTER module. Collected here to make them easier to find.

## Go to the branch for the firmware version of KCX_BT_EMITTER: KCX_BT_EMITTER_V1.1 or KCX_BT_EMITTER_V1.7
This code was originally done for KCX_BT_EMITTER firmware V1.1 (found on modules stamped V1.1 and on some modules stamped V1.2).

There is now a version V1.7 of the KCX_BT_EMITTER and Holger Pandel made some changes to ProgrammingArduino.ino in his fork in branch v1.7 to accomodate the new AT commands. This can be found here
- https://github.com/pandel/BluetoothAudioTransmitter_KCX_BT_EMITTER/tree/v1.7

Holger kindly did a pull request to alert me to this update, and I will merge that code in.

There are now two branches to this repo:
- https://github.com/Mark-MDO47/BluetoothAudioTransmitter_KCX_BT_EMITTER/tree/KCX_BT_EMITTER_V1.1 - works with firmware V1.1 and based on everything from before the pull request
- https://github.com/Mark-MDO47/BluetoothAudioTransmitter_KCX_BT_EMITTER/tree/KCX_BT_EMITTER_V1.7 - works with firmware V1.7, based on Holger's code, and will be under development

My V1.7 parts are expected to arrive in the middle of July 2023 so in the meantime I won't be able to experiment much with the KCX_BT_EMITTER_V1.7 branch; please bear with me. 

## KCX_putty

This tool is designed to help explore the commands of the new V1.7 KCX_BT_EMITTER.

The purpose of KCX_putty.ino is to be loaded in an Arduino connected
the same as the "Programming Arduino" and act like a "dumb" terminal
with echo on communicating between the USB "Serial Monitor" of the
Arduino IDE and the KCX_BT_EMITTER.

This is somewhat like the Linux/Unix utility "putty", thus the name KCX_putty

This is derived from the Rubber Band Gun routine BlueToothTesting.ino
- https://github.com/Mark-MDO47/RubberBandGun/tree/master/RBG_arduino/BlueToothTesting

## Description
Note: These utilities were originally developed by me for the Rubber Band Gun (RBG) project https://github.com/Mark-MDO47/RubberBandGun

![alt text](https://github.com/Mark-MDO47/BluetoothAudioTransmitter_KCX_BT_EMITTER/blob/master/images/KCX_BT_Board_IMG_1351.png "Front side of KCX_BT_EMITTER")

![alt text](https://github.com/Mark-MDO47/BluetoothAudioTransmitter_KCX_BT_EMITTER/blob/master/images/KCX_BT_board_back_IMG_1357.png "Back side of KCX_BT_EMITTER")

![alt text](https://github.com/Mark-MDO47/BluetoothAudioTransmitter_KCX_BT_EMITTER/blob/master/images/KCX-BT-EMITTER_PinFunction.png "Pin Function of KCX_BT_EMITTER")

The KCX_BT_EMITTER Bluetooth Audio Transmitter Module receives line-level (not speaker-level) stereo audio in and transmits to a Bluetooth receiver (speaker, headphones, etc.). Be sure to connect the audio (analog) ground and do not connect audio ground to digital ground. Also due to latency in packetizing/depacketizing the Bluetooth audio, it is best to turn off any parallel wired speaker if using the Bluetooth audio at the same time.

This chip works great! It is not the very latest Bluetooth spec so it doesn't work with every single Bluetooth speaker, but it works with many and then it sounds great. One downside is that it is so small that the soldering is a challenge for me.

This Bluetooth Module uses an "AT" command set to program it to connect to your speaker and not others. Included in this repository is Arduino code (ProgrammingArduino.ino) that is used to program the KCX_BT_EMITTER. This can be used to store information about your Bluetooth speakers and/or headsets that will stay in the KCX_BT_EMITTER even after power off/on; the KCX_BT_EMITTER will scan for the matching speakers/headphones and connect only to items in that list.

Note that the KCX_BT_EMITTER expects 5V inputs for the serial port. I have heard of problems when trying to use an ESP32 as the programming Arduino. The code itself should work on an ESP32, but the ESP32 boards I know of have 3.5 V outputs. As the first document below says: "The Programming Arduino should be a type of Arduino that uses 5 Volt interfaces. For example, an Arduino Uno or an Arduino Nano Classic."

That is why I used a cheap Arduino Uno or Arduino Nano clone, which seems to me to be the simplest. Alternatively (although I have not tried this), you could potentially use the ESP32 by passing the serial transmit to KCX_BT_EMITTER signal through a 3.5 V to 5 V buffer driver and vice-versa for the return signal. If I was doing this alternative method, I would be sure also to provide 5 V power to the KCX_BT_EMITTER.

The following document describes using this capability:
- https://github.com/Mark-MDO47/BluetoothAudioTransmitter_KCX_BT_EMITTER/blob/master/ProgrammingArduino_SerialMonitor_SampleOutput.pdf

Information on the Bluetooth audio transmitter module is found here (along with ordering info)
- https://www.aliexpress.com/item/33058710334.html

Best information on the Bluetooth module I could find is here; need to translate from Chinese (see next link)
- https://item.taobao.com/item.htm?spm=a21wu.12321156-tw.0.0.7e76d1c7xEOcFZ&id=570274835710

Some more usage info on the Bluetooth module, especially English information on controlling with serial AT commands
- https://www.electro-tech-online.com/threads/kcx_bt_emitter-low-cost-bluetooth-bt-audio-module.158156/

Look in the comments in ProgrammingArduino.ino to see my interpretation of the AT commands
- https://github.com/Mark-MDO47/BluetoothAudioTransmitter_KCX_BT_EMITTER/blob/master/ProgrammingArduino/ProgrammingArduino.ino
