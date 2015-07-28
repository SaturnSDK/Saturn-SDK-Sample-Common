#ifndef __SSSDK_COMMON_USBCART_H__
#define __SSSDK_COMMON_USBCART_H__

#include <stdint.h>

#define USB_FIFO	( *( volatile uint8_t * )( 0x22100001 ) )
#define USB_FLAGS	( *( volatile uint8_t * )( 0x22200001 ) )
#define USB_TXE		( 1 << 1 )
#define WAIT_FOR_WRITE_FIFO( ) do { while( ( USB_FLAGS & USB_TXE ) ); }while( 0 )

void USB_Print( const char *p_pString );

#endif /* __SSSDK_COMMON_USBCART_H__ */

