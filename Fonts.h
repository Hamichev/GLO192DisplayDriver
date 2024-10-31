#ifndef INC_FONTS_H_
#define INC_FONTS_H_

#include <stdint.h>

typedef struct {
    uint8_t   w;      // width
    uint8_t   h;      // height
    uint8_t   bpc;    // byte for one symbol
    const uint8_t   *pDataFont; // data
} FONT;

extern FONT Font_6x8;

extern const uint8_t Font_6x8_Data[];

#endif /* INC_FONTS_H_ */
