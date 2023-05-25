#ifndef _ST7735_H_
#define _ST7735_H_

#include "displays/display.h"

#ifdef USE_ST7735

#ifdef __cplusplus
extern "C" {
#endif

namespace display {
    class st7735: public spi {
        public:
        st7735(spi_inst_t *, uint, uint, uint, uint, uint, uint);
        void reset();
        void command(const uint8_t &);
        void data(const uint8_t &);
        void data16(const uint16_t &);
        void rotate(short);
        void flush_pixels(const lv_area_t &, lv_color_t *);

        protected:
        std::vector<init_cmd> cd_list {
            {SWRST, {}, 200},
            {SLPOUT, {}, 255},
            {FRMCTR1, {0x01, 0x2C, 0x2D}},
            {FRMCTR2, {0x01, 0x2C, 0x2D}},
            {FRMCTR3, {0x01, 0x2C, 0x2D, 0x01, 0x2C, 0x2D}},
            {INVCTR, {0x07}},
            {PWCTR1, {0xA2, 0x02, 0x84}},
            {PWCTR2, {0xC5}},
            {PWCTR3, {0x0A, 0x00}},
            {PWCTR4, {0x8A, 0x2A}},
            {PWCTR5, {0x8A, 0xEE}},
            {VMCTR1, {0x0E}},
            {INVOFF, {}},
            {MADCTL, {0xC0}},
            {ITFPXFT, {0x05}},
            {CASET, {0x00, 0x02, 0x00, 0x7F+0x02}},
            {RASET, {0x00, 0x01, 0x00, 0x9F+0x01}},
            {PGMCTR, {0x02, 0x1c, 0x07, 0x12, 0x37, 0x32, 0x29, 0x2d, 0x29, 0x25, 0x2B, 0x39, 0x00, 0x01, 0x03, 0x10}},
            {NGMCTR, {0x03, 0x1d, 0x07, 0x06, 0x2E, 0x2C, 0x29, 0x2D, 0x2E, 0x2E, 0x37, 0x3F, 0x00, 0x00, 0x02, 0x10}},
            {NORON, {}, 10},
            {DISPON, {}, 100}
        };

    };
}

#ifdef __cplusplus
}
#endif

#endif

#endif