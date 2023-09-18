#ifndef _TOOLSLIB_H_
#define _TOOLSLIB_H_

#ifdef __cplusplus
extern "C"{
#endif

/**
* \if english
* \defgroup tools tools library
* \else
* \defgroup tools 工具集
* \endif
* \brief tools library
* @{
*/




/**
 * @brief asc码数组转ulong型整数
 * @param[in]  asc asc码数组
 * @param[in]  asc_len asc码数组长度
 * 
 * @note CONV_AscLong("123456",6) ==> 123456 可用atol替代
 */
unsigned long CONV_AscLong(unsigned char *asc,unsigned int asc_len);

/**
 * @brief BCD码数组转ulong型整数
 * @param[in] bcd BCD码数组
 * @param[in] bcd_len 数组长度
 * @note CONV_BcdLong("\x00\x00\x23\x45",4) ==> 2345
 */
unsigned long CONV_BcdLong(unsigned char *bcd, unsigned int bcd_len);

/**
 * @brief hex数组转ulong型整数
 * @param[in] hex hex数组
 * @param[in] hex_len hex数组长度
 * @note CONV_HexLong("\x1A\xBC\x4E",3)==> 0x1ABC4E 注意与CONV_BcdLong区别
 */
unsigned long CONV_HexLong(unsigned char *hex,unsigned int hex_len);

/**
 * @brief hex数组转ascii码数组
 * @param[in] asc ascii码数组
 * @param[in] hex  hex数组
 * @param[in] asc_len ascii个数
 * @note CONV_HexAsc(dest,"\x12\xA4\xB5",6) ==> dest = {0x31,0x32,0x41,0x34,0x42,0x35}
 *       CONV_HexAsc(dest,"\x12\x34\x65",6) ==> dest = {0x31,0x32,0x33,0x34,0x36,0x35}
 */
void *CONV_HexAsc(unsigned char *asc,unsigned char *hex,unsigned int asc_len);


/**
 * @brief ascii码字符串转hex数组
 * @param[in] hex hex数组
 * @param[in] asc  ascii码数组
 * @param[in] asc_len ascii长度
 * 
 * @note CONV_AscHex(dest,"12A4B5",6) ==> dest = "\x12\xA4\xB5"
 *       CONV_AscHex(dest,"123465",6) ==> dest = "\x12\x34\x65"
 */
void *CONV_AscHex(unsigned char *hex,unsigned char *asc,unsigned int asc_len);

/**
 * @brief CRC32计算
 * @param[in] pucData     待计算数组
 * @param[in] Len         数据长度
 * @param[in] uiPreCRCValue 预计算值
 */
unsigned long CRC_Calc32( unsigned char * pucData, unsigned int Len, unsigned int uiPreCRCValue) ;

/* libdes.h */
/**
 * @brief 8字节数组DES加密
 * @param[in] pucInput 输入数据，8字节
 * @param[out] pucOutput 输出，8字节
 * @param[in] pucKey   密钥，8字节
 */
void Des_Encrypt(unsigned char *pucInput, unsigned char *pucOutput, unsigned char *pucKey);

/**
 * @brief 8字节数组DES解密
 * @param[in] pucInput 输入数据，8字节
 * @param[out] pucOutput 输出，8字节
 * @param[in] pucKey   密钥，8字节
 */
void Des_Decrypt(unsigned char *pucInput, unsigned char *pucOutput, unsigned char *pucKey);

/**
 * @brief 8字节数组TDES加密
 * @param[in] pucInput 输入数据，8字节
 * @param[out] pucOutput 输出，8字节
 * @param[in] pucKey   密钥，16字节
 */
void TDes_Encrypt(unsigned char *pucInput, unsigned char *pucOutput, unsigned char *pucKey);

/**
 * @brief 8字节数组TDES解密
 * @param[in] pucInput 输入数据，8字节
 * @param[out] pucOutput 输出，8字节
 * @param[in] pucKey   密钥，16字节
 */
void TDes_Decrypt(unsigned char *pucInput, unsigned char *pucOutput, unsigned char *pucKey);

/**
 * @brief CBC TDES加密
 * @param[in] pszIn 输入数据
 * @param[out] pszOut 输出
 * @param[in] iDataLen  数据长度
 * @param[in] pszKey 密钥
 * @param[in] iKeyLen 密钥长度
 * @param[in] pucIv iv值
 */
int  TDES_CBCEncrypt(unsigned char * pszIn,unsigned char * pszOut,int iDataLen,unsigned char * pszKey,int iKeyLen,unsigned char*pucIv);

/**
 * @brief CBC TDES解密
 * @param[in] pszIn 输入数据
 * @param[out] pszOut 输出
 * @param[in] iDataLen  数据长度
 * @param[in] pszKey 密钥
 * @param[in] iKeyLen 密钥长度
 * @param[in] pucIv iv值
 */
int  TDES_CBCDecrypt(unsigned char * pszIn,unsigned char * pszOut,int iDataLen,unsigned char * pszKey,int iKeyLen,unsigned char*pucIv);

/*libsm.h */

/**
 * @brief      sm2 非对称算法加密
 * @param[in] pucPlainHex   输入数据
 * @param[in] uiPlainLen   数据长度
 * @param[in] strPub      公钥
 * @param[out] pucEncipher    加密密文
 */
unsigned char SM2_ENC(unsigned char* pucPlainHex,unsigned int uiPlainLen,unsigned char* strPub,unsigned char*pucEncipher);



/**
 * @brief      sm2 非对称算法解密
 * @param[in] cipher    数据密文
 * @param[in] strPri      私钥
 * @param[out] pucPlainHex    解密明文
 */
unsigned char SM2_DEC(unsigned char* cipher,unsigned char* strPri,unsigned char *pucPlainHex);


/**
 * @brief      sm2 非对称算法签名
 * @param[in] pucData   输入数据
 * @param[in] uiDataLen   数据长度
 * @param[in] strPri      私钥
 * @param[out] strSign    签名数据
 */
unsigned char SM2_Sign(unsigned char* pucData,unsigned int uiDataLen,unsigned char* strPri,unsigned char *strSign);

/**
 * @brief      sm2 非对称算法验证
 * @param[in] pucData   输入数据
 * @param[in] uiInLen   数据长度
 * @param[in] Signature 签名
 * @param[in] strPub    公钥
 */
unsigned char  SM2_Verify(unsigned char* pucData,unsigned int uiInLen,unsigned char* Signature,unsigned char* strPub);

/**
 * @brief      PBOC国密算法验证
 * @param[in]  pucPub  公钥
 * @param[in] uiPubicKeyLen 公钥长度，一般为64
 * @param[in] pucInData 输入数据
 * @param[in] uiInLen   输入数据长度
 * @param[in] pucDigest 待验证摘要数据
 * @return 0 验证成功
 * @return other 验证失败
 */
unsigned char PBOC_SMHash(unsigned char *pucPub,unsigned int uiPubicKeyLen,unsigned char *pucInData,unsigned int uiInLen,unsigned char *pucDigest);


/**
 * @brief      sm3 hash 计算
 * @param[in]  InMessage 输入数据
 * @param[in]  uiInLen 输入数据长度
 * @param[out] strDigest 输出数据
 */
int SM3_Compute(unsigned char* InMessage,unsigned int uiInLen,unsigned char*strDigest);

/**
 * @brief SM4加密
 * @param[in]  pKey sm4密钥
 * @param[in]  pDataIn 输入数据
 * @param[out] pDataOut 输出数据
 */
void SM4_Encrypt(unsigned char *pKey, unsigned char *pDataIn, unsigned char *pDataOut);

/**
 * @brief SM4解密
 * @param[in]  pKey sm4密钥
 * @param[in]  pDataIn 输入数据
 * @param[out] pDataOut 输出数据
 */
void SM4_Decrypt(unsigned char *pKey, unsigned char *pDataIn, unsigned char *pDataOut);



/**
 * @brief RSA计算
 * @param[in] pucPublicKey   RSA模
 * @param[in] uiPublicKeyLen RSA模长度
 * @param[in] pucPublicKeyExp RSA公钥/私钥
 * @param[in] uiPublicKeyExpLen RSA公钥/私钥长度
 * @param[in] pucIn             输入数据
 * @param[in] uiInLen           输入数据长度，等于uiPublicKeyLen
 * @param[in] pucOut            计算结果
 */
uint8_t RSA_PublicDecrypt(uint8_t *pucPublicKey,unsigned int uiPublicKeyLen,
					uint8_t *pucPublicKeyExp,unsigned int uiPublicKeyExpLen,
					uint8_t *pucIn,unsigned int uiInLen,
					uint8_t *pucOut);
/**
 * @brief md5计算
 * @param[in] input    输入数据
 * @param[in] inputlen 输入长度
 * @param[out] digest  数据md5摘要
 */
void MD5_Sum(unsigned int *input,unsigned int inputlen,unsigned char digest[16]);

/**
 * @brief SHA1计算
 * @param[in] pucInData    输入数据
 * @param[in] uiInLen 输入长度
 * @param[out] pucDigest  SHA1摘要
 */
void SHA1_Compute(unsigned char *pucInData,unsigned int uiInLen,unsigned char *pucDigest);

/**
 * @brief SHA256计算
 * @param[in] pucInData    输入数据
 * @param[in] uiInLen 输入长度
 * @param[out] pucDigest  SHA256 摘要
 */
void SHA256_Compute(unsigned char *pucInData,unsigned int uiInLen,unsigned char *pucDigest);

/**
 * @brief SHA512计算
 * @param[in] pucInData    输入数据
 * @param[in] uiInLen 输入长度
 * @param[out] pucDigest  SHA512 摘要
 */
void SHA512_Compute(unsigned char *pucInData,unsigned int uiInLen,unsigned char *pucDigest);

/** @}
* end of tools
*/

#ifdef __cplusplus
}
#endif


#endif
