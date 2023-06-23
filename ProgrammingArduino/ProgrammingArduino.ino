// Author: Mark Olson 2020-09-03
//
// originally written for KCX_BT_EMITTER firmware V1.1 for
//   Rubber Band Gun - https://github.com/Mark-MDO47/RubberBandGun
//   RBG - A high-tech imagining of the rubber band gun
//
// updated 2023-06 by Holger Pandel for KCX_BT_EMITTER firmware V1.7 in https://github.com/pandel/BluetoothAudioTransmitter_KCX_BT_EMITTER/tree/v1.7
// further updates 2023-06 by Mark Olson
//

// The (original) purpose of ProgrammingArduino.ino is to program the KCX_BT_EMITTER Bluetooth module to auto-connect
//     to a specific set of bluetooth speakers, headphones, etc.  Once this is done, the list will be retained
//     by the Bluetooth module and the Programming Arduino can be disconnected.
// The KCX_BT_EMITTER module (and some information on it) can be found here: https://www.aliexpress.com/item/33058710334.html

// When running the sketch, you can select steps in any order as follows:
//     1. Pair with Bluetooth receiver devices (such as speaker, headphones, etc.)
//     2. Display stored auto-connect Bluetooth receiver devices
//     3. Add one auto-connect Bluetooth receiver devices to storage
//     4. Delete all auto-connect Bluetooth receiver devices from storage
// You can have up to 10 auto-connect Bluetooth receiver devices in storage
// The Bluetoth module will connect to the first device it finds in order
//
// To gather necessary information about your Bluetooth receiver device, select option 1
// To add a device to the auto-connect storage, select option 2 and enter the necessary information

//////////////////////////////////////////////////////////////////////////////////////
// To do this programming:
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


//////////////////////////////////////////////////////////////////////////////////////
// Updated version for module KCX_BT_EMITTER v1.7
//////////////////////////////////////////////////////////////////////////////////////

// FIXME TODO update this list for KCX_BT_EMITTER v1.7
//
// Here is a list of "AT" commands and the responses I have seen. This list is based on the following plus my experimentation
//     https://item.taobao.com/item.htm?spm=a21wu.12321156-tw.0.0.7e76d1c7xEOcFZ&id=570274835710
//     https://www.electro-tech-online.com/threads/kcx_bt_emitter-low-cost-bluetooth-bt-audio-module.158156/
// < 1>: Test Command
// Description: test communication is normal
//   AT+
//     OK+
// < 2>: system reset
// Description: RESET
//   AT+RESET
//     OK+
//     REST
//     POWER ON
// < 3>: query software version
// Description: Check the software version; my devices show V1.1
//   AT+GMR?
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
//   AT+PAIR
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


#include "Arduino.h"
#include "SoftwareSerial.h"  // to talk to KCX_BT_EMITTER without using up debug serial port

// NOTE that Serial.read() from Arduino serial monitor will have a 0x0A (newline) after the characters typed in
//    This must be removed before sending to the Bluetooth module - it would cause the command to be rejected

#define DPIN_BLUESRL_RX A2  // serial in  - talk to Bluetooth module (KCX_BT_EMITTER) - connect to YELLOW wire
#define DPIN_BLUESRL_TX A3  // serial out  - talk to Bluetooth module (KCX_BT_EMITTER) - connect to GREEN wire

SoftwareSerial myBlueSerial(DPIN_BLUESRL_RX, DPIN_BLUESRL_TX);  // to talk to Bluetooth


#define NUMWAIT 500  // loopcount waiting for response from Bluetooth module

#define NUMOF(a) (sizeof((a)) / sizeof(*(a)))

uint8_t DEBUG_SERIAL_IN = 0;  // 1 to debug the serial input code

char inBytes[100];
char const *cmd_AT = "AT+";
char const *cmd_REST = "AT+RESET";
char const *cmd_GMR = "AT+GMR?";
char const *cmd_BAUD = "AT+BAUD?";
char const *cmd_STATUS = "AT+STATUS?";
char const *cmd_DISCON = "AT+DISCON";
char const *cmd_PAIR = "AT+PAIR";
char const *cmd_ADDLINKADD = "AT+ADDLINKADD=0x";  // NOTE: must be exactly 12 characters for hex string
char const *cmd_ADDLINKNAME = "AT+ADDLINKNAME=";
char const *cmd_VMLINK = "AT+VMLINK?";
char const *cmd_DELVMLINK = "AT+DELVMLINK";
char const *cmd_PWROFF = "AT+POWER_OFF";

char cmd_bld_ADDLINKADD[32];   // must build these in RAM from user input
char cmd_bld_ADDLINKNAME[64];  // must build these in RAM from user input

#define CMD_COUNT 7

char const *cmdsPair[] = { cmd_AT, cmd_REST, cmd_AT, cmd_PAIR };
char const *cmdsDispRAM[] = { cmd_AT, cmd_VMLINK };
char const *cmdsStatus[] = { cmd_AT, cmd_GMR, cmd_BAUD, cmd_STATUS };
char const *cmdsAddRAM[] = { cmd_AT, cmd_DISCON, cmd_VMLINK, cmd_bld_ADDLINKADD, cmd_bld_ADDLINKNAME, cmd_REST, cmd_AT, cmd_VMLINK };
char const *cmdsClearRAM[] = { cmd_AT, cmd_DISCON, cmd_DELVMLINK, cmd_REST, cmd_AT, cmd_VMLINK };
char const *cmdsDiscon[] = { cmd_AT, cmd_DISCON };
char const *cmdsPowerOff[] = { cmd_AT, cmd_PWROFF };

char const *menuOptions[CMD_COUNT] = { "\n1=PAIR", "\n2=DISPLAY", "\n3=ADD", "\n4=DELETE ALL", "\n5=STATUS", "\n6=Disconnect", "\n7=PowerOff module" };

void setup() {

  Serial.begin(115200);  // this is for general debug
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println();
  Serial.print(F("Bluetooth Programming Arduino init..."));
  myBlueSerial.begin(115200);  // this is control for Bluetooth module (KCX_BT_EMITTER)
  Serial.println(F(" completed!"));

}  // end setup()

void loop() {
  static uint8_t menuPrinted = 0;

  if (0 == menuPrinted) {
    menuPrinted = 1;
    Serial.println(F(""));
    Serial.println(F("   1 - Pair with Bluetooth receiver devices (such as speaker, headphones, etc.)"));
    Serial.println(F("   2 - Display stored auto-connect Bluetooth receiver devices"));
    Serial.println(F("   3 - Add one auto-connect Bluetooth receiver device to storage"));
    Serial.println(F("   4 - Delete all auto-connect Bluetooth receiver devices from storage"));
    Serial.println(F("   5 - Current status"));
    Serial.println(F("   6 - BT Disconnect"));
    Serial.println(F(".  7 - PowerOff module"));
    Serial.print(F("==> "));
  }

  if (Serial.available()) {
    uint8_t myChoice = 0;

    menuPrinted = 0;
    myChoice = getSerial_uint8_t();
    if ((0 == myChoice) || (myChoice > CMD_COUNT)) {
      Serial.print(F("ERROR - choice must be 1 through "));
      Serial.println(CMD_COUNT);
    } else {
      Serial.println(menuOptions[myChoice - 1]);
      processCommand(myChoice);
    }
  }

  // testEcho(); // this one is useful for debugging command sequences
}  // end loop()


/////////////////////////////////////////////////////////////////////////////////////////////////////////
// processCommand(uint8_t theChoice)
//    performs one of the four menu options
//    as needed, reports scan info from Bluetooth
//
void processCommand(uint8_t theChoice) {
  uint8_t addlinkOk = 0;

  reportBlueCom();
  switch (theChoice) {
    case 1:  // 1 - Pair with Bluetooth receiver devices (such as speaker, headphones, etc.)");
      sendBlueCmds(cmdsPair, (uint8_t)NUMOF(cmdsPair));
      // need to hang around a little more
      loopReportBlueCom();
      break;
    case 2:  // 2 - Display stored auto-connect Bluetooth receiver devices");
      sendBlueCmds(cmdsDispRAM, (uint8_t)NUMOF(cmdsDispRAM));
      break;
    case 3:  // 3 - Add one auto-connect Bluetooth receiver device to storage");
      addlinkOk = buildADDLINKADD(cmd_bld_ADDLINKADD, NUMOF(cmd_bld_ADDLINKADD) - strlen(cmd_ADDLINKADD) - 2);
      if (0 != addlinkOk) {
        reportBlueCom();
        addlinkOk = buildADDLINKNAME(cmd_bld_ADDLINKNAME, NUMOF(cmd_bld_ADDLINKNAME) - strlen(cmd_ADDLINKNAME) - 2);
      }
      if (0 != addlinkOk) {
        reportBlueCom();
        sendBlueCmds(cmdsAddRAM, (uint8_t)NUMOF(cmdsAddRAM));
        // need to hang around a little more
        loopReportBlueCom();
      } else {
        reportBlueCom();
        Serial.println(F(""));
        Serial.println(F("User aborted storing MAC address for the Bluetooth speaker or headphones\n"));
      }
      break;
    case 4:  // 4 - Delete all auto-connect Bluetooth receiver devices from storage");
      sendBlueCmds(cmdsClearRAM, (uint8_t)NUMOF(cmdsClearRAM));
      loopReportBlueCom();
      break;
    case 5:  // 5 - Show current status");
      sendBlueCmds(cmdsStatus, (uint8_t)NUMOF(cmdsStatus));
      loopReportBlueCom();
      break;
    case 6:  // 6 - Disconnect current BT");
      sendBlueCmds(cmdsDiscon, (uint8_t)NUMOF(cmdsDiscon));
      loopReportBlueCom();
      break;
    case 7:  // 7 - PowerOff module");
      sendBlueCmds(cmdsPowerOff, (uint8_t)NUMOF(cmdsPowerOff));
      loopReportBlueCom();
      break;
  }  // end switch()
  Serial.println(F("\n--- KCX_BT_EMITTER PROGRAMMING STEP COMPLETE ---"));
}  // end processCommand()

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// buildADDLINKADD(char * cmdAddr, uint16_t cmdMaxLen)
//    builds a RAM AT+ADDLINKADD command from user input
//
// NOTE: the hex MAC Address sent in the AT command must be exactly 12 characters
//
uint8_t buildADDLINKADD(char *cmdAddr, uint16_t cmdMaxLen) {
  char myLowerCase[15];  // never want to over-run
  uint8_t myNumChars = 0;
  uint8_t myStart = 0;
  char const *zeroFill = "000000000000";
  Serial.println(F(""));
  Serial.println(F("Enter the unique MAC address for the Bluetooth speaker or headphones; it starts with 0x"));
  Serial.println(F("    upper or lower case does not matter; maximum of 12 characters after the 0x"));
  Serial.println(F("To abort adding an auto-connect Bluetooth receiver device to storage, just enter an empty line"));
  Serial.print(F("==> "));
  while (!Serial.available()) {
    ;  // wait
  }
  Serial.println(F(" "));
  // reportBlueCom();
  myNumChars = getSerial_chars();
  if (0 == myNumChars) {
    return (0);  // abort
  }
  if (('0' == inBytes[0]) && ('x' == tolower(inBytes[1]))) {
    myStart = 2;
  }
  myNumChars -= myStart;
  if (DEBUG_SERIAL_IN) {
    Serial.print(F("   input string |"));
    Serial.print(inBytes);
    Serial.print(F("| myStart "));
    Serial.print(myStart);
    Serial.print(F(" myNumChars "));
    Serial.println(myNumChars);
  }  // end if (DEBUG_SERIAL_IN)
  if (myNumChars > 12) {
    Serial.println(F("Abort - more than 12 characters"));
    return (0);  // abort
  }
  for (int j = myStart; (j < (myNumChars + myStart)) && (0 != inBytes[j]); j++) {
    if (0 == isxdigit(inBytes[j])) {
      Serial.print(F("Abort - non-hex digit: "));
      Serial.print(inBytes[j]);
      Serial.print(F(" j "));
      Serial.println(j);
      return (0);  // abort
    }
    myLowerCase[j] = tolower(inBytes[j]);
  }
  myLowerCase[myNumChars + myStart] = 0;
  Serial.print(F("\nYour entry \""));
  Serial.print(inBytes);
  Serial.println(F("\" was accepted"));
  strcpy(cmdAddr, cmd_ADDLINKADD);
  strcat(cmdAddr, &zeroFill[myNumChars]);
  strcat(cmdAddr, &myLowerCase[myStart]);
  return (1);  // successful command build
}  // end buildADDLINKADD()

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// buildADDLINKNAME(char * cmdAddr, uint16_t cmdMaxLen)
//    builds a RAM AT+ADDLINKNAME command from user input
//
uint8_t buildADDLINKNAME(char *cmdAddr, uint16_t cmdMaxLen) {
  uint8_t myNumChars = 0;
  char const *zeroFill = "000000000000";
  Serial.println(F(""));
  Serial.println(F("Enter the name you choose for this device; it is OK to place spaces between words"));
  Serial.println(F("    maximum of 20 characters total"));
  Serial.println(F("To abort adding an auto-connect Bluetooth receiver device to storage, just enter an empty line"));
  Serial.print(F("==> "));
  while (!Serial.available()) {
    ;  // wait
  }
  Serial.println(F(" "));
  // reportBlueCom();
  myNumChars = getSerial_chars();
  if (0 == myNumChars) {
    return (0);  // abort
  }
  if (myNumChars > 20) {
    Serial.println(F("Abort - more than 20 characters"));
    return (0);  // abort
  }
  Serial.print(F("\nYour entry \""));
  Serial.print(inBytes);
  Serial.println(F("\" was accepted"));
  strcpy(cmdAddr, cmd_ADDLINKNAME);
  strcat(cmdAddr, inBytes);
  return (1);  // successful command build
}  // end buildADDLINKNAME()


/////////////////////////////////////////////////////////////////////////////////////////////////////////
// getSerial_chars()
//    gets character string from USB serial; removes EOL chars; stores in inBytes[] zero terminated
//
//    returns number of characters in inBytes
//
// no code to flag input over-run
// stores zero-terminated string in inBytes
//
uint8_t getSerial_chars() {
  static unsigned char myChar;
  static uint8_t idx = 0;

  idx = 0;
  while (Serial.available()) {
    myChar = Serial.read();
    if (DEBUG_SERIAL_IN) {
      Serial.print(F("\n  getSerial_chars got myChar 0x"));
      Serial.print(myChar, HEX);
    }                             // end if(DEBUG_SERIAL_IN)
    if (((int)myChar) >= 0x20) {  // if it is space or higher
      if (DEBUG_SERIAL_IN) { Serial.print(F(" >= 0x20 ")); }
      if ((idx + 2) <= NUMOF(inBytes)) {
        inBytes[idx++] = myChar;
        if (DEBUG_SERIAL_IN) {
          Serial.print(F("count of inBytes #"));
          Serial.println(idx);
        }  // end if(DEBUG_SERIAL_IN)
      } else {
        if (DEBUG_SERIAL_IN) {
          Serial.print(F(" not stored; idx "));
          Serial.print(idx);
          Serial.print(F(" not stored; NUMOF(inBytes) "));
          Serial.println(NUMOF(inBytes));
        }  // end if(DEBUG_SERIAL_IN)
      }
      delay(5);
    } else {
      delay(5);
      if (DEBUG_SERIAL_IN) {
        Serial.print(F(" --- not stored! Count of inBytes #"));
        Serial.println(idx);
      }  // end if(DEBUG_SERIAL_IN)
      while (Serial.available()) {
        myChar = Serial.read();
        delay(5);
      }
      break;
    }
  }  // end try to read input to the end of line

  inBytes[idx] = 0;
  if (DEBUG_SERIAL_IN) {
    Serial.print(F("\n  getSerial_chars idx "));
    Serial.print(idx);
    Serial.print(F(" inBytes[idx] 0x"));
    Serial.print(inBytes[idx], HEX);
    Serial.print(F(" inBytes |"));
    Serial.print(inBytes);
    Serial.println(F("|"));
  }  // end if(DEBUG_SERIAL_IN)
  return (idx);
}  // end getSerial_chars()

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// getSerial_uint8_t()
//    gets single-digit decimal number from USB serial; returns the number itself (not ASCII)
//
uint8_t getSerial_uint8_t() {
  getSerial_chars();
  if (DEBUG_SERIAL_IN) {
    Serial.print(F("\n  getSerial_uint8_t inBytes[0] 0x"));
    Serial.print(inBytes[0], HEX);
    Serial.print(F(" converted to "));
    Serial.println(uint8_t(inBytes[0] - 0x30), HEX);
  }                                     // end if(DEBUG_SERIAL_IN)
  return (uint8_t(inBytes[0] - 0x30));  // subtract ASCII 0
}  // end getSerial_uint8_t()


/////////////////////////////////////////////////////////////////////////////////////////////////////////
// testEcho()
//    Surprisingly tricky routine to echo chars back and forth between the USB serial port
//       and the KCX_BT_EMITTER serial port.
//    Not super hard but there are some things to watch out for like different baud rates
//
void testEcho() {

  static char eolBytes[10];
  static unsigned char mychar;
  static unsigned int idx = 0;
  static unsigned int odx = 0;
  static unsigned int edx = 0;
  int loopIdx;

  while (Serial.available()) {
    mychar = Serial.read();       // this side is faster
    if (((int)mychar) >= 0x20) {  // if it is space or higher
      inBytes[idx++] = mychar;
      delay(5);
    } else {
      eolBytes[edx++] = mychar;
      delay(5);
      while (Serial.available()) {
        mychar = Serial.read();  // this side is faster
        eolBytes[edx++] = mychar;
        delay(5);
      }
      break;
    }
  }  // end try to read input to the end of line

  if (idx > 0) {  // if anything to send to Bluetooth chip
    // display the input
    for (odx = 0; odx < idx; odx++) {
      Serial.print(F("char 0x"));
      Serial.print((int)inBytes[odx], HEX);
      Serial.print(F(" "));
      Serial.println(inBytes[odx]);
      // printf("\neol char 0x%02X %c\n", (int) inBytes[odx], inBytes[odx]);
    }
    for (odx = 0; odx < edx; odx++) {
      Serial.print(F("\neol char 0x"));
      Serial.println((int)eolBytes[odx], HEX);
      // printf("\neol char 0x%02X\n", (int) eolBytes[odx]);
    }
    // send input (without EOL) to Bluetooth module
    inBytes[idx] = 0;
    sendBlueCmd(inBytes);
    // Clean up
    idx = edx = 0;
  } else {  // just display what there is if anything
    if (myBlueSerial.available()) {
      reportBlueCom();
    }
  }

}  // end testEcho()

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// sendBlueCmds(const char * cmdArray[], const uint8_t numCmds)
//    Sends a number of AT commands to KCX_BT_EMITTER module
//    Will report back any response or scan output from the module
//
void sendBlueCmds(const char *cmdArray[], const uint8_t numCmds) {

  Serial.println(F("------------- RESULT ---------------"));
  for (int cmdIdx = 0; cmdIdx < numCmds; cmdIdx += 1) {
    Serial.print(F("--> CMD "));
    Serial.print(cmdIdx);
    Serial.print(F(": "));
    Serial.println(cmdArray[cmdIdx]);
    sendBlueCmd((char *)cmdArray[cmdIdx]);
    Serial.println(F(""));
  }
  Serial.println(F("------------- RESULT END -----------"));
  delay(100);

}  // end sendBlueCmds()

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// sendBlueCmd( char * cmd )
//    Sends one AT-style zero-terminated string to the Bluetooth module
//
void sendBlueCmd(char *cmd) {
  // Serial.println(cmd); // send this before sending to device
  myBlueSerial.println(cmd);
  loopReportBlueCom();
}  // end sendBlueCmd()

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// reportBlueCom()
//    Reports back any response or scan output from the Bluetooth module
//    Returns number of characters seen
//
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
  }  // if anything received from Bluetooth chip
  return (idx);
}  // end reportBlueCom()

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// loopReportBlueCom()
//    Loops reporting back any response or scan output from the Bluetooth module
//    Returns number of characters seen on the last loop
//
unsigned int loopReportBlueCom() {
  int loopIdx;
  unsigned int numBytes;
  for (loopIdx = 0; loopIdx < NUMWAIT; loopIdx += 1) {
    numBytes = reportBlueCom();
    delay(5);
  }
  return (numBytes);
}  // end loopReportBlueCom()
