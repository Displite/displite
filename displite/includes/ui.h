#ifndef _UI_H_
#define _UI_H_

#include <map>
#include "lvgl/lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

namespace ui {
    class page {
        protected:
        lv_obj_t *parent_object = nullptr;
        public:
        const std::string PAGE_ID;
        page(std::string);
        virtual void init() = 0;
        virtual void deinit() = 0;
        lv_obj_t * get();
        virtual void refresh(std::string &data) = 0;
    };

    class ui {
        private:
        std::map<std::string, page*> page_list;
        std::string current_page = "";
        protected:
        void insert_page(page *);
        public:
        ui(lv_disp_t *);
        std::string get_active_page();
        bool set_active_page(std::string);
        std::string get_pages();
        void send_data(std::string &);
        virtual void show_splash_page() final;
    };
}

#ifdef __cplusplus
}
#endif

#endif