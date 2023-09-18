
#ifndef PHALFELICA_H
#define PHALFELICA_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */


/** \defgroup Felica Felica
* \brief These Components implement the Felica commands.
* @{
*/



/** \defgroup ph_Error Error Code 
* \brief Common Error Codes
* @{
*/

/** \name Success Codes
*/
/** @{*/
#define PH_ERR_SUCCESS                  0x0000U /**< Returned in case of no error. */
#define PH_ERR_SUCCESS_CHAINING         0x0071U /**< Rx chaining is not complete, further action needed. */
#define PH_ERR_SUCCESS_ACK              0x0072U /**< Received ACK packet for a previous transmitted packet. */
#define PH_ERR_SUCCESS_INCOMPLETE_BYTE  0x0073U /**< An incomplete byte was received. */
#define PH_ERR_SUCCESS_DESELECTED       0x0074U /**< DSL is sent for de-selection of target. */
#define PH_ERR_SUCCESS_RELEASED         0x0075U /**< RLS is sent for release of target. */
#define PH_ERR_SUCCESS_INFO_RECEIVED    0x0076U /**< Received I PDU in response for an I PDU. */
/** @}*/

/** \name Communication Errors
*/
/** @{*/
#define PH_ERR_IO_TIMEOUT               0x0001U /**< No reply received, e.g. PICC removal. */
#define PH_ERR_INTEGRITY_ERROR          0x0002U /**< Wrong CRC or parity detected. */
#define PH_ERR_COLLISION_ERROR          0x0003U /**< A collision occurred. */
#define PH_ERR_BUFFER_OVERFLOW          0x0004U /**< Attempt to write beyond buffer size. */
#define PH_ERR_FRAMING_ERROR            0x0005U /**< Invalid frame format. */
#define PH_ERR_PROTOCOL_ERROR           0x0006U /**< Received response violates protocol. */
#define PH_ERR_AUTH_ERROR               0x0007U /**< Authentication error. */
#define PH_ERR_READ_WRITE_ERROR         0x0008U /**< A Read or Write error occurred in RAM/ROM or Flash. */
#define PH_ERR_TEMPERATURE_ERROR        0x0009U /**< The RC sensors signal over heating. */
#define PH_ERR_RF_ERROR                 0x000AU /**< Error due to RF. */
#define PH_ERR_INTERFACE_ERROR          0x000BU /**< An error occurred in RC communication. */
#define PH_ERR_LENGTH_ERROR             0x000CU /**< A length error occurred. */
#define PH_ERR_RESOURCE_ERROR           0x000DU /**< An resource error. */
#define PH_ERR_TX_NAK_ERROR             0x000EU /**< TX Rejected sanely by the counterpart. */
#define PH_ERR_RX_NAK_ERROR             0x000FU /**< RX request Rejected sanely by the counterpart. */
#define PH_ERR_EXT_RF_ERROR             0x0010U /**< Error due to External RF. */
#define PH_ERR_NOISE_ERROR              0x0011U /**< EMVCo EMD Noise Error. */
#define PH_ERR_ABORTED                  0x0012U /**< Used when HAL ShutDown is called. */
#define PH_ERR_INTERNAL_ERROR           0x007FU /**< An internal error occurred. */
/** @}*/

/** \name Parameter & Command Errors
*/
/** @{*/
#define PH_ERR_INVALID_DATA_PARAMS      0x0020U /**< Invalid data parameters supplied (layer id check failed). */
#define PH_ERR_INVALID_PARAMETER        0x0021U /**< Invalid parameter supplied. */
#define PH_ERR_PARAMETER_OVERFLOW       0x0022U /**< Reading/Writing a parameter would produce an overflow. */
#define PH_ERR_UNSUPPORTED_PARAMETER    0x0023U /**< Parameter not supported. */
#define PH_ERR_UNSUPPORTED_COMMAND      0x0024U /**< Command not supported. */
#define PH_ERR_USE_CONDITION            0x0025U /**< Condition of use not satisfied. */
#define PH_ERR_KEY                      0x0026U /**< A key error occurred. */
#define PH_ERR_OSAL_ERROR               0x0027U /**< Error occurred during initialization. */
/** @}*/

/** @}
* end of ph_Error
*/

/** \defgroup Command Command
 * @brief Felica interface
* @{
*/

/**
* \name Definitions of the time slots for the ReqC command
*/
/** @{*/
#define PHPAL_FELICA_NUMSLOTS_1     0x00    /**< Value for \c bNumSlots = 1. */
#define PHPAL_FELICA_NUMSLOTS_2     0x01    /**< Value for \c bNumSlots = 2. */
#define PHPAL_FELICA_NUMSLOTS_4     0x03    /**< Value for \c bNumSlots = 4. */
#define PHPAL_FELICA_NUMSLOTS_8     0x07    /**< Value for \c bNumSlots = 8. */
#define PHPAL_FELICA_NUMSLOTS_16    0x0F    /**< Value for \c bNumSlots = 16. */
/**@}*/

/**
* \name System Code
*/
/**@{*/
#define PHPAL_FELICA_SYSTEM_CODE_BYTE_0    0xFFU
#define PHPAL_FELICA_SYSTEM_CODE_BYTE_1    0xFFU
/** @}*/


/**
* \name Misc. Defines
*/
/**@{*/
#define PHAL_FELICA_MAX_SERVICES    121U    /**< Maximum number of services. */
/**@}*/

/** @}
* end of Command
*/

/**
* \brief When receiving the RequestResponse command, the VICC shall respond.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
uint16_t phalFelica_RequestResponse(
                                      uint8_t * pMode       /**< [Out] Current Card Mode. (0, 1, 2). */
                                      );

/**
* \brief When receiving the RequestService command, the VICC shall respond.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
uint16_t phalFelica_RequestService(
                                     uint8_t  bTxNumServices,   /**< [In] Number of services or areas within the command message. */
                                     uint8_t * pTxServiceList,  /**< [In] Service code or area code list within the command message. */
                                     uint8_t * pRxNumServices,  /**< [Out] Number of received services or areas. */
                                     uint8_t * pRxServiceList   /**< [Out] Received Service Key version or area version list, max 64 bytes. */
                                     );

/**
* \brief When receiving the Read command, the VICC shall respond.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
uint16_t phalFelica_Read(
                           uint8_t  bNumServices,       /**< [In] Number of Services. */
                           uint8_t * pServiceList,      /**< [In] List of Services. */
                           uint8_t  bTxNumBlocks,       /**< [In] Number of Blocks to send. */
                           uint8_t * pBlockList,        /**< [In] List of Blocks to read. */
                           uint8_t  bBlockListLength,   /**< [In] Number of Blocks to read. */
                           uint8_t * pRxNumBlocks,      /**< [Out] Number of received blocks. */
                           uint8_t * pBlockData         /**< [Out] Received Block data. */
                           );

/**
* \brief When receiving the Write command, the VICC shall respond.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
uint16_t phalFelica_Write(
                            uint8_t  bNumServices,      /**< [In] Number of Services. */
                            uint8_t * pServiceList,     /**< [In] List of Services. */
                            uint8_t  bNumBlocks,        /**< [In] Number of Blocks to send. */
                            uint8_t * pBlockList,       /**< [In] List of Blocks to write. */
                            uint8_t  bBlockListLength,  /**< [In] Number of Blocks to write. */
                            uint8_t * pBlockData        /**< [In] Block data to write. */
                            );


/**
* \brief This command activates a Felica card.
*
* \b Note: SystemCode 0xFFFF is passed to poll for all Felica Tags irrespective of the SystemCode.
*          If SystemCode is specified then only corresponding Tag will be Polled and Activated.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
uint16_t phalFelica_ActivateCard(
                                    uint8_t * pSystemCode,          /**< [In] System-Code; uint8_t[2]. */
                                    uint8_t bNumTimeSlots,          /**< [In] Number of timeslots to use. Refer to e.g. #PHPAL_FELICA_NUMSLOTS_1 for valid values. */
                                    uint8_t * pRxBuffer,            /**< [Out] 8 bytes NFCID2 + 8 bytes PAD; uint8_t[16]. */
                                    uint8_t * pRxLength,            /**< [Out] Length of received data. 0 or 16. */
                                    uint8_t * pMoreCardsAvailable   /**< [Out] Whether there are more cards in the field or not; uint8_t. */
                                    );






/** @}
* end of Felica
*/

#ifdef __cplusplus
} /* Extern C */
#endif

#endif /* PHALFELICA_H */
