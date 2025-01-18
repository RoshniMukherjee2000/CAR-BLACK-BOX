#include "header_file.h"
#include <xc.h>
#include "global.h"

// Function to initialize hardware configurations
void initHardware(void)
{
    init_clcd();           // Initialize the character LCD for display functions
    init_matrix_keypad(); // Initialize the matrix keypad for user input
    init_adc();          // Initialize the Analog-to-Digital Converter for reading analog signals
    init_i2c();         // Initialize the I2C interface for communication with peripheral devices
    init_ds1307();     // Initialize the DS1307 Real-Time Clock for timekeeping
    init_uart();      // Initialize the UART for serial communication

    clcd_print("  TIME   EV   SP", LINE1(0));   // Display the initial message on the LCD indicating time, event, and speed
}

// Main Function
void main(void)
{
    initHardware();  // Call initHardware to initialize all hardware settings
    
    // Main loop, constantly running
    while(1) 
    {
        if (dashboard_scroll == 0) 
        {
            dashboard(); // Display the dashboard if dashboard_scroll is 0
        } 
        else if (dashboard_scroll == 1) 
        {
            scrollIt(); // Handle scrolling of the menu if dashboard_scroll is 1
        }
    }
}



void dashboard() 
{
    
    get_time();      // Get current time
    display_time(); // Display time on LCD
    
    adc_reg_val = read_adc(CHANNEL4);   // Read ADC value from channel 4
    speed = (adc_reg_val / 10);        // Calculate speed
    
    // Limit speed to 99
    if (speed > 99) 
    {
        speed = 99;
    }
    
    key = read_switches(STATE_CHANGE);             // Read the key press from the matrix keypad

    // Gear control logic based on key presses
    if (current_gear < 7 && (key == MK_SW1)) 
    {
        current_gear++;                       // Increase gear       
    } 
    else if ((current_gear > 1 && current_gear < 8) && (key == MK_SW2))
    {
        current_gear--;                   // Decrease gear       
    }
    else if (key == MK_SW3)
    {
        current_gear = 8;             // Set gear to collision mode
    }
    else if (current_gear == 8 && (key == MK_SW1 || key == MK_SW2)) 
    {
        current_gear = 1;         // Set gear to neutral
    }
    else if (key == MK_SW11) 
    {
        dashboard_scroll = 1;  // Enable scrolling menu
    }
    
    
    
    // Update gear and speed on the LCD
    clcd_print(gear_labels[current_gear], LINE2(9));   // Display current gear
    clcd_putch((speed / 10) + '0', LINE2(14));
    clcd_putch((speed % 10) + '0', LINE2(15));

    // Store event data if the gear has changed
    if (last_gear != current_gear)
    {
        last_gear = current_gear;
        storeEventToEEPROM();                  // Store current data in External EEPROM
    }

    // Prepare for scrolling if the dashboard_scroll flag is set
    if (dashboard_scroll == 1)
    {
        star_position = 0;
        scrollpage = 0;
        menu_count = 0;
        line_up_down = 0;
        CLEAR_DISP_SCREEN;   // Clear the display screen
    }
}





void storeEventToEEPROM() // Function to store data in EEPROM
{
    // Store current time, speed, and gear information in data_store
   
    
   
    
    data_store[0] = time[0];        // Hour tens
    data_store[1] = time[1];       // Hour units
    data_store[2] = time[3];      // Minute tens
    data_store[3] = time[4];     // Minute units
    data_store[4] = time[6];    // Second tens
    data_store[5] = time[7];   // Second units
    
    data_store[6] = gear_labels[current_gear][0];        // Gear first character
    data_store[7] = gear_labels[current_gear][1];       // Gear second character
    data_store[8] = (speed / 10) + '0';                // Speed tens
    data_store[9] = (speed % 10) + '0';               // Speed units
    data_store[10] = '\0';                           // Null-terminate the string

    // Increment event count and manage EEPROM storage
    if (++event_count > 10) 
    {
        event_count = 0;   // Cap event count at 10
        address = 0x00;   // Reset address for EEPROM writing
        
         //Write existing data from EEPROM back to EEPROM
        for (int a = 0; a < 9; a++) 
        {          
            for (int b = 0; b < 10; b++)
            {
                write_external_eeprom(address++, read_external_eeprom((a + 1) * 10 + b));
            }
        }
    }

    // Write the current data_store values to EEPROM
    int i = 0;
    while (data_store[i])
    {
        write_external_eeprom(address++, data_store[i++]); // Write each character to EEPROM
    }
}




void scrollIt()  // Scrolling menu function for navigation
{
    if (flag == 0) 
    {
        key = read_switches(STATE_CHANGE);   // Read key press
        
        if (key == MK_SW1 || key == MK_SW2 || key == MK_SW11)
            CLEAR_DISP_SCREEN;
        
        // Handle various menu actions based on key press and current count
        if (key == MK_SW11 && menu_count == 0)
        {
            //CLEAR_DISP_SCREEN;
            flag = 1;  // Go to view log screen
        }
        else if(key == MK_SW11 &&  menu_count == 1)
        {
            setflag=1;  // Set flag for setting time
            flag=2;    //Go to set time screen       
        }
        else if (key == MK_SW11 && menu_count == 2)
        {
            flag = 4;  // Go to download log screen
        }
        else if (key == MK_SW11 && menu_count == 3)
        {
            // Clear logs
            clcd_print("WAIT....", LINE1(1));
            clcd_print("Clearing Data..", LINE2(0));
            for (unsigned long int i = 500000; i--;);
            //reset to menu
            event_count = 0;
            star_position = 0;
            scrollpage = 0;
            menu_count = 0;
            line_up_down = 0;
            address = 0x00;
            CLEAR_DISP_SCREEN;
        }
        // Handle scrolling through menu options
        if (key == MK_SW2)
        {
            if (star_position == 0 && line_up_down == 0) 
            {
                star_position = 1;//increment star
                line_up_down = 1;
                menu_count++;
            } 
            else if (star_position == 1 && scrollpage < 2)
            {
                scrollpage++;//increment scroll page
                menu_count++;
            }
        } 
        else if (key == MK_SW1) 
        {
            if (star_position == 1 && line_up_down == 1)
            {
                star_position = 0;
                line_up_down = 0;
                menu_count--;
            } 
            else if (star_position == 0 && scrollpage > 0) 
            {
                menu_count--;
                scrollpage--;
            }
        } 
        else if (key == MK_SW12)
        {
            dashboard_scroll = 0;  // Exit scrolling mode
        }

        // Display appropriate menu options based on scroll_page
        if (scrollpage == 0) 
        {
            clcd_print(scroll_menu[0], LINE1(1));
            clcd_print(scroll_menu[1], LINE2(1));
        } 
        else if (scrollpage == 1) 
        {
            clcd_print(scroll_menu[1], LINE1(1));
            clcd_print(scroll_menu[2], LINE2(1));
        } 
        else if (scrollpage == 2)
        {
            clcd_print(scroll_menu[2], LINE1(1));
            clcd_print(scroll_menu[3], LINE2(1));
        }

        // Indicate the currently selected option
        if (star_position == 0)
        {
            clcd_print("*", LINE1(0));
        } 
        else 
        {
            clcd_print("*", LINE2(0));
        }

        if (dashboard_scroll == 0)
        {
            CLEAR_DISP_SCREEN;
            clcd_print("  TIME   EV   SP  ", LINE1(0));
        }
 
        if (flag != 0)
        {
            CLEAR_DISP_SCREEN;
        }
    } 
    else if(flag == 1)
    {
        view_log();   //goto view_log function if flag=1
    }
    else if(flag == 2)
    {
        set_time();   //goto set time function if flag=2
    }
    else if(flag == 4)
    {
        download_log();   //goto download log function if flag=4
    }
}



//int fool = event_count;

void view_log() // Function to view the event log stored in EEPROM
{
    // Check if there are any events to display
    if (event_count == 0) 
    {
        // If no events are available, display a message
        clcd_print("NO LOGS", LINE1(0));
        clcd_print("AVAILABLE", LINE2(5));
        
        // Delay to allow the user to read the message (non-blocking)
        for (unsigned long int i = 500000; i--;);
        
        // Reset various flags and variables to prepare for the next action
        flag = 0;              // Reset the action flag
        star_position = 0;    // Reset the star position indicator
        scrollpage = 0;      // Reset the scroll page
        menu_count = 0;     // Reset the menu count
        line_up_down = 0;  // Reset line navigation
        CLEAR_DISP_SCREEN;// Clear the display
    }
    else 
    {       
    // If viewing hasn't been initiated, display header and read data
    if (view == 0) 
    {
        // Display the header for the log data
        clcd_print("#   TIME   EV SP", LINE1(0));
        view = 1;             // Set the flag to indicate that logs are being viewed
        int address = 0x00;  // Initialize the address for reading from EEPROM
        
        // Loop through each event to read and format the log data
        for (int i = 0; i < event_count; i++)
        {
            // Loop through each character position in the log buffer
            for (int j = 0; j < 17; j++)
            {
                // Populate the event data fields based on the index
                if (j == 0) 
                {
                    log_buffer[i][j] = i + '0';   // Store event index as a character
                }
                else if (j == 1 || j == 10 || j == 13) 
                {
                    log_buffer[i][j] = ' ';    // Add space for formatting
                }
                else if (j == 4 || j == 7) 
                {
                    log_buffer[i][j] = ':';   // Add time separator
                }
                else if (j == 16) 
                {
                    log_buffer[i][j] = '\0';  // Null-terminate the string
                }
                else 
                {
                    // Read actual data from external EEPROM memory
                    log_buffer[i][j] = read_external_eeprom(address++); // Store data in buffer
                }
            }
        }
    }
         // Display the event data on the LCD screen
        clcd_print(log_buffer[index], LINE2(0)); // Show the current event log entry on the second line of the LCD

        // Check for user input and navigate between events
        key = read_switches(STATE_CHANGE); // Read the state of the switches to detect user input

        // If the user presses the button for the next event and there are more events to show
        if (key == MK_SW2 && index < (event_count - 1)) // Next event
        {
            index++; // Increment the index to show the next event
        }
        // If the user presses the button for the previous event and the index is greater than 0
        else if (key == MK_SW1 && index > 0) // Previous event
        {
            index--; // Decrement the index to show the previous event
        } 
        // If the user presses the button to reset the log viewing
        else if (key == MK_SW12) // Reset the log viewing when SW12 is pressed 
        {
            CLEAR_DISP_SCREEN;          // Clear the display
            flag = 0;                  // Reset the action flag
            view = 0;                 // Set view flag to indicate not viewing logs
            index = 0;               // Reset the index to the first event
            star_position = 0;      // Reset the star position indicator
            scrollpage = 0;        // Reset the scroll page
            menu_count = 0;       // Reset the menu count
            line_up_down = 0;    // Reset line navigation
    
        }
    }
}


void download_log() // Function to download the event log data
{
    // Check if there are any events to download
    if (event_count == 0)  // If no events are available
    {
        // Display message indicating no data available
        puts("No data available \n\r");
        
        // Show a message on the LCD indicating no logs to download
        clcd_print("No Logs to ", LINE1(0));
        clcd_print("Download here...", LINE2(0));
        
        // Delay to allow the user to read the message (non-blocking)
        for (unsigned long int i = 500000; i--;);
        
        // Reset flags and variables for the next action
        flag = 0;                   // Reset the action flag
        star_position = 0;         // Reset the star position indicator
        scrollpage = 0;           // Reset the scroll page
        menu_count = 0;          // Reset the menu count
        line_up_down = 0;       // Reset line navigation
        CLEAR_DISP_SCREEN;     // Clear the display
    }
    else
    {
        // If viewing hasn't been initiated, show download status
        if (view == 0)
        {
            // Display downloading message on the LCD
            clcd_print("DOWNLOADINGG..", LINE1(0));
            for (unsigned long int i = 500000; i--;);  // Delay for user to see the message
            CLEAR_DISP_SCREEN;                        // Clear the display
            
            // Print the header for the log data
            puts("#   TIME   EV SP");
            puts("\n\r");
            
            view = 1;              // Set view flag to indicate logs are being shown
            int address = 0x00;   // Initialize the address for reading from EEPROM
            
            // Loop through each event to read and format the log data
            for (int i = 0; i < event_count; i++)
            {
                // Loop through each character position in the log buffer
                for (int j = 0; j < 17; j++)
                {
                    // Format the event log data based on the index
                    if (j == 0)
                    {
                        log_buffer[i][j] = i + '0';             // Store event ID as a character
                    }
                    else if (j == 1 || j == 10 || j == 13)
                    {
                        log_buffer[i][j] = ' ';             // Add space for formatting
                    }
                    else if (j == 4 || j == 7)
                    {
                        log_buffer[i][j] = ':';         // Add time separator
                    }
                    else if (j == 16)
                    {
                        log_buffer[i][j] = '\0';    // Null-terminate the string
                    }
                    else
                    {
                                                 // Read the event data from EEPROM memory
                      log_buffer[i][j] = read_external_eeprom(address++); // Store data in buffer
                    }                        
                }
            }

            // Print all events to the console
            int i = 0; // Initialize index for printing logs
            while (i < event_count)
            {
                puts(log_buffer[i++]); // Print each log entry
                puts("\n\r"); // New line after each entry
            }

            // Reset flags and variables after downloading
            flag = 0;              // Reset the action flag
            view = 0;             // Set view flag to indicate not viewing logs anymore
            star_position = 0;   // Reset the star position indicator
            scrollpage = 0;     // Reset the scroll page
            menu_count = 0;    // Reset the menu count
            line_up_down = 0; // Reset line navigation
        }
    }
}








void set_time() // Function to set the current time on the system
{
    // Check if the set flag is active, indicating that the user is ready to set the time
    if (setflag == 1)
    {
        // When the set flag is active, convert the time input into integers
        setflag = 0;                       // Reset the set flag
        // Set hours from the input
        hours = (time[0] - 48) * 10;     // Convert the first character to tens place
        hours = hours + (time[1] - 48); // Add the units place
        // Set minutes from the input
        minutes = (time[3] - 48) * 10;       // Convert the third character to tens place
        minutes = minutes + (time[4] - 48); // Add the units place
        // Set seconds from the input
        seconds = (time[6] - 48) * 10;       // Convert the sixth character to tens place
        seconds = seconds + (time[7] - 48); // Add the units place
        // Display "HH:MM:SS" prompt on the screen
        clcd_print("HH:MM:SS", LINE1(0));
    }

    // Read the state of the switches to detect user input
    key = read_switches(STATE_CHANGE);
    
    blink = !blink; // Toggle blink status for input display

    // Navigate through the time components using SW2
    if (key == MK_SW2)
    {
        if (jump >= 0 && jump < 2)
        {
            jump++; // Move to the next time component 
        }
        else if (jump >= 2)
        {
            jump = 0; // Reset to hours if beyond seconds
        }
    }
    
    // Adjust the current time value using SW1
    else if (key == MK_SW1)
    {
        // Increment the selected time component based on the value of jump
        if (jump == 0) // If hours is selected
        {
            if (hours < 23)
            {
                hours++; // Increment hours
            }
            else
            {
                hours = 0; // Reset hours if they exceed 23
            }
        }
        else if (jump == 1) // If minutes is selected
        {
            if ( minutes < 59)
            {
                minutes++; // Increment minutes
            }
            else
            {
                minutes = 0; // Reset minutes if they exceed 59
            }
        }
        else if (jump == 2) // If seconds is selected
        {
            if (seconds < 59)
            {
                seconds++; // Increment seconds
            }
            else
            {
                seconds = 0; // Reset seconds if they exceed 59
            }
        }
    }
    
    // Update the LCD screen with the current time
    clcd_putch('0' + (hours / 10), LINE2(0));  // Tens place of hours
    clcd_putch('0' + (hours % 10), LINE2(1)); // Units place of hours
    clcd_putch(':', LINE2(2)); // Separator
    clcd_putch('0' + (minutes / 10), LINE2(3));  // Tens place of minutes
    clcd_putch('0' + (minutes % 10), LINE2(4)); // Units place of minutes
    clcd_putch(':', LINE2(5)); // Separator
    clcd_putch('0' + (seconds / 10), LINE2(6));  // Tens place of seconds
    clcd_putch('0' + (seconds % 10), LINE2(7)); // Units place of seconds
    clcd_print("        ", LINE2(8)); // Clear the rest of the line

    // Blink the selected time component for user feedback
    if (jump == 0) // If hours is selected
    {
        if (blink)
        {
            clcd_putch(' ', LINE2(0));  // Hide hours if blinking
            clcd_putch(' ', LINE2(1)); // Hide hours if blinking
        }
    }
    else if (jump == 1) // If minutes is selected
    {
        if (blink)
        {
            clcd_putch(' ', LINE2(3));  // Hide minutes if blinking
            clcd_putch(' ', LINE2(4)); // Hide minutes if blinking
        }
    }
    else if (jump == 2) // If seconds is selected
    {
        if (blink)
        {
            clcd_putch (' ', LINE2(6)); // Hide seconds if blinking
            clcd_putch(' ', LINE2(7)); // Hide seconds if blinking
        }
    }

    // Non-blocking delay to control the blink rate
    for (unsigned long int i = 70000; i--;); 

    // Save the set time when SW11 is pressed
    if (key == MK_SW11)
    {
        // Write the time to the DS1307 RTC
        write_ds1307(HOUR_ADDR, (hours / 10) << 4 | (hours % 10));      // Write hours
        write_ds1307(MIN_ADDR, (minutes / 10) << 4 | (minutes % 10));  // Write minutes
        write_ds1307(SEC_ADDR, (seconds / 10) << 4 | (seconds % 10)); // Write seconds
        
        // Reset flags and variables after saving
        dashboard_scroll = 0;                         // Reset dashboard scroll
        flag = 0;                                    // Reset flag
        jump = 0;                                   // Reset jump index
        CLEAR_DISP_SCREEN;                         // Clear the display
        clcd_print("  TIME   EV   SP", LINE1(0)); // Display main menu
    }
    
    // Reset to the main screen when SW12 is pressed
    if (key == MK_SW12)
    {
        jump = 0;               // Reset jump index
        CLEAR_DISP_SCREEN;     // Clear the display
        flag = 0;             // Reset flag
        star_position = 0;   // Reset star position
        scrollpage = 0;     // Reset scroll page
        menu_count = 0;    // Reset menu count
        line_up_down = 0; // Reset line up/down
    }  
}