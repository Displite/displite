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
	#error GUI_HEADER not defined
#else
	#include GUI_HEADER
#endif

#ifndef DISPLITE_INFORMATION
	#error Versions not defined
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
    tinyusb_process();
    return 0;
}

void tinyusb_process() {
	gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    tusb_init();

	dsp_drv = display::get_available_dsp();

	int buf_size{dsp_drv->HORIZONTAL_PX * dsp_drv->VERTICAL_PX / 10};
	static lv_disp_draw_buf_t draw_buf;
	static lv_color_t *buf1{new lv_color_t[buf_size]};
	static lv_color_t *buf2{new lv_color_t[buf_size]};
	lv_disp_draw_buf_init(&draw_buf, buf1, buf2, buf_size);

	static lv_disp_drv_t disp_drv; /*Descriptor of a display driver*/
	lv_disp_drv_init(&disp_drv);
	disp_drv.flush_cb = display_flush;
	disp_drv.draw_buf = &draw_buf;
	disp_drv.hor_res = dsp_drv->HORIZONTAL_PX;
	disp_drv.ver_res = dsp_drv->VERTICAL_PX;
	lv_disp_t *disp = lv_disp_drv_register(&disp_drv);

	#ifdef PREFERRED_THEME
		lv_theme_t * th = PREFERRED_THEME(disp);
		lv_disp_set_theme(disp, th);
	#endif

	gui_cls = new GUI_CLS(dsp_drv->HORIZONTAL_PX, dsp_drv->VERTICAL_PX);
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

		if (element.x2 < 0 || element.y2 < 0 || element.x1 > (disp_lv->hor_res - 1) || element.y1 > (disp_lv->ver_res - 1)) {
			lv_disp_flush_ready(disp_lv);
			continue;
    	}

		/* Truncate the area to the screen */
		element.x1 = element.x1 < 0 ? 0 : element.x1;
		element.y1 = element.y1 < 0 ? 0 : element.y1;
		element.x2 = element.x2 > disp_lv->hor_res - 1 ? disp_lv->hor_res - 1 : element.x2;
		element.y2 = element.y2 > disp_lv->ver_res - 1 ? disp_lv->ver_res - 1 : element.y2;

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
		case 'b': {
			bool new_state = true;
			if(bufsize > 1) {
				new_state = buffer[0] == '1';
			}
			dsp_drv->backlight(new_state);
			result = new_state ? "enabled": "disabled";
			tud_hid_report(0, result.c_str(), result.size());
			break;
		}
		default:
			tud_hid_report(0, "-1", 2);
	}
}

void display_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {

	display::area flush_area_lv {
		area->x1,
		area->y1,
		area->x2,
		area->y2
	};

	disp_lv = disp;
	color_p_lv = color_p;
	queue_add_blocking(&sample_fifo, &flush_area_lv);
}