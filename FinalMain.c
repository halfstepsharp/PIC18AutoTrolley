#include <xc.h>
#include <stdio.h>
#include <math.h>
#include "keypad.h"
#include "delays.h"
#include "lcd.h"
#include "music.h"
/*#define tag1 "1CC6A5FE"
#define tag2 "9CC5A5FE"
#define tag3 "ACBFA5FE"
#define tag4 "1CBEA5FE"
#define tag5 "1CBFA5FE"*/ // no longer used
#define cost1 20
#define cost2 35
#define cost3 61
#define cost4 99
#define cost5 74
#define UCpos 0x8D
#define LCpos 0xCD
#define Keyreset1 PORTBbits.RB5
#define Keyreset2 PORTB


unsigned char key, outchar;
int tno(void);
char NumToStr(int);
double SerialComms(void);
int checkno(char, char, char, char);
int displayItems(int);
int displayCost(int);

//global variables - same across all functions
int i, Tvalid;
double CompareCost;
double totalcost2 = 0;
int latestitem = 0, totalitems = 0;
double totalcost = 0;

void onetone(void) //buzzer tone 1
{
    unsigned int add;
    for (add = 0; add < 100; add++) {
        delay_us(NOTE_E3);
        PORTCbits.RC0 = !PORTCbits.RC0;
    }
}

void twotone(void) //buzzer tone 2
{
    unsigned int sub;
    for (sub = 0; sub < 100; sub++) {
        delay_us(NOTE_B4);
        PORTCbits.RC0 = !PORTCbits.RC0;
    }
}

int tno(void) { //trolley number checker
    unsigned char p1, p2, p3, p4;
    char MessageETN [ ] = "Enter trolley number"; // Defining a 20 char string
    char MessageCTN [ ] = "Starting trolley    ";
    char MessageERR1 [ ] = "Invalid trolley no. ";
    char MessageERR2 [ ] = "please enter again  ";

    lcd_write_cmd(0x80); // Move cursor to line 1 position 1
    for (i = 0; i < 20; i++) //for 20 char LCD module
    {
        outchar = MessageETN[i];
        lcd_write_data(outchar); // write character data to LCD
    }
    lcd_write_cmd(0xC0); // Move cursor to line 2 position 1
    key = getkey(); // waits and get an ascii key number when pressed
    p1 = key;
    lcd_write_data(key); //display on LCD

    key = getkey(); // waits and get an ascii key number when pressed
    p2 = key;
    lcd_write_data(key); //display on LCD

    key = getkey(); // waits and get an ascii key number when pressed
    p3 = key;
    lcd_write_data(key); //display on LCD

    key = getkey(); // waits and get an ascii key number when pressed
    p4 = key;
    lcd_write_data(key); //display on LCD

    if (checkno(p1, p2, p3, p4) == 1) { //IF trolley is valid. 
        lcd_write_cmd(0x80); // Move cursor to line 1 position 1
        for (i = 0; i < 20; i++) //for 20 char LCD module
        {
            outchar = MessageCTN[i];
            lcd_write_data(outchar); // write character data to LCD
        }
        Tvalid = 1;
    } else {
        lcd_write_cmd(0x80); // Move cursor to line 1 position 1
        for (i = 0; i < 20; i++) //for 20 char LCD module
        {
            outchar = MessageERR1[i];
            lcd_write_data(outchar); // write character data to LCD
        }
        lcd_write_cmd(0xC0); // Move cursor to line 2 position 1
        for (i = 0; i < 20; i++) //for 20 char LCD module
        {
            outchar = MessageERR2[i];
            lcd_write_data(outchar); // write character data to LCD
        }
        Tvalid = 0;
    }
    delay_ms(1000); // wait 1 second
    lcd_write_cmd(0x01); // 00000001 Clear Display instruction
}

int checkno(char p1, char p2, char p3, char p4) { //check trolley number
    int result = 0; //initialize trolley "validity"
    // if letter is present, trolley is INVALID
    if (p1 == 'A' || p1 == 'B' || p1 == 'C' || p1 == 'D' || p1 == 'E' || p1 == 'F')
        return 0;
    else if (p1 == '0')
        result = result + 1;
    else if (p1 == '1' || p1 == '2' || p1 == '3' || p1 == '4' || p1 == '5' || p1 == '6' || p1 == '7' || p1 == '8' || p1 == '9')
        result = result + 4;

    if (p2 == 'A' || p2 == 'B' || p2 == 'C' || p2 == 'D' || p2 == 'E' || p2 == 'F')
        return 0;
    else if (p2 == '0')
        result = result + 1;
    else if (p2 == '1' || p2 == '2' || p2 == '3' || p2 == '4' || p2 == '5' || p2 == '6' || p2 == '7' || p2 == '8' || p2 == '9')
        result = result + 4;

    if (p3 == 'A' || p3 == 'B' || p3 == 'C' || p3 == 'D' || p3 == 'E' || p3 == 'F')
        return 0;
    else if (p3 == '0')
        result = result + 1;
    else if (p3 == '1' || p3 == '2' || p3 == '3' || p3 == '4' || p3 == '5' || p3 == '6' || p3 == '7' || p3 == '8' || p3 == '9')
        result = result + 4;

    if (p4 == 'A' || p4 == 'B' || p4 == 'C' || p4 == 'D' || p4 == 'E' || p4 == 'F')
        return 0;
    else if (p4 == '0')
        result = result + 1;
    else if (p4 == '1' || p4 == '2' || p4 == '3' || p4 == '4' || p4 == '5' || p4 == '6' || p4 == '7' || p4 == '8' || p4 == '9')
        result = result + 4;

    if (result > 4) {
        return 1;
    } else return 0;
}

char int_to_char(unsigned char int1) {
    char char1;
    switch (int1) {
        case 0: char1 = '0';
            break;
        case 1: char1 = '1';
            break;
        case 2: char1 = '2';
            break;
        case 3: char1 = '3';
            break;
        case 4: char1 = '4';
            break;
        case 5: char1 = '5';
            break;
        case 6: char1 = '6';
            break;
        case 7: char1 = '7';
            break;
        case 8: char1 = '8';
            break;
        case 9: char1 = '9';
            break;
        default: char1 = '?';
    }
    return (char1);
}

unsigned char char_to_int(char char1) {
    unsigned char int1;
    switch (char1) {
        case '0': int1 = 0;
            break;
        case '1': int1 = 1;
            break;
        case '2': int1 = 2;
            break;
        case '3': int1 = 3;
            break;
        case '4': int1 = 4;
            break;
        case '5': int1 = 5;
            break;
        case '6': int1 = 6;
            break;
        case '7': int1 = 7;
            break;
        case '8': int1 = 8;
            break;
        case '9': int1 = 9;
            break;
        default: int1 = 0;
    }
    return (int1);
}

double SerialComms(void) {
    int count1 = 0, count2 = 0, count3 = 0, count4 = 0, count5 = 0;
    //char data_out[];
    int t = 0;
    int r = 0;
    char tEnd [ ] = "End Shopping?       ";
    char tEnd2 [ ] = "Yes: 1 No: 0        ";
    char thx [ ] = "Thx for Shopping  :)";
    char tItems [ ] = "Total items:        ";
    char tCost [ ] = "Total cost :        ";
    char press, press2;
    char data_in [ ] = "0";
    char Citems [ ] = "0"; //total items CHARACTER VALUE
    unsigned char P1, P2;


    t = 0; // t is index to array of data to be transmitted
    r = 0; // r is index to array for data received
    if (PIR1bits.RCIF == 1) // Check if received 1 byte in UART buffer
    {

        data_in [r] = RCREG; // Read 1 byte from UART receive buffer
        r++; //increment data array
        if (RCREG == '1') {
            totalcost = totalcost + cost1; //calculate total cost
            totalitems++; //increment total items occurrence
            count1++; // increment occurrence of item 1 
            latestitem = 1;
        } else if (RCREG == '2') {
            totalcost = totalcost + cost2; //calculate total cost
            totalitems++; //increment total items occurrence
            count1++; // increment occurrence of item 1 
            latestitem = 2;
        } else if (RCREG == '3') {
            totalcost = totalcost + cost3; //calculate total cost
            totalitems++; //increment total items occurrence
            count1++; // increment occurrence of item 1 
            latestitem = 3;
        } else if (RCREG == '4') {
            totalcost = totalcost + cost4; //calculate total cost
            totalitems++; //increment total items occurrence
            count1++; // increment occurrence of item 1 
            latestitem = 4;
        } else if (RCREG == '5') {
            totalcost = totalcost + cost5; //calculate total cost
            totalitems++; //increment total items occurrence
            count1++; // increment occurrence of item 1 
            latestitem = 5;
        }
        lcd_write_cmd(UCpos);
        displayItems(totalitems);
        lcd_write_cmd(LCpos);
        displayCost(totalcost); 
        /*if (totalitems > 999) {
            totalitems1000 = totalitems / 1000;
            totalitems = totalitems - (totalitems1000 * 1000);
        } else totalitems1000 = 0;

        if (totalitems > 99) {
            totalitems100 = totalitems / 100;
            totalitems = totalitems - (totalitems100 * 100);
        } else totalitems100 = 0;

        if (totalitems > 9) {
            totalitems10 = totalitems / 10;
            totalitems = totalitems - (totalitems10 * 10);
        } else totalitems10 = 0;*/

        totalcost2 = totalcost;

        /*if (PORTBbits.RB5 == 1 && PORTBbits.RB0 == 1 && PORTBbits.RB1 == 0 && PORTBbits.RB2 == 1 && PORTBbits.RB3 == 1) {
            delay_ms(1000); // wait 1 second
            lcd_write_cmd(0x01); // 00000001 Clear Display instruction
            for (i = 0; i < 20; i++) {
                lcd_write_data(0x80);
                outchar = tEnd[i];
                lcd_write_data(outchar);
            }
            for (i = 0; i < 20; i++) {
                lcd_write_cmd(0xC0);
                outchar = tEnd2[i];
                lcd_write_data(outchar);
            }
            press = getkey();
            P1 = press;
            if (P1 == '0') {//No is selected
                delay_ms(1000); // wait 1 second
                lcd_write_cmd(0x01); // 00000001 Clear Display instruction
                lcd_write_cmd(UCpos);
                lcd_write_data(int_to_char(totalitems1000));
                lcd_write_data(int_to_char(totalitems100));
                lcd_write_data(int_to_char(totalitems10));
                lcd_write_data(int_to_char(totalitems1));
                lcd_write_cmd(LCpos);
                lcd_write_data(int_to_char(totalcost1000));
                lcd_write_data(int_to_char(totalcost100));
                lcd_write_data(int_to_char(totalcost10));
                lcd_write_data('.');
                lcd_write_data(int_to_char(totalcost1));
                lcd_write_data('0');
            }
            if (P1 == '1') {
                delay_ms(1000); // wait 1 second
                lcd_write_cmd(0x01); // 00000001 Clear Display instruction
                for (i = 0; i < 20; i++) {
                    lcd_write_cmd(0x80);
                    outchar = thanks[i];
                    lcd_write_data(outchar);
                }
                for (i = 0; i < 20; i++) {
                    lcd_write_data(0xC0);
                    outchar = thanks2[i];
                    lcd_write_data(outchar);
                }
            }
        }*/
        /*press = getkey();
        P1 = press;
        if (P1 == '0')
        {
            for(i=0; i<20; i++){
                lcd_write_data(0x80);
                outchar = tEnd[i];
                lcd_write_data(outchar);
            }
            for(i=0; i<20; i++){
                lcd_write_cmd(0xC0);
                outchar = tEnd2[i];
                lcd_write_data(outchar);
            }
            press = getkey();
            P2 = press;
            if(P2 == '1'){
                for(i=0; i<20; i++){
                    delay_ms(1000); // wait 1 second
                    lcd_write_cmd(0x01); // 00000001 Clear Display instruction
                    lcd_write_cmd(0x80);
                    outchar = thx[i];
                    lcd_write_data(outchar);
                }
                for(i=0; i<20; i++){
                    lcd_write_data(0xC0);
                    outchar = thx2[i];
                    lcd_write_data(outchar);
                }
            }
            if(P2 == '0'){
                Tvalid = 0;
            }
        }*/
    }
    if (PIR1bits.RCIF == 0 && PORTBbits.RB5 == 1) { // if no serial input
        press = getkey();
        P1 = press;
        if (P1 == '0') {
            lcd_write_cmd(0x80); // Move cursor to line 1 position 1
            for (i = 0; i < 20; i++) //for 20 char LCD module
            {
                outchar = tEnd[i];
                lcd_write_data(outchar); // write character data to LCD
            }
            lcd_write_cmd(0xC0); // Move cursor to line 2 position 1
            for (i = 0; i < 20; i++) //for 20 char LCD module
            {
                outchar = tEnd2[i];
                lcd_write_data(outchar); // write character data to LCD
            }
            press2 = getkey();
            P2 = press2;
            if (P2 == '0') {
                delay_ms(3000); // wait 1 second
                lcd_write_cmd(0x80); // Move cursor to line 1 position 1
                for (i = 0; i < 20; i++) //for 20 char LCD module
                {
                    outchar = tItems[i];
                    lcd_write_data(outchar); // write character data to LCD
                }
                lcd_write_cmd(0xC0); // Move cursor to line 2 position 1
                for (i = 0; i < 20; i++) //for 20 char LCD module
                {
                    outchar = tCost[i];
                    lcd_write_data(outchar); // write character data to LCD
                }
                lcd_write_cmd(UCpos);
                displayItems(totalitems);
                lcd_write_cmd(LCpos);
                displayCost(totalcost);

            }
            if (P2 == '1') {
                delay_ms(3000); // wait 1 second
                lcd_write_cmd(0x01);
                lcd_write_cmd(0x80);
                for (i = 0; i < 20; i++) {
                    outchar = thx[i];
                    lcd_write_data(outchar);
                }
                delay_ms(5000);
                while (1);
            }
        }
    }
    // do other things here...

}

int displayItems(int Num1) {
    int totalitems1000 = 0, totalitems100 = 0, totalitems10 = 0, totalitems1 = 0;

    totalitems1 = Num1;
    totalitems1000 = totalitems1 / 1000;
    totalitems1 = totalitems1 - totalitems1000 * 1000;
    totalitems100 = totalitems1 / 100;
    totalitems1 = totalitems1 - totalitems100 * 100;
    totalitems10 = totalitems1 / 10;
    totalitems1 = totalitems1 - totalitems10 * 10;

    lcd_write_data(int_to_char(totalitems1000));
    lcd_write_data(int_to_char(totalitems100));
    lcd_write_data(int_to_char(totalitems10));
    lcd_write_data(int_to_char(totalitems1));

}

int displayCost(int Num2) {
    int totalcost1000 = 0, totalcost100 = 0, totalcost10 = 0, totalcost1 = 0;
    totalcost1 = Num2;

    totalcost1000 = totalcost1 / 1000;
    totalcost1 = totalcost1 - totalcost1000 * 1000;
    totalcost100 = totalcost1 / 100;
    totalcost1 = totalcost1 - totalcost100 * 100;
    totalcost10 = totalcost1 / 10;
    totalcost1 = totalcost1 - totalcost10 * 10;

    lcd_write_data(int_to_char(totalcost1000));
    lcd_write_data(int_to_char(totalcost100));
    lcd_write_data(int_to_char(totalcost10));
    lcd_write_data('.');
    lcd_write_data(int_to_char(totalcost1));
    lcd_write_data('0');
}
// ---- Main Program ---------------------------------------------------------------

void main(void) {
    lcd_init(); // Initialize LCD module
    //TRISCbits.TRISC6 = 0; NOT NEEDED // Configure Port C Pin 6 as a Digital Output (UART TX)
    TRISAbits.TRISA5 = 1;
    TRISCbits.TRISC0 = 0; //Sets RC0 as output for buzzer
    TRISCbits.TRISC7 = 1; // Configure Port C Pin 7 as a Digital Input pin (UART RX)
    RCSTA = 0x90; // Configure UART Receiver
    TXSTA = 0x20; // Configure UART Transmitter
    SPBRG = 77; // Configure UART Baud rate to 9600 bps
    char tItems [ ] = "Total items:        ";
    char tCost [ ] = "Total cost :        ";

    Tvalid = 0;
    while (Tvalid == 0) {
        tno();
    }
    if (Tvalid == 1) {
        lcd_write_cmd(0x80); // Move cursor to line 1 position 1
        for (i = 0; i < 20; i++) //for 20 char LCD module
        {
            outchar = tItems[i];
            lcd_write_data(outchar); // write character data to LCD
        }

        lcd_write_cmd(0xC0); // Move cursor to line 2 position 1
        for (i = 0; i < 20; i++) //for 20 char LCD module
        {
            outchar = tCost[i];
            lcd_write_data(outchar); // write character data to LCD
        }
        while (1) {
            CompareCost = totalcost2; //Sets CompareCost as the previous totalcost
            SerialComms();
            if (CompareCost > totalcost2) {
                onetone();
                delay_ms(10);
            } else if (CompareCost < totalcost2) {
                twotone();
                delay_ms(10);
            }
        }
    }



}