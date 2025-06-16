#include "global_var.h"

void checkUserInput() {
    if (Serial.available()) {
        char input = Serial.read();

        switch (input) {
        case 's':
            Serial.println("Введите целевую температуру: ");
            while (!Serial.available())
                ;
            Setpoint = Serial.parseFloat();
            Serial.print("Новая целевая температура: ");
            Serial.println(Setpoint);
            break;

        case 'p':
            Serial.println("Введите p: ");
            while (!Serial.available())
                ;
            Kp = Serial.parseFloat();
            myPID.SetTunings(Kp, Ki, Kd);
            Serial.print("Новый p: ");
            Serial.println(Kp);
            break;

        case 'i':
            Serial.println("Введите i: ");
            while (!Serial.available())
                ;
            Ki = Serial.parseFloat();
            myPID.SetTunings(Kp, Ki, Kd);
            Serial.print("Новый i: ");
            Serial.println(Ki);
            break;

        case 'd':
            Serial.println("Введите d: ");
            while (!Serial.available())
                ;
            Kd = Serial.parseFloat();
            myPID.SetTunings(Kp, Ki, Kd);
            Serial.print("Новый d: ");
            Serial.println(Kd);
            break;

        case 'r':
            Serial.println("Введите y: ");
            while (!Serial.available())
                ;
            myPID.Reset();
            Serial.println("ПИД сброшен!");
            break;

        case 'e':
            Serial.println("Введите коэффициент заполнения: ");
            while (!Serial.available())
                ;
            dutyFan = Serial.parseInt();
            ledcWrite(FAN_PWM_PIN, dutyFan);
            Serial.print("Новый коэффициент заполнения: ");
            Serial.println(dutyFan);
            break;

        case 'f':
            Serial.println("Введите коэффициент заполнения рециркуляции: ");
            while (!Serial.available())
                ;
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

// Вывод адреса датчика в последовательный порт
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

    sensors.begin();
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
    lv_tick_set_cb(my_tick_get_cb);

    ft6336u.begin();

#if LV_USE_LOG != 0
    lv_log_register_print_cb(my_print); /* register print function for debugging */
#endif

    tft.init();
    tft.setRotation(3); /* Landscape orientation, flipped */

    // Создание дисплея для LVGL 9.3.0
    disp = lv_display_create(TFT_HOR_RES, TFT_VER_RES);

    // Установка функции flush
    lv_display_set_flush_cb(disp, my_disp_flush);

    // Установка буфера отрисовки
    lv_display_set_buffers(disp, draw_buf, NULL, sizeof(draw_buf), LV_DISPLAY_RENDER_MODE_PARTIAL);

    // Установка поворота дисплея
    lv_display_set_rotation(disp, LV_DISPLAY_ROTATION_270);

    // Создание устройства ввода (тачскрин)
    lv_indev_t *indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, my_touchpad_read);

    ui_init();

    set_var_temperature_setpoint(Setpoint);

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
    // if (!isWiFiConnected || !isClientConnected) {
    //     connect(); // Неблокирующая функция
    // }

    connect();

    sensors_event_t humidity, temp;
    sht4.getEvent(&humidity, &temp);

    checkUserInput();

    Input = temp.temperature;

    if (get_var_drier_state() == 1 && myPID.Compute()) {
        windowStartTime = millis();
    }

    if (!relayStatus && Output > (millis() - windowStartTime)) {
        if (millis() > nextSwitchTime) {
            nextSwitchTime = millis();
            relayStatus = true;
            digitalWrite(relayPin, HIGH);
            set_var_solid_relay_st(255);
        }
    } else if (relayStatus && Output < (millis() - windowStartTime)) {
        if (millis() > nextSwitchTime) {
            nextSwitchTime = millis();
            relayStatus = false;
            digitalWrite(relayPin, LOW);
            set_var_solid_relay_st(0);
        }
    }

    if (millis() - previousMillisLVGLwork >= intervalLVGLwork) {
        previousMillisLVGLwork = millis();
        lv_timer_handler(); /* let the GUI do its work */
        ui_tick();
    }

    if (millis() - previousMillisUIvar >= intervalUIvar) {
        previousMillisUIvar = millis();
        set_var_temperature(temp.temperature);
        set_var_humidity(humidity.relative_humidity);
    }

    Setpoint = get_var_temperature_setpoint();

    if (millis() - lastMillis > 1000) {
        lastMillis = millis();

        char temperatureStr[10];
        char humidityStr[10];
        char powerStSrt[2];
        char relayStatusStr[2];

        dtostrf(temp.temperature, 2, 2, temperatureStr);
        dtostrf(humidity.relative_humidity, 2, 2, humidityStr);
        dtostrf(get_var_drier_state(), 1, 0, powerStSrt);

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
