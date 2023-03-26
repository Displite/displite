#include "ui.h"

namespace ui {

    class splash_screen: public page {
        public:
        splash_screen();
        void init();
        void deinit();
        void refresh(std::string &data);
    };

    page::page(std::string page_id) : PAGE_ID{page_id} {

    }

    lv_obj_t * page::get() {
        if(parent_object == nullptr) {
            init();
        }

        return parent_object;
    }

    splash_screen::splash_screen(): page("SS") {

    }

    void splash_screen::refresh(std::string &data) {

    }

    void splash_screen::init() {
        if(parent_object != nullptr) {
            deinit();
        }

        parent_object = lv_obj_create(NULL);
        lv_obj_t * label1 = lv_label_create(parent_object);
        lv_obj_set_style_text_align(label1, LV_TEXT_ALIGN_CENTER, 0);
        lv_label_set_text(label1, "displite");
        lv_obj_align(label1, LV_ALIGN_CENTER, 0 , 0);
    }

    void splash_screen::deinit() {
        lv_obj_del(parent_object);
        parent_object = nullptr;
    }

    ui::ui(lv_disp_t * dsp) {
        splash_screen *ss = new splash_screen();
        page_list[ss->PAGE_ID] = ss;
        show_splash_page();
    }

    void ui::show_splash_page() {
        set_active_page("SS");
    }

    std::string ui::get_active_page() {
        return current_page;
    }

    void ui::insert_page(page *new_page) {
        if(new_page->PAGE_ID == "SS") return;
        page_list[new_page->PAGE_ID] = new_page;
    }

    bool ui::set_active_page(std::string page_id) {
        if(page_list.find(page_id) == page_list.end()) return false;

        if(page_id == current_page) return true;

        page *next_page = page_list.at(page_id);
        lv_scr_load(next_page->get());
        if(current_page != "") {
            next_page->deinit();
        }
        current_page = page_id;
        return true;
    }

    std::string ui::get_pages() {
        std::string result = "";
        for(auto i: page_list) {
            result += i.first + ";";
        }
        return result;
    }

    void ui::send_data(std::string &data) {
        page *curr_page = page_list.at(current_page);
        curr_page->refresh(data);
    }

}