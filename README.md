# CAR-BLACK-BOX
# Introduction:
A Black Box is a device designed to record critical information leading up to a collision. The goal of this project is to implement the core functionalities of a car Black Box using a PIC-based microcontroller, supported by additional peripherals. Events will be logged in EEPROM, enabling the storage and retrieval of vital data.

The system will record details such as the car's speed and gear positions. Additionally, users can adjust the time settings as needed. Access to the recorded information is restricted to individuals who know the password, ensuring data security. The system also allows users to change the password if it is compromised, enhancing the overall security and usability of the device.

# Excution and Features Overview:
*Initial Setup:*
 Navigate to the CBB_EEPROM_Loader folder and run the Makefile to initialize the external EEPROM with default values.
 Next, run the Makefile in the CBB_Main folder to start executing the project on the PIC18F4580 microcontroller board. An onboard CLCD screen serves as an interactive 
 interface.
*Dashboard Interface:*
 Upon startup, a dashboard screen is displayed, showing the current time, the last recorded event, and the vehicle speed.
 Vehicle speed can be adjusted using the onboard potentiometer (POT).
The following Matrix keys are used for event registration:
 SW1: Registers a Gear UP event.
 SW2: Registers a Gear DOWN event.
 SW3: Registers a Collision event.
 Events are stored in the external EEPROM regardless of the current display mode. However, only certain events are shown on the dashboard. Other events can be viewed through 
 the "View Log" or "Download Log" options.
 Password Access and Security:
  View Log: Displays the last 10 events recorded in the data log stored in the external EEPROM.
  Clear Log: Clears the data log from the external EEPROM. After this action, no events will be visible in the View Log mode, and scrolling becomes ineffective.
  Download Log: Allows the data log to be downloaded via UART communication.
 The user must connect the microcontroller board to a PC using a serial terminal like Minicom or PuTTY.
 Upon long pressing the Download Log option, the data log is displayed on the serial monitor.
 Navigation:
  Use Matrix SW11 for scrolling up and returning to the Log Menu or Dashboard:
  Long press Matrix SW11 to return to the Log Menu from a specific option.
  Another long press returns the user to the Dashboard screen.
