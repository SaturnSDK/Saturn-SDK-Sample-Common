#ifndef __SSSDK_COMMON_PERIPHERAL_H__
#define __SSSDK_COMMON_PERIPHERAL_H__

#include <stdint.h>

#define PER_DELAY	16
#define PER_LBUTTON	( 1 << 15 )
#define PER_RBUTTON ( 1 << 3 )
#define PER_X		( 1 << 2 )
#define PER_Y		( 1 << 1 )
#define PER_Z		( 1 << 0 )
#define PER_START	( 1 << 11 )
#define PER_A		( 1 << 10 )
#define PER_B		( 1 << 8 )
#define PER_C		( 1 << 9 )
#define PER_RIGHT	( 1 << 7 )
#define PER_LEFT	( 1 << 6 )
#define PER_DOWN	( 1 << 5 )
#define PER_UP		( 1 << 4 )


void PER_Initialize( void );
void PER_Shutdown( void );
uint16_t PER_Read( int p_Index );

#endif /* __SSSDK_COMMON_PERIPHERAL_H__ */

