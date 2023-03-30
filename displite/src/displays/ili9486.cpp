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
        const uint8_t x{static_cast<char>(d >> 8)};
        const uint8_t y{static_cast<char>(d & 0xFF)};
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
            std::swap(horizontal_px, vertical_px);
            break;
        case 2:
            command(MADCTL);
            data(0x40 | (1 << 3));
            break;
        case 3:
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

    void ili9486::flush_pixels(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
        if (area->x2 < 0 || area->y2 < 0 || area->x1 > (disp->hor_res - 1) || area->y1 > (disp->ver_res - 1)) {
            lv_disp_flush_ready(disp);
            return;
        }

        /* Truncate the area to the screen */
        int32_t act_x1 = area->x1 < 0 ? 0 : area->x1;
        int32_t act_y1 = area->y1 < 0 ? 0 : area->y1;
        int32_t act_x2 = area->x2 > disp->hor_res - 1 ? disp->hor_res - 1 : area->x2;
        int32_t act_y2 = area->y2 > disp->ver_res - 1 ? disp->ver_res - 1 : area->y2;

        command(display::display::CASET);
        data(act_x1 >> 8);
        data(act_x1 >> 0);
        data(act_x2 >> 8);
        data(act_x2 >> 0);

        command(display::display::RASET);
        data(act_y1 >> 8);
        data(act_y1 >> 0);
        data(act_y2 >> 8);
        data(act_y2 >> 0);

        command(display::display::RAMWR);

        uint16_t *ds = (uint16_t *)color_p;

        for (int32_t y{act_y1}; y <= act_y2; y++) {
            for (int32_t x{act_x1}; x <= act_x2; x++) {
                data16(*ds);
                ds++;
            }
        }

        lv_disp_flush_ready(disp);
    }
}

#endif