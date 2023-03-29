#ifndef _GUI_H_
#define _GUI_H_

#include "ui.h"
#include "displays/ili9486.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GUI_PREFERRED_ROTATION 1

namespace ui {
    class portar: public ui {
        public:
        portar(lv_disp_t *);
    };
}




// Do not modify
#define GUI_CLS ui::portar
#define DSP_DRV_CLS display::ili9486

#ifdef __cplusplus
}
#endif

#endif