#include "displays/ili9486.h"

#ifdef USE_ILI9486

namespace display {
    ili9486::ili9486(spi_inst_t *spi_p, uint chip_select, uint data_or_command, 
    uint serial_out, uint signal_clock, uint reset_p, uint backlight) : spi(spi_p, chip_select, data_or_command, serial_out, signal_clock, reset_p, backlight, 320, 480) {
        reset();
    }

    void ili9486::reset() {
        spi::reset();
        init(cd_list);
        rotate(0);
    }

    void ili9486::command(const uint8_t &d) {
        chip_select(true);
        gpio_put(DATA_OR_COMMAND, false);
        spi_write_blocking(this->SPI_P, 0, 1);
        spi_write_blocking(this->SPI_P, &d, 1);
        chip_select(false);
    }

    void ili9486::data(const uint8_t &d) {
        chip_select(true);
        gpio_put(DATA_OR_COMMAND, true);
        spi_write_blocking(this->SPI_P, 0, 1);
        spi_write_blocking(this->SPI_P, &d, 1);
        chip_select(false);
    }

    void ili9486::data16(const uint16_t &d) {
        chip_select(true);
        gpio_put(DATA_OR_COMMAND, true);
        const uint8_t x{d >> 8};
        const uint8_t y{d & 0xFF};
        spi_write_blocking(this->SPI_P, &x, 1);
        spi_write_blocking(this->SPI_P, &y, 1);
        chip_select(false);
    }

    void ili9486::rotate(short rotation) {
        switch (rotation) {
        case 0:
            command(MADCTL);
            data(0x80 | (1 << 3));
            break;
        case 1:
            command(MADCTL);
            data(0x20 | (1 << 3));
            break;
        case 2:
            command(MADCTL);
            data(0x40 | (1 << 3));
            break;
        case 3:
            command(MADCTL);
            data(0xE0 | (1 << 3));
            break;
        default:
            return;
        }

        current_rotation = rotation;
    }

    void ili9486::flush_pixels(const area &area, lv_color_t *color_p) {
        
        command(display::display::CASET);
        data(area.x1 >> 8);
        data(area.x1 >> 0);
        data(area.x2 >> 8);
        data(area.x2 >> 0);

        command(display::display::RASET);
        data(area.y1 >> 8);
        data(area.y1 >> 0);
        data(area.y2 >> 8);
        data(area.y2 >> 0);

        command(display::display::RAMWR);

        uint16_t *ds = (uint16_t *)color_p;

        for (int32_t y{area.y1}; y <= area.y2; y++) {
            for (int32_t x{area.x1}; x <= area.x2; x++) {
                data16(*ds);
                ds++;
            }
        }
    }
}

#endif