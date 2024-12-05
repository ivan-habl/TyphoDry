#include "FT6336U.h"
#include "ui.h"
#include "vars.h"
#include <Adafruit_SHT4x.h>
#include <Arduino.h>
#include <MQTT.h>
#include <QuickPID.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <lvgl.h>

#define I2C_SDA 21
#define I2C_SCL 22
#define RST_N_PIN 32
#define INT_N_PIN 33

#define TFT_HOR_RES 480
#define TFT_VER_RES 320
#define TFT_ROTATION LV_DISPLAY_ROTATION_0

unsigned long previousMillisLVGLwork = 0;
const long intervalLVGLwork = 5;
unsigned long previousMillisPIDprint = 0;
const long intervalPIDprint = 500;
unsigned long previousMillisMQTT = 0;
const long intervalMQTT = 10;

const char ssid[] = "I love pussy";
const char pass[] = "*Cbufhtnf#";

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

void connect() {
    Serial.print("checking wifi...");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(1000);
    }

    Serial.print("\nconnecting...");
    while (!client.connect("arduino", "public", "public")) {
        Serial.print(".");
        delay(1000);
    }

    Serial.println("\nconnected!");

    client.subscribe("Drier/Transmit/SHT40/Temperature");
    client.subscribe("Drier/Transmit/SHT40/Humidity");
    client.subscribe("Drier/Transmit/Signals/Discrete/PowerSt");
    client.subscribe("Drier/Transmit/Signals/Discrete/PIDSt");
    // client.unsubscribe("/hello");
}

void messageReceived(String &topic, String &payload) {
    Serial.println("incoming: " + topic + " - " + payload);

    // Note: Do not use the client in the callback to publish, subscribe or
    // unsubscribe as it may cause deadlocks when other things arrive while
    // sending and receiving acknowledgments. Instead, change a global variable,
    // or push to a queue and handle it in the loop after calling `client.loop()`.
}

/*LVGL draw into this buffer, 1/10 screen size usually works well. The size is in bytes*/
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 20 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE / 4];

TFT_eSPI tft = TFT_eSPI(TFT_HOR_RES, TFT_VER_RES); /* TFT instance */
FT6336U ft6336u(I2C_SDA, I2C_SCL, RST_N_PIN, INT_N_PIN);

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

static uint32_t my_tick(void) {
    return millis();
}

const byte FAN_PWM_PIN = 12;
const byte relayPin = 13;

const unsigned long windowSize = 4000;
const byte debounce = 50;
float Input, Output, Setpoint = 50, Kp = 111.609, Ki = 0.100, Kd = 0.399, Duty = 45; // duty = 45;

unsigned long windowStartTime, nextSwitchTime;
boolean relayStatus = false;

QuickPID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd,
               QuickPID::pMode::pOnError,
               QuickPID::dMode::dOnMeas,
               QuickPID::iAwMode::iAwClamp,
               QuickPID::Action::direct);

Adafruit_SHT4x sht4 = Adafruit_SHT4x();

void checkUserInput() {
    if (Serial.available()) {
        char input = Serial.read();

        switch (input) {
        case 's':
            Serial.println("Введите целевую температуру: ");
            while (!Serial.available()) {
            }
            Setpoint = Serial.parseFloat();
            Serial.print("Новая целевая температура: ");
            Serial.println(Setpoint);
            break;

        case 'f':
            Serial.println("Введите коэффициент заполнения: ");
            while (!Serial.available()) {
            }
            Duty = Serial.parseInt();
            Serial.print("Новый коэффициент заполнения: ");
            Serial.println(Duty);
            break;

        default:
            Serial.println("Неизвестная команда!");
            break;
        }
    }
}

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, pass);

    client.begin("mserver.lan", net);
    client.onMessage(messageReceived);

    connect();

    sht4.begin(&Wire);

    sht4.setPrecision(SHT4X_HIGH_PRECISION);
    sht4.setHeater(SHT4X_NO_HEATER);

    pinMode(relayPin, OUTPUT);

    lv_init();
    lv_tick_set_cb(my_tick);

    ft6336u.begin();

#if LV_USE_LOG != 0
    lv_log_register_print_cb(my_print); /* register print function for debugging */
#endif

    tft.begin();        /* TFT init */
    tft.setRotation(3); /* Landscape orientation, flipped */

    lv_display_t *disp;

    disp = lv_display_create(TFT_HOR_RES, TFT_VER_RES);
    lv_display_set_flush_cb(disp, my_disp_flush);
    lv_display_set_buffers(disp, draw_buf, NULL, sizeof(draw_buf), LV_DISPLAY_RENDER_MODE_PARTIAL);

    lv_indev_t *indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, my_touchpad_read);

    ui_init();

    myPID.SetOutputLimits(0, windowSize);
    myPID.SetSampleTimeUs(windowSize * 1000);
    myPID.SetMode(QuickPID::Control::automatic);
}

void loop() {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillisMQTT >= intervalMQTT) {

        client.loop();

        if (!client.connected()) {
            connect();
        }
    }

    sensors_event_t humidity, temp;
    sht4.getEvent(&humidity, &temp);

    analogWrite(FAN_PWM_PIN, Duty);

    if (currentMillis - previousMillisLVGLwork >= intervalLVGLwork) {
        previousMillisLVGLwork = currentMillis;

        set_var_temperature(temp.temperature);
        set_var_humidity(humidity.relative_humidity);
        Setpoint = get_var_temperature_setpoint();

        lv_timer_handler(); /* let the GUI do its work */
        ui_tick();
    }

    checkUserInput();

    unsigned long msNow = millis();
    Input = temp.temperature;

    if (get_var_drier_state() == 1 && myPID.Compute())
        windowStartTime = msNow;

    if (!relayStatus && Output > (msNow - windowStartTime)) {
        if (msNow > nextSwitchTime) {
            nextSwitchTime = msNow + debounce;
            relayStatus = true;
            digitalWrite(relayPin, HIGH);
            set_var_solid_relay_st(255);
            client.publish("Drier/Transmit/Discrete/PIDSt", "1");
        }
    } else if (relayStatus && Output < (msNow - windowStartTime)) {
        if (msNow > nextSwitchTime) {
            nextSwitchTime = msNow + debounce;
            relayStatus = false;
            digitalWrite(relayPin, LOW);
            set_var_solid_relay_st(0);
            client.publish("Drier/Transmit/Discrete/PIDSt", "0");
        }
    }
    char temperatureStr[10];
    char humidityStr[10];
    char powerStSrt[2];

    dtostrf(temp.temperature, 2, 2, temperatureStr);
    dtostrf(humidity.relative_humidity, 2, 2, humidityStr);
    dtostrf(get_var_drier_state(), 1, 0, powerStSrt);

    if (millis() - lastMillis > 1000) {
        lastMillis = millis();
        client.publish("Drier/Transmit/SHT40/Temperature", temperatureStr);
        client.publish("Drier/Transmit/SHT40/Humidity", humidityStr);
        client.publish("Drier/Transmit/Discrete/PowerSt", powerStSrt);
    }

    if (currentMillis - previousMillisPIDprint >= intervalPIDprint) {
        previousMillisPIDprint = currentMillis;
        Serial.print(">Humidity: ");
        Serial.println(humidity.relative_humidity);
        Serial.print(">Input/Temperature: ");
        Serial.println(Input);
        Serial.print(">Output: ");
        Serial.println(Output);
        Serial.print(">Heater: ");
        Serial.println(digitalRead(relayPin) ? "1" : "0");
        Serial.print(">Kpt: ");
        Serial.println(myPID.GetPterm());
        Serial.print(">Kit: ");
        Serial.println(myPID.GetIterm());
        Serial.print(">Kdt: ");
        Serial.println(myPID.GetDterm());
    }
}