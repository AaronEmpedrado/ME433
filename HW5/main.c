#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
//#include "spi.h"
//#include "dac.h"
#include "i2c_master_noint.h"
#include "mcp23017.h"
#include "font.h"
#include "ssd1306.h"
#include <stdio.h>      // sprintf

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

// Delay for 1 seconds   (24M counts in 1 sec) 
void delay(void) {
    _CP0_SET_COUNT(0);
    while (_CP0_GET_COUNT() < 240000) {     
        // Do nothing for 10ms (100Hz)
    }
}

// Blink A4 every half second
void heartbeat(void) {
    LATAbits.LATA4 = 1;         // High
    /*
    // blink a pixel
    ssd1306_drawPixel(64,16,1);
    ssd1306_update();
    */
    _CP0_SET_COUNT(0);
    while (_CP0_GET_COUNT() < 48000000 / 2 / 2) {
    }
    LATAbits.LATA4 = 0;         // Low
    /*
    ssd1306_drawPixel(64,16,0);
    ssd1306_update();
    */
    _CP0_SET_COUNT(0);
    while (_CP0_GET_COUNT() < 48000000 / 2 / 2) {
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

    // Set A4 as output to blink for heartbeat
    TRISAbits.TRISA4 = 0;       // Set A4 as output
    LATAbits.LATA4 = 0;         // Initially off

    // Enable I2C1 (HW3)
    i2c_master_setup();     // enable i2c
    mcp_init();             // init mcp with B as inputs and A as outputs
    
    // Setup the LCD (HW4)
    ssd1306_setup();
    
    __builtin_enable_interrupts();
  
    char message[100];
    char msg[100];
    unsigned char x = 0;
    sprintf(msg, "hi");

    while (1) {
        _CP0_SET_COUNT(0);
        ssd1306_update();
        sprintf(message, "FPS: %d", (int)(24000000 / _CP0_GET_COUNT()));
        drawString(0,ROW_3,message);
        drawString(x, ROW_1, msg);
        x += 10;
        if (x > 128) {
            x = 0;
        }

        // Read pin B
        unsigned char bVals = readPinB(MCP_ADDRESS);
        // B0 is pull-up
        if (bVals & 0x1) {
            clearPinA(MCP_ADDRESS, 7);  // turn off if not pressed
        } else {
            setPinA(MCP_ADDRESS,7);     // turn on the yellow led
        }
        
        heartbeat();
        ssd1306_clear();
        ssd1306_update();
    }
}
