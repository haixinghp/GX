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
 
/*! \file emv_layer4.h
 *
 * \author Oliver Regenfelder
 *
 * \brief EMV compliant ISO14443-4 data transmission.
 */

#ifndef EMV_LAYER4_H
#define EMV_LAYER4_H

#include"type.h"
#include"stddef.h"

/*
******************************************************************************
* INCLUDES
******************************************************************************
*/

//xjh #include <stddef.h>

//xjh #include "ams_types.h"

//xjh #include "emv_picc.h"
//xjh #include "emv_error_codes.h"

/*
******************************************************************************
* DEFINES
******************************************************************************
*/

/*
******************************************************************************
* GLOBAL MACROS
******************************************************************************
*/
//指定PCD接收缓冲值
#ifndef FSDI
#define FSDI 4		// 5对应64字节
#endif

#define FSD 48

/*
******************************************************************************
* GLOBAL DATA TYPES
******************************************************************************
*/

/*
******************************************************************************
* GLOBAL VARIABLE DECLARATIONS
******************************************************************************
*/
extern unsigned char g_ULKey[16];
extern unsigned char g_ULIv[8];
extern unsigned char g_ULRndA[8];
extern unsigned char g_ULRndB[8];
extern unsigned char g_ULAuthFlg;

extern unsigned char g_PlusPCDcap2[6];
extern unsigned char g_PlusPICCcap2[6];
extern unsigned short g_PlusWCtr;
extern unsigned short g_PlusRCtr;
extern unsigned char g_PlusRndA[16];
extern unsigned char g_PlusRndB[16];
extern unsigned char g_PlusTI[4];
extern unsigned char g_AuthenticationK[16];
extern unsigned char g_PlusKenc[16];
extern unsigned char g_PlusKmac[16];

extern unsigned short g_cFSC;
extern unsigned char g_ISO14443_4;
extern unsigned char g_cRstPara[15];		//非接卡复位参数，用于PCSC
extern unsigned char g_cRstParaLen;		//非接卡复位参数长度
extern int emvTypeA;
extern int emvTypeB;
extern unsigned char g_cBlockNumber;
extern unsigned char g_cCidNad;                                    //
extern unsigned char g_cPiccPcd; /*!< Datarate bits PICC->PCD. */
extern unsigned char g_cPcdPicc; /*!< Datarate bits PCD->PICC. */
extern unsigned char g_cFWI;
extern unsigned char g_cSFGI;                                       //
extern const unsigned short FSC_Table[18];

/*
******************************************************************************
* GLOBAL FUNCTION PROTOTYPES
******************************************************************************
*/

/*! \ingroup emvMain
 *****************************************************************************
 * \brief Initializes the ISO14443-4 protocol layer (EMV layer 4).
 *
 * Resets the ISO14443-4 protocoll stack and initializes it for communicaiton
 * with a specific card.
 *
 * \note The PICC must be in the active state for layer-4 data transmissions
 * to work.
 *
 * \param[in] picc The target PICC for ISO14443-4 data transmissions.
 *
 * \return EMV_ERR_OK: No error, operation successful.
 *****************************************************************************
 */

/*! \ingroup emvMain
 *****************************************************************************
 * \brief Transeive an APDU.
 *
 * Send a command APDI and receives the response APDU from the PICC.
 *
 * \param[in] apdu Command APDU.
 * \param[in] apduLength Length of the command APDI in bytes.
 * \param[out] response Buffer for the response APDU.
 * \param[in] maxResponseLength Size of the response APDI buffer in bytes.
 * \param[out] responseLength The length of the received response APDU in
 * bytes.
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
s16 emvTransceiveApdu(const u8 *apdu, size_t apduLength, u8 *response,
        size_t maxResponseLength, size_t *responseLength);
s16 GuiActive(void);

#endif /* EMV_LAYER4_H */
