#ifndef _ILI9486_H_
#define _ILI9486_H_

#include "displays/display.h"

#ifdef USE_ILI9486

#ifdef __cplusplus
extern "C" {
#endif

namespace display {
    class ili9486: public spi {
        private:
        unsigned short horizontal_px{320};
        unsigned short vertical_px{480};

        public:
        ili9486(spi_inst_t *, uint, uint, uint, uint, uint = 0, uint = 0);
        void reset();
        void command(const uint8_t &);
        void data(const uint8_t &);
        void data16(const uint16_t &);
        void rotate(short);
        void get_display_size(unsigned short &, unsigned short &);
        void flush_pixels(const area &, lv_color_t *);

        protected:
        // https://github.com/Bodmer/TFT_eSPI/blob/master/TFT_Drivers/ILI9486_Init.h
        std::vector<init_cmd> cd_list {
            {ITFCTR1, {0x0}},
            {SLPOUT, {}, 250},
            {ITFPXFT, {0x55}},
            {PWCTR3, {0x44}},
            {VMCTR1, {0x00, 0x00, 0x00, 0x00}},
            {PGMCTR, {0x0F, 0x1F, 0x1C, 0x0C, 0x0F, 0x08, 0x48, 0x98,
	          0x37, 0x0A, 0x13, 0x04, 0x11, 0x0D, 0x00}},
            {NGMCTR, {0x0F, 0x32, 0x2E, 0x0B, 0x0D, 0x05, 0x47, 0x75,
	          0x37, 0x06, 0x10, 0x03, 0x24, 0x20, 0x00}},
            {DGMCTR1, {0x0F, 0x32, 0x2E, 0x0B, 0x0D, 0x05, 0x47, 0x75,
	          0x37, 0x06, 0x10, 0x03, 0x24, 0x20, 0x00}},
            {SLPOUT, {}},
            {DISPON, {}}
        };
    };
}

#ifdef __cplusplus
}
#endif

#endif // END USE_ILI9486

#endif