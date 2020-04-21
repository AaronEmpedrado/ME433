#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include "spi.h"
#include "dac.h"

// DEVCFG0
#pragma config DEBUG = OFF // disable debugging
#pragma config JTAGEN = OFF // disable jtag
#pragma config ICESEL = ICS_PGx1 // use PGED1 and PGEC1
#pragma config PWP = OFF // disable flash write protect
#pragma config BWP = OFF // disable boot write protect
#pragma config CP = OFF // disable code protect

// DEVCFG1
#pragma config FNOSC = PRIPLL // use primary oscillator with pll
#pragma config FSOSCEN = OFF // disable secondary oscillator
#pragma config IESO = OFF // disable switching clocks
#pragma config POSCMOD = HS // high speed crystal mode
#pragma config OSCIOFNC = OFF // disable clock output
#pragma config FPBDIV = DIV_1 // divide sysclk freq by 1 for peripheral bus clock
#pragma config FCKSM = CSDCMD // disable clock switch and FSCM
#pragma config WDTPS = PS1048576  // use largest wdt
#pragma config WINDIS = OFF // use non-window mode wdt
#pragma config FWDTEN = OFF // wdt disabled
#pragma config FWDTWINSZ = WINSZ_25  // wdt window at 25%

// DEVCFG2 - get the sysclk clock to 48MHz from the 8MHz crystal
#pragma config FPLLIDIV = DIV_2 // divide input clock to be in range 4-5MHz (8MHZ / 2) = 4MHz
#pragma config FPLLMUL = MUL_24 // multiply clock after FPLLIDIV (96MHz)
#pragma config FPLLODIV = DIV_2 // divide clock after FPLLMUL to get 48MHz (48MHz)

// DEVCFG3
#pragma config USERID = 0xABE // some 16bit userid, doesn't matter what (my initials)
#pragma config PMDL1WAY = OFF // allow multiple reconfigurations
#pragma config IOL1WAY = OFF // allow multiple reconfigurations

// Delay for 1 seconds    #TODO: Abstract this to delay for any number of seconds
void delay(void) {
    _CP0_SET_COUNT(0);
    while (_CP0_GET_COUNT() < 240000) {     // idk why its 177k lmfao
        // Do nothing for 10ms (100Hz)
    }
}

int main() {

    __builtin_disable_interrupts(); // disable interrupts while initializing things

    // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);

    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;

    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;

    // disable JTAG to get pins back
    DDPCONbits.JTAGEN = 0;

    // do your TRIS and LAT commands here
    TRISBbits.TRISB4 = 1;       // Set B4 as input
    TRISAbits.TRISA4 = 0;       // Set A4 as output
    LATAbits.LATA4 = 0;         // Initially off
    
    // Homework 2 SPI
    initSPI();
    
    __builtin_enable_interrupts();
   
    // Create 2 arrays to represent 1 period of each wave (recall 12-bit resolution => 4096)
    unsigned short triangle[100];
    unsigned short sine[50];
    int i = 0;        // index for triangle wave @ 1Hz
    int j = 0;        // index for sine wave @ 2Hz

    // Populate triangle wave
    int ii;
    for (ii = 0; ii < 100; ii++) {
        if (ii < 50) {     // up
            triangle[ii] = (unsigned short)(4096/50 * ii);
        } else {           // down
            triangle[ii] = triangle[99-ii];     // Just go in reverse
        }
    }

    // Populate sine wave
    int jj;
    for (jj = 0; jj < 50; jj++) {
        sine[jj] = get_sine(jj);
    }


    while (1) {
        // Write the bytes to the DAC
        dac_write('a', triangle[i]);
        dac_write('b', sine[j]);

        i++;
        j++;
        // Reset indexing for triangle wave to start of period
        if (i == 100) {
            i = 0;
        }
        // Reset indexing for sine wave to start of period
        if (j == 50) {
            j = 0;
        }

        //DELAY => remember this runs at half the sysclock, so 24MHz
        delay();        // This is how fast the DAC updates(?) => Delay for 10ms
    }
}
