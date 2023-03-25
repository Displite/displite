#ifndef _GUI_H_
#define _GUI_H_

#include "displays/ili9486.h"

#ifdef __cplusplus
extern "C" {
#endif

class portar {

};

#define GUI_CLS portar
#define DSP_DRV_CLS display::ili9486

#ifdef __cplusplus
}
#endif

#endif