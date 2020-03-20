// Font structures for newer Adafruit_GFX (1.1 and later).
// Example fonts are included in 'Fonts' directory.
// To use a font in your Arduino sketch, #include the corresponding .h
// file and pass address of GFXfont struct to setFont().  Pass NULL to
// revert to 'classic' fixed-space bitmap font.

#ifndef _GFXFONT_H_
#define _GFXFONT_H_

/// Font data stored PER GLYPH
typedef struct {
	uint16_t bitmapOffset;     ///< Pointer into GFXfont->bitmap
	uint8_t  width;            ///< Bitmap dimensions in pixels
        uint8_t  height;           ///< Bitmap dimensions in pixels
	uint8_t  xAdvance;         ///< Distance to advance cursor (x axis)
	int8_t   xOffset;          ///< X dist from cursor pos to UL corner
        int8_t   yOffset;          ///< Y dist from cursor pos to UL corner
} GFXglyph;

/// Data stored for FONT AS A WHOLE
typedef struct {
	uint8_t  *bitmap;      ///< Glyph bitmaps, concatenated
	GFXglyph *glyph;       ///< Glyph array
	uint8_t   first;       ///< ASCII extents (first char)
    uint8_t   last;        ///< ASCII extents (last char)
	uint8_t   yAdvance;    ///< Newline distance (y axis)
} GFXfont;

#include "Fonts/FreeMono9pt7b.h"
#include "Fonts/FreeMono12pt7b.h"//正常字体
#include "Fonts/FreeMono18pt7b.h"//正常字体
#include "Fonts/FreeMono24pt7b.h"//正常字体
#include "Fonts/FreeMonoBold12pt7b.h"
#include "Fonts/FreeMonoBoldOblique12pt7b.h"
#include "Fonts/FreeMonoOblique12pt7b.h"
#include "Fonts/FreeSans12pt7b.h"
#include "Fonts/FreeSansBold9pt7b.h"
#include "Fonts/FreeSansBold12pt7b.h"
#include "Fonts/FreeSansBoldOblique12pt7b.h"
#include "Fonts/FreeSansOblique12pt7b.h"
#include "Fonts/FreeSerif12pt7b.h"
#include "Fonts/FreeSerifBold12pt7b.h"
#include "Fonts/FreeSerifBoldItalic12pt7b.h"
#include "Fonts/FreeSerifItalic12pt7b.h"

#include "Fonts/FreeSans18pt7b.h"

#include "Fonts/FreeSans9pt7b.h"



#endif // _GFXFONT_H_
