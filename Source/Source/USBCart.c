#include <USBCart.h>

void USB_Print( const char *p_pString )
{
#if defined BUILD_USBDEVCART
	char Print = ( *p_pString );

	while( Print != '\0' )
	{
		WAIT_FOR_WRITE_FIFO( );
		USB_FIFO = Print;

		Print = ( *++p_pString );
	}
#endif
}

