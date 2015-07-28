#include <VDP.h>

volatile uint16_t *VDP2_VRAM_PTR	= ( volatile uint16_t * )( VDP2_VRAM );
volatile uint16_t *VDP2_CRAM_PTR	= ( volatile uint16_t * )( VDP2_CRAM );
volatile uint16_t *VDP2_REG_PTR		= ( volatile uint16_t * )( VDP2_REG );


void VDP_Initialize( void )
{
	int Index;

	VDP2_TVMD = 0x0000;

	/* Set NBG0's color RAM mode to mode 0 */
	*( ( volatile uint16_t * )0x25F8000E ) =
		*( ( volatile uint16_t * ) 0x25F8000E ) & ( ~0x3000 );

	/* Set NBG0's map offset to zero */
	*( ( volatile uint16_t * )0x25F8003C ) = 0;

	/* Set up bitmap 256 color mode for NBG0 (1024x256) */
	*( ( volatile uint16_t * )0x25F80028 ) = 0x0012 | 8;

	/* Set the scroll screen direction */
	*( ( volatile uint16_t * )0x25F80070 ) = 0;
	*( ( volatile uint16_t * )0x25F80072 ) = 0;
	*( ( volatile uint16_t * )0x25F80074 ) = 0;
	
	/* Invalidate transparency for NBG0 */
	*( ( volatile uint16_t * )0x25F80020 ) = 0x0001;

	/* Clear VDP2 VRAM */
	for( Index = 0; Index < 0x40000; ++Index )
	{
		VDP2_VRAM_PTR[ Index ] = 0x0000;
	}

	/* Clear VDP2 CRAM */
	for( Index = 0; Index < 0x0800; ++Index )
	{
		VDP2_CRAM_PTR[ Index ] = 0x0000;
	}
}

void VDP_Shutdown( void )
{
	int Index;

	VDP2_TVMD = 0x0000;

	/* Reset VDP2 registers */
	for( Index = 0; Index < 0x100; ++Index )
	{
		VDP2_REG_PTR[ Index ] = 0x0000;
	}

	/* Clear VDP2 VRAM */
	for( Index = 0; Index < 0x40000; ++Index )
	{
		VDP2_VRAM_PTR[ Index ] = 0x0000;
	}

	/* Clear VDP2 CRAM */
	for( Index = 0; Index < 0x0800; ++Index )
	{
		VDP2_CRAM_PTR[ Index ] = 0x0000;
	}
}

void VDP2_SetBackgroundColor( uint8_t p_Red, uint8_t p_Green, uint8_t p_Blue )
{
	( *( volatile uint16_t * )( VDP2_BASE ) ) =
		( ( ( p_Red ) & 0x1F ) |
		( ( p_Green ) & 0x1F ) << 5 |
		( ( p_Blue ) & 0x1F ) << 10 |
		0x8000 );
}

void VDP_WaitHBlankOut( void )
{
	while( ( VDP2_TVSTAT & 4 ) == 4 );
}

void VDP_WaitHBlankIn( void )
{
	while( ( VDP2_TVSTAT & 4 ) == 0 );
}

void VDP_WaitVBlankOut( void )
{
	while( ( VDP2_TVSTAT & 8 ) == 8 );
}

void VDP_WaitVBlankIn( void )
{
	while( ( VDP2_TVSTAT & 8 ) == 0 );
}

void VDP_WaitHBlank( void )
{
	while( ( VDP2_TVSTAT & 4 ) == 0 );
	while( ( VDP2_TVSTAT & 4 ) == 4 );
}

void VDP_WaitVBlank( void )
{
	while( ( VDP2_TVSTAT & 8 ) == 0 );
	while( ( VDP2_TVSTAT & 8 ) == 8 );
}

