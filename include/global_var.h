#include "FT6336U.h"
#include "ui.h"
#include "vars.h"
#include <Adafruit_SHT4x.h>
#include <Arduino.h>
#include <DallasTemperature.h>
#include <MQTT.h>
#include <OneWire.h>
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

/*LVGL draw into this buffer, 1/10 screen size usually works well. The size is in bytes*/
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 20 * (LV_COLOR_DEPTH / 8))

#define ONE_WIRE_BUS 26 // GPIO для подключения шины данных DS18B20

extern uint32_t draw_buf[DRAW_BUF_SIZE / 4];

const char ssid[] = "I love pussy";
const char pass[] = "*Cbufhtnf#";

extern unsigned long previousMillisLVGLwork;
const unsigned long intervalLVGLwork = 10;

extern unsigned long previousMillisPIDprint;
const unsigned long intervalPIDprint = 500;

extern unsigned long previousMillisMQTT;
const unsigned long intervalMQTT = 10;

// Флаги состояния
extern bool isWiFiConnected;
extern bool isClientConnected;

// Таймеры для периодических проверок
extern unsigned long lastWiFiCheck;
extern unsigned long lastClientCheck;

// Интервалы проверки
const unsigned long wifiCheckInterval = 1000;
const unsigned long clientCheckInterval = 1000;

extern unsigned long lastMillis;

const byte FAN_PWM_PIN = 12;
const byte relayPin = 13;
const byte Recylcuration_PWM_PIN = 27;

const unsigned long windowSize = 2000;
extern unsigned long windowStartTime, nextSwitchTime;
extern boolean relayStatus;

extern float Input, Output, Setpoint, Kp, Ki, Kd;
extern int dutyFan, dutyRecylcuration;

const int sensorCount = 4; // Количество датчиков

extern QuickPID myPID;

extern TFT_eSPI tft;
extern FT6336U ft6336u;

extern Adafruit_SHT4x sht4;

extern WiFiClient net;
extern MQTTClient client;
extern OneWire oneWire;
extern DallasTemperature sensors;

extern DeviceAddress sensorAddresses[4];

void connect();
void messageReceived(String &topic, String &payload);
uint32_t my_tick(void);
void my_touchpad_read(lv_indev_t *indev, lv_indev_data_t *data);
void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map);