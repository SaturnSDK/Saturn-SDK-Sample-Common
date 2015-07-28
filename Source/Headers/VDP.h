#ifndef __SSSDK_COMMON_VDP_H__
#define __SSSDK_COMMON_VDP_H__

#include <stdint.h>

/* VDP1 */
/* Base address of VDP1 */
#define VDP1_BASE	0x25C00000
#define VDP1_VRAM	( *( volatile uint16_t * )( VDP1_BASE ) )
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

/* VDP2 */
#define VDP2_BASE	0x25E00000
#define VDP2_VRAM	VDP2_BASE
#define VDP2_CRAM	VDP2_BASE + 0x100000
#define VDP2_REG	VDP2_BASE + 0x180000
#define VDP2_TVMD	( *( volatile uint16_t * )( 0x25F80000 ) )
#define VDP2_TVSTAT	( *( volatile uint16_t * )( 0x25F80004 ) )

/* Pointer access to VDP2 addresses */
extern volatile uint16_t *VDP2_VRAM_PTR;
extern volatile uint16_t *VDP2_CRAM_PTR;
extern volatile uint16_t *VDP2_REG_PTR;

void VDP_Initialize( void );
void VDP_Shutdown( void );
void VDP2_SetBackgroundColor( uint8_t p_Red, uint8_t p_Green, uint8_t p_Blue );

/* Polling V-Blank and H-Blank */
void VDP_WaitHBlankOut( void );
void VDP_WaitHBlankIn( void );
void VDP_WaitVBlankOut( void );
void VDP_WaitVBlankIn( void );
void VDP_WaitHBlank( void );
void VDP_WaitVBlank( void );

#endif /* __SSSDK_COMMON_VDP_H__ */

