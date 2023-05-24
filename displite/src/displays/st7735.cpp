#include "displays/st7735.h"

#ifdef USE_ST7735

namespace display {
    st7735::st7735(spi_inst_t *spi_p, uint chip_select, uint data_or_command, 
    uint serial_out, uint signal_clock, uint reset_p, uint backlight) : spi(spi_p, chip_select, data_or_command, serial_out, signal_clock, reset_p, backlight, 128, 160) {
        reset();
    }

    void st7735::reset() {
        spi::reset();
        init(cd_list);
        rotate(0);
        backlight(true);
    }

    void st7735::command(const uint8_t &d) {
        chip_select(true);
        gpio_put(DATA_OR_COMMAND, false);
        spi_write_blocking(this->SPI_P, &d, 1);
        chip_select(false);
    }

    void st7735::data(const uint8_t &d) {
        chip_select(true);
        gpio_put(DATA_OR_COMMAND, true);
        spi_write_blocking(this->SPI_P, &d, 1);
        chip_select(true);
    }

    void st7735::data16(const uint16_t &d) {
        chip_select(true);
        gpio_put(DATA_OR_COMMAND, true);
        const uint8_t x{d >> 8};
        const uint8_t y{d & 0xFF};
        spi_write_blocking(this->SPI_P, &x, 1);
        spi_write_blocking(this->SPI_P, &y, 1);
        chip_select(false);
    }

    void st7735::rotate(short rotation) {
        constexpr short my{1 << 7};
        constexpr short mx{1 << 6};
        constexpr short mv{1 << 5};

        switch(rotation) {
            case 0:
                command(MADCTL);
                data(mx | my | (1 << 3));
                break;
            case 1:
                command(MADCTL);
                data(my | mv | (1 << 3));
                break;
            case 2:
                command(MADCTL);
                data(1 << 3);
                break;
            case 3:
                command(MADCTL);
                data(mx | mv | (1 << 3));
                break;
            default:
                return;
        }

        current_rotation = rotation;

    }

    void st7735::flush_pixels(const area &area, lv_color_t *color_p) {
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