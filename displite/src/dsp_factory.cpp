#include "displays/display.h"

#include "displays/ili9486.h"
#include "displays/st7735.h"

/*
╔══════╦═════════════╦══════════════╦════════════╦══════════════╦═══════╦═══════════╗
║ spi  ║ chip select ║ data/command ║ serial out ║ signal clock ║ reset ║ backlight ║
╠══════╬═════════════╬══════════════╬════════════╬══════════════╬═══════╬═══════════╣
║ spi0 ║     17      ║      20      ║     19     ║      18      ║  21   ║    22     ║
╚══════╩═════════════╩══════════════╩════════════╩══════════════╩═══════╩═══════════╝
*/

namespace display {
    display * get_available_dsp() {
        #if defined(USE_ILI9486)
        return new ili9486(spi0, 17, 20, 19, 18, 21, 22);
        #elif defined(USE_ST7735)
        return new st7735(spi0, 17, 20, 19, 18, 21, 22);
        #elif
        #error Display driver does not exist
        #endif

    }
}

