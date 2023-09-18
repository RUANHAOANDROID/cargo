#ifndef _MC_LED_H_ 
#define _MC_LED_H_

#include <stdint.h>

/**
\if english
* \defgroup MCLED MCLED
* \brief BroadCasting LED, only #T80_L2 \sa TERM_TYPE
\else
* \defgroup MCLED 广告屏接口
* \brief 广告屏接口
\endif
 * @{
*/



#ifdef __cplusplus
extern "C"{
#endif // __cplusplus
/* additional device */
typedef enum{
	//MCCOLOR_BLACK=0,
	MCCOLOR_RED=1,
	MCCOLOR_GREEN,
	MCCOLOR_YELLOW,
	MCCOLOR_BLUE,
	MCCOLOR_PURPLE,
	MCCOLOR_LIGHT_BLUE,
	MCCOLOR_WHITE
}MCLED_COLOR;

typedef enum{
	MCLED_COLOR_BLACK=0,
	MCLED_COLOR_RED=1,
	MCLED_COLOR_GREEN,
	MCLED_COLOR_YELLOW,
	MCLED_COLOR_BLUE,
	MCLED_COLOR_PURPLE,
	MCLED_COLOR_LIGHT_BLUE,
	MCLED_COLOR_WHITE,
	MCLED_COLOR_MASK   =0x0F,

	MCLED_COLOR_REVERSE=0x10,
	MCLED_COLOR_CONFIG_MASK   =0xF0,

    MCLED_XIAOGUO_000     =0x0000,  //跟随
    MCLED_XIAOGUO_001     =0x0100,  //随机
    MCLED_XIAOGUO_002     =0x0200, //直接显示
    MCLED_XIAOGUO_003     =0x0300, //循环左移
    MCLED_XIAOGUO_004     =0x0400, //向上移入
    MCLED_XIAOGUO_005     =0x0500, //融入
    MCLED_XIAOGUO_006     =0x0600, //向右展开
    MCLED_XIAOGUO_007     =0x0700, //向下展开
    MCLED_XIAOGUO_008     =0x0800, //向左展开
	MCLED_XIAOGUO_009     =0x1000, //循环右移
	MCLED_EX2_NOT_CLRLINE     = 0x8000, //EX2：默认显示前清行
	MCLED_XIAOGUO_MASK    = 0xFF00,
//特效移动速度 MCLED_SPEED_00 极慢 ，约每秒移动2个点阵
//        MCLED_SPEED_06 普通，约每秒移动20个点阵
//        MCLED_SPEED_15 极快
	MCLED_SPEED_00        =0x000000,
	MCLED_SPEED_01        =0x010000,
	MCLED_SPEED_02        =0x020000,
	MCLED_SPEED_03        =0x030000,
	MCLED_SPEED_04        =0x040000,
	MCLED_SPEED_05        =0x050000,
	MCLED_SPEED_06        =0x060000,
	MCLED_SPEED_07        =0x070000,
	MCLED_SPEED_08        =0x080000,
	MCLED_SPEED_09        =0x090000,
	MCLED_SPEED_10        =0x0A0000,
	MCLED_SPEED_11        =0x0B0000,
	MCLED_SPEED_12        =0x0C0000,
	MCLED_SPEED_13        =0x0D0000,
	MCLED_SPEED_14        =0x0E0000,
	MCLED_SPEED_15        =0x0F0000,
	MCLED_SPEED_MASK      =0x0F0000,

   //reserve               =0xF00000,
	//特效停留时间，单位秒
	MCLED_STAY_TIME_00    =0x00000000,
	MCLED_STAY_TIME_01    =0x01000000,
	MCLED_STAY_TIME_02    =0x02000000,
	MCLED_STAY_TIME_03    =0x03000000,
	MCLED_STAY_TIME_04    =0x04000000,
	MCLED_STAY_TIME_05    =0x05000000,
	MCLED_STAY_TIME_06    =0x06000000,
	MCLED_STAY_TIME_07    =0x07000000,
	MCLED_STAY_TIME_08    =0x08000000,
	MCLED_STAY_TIME_09    =0x09000000,
	MCLED_STAY_TIME_10    =0x0A000000,
	//...
	MCLED_STAY_TIME_INFINITELY =0xFF000000, //无限停留 无限停留的区用来直接显示文字，没有特效。
	MCLED_STAY_TIME_MASK  =0xFF000000,

}MCLED_MODE;
/**
 * @brief 打开LED广告屏
 * @param[in] dev_name 设备端口名称 \li "/dev/ttymxc5"
 *
 */
int MCLED_Open(char *dev_name);

/*
* @brief MCLED_OpenEx 使用指定波特率来打开通信串口，仅支持38400 115200
* @param dev_name 0:表示"/dev/ttymxc5"
* @param baud :    38400
                   115200
* @return 0 success
          1 fail
         -2 param err;
*          
*/
int MCLED_OpenEx(char *dev_name,unsigned int baud);

/**
 * @brief 关闭LED广告屏
 * @retval 0 成功
 **/
int MCLED_Close(void);

/**
 * @note 显示UTF-8字符串
 * @brief 实时显示字符串
 * @param[in] disp_str 显示内容UTF-8字符串
 * @param[in] MCColor    字符串颜色 @sa #MCLED_COLOR
 * @param[in] yoffset    y坐标偏移
 * @retval 0 成功
 * @retval -1 失败
 */
int MCLED_Disp_UTF8(char *disp_str,MCLED_COLOR MCColor,int yoffset);


/**
 * @note 显示GBK字符串
 * @brief 实时显示字符串
 * @param[in] disp_str 显示内容GBK字符串
 * @param[in] MCColor    字符串颜色 @sa #MCLED_COLOR
 * @param[in] yoffset    y坐标偏移
 * @retval 0 成功
 * @retval -1 失败
 */
int MCLED_Disp_GBK(char *disp_str,MCLED_COLOR MCColor,int yoffset);
/**
 * @brief 实时显示字符串
 * @param[in] disp_str 显示内容unicode字符串
 * @param[in] len disp_str 长度
 * @param[in] MCColor    字符串颜色 @sa #MCLED_COLOR
 * @param[in] yoffset    y坐标偏移
 * @retval 0 成功
 * @retval -1 失败
 * @note 显示unicode(utf16-le)字符串
 */
int MCLED_Disp_UNICODE(char *disp_str,int len,MCLED_COLOR MCColor,int yoffset);

/**
 * @brief MCLED清屏,清行
 * @param[in] clr_line 清屏行号
 *                    0 清全屏
 *                    1 清1-4行
 * @retval 0 成功
 * @retval -1 失败
 * @retval -2 100ms内没收到指令返回数据(超时)
 */
int MCLED_ClearDisplay( uint8_t clr_line);

// void CL3000_test(char mode);

/**
 * @brief 以图片的形式显示汉字
 * @param[in] str_utf8
 * @param[in] mc_mode \sa MCLED_MODE
 * @param[in] yoffset y轴偏移
 * @note 建议使用MCLED_Disp_UTF8
 */
void MCLED_DispPic_UTF8(char *str_utf8,MCLED_MODE mc_mode,int yoffset);
/**
 * @brief 以图片的形式显示汉字
 * @param[in]  str_gbk
 * @param[in] mc_mode \sa MCLED_MODE
 * @param[in] yoffset y轴偏移
 * @note 建议使用MCLED_Disp_GBK
 */
void MCLED_DispPic_GBK(char *str_gbk,MCLED_MODE mc_mode,int yoffset);
/**
 * @brief 实时显示单色图片bmp，可以指定color，不涉及flash操作
 * @param[in] file_name 图片路径
 * @param[in] x 图片偏移
 * @param[in] y 图片偏移
 * @param[in] color 显示颜色 \sa MCLED_MODE
 * @note  图片的长，宽,建议使用8的倍数,只支持单色bmp格式图片
 */
void MCLED_Disp_Mono_Bmp(char* file_name,int x,int y,MCLED_MODE color);
/**
 * @brief 发送节目表信息，固定4个节目表，整屏64x64，一个节目表h固定16，显示汉字固定16x16尺寸
 * @param[in] utf8_str 显示字符，中文编码为utf8
 * @param[in] style 字符的特效，颜色 \sa MCLED_MODE
 * @param[in] line  0-3 显示位置
 * @note 仅写数据到ram,字符固定8x16的，可以多于8个字符
 * @return /li 0:成功
 *         /li -1 失败，参数错误
 */
int MCLED_Create_Programe_UTF8(char *utf8_str,MCLED_MODE style,unsigned char line);
/**
 * @brief 发送节目表信息，固定4个节目表，整屏64x64，一个节目表h固定16，显示汉字固定16x16尺寸
 * @param[in] gbk_str 显示字符,中文编码为gbk
 * @param[in] style 字符的特效，颜色 \sa MCLED_MODE
 * @param[in] line  0-3 显示位置
 * @note 仅写数据到ram,字符固定8x16的，可以多于8个字符
 * @return /li 0:成功
 *         /li -1 失败，参数错误
 */
int MCLED_Create_Programe_GBK(char *gbk_str,MCLED_MODE style,unsigned char line);
/**
 * @brief 发送节目表信息，固定4个节目表，整屏64x64，一个节目表h固定16，显示汉字固定16x16尺寸
 * @param[in] unicode_str 显示字符，unicode（utf-16le） 编码
 * @param[in] len 字符串长度
 * @param[in] style 字符的特效，颜色
 * @param[in] line  0-3 显示位置
 * @note 仅写数据到ram,字符固定8x16的，可以多于8个字符
 * @return /li 0:成功
 *         /li -1 失败，参数错误
 */
int MCLED_Create_Programe_UNICODE(char *unicode_str,int len,MCLED_MODE style,unsigned char line);
/**
 * @brief 发送节目表信息，固定4个节目表，整屏64x64，一个节目表高固定16
 * @param[in] file_name bmp单色图，一张图片宽度固定16*64
 * @param[in] style 字符的特效，颜色
 * @param[in] line  0-3 显示位置
 * @note 仅写数据到ram,如果需要显示较宽图片，需要逐图发送到同一line
 * @return /li 0:成功
 *         /li -1 失败，参数错误
 */
int MCLED_Create_Programe_BMP(char *file_name,MCLED_MODE style,unsigned char line);
/**
 * @brief 建立节目表开始
 * @note
 */
int MCLED_CreatePrograme_START(void);
/**
 * @brief 建立节目表完成,建立节目表之后调用
 * @note 慎用，本函数将ram数据写到到控制卡flash，频繁调用会耗尽flash擦写次数,节目表效果掉电保存
 */
int MCLED_CreatePrograme_END(void);
/**
 * @brief 以图片的形式显示汉字
  * @param[in] str_gbk GBK格式汉字
 * @param[in] mc_mode \sa MCLED_MODE
 * @param[in] xoffset x轴偏移
 * @param[in] yoffset y轴偏移
 * @param[in] rotate 旋转角度，仅支持0,90,180,270四个值
 * @note xoffset，yoffset是图片左上角的坐标
 */
void MCLED_DispPic_GBK_Ex(char *str_gbk,MCLED_MODE mc_mode,int xoffset,int yoffset,int rotate);
/**
 * @brief 以图片的形式显示汉字
  * @param[in] str_utf8 UTF8格式汉字
 * @param[in] mc_mode \sa MCLED_MODE
 * @param[in] xoffset x轴偏移
 * @param[in] yoffset y轴偏移
 * @param[in] rotate 旋转角度，仅支持0,90,180,270四个值
 * @note xoffset，yoffset是图片左上角的坐标
 */

void MCLED_DispPic_UTF8_Ex(char *str_utf8,MCLED_MODE mc_mode,int xoffset,int yoffset,int rotate);
typedef enum{
	//bit0-3
	/* //fc可替代
	LED_COLOR_BLACK=0,
	LED_COLOR_RED=1,
	LED_COLOR_GREEN,
	LED_COLOR_YELLOW,
	LED_COLOR_BLUE,
	LED_COLOR_PURPLE,
	LED_COLOR_LIGHT_BLUE,
	LED_COLOR_WHITE,
	LED_COLOR_MASK   =0x0F,
	*/
	
	//bit4-7
	LED_LEFT            = (0<<4),	/*!< 居左对齐,缺省 */
	LED_CENTER          = (1u<<5),	/*!< 居中对齐 */
	LED_RIGHT		    = (1u<<6),	/*!< 居右对齐 */
	LED_REVERT			= (1u<<7),	/*!< 反显,保留，可先清块再不清除显示 */

	//bit8-11
	LED_XIAOGUO_000     =(0u<<8),  /*!< 跟随     ,保留 */
    LED_XIAOGUO_001     =(1u<<8),  /*!< 随机     ,保留 */
    LED_XIAOGUO_002     =(2u<<8), /*!< 直接显示,保留 */
    LED_XIAOGUO_003     =(3u<<8), /*!< 循环左移 */
    LED_XIAOGUO_004     =(4u<<8), /*!< 向上移入,保留 */
    LED_XIAOGUO_005     =(5u<<8), /*!< 融入      ,保留 */
    LED_XIAOGUO_006     =(6u<<8), /*!< 向右展开,保留 */
    LED_XIAOGUO_007     =(7u<<8), /*!< 向下展开,保留 */
    LED_XIAOGUO_008     =(8u<<8), /*!< 向左展开,保留 */
    LED_XIAOGUO_009     =(9u<<8), /*!< 循环右移 */

	//bit12-15
	LED_EX2_NOT_CLRLINE    = (1u<<12), //EX2：默认显示前清行
	LED_XIAOGUO_MASK    = 0xFF00,
//特效移动速度 MCLED_SPEED_00 极慢 ，约每秒移动2个点阵
//        LED_SPEED_06 普通，约每秒移动20个点阵
//        LED_SPEED_15 极快
	//bit16-19
	LED_SPEED_00        =0x000000,	/*!< 慢 */
	LED_SPEED_01        =(0x1u<<16),
	LED_SPEED_02        =(0x2u<<16),
	LED_SPEED_03        =(0x3u<<16),
	LED_SPEED_04        =(0x4u<<16),
	LED_SPEED_05        =(0x5u<<16),
	LED_SPEED_06        =(0x6u<<16),
	LED_SPEED_07        =(0x7u<<16),
	LED_SPEED_08        =(0x8u<<16),
	LED_SPEED_09        =(0x9u<<16), /*!< 快 */
	//bit20-23
	LED_ANGLE_00        = 0x0000000,  /*!< 水平显示*/
	LED_ANGLE_90        = (0x1u<<20), /*!< 顺时针旋转90度显示*/
	LED_ANGLE_180       = (0x1u<<21), /*!< 顺时针旋转180度显示*/
	LED_ANGLE_270       = (0x1u<<22), /*!<顺时针旋转270度显示*/
	//bit 24-27
	LED_BOLD            = (1u<<24),  /*!< 字体加粗*/
	LED_ITALIC          = (1u<<25),  /*!<  RFU */
}MCLED_MODE_FT;



//int MCLED_DispPic_UTF8_Ex2(char *str_utf8,MCLED_MODE mc_mode,int xoffset,int yoffset,int rotate,int char_size);

//int MCLED_DispPic_UTF8_Ex3(char *str_utf8,MCLED_MODE mc_mode,int xoffset,int yoffset,int rotate,int char_size);

/**
 * @brief 写块数据
 * @param[in] x 块显示起始横坐标
 * @param[in] y 块显示起始纵坐标
 * @param[in] w 块显示宽度
 * @param[in] h 块显示高度
 * @param[in] block 显示内容
 * @param[in] mc_mode 显示模式 参考MCLED_MODE&MCLED_MODE_TF
 */
int MCLED_WriteBlock(int x,int y,int w,int h,uint8_t *block,MCLED_MODE mc_mode);

/**
 * @brief 把一个块清成一种颜色
 * @param[in] x 块显示起始横坐标
 * @param[in] y 块显示起始纵坐标
 * @param[in] w 块显示宽度
 * @param[in] h 块显示高度
 * @param[in] fc 显示颜色
 */
int MCLED_ClearBlock(int x,int y,int w,int h,uint32_t fc);

/**
 * @brief 显示UTF8-FreeType字符串，带效果
 * @param[in] char_size FreeType显示字体大小
 * @param[in] x1 显示起始左侧横坐标
 * @param[in] x2 显示起始右侧横坐标
 * @param[in] yoffset 纵向起始坐标
 * @param[in] text_utf8 显示内容
 * @param[in] fc 显示颜色
 * @param[in] mode 显示模式 参考MCLED_MODE_TF
 */
int led_utf8_display_align(int char_size,
                     int x1,int x2,int yoffset,
                     char *text_utf8,
                     uint32_t fc,
                     MCLED_MODE_FT mode
                  );
/**
 * @brief 清除led节目表
 */
void MCLED_ClearPlayList(void);

/**
 * @brief 停止滚动效果
 * @note：配合LED_XIAOGUO_003或LED_XIAOGUO_009使用
 *	led_utf8_display_align(16,0,-1,48,"新酷产品第一时间免费试玩，还有众多优质达人分享独到 ",RTGUI_RGB(255,255,0),LED_XIAOGUO_009|LED_SPEED_09);
 */
void mcled_disp_roll_end(void);

/**
 * @brief 使能滚动效果
 * @note：配合LED_XIAOGUO_003或LED_XIAOGUO_009使用
 *	led_utf8_display_align(16,0,-1,48,"新酷产品第一时间免费试玩，还有众多优质达人分享独到 ",RTGUI_RGB(255,255,0),LED_XIAOGUO_009|LED_SPEED_09);
 */
void mcled_disp_roll_start(void);

/** @}
* end of MCLED
*/

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* _MC_LED_H_ */
