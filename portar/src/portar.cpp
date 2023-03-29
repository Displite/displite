#include "gui.h"

class perf: public interface::page {
    public:
    perf(short hor_px, short ver_px): page("PF", hor_px, ver_px) {

    }

    void init() {
        if(parent_object != nullptr) {
            deinit();
        }

        parent_object = lv_obj_create(NULL);
        lv_obj_t * cont1 = lv_obj_create(parent_object);
        lv_obj_set_pos(cont1, 0, 170);
        lv_obj_set_size(cont1, 160, 75);
        lv_obj_t * lbl1 = lv_label_create(cont1);
        lv_label_set_text(lbl1, "CPU");
        lv_obj_align(lbl1, 9, 0, 0);
        lv_obj_t * cont2 = lv_obj_create(parent_object);
        lv_obj_set_pos(cont2, 160, 170);
        lv_obj_set_size(cont2, 160, 75);
        lv_obj_t * lbl2 = lv_label_create(cont2);
        lv_label_set_text(lbl2, "MEM");
        lv_obj_align(lbl2, 9, 0, 0);
        lv_obj_t * cont3 = lv_obj_create(parent_object);
        lv_obj_set_pos(cont3, 320, 170);
        lv_obj_set_size(cont3, 160, 75);
        lv_obj_t * lbl3 = lv_label_create(cont3);
        lv_label_set_text(lbl3, "DISK");
        lv_obj_align(lbl3, 9, 0, 0);

    }

    void refresh(std::string &data) {

    }
};

portar::portar(lv_disp_t *dsp): interface::gui(dsp) {
    interface::page *pf_page = new perf(dsp->driver->hor_res, dsp->driver->ver_res);
    this->insert_page(pf_page);
}

