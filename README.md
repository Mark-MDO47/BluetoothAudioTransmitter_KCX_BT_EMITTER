# BluetoothAudioTransmitter_KCX_BT_EMITTER
Utilities to help program the KCX_BT_EMITTER module. Collected here to make them easier to find.

Note: These utilities were developed by me for the Rubber Band Gun (RBG) project https://github.com/Mark-MDO47/RubberBandGun

![alt text](https://github.com/Mark-MDO47/BluetoothAudioTransmitter_KCX_BT_EMITTER/blob/master/images/KCX_BT_Board_IMG_1351.png "Front side of KCX_BT_EMITTER")

![alt text](https://github.com/Mark-MDO47/BluetoothAudioTransmitter_KCX_BT_EMITTER/blob/master/images/KCX_BT_board_back_IMG_1357.png "Back side of KCX_BT_EMITTER")

![alt text](https://github.com/Mark-MDO47/BluetoothAudioTransmitter_KCX_BT_EMITTER/blob/master/images/KCX-BT-EMITTER_PinFunction.png "Pin Function of KCX_BT_EMITTER")

The KCX_BT_EMITTER Bluetooth Audio Transmitter Module receives line-level (not speaker-level) stereo audio in and transmits to a Bluetooth receiver (speaker, headphones, etc.). Be sure to connect the audio (analog) ground and do not connect audio ground to digital ground. Also due to latency in packetizing/depacketizing the Bluetooth audio, it is best to turn off any parallel wired speaker if using the Bluetooth audio at the same time.

The Bluetooth Module uses an "AT" command set to program it to connect to your speaker and not others. Included in this repository is Arduino code (ProgrammingArduino.ino) that is used to program the KCX_BT_EMITTER. This can be used to store information about your Bluetooth speakers and/or headsets that will stay in the KCX_BT_EMITTER even after power off/on; the KCX_BT_EMITTER will scan for the matching speakers/headphones and connect only to items in that list.

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
