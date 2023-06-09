#include "interface.h"

LV_IMG_DECLARE(displite_400_img)

namespace interface {

    class splash_screen: public page {
        protected:
        const short hor_px;
        const short ver_px;
        public:
        splash_screen(short hor_px, short ver_px): page("SS"), hor_px{hor_px}, ver_px{ver_px}  {

        }

        void init() {
            if(parent_object != nullptr) {
                deinit();
            }

            parent_object = lv_obj_create(NULL);
            lv_obj_set_style_bg_color(parent_object, lv_color_hex(0x333333), LV_STATE_DEFAULT);
            lv_obj_t * img1 = lv_img_create(parent_object);
            lv_img_set_src(img1, &displite_400_img);
            if(hor_px < 400) {
                short reduction_factor{((hor_px - 20)*256)/400};
                lv_img_set_zoom(img1, reduction_factor);
            }
            lv_obj_align(img1, LV_ALIGN_CENTER, 0 , 0);
        }

        void refresh(uint8_t const *buffer, uint16_t bufsize) {
            
        }
    };

    page::page(std::string page_id) : PAGE_ID{page_id} {

    }

    void page::deinit() {
        lv_obj_del(parent_object);
        parent_object = nullptr;
    }

    lv_obj_t * page::get() {
        if(parent_object == nullptr) {
            init();
        }

        return parent_object;
    }

    gui::gui(short hor_res, short ver_res) {
        splash_screen *ss = new splash_screen(hor_res, ver_res);
        page_list[ss->PAGE_ID] = ss;
        show_splash_page();
    }

    void gui::show_splash_page() {
        std::string s{"SS"};
        set_active_page(s);
    }

    std::string gui::get_active_page() {
        return current_page->PAGE_ID;
    }

    void gui::insert_page(page *new_page) {
        if(new_page->PAGE_ID == "SS") return;
        page_list[new_page->PAGE_ID] = new_page;
    }

    bool gui::set_active_page(std::string &page_id) {
        if(page_id == current_page->PAGE_ID) return true;

        if(page_list.find(page_id) == page_list.end()) return false;

        page *next_page = page_list.at(page_id);

        lv_scr_load(next_page->get());

        if(current_page != nullptr) {
            current_page->deinit();
        }

        current_page = next_page;

        return true;
    }

    std::string gui::get_pages() {
        std::string result = "";
        for(auto i: page_list) {
            result += i.first + ";";
        }
        return result;
    }

    void gui::send_data(uint8_t const *buffer, uint16_t bufsize) {
        current_page->refresh(buffer, bufsize);
    }

}