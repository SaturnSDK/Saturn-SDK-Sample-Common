#include <Peripheral.h>
#include <SMPC.h>

void PER_Initialize( void )
{
	SMPC_DDR1	= 0x60;
	SMPC_DDR2	= 0x60;
	SMPC_IOSEL	= 0x03;
	SMPC_EXLE	= 0x00;
}

void PER_Shutdown( void )
{
	SMPC_IOSEL	= 0x00;
}

uint16_t PER_Read( int p_Port )
{
	uint16_t Temp;
	int Count;

	SMPC_PDR1 = 0x60;
	for( Count = 0; Count < PER_DELAY; ++Count )
	{
		asm( "nop" );
	}
	Temp = ( SMPC_PDR1 & 0x08 ) << 12;

	SMPC_PDR1 = 0x40;
	for( Count = 0; Count < PER_DELAY; ++Count )
	{
		asm( "nop" );
	}
	Temp |= ( SMPC_PDR1 & 0x0F ) << 8;

	SMPC_PDR1 = 0x20;
	for( Count = 0; Count < PER_DELAY; ++Count )
	{
		asm( "nop" );
	}
	Temp |= ( SMPC_PDR1 & 0x0F ) << 4;

	SMPC_PDR1 = 0x00;
	for( Count = 0; Count < PER_DELAY; ++Count )
	{
		asm( "nop" );
	}
	Temp |= ( SMPC_PDR1 & 0x0F );

	return ( Temp ^ 0x8FFF );
}

