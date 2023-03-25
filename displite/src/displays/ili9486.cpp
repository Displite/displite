#include "displays/ili9486.h"

#ifdef USE_ILI9486

namespace display {
    ili9486::ili9486(spi_inst_t *spi_p, uint chip_select, uint data_or_command, 
    uint serial_out, uint signal_clock, uint reset_p, uint backlight) : spi(spi_p, chip_select, data_or_command, serial_out, signal_clock, reset_p, backlight) {
        reset();
    }

    void ili9486::reset() {
        spi::reset();
        init(cd_list);
        rotate(1);
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
        case 1:
            command(MADCTL);
            data(0x80 | (1 << 3));
            break;
        case 2:
            command(MADCTL);
            data(0x20 | (1 << 3));
            std::swap(horizontal_px, vertical_px);
            break;
        case 3:
            command(MADCTL);
            data(0x40 | (1 << 3));
            break;
        case 4:
            command(MADCTL);
            data(0xE0 | (1 << 3));
            std::swap(horizontal_px, vertical_px);
            break;
        default:
            break;
        }
    }

    void ili9486::get_display_size(unsigned short &h_size, unsigned short &v_size) {
        h_size = horizontal_px;
        v_size = vertical_px;
    }
}

#endif