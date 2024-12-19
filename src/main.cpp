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

#define ONE_WIRE_BUS 26 // GPIO для подключения шины данных DS18B20

const char ssid[] = "I love pussy";
const char pass[] = "*Cbufhtnf#";

unsigned long previousMillisLVGLwork = 0;
const unsigned long intervalLVGLwork = 10;

unsigned long previousMillisPIDprint = 0;
const unsigned long intervalPIDprint = 500;

unsigned long previousMillisMQTT = 0;
const unsigned long intervalMQTT = 10;

// Флаги состояния
bool isWiFiConnected = false;
bool isClientConnected = false;

// Таймеры для периодических проверок
unsigned long lastWiFiCheck = 0;
unsigned long lastClientCheck = 0;

// Интервалы проверки
const unsigned long wifiCheckInterval = 1000;
const unsigned long clientCheckInterval = 1000;

unsigned long lastMillis = 0;

const byte FAN_PWM_PIN = 12;
const byte relayPin = 13;
const byte Recylcuration_PWM_PIN = 27;

const unsigned long windowSize = 2000;
unsigned long windowStartTime, nextSwitchTime;
boolean relayStatus = false;

float Input, Output, Setpoint = 25, Kp = 8.5, Ki = 0.052, Kd = 0.2;
int dutyFan = 512, dutyRecylcuration = 1023;

QuickPID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd,
               QuickPID::pMode::pOnError,
               QuickPID::dMode::dOnMeas,
               QuickPID::iAwMode::iAwClamp,
               QuickPID::Action::direct);

Adafruit_SHT4x sht4 = Adafruit_SHT4x();

WiFiClient net;
MQTTClient client;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

DeviceAddress sensorAddresses[4];
const int sensorCount = 4; // Количество датчиков

void connect() {
    if (!isWiFiConnected) {
        // Проверяем Wi-Fi с интервалом
        if (millis() - lastWiFiCheck >= wifiCheckInterval) {
            lastWiFiCheck = millis();
            Serial.print("checking wifi...");
            if (WiFi.status() == WL_CONNECTED) {
                isWiFiConnected = true;
                Serial.println("\nWiFi connected!");
            } else {
                Serial.print(".");
            }
        }
    } else if (!isClientConnected) {
        // Проверяем подключение клиента с интервалом
        if (millis() - lastClientCheck >= clientCheckInterval) {
            lastClientCheck = millis();
            Serial.print("connecting client...");
            if (client.connect("arduino", "public", "public")) {
                isClientConnected = true;
                Serial.println("\nClient connected!");

                // Подписываемся на топики
                client.subscribe("Drier/Transmit/SHT40/Temperature");
                client.subscribe("Drier/Transmit/SHT40/Humidity");
                client.subscribe("Drier/Transmit/Signals/Discrete/PowerSt");
                client.subscribe("Drier/Transmit/Signals/Discrete/PIDSt");
            } else {
                Serial.print(".");
            }
        }
    }
}

void messageReceived(String &topic, String &payload) {
    // Serial.println("incoming: " + topic + " - " + payload);

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

        case 'p':
            Serial.println("Введите p: ");
            while (!Serial.available()) {
            }
            Kp = Serial.parseFloat();
            myPID.SetTunings(Kp, Ki, Kd);
            Serial.print("Новый p: ");
            Serial.println(Kp);
            break;

        case 'i':
            Serial.println("Введите i: ");
            while (!Serial.available()) {
            }
            Ki = Serial.parseFloat();
            myPID.SetTunings(Kp, Ki, Kd);
            Serial.print("Новый i: ");
            Serial.println(Ki);
            break;

        case 'd':
            Serial.println("Введите d: ");
            while (!Serial.available()) {
            }
            Kd = Serial.parseFloat();
            myPID.SetTunings(Kp, Ki, Kd);
            Serial.print("Новый d: ");
            Serial.println(Kd);
            break;

        case 'r':
            Serial.println("Введите y: ");
            while (!Serial.available()) {
            }
            myPID.Reset();
            Serial.println("ПИД сброшен!");
            break;

        case 'e':
            Serial.println("Введите коэффициент заполнения: ");
            while (!Serial.available()) {
            }
            dutyFan = Serial.parseInt();
            ledcWrite(FAN_PWM_PIN, dutyFan);
            Serial.print("Новый коэффициент заполнения: ");
            Serial.println(dutyFan);
            break;

        case 'f':
            Serial.println("Введите коэффициент заполнения рециркуляции: ");
            while (!Serial.available()) {
            }
            dutyRecylcuration = Serial.parseInt();
            ledcWrite(Recylcuration_PWM_PIN, dutyRecylcuration);
            Serial.print("Новый коэффициент заполнения рециркуляции: ");
            Serial.println(dutyRecylcuration);
            break;

        default:
            Serial.println("Неизвестная команда!");
            break;
        }
    }
}

// Вывод адреса датчика в сериал
void printAddress(DeviceAddress deviceAddress) {
    for (uint8_t i = 0; i < 8; i++) {
        if (deviceAddress[i] < 16)
            Serial.print("0");
        Serial.print(deviceAddress[i], HEX);
    }
}

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, pass);

    client.begin("mserver.lan", net);
    client.onMessage(messageReceived);

    connect();

    sensors.begin(); // Запуск библиотеки
    // Поиск датчиков и сохранение их адресов
    int foundSensors = sensors.getDeviceCount();
    Serial.print("Обнаружено датчиков: ");
    Serial.println(foundSensors);

    // Проверяем, что обнаружено достаточно датчиков
    if (foundSensors < sensorCount) {
        Serial.println("Внимание! Обнаружено меньше датчиков, чем требуется!");
    }

    for (int i = 0; i < sensorCount && i < foundSensors; i++) {
        if (sensors.getAddress(sensorAddresses[i], i)) {
            Serial.print("Датчик ");
            Serial.print(i + 1);
            Serial.print(" адрес: ");
            printAddress(sensorAddresses[i]);
            Serial.println();
        } else {
            Serial.print("Не удалось получить адрес для датчика ");
            Serial.println(i + 1);
        }
    }

    sht4.begin(&Wire);

    sht4.setPrecision(SHT4X_HIGH_PRECISION);
    sht4.setHeater(SHT4X_NO_HEATER);

    pinMode(relayPin, OUTPUT);

    ledcAttach(FAN_PWM_PIN, 40000, 10);
    ledcWrite(FAN_PWM_PIN, dutyFan);

    ledcAttach(Recylcuration_PWM_PIN, 40000, 10);
    ledcWrite(Recylcuration_PWM_PIN, dutyRecylcuration);

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
    if (millis() - previousMillisMQTT >= intervalMQTT) {
        previousMillisMQTT = millis();
        client.loop(); // Обработка MQTT-сообщений
    }

    // Проверка подключения и неблокирующее подключение
    if (!isWiFiConnected || !isClientConnected) {
        connect(); // Неблокирующая функция
    }

    sensors_event_t humidity, temp;
    sht4.getEvent(&humidity, &temp);

    checkUserInput();

    Input = temp.temperature;

    if (get_var_drier_state() == 1 && myPID.Compute()) {
        windowStartTime = millis();
        // Serial.println("ПИД, сработал,");
    }

    if (!relayStatus && Output > (millis() - windowStartTime)) {
        if (millis() > nextSwitchTime) {
            nextSwitchTime = millis();
            relayStatus = true;
            digitalWrite(relayPin, HIGH);
            set_var_solid_relay_st(255);
            client.publish("Drier/Transmit/Discrete/PIDSt", "1");
            Serial.print(">Heater: ");
            Serial.println("1");
        }
    } else if (relayStatus && Output < (millis() - windowStartTime)) {
        if (millis() > nextSwitchTime) {
            nextSwitchTime = millis();
            relayStatus = false;
            digitalWrite(relayPin, LOW);
            set_var_solid_relay_st(0);
            client.publish("Drier/Transmit/Discrete/PIDSt", "0");
            Serial.print(">Heater: ");
            Serial.println("0");
        }
    }

    if (millis() - previousMillisLVGLwork >= intervalLVGLwork) {
        previousMillisLVGLwork = millis();

        set_var_temperature(temp.temperature);
        set_var_humidity(humidity.relative_humidity);
        Setpoint = get_var_temperature_setpoint();

        lv_timer_handler(); /* let the GUI do its work */
        ui_tick();
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

        sensors.requestTemperatures(); // Запрос температуры со всех датчиков
        // Serial.println("Температура датчиков:");

        for (int i = 0; i < sensorCount; i++) {
            if (sensors.validAddress(sensorAddresses[i])) {
                float tempC = sensors.getTempC(sensorAddresses[i]);
                char tempCStr[10];
                dtostrf(tempC, 2, 2, tempCStr);

                // Формирование уникального топика для каждого сенсора
                char topic[50];
                snprintf(topic, sizeof(topic), "Drier/Transmit/Analog/Temperature/DS18B20n%d/", i + 1);

                client.publish(topic, tempCStr);

                // Serial.print("Датчик ");
                // Serial.print(i + 1);
                // Serial.print(": ");
                // Serial.print(tempC);
                // Serial.println(" °C");
            } else {
                Serial.print("Датчик ");
                Serial.print(i + 1);
                Serial.println(": Ошибка чтения.");
            }
        }
    }

    if (millis() - previousMillisPIDprint >= intervalPIDprint) {
        previousMillisPIDprint = millis();
        Serial.print(">Input/Temperature: ");
        Serial.println(Input);
        Serial.print(">Output: ");
        Serial.println(Output);
        // Serial.print(">Heater: ");
        // Serial.println(digitalRead(relayPin) ? "1" : "0");
        Serial.print(">Kpt: ");
        Serial.println(myPID.GetPterm());
        Serial.print(">Kit: ");
        Serial.println(myPID.GetIterm());
        Serial.print(">Kdt: ");
        Serial.println(myPID.GetDterm());
    }
}