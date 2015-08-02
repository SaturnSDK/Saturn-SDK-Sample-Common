#ifndef __SSSDK_COMMON_PRINT_H__
#define __SSSDK_COMMON_PRINT_H__

#include <stdint.h>

extern uint16_t DBG_Palette[ 256 ];
extern uint8_t DBG_Font [ ];

void DBG_Initialize( void );
void DBG_PrintChar( int p_X, int p_Y, uint8_t p_Color, char p_Char );
void DBG_Print( int p_X, int p_Y, uint8_t p_Color, char *p_pString );
void DBG_ClearLine( int p_Line );


#endif /* __SSSDK_COMMON_PRINT_H__ */

