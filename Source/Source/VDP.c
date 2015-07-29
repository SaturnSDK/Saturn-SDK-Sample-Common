#include <VDP.h>
#include <USBCart.h>

volatile uint16_t *VDP1_VRAM_PTR	= ( volatile uint16_t * )( VDP1_VRAM );

volatile uint16_t *VDP2_VRAM_PTR	= ( volatile uint16_t * )( VDP2_VRAM );
volatile uint16_t *VDP2_CRAM_PTR	= ( volatile uint16_t * )( VDP2_CRAM );
volatile uint16_t *VDP2_REG_PTR		= ( volatile uint16_t * )( VDP2_REG );

uint16_t VDP1_CommandListIndex = 0;


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

void VDP1_ClearCommandList( void )
{
	/* Just reset the command list offset */
	VDP1_CommandListIndex = 0;
}

void VDP1_EndCommandList( void )
{
	uint16_t *pListPtr =
		( uint16_t * )( VDP1_VRAM + ( VDP1_CommandListIndex << 5 ) );

	pListPtr[ VDP1_CMD_CTRL ] = 0x8000;
	++VDP1_CommandListIndex;
}

void VDP1_SetSystemClipCoordinates( int16_t p_X, int16_t p_Y )
{
	uint16_t *pListPtr =
		( uint16_t * )( VDP1_VRAM + ( VDP1_CommandListIndex << 5 ) );
	
	/* System clip coordinates */
	pListPtr[ VDP1_CMD_CTRL ] = 0x0009;
	/* Continue to the next command */
	pListPtr[ VDP1_CMD_LINK ] = 0x0000;
	/* Set the coordinates */
	pListPtr[ VDP1_CMD_XC ] = p_X;
	pListPtr[ VDP1_CMD_YC ] = p_Y;

	++VDP1_CommandListIndex;
}

void VDP1_SetUserClipCoordinates( int16_t p_X0, int16_t p_Y0,
	int16_t p_X1, int16_t p_Y1 )
{
	uint16_t *pListPtr =
		( uint16_t * )( VDP1_VRAM + ( VDP1_CommandListIndex << 5 ) );
	
	/* User clip coordinates */
	pListPtr[ VDP1_CMD_CTRL ] = 0x0008;

	pListPtr[ VDP1_CMD_LINK ] = 0x0000;
	pListPtr[ VDP1_CMD_XA ] = p_X0;
	pListPtr[ VDP1_CMD_YA ] = p_Y0;
	pListPtr[ VDP1_CMD_XC ] = p_X1;
	pListPtr[ VDP1_CMD_YC ] = p_Y1;

	++VDP1_CommandListIndex;
}

void VDP1_SetLocalCoordinates( int16_t p_X, int16_t p_Y )
{
	uint16_t *pListPtr =
		( uint16_t * )( VDP1_VRAM + ( VDP1_CommandListIndex << 5 ) );
	
	/* Local coordinates */
	pListPtr[ VDP1_CMD_CTRL ] = 0x000A;
	/* Continue to the next command */
	pListPtr[ VDP1_CMD_LINK ] = 0x0000;
	/* Set the local coordinates */
	pListPtr[ VDP1_CMD_XA ] = p_X;
	pListPtr[ VDP1_CMD_YA ] = p_Y;

	++VDP1_CommandListIndex;
}

void VDP1_DrawSpriteNormalRGB( int16_t p_X, int16_t p_Y,
	int16_t p_Width, int8_t p_Height, uint32_t p_Texture )
{
	uint16_t *pListPtr =
		( uint16_t * )( VDP1_VRAM + ( VDP1_CommandListIndex << 5 ) );

	/* Normal sprite */
	pListPtr[ VDP1_CMD_CTRL ] = 0x0000;
	/* Continue to the next command */
	pListPtr[ VDP1_CMD_LINK ] = 0x0000;
	/* Set color mode to RGB */
	pListPtr[ VDP1_CMD_PMOD ] = 0x00A8;
	/* Set the texture pointer */
	pListPtr[ VDP1_CMD_SRCA ] = p_Texture >> 3;
	/* The Width is divided by 8 before being set, height is allowed as-is
	 * Bits 15 and 14 are zero */
	pListPtr[ VDP1_CMD_SIZE ] =
		( ( ( p_Width >> 3 ) << 8 ) | ( p_Height & 0xFF ) ) & 0x3FFF;
	/* Set the position */
	pListPtr[ VDP1_CMD_XA ] = p_X;
	pListPtr[ VDP1_CMD_YA ] = p_Y;

	++VDP1_CommandListIndex;
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

