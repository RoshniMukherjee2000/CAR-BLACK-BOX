

#ifndef GLOBAL_H
#define	GLOBAL_H

//declaring global variables
unsigned char scroll_menu[5][20] = {"VIEW LOG", "SET TIME", "DOWNLOAD LOG", "CLEAR LOG"}; // Array of menu options for scrolling
unsigned char data_store[11] = {};                                                       // Array to store data for events
unsigned char *gear_labels[] = {"ON", "GN", "GR", "G1", "G2", "G3", "G4", "G5", "C "};  // Array of gear labels for display


unsigned char key;                                                       // Variable to store the key pressed from the keypad
unsigned char time[9];                                                  // Array to hold the current time as a string (HH:MM:SS)
int current_gear = 0, last_gear = 0, dashboard_scroll;                 // Variables to track the current gear, last gear, and dashboard scroll state
unsigned short int adc_reg_val;                                       // Variable to store the ADC value read from the sensor
unsigned int speed;                                                  // Variable to store the calculated speed
int address = 0x00;                                                 // EEPROM address for data storage
int overwrite_flag = 0;                                            // Flag to indicate if data should be overwritten
int event_count;                                                  // Counter for the number of events logged

unsigned int flag;                                              // General-purpose flag for various states

int star_position;                                            // Position of the selection indicator in the scrolling menu
int scrollpage;                                              // Current page of the scrolling menu
int menu_count;                                             // Current index of the selected menu item
int line_up_down;                                          // Variable to track scrolling direction in the menu

unsigned int hours, minutes, seconds;                    // Variables to hold the current time components
int setflag, set;                                       // Flags for setting time functionality


void storeEventToEEPROM(void);        // Function to store the current event data (time, speed, gear) in external EEPROM
void dashboard(void);                // Function to display the dashboard with current time, speed, and gear information
void scrollIt(void);                // Function to handle the scrolling menu for navigation through options
void view_log(void);               // Function to view the event log stored in EEPROM
void download_log(void);          // Function to download the event log data to the console or display
void set_time(void);             // Function to set the current time on the system using user input


// Buffer to store event log data, with a maximum of 10 entries, each 16 characters long (plus null terminator)
unsigned char log_buffer[10][17] = {}; 

// Index to track the current position in the log buffer for viewing logs
int index = 0; 

// Flag to indicate whether the log viewing mode is active (0: not viewing, 1: viewing)
int view = 0; 
int jump = 0;       // This variable is used to track the currently selected component of the time being set (hours, minutes, seconds).
int blink = 0;     // This variable is used to control the blinking of the currently selected time component on the display.



#endif	/* GLOBAL_H */

