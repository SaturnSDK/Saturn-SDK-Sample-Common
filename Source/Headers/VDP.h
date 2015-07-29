#ifndef __SSSDK_COMMON_VDP_H__
#define __SSSDK_COMMON_VDP_H__

#include <stdint.h>

/* VDP1 */
/* Base address of VDP1 */
#define VDP1_BASE	0x25C00000
#define VDP1_VRAM	VDP1_BASE
/* TV mode selection */
#define VDP1_TVMR	( *( volatile uint16_t * )( VDP1_BASE + 0x100000 ) )
/* Frame buffer switch mode */
#define VDP1_FBCR	( *( volatile uint16_t * )( VDP1_BASE + 0x100002 ) )
/* Plot trigger */
#define VDP1_PTMR	( *( volatile uint16_t * )( VDP1_BASE + 0x100004 ) )
/* Erase/Write data */
#define VDP1_EWDR	( *( volatile uint16_t * )( VDP1_BASE + 0x100006 ) )
/* Erase/Write upper-left coordinate */
#define VDP1_EWUL	( *( volatile uint16_t * )( VDP1_BASE + 0x100008 ) )
/* Erase/Write lower-right coordinate */
#define VDP1_EWLR	( *( volatile uint16_t * )( VDP1_BASE + 0x10000A ) )
/* Plot abnormal end */
#define VDP1_ENDR	( *( volatile uint16_t * )( VDP1_BASE + 0x10000C ) )
/* Transfer end status */
#define VDP1_EDSR	( *( volatile uint16_t * )( VDP1_BASE + 0x100010 ) )
/* Last operation command address */
#define VDP1_LOCR	( *( volatile uint16_t * )( VDP1_BASE + 0x100012 ) )
/* Current operation command address */
#define VDP1_COCR	( *( volatile uint16_t * )( VDP1_BASE + 0x100014 ) )
/* Mode status */
#define VDP1_MODR	( *( volatile uint16_t * )( VDP1_BASE + 0x100016 ) )

/* VDP1 command offets */
/* Control */
#define VDP1_CMD_CTRL	0x00
/* Link specification */
#define VDP1_CMD_LINK	0x01
/* Draw mode */
#define VDP1_CMD_PMOD	0x02
/* Color control */
#define VDP1_CMD_COLR	0x03
/* Character address */
#define VDP1_CMD_SRCA	0x04
/* Character size */
#define VDP1_CMD_SIZE	0x05
/* Vertex coordinate data XA-YD */
#define VDP1_CMD_XA		0x06
#define VDP1_CMD_YA		0x07
#define VDP1_CMD_XB		0x08
#define VDP1_CMD_YB		0x09
#define VDP1_CMD_XC		0x0A
#define VDP1_CMD_YC		0x0B
#define VDP1_CMD_XD		0x0C
#define VDP1_CMD_YD		0x0D
/* Gouraud shading table */
#define VDP1_CMD_GRDA	0x0E
/* Unused, dummy */
#define VDP1_CMD_DUMY	0x0F

/* VDP2 */
#define VDP2_BASE	0x25E00000
#define VDP2_VRAM	VDP2_BASE
#define VDP2_CRAM	VDP2_BASE + 0x100000
#define VDP2_REG	VDP2_BASE + 0x180000
#define VDP2_TVMD	( *( volatile uint16_t * )( 0x25F80000 ) )
#define VDP2_TVSTAT	( *( volatile uint16_t * )( 0x25F80004 ) )

/* Back screen table */
#define VDP2_BKTAU	( *( volatile uint16_t * )( VDP2_REG + 0xAC ) )

/* Sprite priority */
#define VDP2_PRISA	( *( volatile uint16_t * )( VDP2_REG + 0xF0 ) )
#define VDP2_PRISB	( *( volatile uint16_t * )( VDP2_REG + 0xF2 ) )
#define VDP2_PRISC	( *( volatile uint16_t * )( VDP2_REG + 0xF4 ) )
#define VDP2_PRISD	( *( volatile uint16_t * )( VDP2_REG + 0xF6 ) )

/* Background priority */
#define VDP2_PRINA	( *( volatile uint16_t * )( VDP2_REG + 0xF8 ) )
#define VDP2_PRIMB	( *( volatile uint16_t * )( VDP2_REG + 0xFA ) )

/* Pointer access to VDP1 addresses */
extern volatile uint16_t *VDP1_VRAM_PTR;

/* Pointer access to VDP2 addresses */
extern volatile uint16_t *VDP2_VRAM_PTR;
extern volatile uint16_t *VDP2_CRAM_PTR;
extern volatile uint16_t *VDP2_REG_PTR;

/* Initialise and shutdown */
void VDP_Initialize( void );
void VDP_Shutdown( void );

/* VDP1 functions */
/* Reset the command list */
void VDP1_ClearCommandList( void );
/* Signal the end of the command list */
void VDP1_EndCommandList( void );
/* Set the width and height of the clip coordinates */
void VDP1_SetSystemClipCoordinates( int16_t p_X, int16_t p_Y );
/* Set the upper-left and lower-right of the user clip coordinates */
void VDP1_SetUserClipCoordinates( int16_t p_X0, int16_t p_Y0,
	int16_t p_X1, int16_t p_Y1 );
/* Set up the local draw coordinates */
void VDP1_SetLocalCoordinates( int16_t p_X, int16_t p_Y );
/* Sprite drawing */
/* - Normal sprite */
void VDP1_DrawSpriteNormalRGB( int16_t p_X, int16_t p_Y,
	int16_t p_Width, int8_t p_Height, uint32_t p_Texture );

/* VDP2 functions */
void VDP2_SetBackgroundColor( uint8_t p_Red, uint8_t p_Green, uint8_t p_Blue );

/* Polling V-Blank and H-Blank */
void VDP_WaitHBlankOut( void );
void VDP_WaitHBlankIn( void );
void VDP_WaitVBlankOut( void );
void VDP_WaitVBlankIn( void );
void VDP_WaitHBlank( void );
void VDP_WaitVBlank( void );

/* Global variables */
/* Keep track of the command list offset */
extern uint16_t	VDP1_CommandListIndex;

#endif /* __SSSDK_COMMON_VDP_H__ */

