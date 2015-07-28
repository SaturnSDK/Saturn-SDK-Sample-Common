#ifndef __SSSDK_COMMON_SMPC_H__
#define __SSSDK_COMMON_SMPC_H__

#include <stdint.h>

#define SMPC_COMREG	( *( volatile uint8_t * )( 0x2010001F ) )
#define SMPC_SR		( *( volatile uint8_t * )( 0x20100061 ) )
#define SMPC_SF		( *( volatile uint8_t * )( 0x20100063 ) )

#define SMPC_IREG0	( *( volatile uint8_t * )( 0x20100001 ) )
#define SMPC_IREG1	( *( volatile uint8_t * )( 0x20100003 ) )
#define SMPC_IREG2	( *( volatile uint8_t * )( 0x20100005 ) )
#define SMPC_IREG3	( *( volatile uint8_t * )( 0x20100007 ) )
#define SMPC_IREG4	( *( volatile uint8_t * )( 0x20100009 ) )
#define SMPC_IREG5	( *( volatile uint8_t * )( 0x2010000B ) )
#define SMPC_IREG6	( *( volatile uint8_t * )( 0x2000000D ) )

#define SMPC_OREG0	( *( volatile uint8_t * )( 0x20100021 ) )
#define SMPC_OREG1	( *( volatile uint8_t * )( 0x20100023 ) )
#define SMPC_OREG2	( *( volatile uint8_t * )( 0x20100025 ) )
#define SMPC_OREG3	( *( volatile uint8_t * )( 0x20100027 ) )
#define SMPC_OREG4	( *( volatile uint8_t * )( 0x20100029 ) )
#define SMPC_OREG5	( *( volatile uint8_t * )( 0x2010002B ) )
#define SMPC_OREG6	( *( volatile uint8_t * )( 0x2010002D ) )
#define SMPC_OREG7	( *( volatile uint8_t * )( 0x2010002F ) )
#define SMPC_OREG8	( *( volatile uint8_t * )( 0x20100031 ) )
#define SMPC_OREG9	( *( volatile uint8_t * )( 0x20100033 ) )
#define SMPC_OREG10	( *( volatile uint8_t * )( 0x20100035 ) )
#define SMPC_OREG11	( *( volatile uint8_t * )( 0x20100037 ) )
#define SMPC_OREG12	( *( volatile uint8_t * )( 0x20100039 ) )
#define SMPC_OREG13	( *( volatile uint8_t * )( 0x2010003B ) )
#define SMPC_OREG14	( *( volatile uint8_t * )( 0x2010003D ) )
#define SMPC_OREG15	( *( volatile uint8_t * )( 0x2010003F ) )
#define SMPC_OREG16	( *( volatile uint8_t * )( 0x20100041 ) )
#define SMPC_OREG17	( *( volatile uint8_t * )( 0x20100043 ) )
#define SMPC_OREG18	( *( volatile uint8_t * )( 0x20100045 ) )
#define SMPC_OREG19	( *( volatile uint8_t * )( 0x20100047 ) )
#define SMPC_OREG20	( *( volatile uint8_t * )( 0x20100049 ) )
#define SMPC_OREG21	( *( volatile uint8_t * )( 0x2010004B ) )
#define SMPC_OREG22	( *( volatile uint8_t * )( 0x2010004D ) )
#define SMPC_OREG23	( *( volatile uint8_t * )( 0x2010004F ) )
#define SMPC_OREG24	( *( volatile uint8_t * )( 0x20100051 ) )
#define SMPC_OREG25	( *( volatile uint8_t * )( 0x20100053 ) )
#define SMPC_OREG26	( *( volatile uint8_t * )( 0x20100055 ) )
#define SMPC_OREG27	( *( volatile uint8_t * )( 0x20100057 ) )
#define SMPC_OREG28	( *( volatile uint8_t * )( 0x20100059 ) )
#define SMPC_OREG29	( *( volatile uint8_t * )( 0x2010005B ) )
#define SMPC_OREG30	( *( volatile uint8_t * )( 0x2010005D ) )
#define SMPC_OREG31	( *( volatile uint8_t * )( 0x2010005F ) )

#define SMPC_PDR1	( *( volatile uint8_t * )( 0x20100075 ) )
#define SMPC_PDR2	( *( volatile uint8_t * )( 0x20100077 ) )
#define SMPC_DDR1	( *( volatile uint8_t * )( 0x20100079 ) )
#define SMPC_DDR2	( *( volatile uint8_t * )( 0x2010007B ) )
#define SMPC_IOSEL	( *( volatile uint8_t * )( 0x2010007D ) )
#define SMPC_EXLE	( *( volatile uint8_t * )( 0x2010007F ) )

/* Resetable system management commands */
extern const uint8_t SMPC_COMMAND_MSHON;
extern const uint8_t SMPC_COMMAND_SSHON;
extern const uint8_t SMPC_COMMAND_SSHOFF;
extern const uint8_t SMPC_COMMAND_SNDON;
extern const uint8_t SMPC_COMMAND_SNDOFF;
extern const uint8_t SMPC_COMMAND_RESET;
extern const uint8_t SMPC_COMMAND_CLKCHG352;
extern const uint8_t SMPC_COMMAND_CLKCHG320;
extern const uint8_t SMPC_COMMAND_NMIREQ;
extern const uint8_t SMPC_COMMAND_RESETENAB;
extern const uint8_t SMPC_COMMAND_RESETDISA;

/* Non-resetable system management commands */
extern const uint8_t SMPC_COMMAND_INTBACK;
extern const uint8_t SMPC_COMMAND_SETSMEM;

/* RTC */
extern const uint8_t SMPC_COMMAND_SETTIME;

/* INTBACK */
extern const uint8_t SMPC_IREG0_PERONLY;
extern const uint8_t SMPC_IREG1_PEN;
extern const uint8_t SMPC_IREG2_INTBACK;
extern const uint8_t SMPC_IREG0_CONT;
extern const uint8_t SMPC_IREG0_BREAK;
extern const uint8_t SMPC_PORT1_15;
extern const uint8_t SMPC_PORT1_255;
extern const uint8_t SMPC_PORT1_0;
extern const uint8_t SMPC_PORT2_1;
extern const uint8_t SMPC_PORT2_255;
extern const uint8_t SMPC_PORT2_0;

#endif /* __SSSDK_COMMON_SMPC_H__ */

