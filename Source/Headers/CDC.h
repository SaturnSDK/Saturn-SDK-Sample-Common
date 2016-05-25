#ifndef __SSSDK_COMMON_CDCOM_H__
#define __SSSDK_COMMON_CDCOM_H__

#include <stdint.h>

#define CDC_HIRQ_CMOK	0x0001 /* Command can be issued */
#define CDC_HIRQ_DRDY	0x0002 /* Data transfer ready */
#define CDC_HIRQ_CSCT	0x0004 /* Sector read complete */
#define CDC_HIRQ_BFUL	0x0008 /* Buffer full */
#define CDC_HIRQ_PEND	0x0010 /* End of CD playback */
#define CDC_HIRQ_DCHG	0x0020 /* Disc change */
#define CDC_HIRQ_ESEL	0x0040 /* End of the selector setting process */
#define CDC_HIRQ_EHST	0x0080 /* Host I/O end */
#define CDC_HIRQ_ECPY	0x0100 /* End of the CD copy process */
#define CDC_HIRQ_EFLS	0x0200 /* End of the file system processing */

typedef enum
{
	CDC_STATUS_BUSY		= 0x00,
	CDC_STATUS_PAUSE	= 0x01,
	CDC_STATUS_STANDBY	= 0x02,
	CDC_STATUS_PLAY		= 0x03,
	CDC_STATUS_SEEK		= 0x04,
	CDC_STATUS_SCAN		= 0x05,
	CDC_STATUS_OPEN		= 0x06,
	CDC_STATUS_NODISC	= 0x07,
	CDC_STATUS_RETRY	= 0x08,
	CDC_STATUS_ERROR	= 0x09,
	CDC_STATUS_FATAL	= 0x0A,
	CDC_STATUS_PERIODIC	= 0x20,
	CDC_STATUS_TRANSFER	= 0x40,
	CDC_STATUS_WAIT		= 0x80,
	CDC_STATUS_REJECT	= 0xFF
}CDC_STATUS;

typedef enum
{
	CDC_ERROR_OK		= 0,
	CDC_ERROR_CMDBUSY	= -1,
	CDC_ERROR_AUTH		= -99,
	CDC_ERROR_UNKNOWN	= -100
}CDC_ERROR;

typedef enum
{
	CDC_SECTOR_LENGTH_2048		= 0,
	CDC_SECTOR_LENGTH_2336		= 1,
	CDC_SECTOR_LENGTH_2340		= 2,
	CDC_SECTOR_LENGTH_2352		= 3,
	CDC_SECTOR_LENGTH_NOCHANGE	= -1
}CDC_SECTOR_LENGTH;

typedef struct
{
	uint8_t	Status;
	uint8_t	Flags;
	uint8_t	Control;
	uint8_t	TNO;
	uint8_t	Index;
	uint8_t	RepeatCount;
	int32_t	FrameAddress;
}CDC_STAT,*PCDC_STAT;

CDC_ERROR CDC_CDInitialise( void );
CDC_ERROR CDC_CDSetSectorSize( CDC_SECTOR_LENGTH p_Size );
int CDC_CDIsDataReady( int p_SelectorNumber );
CDC_ERROR CDC_CDReadSector( void *p_pBuffer, unsigned long p_FrameAddress,
	int p_SectorSize, unsigned long p_ByteCount );
CDC_ERROR CDC_CDGetStat( PCDC_STAT p_pCDStat );
int CDC_CDIsAuthenticated( uint16_t *p_pDiscAuth );
CDC_ERROR CDC_CDAuthenticate( void );

#endif /* __SSSDK_COMMON_CDCOMM_H__ */

