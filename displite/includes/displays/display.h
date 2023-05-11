#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <vector>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "lvgl/lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

namespace display {
    struct init_cmd {
        uint8_t cmd;
        std::vector<uint8_t> data;
        uint32_t ms_time{};
    };

    struct area {
        int32_t x1;
        int32_t y1;
        int32_t x2;
        int32_t y2;
    };

    class display {
        protected:
        const uint CHIP_SELECT;
        const uint DATA_OR_COMMAND;
        const uint SERIAL_DATA_OUT;
        const uint SIGNAL_CLOCK;
        const uint RESET;
        const uint BACKLIGHT;
        public:
        display(uint, uint, uint, uint, uint = 0, uint = 0);
        void backlight(bool);
        void reset();
        void chip_select(bool);
        void init(std::vector<init_cmd> &);
        virtual void command(const uint8_t &) = 0;
        virtual void data(const uint8_t &) = 0;
        virtual void data16(const uint16_t &) = 0;
        virtual void get_display_size(unsigned short &, unsigned short &) = 0;
        virtual void flush_pixels(const area &, lv_color_t *) = 0;
        /**
         * set display orientation. 
         * 
         * 0 - 0°
         * 1 - 90°
         * 2 - 180°
         * 3 - 270°
         *
         * @param[in] rotation: allowed values: 0, 1, 2, 3
         * @return void
        */
        virtual void rotate(short) = 0;

        static constexpr uint8_t NOP     {0x00}; // NOP
        static constexpr uint8_t SWRST {0x01}; //soft reset
        static constexpr uint8_t RDDID   {0x04}; // Read display identification 
        static constexpr uint8_t RDDST   {0x09}; // Read Display Status 
        static constexpr uint8_t SLPIN   {0x10}; // Sleep in
        static constexpr uint8_t SLPOUT  {0x11}; // sleep out
        static constexpr uint8_t PTLON   {0x12}; // Partial Mode ON 
        static constexpr uint8_t NORON   {0x13}; // Normal Display Mode ON 
        static constexpr uint8_t INVOFF  {0x20}; // Display Inversion OFF 
        static constexpr uint8_t INVON   {0x21}; // Display Inversion ON 
        static constexpr uint8_t DISPOFF {0x28}; // Display OFF
        static constexpr uint8_t DISPON  {0x29}; // Display ON
        static constexpr uint8_t CASET   {0x2A}; // Column Address Set
        static constexpr uint8_t RASET   {0x2B}; // Row Address Set 
        static constexpr uint8_t RAMWR   {0x2C}; // Memory Write
        static constexpr uint8_t RAMRD   {0x2E}; // Memory Read
        static constexpr uint8_t PTLAR   {0x30}; // Partial Area
        static constexpr uint8_t MADCTL  {0x36}; // Memory Access Control 
        static constexpr uint8_t ITFPXFT  {0x3A}; // Interface Pixel Format
        static constexpr uint8_t RDID1   {0xDA}; // Read ID1
        static constexpr uint8_t RDID2   {0xDB}; // Read ID2
        static constexpr uint8_t RDID3   {0xDC}; // Read ID3
        static constexpr uint8_t ITFCTR1 {0xB0}; // Interface Mode Control 
        static constexpr uint8_t FRMCTR1 {0xB1}; // Frame Rate Control（In Normal Mode/Full Colors）
        static constexpr uint8_t FRMCTR2 {0xB2}; // Frame Rate Control（In Idle Mode/8 colors）
        static constexpr uint8_t FRMCTR3 {0xB3}; // Frame Rate Control（In Partial Mode/Full colors）
        static constexpr uint8_t INVCTR  {0xB4}; // Display Inversion Control 
        static constexpr uint8_t DISSET5 {0xB6}; // Display Function Control 
        static constexpr uint8_t PWCTR1  {0xC0}; // Power Control 1 
        static constexpr uint8_t PWCTR2  {0xC1}; // Power Control 2
        static constexpr uint8_t PWCTR3  {0xC2}; // Power Control 3
        static constexpr uint8_t PWCTR4  {0xC3}; // Power Control 4 
        static constexpr uint8_t PWCTR5  {0xC4}; // Power Control 5
        static constexpr uint8_t VMCTR1  {0xC5}; // VCOM Control 1 
        static constexpr uint8_t RDID4   {0xD3}; // Read ID4 
        static constexpr uint8_t PGMCTR {0xE0}; // Positive Gamma Control
        static constexpr uint8_t NGMCTR {0xE1}; // Negative Gamma Control
        static constexpr uint8_t DGMCTR1 {0xE2}; // Digital Gamma Control 1 
        static constexpr uint8_t DGMCTR2 {0xE3}; // Digital Gamma Control 2
    };

    class spi: public display {
        protected:
        spi_inst_t *SPI_P;
        public:
        spi(spi_inst_t *, uint, uint, uint, uint, uint = 0, uint = 0);
        void command(const uint8_t &);
        void data(const uint8_t &);
    };

    class parallel {

    };

    display * get_available_dsp();
}

#ifdef __cplusplus
}
#endif

#endif