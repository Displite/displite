#include "hid.h"
#include "tusb.h"
#include "pico/stdlib.h"
#include "millis.h"
#include "pico/multicore.h"
#include "lvgl/lvgl.h"
#include "gui.h"
#include "displays/display.h"
#include "lvgl/demos/lv_demos.h"

static unsigned short blink_interval_ms = UsbState::not_mounted;
GUI_CLS *ui_class;
display::display *dsp_drv;

void tinyusb_process();
void lvgl_process();
void led_blinking_task();
void display_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);

int main() {
	lv_init();
    stdio_init_all();
    sleep_ms(250); // core1 hanging when there's no delay
    multicore_launch_core1(tinyusb_process);
    lvgl_process();
    return 0;
}

void tinyusb_process() {
	gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    tusb_init();

    while (true) {
		tud_task();
		led_blinking_task();
	}
}

void lvgl_process() {
	dsp_drv = new DSP_DRV_CLS(spi0, 17, 20, 19, 18, 21);
	unsigned short hor_res{};
	unsigned short ver_res{};
	dsp_drv->get_display_size(hor_res, ver_res);

	static lv_disp_draw_buf_t draw_buf;
	static lv_color_t *buf1{new lv_color_t[hor_res * ver_res / 10]};
	static lv_color_t *buf2{new lv_color_t[hor_res * ver_res / 10]};
	lv_disp_draw_buf_init(&draw_buf, buf1, buf2, hor_res * ver_res / 10);

	static lv_disp_drv_t disp_drv; /*Descriptor of a display driver*/
	lv_disp_drv_init(&disp_drv);
	disp_drv.flush_cb = display_flush;
	disp_drv.draw_buf = &draw_buf;
	disp_drv.hor_res = hor_res;
	disp_drv.ver_res = ver_res;
	lv_disp_t *disp = lv_disp_drv_register(&disp_drv);

	lv_demo_stress();

	while(true) {
        lv_task_handler();
		lv_timer_handler();
		sleep_ms(5);
    }

    delete dsp_drv;
    dsp_drv = nullptr;
}

void led_blinking_task() {
    static unsigned long start_ms = 0;
	static bool led_state = false;

	// Blink every interval ms
	if (millis() - start_ms < blink_interval_ms)
		return; // not enough time
	start_ms += blink_interval_ms;
    // printf("toggled\n");

	gpio_put(PICO_DEFAULT_LED_PIN, led_state);
	led_state = 1 - led_state; // toggle
}

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void) { blink_interval_ms = UsbState::mounted; }

// Invoked when device is unmounted
void tud_umount_cb(void) { blink_interval_ms = UsbState::not_mounted; }

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en) {
	(void)remote_wakeup_en;
	blink_interval_ms = UsbState::suspended;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void) { 
	blink_interval_ms = UsbState::mounted; 
}

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t itf, uint8_t report_id,
							   hid_report_type_t report_type, uint8_t *buffer,
							   uint16_t reqlen) {
	// TODO not Implemented
	(void)itf;
	(void)report_id;
	(void)report_type;
	(void)buffer;
	(void)reqlen;

	return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t itf, uint8_t report_id,
						   hid_report_type_t report_type, uint8_t const *buffer,
						   uint16_t bufsize) {
	(void)itf;
	(void)report_id;
	(void)report_type;

}

void display_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    dsp_drv->flush_pixels(disp, area, color_p);
}