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
        lv_obj_t *parent_object = nullptr;
        public:
        const std::string PAGE_ID;
        page(std::string);
        virtual void init() = 0;
        void deinit();
        lv_obj_t * get();
        virtual void refresh(const uint8_t *, uint16_t) = 0;
    };

    class gui {
        private:
        std::map<std::string, page*> page_list;
        page *current_page = nullptr;
        protected:
        virtual void insert_page(page *) final;
        public:
        gui(short, short);
        virtual std::string get_active_page() final;
        virtual bool set_active_page(std::string) final;
        virtual std::string get_pages() final;
        virtual void send_data(const uint8_t *, uint16_t) final;
        virtual void show_splash_page() final;
    };
}

#ifdef __cplusplus
}
#endif

#endif