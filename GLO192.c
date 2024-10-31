#include <glo192.h>

void GLO192Init(GLO192_t *glo192, void (*GpioSetA0Func)(uint8_t state), void (*GpioSetnRSTFunc)(uint8_t state), void (*GpioSetCSFunc)(uint8_t state),
                                  void (PointSendData)(const uint8_t *data, uint16_t start, uint16_t size), void (*DelayFunc)(uint32_t ms))
{
    glo192->SetA0 = GpioSetA0Func;
    glo192->SetnRST = GpioSetnRSTFunc;
    glo192->SetCS = GpioSetCSFunc;
    glo192->SendData = PointSendData;
    glo192->Delay = DelayFunc;

    if (glo192->SetA0 != NULL) (*glo192->SetA0)(GLO192_GPIO_PIN_RESET);
    if (glo192->SetnRST != NULL) (*glo192->SetnRST)(GLO192_GPIO_PIN_SET);
    if (glo192->SetCS != NULL) (*glo192->SetCS)(GLO192_GPIO_PIN_SET);

    DisplayOnOff(glo192, DISP_OFF);
    DisplaySetClock(glo192, 0x80);
    DisplaySetMiltiplexRatio(glo192, 0x3F);
    DisplaySetOffset(glo192, 0x00);
    DisplaySetHWConfig(glo192, 0x12);
    DisplaySetPrecharge(glo192, 0xF1);
    DisplaySetVCOMHLevel(glo192, 0x40);
}

void SendCommand(GLO192_t *glo192)
{
	if (glo192->SetA0 != NULL) (*glo192->SetA0)(GLO192_GPIO_PIN_RESET);
    if (glo192->SetCS != NULL) (*glo192->SetCS)(GLO192_GPIO_PIN_RESET);
    if (glo192->SendData != NULL) (*glo192->SendData)(glo192->CmdData, 0, glo192->CmdDataLen);
    if (glo192->SetCS != NULL) (*glo192->SetCS)(GLO192_GPIO_PIN_SET);
}
void SendData(GLO192_t *glo192, const uint8_t *data, uint16_t start, uint16_t size)
{
	if (glo192->SetA0 != NULL) (*glo192->SetA0)(GLO192_GPIO_PIN_SET);
    if (glo192->SetCS != NULL) (*glo192->SetCS)(GLO192_GPIO_PIN_RESET);
    if (glo192->SendData != NULL) (*glo192->SendData)(data, start, size);
    if (glo192->SetCS != NULL) (*glo192->SetCS)(GLO192_GPIO_PIN_SET);
}

void GLO192Power(GLO192_t *glo192, uint8_t on)
{
    if (on) {
        if (glo192->SetnRST != NULL) (*glo192->SetnRST)(GLO192_GPIO_PIN_RESET);
        if (glo192->Delay != NULL) (*glo192->Delay)(DISPLAY_STARTUP_TIME1);
        if (glo192->SetnRST != NULL) (*glo192->SetnRST)(GLO192_GPIO_PIN_RESET);
        if (glo192->Delay != NULL) (*glo192->Delay)(DISPLAY_STARTUP_TIME2);

        EntireDisplayOn(glo192, 0);
        DisplayOnOff(glo192, DISP_ON);
    } else {
        DisplayOnOff(glo192, DISP_OFF);
    }
}

void ConvertSizeSymbol(uint8_t *new_data, uint16_t new_data_size, uint8_t *data, uint16_t data_xSize, uint16_t data_ySize, uint8_t koef)
{
	uint8_t Hi4bit;
	uint8_t Lo4bit;

	for(uint16_t i=0; i<data_ySize; i++){
		for(uint16_t j=0; j<(data_xSize/2); j++){ //div 2 - spec font 6x8
			switch(koef){
			case RATIO_1_TO_2:
				Hi4bit = (*(data + i * (data_xSize/2) + j) & 0xF0) >> 4;
				Lo4bit = *(data + i * (data_xSize/2) + j) & 0x0F;
				*(new_data + i * (data_xSize/2) * (koef * koef) + j*koef) = Hi4bit << 4 | Hi4bit;
				*(new_data + i * (data_xSize/2) * (koef * koef) + j*koef + ((data_xSize/2) * koef)) = Hi4bit << 4 | Hi4bit; //  + ((data_xSize/2) * koef) - next str
				*(new_data + i * (data_xSize/2) * (koef * koef) + (j*koef+1)) = Lo4bit << 4 | Lo4bit;
				*(new_data + i * (data_xSize/2) * (koef * koef) + (j*koef+1) + ((data_xSize/2) * koef)) = Lo4bit << 4 | Lo4bit;
				break;
			case RATIO_1_TO_3:
				Hi4bit = (*(data + i * (data_xSize/2) + j) & 0xF0) >> 4;
				Lo4bit = *(data + i * (data_xSize/2) + j) & 0x0F;
				*(new_data + i * (data_xSize/2) * (koef * koef) + j*koef) = Hi4bit << 4 | Hi4bit;
				*(new_data + i * (data_xSize/2) * (koef * koef) + j*koef + ((data_xSize/2) * koef)) = Hi4bit << 4 | Hi4bit; //  + ((data_xSize/2) * koef) - next str
				*(new_data + i * (data_xSize/2) * (koef * koef) + j*koef + 2 * ((data_xSize/2) * koef)) = Hi4bit << 4 | Hi4bit;
				*(new_data + i * (data_xSize/2) * (koef * koef) + (j*koef+1)) = Hi4bit << 4 | Lo4bit;
				*(new_data + i * (data_xSize/2) * (koef * koef) + (j*koef+1) + ((data_xSize/2) * koef)) = Hi4bit << 4 | Lo4bit;
				*(new_data + i * (data_xSize/2) * (koef * koef) + (j*koef+1) + 2 * ((data_xSize/2) * koef)) = Hi4bit << 4 | Lo4bit;
				*(new_data + i * (data_xSize/2) * (koef * koef) + (j*koef+2)) = Lo4bit << 4 | Lo4bit;
				*(new_data + i * (data_xSize/2) * (koef * koef) + (j*koef+2) + ((data_xSize/2) * koef)) = Lo4bit << 4 | Lo4bit;
				*(new_data + i * (data_xSize/2) * (koef * koef) + (j*koef+2) + 2 * ((data_xSize/2) * koef)) = Lo4bit << 4 | Lo4bit;
				break;
			case RATIO_1_TO_4:
				Hi4bit = (*(data + i * (data_xSize/2) + j) & 0xF0) >> 4;
				Lo4bit = *(data + i * (data_xSize/2) + j) & 0x0F;
				*(new_data + i * (data_xSize/2) * (koef * koef) + j*koef) = Hi4bit << 4 | Hi4bit;
				*(new_data + i * (data_xSize/2) * (koef * koef) + j*koef + ((data_xSize/2) * koef)) = Hi4bit << 4 | Hi4bit; //  + ((data_xSize/2) * koef) - next str
				*(new_data + i * (data_xSize/2) * (koef * koef) + j*koef + 2 * ((data_xSize/2) * koef)) = Hi4bit << 4 | Hi4bit;
				*(new_data + i * (data_xSize/2) * (koef * koef) + j*koef + 3 * ((data_xSize/2) * koef)) = Hi4bit << 4 | Hi4bit;
				*(new_data + i * (data_xSize/2) * (koef * koef) + (j*koef+1)) = Hi4bit << 4 | Hi4bit;
				*(new_data + i * (data_xSize/2) * (koef * koef) + (j*koef+1) + ((data_xSize/2) * koef)) = Hi4bit << 4 | Hi4bit;
				*(new_data + i * (data_xSize/2) * (koef * koef) + (j*koef+1) + 2 * ((data_xSize/2) * koef)) = Hi4bit << 4 | Hi4bit;
				*(new_data + i * (data_xSize/2) * (koef * koef) + (j*koef+1) + 3 * ((data_xSize/2) * koef)) = Hi4bit << 4 | Hi4bit;
				*(new_data + i * (data_xSize/2) * (koef * koef) + (j*koef+2)) = Lo4bit << 4 | Lo4bit;
				*(new_data + i * (data_xSize/2) * (koef * koef) + (j*koef+2) + ((data_xSize/2) * koef)) = Lo4bit << 4 | Lo4bit;
				*(new_data + i * (data_xSize/2) * (koef * koef) + (j*koef+2) + 2 * ((data_xSize/2) * koef)) = Lo4bit << 4 | Lo4bit;
				*(new_data + i * (data_xSize/2) * (koef * koef) + (j*koef+2) + 3 * ((data_xSize/2) * koef)) = Lo4bit << 4 | Lo4bit;
				*(new_data + i * (data_xSize/2) * (koef * koef) + (j*koef+3)) = Lo4bit << 4 | Lo4bit;
				*(new_data + i * (data_xSize/2) * (koef * koef) + (j*koef+3) + ((data_xSize/2) * koef)) = Lo4bit << 4 | Lo4bit;
				*(new_data + i * (data_xSize/2) * (koef * koef) + (j*koef+3) + 2 * ((data_xSize/2) * koef)) = Lo4bit << 4 | Lo4bit;
				*(new_data + i * (data_xSize/2) * (koef * koef) + (j*koef+3) + 3 * ((data_xSize/2) * koef)) = Lo4bit << 4 | Lo4bit;
				break;
			default:
				break;
			}
		}
	}
}

void GLO192Display(GLO192_t *glo192, uint8_t xStart, uint8_t xSize, uint8_t yStart, uint8_t ySize, uint8_t *data, uint8_t koef)
{
	//Column GLO192 - this is row and row GLO192 - column
    SetMemAddrMode(glo192, 0);

    uint8_t t[(xSize*ySize/2) * koef * koef];

    if(RATIO_1_TO_1 != koef){

        ConvertSizeSymbol(&t, (xSize*ySize/2) * 2 * 2, data, xSize, ySize, koef);

        data = &t;
        xSize *= koef;
        ySize *= koef;
    }

    uint16_t size = ySize / 2;
    uint8_t buf[size];

    for(uint16_t i=0; i < xSize; i++){
        SetColumnAddr(glo192, yStart/2, (yStart + ySize - 1)/2);
        SetRowAddr(glo192, xStart+i, size);

        for(uint16_t j=0; j < sizeof(buf); j++){
        	if(i%2 == 0){
            	buf[j] = (*(data+(xSize/2)*(j*2 + 1) + i/2) & 0xF0) | ((*(data+(xSize/2)*j*2 + i/2) & 0xF0) >> 4);
        	}
        	else {
            	buf[j] = ((*(data+(xSize/2)*(j*2 + 1) + i/2) & 0x0F) << 4) | (*(data+(xSize/2)*j*2 + i/2) & 0x0F);
        	}
        }

        SendData(glo192, buf, 0, size);
    }

}

void EntireDisplayOn(GLO192_t *glo192, uint8_t on)
{
    if (on) {
    	glo192->CmdData[0] = (GLO192_CMD_ENTIRE_DISPLAY_ON | 1);
    } else {
    	glo192->CmdData[0] = (GLO192_CMD_ENTIRE_DISPLAY_ON);
    }
    glo192->CmdDataLen = 1;
    SendCommand(glo192);
}

void DisplayAnyComand(GLO192_t *glo192, uint8_t cmd)
{
	glo192->CmdData[0] = cmd;
    glo192->CmdData[1] = 0x01;

	glo192->CmdDataLen = 1;
	SendCommand(glo192);
}

void DisplayOnOff(GLO192_t *glo192, uint8_t on)
{
    if (on) {
        glo192->CmdData[0] = (GLO192_CMD_DISPLAY_ON_OFF | 1);
    } else {
        glo192->CmdData[0] = (GLO192_CMD_DISPLAY_ON_OFF);
    }
    glo192->CmdDataLen = 1;
    SendCommand(glo192);
}
void DisplaySetClock(GLO192_t *glo192, uint8_t val)
{
    glo192->CmdData[0] = GLO192_CMD_SET_CLOCK;
    glo192->CmdData[1] = val;

    glo192->CmdDataLen = 2;
    SendCommand(glo192);
}
void DisplaySetMiltiplexRatio(GLO192_t *glo192, uint8_t val)
{
    glo192->CmdData[0] = GLO192_CMD_SET_MULTIPLEX_RATIO;
    glo192->CmdData[1] = val;

    glo192->CmdDataLen = 2;
    SendCommand(glo192);
}
void DisplaySetOffset(GLO192_t *glo192, uint8_t val)
{
    glo192->CmdData[0] = GLO192_CMD_SET_OFFSET;
    glo192->CmdData[1] = val;

    glo192->CmdDataLen = 2;
    SendCommand(glo192);
}
void DisplaySetStartLine(GLO192_t *glo192, uint8_t val)
{
    glo192->CmdData[0] = GLO192_CMD_SET_START_LINE;
    glo192->CmdData[1] = val;

    glo192->CmdDataLen = 2;
    SendCommand(glo192);
}
void DisplaySetHWConfig(GLO192_t *glo192, uint8_t val)
{
    glo192->CmdData[0] = GLO192_CMD_SET_HW_CONFIG;
    glo192->CmdData[1] = val;

    glo192->CmdDataLen = 2;
    SendCommand(glo192);
}
void DisplaySetPrecharge(GLO192_t *glo192, uint8_t val)
{
    glo192->CmdData[0] = GLO192_CMD_SET_PRECHARGE;
    glo192->CmdData[1] = val;

    glo192->CmdDataLen = 2;
    SendCommand(glo192);
}
void DisplaySetVCOMHLevel(GLO192_t *glo192, uint8_t val)
{
    glo192->CmdData[0] = GLO192_CMD_SET_VCOMH_LEVEL;
    glo192->CmdData[1] = val;

    glo192->CmdDataLen = 2;
    SendCommand(glo192);
}

void SetMemAddrMode(GLO192_t *glo192, uint8_t mode)
{
	glo192->CmdData[0] = GLO192_CMD_SET_MEM_ADDR_MODE;
    glo192->CmdData[1] = mode & 0x03;

    glo192->CmdDataLen = 2;
    SendCommand(glo192);
}

void SetColumnAddr(GLO192_t *glo192, uint8_t start, uint8_t end)
{
	glo192->CmdData[0] = GLO192_CMD_SET_COLUMN_ADDR;
    glo192->CmdData[1] = start;
    glo192->CmdData[2] = end;

    glo192->CmdDataLen = 3;
    SendCommand(glo192);
}

void SetRowAddr(GLO192_t *glo192, uint8_t start, uint8_t end)
{
	glo192->CmdData[0] = GLO192_CMD_SET_ROW_ADDR;
	glo192->CmdData[1] = start;
	glo192->CmdData[2] = end;

	glo192->CmdDataLen = 3;
    SendCommand(glo192);
}
void ReverseDisplayOn(GLO192_t *glo192, uint8_t on)
{
    if (on) {
        glo192->CmdData[0] = (GLO192_CMD_SET_INVERSION | 1);
    } else {
        glo192->CmdData[0] = (GLO192_CMD_SET_INVERSION);
    }
    glo192->CmdDataLen = 1;
    SendCommand(glo192);
}
