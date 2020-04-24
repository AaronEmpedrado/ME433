#include "mcp23017.h"

//TODO: write macros for address with write and read set
// Initialize the MCP23017
// Use default IOCON.BANK value of 0 (interleaved)
void mcp_init(void) {
    // Make all A pins be output
    mcp_write(MCP_ADDRESS, MCP_IODIRA, 0x00);
    // Make all B pins be input
    mcp_write(MCP_ADDRESS, MCP_IODIRB, 0xff);
}


// Write a certain value to a specific MCP register
// aka writing to MCP => start, address/write, register, value, stop
void mcp_write(unsigned char address, unsigned char reg, unsigned char value) {
    i2c_master_start();
    i2c_master_send(address & 0xfe);       // address of MCP23017, R/W = 0
    i2c_master_send(reg);
    i2c_master_send(value);
    i2c_master_stop();
}


// Read value from a MCP register
// => start, address/write, register, restart, address/read, recv, ack, stop
unsigned char mcp_read(unsigned char address, unsigned char reg) {
    i2c_master_start();
    i2c_master_send(address & 0xfe);        // address of MCP23017, R/W = 0
    i2c_master_send(reg);
    i2c_master_restart();
    i2c_master_send(address | 0x01);        // address of MCP23017, R/W = 1
    unsigned char read = i2c_master_recv();
    i2c_master_ack(1);                      // NACK => no more bytes requested
    i2c_master_stop();
    return read;
}

// Set a certain Output pin high on the MCP (only A pins and only one at a time)
void setPinA(unsigned char address, int pin) {
//    unsigned char curr = mcp_read(address, MCP_OLATA);
//    unsigned char val = curr | (0x01 << pin);
//    mcp_write(address, MCP_OLATA, val);
    mcp_write(address, MCP_OLATA, (0x01 << 7));    
}

// Clear a certain Output pin on the MCP (only A pins and only one at a time)
void clearPinA(unsigned char address, int pin) {
    unsigned char curr = mcp_read(address, MCP_OLATA);
    unsigned char val = curr & ~(0x01 << pin);
    mcp_write(address, MCP_OLATA, val);
}

// Read all the output B pins
unsigned char readPinB(unsigned char address) {
    return mcp_read(address, MCP_GPIOB);
}

