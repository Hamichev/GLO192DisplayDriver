#ifndef INC_DISPLAY_SCREENS_H_
#define INC_DISPLAY_SCREENS_H_

#include <stdint.h>
#include <Fonts.h>
#include <GLO192.h>
#include <stdbool.h>
#include <sys_utils.h>

#include "Display.h"

#define SCREENS_NUM             4

#define MAIN_SCREEN_TO          100000
#define SCREEN_BLINK_PERIOD     400

#define LANG_NUM                    2
#define MAX_LONG_WIDTH              21

typedef struct {
    char                Name[LANG_NUM][MAX_LONG_WIDTH];
} LONG_MESSAGE;

#define STATUS_ON_STRING   						{.Name[0] = {0x3C, 0x2D, 0x20, 0x20, 0xC2, 0xCA, 0xCB, 0x20, 0x20, 0x2D, 0x3E}, .Name[1] = "<-  ON  ->"}
#define STATUS_OFF_STRING    					{.Name[0] = {0x3C, 0x2D, 0x20, 0x20, 0xC2, 0xDB, 0xCA, 0xCB, 0x20, 0x20, 0x2D, 0x3E}, .Name[1] = "<-  OFF  ->"}
#define STATUS_NAN_STRING    					{.Name[0] = {0x3C, 0x2D, 0x20, 0x20, 0x4E, 0x61, 0x4E, 0x20, 0x20, 0x2D, 0x3E}, .Name[1] = "<-  NaN  ->"}


#define BME280_HEADER	    					{.Name[0] = {0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0}, .Name[1] = "<-  NaN  ->"}

enum{
	NAN_TESTING = -1,
	OFF_TESTING = 0,
	ON_TESTING
};


struct screen_t {
    struct screen_t *prev;
    struct screen_t *next;
    void (*DisplayScreen)(bool left, bool right, bool enter, uint8_t arg);
};

typedef enum {SM_DISPLAY = 0, SM_SETTING, SM_CONFIRM} screen_mode_t;

void ScreensInit(void);
void ShowStartScreen(uint32_t counter, uint32_t interval);
void ShowScreen(bool down, bool up, bool enter, uint8_t arg);

#endif /* INC_DISPLAY_SCREENS_H_ */
