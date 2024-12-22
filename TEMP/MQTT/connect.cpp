void connect() {
    if (!isWiFiConnected) {
        // Existing WiFi check code...
    } else if (!isClientConnected) {
        if (millis() - lastClientCheck >= clientCheckInterval) {
            lastClientCheck = millis();

            // Start new connection if needed
            if (client.getConnectState() == MQTTClient::MQTT_DISCONNECTED) {
                if (client.beginConnect("arduino", "public", "public")) {
                    Serial.println("Starting MQTT connection...");
                }
            }
            // Continue existing connection
            else if (client.isConnecting()) {
                if (client.continueConnect()) {
                    if (client.getConnectState() == MQTTClient::MQTT_CONNECTED) {
                        isClientConnected = true;
                        Serial.println("MQTT Connected!");
                        // Subscribe to topics
                        client.subscribe("Drier/Transmit/SHT40/Temperature");
                        client.subscribe("Drier/Transmit/SHT40/Humidity");
                        client.subscribe("Drier/Transmit/Signals/Discrete/PowerSt");
                        client.subscribe("Drier/Transmit/Signals/Discrete/PIDSt");
                    }
                }
            }
        }
    }
}