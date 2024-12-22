bool MQTTClient::beginConnect(const char clientID[], const char username[], const char password[]) {
    // Check if already connecting
    if (_connectionInProgress) {
        return false;
    }

    // Store connection parameters
    _pendingClientID = clientID;
    _pendingUsername = username;
    _pendingPassword = password;

    // Initialize connection state
    _connectState = MQTT_TCP_CONNECTING;
    _connectionInProgress = true;
    _connectStartTime = millis();

    // Close existing connection if any
    if (this->connected()) {
        this->close();
    }

    // Save client
    this->network.client = this->netClient;

    return true;
}

bool MQTTClient::continueConnect() {
    if (!_connectionInProgress) {
        return false;
    }

    switch (_connectState) {
    case MQTT_TCP_CONNECTING: {
        // Try TCP connection
        int ret;
        if (this->hostname != nullptr) {
            ret = this->netClient->connect(this->hostname, (uint16_t)this->port);
        } else {
            ret = this->netClient->connect(this->address, (uint16_t)this->port);
        }

        if (ret <= 0) {
            this->_lastError = LWMQTT_NETWORK_FAILED_CONNECT;
            _connectionInProgress = false;
            _connectState = MQTT_DISCONNECTED;
            return false;
        }

        _connectState = MQTT_MQTT_CONNECTING;
        return true;
    }

    case MQTT_MQTT_CONNECTING: {
        // Prepare MQTT connection options
        lwmqtt_connect_options_t options = lwmqtt_default_connect_options;
        options.keep_alive = this->keepAlive;
        options.clean_session = this->cleanSession;
        options.client_id = lwmqtt_string(_pendingClientID);

        if (_pendingUsername != nullptr) {
            options.username = lwmqtt_string(_pendingUsername);
        }
        if (_pendingPassword != nullptr) {
            options.password = lwmqtt_string(_pendingPassword);
        }

        // Attempt MQTT connection
        this->_lastError = lwmqtt_connect(&this->client, &options, this->will, this->timeout);

        // Copy return code
        this->_returnCode = options.return_code;

        if (this->_lastError != LWMQTT_SUCCESS) {
            this->close();
            _connectionInProgress = false;
            _connectState = MQTT_DISCONNECTED;
            return false;
        }

        // Set flags
        this->_connected = true;
        this->_sessionPresent = options.session_present;
        _connectionInProgress = false;
        _connectState = MQTT_CONNECTED;
        return true;
    }

    default:
        return false;
    }
}