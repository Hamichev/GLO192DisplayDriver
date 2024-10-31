#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_

#include <Fonts.h>
#include <GLO192.h>
#include <string.h>
#include <stdio.h>

enum{
	FIRST_LAYER = 0,
	SECOND_LAYER,
	THIRD_LAYER,
	FOURTH_LAYER
};

#define MAX_STR_LEN	21
#define MAX_VALUE_WIDTH             6
#define MAX_VALUE_1RANGE            10
#define MAX_VALUE_2RANGE            100
#define MAX_VALUE_3RANGE            9999
#define MAX_VALUE_4RANGE            999999
#define MIN_VALUE                   -999

#define DISPLAY_ERROR_NONE          0x00
#define DISPLAY_ERROR_INVALID_VALUE 0x01

#define DISPLAY_LEFT_SIDE_LINE_X            	0
#define DISPLAY_CENTER_LINE_X            		64
#define DISPLAY_RIGHT_SIDE_LINE_X            	128

#define DISPLAY_FIRST_LINE_Y            		0
#define DISPLAY_SECOND_LINE_Y            		8
#define DISPLAY_THIRD_LINE_Y            		16
#define DISPLAY_FOURTH_LINE_Y            		24
#define DISPLAY_FIFTH_LINE_Y            		32
#define DISPLAY_SIXTH_LINE_Y            		40
#define DISPLAY_SEVENTH_LINE_Y            		48
#define DISPLAY_EIGHTH_LINE_Y            		56
#define DISPLAY_NINTH_LINE_Y            		64
#define DISPLAY_TENTH_LINE_Y            		72
#define DISPLAY_ELEVENTH_LINE_Y            		80
#define DISPLAY_TWELFTH_LINE_Y            		88
#define DISPLAY_THIRTEENTH_LINE_Y            	96
#define DISPLAY_FOURTEENTH_LINE_Y            	104
#define DISPLAY_FIFTEENTH_LINE_Y            	112
#define DISPLAY_SIXTEENTH_LINE_Y            	120
#define DISPLAY_SEVENTEENTH_LINE_Y            	128
#define DISPLAY_EIGHTEENTH_LINE_Y            	136
#define DISPLAY_NINTEENTH_LINE_Y            	144
#define DISPLAY_TWENTIETH_LINE_Y            	152

void DisplayClear(GLO192_t *glo192);
void DisplayClearLayer(GLO192_t *glo192, uint8_t Num_layer, uint8_t Font_height, uint8_t Font_width);
void DisplayString(GLO192_t *glo192, FONT *CurrentFont, char *string, uint8_t maxLen, uint8_t xStart, uint8_t yStart, uint8_t ratio);
void DisplayEmptiness(GLO192_t *glo192, FONT *CurrentFont, uint8_t xStart, uint8_t yStart, uint8_t code, uint8_t widthLimit, uint8_t ratio);
void DisplayValue(GLO192_t *glo192, FONT *CurrentFont, float value, uint8_t precision, uint8_t xStart, uint8_t yStart, uint8_t ratio, uint8_t clear);

extern GLO192_t        GLO192;
#endif /* INC_DISPLAY_H_ */
