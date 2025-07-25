#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *charts;
    lv_obj_t *obj0;
    lv_obj_t *obj1;
    lv_obj_t *obj2;
    lv_obj_t *temperature_setpoint;
    lv_obj_t *stop;
    lv_obj_t *start;
    lv_obj_t *keyboard;
    lv_obj_t *obj3;
    lv_obj_t *obj4;
    lv_obj_t *obj5;
    lv_obj_t *obj6;
    lv_obj_t *obj7;
    lv_obj_t *obj8;
    lv_obj_t *temperature;
    lv_obj_t *obj9;
    lv_obj_t *obj10;
    lv_obj_t *humidity;
    lv_obj_t *obj11;
    lv_obj_t *obj12;
    lv_obj_t *humidity_1;
    lv_obj_t *obj13;
    lv_obj_t *obj14;
    lv_obj_t *countdown;
    lv_obj_t *obj15;
    lv_obj_t *arc_timer;
    lv_obj_t *obj16;
    lv_obj_t *obj17;
    lv_obj_t *obj18;
    lv_obj_t *obj19;
    lv_obj_t *obj20;
    lv_obj_t *cont_1;
    lv_obj_t *chart_1;
    lv_obj_t *obj21;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_MAIN = 1,
    SCREEN_ID_CHARTS = 2,
};

void create_screen_main();
void tick_screen_main();

void create_screen_charts();
void tick_screen_charts();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/