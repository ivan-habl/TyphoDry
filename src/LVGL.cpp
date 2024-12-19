#include "global_var.h"

#if LV_USE_LOG != 0
void my_print(lv_log_level_t level, const char *buf) {
    LV_UNUSED(level);
    Serial.println(buf);
    Serial.flush();
}
#endif

void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t *)px_map, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}

void my_touchpad_read(lv_indev_t *indev, lv_indev_data_t *data) {
    if (digitalRead(INT_N_PIN) != LOW) {
        switch (ft6336u.read_touch1_event()) {
        case 0x00:
            data->state = LV_INDEV_STATE_RELEASED;
            break;
        case 0x02:
            data->point.x = TFT_HOR_RES - ft6336u.read_touch1_y();
            data->point.y = ft6336u.read_touch1_x();
            Serial.printf("x:%d, y:%d\n", data->point.x, data->point.y);
            data->state = LV_INDEV_STATE_PRESSED;
            break;
        }
    }
}

uint32_t my_tick(void) {
    return millis();
}