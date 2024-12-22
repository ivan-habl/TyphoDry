class MQTTClient {
  public:
    // Connection states
    enum ConnectState {
        MQTT_DISCONNECTED,    // Not connected
        MQTT_TCP_CONNECTING,  // TCP connection in progress
        MQTT_MQTT_CONNECTING, // MQTT protocol handshake
        MQTT_CONNECTED        // Fully connected
    };

  private:
    // Connection state tracking
    ConnectState _connectState = MQTT_DISCONNECTED;
    unsigned long _connectStartTime = 0;
    const char *_pendingClientID = nullptr;
    const char *_pendingUsername = nullptr;
    const char *_pendingPassword = nullptr;
    bool _connectionInProgress = false;

    // Existing private members...
    size_t readBufSize = 0;
    size_t writeBufSize = 0;
    uint8_t *readBuf = nullptr;
    uint8_t *writeBuf = nullptr;
    // ...existing code...

  public:
    // New public methods
    bool beginConnect(const char clientID[], const char username[] = nullptr, const char password[] = nullptr);
    bool continueConnect();
    ConnectState getConnectState() const { return _connectState; }
    bool isConnecting() const { return _connectionInProgress; }

    // Existing public members...
    void *ref = nullptr;
    explicit MQTTClient(int bufSize = 128) : MQTTClient(bufSize, bufSize) {}
    MQTTClient(int readSize, int writeBufSize);
    // ...existing code...
};