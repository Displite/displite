#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#include <map>
#include "lvgl/lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

namespace interface {
    class page {
        protected:
        const short hor_px;
        const short ver_px;
        lv_obj_t *parent_object = nullptr;
        public:
        const std::string PAGE_ID;
        page(std::string, short, short);
        virtual void init() = 0;
        void deinit();
        lv_obj_t * get();
        virtual void refresh(std::string &data) = 0;
    };

    class gui {
        private:
        std::map<std::string, page*> page_list;
        std::string current_page = "";
        protected:
        void insert_page(page *);
        public:
        gui(const lv_disp_t *);
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