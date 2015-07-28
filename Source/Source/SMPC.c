#include <SMPC.h>

/* Resetable system management commands */
const uint8_t SMPC_COMMAND_MSHON		= 0x00;
const uint8_t SMPC_COMMAND_SSHON		= 0x02;
const uint8_t SMPC_COMMAND_SSHOFF		= 0x03;
const uint8_t SMPC_COMMAND_SNDON		= 0x06;
const uint8_t SMPC_COMMAND_SNDOFF		= 0x07;
const uint8_t SMPC_COMMAND_RESET		= 0x0D;
const uint8_t SMPC_COMMAND_CLKCHG352	= 0x0E;
const uint8_t SMPC_COMMAND_CLKCHG320	= 0x0F;
const uint8_t SMPC_COMMAND_NMIREQ		= 0x18;
const uint8_t SMPC_COMMAND_RESETENAB	= 0x19;
const uint8_t SMPC_COMMAND_RESETDISA	= 0x1A;

/* Non-resetable system management commands */
const uint8_t SMPC_COMMAND_INTBACK		= 0x10;
const uint8_t SMPC_COMMAND_SETSMEM		= 0x17;

/* RTC */
const uint8_t SMPC_COMMAND_SETTIME		= 0x16;

/* INTBACK */
const uint8_t SMPC_IREG0_PERONLY	= 0x00;
const uint8_t SMPC_IREG1_PEN		= 1 << 3;
const uint8_t SMPC_IREG2_INTBACK	= 0xF0;
const uint8_t SMPC_IREG0_CONT		= 1 << 7;
const uint8_t SMPC_IREG0_BREAK		= 1 << 6;
const uint8_t SMPC_PORT1_15			= 0xCF;
const uint8_t SMPC_PORT1_255		= 0xEF;
const uint8_t SMPC_PORT1_0			= 0xFF;
const uint8_t SMPC_PORT2_15			= 0x3F;
const uint8_t SMPC_PORT2_255		= 0x7F;
const uint8_t SMPC_PORT2_0			= 0xFF;

