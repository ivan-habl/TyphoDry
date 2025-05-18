#include "global_var.h"

#if LV_USE_LOG != 0
void my_print(lv_log_level_t level, const char *buf) {
    LV_UNUSED(level);
    Serial.println(buf);
    Serial.flush();
}
#endif

void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *data) {
    uint32_t w = lv_area_get_width(area);
    uint32_t h = lv_area_get_height(area);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushPixels((uint16_t *)data, w * h);
    tft.endWrite();

    lv_display_flush_ready(disp);
}

void my_touchpad_read(lv_indev_t *indev, lv_indev_data_t *data) {
    // Проверяем наличие касания
    if (digitalRead(INT_N_PIN) == LOW) {
        // Нет касания
        data->state = LV_INDEV_STATE_RELEASED;
        data->point.x = 0;
        data->point.y = 0;
        return;
    }

    uint8_t event = ft6336u.read_touch1_event();
    if (event == 0x02) { // Touch/Stream
        data->point.x = TFT_HOR_RES - ft6336u.read_touch1_x();
        data->point.y = TFT_VER_RES - ft6336u.read_touch1_y();
        data->state = LV_INDEV_STATE_PRESSED;
        // Serial.printf("x:%d, y:%d\n", data->point.x, data->point.y);
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
        data->point.x = 0;
        data->point.y = 0;
    }
}

uint32_t my_tick_get_cb() {
    return millis();
}