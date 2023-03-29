#ifndef _GUI_H_
#define _GUI_H_

#include "interface.h"
#include "displays/ili9486.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GUI_PREFERRED_ROTATION 1

class portar: public interface::gui {
    public:
    portar(const lv_disp_t *);
};




// Do not modify
#define GUI_CLS portar
#define DSP_DRV_CLS display::ili9486

#ifdef __cplusplus
}
#endif

#endif