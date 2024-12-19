#include "global_var.h"

unsigned long previousMillisLVGLwork = 0;
unsigned long previousMillisPIDprint = 0;
unsigned long previousMillisMQTT = 0;

// Флаги состояния
bool isWiFiConnected = false;
bool isClientConnected = false;

// Таймеры для периодических проверок
unsigned long lastWiFiCheck = 0;
unsigned long lastClientCheck = 0;
unsigned long lastMillis = 0;

unsigned long windowStartTime, nextSwitchTime;

float Input, Output, Setpoint = 25, Kp = 8.5, Ki = 0.052, Kd = 0.2;
int dutyFan = 512, dutyRecylcuration = 1023;

uint32_t draw_buf[DRAW_BUF_SIZE / 4];

DeviceAddress sensorAddresses[4];

boolean relayStatus = false;

QuickPID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd,
               QuickPID::pMode::pOnError,
               QuickPID::dMode::dOnMeas,
               QuickPID::iAwMode::iAwClamp,
               QuickPID::Action::direct);

TFT_eSPI tft = TFT_eSPI(TFT_HOR_RES, TFT_VER_RES);
FT6336U ft6336u(I2C_SDA, I2C_SCL, RST_N_PIN, INT_N_PIN);

Adafruit_SHT4x sht4 = Adafruit_SHT4x();

WiFiClient net;
MQTTClient client;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
