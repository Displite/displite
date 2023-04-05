#include "usbstate.h"
#include "tusb.h"
#include "pico/stdlib.h"
#include "millis.h"
#include "pico/multicore.h"
#include "lvgl/lvgl.h"
#include "interface.h"
#include "displays/display.h"
#include "hardware/watchdog.h"
#include "pico/util/queue.h"
#include "pico/bootrom.h"

#ifndef GUI_HEADER
	#error GUI Header not defined
#else
	#include GUI_HEADER
#endif

#ifndef DISPLITE_INFORMATION
	#define DISPLITE_INFORMATION "undefined"
#endif

/*-------------
 * Globals
 *-----------*/

interface::gui *gui_cls;
display::display *dsp_drv;
queue_t sample_fifo;
lv_disp_drv_t *disp_lv; 
lv_color_t *color_p_lv;
static unsigned short blink_interval_ms = usbstate::not_mounted;

/*-------------
 * Forward declarations
 *-----------*/

void tinyusb_process();
void display_flush_process();
void display_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);

int main() {
    stdio_init_all();
	lv_init();
	queue_init(&sample_fifo, sizeof(display::area), 32);
    sleep_ms(250); // core1 hanging when there's no delay
    tinyusb_process();
    return 0;
}

void tinyusb_process() {
	gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    tusb_init();
	/*
	╔══════╦═════════════╦══════════════╦════════════╦══════════════╦═══════╦═══════════╗
	║ spi  ║ chip select ║ data/command ║ serial out ║ signal clock ║ reset ║ backlight ║
	╠══════╬═════════════╬══════════════╬════════════╬══════════════╬═══════╬═══════════╣
	║ spi0 ║     17      ║      20      ║     19     ║      18      ║  21   ║    22     ║
	╚══════╩═════════════╩══════════════╩════════════╩══════════════╩═══════╩═══════════╝
	*/
	dsp_drv = new DSP_DRV_CLS(spi0, 17, 20, 19, 18, 21, 22);
	unsigned short hor_res{};
	unsigned short ver_res{};
	dsp_drv->rotate(GUI_PREFERRED_ROTATION);
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

	#ifdef PREFERRED_THEME
		lv_theme_t * th = PREFERRED_THEME(disp);
		lv_disp_set_theme(disp, th);
	#endif

	gui_cls = new GUI_CLS(hor_res, ver_res);
	multicore_launch_core1(display_flush_process);
	static bool led_state = false;

	unsigned int prev_lvgl_ms, prev_led_ms, current_ms;
	prev_lvgl_ms = prev_led_ms = current_ms = millis();

    while (true) {
		tud_task();
		current_ms = millis();
		if(current_ms - prev_lvgl_ms > 5) {
			lv_task_handler();
			lv_timer_handler();
			prev_lvgl_ms = current_ms;
		}

		if(current_ms - prev_led_ms > blink_interval_ms) {
			gpio_put(PICO_DEFAULT_LED_PIN, led_state);
			led_state = 1 - led_state; // toggle
			prev_led_ms = current_ms;
		}
	}

	delete dsp_drv;
    dsp_drv = nullptr;
}

void display_flush_process() {
	
	while(true) {
        display::area element;
		queue_remove_blocking(&sample_fifo, &element);
		dsp_drv->flush_pixels(element, color_p_lv);
		lv_disp_flush_ready(disp_lv);
    }

}

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void) { blink_interval_ms = usbstate::mounted; }

// Invoked when device is unmounted
void tud_umount_cb(void) { 
	blink_interval_ms = usbstate::not_mounted; 
	gui_cls->show_splash_page();
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en) {
	(void)remote_wakeup_en;
	blink_interval_ms = usbstate::suspended;
	gui_cls->show_splash_page();
}

// Invoked when usb bus is resumed
void tud_resume_cb(void) { 
	blink_interval_ms = usbstate::mounted; 
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


	if((buffer == NULL) || (buffer[0] == '\0')) {
		return;
	}

	uint8_t command = buffer[0];
	buffer++;
	std::string result = "";
	switch(command) {
		case 'r': {
			tud_hid_report(0, "1", 1);
			watchdog_reboot(0, 0, 100);
			break;
		}
		case 'g': {
			result = gui_cls->get_pages();
			tud_hid_report(0, result.c_str(), result.size());
			break;
		}
		case 'c': {
			result = gui_cls->get_active_page();
			tud_hid_report(0, result.c_str(), result.size());
			break;
		}
		case 'p': {
			std::string data(buffer, buffer+ (bufsize - 1));
			result = gui_cls->set_active_page(data) ? "1" : "0";
			tud_hid_report(0, result.c_str(), result.size());
			break;
		}
		case 'd': {
			if(bufsize-1 < 1) {
				tud_hid_report(0, "0", 1);
			} else {
				tud_hid_report(0, "1", 1);
				gui_cls->send_data(buffer, bufsize-1);
			}
			break;
		}
		case 'i': { 
			tud_hid_report(0, DISPLITE_INFORMATION, sizeof(DISPLITE_INFORMATION));
			break;
		}
		case 'u': {
			reset_usb_boot(PICO_DEFAULT_LED_PIN, 0);
			break;
		}
		default:
			tud_hid_report(0, "0", 1);
	}
}

void display_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
	if (area->x2 < 0 || area->y2 < 0 || area->x1 > (disp->hor_res - 1) || area->y1 > (disp->ver_res - 1)) {
		lv_disp_flush_ready(disp);
		return;
    }

	/* Truncate the area to the screen */
	int32_t act_x1 = area->x1 < 0 ? 0 : area->x1;
	int32_t act_y1 = area->y1 < 0 ? 0 : area->y1;
	int32_t act_x2 = area->x2 > disp->hor_res - 1 ? disp->hor_res - 1 : area->x2;
	int32_t act_y2 = area->y2 > disp->ver_res - 1 ? disp->ver_res - 1 : area->y2;

	display::area flush_area_lv {
		act_x1,
		act_y1,
		act_x2,
		act_y2
	};

	disp_lv = disp;
	color_p_lv = color_p;
	queue_add_blocking(&sample_fifo, &flush_area_lv);
}