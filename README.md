# BluetoothAudioTransmitter_KCX_BT_EMITTER
# This is the KCX_BT_EMITTER_V1.7 Branch
Utilities to help program the KCX_BT_EMITTER module. Collected here to make them easier to find.

## Go to the branch for the firmware version of KCX_BT_EMITTER: KCX_BT_EMITTER_V1.1 or KCX_BT_EMITTER_V1.7
This code was originally done for KCX_BT_EMITTER firmware V1.1 (found on modules stamped V1.1 and on some modules stamped V1.2).

There is now a version V1.7 of the KCX_BT_EMITTER and Holger Pandel made some changes to ProgrammingArduino.ino in his fork in branch v1.7 to accomodate the new AT commands. This can be found here
- https://github.com/pandel/BluetoothAudioTransmitter_KCX_BT_EMITTER/tree/v1.7

Holger kindly did a pull request to alert me to this update, that code is now merged in to this V1.7 branch.

There are now two branches to this repo:
- https://github.com/Mark-MDO47/BluetoothAudioTransmitter_KCX_BT_EMITTER/tree/KCX_BT_EMITTER_V1.1 - works with firmware V1.1 and based on everything from before the pull request
- https://github.com/Mark-MDO47/BluetoothAudioTransmitter_KCX_BT_EMITTER/tree/KCX_BT_EMITTER_V1.7 - works with firmware V1.7, based on Holger's code, and will be under development

I have done some development after the original pull request and this branch reflects that.

### Differences from V1.1 to V1.7

Looking over the spec, there are a lot of new capabilities in V1.7 but two changes that make the V1.1 version of ProgrammingArduino.ino not work with V1.7 hardware
- The default baud rate changed from 9600 to 115200
- The "reset" command spelling changed from "AT+REST" to "AT+RESET"

My V1.7 parts arrived and I have started experimenting.
- I confirmed Holger's finding that the "AT+SCAN" command in the documentation does not work.
- It was quite difficult to make the code reliably echo the V1.7 KCX_BT_EMITTER at its default baud rate of 115200. I made the startup of KCX_putty.ino and ProgrammingArduino.ino send a command at 115200 baud to switch to 9600 baud and then the code also switches its KCX communication to 9600 baud while maintining USB Serial Monitor communication at 115200. This worked more reliably.
- **WARNING** In an experiment I tried programming the VMLINK area with a byte-reversed MAC address (0xf44efdecd39d). Afterwards the KCX_BT_EMITTER continued to report SCAN results but does not seem to respond to any commands. I am not sure if this is because of the strange MAC address or if I burned out the chip somehow; just be warned.

The latest versions of the KCX_BT_EMITTER_V1.7 code can perform the same functions as the previous V1.1 code. I am working on updating the documentation.

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
Note: The V1.1 utilities were originally developed by me for the Rubber Band Gun (RBG) project https://github.com/Mark-MDO47/RubberBandGun
There is a start on V1.7 utilities. The following have been updated to some extent:
- ProgrammingArduino.ino - just enough changes to make it work with V1.7. It doesn't yet access all the V1.7 firmware capabilities.

It took me a while to find sources to purchase the V1.7 version. Here are two sources I found.
- https://store.siqma.com/kcx-bt-emitter.html
- https://www.aliexpress.us/item/3256805363984973.html

The KCX_BT_EMITTER V1.7 looks like this

<img src="https://github.com/Mark-MDO47/BluetoothAudioTransmitter_KCX_BT_EMITTER/blob/V1.7_001/images/KCX_BT_EMITTER_1.7_front.png" width="300" alt="aliexpress.com picture of KCX BT EMITTER version 1.7 Front Side"> <img src="https://github.com/Mark-MDO47/BluetoothAudioTransmitter_KCX_BT_EMITTER/blob/V1.7_001/images/KCX_BT_EMITTER_1.7_back.png" width="300" alt="aliexpress.com picture of KCX BT EMITTER version 1.7 Back Side">

The wires I attached for the V1.7 KCX_BT_EMITTER are as follows (upper case from one box of wires; lower case from the other)

<table style="width: 100%; border: none;" cellspacing="0" cellpadding="0" border="0">
  <tr>
    <td><b>Pin</b></td>
    <td>DM</td>
    <td>DP</td>
    <td>AGND</td>
    <td>AUDIO_R</td>
    <td>AUDIO_L</td>
    <td>TX</td>
    <td>RX</td>
    <td>LINK</td>
    <td>PGND</td>
    <td>+5V</td>
  </tr>
  <tr>
    <td><b>Color</b></td>
    <td>pink</td>
    <td>purple</td>
    <td>orange</td>
    <td>WHITE</td>
    <td>BLUE</td>
    <td>yellow</td>
    <td>GREEN</td>
    <td>brown</td>
    <td>BLACK</td>
    <td>RED</td>
  </tr>
</table>

## The text below is currently for firmware version V1.1; working now on updates for V1.7.

The KCX_BT_EMITTER Bluetooth Audio Transmitter Module receives line-level (not speaker-level) stereo audio in and transmits to a Bluetooth receiver (speaker, headphones, etc.). Be sure to connect the audio (analog) ground and do not connect audio ground to digital ground. Also due to latency in packetizing/depacketizing the Bluetooth audio, it is best to turn off any parallel wired speaker if using the Bluetooth audio at the same time.

This chip works great! It is not the very latest Bluetooth spec so it doesn't work with every single Bluetooth speaker, but it works with many and then it sounds great. One downside is that it is so small that the soldering is a challenge for me.

This Bluetooth Module uses an "AT" command set to program it to connect to your speaker and not others. Included in this repository is Arduino code (ProgrammingArduino.ino) that is used to program the KCX_BT_EMITTER. This can be used to store information about your Bluetooth speakers and/or headsets that will stay in the KCX_BT_EMITTER even after power off/on; the KCX_BT_EMITTER will scan for the matching speakers/headphones and connect only to items in that list.

Note that the KCX_BT_EMITTER expects 5V inputs for the serial port. I have heard of problems when trying to use an ESP32 as the programming Arduino. The code itself should work on an ESP32, but the ESP32 boards I know of have 3.5 V outputs. As the first document below says: "The Programming Arduino should be a type of Arduino that uses 5 Volt interfaces. For example, an Arduino Uno or an Arduino Nano Classic."

That is why I used a cheap Arduino Uno or Arduino Nano clone, which seems to me to be the simplest. Alternatively (although I have not tried this), you could potentially use the ESP32 by passing the serial transmit to KCX_BT_EMITTER signal through a 3.5 V to 5 V buffer driver and vice-versa for the return signal. If I was doing this alternative method, I would be sure also to provide 5 V power to the KCX_BT_EMITTER.

## How to use ProgrammingArduino
The following document describes using this capability:
- https://github.com/Mark-MDO47/BluetoothAudioTransmitter_KCX_BT_EMITTER/blob/KCX_BT_EMITTER_V1.7/ProgrammingArduino_SerialMonitor_SampleOutput_V1.7.pdf
- **WARNING** In an experiment I tried programming the VMLINK area with a byte-reversed MAC address (0xf44efdecd39d). Afterwards the KCX_BT_EMITTER continued to report SCAN results but does not seem to respond to any commands. I am not sure if this is because of the strange MAC address or if I burned out the chip somehow; just be warned.

Best information on the V1.7 KCX_BT_EMITTER Bluetooth module I have is here (thanks Holger!)
- https://github.com/Mark-MDO47/BluetoothAudioTransmitter_KCX_BT_EMITTER/blob/KCX_BT_EMITTER_V1.7/specs/BT%20GY19733-CN.zh-CN.en.pdf

Look in the comments in the V1.7 ProgrammingArduino.ino to see my interpretation of the AT commands
- https://github.com/Mark-MDO47/BluetoothAudioTransmitter_KCX_BT_EMITTER/blob/master/ProgrammingArduino/ProgrammingArduino.ino
