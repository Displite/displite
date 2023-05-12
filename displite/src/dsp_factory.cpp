#include "displays/display.h"

#include "displays/ili9486.h"

/*
╔══════╦═════════════╦══════════════╦════════════╦══════════════╦═══════╦═══════════╗
║ spi  ║ chip select ║ data/command ║ serial out ║ signal clock ║ reset ║ backlight ║
╠══════╬═════════════╬══════════════╬════════════╬══════════════╬═══════╬═══════════╣
║ spi0 ║     17      ║      20      ║     19     ║      18      ║  21   ║    22     ║
╚══════╩═════════════╩══════════════╩════════════╩══════════════╩═══════╩═══════════╝
*/

namespace display {
    display * get_available_dsp() {
        #ifdef USE_ILI9486
        return new ili9486(spi0, 17, 20, 19, 18, 21, 22);
        #endif
    }
}

