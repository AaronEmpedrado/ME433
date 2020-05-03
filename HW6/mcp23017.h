#ifndef MCP_H__
#define MCP_H__
#include "i2c_master_noint.h"
// Header file for the MCP23017

// Macros
// [7:1] == Slave Address
// [3:1] == [A2,A1,A0]
// [0] == R/W => {0:write, 1:read}
#define MCP_ADDRESS 0b01000000
#define MCP_IODIRA  0x00
#define MCP_IODIRB  0x01
#define MCP_OLATA   0x14
#define MCP_GPIOB   0x13

void mcp_init(void);    // initialize the mcp
void mcp_write(unsigned char address, unsigned char reg, unsigned char value);
unsigned char mcp_read(unsigned char address, unsigned char reg);
void setPinA(unsigned char address, int pin);
void clearPinA(unsigned char address, int pin);
unsigned char readPinB(unsigned char address);
#endif
