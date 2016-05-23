#ifndef __SSSDK_COMMON_INTERRUPT_H__
#define __SSSDK_COMMON_INTERRUPT_H__

#define INT_GetLevelMask( ) ( *( volatile uint32_t * )0x06000348 )
#define INT_SetLevelMask( p_Mask ) \
	( ( **( volatile void( ** )( uint32_t ) )0x6000340 )( p_Mask ) )

#endif /* __SSSDK_COMMON_INTERRUPT_H__ */
