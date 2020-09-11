# BluetoothAudioTransmitter_KCX_BT_EMITTER
Utilities to help program the KCX_BT_EMITTER module. Collected here to make them easier to find.

Note: These utilities were developed by me for the Rubber Band Gun (RBG) project https://github.com/Mark-MDO47/RubberBandGun

The KCX_BT_EMITTER Bluetooth Audio Transmitter Module receives line-level (not speaker-level) stereo audio in and transmits to a Bluetooth receiver (speaker, headphones, etc.). Be sure to connect the analog ground and do not connect analog ground to digital ground. Also due to latency in packetizing/depacketizing the Bluetooth audio, it is best to turn off any parallel wired speaker if using the Bluetooth audio.

The Bluetooth Module uses an "AT" command set to program it to connect to your speaker and not others. Included in this repository is an Arduino sketch for a 2nd Arduino that is used to program the KCX_BT_EMITTER in-place. The following document describes using this capability:
# TBS

Information on the Bluetooth audio transmitter module is found here (along with ordering info)
- https://www.aliexpress.com/item/33058710334.html

Best information on the Bluetooth module I could find is here; need to translate from Chinese (see next link)
- https://item.taobao.com/item.htm?spm=a21wu.12321156-tw.0.0.7e76d1c7xEOcFZ&id=570274835710

Some more usage info on the Bluetooth module, especially English information on controlling with serial AT commands
- https://www.electro-tech-online.com/threads/kcx_bt_emitter-low-cost-bluetooth-bt-audio-module.158156/
