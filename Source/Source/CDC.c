#include <CDC.h>
#include <Interrupt.h>

#define CD_REG_CR1 *( ( volatile uint16_t * )0x25890018 )
#define CD_REG_CR2 *( ( volatile uint16_t * )0x2589001C )
#define CD_REG_CR3 *( ( volatile uint16_t * )0x25890020 )
#define CD_REG_CR4 *( ( volatile uint16_t * )0x25890024 )
#define CDB_REG_HIRQ *( ( volatile uint16_t * )0x25890008 )
/* Unsure if this is the correct register */
#define CDB_REG_DATATRNS *( ( volatile uint16_t * )0x25818000 )

static CDC_SECTOR_LENGTH CD_SECTOR_SIZE = CDC_SECTOR_LENGTH_2048;
static int SECTOR_SIZE_TABLE[ 4 ] =
{
	2048,
	2336,
	2340,
	2352
};

typedef struct
{
	uint16_t	CR1;
	uint16_t	CR2;
	uint16_t	CR3;
	uint16_t	CR4;
}CD_CMD_REGS,*PCD_CMD_REGS;

void CDC_CDWriteCommand( PCD_CMD_REGS p_pCDRegs )
{
	CD_REG_CR1 = p_pCDRegs->CR1;
	CD_REG_CR2 = p_pCDRegs->CR2;
	CD_REG_CR3 = p_pCDRegs->CR3;
	CD_REG_CR4 = p_pCDRegs->CR4;
}

void CDC_CDReadReturnStatus( PCD_CMD_REGS p_pCDRegs )
{
	p_pCDRegs->CR1 = CD_REG_CR1;
	p_pCDRegs->CR2 = CD_REG_CR2;
	p_pCDRegs->CR3 = CD_REG_CR3;
	p_pCDRegs->CR4 = CD_REG_CR4;
}

CDC_ERROR CDC_CDExecCommand( uint16_t p_HIRQMask, PCD_CMD_REGS p_pCDCommand,
	PCD_CMD_REGS p_pCDCommandReturn )
{
	int OldLevelMask;
	uint16_t HIRQ;
	CDC_STATUS CDStatus;
	int Index;

	OldLevelMask = INT_GetLevelMask( );
	INT_SetLevelMask( 0xF );

	HIRQ = CDB_REG_HIRQ;

	/* Okay to send commands? */
	if( !( HIRQ & CDC_HIRQ_CMOK ) )
	{
		return CDC_ERROR_CMDBUSY;
	}

	/* Clear CMOK and user-defined flags */
	CDB_REG_HIRQ = ~( p_HIRQMask | CDC_HIRQ_CMOK );

	CDC_CDWriteCommand( p_pCDCommand );

	/* Wait until the command has finished executing */
	for( Index = 0; Index < 0x240000; ++Index )
	{
		HIRQ = CDB_REG_HIRQ;

		if( HIRQ & CDC_HIRQ_CMOK )
		{
			break;
		}
	}

	if( !( HIRQ & CDC_HIRQ_CMOK ) )
	{
		return CDC_ERROR_CMDBUSY;
	}

	CDC_CDReadReturnStatus( p_pCDCommandReturn );

	CDStatus = p_pCDCommandReturn->CR1 >> 8;

	if( CDStatus == CDC_STATUS_REJECT )
	{
		return CDC_ERROR_CMDBUSY;
	}
	else if( CDStatus & CDC_STATUS_WAIT )
	{
		return CDC_ERROR_CMDBUSY;
	}

	INT_SetLevelMask( OldLevelMask );

	return CDC_ERROR_OK;
}

CDC_ERROR CDC_CDAbortFile( void )
{
	CD_CMD_REGS CDCommand, CDCommandReturn;

	CDCommand.CR1 = 0x7500;
	CDCommand.CR2 = 0x0000;
	CDCommand.CR3 = 0x0000;
	CDCommand.CR4 = 0x0000;

	return CDC_CDExecCommand( CDC_HIRQ_EFLS, &CDCommand, &CDCommandReturn );
}

CDC_ERROR CDC_CDBlockInitialise( uint16_t p_StandBy )
{
	CD_CMD_REGS CDCommand, CDCommandReturn;

	CDCommand.CR1 = 0x0400;
	CDCommand.CR2 = p_StandBy;
	CDCommand.CR3 = 0x0000;
	CDCommand.CR4 = 0x0000;

	return CDC_CDExecCommand( 0, &CDCommand, &CDCommandReturn );
}

/* Stop memory transfer between the CD block and the SH-2 RAM */
CDC_ERROR CDC_CDEndTransfer( void )
{
	CDC_ERROR Return;
	CD_CMD_REGS CDCommand, CDCommandReturn;

	CDCommand.CR1 = 0x0600;
	CDCommand.CR2 = 0x0000;
	CDCommand.CR3 = 0x0000;
	CDCommand.CR4 = 0x0000;

	Return = CDC_CDExecCommand( 0, &CDCommand, &CDCommandReturn );

	CDB_REG_HIRQ = ( ~CDC_HIRQ_DRDY ) | CDC_HIRQ_CMOK;

	return Return;
}

CDC_ERROR CDC_CDResetSelector( int p_ResetFlags, int p_SelectorNumber )
{
	CDC_ERROR Return;
	CD_CMD_REGS CDCommand, CDCommandReturn;

	CDCommand.CR1 = 0x4800 | ( ( uint8_t )p_ResetFlags );
	CDCommand.CR2 = 0x0000;
	CDCommand.CR3 = p_SelectorNumber << 8;
	CDCommand.CR4 = 0x0000;

	Return = CDC_CDExecCommand( CDC_HIRQ_EFLS, &CDCommand, &CDCommandReturn );

	if( Return != CDC_ERROR_OK )
	{
		return Return;
	}

	while( !( CDB_REG_HIRQ & CDC_HIRQ_ESEL ) )
	{
	}

	return CDC_ERROR_OK;
}

CDC_ERROR CDC_CDResetSelectorAll( void )
{
	return CDC_CDResetSelector( 0xFC, 0 );
}

CDC_ERROR CDC_CDInitialise( void )
{
	CDC_ERROR Return;
	char PrintBuffer[ 80 ];
	char Buffer[ 80 ];

	/* Stop all in-progress file transfers */
	if( ( Return = CDC_CDAbortFile( ) ) != CDC_ERROR_OK )
	{
		return Return;
	}

	/* Initialise the CD block */
	if( ( Return = CDC_CDBlockInitialise( 0 ) ) != CDC_ERROR_OK )
	{
		return Return;
	}

	/* Stop all data transfers */
	if( ( Return = CDC_CDEndTransfer( ) ) != CDC_ERROR_OK )
	{
		return Return;
	}

	/* Reset everything */
	Return = CDC_CDResetSelectorAll( );

	return Return;
}

CDC_ERROR CDC_CDSetSectorSize( CDC_SECTOR_LENGTH p_Size )
{
	CD_CMD_REGS CDCommand, CDCommandReturn;

	CD_SECTOR_SIZE = p_Size;

	CDCommand.CR1 = 0x6000 | ( p_Size & 0xFF );
	CDCommand.CR2 = p_Size << 8;
	CDCommand.CR3 = 0x0000;
	CDCommand.CR4 = 0x0000;

	return CDC_CDExecCommand( CDC_HIRQ_ESEL, &CDCommand, &CDCommandReturn );
}

CDC_ERROR CDC_CDResetSelectorOne( int p_SelectorNumber )
{
	return CDC_CDResetSelector( 0, p_SelectorNumber );
}

CDC_ERROR CDC_CDConnectCDToFilter( int p_FilterNumber )
{
	CD_CMD_REGS CDCommand, CDCommandReturn;

	CDCommand.CR1 = 0x3000;
	CDCommand.CR2 = 0x0000;
	CDCommand.CR3 = p_FilterNumber << 8;
	CDCommand.CR4 = 0x0000;

	return CDC_CDExecCommand( CDC_HIRQ_ESEL, &CDCommand, &CDCommandReturn );
}

CDC_ERROR CDC_CDPlayFrameAddress( int p_PlayMode, int p_StartFrameAddress,
	int p_NumberOfSectors )
{
	CD_CMD_REGS CDCommand, CDCommandReturn;

	CDB_REG_HIRQ = ~( CDC_HIRQ_PEND | CDC_HIRQ_CSCT ) | CDC_HIRQ_CMOK;

	CDCommand.CR1 = 0x1080 | ( p_StartFrameAddress >> 16 );
	CDCommand.CR2 = p_StartFrameAddress;
	CDCommand.CR3 = ( p_PlayMode << 8 ) | 0x80 | ( p_NumberOfSectors >> 16 );
	CDCommand.CR4 = p_NumberOfSectors;

	return CDC_CDExecCommand( 0, &CDCommand, &CDCommandReturn );
}

int CDC_CDIsDataReady( int p_SelectorNumber )
{
	CD_CMD_REGS CDCommand, CDCommandReturn;

	CDCommand.CR1 = 0x5100;
	CDCommand.CR2 = 0x0000;
	CDCommand.CR3 = p_SelectorNumber << 8;
	CDCommand.CR4 = 0x0000;

	if( CDC_CDExecCommand( 0, &CDCommand, &CDCommandReturn ) == CDC_ERROR_OK )
	{
		return 0;
	}

	return CDCommandReturn.CR4;
}

CDC_ERROR CDC_CDGetThenDeleteSectorData( int p_SectorOffset,
	int p_BufferNumber, int p_SectorNumber )
{
	CD_CMD_REGS CDCommand, CDCommandReturn;
	CDC_ERROR Return;

	CDCommand.CR1 = 0x6300;
	CDCommand.CR2 = p_SectorOffset;
	CDCommand.CR3 = p_BufferNumber << 8;
	CDCommand.CR4 = p_SectorNumber;

	if( ( Return = CDC_CDExecCommand( 0, &CDCommand, &CDCommandReturn ) )
		!= CDC_ERROR_OK )
	{
		return Return;
	}

	/* Not sure if this function should wait */
	while( !( CDB_REG_HIRQ & ( CDC_HIRQ_EHST | CDC_HIRQ_DRDY ) ) )
	{
	}

	return CDC_ERROR_OK;
}

CDC_ERROR CDC_CDTransferDataBytes( uint32_t p_ByteCount, uint32_t *p_pBuffer )
{
	uint32_t Index;
	CDC_ERROR Return;
	int SectorCount = p_ByteCount / SECTOR_SIZE_TABLE[ CD_SECTOR_SIZE ];

	if( p_ByteCount % SECTOR_SIZE_TABLE[ CD_SECTOR_SIZE ] )
	{
		++SectorCount;
	}

	if( Return = ( CDC_CDGetThenDeleteSectorData( 0, 0, SectorCount ) ) !=
		CDC_ERROR_OK )
	{
		return Return;
	}

	for( Index = 0; Index < ( p_ByteCount >> 2 ); ++Index, ++p_pBuffer )
	{
		p_pBuffer[ 0 ] = CDB_REG_DATATRNS;
	}

	if( p_ByteCount % 4 )
	{
		uint32_t Data;
		uint8_t *pDataPointer = &Data;

		Data = CDB_REG_DATATRNS;

		for( Index = 0; Index < ( p_ByteCount % 4 ); ++Index )
		{
			( ( uint8_t * )p_pBuffer )[ Index ] = pDataPointer[ Index ];
		}
	}

	return CDC_CDEndTransfer( );
}

CDC_ERROR CDC_CDReadSector( void *p_pBuffer, unsigned long p_FrameAddress,
	int p_SectorSize, unsigned long p_ByteCount )
{
	CDC_ERROR Return;
	int Done = 0;
	int SectorCount = p_ByteCount / SECTOR_SIZE_TABLE[ CD_SECTOR_SIZE ];

	if( p_ByteCount % SECTOR_SIZE_TABLE[ CD_SECTOR_SIZE ] != 0 )
	{
		++SectorCount;
	}

	if( ( Return = CDC_CDSetSectorSize( p_SectorSize ) ) != CDC_ERROR_OK )
	{
		return Return;
	}

	if( ( Return = CDC_CDResetSelectorOne( 0 ) ) != CDC_ERROR_OK )
	{
		return Return;
	}

	if( ( Return = CDC_CDConnectCDToFilter( 0 ) ) != CDC_ERROR_OK )
	{
		return Return;
	}

	if( ( Return = CDC_CDPlayFrameAddress( 0, p_FrameAddress,
		SectorCount ) ) != CDC_ERROR_OK )
	{
		return Return;
	}

	while( !Done )
	{
		unsigned long SectorsToRead = 0;
		unsigned long BytesToRead;

		while( ( SectorsToRead = CDC_CDIsDataReady( 0 ) ) == 0 )
		{
		}

		if( ( SectorsToRead * SECTOR_SIZE_TABLE[ CD_SECTOR_SIZE ] ) >
			p_ByteCount )
		{
			BytesToRead = p_ByteCount;
		}
		else
		{
			BytesToRead = SectorsToRead * SECTOR_SIZE_TABLE[ CD_SECTOR_SIZE ];
		}

		if( ( Return = CDC_CDTransferDataBytes( BytesToRead, p_pBuffer ) ) !=
			CDC_ERROR_OK )
		{
			return Return;
		}

		p_ByteCount -= BytesToRead;
		p_pBuffer += BytesToRead;

		if( p_ByteCount == 0 )
		{
			Done = 1;
		}
	}

	return CDC_ERROR_OK;
}

CDC_ERROR CDC_CDGetStat( PCDC_STAT p_pCDStat )
{
	CD_CMD_REGS CDCommand, CDCommandReturn;
	CDC_ERROR Return;

	CDCommand.CR1 = 0x0000;
	CDCommand.CR2 = 0x0000;
	CDCommand.CR3 = 0x0000;
	CDCommand.CR4 = 0x0000;

	if( ( Return = CDC_CDExecCommand( 0, &CDCommand, &CDCommandReturn ) ) !=
		CDC_ERROR_OK )
	{
		return Return;
	}

	p_pCDStat->Status = CDCommandReturn.CR1 >> 8;
	p_pCDStat->Flags = ( CDCommandReturn.CR1 >> 4 ) & 0xF;
	p_pCDStat->RepeatCount = CDCommandReturn.CR1 & 0xF;
	p_pCDStat->Control = CDCommandReturn.CR2 >> 8;
	p_pCDStat->TNO = CDCommandReturn.CR2 & 0xFF;
	p_pCDStat->Index = CDCommandReturn.CR3 >> 8;
	p_pCDStat->FrameAddress = ( ( CDCommandReturn.CR3 & 0xFF ) << 16 ) |
		CDCommandReturn.CR4;

	return CDC_ERROR_OK;
}

int CDC_CDIsAuthenticated( uint16_t *p_pDiscAuth )
{
	CD_CMD_REGS CDCommand, CDCommandReturn;
	char Buffer[ 80 ];

	CDCommand.CR1 = 0xE100;
	CDCommand.CR2 = 0x0000;
	CDCommand.CR3 = 0x0000;
	CDCommand.CR4 = 0x0000;

	if( CDC_CDExecCommand( 0, &CDCommand, &CDCommandReturn ) != CDC_ERROR_OK )
	{
		return 0;
	}

	if( p_pDiscAuth )
	{
		( *p_pDiscAuth ) = CDCommandReturn.CR2;
	}

	if( CDCommandReturn.CR2 != 0x04 && CDCommand.CR2 != 0x02 )
	{
		return 0;
	}

	return 1;
}

CDC_ERROR CDC_CDAuthenticate( void )
{
	CDC_ERROR Return;
	CD_CMD_REGS CDCommand, CDCommandReturn;
	uint16_t Auth;
	CDC_STAT CDStat;
	int Index;

	CDB_REG_HIRQ =  ~( CDC_HIRQ_DCHG | CDC_HIRQ_EFLS );

	CDCommand.CR1 = 0xE000;
	CDCommand.CR2 = 0x0000;
	CDCommand.CR3 = 0x0000;
	CDCommand.CR4 = 0x0000;

	if( ( Return = CDC_CDExecCommand( CDC_HIRQ_EFLS, &CDCommand,
		&CDCommandReturn ) ) != CDC_ERROR_OK )
	{
		return Return;
	}

	if( ( Return = CDC_CDEndTransfer( ) ) != CDC_ERROR_OK )
	{
	}
	if( ( Return = CDC_CDAbortFile( ) ) != CDC_ERROR_OK )
	{
	}

	while( !( CDB_REG_HIRQ & CDC_HIRQ_EFLS ) )
	{
	}

	for( ;; )
	{
		for( Index = 0; Index < 100000; ++Index )
		{
		}

		if( CDC_CDGetStat( &CDStat ) != CDC_ERROR_OK )
		{
			continue;
		}

		if( CDStat.Status == CDC_STATUS_PAUSE )
		{
			break;
		}
		else if( CDStat.Status == CDC_STATUS_FATAL )
		{
			return CDC_ERROR_UNKNOWN;
		}
	}

	if( CDC_CDIsAuthenticated( &Auth ) )
	{
		return CDC_ERROR_OK;
	}
	
	return CDC_ERROR_AUTH;
}

