#ifndef HID_H
#define HID_H

#include "tusb.h"

#ifdef __cplusplus
extern "C" {
#endif

class usbstate {
    public:
    static const unsigned short not_mounted{250};
    static const unsigned short mounted{1000};
    static const unsigned short suspended{2500};
};

#ifdef __cplusplus
}
#endif

#endif //HID_H