// Author: Mark Olson 2019-11-03
//
// originally written for KCX_BT_EMITTER firmware V1.1 for
//   Rubber Band Gun - https://github.com/Mark-MDO47/RubberBandGun
//   RBG - A high-tech imagining of the rubber band gun
//
// The purpose of KCX_putty.ino is to be loaded in an Arduino connected
//   the same as the "Programming Arduino" and act like a "dumb" terminal
//   with echo on communicating between the USB "Serial Monitor" of the
//   Arduino IDE and the KCX_BT_EMITTER.
// This is somewhat like the Linux/Unix utility "putty", thus the name KCX_putty
//
// At least for the V1.1 KCX_BT_EMITTER, the End-Of-Line (EOL) characters must
//   be removed before sending the command to the KCX_BT_EMITTER.
//


//////////////////////////////////////////////////////////////////////////////////////
// To use this tool:
//   With power off:
//     Find the female jumper connectors for the KCX_BT_EMITTER chip
//          label      color of wire
//          GND        BLACK
//          2 RX       GREEN
//          9 TX       YELLOW
//   On separate programming Arduino (using +5V interfaces)
//     Power off programming Arduino by disconnecting from USB
//     Connect jumper wires with male ends as follows
//          Pin      color of wire
//          GND        BLACK
//          2 RX       GREEN  (Arduino TX)
//          9 TX       YELLOW (Arduino RX)
//     Connect the programming Arduino jumper wires to the RBG jumper wires using color as the guide.
//   On your KCX_BT_EMITTER part
//     Power on KCX_BT_EMITTER
//   On the separate programming Arduino (using +5V interfaces)
//     Connect programming Arduino to USB for PC running the Arduino software
//     Upload the sketch from this file into the programming Arduino
//     Open Serial Monitor by selecting menu "Tools" -> "Serial Monitor" and follow instructions
//        After each selected step, wait for the string "--- KCX_BT_EMITTER PROGRAMMING STEP COMPLETE ---"
//     Disconnect programming Arduino from USB for PC running the Arduino software
//
//   Disconnect the Programming Arduino (see below)
//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////
// To disconnect the Programming Arduino
//   Ensure that programming Arduino is disconnected from USB for PC running the Arduino software
//   On your KCX_BT_EMITTER part
//     Power off the KCX_BT_EMITTER
//   On separate programming Arduino (using +5V interfaces)
//     Disconnect the programming Arduino jumper wires from the RBG jumper wires
//   On your KCX_BT_EMITTER part
//     Store the jumper connectors safely
//////////////////////////////////////////////////////////////////////////////////////

#include "Arduino.h"
#include "SoftwareSerial.h"                  // to talk to KCX_BT_EMITTER without using up debug serial port

// NOTE that Serial.read() from Arduino serial monitor will have a 0x0A (newline) after the characters typed in
//    This must be removed before sending to the Bluetooth module - it would cause the command to be rejected

// The KCX_BT_EMITTER (and some information on it) can be found here
//     https://www.aliexpress.com/item/33058710334.html

// FIXME TODO update this list for KCX_BT_EMITTER v1.7
//
// Here is a list of commands and the responses I have seen. This list is based on the following plus my experimentation
//     https://item.taobao.com/item.htm?spm=a21wu.12321156-tw.0.0.7e76d1c7xEOcFZ&id=570274835710
//     https://www.electro-tech-online.com/threads/kcx_bt_emitter-low-cost-bluetooth-bt-audio-module.158156/
// < 1>: Test Command
// Description: test communication is normal
//   AT+
//     OK+
// < 2>: system reset
// Description: RESET
//   AT+REST
//     OK+
//     REST
//     POWER ON
// < 3>: query software version
// Description: Check the software version; my devices show V1.1
//   AT+GMR
//     OK+
//     KCX_BTEMITTER_Vx.x
// < 4>: query connection status
// Description: search for Bluetooth connection status; x=0: no connection, x=1 connected
//   AT+STATUS
//     OK+
//     STATUS:x
// < 5>: specify MAC address connection
// Description: connect to the MAC address link (here shows the device connecting to the MAC-0xf44efdecd39d )
// NOTE: hex string must be EXACTLY 12 characters; add leading zeros if needed
//   AT+CONADD=0xf44efdecd39d
//     OK+
//     CON:0xf44efdecd39d
//     CONNECTED
// < 6>: disconnect
// Description: disconnect the current bluetooth connection
//   Send : AT+DISCON
//     OK+DISCON
//     DISCONNECT
// < 7>: Scan for any visible Bluetooth devices
// Description: search Bluetooth receive device, cycle search and list ? Blue tooth device information found
//   AT+SCAN
//     OK+SCAN
//     New Devices:1
//     MacAdd:0xf44efdecd39d
//     Name:Bluetooth Audio
//     ALL Devices=1
// < 8>: add the Bluetooth device MAC address to internal list of MAC addresses for auto-connection
// Description: with this command-set, a total of 10 MAC address records can be attached to 10 devices in a sequence.
// Set the memory. When the module is started , it will search the MAC address of the device and the MAC address of the recording area.
// The address is automatically connected to any one of the MAC addresses of the recording area, and is not connected when the address is not consistent, so as to achieve the specified MAC address connection.
// When 10 recording spaces are empty , MAC address filtering is not performed ( by default, this area is empty in the factory).
//   AT+ADDLINKADD=0xf44efdecd39d
//     OK+
//     ADDLINKADD
//     VM_MacAdd 1 =0xf44efdecd39d
// < 9>: associate the Bluetooth device name with internal list of MAC addresses for auto-connection
// Description: with this command setting, specifying the blue tooth name connection, a total of 10 blue tooth names can be added, and 10 devices can be recorded sequentially on the chip.
// Memory. When the module is started, it will search the blue tooth name of the device and the blue tooth name of the recording area.
// The blue tooth name is automatically associated with any of the blue tooth names in the recording area , and is not associated with any of the blue tooth names in the recording area , so as to achieve the specified blue tooth name connection function . When 10
// When all the recording spaces are empty , the blue tooth name is not filtered ( by default, this area is empty in the factory).
//   AT+ADDLINKNAME=S1 Pro MDO
//     OK+ADDLINKNAME
//     VM_Name 0 =S1 Pro MDO
// <10>: query the auto-connect memory area
// Description: sending this command returns all information recorded in the automatically reconnect MAC and device name memory.
//   AT+VMLINK?
//     OK+VMLINK
//     BT_ADD_NUM=1
//     BT_NAME_NUM=1
//     Last_Add=0xf44efdecd39d
//     VM_MacAdd0=0xf44efdecd39d
//     VM_Name0=S1 Pro MDO
// <11>: delete all records in the auto-connect memory
//   AT+DELVMLINK
//     Delete_Vmlink

#define DPIN_BLUESRL_RX   9  // serial in  - talk to Bluetooth module (KCX_BT_EMITTER) - connect to YELLOW wire
#define DPIN_BLUESRL_TX   2  // serial out  - talk to Bluetooth module (KCX_BT_EMITTER) - connect to GREEN wire

SoftwareSerial myBlueSerial(DPIN_BLUESRL_RX, DPIN_BLUESRL_TX); // to talk to Bluetooth 

#define NUMWAIT  3000 // loopcount waiting for response from Bluetooth module

char inBytes[100];
//char const * cmd1 = "AT+";
//char const * cmd2 = "AT+REST";
//char const * cmd3 = "AT+GMR";
//char const * cmd4 = "AT+STATUS";
//char const * cmd5 = "AT+VMLINK?";
//char const * cmd6 = "AT+DISCON";
//char const * cmd7 = "AT+ADDLINKADD=0xf44efdecd39d"; // NOTE: must be exactly 12 characters for hex string
//char const * cmd8 = "AT+ADDLINKNAME=S1 Pro MDO";
//char const * cmd9 = "AT+VMLINK?";
//char const * cmd10 = "AT+";

//#define NUMBAUDS 10
//int long bauds[NUMBAUDS] = { 1200, 2400, 4800, 9600, 14400, 19200, 28800, 31250, 38400, 57600 };

//char const * cmds[10] = { cmd1, cmd2, cmd3, cmd4, cmd5, cmd6, cmd7, cmd8, cmd9, cmd10 };

// program
// #define NUMCMDS  10

// examine
// #define NUMCMDS 5

unsigned int reportBlueCom();
void sendBlueCmd( char * cmd );

void setup() {

  Serial.begin(115200);         // this is for general debug
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println();
  Serial.println(F("Bluetooth testing init..."));

  myBlueSerial.begin(9600); // this is control for Bluetooth module (KCX_BT_EMITTER)

}  // end setup()

void loop() {

  // testRx();
  testEcho();
  // testCmds();

} // end loop()

void testRx() {

  reportBlueCom();

}
void testEcho() {

  static char eolBytes[10];
  static unsigned char mychar;
  static unsigned int idx = 0;
  static unsigned int odx = 0;
  static unsigned int edx = 0;
  int loopIdx;

  while (Serial.available()) {
    mychar = Serial.read(); // this side is faster
    if (((int) mychar) >= 0x20) { // if it is space or higher
      inBytes[idx++] = mychar;
      delay(5);
    } else {
      eolBytes[edx++] = mychar;
      delay(5);
      while (Serial.available()) {
        mychar = Serial.read(); // this side is faster
        eolBytes[edx++] = mychar;
        delay(5);
      }
      break;
    }
  } // end try to read input to the end of line

  if (idx > 0) { // if anything to send to Bluetooth chip
    // display the input
    for (odx = 0; odx < idx; odx++) {
      Serial.print(F("char 0x"));
      Serial.print((int) inBytes[odx], HEX);
      Serial.print(F(" "));
      Serial.println(inBytes[odx]);
      // printf("\neol char 0x%02X %c\n", (int) inBytes[odx], inBytes[odx]);
    }
    for (odx = 0; odx < edx; odx++) {
      Serial.print(F("\neol char 0x"));
      Serial.println((int) eolBytes[odx], HEX);
      // printf("\neol char 0x%02X\n", (int) eolBytes[odx]);
    }
    // send input (without EOL) to Bluetooth module
    inBytes[idx] = 0;
    sendBlueCmd(inBytes);
    // Clean up
    idx = edx = 0;
  } else { // just display what there is if anything
    if (myBlueSerial.available()) {
      reportBlueCom();
    }
  }
  
} // end test1()

//void testCmds() {
//
//  int cmdIdx;
//
//  for (cmdIdx = 0; cmdIdx < NUMCMDS; cmdIdx += 1) {
//    Serial.println(F(""));
//    Serial.print(F("--CMD "));
//    Serial.print(cmdIdx);
//    Serial.print(F(" "));
//    Serial.println(cmds[cmdIdx]);
//    sendBlueCmd((char *) cmds[cmdIdx]);
//  }
//
//  delay(100);
//
//}

unsigned int reportBlueCom() {

  unsigned int idx = 0;
  unsigned int odx = 0;

  while (myBlueSerial.available()) {
    inBytes[idx++] = myBlueSerial.read();
  }
  if (idx > 0) {
    odx = 0;
    for (odx = 0; odx < idx; odx += 1) {
      Serial.print(inBytes[odx]);
    }
  } // if anything received from Bluetooth chip
  return(idx);
} // end reportBlueCom

void sendBlueCmd( char * cmd ) {
  int loopIdx;
  // Serial.println(cmd); // send this before sending to device
  myBlueSerial.print(cmd);
  for (loopIdx = 0; loopIdx < NUMWAIT; loopIdx += 1) {
    reportBlueCom();
    delay(5);
  }
} // end sendBlueCmd
