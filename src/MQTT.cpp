#include "global_var.h"

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
