/*
 *****************************************************************************
 * Copyright @ 2009 by austriamicrosystems AG                                *
 * All rights are reserved.                                                  *
 *                                                                           *
 * Reproduction in whole or in part is prohibited without the written consent*
 * of the copyright owner. Austriamicrosystems reserves the right to make    *
 * changes without notice at any time. The software is provided as is and    *
 * Austriamicrosystems makes no warranty, expressed, implied or statutory,   *
 * including but not limited to any implied warranty of merchantability or   *
 * fitness for any particular purpose, or that the use will not infringe any *
 * third party patent, copyright or trademark. Austriamicrosystems should    *
 * not be liable for any loss or damage arising from its use.                *
 *****************************************************************************
 */

/*
 * PROJECT: AS3911 firmware
 * $Revision: $
 * LANGUAGE: ANSI C
 */

/*! \file emv_layer4.c
 *
 * \author Oliver Regenfelder
 *
 * \brief EMV compliant ISO14443-4 data transmission.
 */

/*
 ******************************************************************************
 * INCLUDES
 ******************************************************************************
 */

#include "emv_layer4.h"
#include "emv_error_codes.h"
#include "emv_standard.h"
#include "pcd_interface.h"
#include "string.h"
#include "iso14443a.h"
#include "iso14443b.h"
//#include "ept_drv.h"
#include "type.h"

#define USB_CCID_CLASS
#define VERSION_RC523

unsigned char guc_PlusKeyNo;
unsigned char g_cUID[12];		//卡UID或ATQB
unsigned char g_cUIDSize;		//UID大小
unsigned char g_cCLcardType;	//非接卡类型
unsigned char g_cReceBuf[512];
unsigned char g_cCLcardType;	//非接卡类型

#define CARD_UNKOWN				0
#define CARD_ULTRA_LIGHT		1
#define CARD_MIFARE_S50			2
#define CARD_MIFARE_S70			3
#define CARD_MIFARE_DESFIRE		4
#define CARD_MIFARE_PRO			5
#define CARD_MIFARE_PROX		6
#define CARD_MIFARE_PLUS		7

/*
 ******************************************************************************
 * DEFINES
 ******************************************************************************
 */

/*! Maximum number of retransmission for the layer 4 protocol. */
//#define EMV_MAX_RETRANSMISSIONS     2
#define EMV_MAX_RETRANSMISSIONS     1		//重发次数改为2次，避免卡移出时查询时间太长
/*! Maximum number of S(WTX) requests allowed for the PICC during a single layer-4 block transfer. */
#define EMV_MAX_NUM_CONSECUTIVE_SWTX_REQUESTS_AFTER_RETRANSMISSION_REQUESTS 2
/*! ISO14443-4 I-block PCB with the chaining bit set. */
#define EMV_PCB_IBLOCK_CHAINED      0x12
/*! ISO14443-4 I-block PCB with the chaining bit cleared. */
#define EMV_PCB_IBLOCK_UNCHAINED    0x02
/*! ISO14443-4 acknowledge R-block PCB. This constant needs to be xored with the current block number. */
#define EMV_PCB_RACK                0xA2
/*! ISO14443-4 not acknowledge R-block PCB. This constant needs to be xored with the current block number. */
#define EMV_PCB_RNAK                0xB2
/*! ISO14443-4 WTX request or response S-block PCB. This must not be combined with the current block number. */
#define EMV_PCB_SWTX                0xF2
/*! Bitmask for the chaining bit of an ISO14443-4 I-block PCB. */
#define EMV_PCB_CHAINING_BIT_MASK   0x10
/*! Bitmask for the block number of an ISO14443-4 R-block or I-block PCB. */
#define EMV_PCB_BLOCK_NUMBER_MASK   0x01
/*! Bitmask for the WTXM bits of the inf byte of an WTX request or response. */
#define EMV_WTXM_MASK               0x3F
/*!
 *****************************************************************************
 * Maximum allowed value for the WTXM of an WTX request. If a value above
 * EMV_MAX_WTXM is requested, then EMV_MAX_WTXM will be used instead of the
 * requested value.
 *****************************************************************************
 */
#define EMV_MAX_WTXM                59

/*
 ******************************************************************************
 * MACROS
 ******************************************************************************
 */

/*
 ******************************************************************************
 * LOCAL DATA TYPES
 ******************************************************************************
 */

/*!
 *****************************************************************************
 * Retransmission request type to use when data from the PICC is not received
 * correctly (e.g. timeut or transmission error).
 *****************************************************************************
 */
typedef enum EMVRetransmissionRequestType
{
	/*! Retransmissions are requested with an R(ACK) block. */
	EMV_RETRANSMISSION_REQUEST_RACK,
	/*! Retransmissions are requested with an R(NAK) block. */
	EMV_RETRANSMISSION_REQUEST_RNAK
} EMVRetransmissionRequestType_t;

/*
 ******************************************************************************
 * LOCAL VARIABLES
 ******************************************************************************
 */

/*!
 *****************************************************************************
 * Buffer for low level data blocks (R,S and I blocks). This buffer is used
 * for transmission AND reception.
 *****************************************************************************
 */
static u8 emvBlockBuffer[EMV_FSD_MIN_PCD];

/*
 ******************************************************************************
 * LOCAL TABLES
 ******************************************************************************
 */

/*
 ******************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 ******************************************************************************
 */

/*!
 *****************************************************************************
 * \brief Transceive a single low level block of the ISO1443-4 protocol and
 * employ error handling.
 *
 * Transceive a single low level block of the ISO14443-4 protocol. Error
 * handling is performed according to requirement 10.13. The parameter
 * \a retransmissionRequestType is used to distinguish between error
 * handling after a non chaining block has been received and error handling
 * after a chaining block has beeen received.
 *
 * \param[in] pcb The PCB of the block.
 * \param[in] inf Pointer to the INF field of the block.
 * \param[in] infLength Length of the INF field in bytes.
 * \param[out] response Buffer for the piccs response.
 * \param[in] maxResponseLength Size of the response buffer in bytes.
 * \param[out] responseLength Length of the received response in bytes.
 * \param[in] retransmissionRequestType Defines the PCB Block to use for
 * retransmission requests.
 *
 * \return EMV_ERR_OK: No error, response APDU received.
 * \return EMV_ERR_PROTOCOL: Protocl error during reception of the response
 * APDU.
 * \return EMV_ERR_TRANSMISSION: Transmission error during reception of the
 * response APDU.
 * \return EMV_ERR_TIMEOUT: No response APDU receied, or a timeout occured during
 * reception of the response APDU.
 * \return EMV_ERR_INTERNAL: Internal buffer overflow during reception of the
 * response APDU.
 *****************************************************************************
 */
static s16 emvTransceiveBlock(u8 pcb,
							 const u8 *inf,
							 size_t infLength,
							 u8 *response,
							 size_t maxResponseLength,
							 size_t *responseLength,
							 enum EMVRetransmissionRequestType retransmissionRequestType);

/*
 ******************************************************************************
 * GLOBAL VARIABLE DEFINITIONS
 ******************************************************************************
 */
unsigned char g_ULKey[16];
unsigned char g_ULIv[8];
unsigned char g_ULRndA[8];
unsigned char g_ULRndB[8];
unsigned char g_ULAuthFlg;

unsigned char g_PlusPCDcap2[6];
unsigned char g_PlusPICCcap2[6];
unsigned short g_PlusWCtr;
unsigned short g_PlusRCtr;
unsigned char g_PlusRndA[16];
unsigned char g_PlusRndB[16];
unsigned char g_PlusTI[4];
unsigned char g_AuthenticationK[16];
unsigned char g_PlusKenc[16];
unsigned char g_PlusKmac[16];

unsigned short g_cFSC;                                    //
unsigned char g_ISO14443_4;
unsigned char g_cRstPara[15];		//非接卡复位参数，用于PCSC
unsigned char g_cRstParaLen;		//非接卡复位参数长度
int emvTypeA;
int emvTypeB;
unsigned char g_cBlockNumber;
unsigned char g_cFWI;                                       //
unsigned char g_cSFGI;                                       //
unsigned char g_cCidNad;                                    //
unsigned char g_cPiccPcd; /*!< Datarate bits PICC->PCD. */
unsigned char g_cPcdPicc; /*!< Datarate bits PCD->PICC. */
//unsigned int  g_nBeepTime;                            //蜂鸣时间

const unsigned short FSC_Table[18] = { 0, 16, 1, 24, 2, 32, 3, 40, 4, 48, 5, 64, 6, 96, 7, 128, 8, 256 };

/*
 ******************************************************************************
 * GLOBAL FUNCTIONS
 ******************************************************************************
 */
void DelayMs(UINT32 delaymS)
{
	unsigned int  counter=0;
	unsigned int  k;
	for(k=0; k<delaymS; k++)
	{
		for (counter=0; counter<6000; counter++);
	}
}
s16 emvTransceiveApdu(const u8 *apdu, size_t apduLength, u8 *response, size_t maxResponseLength, size_t *responseLength)
{
	size_t numApduBytesTransmitted = 0;
	size_t numResponseBytesReceived = 0;
	size_t index = 0;
	size_t piccResponseLength = 0;
	u16 fsc = 0;
	u8 pcb = 0;
	u8 numIBlockRetransmissions = 0;
	s16 error = EMV_ERR_OK;
	bool_t awaitingAnotherIBlock = FALSE;

	fsc = g_cFSC;
	numIBlockRetransmissions = 0;

	while ((apduLength - numApduBytesTransmitted) > (fsc - 3))
	{
		/* I-block with chaining. */
		pcb = EMV_PCB_IBLOCK_CHAINED | g_cBlockNumber;

		error = emvTransceiveBlock(pcb,
									&apdu[numApduBytesTransmitted],
									fsc - 3,
									emvBlockBuffer,
									sizeof(emvBlockBuffer),
									&piccResponseLength,
									EMV_RETRANSMISSION_REQUEST_RNAK);

		if (EMV_ERR_OK != error)
		{
			/* Pass on errors to higher layer. */
			return error;
		}

		/* PICC response must have a length of 1 (R(ACK)). */
		if (piccResponseLength != 1)
		{
			return EMV_ERR_PROTOCOL;
		}

		/* Response must be a R(ACK) */
		if ((emvBlockBuffer[0] & ~EMV_PCB_BLOCK_NUMBER_MASK) != 0xA2)
		{
			/* The received block is not a proper R(ACK). */
			return EMV_ERR_PROTOCOL;
		}

		/* Check block number of the received R(ACK) */
		if ((emvBlockBuffer[0] & EMV_PCB_BLOCK_NUMBER_MASK) != g_cBlockNumber)
		{
			/* Non matching block number. Retransmit last I-block. */
			if (numIBlockRetransmissions >= EMV_MAX_RETRANSMISSIONS)
			{
				/* Too many retransmissions already, report protocol error. */
				return EMV_ERR_PROTOCOL;
			}
			else
			{
				numIBlockRetransmissions++;
				/* Do not toggle the block number and also do not increase the
				 * number of transmitted apdu bytes. This will cause the next I-block
				 * to be equal to the last one.
				 */
			}
		}
		else
		{
			/* Received proper R(ACK) =>  Toggle block number. */
			g_cBlockNumber ^= 0x01;
			numApduBytesTransmitted += fsc - 3;
			numIBlockRetransmissions = 0;
		}
	}

	/* Send an I-block without chaining. Either as the last I-block of a chained sequence.
	 * or as a single I-block if the APDU is small enough.
	 */
	numIBlockRetransmissions = 0;
	while (1)
	{
		pcb = EMV_PCB_IBLOCK_UNCHAINED | g_cBlockNumber;

		error = emvTransceiveBlock(pcb,
									&apdu[numApduBytesTransmitted],
									apduLength - numApduBytesTransmitted,
									emvBlockBuffer,
									sizeof(emvBlockBuffer),
									&piccResponseLength,
									EMV_RETRANSMISSION_REQUEST_RNAK);

		if (EMV_ERR_OK != error)
		{
			/* Pass on errors to higher layer. */
			return error;
		}

		/* If the response is an R(ACK) block, then we must check whether a retransmission
		 * has to be performed.
		 */
		if ((emvBlockBuffer[0] & ~EMV_PCB_BLOCK_NUMBER_MASK) != EMV_PCB_RACK)
		{
			/* The received block is not an R(ACK).
			 * Break the while loop and continue with the handling of the response I-block.
			 */
			break;
		}

		/* Check block number of the received R(ACK) */
		if ((emvBlockBuffer[0] & EMV_PCB_BLOCK_NUMBER_MASK) != g_cBlockNumber)
		{
			/* Non matching block number. Retransmit last I-block. */
			if (numIBlockRetransmissions >= EMV_MAX_RETRANSMISSIONS)
			{
				/* Too many retransmissions already, report protocol error. */
				return EMV_ERR_PROTOCOL;
			}
			else
			{
				numIBlockRetransmissions++;
				/* Do not toggle the block number and also do not increase the
				 * number of transmitted apdu bytes. This will cause the next I-block
				 * to be equal to the last one.
				 */
			}
		}
		else
		{
			/* Received an R(ACK) with correct block number as response to an unchained
			 * I-block. This is a protocol error.
			 */
			return EMV_ERR_PROTOCOL;
		}
	}

	/* Handle the answer of the card to the APDU. */
	do
	{
		/* Pass on errors to the higher layer. */
		if (EMV_ERR_OK != error)
			return error;

		/* An I-block must have a size > 0. */
		if (0 == piccResponseLength)
			return EMV_ERR_PROTOCOL;

		/* The received block must be a proper I-block. */
		if (((emvBlockBuffer[0] & ~EMV_PCB_BLOCK_NUMBER_MASK)
				!= EMV_PCB_IBLOCK_UNCHAINED)
				&& ((emvBlockBuffer[0] & ~EMV_PCB_BLOCK_NUMBER_MASK)
						!= EMV_PCB_IBLOCK_CHAINED))
			return EMV_ERR_PROTOCOL;

		/* The received block must have a correct block number. */
		if ((emvBlockBuffer[0] & EMV_PCB_BLOCK_NUMBER_MASK) != g_cBlockNumber)
			return EMV_ERR_PROTOCOL;

		/* Received proper I-block => toggle block number. */
		g_cBlockNumber ^= 0x01;

		/* Check for response buffer overflow */
		if (numResponseBytesReceived + piccResponseLength > maxResponseLength)
		{
			/* Copy as much bytes as possible into the response buffer. */
			for (index = 0;index < maxResponseLength - numResponseBytesReceived;index++)
				response[numResponseBytesReceived + index] = emvBlockBuffer[1 + index];

			/* Signal buffer overflow to caller. */
			return EMV_ERR_INTERNAL;
		}

		for (index = 0; index < piccResponseLength - 1; index++)
			response[numResponseBytesReceived + index] = emvBlockBuffer[1 + index];
		numResponseBytesReceived += index;

		if (emvBlockBuffer[0] & EMV_PCB_CHAINING_BIT_MASK)
		{
			/* Chaining bit set. Send R(ACK) and receive next I-block. */
			emvBlockBuffer[0] = EMV_PCB_RACK | g_cBlockNumber;

			error = emvTransceiveBlock(emvBlockBuffer[0],
										NULL,
										0,
										emvBlockBuffer,
										sizeof(emvBlockBuffer),
										&piccResponseLength,
										EMV_RETRANSMISSION_REQUEST_RACK);
			awaitingAnotherIBlock = TRUE;
		}
		else
			awaitingAnotherIBlock = FALSE;

	} while (awaitingAnotherIBlock); /* Continue while the chained receive is in progress. */

	*responseLength = numResponseBytesReceived;
	return EMV_ERR_OK;
}

/*
 ******************************************************************************
 * LOCAL FUNCTIONS
 ******************************************************************************
 */

static s16 emvTransceiveBlock(u8 pcb,
								const u8 *inf,
								size_t infLength,
								u8 *response,
								size_t maxResponseLength,
								size_t *responseLength,
								enum EMVRetransmissionRequestType retransmissionRequestType)
{
	size_t index = 0;
	s16 error = EMV_HAL_ERR_OK;
	u8 numRetransmissions = 0;
	u8 numConsecutiveSwtxRequestsAfterRnakReceived = 0;
	bool_t lastBlockWasRetransmissionRequest = 0;

	if (infLength + 3 > sizeof(emvBlockBuffer))
	{
		/* The given block is bigger than the internal buffer.
		 * This should actually never happen.
		 */
		return EMV_ERR_INTERNAL;
	}

	/* Construct layer 4 low level block */
	emvBlockBuffer[0] = pcb;
	for (index = 0; index < infLength; index++)
		emvBlockBuffer[1 + index] = inf[index];

	/* Calculate frame wait time */
	error = MifareTransmit(emvBlockBuffer, infLength + 1, response,responseLength, g_cFWI);
	*responseLength += 2;	//收到的数据长度不包含2字节CRC，后面是按照有CRC处理的，所以长度加上2

	numRetransmissions = 0;
	numConsecutiveSwtxRequestsAfterRnakReceived = 0;
	lastBlockWasRetransmissionRequest = FALSE;
	while (numRetransmissions < EMV_MAX_RETRANSMISSIONS)
	{
		/* Mesages larger than the reported reader buffer size (FSD) must be
		 * treated as a protocol error.
		 * Note: There have been diverting statements from FIME regarding
		 * treatment of recieved block exceeding FSD. One statement was to
		 * tread it as a protocol error (which implies that no further
		 * retransmissions are requested). Another statement was to treat this
		 * as a transmission error which might require a retransmission request
		 * to be send.
		 *
		 * This implementation treats too long messages as protocol error.
		 */
		if (*responseLength > EMV_FSD_MIN_PCD)
			return EMV_ERR_PROTOCOL;
		else if (EMV_HAL_ERR_OVERFLOW == error)
			return EMV_ERR_PROTOCOL;
		else if (EMV_HAL_ERR_OK == error)
		{
			if (response[0] == 0xF2)
			{
				/* Handle frame wait extension requests. */
				u8 requestedWtxm = 0;
				u8 usedWtxm = 0;
				u8 swtxResponse[2];
				u8 fwi;

				/* The EMV standard limits the number of consecutive sequences of
				 * 'retransmission request followed by S(WTX) request' but there is
				 * no limit to the number of S(WTX) requests received after any
				 * other block.
				 */
				if (lastBlockWasRetransmissionRequest)
					numConsecutiveSwtxRequestsAfterRnakReceived++;
				else
					numConsecutiveSwtxRequestsAfterRnakReceived = 0;

				/* According to FIME reception of an S(WTX) request counts as reception of a correct frame.
				 * Thus the counter for consecutive transmission errors must be reseted.
				 */
				numRetransmissions = 0;

				/* Check for proper S(WTX) request size (2 byte + 2 bytes CRC). */
				if (*responseLength != 2 + 2)
					return EMV_ERR_PROTOCOL;

				/* An excess of S(WTX) requests after retransmission requests must
				 * be treated as timeout error.
				 * See 10.3.5.5. The explanation from this clause also applies to
				 * 10.3.5.8 although consecutive S(WTX) requests are not mentioned there.
				 */
				if (numConsecutiveSwtxRequestsAfterRnakReceived > EMV_MAX_NUM_CONSECUTIVE_SWTX_REQUESTS_AFTER_RETRANSMISSION_REQUESTS)
					return EMV_ERR_TIMEOUT;

				requestedWtxm = response[1] & EMV_WTXM_MASK;

				/* Check validity of wtxm value and adjust range. */
				if (requestedWtxm == 0)
					return EMV_ERR_PROTOCOL;
				else if (requestedWtxm > EMV_MAX_WTXM)
					//usedWtxm = EMV_MAX_WTXM;
					return EMV_ERR_PROTOCOL;//TA404_10, TA408_8, TA411_10, TA417_6, TB404_10, TB408_08, TB411_10, TB417_6, 20140611
				else
					usedWtxm = requestedWtxm;

				/* Calculate requested frame wait time in carrier cycles. */
				fwi = g_cFWI * usedWtxm;
				/* In the EMV standard there is no clear definition on what to do
				 * if the requested FWT_TEMP exceeds FWT_MAX.
				 * But, according to FIME the PCD shall limit FWT_TEMP to FWT_MAX if that
				 * happens.
				 */
				if (fwi > EMV_FWI_MAX_PCD)
				{
					fwi = EMV_FWI_MAX_PCD;
				}

				swtxResponse[0] = 0xF2;
				swtxResponse[1] = requestedWtxm;

				lastBlockWasRetransmissionRequest = FALSE;

				error = MifareTransmit(&swtxResponse[0], 2, response, responseLength, fwi);
				*responseLength += 2;	//收到的数据长度不包含2字节CRC，后面是按照有CRC处理的，所以长度加上2
				/* ToDo: Check whether S(WTX) pairs are counted as retransmissions. */
			}
			else
			{
				/* The CRC bytes are not reported in the response length. */
				*responseLength -= 2;
				return EMV_ERR_OK;
			}
		}
		else
		{
			/* Check for a timeout after consecutive R(NAK), S(WTX), S(WTX) response chains.
			 * See PCD 10.3.5.5 for details.
			 *
			 * Note: The >= is there for a reason. Please think _twice_ before changing it into >.
			 */
			if ((EMV_HAL_ERR_TIMEOUT == error)
					&& (numConsecutiveSwtxRequestsAfterRnakReceived >= EMV_MAX_NUM_CONSECUTIVE_SWTX_REQUESTS_AFTER_RETRANSMISSION_REQUESTS))
				return EMV_ERR_TIMEOUT;

			/* All (other) error conditions lead to a retransmission request. */
			if (EMV_RETRANSMISSION_REQUEST_RACK == retransmissionRequestType)
				emvBlockBuffer[0] = EMV_PCB_RACK | g_cBlockNumber;
			else if (EMV_RETRANSMISSION_REQUEST_RNAK == retransmissionRequestType)
				emvBlockBuffer[0] = EMV_PCB_RNAK | g_cBlockNumber;
			else
				return EMV_ERR_INTERNAL;

			lastBlockWasRetransmissionRequest = TRUE;

			error = MifareTransmit(emvBlockBuffer, 1, response, responseLength,g_cFWI);
			*responseLength += 2;	//收到的数据长度不包含2字节CRC，后面是按照有CRC处理的，所以长度加上2
			numRetransmissions++;
		}
	}

//SendStringA("transmitblock4\n\0");

	/* Blocks greater than the FSD must be considered as a protocol error. */
	if (*responseLength > EMV_FSD_MIN_PCD)
		return EMV_ERR_PROTOCOL;
	else if (EMV_HAL_ERR_OVERFLOW == error)
		return EMV_ERR_PROTOCOL;
	else if (EMV_HAL_ERR_OK == error)
	{
		/* The last retransmitted block is not allowed to be an S(WTX) request. */
		if (response[0] == EMV_PCB_SWTX)
			return EMV_ERR_TIMEOUT;
		/* The CRC bytes are not reported as part of the repsonse length. */
		*responseLength -= 2;
		return EMV_ERR_OK;
	}
	else if (EMV_HAL_ERR_TIMEOUT == error)
		return EMV_ERR_TIMEOUT;
	else
		return EMV_ERR_TRANSMISSION;
}

s16 emvTransceiveNAK(void)
{
	s16 error = EMV_ERR_OK;
	size_t piccResponseLength = 0;

	emvBlockBuffer[0] = EMV_PCB_RNAK | g_cBlockNumber;

	error = emvTransceiveBlock(emvBlockBuffer[0],
								NULL,
								0,
								emvBlockBuffer,
								sizeof(emvBlockBuffer),
								&piccResponseLength,
								EMV_RETRANSMISSION_REQUEST_RNAK);

	if (EMV_ERR_OK != error)
	{
		/* Pass on errors to higher layer. */
		return error;
	}

	/* PICC response must have a length of 1 (R(ACK)). */
	if (piccResponseLength != 1)
	{
		return EMV_ERR_PROTOCOL;
	}

	/* Response must be a R(ACK) */
	if ((emvBlockBuffer[0] & ~EMV_PCB_BLOCK_NUMBER_MASK) != 0xA2)
	{
		/* The received block is not a proper R(ACK). */
		return EMV_ERR_PROTOCOL;
	}

	/* Check block number of the received R(ACK) */
	if ((emvBlockBuffer[0] & EMV_PCB_BLOCK_NUMBER_MASK) == g_cBlockNumber)
	{
		return EMV_ERR_PROTOCOL;
	}
	else
	{
		/* Received proper R(ACK) =>  Toggle block number. */
		// emvBlockNumber ^= 0x01;
	}

	return EMV_ERR_OK;
}

#ifdef ISO14443_SUPPORT_PPS
s16 emvTypeAPPS(void)
{
	u8 pps_req[3];
	u8 pps_resp[3];
	u8 rate = 0;
	s8 error = EMV_ERR_OK;
	size_t responseLength = 0;

	if ((g_cPcdPicc==0) && (g_cPiccPcd==0))		//卡不支持高速率
	{
		return EMV_ERR_OK;
	}

	pps_req[0] = 0xD0;
	pps_req[1] = 0x11;
	pps_req[2] = 0x00;
	if ((g_cPcdPicc&0x04) == 0x04)		//DR=8
	{
		pps_req[2] |= 0x03;
	}
	else if ((g_cPcdPicc&0x02) == 0x02)		//DR=4
	{
		pps_req[2] |= 0x02;
	}
	else if ((g_cPcdPicc&0x01) == 0x01)		//DR=2
	{
		pps_req[2] |= 0x01;
	}

	if ((g_cPiccPcd&0x04) == 0x04)		//DS=8
	{
		pps_req[2] |= 0x0C;
	}
	else if ((g_cPiccPcd&0x02) == 0x02)		//DS=4
	{
		pps_req[2] |= 0x08;
	}
	else if ((g_cPiccPcd&0x01) == 0x01)		//DS=2
	{
		pps_req[2] |= 0x04;
	}

	error = MifareTransmit(pps_req, 3, pps_resp, &responseLength, g_cFWI);
	responseLength += 2;	//收到的数据长度不包含2字节CRC，后面是按照有CRC处理的，所以长度加上2

	/* Report errors back to higher layer. */
	/* The response must at least 1 byte. */
	if (error != EMV_ERR_OK)
	return error;
	else if (responseLength < (3))
	return EMV_ERR_PROTOCOL;

	if (pps_resp[0] != pps_req[0])
	return EMV_ERR_PROTOCOL;

#ifdef VERSION_RC531
	if ((g_cPcdPicc&0x04) == 0x04)		//DR(PCD to PICC)=8,848k, tx
	{
		WriteRawRC(RegCoderControl,0x01);
		WriteRawRC(RegModWidth,0x01);
	}
	else if ((g_cPcdPicc&0x02) == 0x02)		//DR=4,424k
	{
		WriteRawRC(RegCoderControl,0x09);
		WriteRawRC(RegModWidth,0x03);
	}
	else if ((g_cPcdPicc&0x01) == 0x01)		//DR=2,212k
	{
		WriteRawRC(RegCoderControl,0x11);
		WriteRawRC(RegModWidth,0x07);
	}

	if ((g_cPiccPcd&0x04) == 0x04)		//DS(PICC to PCD)=8,848k, rx
	{
		WriteRawRC(RegRxControl1,0x13);
		WriteRawRC(RegDecoderControl,0x09);
		WriteRawRC(RegRxThreshold,0x50);
		WriteRawRC(RegBPSKDemControl,0x0C);
	}
	else if ((g_cPiccPcd&0x02) == 0x02)		//DS=4,424k
	{
		WriteRawRC(RegRxControl1,0x33);
		WriteRawRC(RegDecoderControl,0x09);
		WriteRawRC(RegRxThreshold,0x50);
		WriteRawRC(RegBPSKDemControl,0x0C);
	}
	else if ((g_cPiccPcd&0x01) == 0x01)		//DS=2,212k
	{
		WriteRawRC(RegRxControl1,0x53);
		WriteRawRC(RegDecoderControl,0x09);
		WriteRawRC(RegRxThreshold,0x50);
		WriteRawRC(RegBPSKDemControl,0x0C);
	}
#else	//VERSION_RC523
	if ((g_cPcdPicc&0x04) == 0x04)		//DR(PCD to PICC)=8,848k, tx
	{
		SetBitMask(TxModeReg, BIT5|BIT4);
	}
	else if ((g_cPcdPicc&0x02) == 0x02)		//DR=4,424k
	{
		SetBitMask(TxModeReg, BIT5);
	}
	else if ((g_cPcdPicc&0x01) == 0x01)		//DR=2,212k
	{
		SetBitMask(TxModeReg, BIT4);
	}

	if ((g_cPiccPcd&0x04) == 0x04)		//DS(PICC to PCD)=8,848k, rx
	{
		SetBitMask(RxModeReg, BIT5|BIT4);
	}
	else if ((g_cPiccPcd&0x02) == 0x02)		//DS=4,424k
	{
		SetBitMask(RxModeReg, BIT5);
	}
	else if ((g_cPiccPcd&0x01) == 0x01)		//DS=2,212k
	{
		SetBitMask(RxModeReg, BIT4);
	}
#endif

	return EMV_ERR_OK;
}
#endif

s8 emvHalResetField(void)
{
	PcdAntennaOff();
	DelayMs(EMV_T_RESET);
	PcdAntennaOn();

	return 0;
}

s16 emvTypeACardPresent()
{
	u8 atqa[2] = { 0, 0 };
	char status;

	if (PcdConfigISOType('A') != MI_OK)
	{
		return 0;
	}
						//26 REQA 52 WKA
	status = PcdRequest(0x52, &atqa[0]);

	if (status != MI_OK)
		return 0;
	else
		return 1;
}

s16 emvTypeBCardPresent()
{
	u8 atqb[13 + 2];
	char status;

	if (PcdConfigISOType('B') != MI_OK)
	{
		return 0;
	}

	status = PcdRequestB(0x01, 0, 0, &atqb[0]);

	if (status != MI_OK)
		return 0;
	else
		return 1;
}

s16 readerPoll()
{
	emvTypeA = 0;
	emvTypeB = 0;

	/* Wait for t_p. */
	DelayMs(EMV_T_P);
	if (emvTypeACardPresent())
	{
		/* ISO14443-A card(s) found. */
		emvTypeA = 1;
		/* Send HLTA command. */
		PcdHalt();
		return EMV_ERR_OK;
	}
	// Wait for t_p.
	DelayMs(EMV_T_P);
	if (emvTypeBCardPresent())
	{
		// ISO14443-B card(s) found.
		emvTypeB = 1;
		return EMV_ERR_OK;
	}

	return EMV_ERR_INTERNAL;
}

s16 emvTypeAAnticollision(void)
{
	unsigned char status, i;
	unsigned char T0;
	unsigned short cardType;
	s16 error = EMV_ERR_OK;

	i = (FSDI << 4) & 0xF0;
	status = PcdRequest(0x52, &g_cReceBuf[0]);

	if (status == MI_OK)
	{
		status = TypeaAnticoll(g_cUID, &g_cUIDSize);
	}
	cardType = (g_cReceBuf[0] << 8) + g_cReceBuf[1];
	switch (cardType)
	{
	case 0x4400:	//ultra_light或个别Plus X
	{
		g_cCLcardType = CARD_ULTRA_LIGHT;
		break;
	}
	case 0x0400:	//Mifare_One(S50)
	{
		g_cCLcardType = CARD_MIFARE_S50;
		break;
	}
	case 0x0200:	//Mifare_One(S70)
	{
		g_cCLcardType = CARD_MIFARE_S70;
		break;
	}
	case 0x4403:	//Mifare_DESFire
	{
		g_cCLcardType = CARD_MIFARE_DESFIRE;
		break;
	}
	case 0x0800:	//Mifare_Pro
	{
		g_cCLcardType = CARD_MIFARE_PRO;
		break;
	}
	case 0x0403:	//Mifare_ProX
	{
		g_cCLcardType = CARD_MIFARE_PROX;
		break;
	}
	case 0x4200:	//Plus S/X
	{
		g_cCLcardType = CARD_MIFARE_PLUS;
		break;
	}
	default:
	{
		g_cCLcardType = CARD_UNKOWN;
	}
	}

	if (status == MI_OK)
	{
		if (g_ISO14443_4 == 1)		//CPU卡
		{
			status = MifareProRst(i, &g_cReceBuf[0], &g_cReceBuf[5]);
			if (status == MI_OK)
			{
				g_cFWI = 8;
				g_cSFGI = 8;
				g_cCidNad = 0;
				g_cFSC = FSD;
				g_cPcdPicc = 0;
				g_cPiccPcd = 0;

				T0 = g_cReceBuf[6];
				for (i = 0; i < 9; i++)
				{
					if ((T0 & 0x0F) == FSC_Table[i * 2])
					{
						g_cFSC = FSC_Table[i * 2 + 1];
						break;
					}
				}
				if (g_cFSC > FSD)
				{
					g_cFSC = FSD;
				}

				i = 0;
				if ((T0 & 0x10) == 0x10)		//TA1存在
				{
					i++;
					g_cPcdPicc = (g_cReceBuf[6 + i] & 0x07);
					g_cPiccPcd = ((g_cReceBuf[6 + i] >> 4) & 0x07);
				}
				if ((T0 & 0x20) == 0x20)		//TB1存在
				{
					i++;
					g_cFWI = g_cReceBuf[6 + i] >> 4;
					g_cSFGI = (g_cReceBuf[6 + i] & 0x0F);
				}
				if ((T0 & 0x40) == 0x40)		//TC1存在
				{
					i++;
					g_cCidNad = (g_cReceBuf[6 + i] & 0x03) << 2;
				}
				g_cBlockNumber = 0;
				g_cRstParaLen = g_cReceBuf[5] - 2 - i;	//去掉TS、T0、和TA1 TB1 TC1
				memcpy(g_cRstPara, &g_cReceBuf[6 + i + 1], g_cRstParaLen);//历史字节
			}
			else
			{
				error = EMV_ERR_TIMEOUT;
			}
		}
		else		//非CPU卡
		{
			//error = EMV_ERR_PROTOCOL;
		}
	}
	return error;
}

s16 emvTypeBAnticollision(void)
{
	unsigned char status, i;

	g_cFWI = 8;
	g_cSFGI = 8;
	g_cCidNad = 0;
	g_cFSC = FSD;
	if ((status = PcdRequestB(0x01, 0, 0, &g_cReceBuf[0])) == MI_OK)
	{

		/* Parse bitrate definition fields. */
		g_cPiccPcd = (g_cReceBuf[9] & 0x70) >> 4;
		g_cPcdPicc = (g_cReceBuf[9] & 0x07);

		g_cFWI = g_cReceBuf[11] >> 4;
		g_cCidNad = ((g_cReceBuf[11] & 0x02) << 1)
				| ((g_cReceBuf[11] & 0x01) << 3);
		g_cSFGI = g_cReceBuf[12] >> 4;
		for (i = 0; i < 9; i++)
		{
			if ((g_cReceBuf[10] >> 4) == FSC_Table[i * 2])
			{
				g_cFSC = FSC_Table[i * 2 + 1];
				break;
			}
		}
		if (g_cFSC > FSD)
		{
			g_cFSC = FSD;
		}
		g_cBlockNumber = 0;
		g_cRstParaLen = 12;
		memcpy(g_cRstPara, &g_cReceBuf[0], g_cRstParaLen);		//历史字节

		g_cUIDSize = 12;
		memcpy(g_cUID, &g_cReceBuf[0], g_cUIDSize);

		status = PcdAttrib(&g_cReceBuf[1], g_cReceBuf[10] & 0x0F,
				&g_cReceBuf[12]);
	}

	return status;
}

s16 emvCollisionDetection(void)
{
	if ((emvTypeA) != 0 && (emvTypeB != 0))
		return EMV_ERR_COLLISION;

	if (emvTypeA != 0)
	{
		return emvTypeAAnticollision();
	}
	else if (emvTypeB != 0)
	{
		return emvTypeBAnticollision();
	}
	else
		return EMV_ERR_COLLISION;
}

/////////////////////////////////////////////////////////////////////
//TypeA寻卡
/////////////////////////////////////////////////////////////////////
s16 GuiActive(void)
{
	s16 error = EMV_ERR_OK;

	//复位场强
	PcdSoftReset();
	DelayMs(100);
	error = readerPoll();

	if (EMV_ERR_OK != error)
	{
		return error;
	}

	/* Anticollision. */
	DelayMs(EMV_T_P);
	error = emvCollisionDetection();
	if (EMV_ERR_OK != error)
	{
		return error;
	}

	return EMV_ERR_OK;
}

s16 GuiAPDU(u8 *apdu, u16 apduLen, u8 *response, u16 *respLen)
{
	s16 error = EMV_ERR_OK;
	size_t responseLength = 0;
	error = emvTransceiveApdu(apdu, apduLen, response, 256, &responseLength);
	*respLen = responseLength;
	return error;
}

s16 GuiMaintain(void)
{
	s16 error = EMV_ERR_OK;
	UINT8 uid[10];
	UINT8 uidsize;

	if (g_ISO14443_4 == 1)
	{
		error = emvTransceiveNAK();
	}
	else
	{
		if (0)	//执行过认证命令，根据具体卡片类型进行读操作
		{

		}
		else	//未执行过认证命令，通过不断复位来判断
		{
			PcdHalt();
			error = PcdRequest(0x52, &g_cReceBuf[0]);
			if (error != MI_OK)
			{
				error = PcdRequest(0x52, &g_cReceBuf[0]);
			}
			if (error == MI_OK)
			{
				error = TypeaAnticoll(uid, &uidsize);
				if (error == MI_OK)
				{
					if ((uidsize != g_cUIDSize)
							|| (memcmp(uid, g_cUID, uidsize) != 0))	//不是同一张卡
					{
						error = EMV_ERR_PROTOCOL;
					}
					else
					{
						error = EMV_ERR_OK;
					}
				}
			}
		}
	}

	return error;
}

