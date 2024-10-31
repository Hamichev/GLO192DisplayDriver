#ifndef INC_GLO192_H_
#define INC_GLO192_H_

#define GLO192_CS								0

#define GLO192_CMD_SET_CONTRAST                    0x81
#define GLO192_CMD_ENTIRE_DISPLAY_ON               0xA4
#define GLO192_CMD_SET_INVERSION                   0xA6
#define GLO192_CMD_DISPLAY_ON_OFF                  0xAE
#define GLO192_CMD_SET_MEM_ADDR_MODE               0x20
#define GLO192_CMD_SET_COLUMN_ADDR                 0x21
#define GLO192_CMD_SET_ROW_ADDR                    0x22
#define GLO192_CMD_SET_PAGE_START_ADDR             0xB0
#define GLO192_CMD_SET_START_LINE                  0x40
#define GLO192_CMD_SET_CLOCK                       0xD5
#define GLO192_CMD_SET_MULTIPLEX_RATIO             0xA8
#define GLO192_CMD_SET_OFFSET                      0xD3
#define GLO192_CMD_SET_HW_CONFIG                   0xDA
#define GLO192_CMD_SET_PRECHARGE                   0xD9
#define GLO192_CMD_SET_VCOMH_LEVEL                 0xDB

#define GLO192_CMD_SET_LOWER_COL_START_ADDR        0x00
#define GLO192_CMD_SET_HIGHER_COL_START_ADDR       0x10

//TIMINGS
#define DISPLAY_STARTUP_TIME1                   2
#define DISPLAY_STARTUP_TIME2                   2

//COMMON
#define DISP_OFF                                0
#define DISP_ON                                 1

#define GLO192_GPIO_PIN_RESET					0
#define GLO192_GPIO_PIN_SET						1

//COMMON
#define MODE_COMMAND                                0
#define MODE_DATA                                 	 1

#define GLO192_HEIGHT                              160
// each page contains 128 columns 1 byte in height, each bit represents 1 dot
#define GLO192_WIDTH                               128

enum{
	RATIO_1_TO_1 = 1,
	RATIO_1_TO_2,
	RATIO_1_TO_3,
	RATIO_1_TO_4
};


typedef struct {
	void (*SetA0)(uint8_t state);
	void (*SetnRST)(uint8_t state);
	void (*SetCS)(uint8_t state);
	void (*SendData)(const uint8_t *data, uint16_t start, uint16_t size);
	void (*Delay)(uint32_t ms);

	uint8_t CmdData[10];
	uint8_t CmdDataLen;
} GLO192_t;

void GLO192Init(GLO192_t *glo192, void (*GpioSetA0Func)(uint8_t state), void (*GpioSetnRSTFunc)(uint8_t state), void (*GpioSetCSFunc)(uint8_t state), void (PointSendData)(const uint8_t *data, uint16_t start, uint16_t size));
void SendCommand(GLO192_t *glo192);
void SendData(GLO192_t *glo192, const uint8_t *data, uint16_t start, uint16_t size);

void GLO192Power(GLO192_t *glo192, uint8_t on);
void GLO192Display(GLO192_t *glo192, uint8_t xStart, uint8_t xSize, uint8_t yStart, uint8_t ySize, uint8_t *data, uint8_t koef);


#endif /* INC_GLO192_H_ */
