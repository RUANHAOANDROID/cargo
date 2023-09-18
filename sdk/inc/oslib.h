#ifndef _OSLIB_H_ 
#define _OSLIB_H_ 

#include <stdint.h>

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

/**
* \ingroup DISPLAY
* \brief DISPLAY Parameter
*/
/** @{*/

//状态表
#define LCD_SUCESS              0x00   //success
#define LCD_BUSY                0x01   //busy
#define LCD_ERR_PARA            0x02   //params error 
#define LCD_ERR_FONT            0x03   //font error 


//#define LCDICONTYPE_MAXNB		6
#define LCDDATE_LEN				13
/*
\enum LCD_ICON lcd icon
*/
typedef enum{
ICON_MINNUM		= 0x00,
ICON_SIGNAL		= 0x00,
ICON_TELEPHONE	= 0x01,
ICON_MESSAGE	= 0x02,
ICON_PRINTER	= 0x03,
ICON_MAGCARD	= 0x04,
ICON_ICCCARD	= 0x05,
ICON_BATTERY	= 0x06,
ICON_DATETIME	= 0x07,
ICON_UPICON		= 0x08,
ICON_DOWNICON	= 0x09,
ICON_LOCK		= 0x0A,
ICON_USB		= 0x0B,
ICON_BT			= 0x0C,
ICON_GPS        = 0x0D,
ICON_NETWORK    = 0x0E,
ICON_WIFI		= 0X0F,
                
ICON_MAXNUM		= 0x0F,
}LCD_ICON;
typedef unsigned short ushort;
typedef enum{
DISP_BIGFONT        = 0x0000,	/*!< bit font \li Enlglish 8x16 \li Chinese 16X16 \li default */
DISP_LEFT           = 0x0000,	/*!< left align, default */
DISP_FONT6X8        = 0x0001,	/*!< small font , English */
DISP_FONT12         = 0x0002,	/*!< middle font \li English 6x12 \li Chinese 12X12*/
DISP_CENTER         = 0x0020,	/*!< center align  */
DISP_RIGHT		    = 0x0040,	/*!< right align */
DISP_ALIGN_MASK     = 0x0060,
DISP_REVERT			= 0x0080,	/*!< reverse display */
DISP_UNDERLINE      = 0x0100, /*!< display with under line */
DISP_CLRLINE        = 0x0200, /* clear line */
DISP_FONT24         = 0x0400, /* 24x24 */
DISP_FONT32         = 0x0800, /* 32x32 */
DISP_GBK            = 0x1000, /* display with GBK */
}LCDMODE;

#define RTGUI_ARGB(a, r, g, b)  \
        ((uint32_t)(((uint8_t)(b)|\
        (((unsigned long)(uint8_t)(g))<<8))|\
        (((unsigned long)(uint8_t)(r))<<16)|\
        (((unsigned long)(uint8_t)(a))<<24)))

#define RTGUI_RGB(r, g, b)  RTGUI_ARGB(0, (r), (g), (b))
        
#define  CLOSE       0    /*!< close icon(all of the icons) */
#define  OPEN        1    /*!< display icon(printer, IC card, lock, battery, up, down) */

#define _RESERVER_STATUS_BAR 16 /* area of display icon */


typedef enum{
    ROW0 = 0,
    ROW1 = 2,
    ROW2 = 4,
    ROW3 = 6,
    ROW4 = 8,
    ROW5 = 10,
    ROW6 = 12,
    ROW7 = 14,
    ROW8 = 16,
}LCD_ROW;

#define LCD_WIDTH 600
#define LCD_HEIGHT 1024

#define RGB888(r, g, b)  \
        ((uint32_t)(((uint8_t)(b)|\
        (((unsigned long)(uint8_t)(g))<<8))|\
        (((unsigned long)(uint8_t)(r))<<16)))

#define RGB_B(c)  ((c) & 0xff)
#define RGB_G(c)  (((c) >> 8)  & 0xff)
#define RGB_R(c)  (((c) >> 16) & 0xff)
#define RGB888_RGB565(r,g,b) ((((r) >> 3) << 11) | (((g) >> 2) << 5) | ((b) >> 3))

typedef enum{
	ALIGN_LEFT = 0,
	ALIGN_CENTER,
	ALIGN_RIGHT
}XFT_ALIGN;
/** @}*/

/**
* \ingroup ICC
* \brief ICC Parameter
*/
/** @{*/

#define OK              0
#define ICCERR_SUCCESS						0x00 /**< IC Card Success */
#define ICCERR_NO_CARD						0x01 /**< IC Card Not Present */
#define ICCERR_ATR_ERROR					0x02 /**< IC Card Faile to Get ATR */
#define ICCERR_INVALID_TYPE					0x03 /**< IC Card Slot invalid */
#define ICCERR_DO_WARM_RESET				0x04 /**< IC Card Warm Reset Failed */
#define ICCERR_RECEIVE_TIMEOUT				0x05 /**< */
#define ICCERR_EXCHANGE							0x06 /**< IC Card Apdu Error*/
#define ICCERR_NOPOWER						0x07 /**< IC Card Not Powered */
#define ICCERR_OPEN 0x08  /**< IC Open Error*/


typedef enum
{
	ICC_USER = 1,	/**< Contact Card 1 */
	ICC_CL,
	ICC_SIM1,
	ICC_SIM2,
	ICC_SIM3,
	ICC_SIM4,
	ICC_SIM5,
    ICC_ESAM,  /**< ESAM Crypto Module */
	ICC_USER2, /**< Contact Card 2 */
	ICC_INVALID = ICC_USER2,
    ICC_MAX_READER=ICC_USER2,
	ICC_MAG = 0x80,/*< Magstripe card */
}ICC_TYPE;

typedef enum{
	BAUD_9600  	= 0,
	BAUD_19200  = 1,
	BAUD_38400  = 2,
	BAUD_57600  = 3,
	BAUD_115200 = 4,
	BAUD_55800  = 5,
	BAUD_111600 = 6,
	BAUD_223200 = 7,
	BAUD_446400 = 8,
}ICC_BAUD;

typedef enum{

 VOLT_5V  =  0x00,   /**< 5V */
 VOLT_3V3 = 0x01,    /**< 3V */
 VOLT_1V8 = 0x02,    /**< 1.8V */

CPU_MCLK_DEFAULT  = ( 0 << 2), /**< only #P18_L2S support MCLK */
CPU_MCLK_4MHZ     = ( 4 << 2),
CPU_MCLK_5MHZ     = ( 5 << 2),
CPU_MCLK_6MHZ     = ( 6 << 2),
CPU_MCLK_8MHZ     = ( 8 << 2),
CPU_MCLK_10MHZ    = (10 << 2),
CPU_MCLK_12MHZ    = (12 << 2),
CPU_MCLK_16MHZ    = (16 << 2),
CPU_MCLK_20MHZ    = (20 << 2),

 PPS_SUPPORT = 0x80, /**< 
 \if english
 BIT8  1:support PPS,0: don't support, default
 \else
 BIT8  1:支持PPS,0:不支持（缺省) 
 \endif
 */
}ICC_VOLT;

#define     SAK_TypeA_TCL				0x20  
#define     SAK_MiFare1K				0x08 
#define     SAK_MiFare4K				0x18 
#define     SAK_MiFareUltralight		0x00
#define     SAK_MiFareMINI				0x09
#define     SAK_XIZHOU_CITIZEN_CARD		0x53

typedef enum{
TYPE_NO_PICC                = 0x00, /**< No Picc Card */
TYPE_A_TCL                  = 0x11, /**< Type A Card */
TYPE_A_MIFARE               = 0x12, /**< Mifare Card */
TYPE_A_MIFARE_1K            = 0x13, /**< 1K Mifare Card */
TYPE_A_MIFARE_4K            = 0x14, /**< 4K Mifare Card */
TYPE_A_MIFARE_Ultralight    = 0x15, /**< Ultralight Card */
TYPE_A_MiFareMINI			= 0x16, /**< Mifare mini Card */
TYPE_A_Ntag_21x             = 0x18, /**< Ntag Card */
TYPE_A_OTHERS_MEM			= 0x1F, /**< Other Memory Card */
TYPE_A_MASK                 = 0x10, /**< */

TYPE_B_TCL                  = 0x21, /**< type B Card */
TYPE_B_OTHER                = 0x22, /**< Other B Card, such as ID Card*/
TYPE_B_MASK                 = 0x20,

TYPE_C_TCL                  = 0x41, /**< Type C Card, i.e Felica Card */

TYPE_UNKNOWN                = 0xFF,
}PICC_CARD_TYPE;
/** @}*/

/**
* \ingroup LED
* \brief LED Parameter
*/
/** @{*/

typedef enum
{
	LED_BLUE      = 0x01,
	LED_YELLOW    = 0x02,
	LED_GREEN     = 0x04,
	LED_RED       = 0x08,
    LED_BACKRED   = 0x10,
    LED_BACKGREEN = 0x20,
    LED_RF  =       0x40,
	LED_QR  =       0x80,
	LED_ALL = 0xFF,
}LED_TYPE;
/** @}*/

/**
* \ingroup System
* \brief System Parameter
*/
/** @{*/

typedef struct __gprmc__  
{  
   uint32_t time;/**< time of gps fixed location */
   char pos_state;/**< status of gps */
   float latitude;/**< latitude */
   float longitude;/**< longitude */
   float speed; /**<  speed */
   float direction;/**< azimuth */
   uint32_t date;  /**< date  */
   float declination; /**<  magnetic declination */
   char dd;  
   char mode;/**<  mode of GPS */
   char hemisphere[2+1];  /**< hemisphere info */
   int sate_num;  /**<  number of satellite */
   char quality; /**< signal stength of gps */
   char date_time[14+1]; /**<  local time YYYYMMDDHHMMSS */
   double latitude_wgs84;/**< wgs84 latitude */
   double longitude_wgs84;/**< wgs84 longitude */
   char strgps[1024];
}GNRMC;

typedef struct __gpgsa__  
{  
   int mode1;			/**<  M=manual，A=auto */
   int mode2;			/**< 1=didn't fix position，2=2D fix position，3=3D fix position*/
   int satelliteID1;	/**< satellite ID1 */
   int satelliteID2;	/**< satellite ID2 */
   int satelliteID3;	/**< satellite ID3 */
   int satelliteID4;	/**< satellite ID4 */
   int satelliteID5;	/**< satellite ID5 */
   int satelliteID6;	/**< satellite ID6 */
   int satelliteID7;	/**< satellite ID7 */
   int satelliteID8;	/**< satellite ID8 */
   int satelliteID9;	/**< satellite ID9 */
   int satelliteID10;	/**< satellite ID10 */
   int satelliteID11;	/**< satellite ID11 */
   int satelliteID12;	/**< satellite ID12 */
   float pdop;			/**< Position accuracy dilution value PDOP: 0.5 ~ 99.9*/
   float hdop; 			/**< Horizontal precision dilution value HDOP:0.5 ~ 99.9 */
   float vdop;			/**< Vertical precision dilution value VDOP:0.5 ~ 99.9 */
   uint32_t Checksum;  /**< check sum  */
}GPGSA;

typedef struct __gpgsv__  
{  
   int numbermsg;			/**< total number of message  */
   int sequencenum;			/**< message id*/
   int satellitesnum;		/**< total number of satellite */
   int satelliteID1;		/**< satelliteID1 */
   int elevation1; 			/**< satellite elevation */
   int azimuth1;			/**< satellite azimuth */
   int SNR1;  				/**< signal to noise ratio 1:00 ~ 99 dB；if didn't detect signal, this value will be none */
   int satelliteID2;		/**< satelliteID2 */
   int elevation2; 			/**< satellite elevation */
   int azimuth2;			/**< satellite azimuth */
   int SNR2;  				/**< ignal to noise ratio 2:00 ~ 99 dB；if didn't detect signal, this value will be none*/
   int satelliteID3;		/**< satelliteID3 */
   int elevation3; 			/**< satellite elevation */
   int azimuth3;			/**< satellite azimuth */
   int SNR3;  				/**< signal to noise ratio 3:00 ~ 99 dB；if didn't detect signal, this value will be none*/
   uint32_t Checksum;  		/**< check sum  */
}GPGSV;
/** @}*/

/**
* \ingroup Mifare
* \brief Mifare Parameter
*/
/** @{*/
/** 
* \name mifare error define  
*/
/** @{*/
#define MIFARE_SUCCESS                 0 
#define MIFARE_ERROR                   1
#define MIFARE_PARA_ERROR              2
#define MIFARE_WALLETFORMAT_ERROR      3
/** @}*/
/** @}*/

/**
* \ingroup system
*/
/** @{*/
#define SYS_OPENDEVICE -0001
#define SYS_TIMEOUT -2001

typedef enum{
_KEY_0			= 0x30, /**< key 0 */
_KEY_1			= 0x31, /**< key 1 */
_KEY_2			= 0x32,	/**< key 2 */
_KEY_3			= 0x33,	/**< key 3 */
_KEY_4			= 0x34,	/**< key 4 */
_KEY_5			= 0x35,	/**< key 5 */
_KEY_6			= 0x36,	/**< key 6 */
_KEY_7			= 0x37,	/**< key 7 */
_KEY_8			= 0x38,	/**< key 8 */
_KEY_9			= 0x39,	/**< key 9 */

_KEY_ENTER       = 0x3A,	/**< Enter Key */
_KEY_CANCEL		= 0x3B,	/**< Cancel Key */
_KEY_CLEAR       = 0x3D,	/**< Clear/Backspace Key*/
_KEY_00_PT       = 0x3E,	/**< Feed */
_KEY_BCKSP       = 0x3D,	/**< BackSpace */
_KEY_F1          = 0x40,	/**< F1 Key */
_KEY_F2          = 0x41,
_KEY_F3          = 0x42,
_KEY_F4          = 0x43,
_KEY_F5          = 0x44,
_KEY_F6          = 0x45,
_KEY_F7          = 0x46,
_KEY_F8          = 0x47,
_KEY_F9          = 0x48,
_KEY_F           = 0x45,	/**< =KEY_F6 */
_KEY_R           = 0x46,	/**< =KEY_F7 */

_KEY_SYS         = 0x3F,	/**<  */
_KEY_PAPER_FEED  = 0x40,	/**< Feed */
_KEY_MENU		= 0x50,	/**< Menu */
_KEY_UP			= 0x60,	/**< Up */
_KEY_DOWN		= 0x61,	/**< Down*/
_KEY_LEFT 		= 0x62,	/**< Left */
_KEY_RIGHT		= 0x63,	/**< Right*/

_KEY_ALPHA		= 0xF2,	/**< '*' Key*/
_KEY_JIN			= 0xF3,	/**< '#' Key */
_KEY_ABORT		= 0xFF,	/**< used-defined, */

}KEYVALUE;

typedef enum{
	T80_A2 = 0,
	P18_L2,
	T80_L2,
	P18_L2C,
    P18_L2S=5,
	P18_L2L = 9,
	TERM_TYPE_INVALID = 0x0A
}TERM_TYPE;
/** @}*/

/**
 * \if english
 * \defgroup Display Display
 * \brief Screen display api
 * \else
 * \defgroup Display Display
 * \brief 显示接口
 * \endif
* @{
*/

/**
 * \if english
 * @brief Initialize Display Screen
 * \else
 * @brief 显示屏初始化
 * \endif
 */
int LCD_Init(void);

/**
 * \if english
 * @brief Get Screen Height
 * \else
 * @brief 获取显示屏点阵高度
 * \endif
 */
uint32_t LCD_GetHeight(void);

/**
 * \if english
 * @brief Get Screen Width
 * \else
 * @brief 获取显示屏点阵宽度
 * \endif
 */
uint32_t LCD_GetWidth(void);

/**
* \if english
* @brief Clear full-screen
* @param [in] color color to cleared
* 		     for mono-screen
*            \li 0 Clear
*           \li non-zero display dot
*            for color-screen
*            ARGB 32 bit color
* \else
* @brief 清除全屏
* @param [in] color 清除使用的颜色
* 	           对于黑白屏
*            \li 0 清除
*            \li 非0 反显颜色
*            对于彩屏
*            ARGB 32 bit 颜色
* \endif
*/
uint8_t LCD_ClearScreen(uint32_t color);

/**
 * \if english
 * @brief clear display aread,not include status bar
 * \else
 * @brief 清除可显区域，不包括状态栏
 * \endif
 */
uint8_t LCD_ClearAll(void);

/**
 * \if english
 * @brief clear status bar
 * @note 12-dot
 * \else
 * @brief 清除状态栏(图标栏)
 * @note 12行点阵
 * \endif
 */
void LCD_ClearIconLine(void);
/**
 * \if  english
 * @brief Screen Display
 * @param [in] row  y [0~LCD_HEIGHT)
 * @param [in] mode display parameter \sa LCDMODE
 * @param [in] fmt  String to display, encoded in UTF-8
 * @note for GBK-encoded string, use #DISP_GBK
 * \else
 * @brief 屏幕显示
 * @param [in] row  y方向坐标 [0~LCD_HEIGHT)
 * @param [in] mode 显示模式参数 \sa LCDMODE
 * @param [in] fmt  待显示字符串，默认UTF-8编码，
 * @note 如果要显示GBK字符串，mode 需要使用DISP_GBK
 * \endif
 */
uint8_t LCD_Printf(uint16_t row,uint32_t mode,void *fmt,...);

/**
 * \if  english
 * @brief Clear few rows
 * @param [in] pixel_y y [0~LCD_HEIGHT)
 * @param [in] height dot height to clear
 * \else
 * @brief 清多行
 * @param [in] pixel_y y方向坐标 [0~LCD_HEIGHT)
 * @param [in] height 点阵高度
 * \endif
 */
uint8_t LCD_Clear_rows(uint16_t pixel_y, uint16_t height);

/**
 * \if  english
 * @brief Display GB1803 encoded string
 * @param [in] str     String to display
 * @param [in] pixel_y y [0~LCD_HEIGHT)
 * @param [in] pixel_x x [0~LCD_WIDTH)
 * @param [in] mode Display mode \sa LCDMODE
 * \else
 * @brief 单行显示GB18030字符串
 * @param [in] str     待显示字符串
 * @param [in] pixel_y y方向坐标 [0~LCD_HEIGHT)
 * @param [in] pixel_x x方向坐标 [0~LCD_WIDTH)
 * @param [in] mode 显示模式参数 \sa LCDMODE
 * \endif
 */
uint8_t LCD_Display_Row(char *str,uint16_t pixel_y,uint16_t pixel_x,unsigned int mode);

/**
 * \if  english
 * @brief 显示icon
 * @param [in] ucIcon icon index
 * @param [in] bOnOff display on or off
 * \else
 * @brief 显示icon
 * @param [in] ucIcon 待显示icon
 * @param [in] bOnOff 亮/灭
 * \endif
 */
void LCD_DisplayIcon(LCD_ICON ucIcon,uint8_t bOnOff);


/**
 * @brief fill ellipse graphics 
 */
void LCD_FillEllipse(int xpos, int ypos, int a, int b,unsigned char mode);

/**
 * @brief draw ellipse graphics 
 */
void LCD_DrawEllipse(unsigned char xpos,unsigned char ypos,unsigned char a,unsigned char b,unsigned char mode);

/**
 * @brief fill rectangle graphics 
 */
void LCD_FillRect(uint16_t xstart,uint16_t ystart,uint16_t xlen, uint16_t ylen ,unsigned char mode);

/**
 * @brief draw rectangle graphics 
 */
uint8_t LCD_DrawRect(uint16_t xstart,uint16_t ystart,uint16_t xlen,uint16_t ylen,void *color);

/**
 * \if  english
 * @brief Display logo
 * @param [in] x0  x-coordinate [0-LCD_WIDTH-1]
 * @param [in] y0  y-coordinate [0 * 8 - LCD_HEIGHT]
 * @param [in] width  width of logo
 * @param [in] height  height of logo
 * @param [in] logo_data logo data 
 * @param [in] ucMode reserved
 * \else
 * @brief 显示单色图标
 * @param [in] x0  图标显示ｘ坐标 [0-LCD_WIDTH-1]
 * @param [in] y0  图标显示y坐标 [0 * 8 - LCD_HEIGHT]
 * @param [in] width  图标显示宽度
 * @param [in] height  图标显示高度
 * @param [in] logo_data 待显示图标数据
 * @param [in] ucMode 保留
 * \endif
 */
void LCD_DisplayLogo(uint32_t x0,unsigned int y0,                       
        unsigned int width,
        unsigned int height,
        unsigned char *logo_data,unsigned char ucMode);

/**
 * \if  english
 * @brief Clear screen block
 * @param [in] x0  start x-coordinate
 * @param [in] y0  start y-coordinate
 * @param [in] w width of block
 * @param [in] h height of block
 * @param [in] color color
 * \else
 * @brief 清除行
 * @param [in] x0  起始ｘ坐标
 * @param [in] y0  起始y坐标
 * @param [in] w 清除宽度
 * @param [in] h 清除高度
 * @param [in] color 颜色选择
 * \endif
 */
void lcd_clearblock(int x0, int y0,int w,int h,uint8_t color); 

/**
 * \if  english 
 * @brief Set font and background picture 
 * @param [in] file_ttf  font
 * @param [in] bg_file  background picture
 * \else
 * @brief 设置字体和背景图片
 * @param [in] file_ttf  字体
 * @param [in] bg_file  背景图片
 * \endif
 */
int fb_init(char * file_ttf,char *bg_file);

/**
 * \if  english 
 * @brief Draw picture on screen
 * @param [in] x  start x-coordinate 
 * @param [in] y  start y-coordinate 
 * @param [in] file_name picture
 * \else
 * @brief 在屏幕指定坐标画图
 * @param [in] x  起始ｘ坐标
 * @param [in] y  起始y坐标
 * @param [in] file_name 图片
 * \endif
 */
int picture_write(int x, int y, char *file_name);

/**
 * \if  english 
 * @brief Display signal icon 
 * @param [in] net_type  signale mode 2-4G:2-4
 * @param [in] signal  signale strength 
 * \else
 * @brief 显示信号图标
 * @param [in] net_type  信号模式2-4G:2-4
 * @param [in] signal  信号质量
 * \endif
 */
int fb_show_signal(int net_type,int signal);

/**
 * \if  english 
 * @brief display WIFI icon 
 * @param [in] level  signale strength: 0-4 
 * \else
 * @brief 显示WIFI图标
 * @param [in] level  信号强度0-4 4为最大
 * \endif
 */
int fb_show_wifi(int level);

/**
 * \if  english 
 * @brief display GPS icon 
 * @param [in] satilite_num  number of satellite, 0-29, depend on the result of GPS return
 * \else
 * @brief 显示GPS图标
 * @param [in] satilite_num  连接卫星数量，以GPS模块返回为准，0-29
 * \endif
 */
int fb_show_GPS(int satilite_num);

/**
 * \if  english 
 * @brief display Bluetooth status icon 
 * @param [in] value status
 *          \li 0 clear
 *          \li 1 closed
 *          \li 2 opened
 * \else
 * @brief 显示蓝牙状态图标
 * @param [in] value 蓝牙状态
 *          \li 0 清除
 *          \li 1 关闭
 *          \li 2 已打开
 * \endif
 */
int fb_show_bluetooth(int value);


/**
 * \if  english 
 * @brief display text (GBK)
 * @param [in] char_size  letter size
 * @param [in] xoffset  start x-coordinate
 * @param [in] yoffset  start y-coordinate(P18-L2C: y-coordinate between 80 to 475)
 * @param [in] text_gb2312 text
 * @param [in] fc display mode
 * \else
 * @brief 指定坐标显示字
 * @param [in] char_size  字大小
 * @param [in] xoffset  起始ｘ坐标
 * @param [in] yoffset  起始y坐标(L2C可显示坐标为80-475)
 * @param [in] text_gb2312 字符串
 * @param [in] fc 显示模式
 * \endif
 */
int fb_gbk_display(int char_size,
                   int xoffset,int yoffset,
                   char *text_gb2312,
                   uint32_t fc);
/**
 * \if  english 
 * @brief display text (UTF8)
 * @param [in] char_size  letter size
 * @param [in] x1  start x-coordinate
 * @param [in] x2  end x-coordinate 
 * @param [in] yoffset  start y-coordinate(P18-L2C: y-coordinate between 80 to 475)
 * @param [in] text_utf8 text
 * @param [in] fc color of foreground
 * @param [in] mode display mode @sa #LCDMODE
 * \else
 * @brief 指定坐标显示字
 * @param [in] char_size  字大小
 * @param [in] x1  起始x坐标
 * @param [in] x2  结束x坐标
 * @param [in] yoffset  起始y坐标(L2C可显示坐标为80-475)
 * @param [in] text_utf8 字符串
 * @param [in] fc 显示前景色
 * @param [in] mode 显示模式 @sa #LCDMODE
 * \endif
 */
int fb_utf8_display_align(int char_size,
                     int x1,int x2,int yoffset,
                     char *text_utf8,
                     uint32_t fc,
                     uint32_t mode
                  );

/**
 * \if  english
 * @brief clear foreground 
 * @param [in] x      start x-coordinate
 * @param [in] y      start y-coordinate
 * @param [in] width  width of foreground
 * @param [in] height heigth of foreground 
 * \else
 * @brief 清除前景色块
 * @param [in] x      起始x坐标
 * @param [in] y      起始y坐标
 * @param [in] width  待清除色块宽度
 * @param [in] height 待清除色块高度
 * \endif
 */
int lcd_erase_foreground(int x,int y,int width,int height);
/**
 * \if  english
 * @brief start to operate screen 
 * \else
 * @brief 开启屏幕操作锁
 * \endif
 */
void fb_disp_start(void);

/**
 * \if  english
 * @brief close screen 
 * \else
 * @brief 关闭屏幕操作锁
 * \endif
 */
void fb_disp_end(void);

/**
 * \if  english
 * @brief sset TTF font 
 * @param [in] file_ttf path of font 
 * \else
 * @brief 设置fb接口使用的字库，立即有效
 * @param [in] file_ttf 字库绝对路径
 * \endif
 */
int fb_set_ttf(char *file_ttf);
/** @}
* end of Display
*/


/**
 * \if  english
* \defgroup UART UART
* \brief USART Communication
 * \else
* \defgroup UART UART 
* \brief 串口通信
 * \endif
* @{
*/

/**
 * \if  english
 * @brief Init communication port
 * @param [in] dev_name name of device eg："/dev/ttymxc1"
 * @param [in] attr params of serial port eg "115200,n,8,1"
 *               \li "115200" default 'n,8,1'
 * @return >0 port handle
 * @return <0 failed
 * \else
 * @brief 串口初始化
 * @param [in] dev_name 串口名称，如/dev/ttymxc1
 * @param [in] attr 串口参数 "115200,n,8,1"
 *               \li "115200"使用缺省'n,8,1'
 * @return >0 串口句柄
 * @return <0 打开失败
 * \endif
 */
int OSUART_Init(char *dev_name,char *attr);
/** 

 * \if english
 * @brief Write data into port
 * @param [in] handle HANDLE of port
 * @param [in] pucSend sent data
 * @param [in] uiSendLen length of sent data
 * \else
 * @brief 串口发送
 * @param [in] handle 串口句柄
 * @param [in] pucSend 待发送数据
 * @param [in] uiSendLen 数据长度
 * \endif
 */
int OSUART_TxStr(int handle,uint8_t *pucSend,uint32_t uiSendLen);


/**
 * \if  english 
 * @brief Read data from port
 * @param [in] handle HANDLE of port
 * @param [in] pucRev data arry which will receive data
 * @param [in] uiLen  max length of received data
 * @param [in] uiTimeout timeout, unit:ms
 * @note Interval time of receive char: 10ms,
		 only one packet of data will be received at each time. 
		 For big data which more than one packet, it needs to receive multiple times to ensure to read whole data.
 * \else
 * @brief 串口接收
 * @param [in] handle 串口句柄
 * @param [in] pucRev 待接收数据数组
 * @param [in] uiLen  接收最大长度，一般为数组大小
 * @param [in] uiTimeout 最大接收超时时间，单位毫秒
 * @note 接收字符间隔超时为10ms，
 *       每次接收一包数据，对于多包发送的协议，需要多次接收，以确定完整性
 * \endif
 */
int  OSUART_RxStr(int handle,unsigned char *pucRev,uint32_t uiLen,uint32_t uiTimeout);
/**
 * \if  english
 * @brief Read char from port
 * @param [in] handle HANDLE of port
 * @param [in] pucRev data arry which will receive data
 * @param [in] uiTimeout timeout, unit:ms
 * \else
 * @brief 串口单字节接收
 * @param [in] handle 串口句柄
 * @param [in] pucRev 待接收数据数组
 * @param [in] uiTimeout 最大接收超时时间，单位毫秒
 * \endif
 */
int  OSUART_RxChar(int handle,uint8_t *pucRev,uint32_t uiTimeout);

/**
 * \if  english
 * @brief Close port
 * @param [in] handle HANDLE of port
 * @note Not need to close COM port, if it would not be used by other process
 * \else
 * @brief 串口关闭
 * @param [in] handle 串口句柄
 * @note 对于一般通信串口，可不做关闭操作
 * \endif
 */
void OSUART_Close(int handle);

/**
 * \if  english
 * @brief Clear COM cache
 * @param [in] handle HANDLE of port
 * \else
 * @brief 清空串口缓存
 * @param [in] handle 串口句柄
 * \endif
 */
void OSUART_Flush(int handle);

/**
 * \if  english
 * @brief libpos debug switch
 * @param [in] on_off
 * \else
 * @brief libpos调试信息配置
 * @param [in] on_off
 * \endif
 */
void Uart_SetDebug(uint8_t on_off);

/**
 * \if  english
 * @brief printf debug info
 * @param pcFmt format
 * \else
 * @brief 打印调试信息
 * @param pcFmt 输入格式
 * \endif
 */
void Uart_Printf(char *pcFmt,...);

/**
 * \if english
 * @brief dump array with hex info
 * @param [in] str  array name
 * @param [in] pBuf array
 * @param [in] len  array length
 * \else
 * @brief dump 16进制数组
 * @param [in] str  数组名称
 * @param [in] pBuf 数组缓存
 * @param [in] len  数组长度
 * \endif
 */
void DumpData(const char * str, uint8_t * pBuf, uint32_t len);

/**
 * \if  english
 * @brief  dump array with hex info in one line
 * @param [in] str  array name
 * @param [in] pBuf array
 * @param [in] len  array length
 * \else
 * @brief 单行dump16进制数组
 * @param [in] str  数组名称
 * @param [in] pBuf 数组缓存
 * @param [in] len  数组长度
 * \endif
 */
void sDumpData(const char * str, uint8_t * pBuf, uint32_t len);
/** @}
* end of UART
*/


/**
 * \if  english
 * \defgroup 4G 4G
 * \else
 * \defgroup 4G GPRS模块接口
 * \endif
* \brief LTE Module API
 * @{
*/


/**
 * \if  english
 * @brief Get signal strength of 4G
 * @retval 0-31 available value
 * @retval 99 unkown
 * @retval < 0 failed
 * \else
 * @brief 获取4G模块信号强度
 * @retval 0-31 有效值
 * @retval 99 未知状态（可能未插天线）
 * @retval < 0 查询失败
 * \endif
 */
int ppp_get_signal(void);

/**
 * \if  english
 * @brief Get name of Operator and network standard
 * \else
 * @brief 获取运营商名称和当前网络制式
 * \endif
 */
int ppp_network_type(char *operator_name,int *act);

/**
 * \if  english
* @brief atcommand to check the status of 4G
* @param [in] at_command at command
* @param [in] ack normally, return string 
* @param [in] nck abnormally, return string 
* @param [out] recvbuff response data buffer 
 * \else
* @brief at指令查询4G状态
* @param [in] at_command at指令。
* @param [in] ack 正常返回字符串。
* @param [in] nck 非正常返回字符串。
* @param [out] recvbuff 指令返回字符串.
* @return 0 ack,-1 nck.
 * \endif
*/
int ppp_at_command(char *at_command,char *ack, char *nck, char *recvbuff);
/** @}
* end of 4G
*/

/**
* \defgroup GPS GPS
* \brief GPS API
* @{
*/
/**
 * @brief Open GPS
 * @return HANDLE of GPS
 */
int Sys_Gps_Open(void);

/**
 * \if  english
 * @brief init GPS module
 * @param [in] mxc uart name  i.e: "/dev/ttymxc3"
 * @param [in] para uart configuration i.e:"9600,8,n,1"
 * \else
 * @brief GPS模块硬件初始化
 * @param [in] mxc 串口名称  例:"/dev/ttymxc3"
 * @param [in] para 串口配置参数 例:"9600,8,n,1"
 * \endif
 */
int Sys_Gps_Init(char *mxc,char *para);



/**
 * \if  english
 * @brief Close GPS
 * \else
 * @brief GPS模块关闭
 * \endif
 */
void Sys_Gps_Close(void);


/**
 * \if  english
 * @brief GPS debug switch
 * @param [in] mode =0 close; =1 open;
 * \else
 * @brief GPS调试信息开关
 * @param [in] mode =0关闭; =1打开;
 * \endif
 */
void Sys_Gps_Debug(unsigned char mode);

/** 

 * \if english
 * @brief Get information from GPS
 * @param [in] timezone timezone
 * @return information
 
  typedef struct __gprmc__  
  {  
    uint32_t time; //time of gps fixed location   
    char pos_state; //status of gps 
    float latitude; //latitude  
    float longitude;//longitude
    float speed; // speed  
    float direction;// speed    
    uint32_t date;  //date   
    float declination; // magnetic declination  
    char dd;  
    char mode;  //mode of GPS
    char hemisphere[2+1];  //hemisphere info
    int sate_num;  // number of satellite
    char quality;  //signal stength of gps
   
    char date_time[14+1]; //local time YYYYMMDDHHMMSS
    double latitude_wgs84;//wgs84 latitude
    double longitude_wgs84;//wgs84 longitude
    char strgps[1024];	//raw data from gps
   }GNRMC;
 * \else
 * @brief GPS获取解析后信息
 * @param [in] timezone 当前所在时区
 * @return 解析后信息结构体指针
 
  typedef struct __gprmc__  
  {  
    uint32_t time; //gps定位时间
    char pos_state; //gps状态位
    float latitude; //获取纬度
    float longitude;//获取经度
    float speed; // 速度
    float direction;//航向
    uint32_t date;  //日期
    float declination; // 磁偏角
    char dd;  
    char mode;  //GPS模式位
    char hemisphere[2+1];  //半球信息
    int sate_num;  // 卫星数
    char quality;  //gps信号质量
   
    char date_time[14+1]; //转换的当地时间 YYYYMMDDHHMMSS
    double latitude_wgs84;//转换后wgs84纬度坐标
    double longitude_wgs84;//转换后wgs84经度坐标
    char strgps[1024];	//获取到的未解析数据
   }GNRMC;  
 * \endif
 */
GNRMC* Sys_Gps_QueryLaLO(char *timezone);


/** 

 * \if english
 * @brief Get Raw information from GPS
 * @param [in] timezone timezone
 * @return information
 * @note location info may be invalid
 * \else
 * @brief GPS获取原始解析后信息
 * @param [in] timezone 当前所在时区
 * @return 解析后信息结构体指针
 * @note 坐标值可能无效
 * \endif
 */
GNRMC* Sys_Gps_Data(char *timezone);
/** @}
* end of GPS
*/



/**
 * \if  english
 * \defgroup QRCode QRCode
 * \else
 * \defgroup QRCode QRCode驱动接口
 * \endif
* \brief QRCode API
* @{
*/

/**
 * \if  english
 * @brief Open QR scanner
 * @param [in] index index of scanner, set value as 0, if is single scanner
 * @return handle HANDLE of scanner
 * \else
 * @brief 二维码模块打开
 * @param [in] index 扫码模块索引
 * @return 模块句柄
 * \endif
 */
int QRCode_Open(int index);

/**
 * \if  english
 * @brief Read data from scanner
 * @param [in] handle HANDLE of scanner
 * @param [in] pucRev data buffer which will store the received data
 * @param [in] uiLen length of received data
 * @param [in] uiTimeout timeout, unit:ms
 * @return length received data
 * \else
 * @brief 二维码模块接收
 * @param [in] handle 串口句柄
 * @param [in] pucRev 待接收数据
 * @param [in] uiLen  待接收数据最大长度
 * @param [in] uiTimeout 超时时间，毫秒
 * \endif
 */
int  QRCode_RxStr(int handle,unsigned char *pucRev,uint32_t uiLen,uint32_t uiTimeout);

/**
 * \if  english
 * @brief Close scanner
 * @param [in] handle HANDLE of scanner
 * @return 0 Success
 * \else
 * @brief 关闭二维码模块
 * @param [in] handle 模块句柄
 * \endif
 */
int QRCode_Close(int handle);

/** @}
* end of QRCode
*/

/**
 * \if  english
 * \defgroup system System
 * \else
 * \defgroup System 系统接口
 * \endif
* \brief System API
 *@{
*/

/**
 * \if  english
 * @brief Get system tick, unit:ms
 * \else
 * @brief 获取系统运行滴答，单位毫秒
 * \endif
 */
unsigned long OSTIMER_GetTickCount(void);

/**
 * @brief delay us
 * @param [in] num microseconds
 */
void OSTIMER_DelayUs(int num);

/**
 * \if  english
 * @brief delay ms
 * \else
 * @brief 毫秒延时
 * \endif
 * @param [in] ms milliseconds
 */
void OSTIMER_DelayMs(int ms);

/**
 * \if  english
 * @brief set datetime of system, format: YYYYMMDDHHMMSS\0
 * @param [in] DateTime
 * @note Set system datetime and system RTC
 * \else
 * @brief 设置系统时间，格式YYYYMMDDHHMMSS\0
 * @param [in] DateTime
 * @note 设置系统时间和rtc时钟
 * \endif
 */
int Sys_SetRealTime(char *DateTime);

/**
 * \if  english
 * @brief Get system datetime, format: YYYYMMDDHHMMSS\0
 * @param [out] DateTime
 * @note get system datetime
 * \else
 * @brief 获取系统时间，格式YYYYMMDDHHMMSS\0
 * @param [out] DateTime
 * @note 获取系统时钟
 * \endif
 */
int Sys_GetRealTime(char *DateTime);

/**
 * \if  english
 * @brief Get serial number of device
 * @param [out] sn serial number 
 * @note get serial number of device
 * \else 
 * @brief 读取终端序列号
 * @param [in] sn serial number
 * \endif
 */
int Sys_GetSN(char *sn);

/**
 * @brief Write serial number into device, not suggest to use this function
 * @param [out] sn serial number
 * @note write serial number into device
 */
int Sys_SetSN(char *sn);


/**
 * \if  english
 *@brief Query Terminal Name
 *@return Terminal Type @sa TERM_TYPE
 * \else
 *@brief 查询终端类型
 *@return 终端类型 @sa TERM_TYPE
 * \endif
 */
int Sys_TermType(void);

/**
 * \if  english
 *@brief Query Terminal Name
 *@param [out] term_name Terminal Name
 * \else
 *@brief 查询终端名称
 *@param [out] term_name 终端名称
 * \endif
 */
int Sys_TermName(char *term_name);

/**
 * \if  english
 * @brief Get Terminal display resolution
 * @param [out] x width
 * @param [out] y height
 * \else
 * @brief 读取终端分辨率
 * @param [out] x 宽度
 * @param [out] y 高度
 * \endif
 */
int Sys_GetResolution(int *x,int *y);
/**
 * \if  english
 * @brief Get the version of library
 * @return version information
 * \else
 * @brief 获取函数库版本号
 * @return 版本信息
 * \endif
 */
char *Get_HardwareVer(void);



/**
 * \if  english
 * @brief Get the version of libpos api
 * @param [out] version version information
 * @return API version less than 63 bytes
 * \else
 * @brief 获取libpos api 版本
 * @param [out] version 版本，可传NULL
 * @return API版本 < 63字节
 * \endif
 */
char *Sys_ApiVer(char *version);

/**
 * \if  english
 * @brief Get the version of rootfsi
 * @param [out] ver version information
 * @return 0: true  other: false
 * \else
 * @brief 获取rootfs 版本
 * @param [out] ver 版本
 * @return 0: true  other: false
 * \endif
 */
char  Sys_RootfsVer(char *ver);

/** @}
* end of System
*/

/**
* \defgroup KEY KEY
* \brief KEY
*@{
*/

/**
 * \if  english
 *@brief Get key value 
 *@param [in] TimeOut timeout, unit: ms. Set this value as 0, if need wait key all the time
 *@return UTIL_TIMEOUT or key value
 * \else
 *@brief 查询按键
 *@param [in] TimeOut 超时时间，毫秒
 * \endif
 */
int Sys_WaitKey(int TimeOut);
/**
 * \if  english
 *@brief Clear key cache 
 * \else
 *@brief 清空按键缓存
 * \endif
 */
int Sys_FlushKey(void);

/** @}
* end of KEY
*/

/**
* \defgroup BEEP BEEP
* \brief BEEP
* @{
*/
/**


 * \if english
 * @brief Init buzzer 
 * \else
 * @brief 蜂鸣初始化
 * \endif
 */
void Beep_Init(void);
/**
 * \if  english
 * @brief Beep, non-block
 * \else
 * @brief 蜂鸣，非阻塞
 * \endif
 */
void Sys_Beep(void);

/**
 * \if  english
 * @brief Beep, block
 * @param [in] ms time, unit: ms
 * \else
 * @brief 蜂鸣，阻塞
 * @param [in] ms 毫秒
 * \endif
 */
void Sys_BeepMs(uint32_t ms);
/**
 * \if  english
 * @brief Beep repeatedly, non-block
 * @param cycles number of times 
 * @param beep_time time of each beep 
 * @param silent_time interval time
 * \else
 * @brief 多次蜂鸣，非阻塞
 * @param cycles 共蜂鸣次数
 * @param beep_time 单次蜂鸣时间
 * @param silent_time 单次蜂鸣后间隔时间
 * \endif
 */
void Sys_BeepBeep(uint32_t cycles,uint32_t beep_time,uint32_t silent_time);

/**
 * \if  english
 * @brief Warning beep, block
 * @param [in] num number of times of beep
 * \else
 * @brief 警告蜂鸣，阻塞
 * @param [in] num 蜂鸣次数
 * \endif
 */
void Sys_ErrorBeep(int num);

/** @}
* end of BEEP
*/

/**
 * \if  english
 * \defgroup CAN CAN
 * \else
 * \defgroup CAN CAN通信接口
 * \endif
* \brief CAN
 * @{
*/

/**
 * \if  english
 * @brief Init CAN
 * @param [in] devname number of CAN device, eg: can0
 * @param [in] bitrate bit rate 100000，100Kb/s
 * @return HANDLE of CAN
 * \else
 * @brief CAN总线初始化
 * @param [in] devname CAN设备名称，如can0
 * @param [in] bitrate 速率 100000，100Kb/s
 * @return can句柄
 * \endif
 */
int Can_Init(char *devname,unsigned int bitrate);
/**
 * \if  english
 * @brief Write data into CAN
 * @param [in] canfd HANDLE of CAN
 * @param [in] data_id data id
 * @param [in] sndlen length of sent data 
 * @param [in] sndbuff sent data buffer
 * \else
 * @brief CAN总线写入
 * @param [in] canfd 句柄
 * @param [in] data_id 数据识别符
 * @param [in] sndlen 发送数据长度
 * @param [in] sndbuff 发送数据缓存
 * \endif
 */
int Can_Write(int canfd ,int data_id, unsigned int sndlen, unsigned char *sndbuff);

/**
 * \if  english
 * @brief Read data from CAN
 * @param [in] canfd HANDLE of CAN
 * @param[in,out] pdata_id set received data id,
 *                        -1//receive whole data, return the id of real data which is received, 
 							data id is 11 bit and ranges from 0 to 0x7FF 
 * @param[in,out] len length of data which will be received
 * @param [in] rcvbuff buffer which will store the received data 
 * @param [in] timeout_ms timeout 
 * @return length of real received data
 *         <=0 failed
 * \else
 * @brief CAN总线读取
 * @param [in] canfd 句柄
 * @param[in,out] pdata_id 设置接收的数据识别符（忽略其他数据）,
 *                        -1表示接收所有数据，返回值是实际接受的数据id
 *                        注意数据识别符范围（0-0x7FF）11bit
 * @param[in,out] len 期望接收的数据长度
 * @param [in] rcvbuff 接收数据缓存
 * @param [in] timeout_ms 等待接收的超时时间
 * @return 实际接收的数据长度
 *         <=0 表示失败
 * \endif
 */
int Can_Read(int canfd ,int *pdata_id, unsigned int len, unsigned char *rcvbuff, unsigned int timeout_ms);

/**
 * \if  english
 * @brief Close CAN 
 * @param [in] canfd HANDLE of CAN 
 * \else
 * @brief 关闭句柄
 * @param [in] canfd 句柄
 * \endif
 */
int Can_Close(int canfd);
/** @}
* end of CAN
*/


/**
 * \if  english
* \defgroup eeprom eeprom
 * \else
* \defgroup eeprom eeprom 读写接口
 * \endif
* \brief eeprom read/write 
*@{
*/

/**
 * \if  english 
 * @brief Write EEPROM
 * @note Write data into EEPROM 
 * @param [in] icdev identifier of device
 * @param [in] offset address offset 
 * @param [in] length length of data length 
 * @param [in] send_buffer data buffer 
 * @return <0 failed
 * @return 0 success
 * \else
 * @brief 写EEPROM。
 * @note 写入数据到设备内部EEPROM中，可以用作数据保存等。
 * @param [in] icdev 设备标识符。
 * @param [in] offset 偏移地址。
 * @param [in] length 写入长度。
 * @param [in] send_buffer 传入数据。
 * @return <0 表示失败
 * @return 0表示成功
 * \endif
*/
short dc_swr_eeprom  ( int  icdev,
					short  offset,
					short  length,
					unsigned char *  send_buffer
 );
 
/**
 * \if  english 
 * @brief Read EEPROM
 * @note Read data from EEPROM 
 * @param [in] icdev identifier of device 
 * @param [in] offset address offset
 * @param [in] length length of data which will be read 
 * @param [out] rec_buffer data buffer 
 * @return <0 failed
 * @return 0 success
 * \else
 * @brief 读EEPROM。
 * @note 读取设备内部EEPROM中的数据，可以用作数据保存等。
 * @param [in] icdev 设备标识符。
 * @param [in] offset 偏移地址。
 * @param [in] length 读取长度。
 * @param [out] rec_buffer 返回的数据。
 * @return <0 表示失败
 * @return 0 表示成功
 * \endif
 */
short dc_srd_eeprom  ( int  icdev,
						short  offset,
						short  length,
						unsigned char *  rec_buffer
 );
/** @}
* end of eeprom
*/

/**
 * \if  english
 * \defgroup LED LED
 * \else
* \defgroup LED LED灯和数码管接口
 * \endif
* \brief LED and segment interface
* @{
*/

/**
 * \if  english
 * @brief led flicker control 
 * @param [in] led   led id
 * @param [in] uiOnTime single time, time of light on, unit: ms 
 * @param [in] uiOffTime single time, time of light off, unit: ms
 * @param [in] uiCycle cycle
 * \else
 * @brief led灯闪烁控制
 * @param [in] led     led灯索引
 * @param [in] uiOnTime 单次循环，灯亮时间，单位毫秒
 * @param [in] uiOffTime 单次循环，灯灭时间，单位毫秒
 * @param [in] uiCycle 亮灭周期
 * \endif
 */
uint8_t LED_Flicker(LED_TYPE led, uint32_t uiOnTime, uint32_t uiOffTime, uint32_t uiCycle);

/**
 * \if  english
 * @brief led control 
 * @param [in] led     led id
 * @param [in] ucOnOff turn on/turn off 
 * \else
 * @brief led灯控制
 * @param [in] led     led灯索引
 * @param [in] ucOnOff 打开/关闭
 * \endif
 */
uint8_t LED_Control(LED_TYPE led,uint8_t ucOnOff);

/**
 * \if  english
 * @brief Clear LED segment 
 * \else
 * @brief 清空数码管显示
 * \endif
 */
void LedSeg_Clear(void);
/**
 * \if  english
 * @brief Show balance 
 * @param [in] uiBalance balance 
 * @note the max balance depend on LED segment 
 * @note Adapt the current device automatically
 * \else
 * @brief 数码管显示余额
 * @param [in] uiBalance 待显示余额
 * @note 最大显示金额由当前数码管数量决定
 * @note 自动适配当前终端类型
 * \endif
 */
void LedSeg_Balance(uint32_t uiBalance);
/**
 * \if  english
 * @brief LED segment display 
 * @param [in] index id[0-15]
 * @param [in] str    String to be displayed, support '0'-'9', 'A'-'Z', '.' 
 *        \li  " 1.23"
 *        \li  "  234.11"
 *        \li  "A2.3B" 
 * \else
 * @brief 数码管显示接口
 * @param [in] index 索引号[0-15]
 * @param [in] str 待显示字符串,支持’0‘-’9‘ ’A'-'Z',‘.'指示当前数码管是否显示小数点
 *        \li  " 1.23"
 *        \li  "  234.11"
 *        \li  "A2.3B"
 * \endif
 */
void  LedSeg_Display(int index,char *str);

/**
 * \if  english
 * @brief Init LED segment 
 * \else
 * @brief 数码管初始化
 * \endif
 */
int LedSeg_Init(void);
/** @}
* end of LED
*/

/**
 * \if  english
* \defgroup ICC IC card
* \brief Contact ICC and Proximity ICC
 * \else
* \defgroup ICC ICC
* \brief IC卡接口
 * \endif
* @{
*/
/**
 * \if  english
 * @brief  Init module 
 * @param [in] slot slot socket
 * \else
 * @brief ic卡或sam模块初始化
 * @param [in] slot 卡槽号
 * \endif
 */
int ICC_Init(uint8_t slot);
/**
 * \if  english
 * @brief Get ATR from the IC card or SAM card 
 * @param [in] slot slot socket
 * @param [in] atr ATR
 * @param [in] len length of ATR 
 * \else
 * @brief ic卡或sam模块上电
 * @param [in] slot 卡槽号
 * @param [in] atr 复位值
 * @param [in] len atr长度
 * \endif
 */

int ICC_GetATR(uint8_t slot,uint8_t* atr,uint32_t* len);

/**
 * \if  english
 * @brief IC card or SAM module power on 
 * @param [in] slot slot socket
 * @param [in] baud baud-rate, default: 9600
 *            \li BAUD_9600  9600
 *            \li BAUD_19200 19200
 *            \li BAUD_38400 38400
 *            \li BAUD_57600  57600
 *            \li BAUD_115200 115200
 * @param [in] volt voltage, default: 5.0V
 *              BIT1-BIT2
 *                  \li 0x00：5V
 *                  \li 0x01：3V
 *                  \li 0x02：1.8V
 *              \li BIT8  1: support PPS,0:default, don't support
 * @param [out] atr ATR
 * @param [out] len length of ATR 
 * \else
 * @brief ic卡或sam模块上电
 * @param [in] slot 卡槽号
 * @param [in] baud 波特率，默认9600
 *            \li BAUD_9600  9600
 *            \li BAUD_19200 19200
 *            \li BAUD_38400 38400
 *            \li BAUD_57600  57600
 *            \li BAUD_115200 115200
 * @param [in] volt 操作电压，默认5.0V
 *              BIT1-BIT2
 *                  \li 0x00：5V
 *                  \li 0x01：3V
 *                  \li 0x02：1.8V
 *              \li BIT8  1:支持PPS,0:不支持（缺省)
 * @param [out] atr 卡片复位应答信息
 * @param [out] len atr长度
 * \endif
 */
int ICC_PowerUp(uint8_t slot,ICC_BAUD baud,ICC_VOLT volt,uint8_t* atr,uint32_t* len);
/**
 * \if  english
 * @brief Check card 
 * @param [in] slot slot socket
 * @return 0 detectd card 
 * @return 1 no card
 * \else
 * @brief 卡在位检测
 * @param [in] slot 卡槽号
 * @return 0 卡在位检测
 * @return 1 卡片不在位
 * \endif
 */
int ICC_Detect(uint8_t slot);
/**
 * \if  english
 * @brief Card APDU 
 * @param [in] slot slot socket
 * @param [in] apdu  apdu data buffer
 * @param [in] apdu_len   length of apdu
 * @param [out] rpdu response data 
 * @param [out] rpdu_len length of response data
 * \else
 * @brief 卡片APDU交互
 * @param [in] slot 卡槽号
 * @param [in] apdu  apdu数据
 * @param [in] apdu_len   apdu长度
 * @param [out] rpdu 返回数据
 * @param [out] rpdu_len 返回长度
 * \endif
 */
int	ICC_Exchange(uint8_t slot,const uint8_t* apdu,const uint32_t apdu_len, uint8_t* rpdu, uint32_t* rpdu_len);

/**
 * \if  english
 * @brief Card power off 
 * @param [in] slot slot socket
 * @note just support the card slot socket which can be powered off 
 * \else
 * @brief 卡片下电
 * @param [in] slot 卡槽号
 * @note 仅对支持下电的卡槽有效
 * \endif
 */
int ICC_Remove(uint8_t slot);
/**
 * \if  english
 * @brief Close card slot socket 
 * @param [in] slot slot socket
 * \else
 * @brief SAM卡关闭
 * @param [in] slot 卡槽号
 * \endif
 */
int ICC_Close(uint8_t slot);

/**
 * \if  english
 * @brief Detect ISO14443，Type A&B
 * @param [in] slot slot socket
 * @param [out] uid  Card UID 
 * \else
 * @brief ISO14443寻卡，A&B
 * @param [in] slot 卡槽号
 * @param [out] uid 卡片UID
 * \endif
 */
int PICC_PollCard(uint8_t slot,uint8_t uid[10]);
/**
 * \if  english
 * @brief Close RF module
 * @param [in] slot slot socket
 * \else
 * @brief 非接关场
 * @param [in] slot 卡槽号
 * \endif
 */
int PICC_Close(uint8_t slot);

/**
 * \if  english
 * @brief Init RF module 
 * @param [in] slot slot id, such as 0, 1, 2 
 * \else
 * @brief 非接模块初始化
 * @param [in] slot 对于多个非接模块，使用0,1,2区分
 * \endif
 */
int PICC_Open(uint8_t slot);
/**
 * \if  english
 * @brief 场复位
 * @param [in] slot 卡槽号
 * \else
 * @brief 场复位
 * @param [in] slot 卡槽号
 * \endif
 */
int PICC_Reset(uint8_t slot);
/**
 * \if  english
 * @brief Contactless card power on 
 * @param [in] slot slot socket
 * @param [in] mode mode of detect card
 *                \li 'A' just detect TypeA 
 *                \li 'B' just detect TypeB 
 *                \li 'C' just detect FeliCa 
 *                \li 0   detect TypeA and TypeB 
 * @param [out] cardType Card Type \sa #PICC_CARD_TYPE
 * @param [out] uid card UID
 * @param [out] uid_size length off UID
 * @note Manufacture ID + PMm, if card is FeliCa , uid_size is 16 
 * \else
 * @brief 非接上电
 * @param [in] slot 卡槽号
 * @param [in] mode 寻卡模式
 *                \li 'A' 只寻A卡
 *                \li 'B' 只寻B卡
 *                \li 'C' 只寻Felica卡
 *                \li 0   寻AB卡
 * @param [out] cardType 卡类型 \sa #PICC_CARD_TYPE
 * @param [out] uid 卡片UID
 * @param [out] uid_size 卡片UID长度
 * @note 对于Felica,uid 返回 Manufacture ID + PMm, uid_size 为16
 * \endif
 */
int PICC_PowerOn(uint8_t slot,uint8_t mode,uint8_t *cardType,uint8_t *uid,uint8_t *uid_size);
/**
 * @brief Get ATR, the card must be powered on 
 * @param [out] atr
 * @param [out] atr_len length of atr
 * @retval       TRUE/FALSE
 */
uint8_t PICC_GetPiccATR(uint8_t *atr,uint8_t *atr_len);
/**
 * \if  english
 * @brief get card ats
 * @param [in] slot card slot
 * @param [out] ats card ats
 * @param [out] atsLen ats length
 * @note power-up card first \sa#PICC_PowerOn
 * @return \sa#ICCERR_INVALID_TYPE
 * @return \sa#ICCERR_NO_CARD
 * \else
 * @brief 获取A/B 卡片ats/atqb
 * @param [in] slot 卡槽号
 * @param [out] ats 卡片ats
 * @param [out] atsLen ats长度
 * @note 需要先上电 \sa#PICC_PowerOn
 * @return \sa#ICCERR_INVALID_TYPE
 * @return \sa#ICCERR_NO_CARD
 * \endif
 */
int PICC_GetAts(uint8_t slot,uint8_t *ats,uint8_t *atsLen);
/**
 * \if  english 
 * @brief PICC apdu
 * @param [in] slot slot socket 
 * @param [in] apdu_buff  apdu data buffer
 * @param [in] apdu_len   length of apdu
 * @param [out] resp_buffer response data 
 * @param [out] resp_len length of response data 
 * \else
 * @brief PICC apdu通信
 * @param [in] slot 卡槽号
 * @param [in] apdu_buff  apdu数据
 * @param [in] apdu_len   apdu长度
 * @param [out] resp_buffer 返回数据
 * @param [out] resp_len 返回长度
 * \endif
 */
int PICC_Exchange(uint8_t slot,uint8_t * apdu_buff, uint32_t apdu_len, uint8_t *resp_buffer, uint32_t * resp_len);

/**
 * @brief init type c reader mode
 * @param [in] bitrate
 *             \li 0 212kb/s
 *             \li 1 424kb/s
 * @note to use with #PICC_TypeCExchange
 */
int PICC_TypeCInit(uint8_t bitrate);
/**
 * \if  english 
 * @brief Type C low-level communication
 * @param [in] slot slot socke(Reserved)
 * @param [in] apdu_buff  apdu data buffer 
 * @param [in] apdu_len    length of apdu
 * @param [out] resp_buffer  response data 
 * @param [out] resp_len length of response data 
 * \else
 * @brief Type C low-level communication
 * @param [in] slot 卡槽号(Reserved)
 * @param [in] apdu_buff  apdu数据
 * @param [in] apdu_len   apdu长度
 * @param [out] resp_buffer 返回数据
 * @param [out] resp_len 返回长度
 * \endif
 */
int PICC_TypeCExchange(uint8_t slot,uint8_t *apdu_buff, uint16_t apdu_len, uint8_t *resp_buffer, uint32_t *resp_len);
/**
 * \if  english
 * @brief Open/Close RF module 
 * @param [in] slot slot socket
 * @param [in] carrier_on_off open/close 
 * \else
 * @brief 非接场开启/关闭
 * @param [in] slot 卡槽号
 * @param [in] carrier_on_off 开启/关闭
 * \endif
 */
int PICC_Carrier(uint8_t slot,uint8_t carrier_on_off);
/**
 * \if  english
 * @brief Set the field strength 
 * \else
 * @brief 设置非接场强大小，默认最大
 * @param [in] level 0-8 0最大 8最小
 * \endif
 */
void PICC_FieldLevel(uint8_t level);
/**
 * \if  english
 * @brief Close RF module 
 * @param [in] slot slot socket
 * @note Close 
 * \else
 * @brief 非接模块关闭
 * @param [in] slot 卡槽号
 * @note 暂时只做关场操作
 * \endif
 */
int PICC_Close(uint8_t slot);
/**
 * \if  english
 * @brief wait card removed
 * @param [in] slot card slog
 * @note blocked until card is removed
 * @return 0 card is removed
 * \else
 * @brief 非接等待移卡
 * @param [in] slot 卡槽号
 * @note 该函数调用后被阻塞，直到卡被移开后返回
 * @return 0表示卡片被移开
 * \endif
 */
int PICC_Remove(uint8_t slot);
/** @}
* end of ICC
*/



/**
 * \if  english
* \defgroup Mifare Mifare
 * \else
* \defgroup Mifare Mifare卡接口
 * \endif
* \brief Mifare Cards
*@{
*/


typedef enum _ENUM_MIFARE_KEY_TYPE_
{
    MIFARE_KEY_A = 0x60,
    MIFARE_KEY_B = 0x61
}ENUM_MIFARE_KEY_TYPE;

/**
 * \if  english
 * @brief Mifare authentication 
 * @param [in] type key type
 * @param [in] addr address of sector 
 * @param [in] key key data, 6 bytes
 * \else
 * @brief Mifare扇区密钥认证
 * @param [in] type 密钥类型
 * @param [in] addr 扇区地址
 * @param [in] key Mifare扇区密钥，6字节
 * \endif
 */
uint8_t Mifare_AuthenBlock(ENUM_MIFARE_KEY_TYPE type,uint8_t addr,uint8_t *key);
/**
 * \if  english
 * @brief Read Mifare data 
 * @param [in] addr address id[0~63]
 * @param [out] out_data data buffer 
 * \else
 * @brief 读取Mifare卡扇区数据
 * @param [in] addr 扇区地址[0~63]
 * @param [out] out_data 读出的扇区数据
 * \endif
 */
uint8_t Mifare_ReadBlock(uint8_t addr,uint8_t *out_data);
/**
 * \if  english
 * @brief Write Mifare Block
 * @param [in] addr address of block 
 * @param [in] in_data init value
 * \else
 * @brief 写入Mifare卡扇区数据
 * @param [in] addr 扇区地址[0~63]
 * @param [in] in_data 写入的扇区数据
 * \endif
 */
uint8_t Mifare_WriteBlock(uint8_t addr,uint8_t *in_data);
/**
 * \if  english 
 * @brief Init e-wallet 
 * @param [in] addr address of block 
 * @param [in] balance init value
 * \else
 * @brief 电子钱包初始化
 * @param [in] addr 钱包块地址
 * @param [in] balance 初始值
 * \endif
 */
uint8_t Mifare_InitWallet(uint8_t addr,uint32_t balance);
/**
 * \if  english 
 * @brief read e-wallet 
 * @param [in] addr address of e-wallet block 
 * @param [out] balance balance 
 *         \li 0：success
 *         \li 1：failed 
 *         \li 2:other error  
 * \else
 * @brief 读钱包值
 * @param [in] addr 钱包块地址
 * @param [out] balance 钱包值
 *         \li 0：正确
 *         \li 1：失败
 *         \li 2:其他错误
 * \endif
 */                   
uint8_t Mifare_ReadWallet(uint8_t addr,uint32_t *balance);
/**
 * \if  english 
 * @brief Increase value on e-wallet 
 * @param [in] addr address of e-wallet block
 * @param [in] add_value value 
 * @return \li 0：success
 *         \li 1：failed
 *         \li 2: other  error 
 * \else
 * @brief 钱包增值
 * @param [in] addr 钱包块地址
 * @param [in] add_value 钱包增值
 * @return \li 0：正确
 *         \li 1：失败
 *         \li 2:其他错误
 * \endif
 */                   
uint8_t Mifare_AddWallet(uint8_t addr,uint32_t add_value);

/**
 * \if  english 
 * @brief Decrease value on e-wallet 
 * @param [in] addr address of e-wallet block
 * @param [in] dec_value value 
 * @return \li 0 success 
 *         \li 1 failed
 *         \li 2 other error 
 * \else
 * @brief 钱包扣值
 * @param [in] addr 钱包块地址
 * @param [in] dec_value 钱包扣值
 * @return \li 0 正确
 *         \li 1 失败
 *         \li 2 其他错误
 * \endif
 */                   
uint8_t Mifare_DecWallet(uint8_t addr,uint32_t dec_value);

/**
 * \if  english 
 * @brief e-wallet backup 
 * @param [in] addr_from source address of e-wallet 
 * @param [in] addr_to   destination address of e-wallet backup
 * @return \li 0 success
 *         \li 1 failed 
 *         \li 2 other error
 * \else
 * @brief 钱包备份
 * @param [in] addr_from 待备份区地址
 * @param [in] addr_to   钱包备份地址
 * @return \li 0 正确
 *         \li 1 失败
 *         \li 2 其他错误
 * \endif
 */      
uint8_t Mifare_BackupWallet(uint8_t addr_from,uint8_t addr_to);

/**
 * \if  english 
 * @brief Detect Mifare
 * @param [in] slot slot socket 
 * @param [out] snr SNR of card
 *             atqa(2) + sak(1) + uid(4-10)
 * @param [out] snr_len length of SNR 
 * \else
 * @brief Mifare寻卡
 * @param [in] slot 卡槽号
 * @param [out] snr 卡片数据
 *             atqa(2) + sak(1) + uid(4-10)
 * @param [out] snr_len snr长度
 * \endif
 */
int Mifare_PowerOn(uint8_t slot,uint8_t *snr,uint8_t *snr_len);

/** @}
* end of Mifare
*/

/**
 * \if  english
* \defgroup MifarePlus MifarePlus
 * \else
* \defgroup MifarePlus MifarePlus卡接口
 * \endif
* \brief MifarePlus Card
*@{
*/

/**
 * @brief Set personalization data, support card password
 * @param [in] BNr data block which will write personalization data 
 * @param [in] dataperso personalization data, 16 bytes 
 * @return 0 true,1 false.
 */
int MFPLUS0_writeperso(unsigned int BNr,unsigned char * dataperso);

/**
 * @brief personal card, card will be in level 1  
 * @return 0 true,1 false.
 */
int MFPLUS0_commitperso();

/**
 * @brief Card has the same operation with Mifare one, while card is in level 1. This function is used to authenticate the card password 
      (card cannot excute RATS)
 * @param [in] authkey password data, 16 bytes 
 * @return 0 true,1 false.
 */
int MFPLUS1_authl1key(unsigned char *authkey);
/**
 * @brief Card(level 1) switch into level 2 
 * @param [in] authkey card password 
 * @return 0 true,1 false.
 */
int MFPLUS1_switchtol2(unsigned char *authkey);
/**
 * @brief Card(level 1) switch into level 3 
 * @param [in] authkey card password 
 * @return 0 true,1 false.
 */
int MFPLUS1_switchtol3(unsigned char *authkey);

/**
 * @brief Card(level 2) switch into level 3 
 * @param [in] authkey card password
 * @return 0 true,1 false.
 */
int MFPLUS2_switchtol3(unsigned char *authkey);

/**
 * @brief Authenticate password of the card which in level 3, different key block id , authenticate different password    
 * @param [in] keyBNr key block id 
 * @param [in] authkey passowrd, 16 bytes 
 * @return 0 true,1 false.
 */
int MFPLUS3_authl3key(unsigned int keyBNr,unsigned char *authkey);

/**
 * @brief Authenticate sector password of the card which in level 3 
 * @param [in] mode authentication mode: 0 // authenticate keyA, 4 // authenticate keyB
 * @param [in] sectorBNr sector number 
 * @param [in] authkey passowrd, 16 bytes 
 * @return 0 true,1 false.
 */
int MFPLUS3_authl3sectorkey(unsigned char mode,unsigned int sectorBNr,unsigned char *authkey);

/**
 * @brief Read encrypted data from card which is in level 3, 16 bytes of each block, can read several blocks  
 * @param [in] BNr start block address
 * @param [in] num number of blocks, should less than 6 
 * @param [out] readdata data buffer, length is Numblock*16
 * @return 0 true,1 false.
 */
int MFPLUS3_readinplain(unsigned int BNr,unsigned char num, unsigned char *readdata);

/**
 * @brief Write data into card which is in level 3, 16 bytes of each block, can write several blocks 
 * @param [in] BNr start block address 
 * @param [in] numblock number of blocks, should less than 6  
 * @param [in] writedata data buffer 
 * @return 0 true,1 false.
 */
int MFPLUS3_writeinplain(unsigned int BNr,unsigned char numblock,unsigned char *writedata);

/**
 * @brief Read encrypted data in level 3 
 * @param [in] BNr start block address 
 * @param [in] num number of blocks
 * @param [out] readdata data to readout
 * @param [out] flag read flag
	\li 1 return cipher
	\li other
 * @return 0 true,1 false.
 */
int MFPLUS3_readencrypted(unsigned int BNr,unsigned char num,unsigned char *readdata, unsigned char flag);

/**
 * @brief Write data into card which is in level 3, 16 bytes of each block, can write several blocks; this function can be used to write key block and only one key block can be wrote
 * @param [in] BNr start block address
 * @param [in] numblock number of blocks
 * @param [in] writedata data buffer 
 * @param [in] flag 1：data had be encrypted already; 0：data had not be encrypted, need encrypt data at first, then can wirte into card 
 * @return 0 true,1 false.
 */
int MFPLUS3_writeencrypted(unsigned int BNr,unsigned char numblock,unsigned char *writedata, unsigned char flag);
/** @}
* end of MifarePlus
*/

/**
* \defgroup IDCARD ID card 
* \brief Identification Card
* @{
*/
/**
 * \if  english
 * @brief Return ID card information
 * @param [in] cmd part command of read ID card.
 * @param [in] send_len length of received data.
 * @param [in] send_para sent data.
 * @param [out] rev_len length of received data.
 * @param [out] rev_para received data.
 * \else
 * @brief 传送上层身份证相关信息
 * @param [in] cmd 身份证信息部分命令。
 * @param [in] send_len 发送数据长度。
 * @param [in] send_para 发送数据。
 * @param [out] rev_len 返回数据长度.
 * @param [out] rev_para 返回数据.
 * @return 0 true,1 false.
 * \endif
 */
unsigned char IDCARD_ReadMess(unsigned char cmd, ushort send_len, 
                      		  unsigned char *send_para, ushort *rev_len, 
                      		  unsigned char *rev_para);

/**
 * \if  english
 * @brief Read IC card automatically
 * @param [out] len length of received data.
 * @param [out] para received data.
 * @return 0 true,1 false.
 * \else
 * @brief 全自动读取身份证数据
 * @param [out] len 返回数据长度.
 * @param [out] para 返回数据.
 * @return 0 true,1 false.
 * \endif
 */
ushort IDCARD_AutoRead(ushort* len,unsigned char* para);
/**
 * \if  english
 * @brief Read ID card and fingerprint automatically
 * @param [out] len length of received data.
 * @param [out] para received data.
 * @return 0 true,1 false.
 * \else
 * @brief 全自动读取身份证数据+指纹信息
 * @param [out] len 返回数据长度.
 * @param [out] para 返回数据.
 * @return 0 true,1 false.
 * \endif
 */
ushort IDCARD_AutoRead_Fig(ushort* len,unsigned char* para);

/**
 * \if english
 * @brief Standard protocol transparent
 * @param [out] len length of received data.
 * @param [out] para received data.
 * @return 0 true,1 false.
 * \else
 * @brief 标准协议透传调用
 * @param [out] len 返回数据长度.
 * @param [out] para 返回数据.
 * \endif
 * @return 0 true,1 false.
 */
ushort IDTWO_ReadMsg(ushort* len, unsigned char* para);
/**
 * \if  english
 * @brief Active ID card
 * @return 0 true,1 false.
 * \else
 * @brief 激活身份证
 * @return 0 true,1 false.
 * \endif
 */
ushort IDCARD_ActiveIdcard(void);

/**
 * \if  english
 * @brief Get UID of ID card.
 * @param [out] rev_len length of received data.
 * @param [out] rev_para received data.
 * @return 0 true,1 false.
 * \else
 * @brief 获取身份证UID
 * @param [out] rev_len 返回数据长度.
 * @param [out] rev_para 返回数据.
 * @return 0 true,1 false.
 * \endif
 */
ushort IDCARD_GetUid(unsigned char *rev_len,unsigned char *rev_para);
/** @}
* end of IDCARD
*/

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* _OSLIB_H_ */
