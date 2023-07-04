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
//     1. Scan for Bluetooth receiver devices (such as speaker, headphones, etc.)
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

// KCX_BT_EMITTER v1.7 list of commands
//
// Here is a list of "AT" commands and the responses I have seen. This list is based on the following plus my experimentation
//     https://github.com/Mark-MDO47/BluetoothAudioTransmitter_KCX_BT_EMITTER/blob/master/specs/BT%20GY19733-CN.zh-CN.en.pdf
//
// <1>: test command
// send: AT+
// Description: Test whether the communication is normal
// Return value: OK+ (Return response)
//
// <2>: System reset
// send: AT+RESET
// Description: reset
// return value: OK+RESET (return answer)
//               POWER ON (restart)
//
// <3>: Read the current serial port baud rate
// send: AT+BAUD?
// Description: Get the baud rate
// return value: OK+BAUD=(no),BAUD = baud (returns the current baud rate)
// >>> DID THEY USE no INSTEAD OF n ???
// Note:baudRange is0-4, the corresponding baud rate is as follows
// n=0,9600
// n=1,19200
// n=2,38400
// n=3,57600
// n=4,115200
//
//
//
// <4>: Set the current serial port baud rate
// send: =AT+BAUD=no
// >>> OR IS IT send: =AT+BAUD=n ???
// Description: Get the baud rate
// return value: OK+BAUD=(no),BAUD = baud (returns the current baud rate)
// >>> AGAIN DID THEY USE no INSTEAD OF n ???
// Note:baud Range is 0-4, the corresponding baud rate is as follows
// n=0,9600
// n=1,19200
// n=2,38400
// n=3,57600
// n=4,115200
// After setting the baud rate, the chip will restart
//
//
// <5>: query version
// send: AT+GMR?
// Description: Check the software version
// return value: OK+VERS :KCX_BT_RTX_V1.x (returns software version)
//
//
// <6>: Query receive/transmit mode
// send: AT+BT_MODE?
// Description: Inquire whether the module is working in transmit mode or receive mode
// return value: OK+BT_EMITTER launch mode
// return value: OK+BT_RECEIVER receive mode
//
// <7>: shutdown
// send: AT+POWER_OFF
// Description: Module shutdown
// return value: OK+POWEROFF_MODE enter shutdown
// Note: To turn on again after shutting down, you need to press the button to wake up or power off and restart to wake up
//
// <8>: Query the currently playing audio source signal channel
// send: AT+CHANNEL?
// Description: Search the currently playing audio source signal channel
// return value: OK+CHANNEL=BT CHANNEL Bluetooth audio channel
// return value: OK+CHANNEL=LINE CHANNEL Analog audio input port
// return value: OK+CHANNEL=PC CHANNEL USBSound card to computer port
//
//
// <9>: Set the current audio source signal channel
// send: AT+CHANNEL=ch
// Description: Search the currently playing audio source signal channel
// return value: ch=0 reserved (void)
// return value: ch=1 Analog audio input port
// return value: ch=2 USB Sound card to computer port
//
// <10>: get connection status
// send: AT+STATUS?
// illustrate:get connection status
// return value: OK+STATUS:0 bluetooth not connected
// return value: OK+STATUS:1 bluetooth connected
//
// <11>: get bluetooth name
// send: AT+NAME?
// illustrate:get bluetooth name
// return value: OK+NAME=(name)
// Name:bluetooth name
// Note: This command is effective in receiving mode, and the transmitting mode does not broadcast the Bluetooth name
//
// <12>: set bluetooth name
// send: AT+NAME+(name)
// illustrate:set bluetooth name
// return value: OK+NAME=(name)
// Name: bluetooth name
// Note: This command is effective in receiving mode, and the transmitting mode does not broadcast the Bluetooth name. After the setting is successful, the chip will restart.
//
// <13>: get bluetoothMACaddress
// send: AT+MAC?
// illustrate:get bluetooth MAC
// return value: OK+MAC:(mac) 3f3734a1c450
//               mac: 6 bytes MAC
// Note: This command reception mode is valid
//
// <14>: get volume
// send: AT+VOL?
// illustrate:get volume
// return value: OK+VOL=(vol)
// Note: vol:volume, range: 00-31, common 32 level, the default maximum volume when booting:31
//
// <15>: Set the volume
// send: AT+VOL=(vol)
// illustrate:set volume
// return value: OK+VOL=(vol)
// Note: vol:volume, range: 00-31, common 32 level, the default maximum volume when booting:31
//
// <16>:play / Pause
// send: AT+PAUSE
// illustrate: play / Pause
// return value: OK+PAUSE pause
// return value: OK+PLAY play
// Note: The play/pause state changes every time this command is sent
//
//
// <17>: get play/pause status
// send: AT+PAUSE?
// illustrate:Get play/pause status
// return value: OK+PAUSE pause
// return value: OK+PLAY play
//
//
// <18>: Disconnect the current connection and search for pairing again
// send: AT+PAIR
// illustrate:Disconnect current connection and search for pairing again
// return value: OK+PAIR
// NOTE: This command and pressing PAIR Same function as a button
//
// <19>: <<< This was a duplicate of <18> >>>
//
// <20>: Search for Bluetooth receiving devices
// send: AT+SCAN
// Description: Search for bluetooth receiving devices, cycle through and list all searched bluetooth device information
// return value: OK+SCAN (Perform device search operation)
// return value: New Devices: 1 (found on the N devices found)
// return value: MacAdd:0x32a16c6f7f99 (This device's MAC address is 0x32a16c6f7f99)
// return value: Name: Bluetooth Audio (The bluetooth name for this device is Bluetooth Audio)
// return value: ALL Devices=1 (The total number of currently searched devices is 1)
//
// <21>: add auto-connected MAC address
// send: AT+ADDLINKADD=(mac)
// Description: Set the specified by this command MAC Address connection, a total of10 individual MAC Address record space can be added 10 Each device is sequentially recorded in the chip's built-in memory.
// When the module is turned on to search for the Bluetooth receiving device, it will search for the device MAC address and record area of MAC Addresses are compared one by one, when the device MAC One of the
// address and recording areas MAC Automatically connect when the address is consistent, and do not connect if there is no match, so as to achieve the specified MAC The function of the address connection. When all record spaces are empty, no MAC The address is matched and filtered, and it will be connected when it is searched (by default, this area is empty, and this command is valid when it is set to transmit
// mode).
// return value: OK+ADDLINKADD=(mac) implementMACaddress memory
// return value: BT_ADD_NUM=(add_num)
// return value: BT_NAME_NUM=(name_num)
// return value: Auto_link_Add:(Auto_link mac)
// return value: VM_MacAdd(add_num) = (add) (MACaddress macmemory in VMdistrict vm_num stored successfully)
// Note: A total of 10 individual MAC address, stored in sequential order at MacAdd 00 - MacAdd 09 common 10
// If the maximum memory area is exceeded, an error will be reported: Addr More than 10 !
//
// <22>: Add the name of the Bluetooth device to be automatically connected
// send: AT+ADDLINKNAME=(name)
// Description: Set the specified Bluetooth name connection through this command, there are a total of10Bluetooth name record spaces can be added10Each device is sequentially recorded in the chip's builtin
// memory. When the module is turned on to search for Bluetooth receiving devices, it will compare the Bluetooth name of the searched device with the Bluetooth name in the recording area one by one. When
// the Bluetooth name of the device is consistent with any of the Bluetooth names in the recording area, it will automatically connect. This achieves the function of specifying the Bluetooth name connection. When all record spaces are empty, no Bluetooth name filtering will be performed (by default, this area is empty) .
// return value: OK+ADDLINKNAME (implement name memory)
// return value: VM_Name (name_num) = (name) (Bluetooth name (name) memory in VM district name_num store successfully)
// Note: A total of 10 Bluetooth names, stored sequentially in VM_Name 00 - VM_Name 09 common 10
// If the maximum memory area is exceeded, an error will be reported: Name More than 10 !
//
// <23>: Query the automatic connection record area
// send: AT+VMLINK?
// Description: Sending this command will return the record in the auto-reconnect MAC All information in the record area and device name record area.
// return value: OK+VMLINK (return answer)
// return value: BT_ADD_NUM=(add_num ) (add_num: memory MAC Number of addresses 0-9 common 10 individual)
// return value: BT_NAME_NUM=(name_num) (name_num: The number of memorized device names and addresses 0-9 common 10 individual)
// return value: VM_MacAdd0=(mac) Last_Add=(last add) (last add: last connected MAC address) (mac: Automatically reconnect recording area 1 of MAC address)
// return value: VM_Name0=(name) (name: Automatically reconnect recording area 1 device name)
//
// <24>: Delete all the records in the automatic connection record area
// send: AT+DELVMLINK
// Description: Delete all records in the automatic connection area (that is, restore to the factory default without filtering MAC address and device name)
// Return value: Delete_Vmlink (Perform delete operation)
//
//
//
// EXAMPLES:
// [1] send AT+ test command
// $$$EXAMPLE 1$$$
// <<<send>>> AT+
// {{{recv}}} OK+
//
// [2] Set the automatic connection address (for example, we have 2 bluetooth receivers, we know these device MAC address are 50c4a134373f and 123456789abc,
// we need to search only these two in many Bluetooth receiving devices MAC
// The device with the matching address will be connected, and other devices will not be connected after searching.
// At this time, you only need to connect the two Bluetooth MAC
// The address can be added to the filter list, and the maximum can be added 10 group, the secondary record group has power-off save function)
// $$$EXAMPLE 2$$$
// <<<send>>> AT+ADDLINKADD=50c4a134373f
// {{{recv}}} OK+ADDLINKADD=50c4a134373f
// {{{recv}}} BT_ADD_NUM=01
// {{{recv}}} BT_NAME_NUM=00
// {{{recv}}} Auto_link_Add:null
// {{{recv}}} MEM_MacAdd 00:50c4a134373f
// <<<send>>> AT+ADDLINKADD=123456789abc
// {{{recv}}} OK+ADDLINKADD=123456789abc
// {{{recv}}} BT_ADD_NUM=02
// {{{recv}}} BT_NAME_NUM=00
// {{{recv}}} Auto_link_Add:null
// {{{recv}}} MEM_MacAdd 00:50c4a134373f
// {{{recv}}} MEM_MacAdd 01:123456789abc
// The data returned by the first command is analyzed as follows:
//   OK+ADDLINKADD=50c4a134373f After receiving the command, return the to-be-written MAC address is 50c4a134373f
//   BT_ADD_NUM=01              ADD_NUM district( MACaddress filtering connection) records the 1 Group
//   BT_NAME_NUM=00             recorded in NAME_NUM Zone (Bluetooth Name Filter Connections) records the 0 group, no record
//   Auto_link_Add:null         last connected device MAC address: null none
//   MEM_MacAdd 00:50c4a134373f MEM_MacAddrecording area 00 District has a set of records, MACaddress is 50c4a134373f
// The data returned by the second command is analyzed as follows:
//   OK+ADDLINKADD=123456789abc After receiving the command, return the to-be-written MAC address is 123456789abc
//   BT_ADD_NUM=02              ADD_NUM district (MACaddress filtering connection) records the 2 Group
//   BT_NAME_NUM=00             recorded in NAME_NUM Zone (Bluetooth Name Filter Connections) records the 0 group, no record
//   Auto_link_Add:null         last connected device MAC address: null none
//   MEM_MacAdd 00:50c4a134373f MEM_MacAddrecording area 00 District has a set of records, MACaddress is 50c4a134373f
//   MEM_MacAdd 01:123456789abc MEM_MacAddrecording area 01 District has a set of records, MACaddress is 123456789abc
//
// [3] Set the automatic connection Bluetooth name (for example, we have 2 bluetooth receivers, we know the bluetooth name for Bluetooth Audio and bt_audio, we need to search only these two in many Bluetooth receiving devices.
// The device with the Bluetooth device name is only connected, and other devices are not connected after searching.
// At this time, you only need to put the two Bluetooth device names.
// Just add to the filter list, up to support 10 group, this record group has power-off save function).
// <<<send>>> AT+ADDLINKNAME=Bluetooth Audio
// {{{recv}}} OK+ADDLINKNAME=Bluetooth Audio
// {{{recv}}} BT_ADD_NUM=00
// {{{recv}}} BT_NAME_NUM=01
// {{{recv}}} Auto_link_Add:null
// {{{recv}}} MEM_Name 00:Bluetooth Audio
// <<<send>>> AT+ADDLINKNAME=bt_audio
// {{{recv}}} OK+ADDLINKNAME=bt_audio
// {{{recv}}} BT_ADD_NUM=00
// {{{recv}}} BT_NAME_NUM=02
// {{{recv}}} Auto_link_Add:null
// {{{recv}}} MEM_Name 00:Bluetooth Audio
// {{{recv}}} MEM_Name 01:bt_audio
// The data returned by the first command is parsed as follows:
//   OK+ADDLINKNAME=Bluetooth Audio   After receiving the command, return the name of the bluetooth to be written into the automatic connection filterBluetooth Audio
//   BT_ADD_NUM=00                    ADD_NUMdistrict(MACaddress filtering connection) records the 0 group, no record
//   BT_NAME_NUM=01                   recorded inNAME_NUMZone (Bluetooth Name Filter Connections) records the 1 Group
//   Auto_link_Add: null              last connected deviceMACaddress: null none
//   MEM_Name 00: Bluetooth Audio     MEM_Namerecording area 00 zone has a group of records, bluetooth named Bluetooth Audio
// The data returned by the second command is parsed as follows:
//   OK+ADDLINKNAME=bt_audio          After receiving the command, return the name of the bluetooth to be written into the automatic connection filter bt_audio
//   BT_ADD_NUM=00                    ADD_NUMdistrict(MACaddress filtering connection) records the 0 group, no record
//   BT_NAME_NUM=02                   recorded in NAME_NUM Zone (Bluetooth Name Filter Connections) records the 2 Group
//   Auto_link_Add: null              last connected deviceMACaddress:nullnone
//   MEM_Name 00: Bluetooth Audio     MEM_Name recording area 00 zone has a group of records, bluetooth named Bluetooth Audio
//   MEM_Name 01: bt_audio            MEM_Name recording area 01 zone has a group of records, bluetooth named bt_audio
// Note: MAC The relationship between address filtering and Bluetooth name filtering is OR, that is, as long as the searched MAC address or Bluetooth name is in the filter connection list, as long as there is a match, it will be automatically connected.
// If there is no record in the filter list, the device will be connected if it is found.
// Memories that need to delete the filter list only need to send the command:AT+DELVMLINK
// You can delete all the Bluetooth name and MAC Filter lists.
//

#include "Arduino.h"
#include "SoftwareSerial.h"  // to talk to KCX_BT_EMITTER without using up debug serial port

// NOTE that Serial.read() from Arduino serial monitor will have a 0x0A (newline) after the characters typed in
//    This must be removed before sending to the Bluetooth module - it would cause the command to be rejected

#define DPIN_BLUESRL_RX 9  // serial in  - talk to Bluetooth module (KCX_BT_EMITTER) - connect to YELLOW wire
#define DPIN_BLUESRL_TX 2  // serial out  - talk to Bluetooth module (KCX_BT_EMITTER) - connect to GREEN wire

SoftwareSerial myBlueSerial(DPIN_BLUESRL_RX, DPIN_BLUESRL_TX);  // to talk to Bluetooth


#define NUMWAIT 500  // loopcount waiting for response from Bluetooth module

#define NUMOF(a) (sizeof((a)) / sizeof(*(a)))

uint8_t DEBUG_SERIAL_IN = 0;  // 1 to debug the serial input code

char inBytes[100];
char const *cmd_AT = "AT+";
char const *cmd_RESET = "AT+RESET";
char const *cmd_BAUD_RD = "AT+BAUD?";
char const *cmd_BAUD = "AT+BAUD="; // 0=9600 1=19200 2=38400 3=57600 4=115200 (default = 4)
char const *cmd_GMR_RD = "AT+GMR?";
char const *cmd_BT_MODE_RD = "AT+BT_MODE?";
char const *cmd_PWROFF = "AT+POWER_OFF";
char const *cmd_AUD_CH_RD = "AT+CHANNEL?";
char const *cmd_AUD_CH = "AT+CHANNEL="; // 0=reserved 1=Analog audio input port 2=USB Sound card to computer port
char const *cmd_STATUS_RD = "AT+STATUS?";
char const *cmd_BT_NAME_RD = "AT+NAME?"; // receiving mode only
char const *cmd_BT_NAME = "AT+NAME="; // receiving mode only
char const *cmd_BT_MAC_RD = "AT+MAC?";
char const *cmd_VOL_RD = "AT+VOL?";
char const *cmd_VOL = "AT+VOL="; // range: 00-31 ?must be exactly 2 characters?
char const *cmd_TOGGLE_PLAY_PAUSE = "AT+PAUSE";
char const *cmd_PAIR = "AT+PAIR";
char const *cmd_SCAN = "AT+SCAN";
char const *cmd_DISCON = "AT+DISCON";
char const *cmd_ADDLINKADD = "AT+ADDLINKADD=";  // NOTE: must be exactly 12 characters for hex string
char const *cmd_ADDLINKNAME = "AT+ADDLINKNAME=";
char const *cmd_VMLINK_RD = "AT+VMLINK?";
char const *cmd_DELVMLINK = "AT+DELVMLINK";

char cmd_bld_ADDLINKADD[32];   // must build these in RAM from user input
char cmd_bld_ADDLINKNAME[64];  // must build these in RAM from user input

#define CMD_COUNT 8 // 0 counts as a command
#define CMD_MAX (CMD_COUNT-1)

char const *cmdsPair[] = { cmd_AT, cmd_RESET, cmd_AT, cmd_PAIR };
char const *cmdsScan[] = { cmd_AT, cmd_RESET, cmd_AT, cmd_SCAN };
char const *cmdsDispRAM[] = { cmd_AT, cmd_VMLINK_RD };
char const *cmdsStatus[] = { cmd_AT, cmd_GMR_RD, cmd_BAUD_RD, cmd_BT_MODE_RD, cmd_AUD_CH_RD, cmd_BT_NAME_RD, cmd_BT_MAC_RD, cmd_VOL_RD, cmd_STATUS_RD };
char const *cmdsAddRAM[] = { cmd_AT, cmd_DISCON, cmd_VMLINK_RD, cmd_bld_ADDLINKADD, cmd_bld_ADDLINKNAME, cmd_RESET, cmd_AT, cmd_VMLINK_RD };
char const *cmdsClearRAM[] = { cmd_AT, cmd_DISCON, cmd_DELVMLINK, cmd_RESET, cmd_AT, cmd_VMLINK_RD };
char const *cmdsDiscon[] = { cmd_AT, cmd_DISCON };
char const *cmdsPowerOff[] = { cmd_AT, cmd_PWROFF };

char const *menuOptions[CMD_COUNT] = { "\n0=SCAN", "\n1=PAIR", "\n2=DISPLAY", "\n3=ADD", "\n4=DELETE ALL", "\n5=STATUS", "\n6=Disconnect", "\n7=PowerOff module" };

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
    Serial.println(F("   0 - Pair with Bluetooth receiver devices (such as speaker, headphones, etc.)"));
    Serial.println(F("   1 - Scan for Bluetooth receiver devices"));
    Serial.println(F("   2 - Display stored auto-connect Bluetooth receiver devices"));
    Serial.println(F("   3 - Add one auto-connect Bluetooth receiver device to storage"));
    Serial.println(F("   4 - Delete all auto-connect Bluetooth receiver devices from storage"));
    Serial.println(F("   5 - Current status"));
    Serial.println(F("   6 - BT Disconnect"));
    Serial.println(F(".  7 - PowerOff module"));
    Serial.print(F("==> "));
  }

  if (Serial.available()) {
    uint8_t myChoice = 99;

    menuPrinted = 0;
    myChoice = getSerial_uint8_t();
    if (myChoice > CMD_MAX) {
      Serial.print(F("ERROR - choice must be 0 through "));
      Serial.println(CMD_MAX);
    } else {
      Serial.println(menuOptions[myChoice]);
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
    case 0:  // 0 - Pair with Bluetooth receiver devices (such as speaker, headphones, etc.)");
      sendBlueCmds(cmdsPair, (uint8_t)NUMOF(cmdsPair));
      // need to hang around a little more
      loopReportBlueCom();
      break;
    case 1:  // 1 - Scan for Bluetooth receiver devices");
      sendBlueCmds(cmdsScan, (uint8_t)NUMOF(cmdsScan));
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
