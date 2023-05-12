#ifndef _{{ cookiecutter.__project_slug.upper() }}_H_
#define _{{ cookiecutter.__project_slug.upper() }}_H_

#include "interface.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PREFERRED_ROTATION 1
#define PREFERRED_THEME lv_theme_basic_init

class {{cookiecutter.__project_slug}}: public interface::gui {
    public:
    {{cookiecutter.__project_slug}}(short, short);
};




// Do not modify
#define GUI_CLS {{cookiecutter.__project_slug}}

#ifdef __cplusplus
}
#endif

#endif