#include <Display_screens.h>

static struct screen_t              ScreensMain[SCREENS_NUM] = {0};
static struct screen_t              *screenMain = &ScreensMain[0];
static screen_mode_t                ScreenMainMode = SM_DISPLAY;

static uint8_t BlockEnter = 0;

uint8_t	 Language = 0;

static void MainScreen(bool down, bool up, bool enter, uint8_t arg);
static void BME280DataScreen(bool down, bool up, bool enter, uint8_t arg);
static void TAU1103DataScreen(bool down, bool up, bool enter, uint8_t arg);


const LONG_MESSAGE          StatusOn = STATUS_ON_STRING;
const LONG_MESSAGE          StatusOff = STATUS_OFF_STRING;
const LONG_MESSAGE          StatusNaN = STATUS_NAN_STRING;

void ShowScreen(bool down, bool up, bool enter, uint8_t arg)
{
    switch (arg) {
        case 1:
        case 2:
        case 3:
        break;
        default:
            if (ScreenMainMode == SM_DISPLAY) {
                if (down) {
                    screenMain = screenMain->prev;
                    DisplayClear();
                } else {
                    if (up) {
                        screenMain = screenMain->next;
                        DisplayClear();
                    }
                }
            }

            screenMain->DisplayScreen(down, up, enter, arg);
    }
}

void IntermediateScreen(bool down, bool up, bool enter,
						uint8_t next_layer, uint8_t clear_layer,
						screen_mode_t *Mode, struct screen_t** Screen,
						LONG_MESSAGE *Header, uint8_t header_width, uint8_t header_line_x, uint8_t header_line_y,
						LONG_MESSAGE *Bottom, uint8_t bottom_width, uint8_t bottom_line_x, uint8_t bottom_line_y,
						FONT *Font,
						uint8_t ratio)
{
    switch (*Mode) {
        case SM_DISPLAY:
        	DisplayString(Font, (char *)Header, header_width, header_line_x, header_line_y, ratio); //Header
            DisplayString(Font, (char *)Bottom, bottom_width, bottom_line_x, bottom_line_y, ratio); //bottom confirm
            if (BlockEnter == 0) {
            	BlockEnter = 1;
            	(*Mode) = SM_SETTING;
            }
        break;
        case SM_SETTING:
            if (down){
            	*Screen = ((**Screen).prev);
            	DisplayClearLayer(clear_layer, Font->h, Font->w);
            }
            else if (up) {
            	*Screen = ((**Screen).next);
            	DisplayClearLayer(clear_layer, Font->h, Font->w);
			}
            if (BlockEnter == 0) {
            	(*Mode) = SM_CONFIRM;
                DisplayClearLayer(clear_layer, Font->h, Font->w);
            }

            (**Screen).DisplayScreen(down, up, enter, next_layer);
        break;
        case SM_CONFIRM:
        	(**Screen).DisplayScreen(down, up, enter, next_layer);
		break;
        default:
        break;
    }
}

screen_mode_t SwitchScreen(bool down, bool up, bool enter,
				  screen_mode_t *Mode,
				  uint16_t *val,
				  LONG_MESSAGE *Header, uint8_t header_width, uint8_t header_line_x, uint8_t header_line_y,
				  LONG_MESSAGE *Bottom_Display, uint8_t botton_display_width, uint8_t botton_display_line_x, uint8_t botton_display_line_y,
				  LONG_MESSAGE *Bottom_Setting, uint8_t botton_setting_width, uint8_t botton_setting_line_x, uint8_t botton_setting_line_y,
				  uint8_t status_width, uint8_t status_line_x, uint8_t status_line_y,
				  FONT *Font,
				  uint8_t num_bits,
				  uint8_t ratio)
{
    switch (*Mode) {
        case SM_DISPLAY:
        	DisplayString(Font, (char *)Header, header_width, header_line_x, header_line_y, ratio); //Header
            DisplayString(Font, (char *)Bottom_Display, botton_display_width, botton_display_line_x, botton_display_line_y, ratio); //bottom confirm
            if (BlockEnter == 0) {
            	BlockEnter = 1;
            	(*Mode) = SM_SETTING;
            }
        break;
        case SM_SETTING:
            DisplayString(Font, (char *)Bottom_Setting, botton_setting_width, botton_setting_line_x, botton_setting_line_y, ratio);
            if ((down) || (up)){
            	if((((*val) >> num_bits) & 1) == OFF_TESTING)
					(*val) |= (1 << num_bits);
				else if ((((*val) >> num_bits) & 1) == ON_TESTING)
					(*val) &= ~(1 << num_bits);
				else
					(*val) = NAN_TESTING;
            }
            else if (BlockEnter == 0) {
            	(*Mode) = SM_CONFIRM;
            }

            if((((*val) >> num_bits) & 1) == ON_TESTING)
            	DisplayString(Font, (char *)&StatusOn.Name[Language][0], status_width, status_line_x, status_line_y, ratio);
            else if((((*val) >> num_bits) & 1) == OFF_TESTING)
            	DisplayString(Font, (char *)&StatusOff.Name[Language][0], status_width, status_line_x, status_line_y, ratio);
            else
            	DisplayString(Font, (char *)&StatusNaN.Name[Language][0], status_width, status_line_x, status_line_y, ratio);
        break;
        default:
        break;
    }

    return (*Mode);
}

static void MainScreen(bool down, bool up, bool enter, uint8_t arg)
{
	char buf_str[MAX_STR_LEN];

	char coin_str[MAX_STR_LEN/RATIO_1_TO_4];
	for(uint8_t i=0; i<(MAX_STR_LEN/RATIO_1_TO_4); i++)	coin_str[i] = 0x01;

#define FW_VERSION	1
#define FW_VERSION_LO	1
#define FW_VERSION_HI	1


	DisplayString(&Font_6x8, "Device", (MAX_STR_LEN/RATIO_1_TO_2), DISPLAY_LEFT_SIDE_LINE_X, DISPLAY_FIRST_LINE_Y, RATIO_1_TO_2);

	sprintf(buf_str, "Version : %3d", FW_VERSION);
	DisplayString(&Font_6x8, (char *)&buf_str[0], MAX_STR_LEN, DISPLAY_LEFT_SIDE_LINE_X, DISPLAY_THIRD_LINE_Y, RATIO_1_TO_1);

	sprintf(buf_str, "Date Version : %4d", FW_VERSION_LO);
	DisplayString(&Font_6x8, (char *)&buf_str[0], MAX_STR_LEN, DISPLAY_LEFT_SIDE_LINE_X, DISPLAY_FOURTH_LINE_Y, RATIO_1_TO_1);

	sprintf(buf_str, "Year Version : %4d", FW_VERSION_HI);
	DisplayString(&Font_6x8, (char *)&buf_str[0], MAX_STR_LEN, DISPLAY_LEFT_SIDE_LINE_X, DISPLAY_FIFTH_LINE_Y, RATIO_1_TO_1);

	DisplayString(&Font_6x8, (char *)&coin_str[0], (MAX_STR_LEN/RATIO_1_TO_4), DISPLAY_LEFT_SIDE_LINE_X, DISPLAY_SIXTH_LINE_Y, RATIO_1_TO_4);
}

static void BME280DataScreen(bool down, bool up, bool enter, uint8_t arg)
{
	char temp_str[MAX_STR_LEN];
	char press_str[MAX_STR_LEN];
	char hum_str[MAX_STR_LEN];

	float Temperature = 30.02;
	uint32_t Pressure = 100500;
	float Humidity = 10.3;

	DisplayString(&Font_6x8, "BME280", (MAX_STR_LEN/RATIO_1_TO_2), DISPLAY_LEFT_SIDE_LINE_X, DISPLAY_FIRST_LINE_Y, RATIO_1_TO_2);

	switch (ScreenMainMode) {
		case SM_DISPLAY:
			sprintf(&temp_str[0], "Temp: %.2f C", Temperature);
			DisplayString(&Font_6x8, (char *)&temp_str[0], MAX_STR_LEN, DISPLAY_LEFT_SIDE_LINE_X, DISPLAY_FOURTH_LINE_Y, RATIO_1_TO_1);
			sprintf(&press_str[0], "Pressure: %8d Pa", (int)Pressure);
			DisplayString(&Font_6x8, (char *)&press_str[0], MAX_STR_LEN, DISPLAY_LEFT_SIDE_LINE_X, DISPLAY_FIFTH_LINE_Y, RATIO_1_TO_1);
			sprintf(&hum_str[0], "Humidity: %.2f %%RH", Humidity);
			DisplayString(&Font_6x8, (char *)&hum_str[0], MAX_STR_LEN, DISPLAY_LEFT_SIDE_LINE_X, DISPLAY_SIXTH_LINE_Y, RATIO_1_TO_1);
			if (enter) {
				ScreenMainMode = SM_SETTING;
				BlockEnter = 1;
			}
		break;
		case SM_SETTING:

			if (down){


			}
			else if(up){

			}
			else if (BlockEnter == 0) {
				ScreenMainMode = SM_CONFIRM;
			}
		break;
		case SM_CONFIRM:
			BlockEnter = 1;
		break;
	}
}

static void SecondScreen(bool down, bool up, bool enter, uint8_t arg)
{
	IntermediateScreen(left, right, enter,
						/*next_layer*/ SECOND_LAYER,
						/*clear_layer*/ FIRST_LAYER,
						/*Mode*/ &ScreenMainMode,
						/*Screen*/ &screenTesting,
						/*Header*/ &Header.Name[Language][0], /*header_width*/ MAX_LONG_WIDTH_FIRST_HEADER, /*header_line_x*/ DISPLAY_FIRST_LINE_X, /*header_line_y*/ DISPLAY_FIRST_LINE_Y,
						/*Bottom*/ &Confirm.Name[Language][0], /*bottom_width*/ MAX_LONG_WIDTH, /*bottom_line_x*/ DISPLAY_SHORT_MESSAGE_X, /*bottom_line_y*/ DISPLAY_SHORT_MESSAGE_Y,
						/*Font*/ &Font_6x8);
}

static void ThirdScreen(bool down, bool up, bool enter, uint8_t arg)
{
	if(SwitchScreen(left,right, enter,
					   /*Mode*/ &ScreenOLEDMode,
					   /*val*/ &LedGreen,
					   /*Header*/ &LedGreenHeader.Name[Language][0], /*header_width*/ MAX_LONG_WIDTH, /*header_line_x*/ DISPLAY_THIRD_LINE_X, /*header_line_y*/ DISPLAY_THIRD_LINE_Y,
					   /*Bottom_Display*/ &Confirm.Name[Language][0], /*botton_display_width*/ MAX_LONG_WIDTH, /*botton_display_line_x*/ DISPLAY_SHORT_MESSAGE_X, /*botton_display_line_y*/ DISPLAY_SHORT_MESSAGE_Y,
					   /*Bottom_Setting*/ &EnterBack.Name[Language][0], /*botton_setting_width*/ MAX_LONG_WIDTH, /*botton_setting_line_x*/ DISPLAY_SHORT_MESSAGE_X, /*botton_setting_line_y*/ DISPLAY_SHORT_MESSAGE_Y,
					   /*status_width*/ MAX_LONG_WIDTH, /*status_line_x*/ DISPLAY_FOURTH_LINE_X, /*status_line_y*/ DISPLAY_FOURTH_LINE_Y,
					   /*FONT*/ &Font_6x8,
					   /*num_bits*/ DEFAULT_NUM_BITS) == SM_CONFIRM)
		EXIT(ScreenOLEDMode,LedGreen);
}

void ScreensLink(struct screen_t *pointerScreens, uint16_t lenScreens)
{
#define SCREEN_LINK(_IND_)          		((struct screen_t *)((uint16_t *)pointerScreens + (sizeof(struct screen_t)/2) * (_IND_)))

	SCREEN_LINK(0)->prev = SCREEN_LINK(lenScreens - 1);
	SCREEN_LINK(0)->next = SCREEN_LINK(1);
	for (uint8_t i = 1; i < lenScreens - 1; i++){
		SCREEN_LINK(i)->prev = SCREEN_LINK(i - 1);
		SCREEN_LINK(i)->next = SCREEN_LINK(i + 1);
	}
	SCREEN_LINK(lenScreens - 1)->prev = SCREEN_LINK(lenScreens - 2);
	SCREEN_LINK(lenScreens - 1)->next = SCREEN_LINK(0);
}

void ScreensInit(void)
{
	ScreensLink(&ScreensMain[0], SCREENS_NUM);

    ScreensMain[0].DisplayScreen = MainScreen;
    ScreensMain[1].DisplayScreen = BME280DataScreen;
    ScreensMain[2].DisplayScreen = SecondScreen;
    ScreensMain[3].DisplayScreen = ThirdScreen;
}
