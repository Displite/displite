#include "displays/display.h"

namespace display {
    display::display(uint chip_select, uint data_or_command, uint serial_out, 
    uint signal_clock, uint reset_p, uint backlight, unsigned short hor_px, unsigned short ver_px): CHIP_SELECT{chip_select}, DATA_OR_COMMAND{data_or_command}, 
    SERIAL_DATA_OUT{serial_out}, SIGNAL_CLOCK{signal_clock}, RESET{reset_p}, BACKLIGHT{backlight}, 
    HORIZONTAL_PX{hor_px}, VERTICAL_PX{ver_px} {
        gpio_init(CHIP_SELECT);
        gpio_set_dir(CHIP_SELECT, true);
        gpio_put(CHIP_SELECT, true);

        gpio_init(DATA_OR_COMMAND);
        gpio_set_dir(DATA_OR_COMMAND, true);

        if(RESET){ 
            gpio_init(RESET);
            gpio_set_dir(RESET, true);
        }
        if(BACKLIGHT) {
            gpio_init(BACKLIGHT);
            gpio_set_dir(BACKLIGHT, true);
        }
    }

    void display::backlight(bool enable) {
        if(BACKLIGHT) {
            gpio_put(BACKLIGHT, enable);
        }
    }

    void display::reset() {
        if(RESET) {
            gpio_put(RESET, true);
            sleep_ms(5);
            gpio_put(RESET, false);
            sleep_ms(20);
            gpio_put(RESET, true);
            sleep_ms(150);
        }
    }

    void display::chip_select(bool select) {
        gpio_put(CHIP_SELECT, !select);
    }

    void display::init(std::vector<init_cmd> &cdl) {
        for(init_cmd &ls : cdl) {
            command(ls.cmd);
            for(uint8_t &dt: ls.data) {
                data(dt);
            }
            if(ls.ms_time) {
                sleep_ms(ls.ms_time);
            }
        }
    }

    short display::get_rotation() {
        return current_rotation;
    }

    spi::spi(spi_inst_t *spi_p, uint chip_select, uint data_or_command, 
    uint serial_out, uint signal_clock, uint reset_p, uint backlight, unsigned short hor_px, unsigned short ver_px): SPI_P{spi_p}, 
    display(chip_select, data_or_command, serial_out, signal_clock, reset_p, backlight, hor_px, ver_px) {
        spi_init(SPI_P, 20000*1000);
        gpio_set_function(SERIAL_DATA_OUT, GPIO_FUNC_SPI);
        gpio_set_function(SIGNAL_CLOCK, GPIO_FUNC_SPI);
    }

    void spi::command(const uint8_t &d) {
        chip_select(true);
        gpio_put(DATA_OR_COMMAND, false);
        spi_write_blocking(this->SPI_P, &d, 1);
        chip_select(false);
    }

    void spi::data(const uint8_t &d) {
        chip_select(true);
        gpio_put(DATA_OR_COMMAND, true);
        spi_write_blocking(this->SPI_P, &d, 1);
        chip_select(false);
    }
} 