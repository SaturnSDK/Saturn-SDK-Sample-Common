#include <CDC.h>
#include <Interrupt.h>

#define CD_REG_CR1 *( ( volatile uint16_t * )0x25890018 )
#define CD_REG_CR2 *( ( volatile uint16_t * )0x2589001C )
#define CD_REG_CR3 *( ( volatile uint16_t * )0x25890020 )
#define CD_REG_CR4 *( ( volatile uint16_t * )0x25890024 )
#define CDB_REG_HIRQ *( ( volatile uint16_t * )0x25890008 )

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

