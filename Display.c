#include "Display.h"

uint8_t	DisplayData[GLO192_WIDTH*GLO192_HEIGHT];

void DisplayClear(GLO192_t *glo192)
{
    memset(DisplayData, 0x00, GLO192_WIDTH*GLO192_HEIGHT);
    GLO192Display(glo192, 0, GLO192_WIDTH, 0, GLO192_HEIGHT, DisplayData, RATIO_1_TO_1);
}

void DisplayClearLayer(GLO192_t *glo192, uint8_t Num_layer, uint8_t Font_height, uint8_t Font_width)
{
    memset(DisplayData, 0x00, GLO192_WIDTH * GLO192_HEIGHT);
    GLO192Display(glo192, 							 0, 										 GLO192_WIDTH,
    					 Font_height * (Num_layer + 1), (GLO192_HEIGHT * 8) - (Font_height * (Num_layer + 1)), DisplayData, RATIO_1_TO_1);
}

void DisplayString(GLO192_t *glo192, FONT *CurrentFont, char *string, uint8_t maxLen, uint8_t xStart, uint8_t yStart, uint8_t ratio)
{
char str[MAX_STR_LEN];
uint8_t i, n, s;

    n = strlen(string);
    if (n > maxLen){
        n = maxLen;
    }
    strncpy(str, string, n);
    s = (maxLen - n) / 2;    // for center align
    for (i = 0; i < maxLen - s; i++){
        str[maxLen - 1 - i] = str[maxLen - 1 - i - s];
    }
    for (i = 0; i < s; i++) str[i] = 0x20;
    for (i = n + s; i < maxLen; i++) str[i] = 0x20;
    for (i = 0; i < maxLen; i++)
    	GLO192Display(glo192, xStart + CurrentFont->w * i * ratio, CurrentFont->w, yStart, CurrentFont->h, &CurrentFont->pDataFont[CurrentFont->bpc * str[i]], ratio);
}

void DisplayEmptiness(GLO192_t *glo192, FONT *CurrentFont, uint8_t xStart, uint8_t yStart, uint8_t code, uint8_t widthLimit, uint8_t ratio)
{
char strConc[MAX_VALUE_WIDTH];

    if (widthLimit > MAX_VALUE_WIDTH) widthLimit = MAX_VALUE_WIDTH;

    memset(strConc, 0x20, widthLimit);
    for (uint8_t i = 0; i < widthLimit; i++)
    	GLO192Display(glo192, xStart + CurrentFont->w * i * ratio, CurrentFont->w, yStart, CurrentFont->h, &CurrentFont->pDataFont[CurrentFont->bpc * strConc[i]], ratio);
}

void DisplayValue(GLO192_t *glo192, FONT *CurrentFont, float value, uint8_t precision, uint8_t xStart, uint8_t yStart, uint8_t ratio, uint8_t clear)
{

typedef enum{
	NONE_CLEAR,
	CLEAR_SYMBOL,
	CLEAR_STRING
};
char str[MAX_VALUE_WIDTH];
uint8_t i, n, s;
uint8_t widthLimit;

    if (CLEAR_SYMBOL == clear) {
        if (value <= MAX_VALUE_3RANGE) DisplayEmptiness(CurrentFont, xStart, yStart, DISPLAY_ERROR_INVALID_VALUE, 5, ratio);
        else DisplayEmptiness(CurrentFont, xStart, yStart, DISPLAY_ERROR_INVALID_VALUE, 6, ratio);
        return;
    }
    else if(CLEAR_STRING == clear){
    	DisplayEmptiness(CurrentFont, xStart, yStart, DISPLAY_ERROR_INVALID_VALUE, 21, ratio);
    	return;
    }
    if ((value > MIN_VALUE) && (value < MAX_VALUE_1RANGE)) {
        widthLimit = 5;
        switch (precision) {
            case 1:
                n = sprintf(str, "%4d", (uint16_t)value);
            break;
            case 10:
                n = sprintf(str, "%.1f", value);
            break;
            case 100:
                n = sprintf(str, "%.2f", value);
            break;
            default:
                n = sprintf(str, "%.1f", value);
        }
        if ((n > 0) && (n <= widthLimit)){
            s = widthLimit - n;
            for (i = 0; i < widthLimit - s; i++){
                str[widthLimit - 1 - i] = str[widthLimit - 1 - i - s];
            }
            for (i = 0; i < s; i++) str[i] = 0x20;
            for (i = 0; i < widthLimit; i++)
            	GLO192Display(glo192, xStart + CurrentFont->w * i * ratio, CurrentFont->w, yStart, CurrentFont->h, &CurrentFont->pDataFont[CurrentFont->bpc * str[i]], ratio);
        }
    } else {
        if ((value >= MAX_VALUE_1RANGE) && (value < MAX_VALUE_2RANGE)){
            widthLimit = 5;
            if (precision == 1) {
                n = sprintf(str, "%4d", (uint16_t)value);
            } else {
                n = sprintf(str, "%.1f", value);
            }
            if ((n > 0) && (n <= widthLimit)){
                s = widthLimit - n;
                for (i = 0; i < widthLimit - s; i++){
                    str[widthLimit - 1 - i] = str[widthLimit - 1 - i - s];
                }
                for (i = 0; i < s; i++) str[i] = 0x20;
                for (i = 0; i < widthLimit; i++)
                	GLO192Display(glo192, xStart + CurrentFont->w * i * ratio, CurrentFont->w, yStart, CurrentFont->h, &CurrentFont->pDataFont[CurrentFont->bpc * str[i]], ratio);
            }
        } else {
            if ((value >= MAX_VALUE_2RANGE) && (value <= MAX_VALUE_3RANGE)){
                widthLimit = 5;
                value += 0.5;
                sprintf(str, "%4d", (uint16_t)value);
                for (i = 0; i < widthLimit; i++)
                	GLO192Display(glo192, xStart + CurrentFont->w * i * ratio, CurrentFont->w, yStart, CurrentFont->h, &CurrentFont->pDataFont[CurrentFont->bpc * str[i]], ratio);
            } else {
                widthLimit = 6;
                if ((value >= MAX_VALUE_3RANGE) && (value <= MAX_VALUE_4RANGE)){
                    value += 0.5;
                    sprintf(str, "%6d", (int)value);
                    for (i = 0; i < widthLimit; i++)
                    	GLO192Display(glo192, xStart + CurrentFont->w * i * ratio, CurrentFont->w, yStart, CurrentFont->h, &CurrentFont->pDataFont[CurrentFont->bpc * str[i]], ratio);
                } else {
                    DisplayEmptiness(CurrentFont, xStart, yStart, DISPLAY_ERROR_INVALID_VALUE, 5, ratio);
                }
            }
        }
    }
}
