#ifndef _DEMO_H_
#define _DEMO_H_

#include "interface.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PREFERRED_ROTATION 1
#define PREFERRED_THEME lv_theme_basic_init

class demo: public interface::gui {
    public:
    demo(short, short);
};




// Do not modify
#define GUI_CLS demo

#ifdef __cplusplus
}
#endif

#endif