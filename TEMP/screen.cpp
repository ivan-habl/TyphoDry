#include <string.h>

#include "actions.h"
#include "fonts.h"
#include "images.h"
#include "screens.h"
#include "styles.h"
#include "ui.h"
#include "vars.h"
#include <time.h>

#include <string.h>

#define CHART_POINTS 7
static lv_coord_t humidity_data[CHART_POINTS];
static uint16_t humidity_data_index = 0;

static time_t last_update_time = 0; // Время последнего добавления точки
static int point_add_interval = 5;  // Интервал добавления новых точек (в секундах)

lv_obj_t *scale_bottom;

objects_t objects;
lv_obj_t *tick_value_change_obj;

static void event_handler_cb_main_obj0(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    void *flowState = lv_event_get_user_data(e);

    if (event == LV_EVENT_PRESSED) {
        e->user_data = (void *)0;
        flowPropagateValueLVGLEvent(flowState, 2, 0, e);
    }
}

static void event_handler_cb_main_temperature_setpoint(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    void *flowState = lv_event_get_user_data(e);

    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            const char *value = lv_textarea_get_text(ta);
            if (tick_value_change_obj != ta) {
                assignStringProperty(flowState, 5, 3, value, "Failed to assign Text in Textarea widget");
            }
        }
    }

    if (event == LV_EVENT_PRESSING) {
        e->user_data = (void *)0;
        flowPropagateValueLVGLEvent(flowState, 5, 0, e);
    }
    if (event == LV_EVENT_DEFOCUSED) {
        e->user_data = (void *)0;
        flowPropagateValueLVGLEvent(flowState, 5, 1, e);
    }
}

static void event_handler_cb_main_keyboard(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    void *flowState = lv_event_get_user_data(e);

    if (event == LV_EVENT_READY) {
        e->user_data = (void *)0;
        flowPropagateValueLVGLEvent(flowState, 15, 0, e);
    }
    if (event == LV_EVENT_CANCEL) {
        e->user_data = (void *)0;
        flowPropagateValueLVGLEvent(flowState, 15, 1, e);
    }
}

static void event_handler_cb_charts_obj1(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    void *flowState = lv_event_get_user_data(e);

    if (event == LV_EVENT_PRESSED) {
        e->user_data = (void *)0;
        flowPropagateValueLVGLEvent(flowState, 4, 0, e);
    }
}

void create_screen_main() {
    void *flowState = getFlowState(0, 0);
    lv_obj_t *obj = lv_obj_create(0);
    objects.main = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 480, 320);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffa4cbcd), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 387, 278);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "MAIN");
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_30, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.obj0 = obj;
            lv_obj_set_pos(obj, 3, 267);
            lv_obj_set_size(obj, 100, 50);
            lv_obj_add_event_cb(obj, event_handler_cb_main_obj0, LV_EVENT_ALL, flowState);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Charts");
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 24, 25);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "УСТАВКА \nТЕМПЕРАТУРЫ");
            lv_obj_set_style_text_font(obj, &ui_font_roboto_24, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // Temperature setpoint
            lv_obj_t *obj = lv_textarea_create(parent_obj);
            objects.temperature_setpoint = obj;
            lv_obj_set_pos(obj, 315, 22);
            lv_obj_set_size(obj, 61, LV_SIZE_CONTENT);
            lv_textarea_set_max_length(obj, 2);
            lv_textarea_set_one_line(obj, true);
            lv_textarea_set_password_mode(obj, false);
            lv_obj_add_event_cb(obj, event_handler_cb_main_temperature_setpoint, LV_EVENT_ALL, flowState);
            lv_obj_set_style_text_font(obj, &ui_font_roboto_30, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xff1341b6), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff3d94e5), LV_PART_MAIN | LV_STATE_FOCUSED);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 387, 53);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "°C");
            lv_obj_set_style_text_font(obj, &ui_font_roboto_24, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.obj2 = obj;
            lv_obj_set_pos(obj, 24, 172);
            lv_obj_set_size(obj, 363, LV_SIZE_CONTENT);
            lv_obj_set_style_pad_top(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff14f468), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xffe0e0e0), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, -10, 6);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "ОТНОСИТЕЛЬНАЯ\nВЛАЖНОСТЬ");
                    lv_obj_set_style_text_font(obj, &ui_font_roboto_24, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // Humidity
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.humidity = obj;
                    lv_obj_set_pos(obj, 213, 9);
                    lv_obj_set_size(obj, 93, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "");
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_roboto_45, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 306, 22);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "%");
                    lv_obj_set_style_text_font(obj, &ui_font_roboto_24, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.obj3 = obj;
            lv_obj_set_pos(obj, 24, 104);
            lv_obj_set_size(obj, 363, LV_SIZE_CONTENT);
            lv_obj_set_style_pad_top(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff14f468), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xffe0e0e0), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, -9, 14);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "ТЕМПЕРАТУРА");
                    lv_obj_set_style_text_font(obj, &ui_font_roboto_24, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // Temperature
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.temperature = obj;
                    lv_obj_set_pos(obj, 213, 2);
                    lv_obj_set_size(obj, 93, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "");
                    lv_obj_set_style_text_font(obj, &ui_font_roboto_45, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 306, 15);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "°C");
                    lv_obj_set_style_text_font(obj, &ui_font_roboto_24, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
        {
            // Keyboard
            lv_obj_t *obj = lv_keyboard_create(parent_obj);
            objects.keyboard = obj;
            lv_obj_set_pos(obj, 216, 83);
            lv_obj_set_size(obj, 255, 228);
            lv_keyboard_set_mode(obj, LV_KEYBOARD_MODE_NUMBER);
            lv_obj_add_event_cb(obj, event_handler_cb_main_keyboard, LV_EVENT_ALL, flowState);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }
    lv_keyboard_set_textarea(objects.keyboard, objects.temperature_setpoint);
}

void tick_screen_main() {
    void *flowState = getFlowState(0, 0);
    {
        const char *new_val = evalTextProperty(flowState, 5, 3, "Failed to evaluate Text in Textarea widget");
        const char *cur_val = lv_textarea_get_text(objects.temperature_setpoint);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.temperature_setpoint;
            lv_textarea_set_text(objects.temperature_setpoint, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = evalTextProperty(flowState, 9, 3, "Failed to evaluate Text in Label widget");
        const char *cur_val = lv_label_get_text(objects.humidity);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.humidity;
            lv_label_set_text(objects.humidity, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = evalTextProperty(flowState, 13, 3, "Failed to evaluate Text in Label widget");
        const char *cur_val = lv_label_get_text(objects.temperature);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.temperature;
            lv_label_set_text(objects.temperature, new_val);
            tick_value_change_obj = NULL;
        }
    }
}

void create_screen_charts() {
    void *flowState = getFlowState(0, 1);
    lv_obj_t *obj = lv_obj_create(0);
    objects.charts = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 480, 320);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffd6aee6), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // Родительский контейнер
            lv_obj_t *cont_1 = lv_obj_create(parent_obj);
            lv_obj_remove_style_all(cont_1);
            lv_obj_set_size(cont_1, 480, 320); // Размер контейнера
            lv_obj_center(cont_1);
            lv_obj_set_layout(cont_1, LV_LAYOUT_GRID);

            // Настройка сетки контейнера
            static lv_coord_t col_dsc[] = {30, LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST}; // 30px для вертикальной шкалы, остальное для графика
            static lv_coord_t row_dsc[] = {LV_GRID_FR(1), 30, LV_GRID_TEMPLATE_LAST}; // Остаток высоты для графика, 30px для горизонтальной шкалы
            lv_obj_set_grid_dsc_array(cont_1, col_dsc, row_dsc);

            // Вертикальная шкала (Y-ось)
            lv_obj_t *scale_left = lv_scale_create(cont_1);
            lv_scale_set_mode(scale_left, LV_SCALE_MODE_VERTICAL_LEFT);
            lv_obj_set_size(scale_left, 30, LV_SIZE_CONTENT); // Проверить, что высота соответствует содержимому
            lv_scale_set_total_tick_count(scale_left, 11);
            lv_scale_set_major_tick_every(scale_left, 2);
            static const char *values_y[] = {"0", "20", "40", "60", "80", "100", NULL};
            lv_scale_set_text_src(scale_left, values_y);
            lv_obj_set_grid_cell(scale_left, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);

            // График
            objects.chart_1 = lv_chart_create(cont_1);
            lv_obj_set_size(objects.chart_1, LV_PCT(100), LV_PCT(100)); // Использует всю доступную площадь
            lv_chart_set_type(objects.chart_1, LV_CHART_TYPE_LINE);
            lv_chart_set_range(objects.chart_1, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
            lv_obj_set_grid_cell(objects.chart_1, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
            lv_chart_set_div_line_count(objects.chart_1, 6, 7);                                 // 6 горизонтальных, 10 вертикальных линий
            lv_obj_set_style_line_color(objects.chart_1, lv_color_hex(0xe8e8e8), LV_PART_MAIN); // Цвет линий
            lv_obj_set_style_line_width(objects.chart_1, 2, LV_PART_MAIN);                      // Толщина линий
            lv_obj_set_style_border_width(objects.chart_1, 0, LV_PART_MAIN);                    // Устанавливаем толщину рамки в 2 пикселя
            lv_obj_set_style_radius(objects.chart_1, 0, LV_PART_MAIN);                          // Скругление углов на 5 пикселей.
            lv_obj_set_style_pad_all(objects.chart_1, 0, LV_PART_MAIN);                         // Отступы от краёв
            lv_chart_set_point_count(objects.chart_1, CHART_POINTS);                            //  NEW_POINT_COUNT - нужное количество точек
            objects.ser_humidity = lv_chart_add_series(objects.chart_1, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);

            // Горизонтальная шкала (X-ось)
            scale_bottom = lv_scale_create(cont_1);
            lv_scale_set_mode(scale_bottom, LV_SCALE_MODE_HORIZONTAL_BOTTOM);
            lv_obj_set_size(scale_bottom, LV_SIZE_CONTENT, 30); // Проверить, что ширина соответствует содержимому
            lv_scale_set_total_tick_count(scale_bottom, 7);
            lv_scale_set_major_tick_every(scale_bottom, 1);
            static const char *minutes[] = {"0", "10", "20", "30", "40", "50", "60", NULL};
            lv_scale_set_text_src(scale_bottom, minutes);
            lv_obj_set_grid_cell(scale_bottom, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_START, 1, 1);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj4 = obj;
            lv_obj_set_pos(obj, 177, 0);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "CHARTS");
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_30, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff0bb91d), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.obj1 = obj;
            lv_obj_set_pos(obj, 375, 5);
            lv_obj_set_size(obj, 100, 50);
            lv_obj_add_event_cb(obj, event_handler_cb_charts_obj1, LV_EVENT_ALL, flowState);
            lv_obj_set_style_bg_opa(obj, 128, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Main");
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
    }
}

// Основная функция обновления графика
void tick_screen_charts() {
    void *flowState = getFlowState(0, 1);

    // Функция для задания интервала добавления точек
    void set_point_add_interval(int interval) {
        point_add_interval = interval;
    }

    // Получаем текущее время
    time_t current_timestamp = time(NULL);

    // Проверяем, прошёл ли заданный интервал
    if (current_timestamp - last_update_time < point_add_interval) {
        return; // Если интервал не прошёл, выходим из функции
    }
    last_update_time = current_timestamp; // Обновляем время последнего добавления точки

    // Обновление данных влажности
    lv_chart_series_t *series = objects.ser_humidity;
    humidity_data[humidity_data_index] = get_var_humidity();
    humidity_data_index = (humidity_data_index + 1) % CHART_POINTS;

    // Заполнение точек графика
    for (int i = 0; i < CHART_POINTS; i++) {
        series->y_points[i] = humidity_data[(humidity_data_index + CHART_POINTS - 1 - i) % CHART_POINTS];
    }

    lv_chart_refresh(objects.chart_1);

    // Обновление меток времени на оси X
    static char time_labels[7][10];
    static const char *dynamic_minutes[7] = {
        time_labels[0],
        time_labels[1],
        time_labels[2],
        time_labels[3],
        time_labels[4],
        time_labels[5],
        NULL};

    static int total_seconds = 0;
    total_seconds += point_add_interval; // Увеличиваем время на заданный интервал

    if (total_seconds % 60 == 0) {
        int current_minute = (total_seconds / 60) % 10;

        // Обновление меток времени
        for (int i = 0; i < 5; i++) {
            int minute_value = (current_minute - (5 - i) + 10) % 10;
            snprintf(time_labels[i], sizeof(time_labels[i]), "%d", minute_value);
        }

        snprintf(time_labels[5], sizeof(time_labels[5]), "%d", current_minute);

        lv_scale_set_text_src(scale_bottom, dynamic_minutes);
    }
}

extern void add_style(lv_obj_t *obj, int32_t styleIndex);
extern void remove_style(lv_obj_t *obj, int32_t styleIndex);

static const char *screen_names[] = {"Main", "Charts"};
static const char *object_names[] = {"main", "charts", "obj0", "temperature_setpoint", "keyboard", "obj1", "obj2", "humidity", "obj3", "temperature", "cont_1", "chart_1", "obj4"};
static const char *style_names[] = {"123"};

void create_screens() {
    eez_flow_init_styles(add_style, remove_style);

    eez_flow_init_screen_names(screen_names, sizeof(screen_names) / sizeof(const char *));
    eez_flow_init_object_names(object_names, sizeof(object_names) / sizeof(const char *));
    eez_flow_init_style_names(style_names, sizeof(style_names) / sizeof(const char *));

    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);

    create_screen_main();
    create_screen_charts();
}

typedef void (*tick_screen_func_t)();

tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_main,
    tick_screen_charts,
};

void tick_screen(int screen_index) {
    tick_screen_funcs[screen_index]();
}
