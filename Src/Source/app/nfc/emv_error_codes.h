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
 
/*! \file emv_error_codes.h
 *
 * \author Oliver Regenfelder
 *
 * \brief Error codes specific to the EMV code.
 */

#ifndef EMV_ERROR_CODES_H
#define EMV_ERROR_CODES_H


/*
******************************************************************************
* INCLUDES
******************************************************************************
*/

/*
******************************************************************************
* DEFINES
******************************************************************************
*/
/*! \name EMV HAL error codes. */
/**@{*/
/*! \ingroup emvHal */
#define EMV_HAL_ERR_OK        0 /*!< No error. */
#define EMV_HAL_ERR_TIMEOUT  -1 /*!< Timeout error. */
#define EMV_HAL_ERR_OVERFLOW -2 /*!< Buffer overflow error. */
#define EMV_HAL_ERR_ENCODING -3 /*!< Encoding error (CRC, parity, bit encoding, or framing). */
#define EMV_HAL_ERR_PARAM    -4 /*!< Invalid function call parameter. */
/**@}*/

/*! \name EMV module error codes
 *****************************************************************************
 * Error codes used by the EMV module.
 *****************************************************************************
 */
/**@{*/
/*! \ingroup emv
 */
#define EMV_ERR_OK               0 /*!< No error occured. */
#define EMV_ERR_COLLISION       -1 /*!< Received a card response with a bit collision. */
#define EMV_ERR_PROTOCOL        -2 /*!< Received a card response with a protocol error. */
#define EMV_ERR_TRANSMISSION    -3 /*!< Received a card response with a transmission error. */
#define EMV_ERR_TIMEOUT         -4 /*!< Timeout occured while waiting for a card response. */
#define EMV_ERR_INTERNAL        -5 /*!< EMV software stack internal error. */
#define EMV_ERR_STOPPED         -6 /*!< Stop current operation request received. */
/**@}*/

/*! \defgroup errorcodes Basic application error codes
 * Error codes to be used within the application.
 * They are represented by an s8
 */
#define ERR_NONE   0 /*!< \ingroup errorcodes
               no error occured */
#define ERR_NOMEM -1 /*!< \ingroup errorcodes
               not enough memory to perform the requested
               operation */
#define ERR_BUSY  -2 /*!< \ingroup errorcodes
               device or resource busy */
#define ERR_IO    -3 /*!< \ingroup errorcodes
                generic IO error */
#define ERR_TIMEOUT -4 /*!< \ingroup errorcodes
                error due to timeout */
#define ERR_REQUEST -5 /*!< \ingroup errorcodes
                invalid request or requested
                function can't be executed at the moment */
#define ERR_NOMSG -6 /*!< \ingroup errorcodes
                       No message of desired type */
#define ERR_PARAM -7 /*!< \ingroup errorcodes
                       Parameter error */

#define ERR_LAST_ERROR -32

/*
******************************************************************************
* GLOBAL MACROS
******************************************************************************
*/

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

/*
******************************************************************************
* GLOBAL FUNCTION PROTOTYPES
******************************************************************************
*/


#endif /* EMV_ERROR_CODES_H */
