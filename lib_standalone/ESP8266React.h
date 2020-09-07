#ifndef ESP8266React_h
#define ESP8266React_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <AsyncMqttClient.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <SecurityManager.h>
#include <SecuritySettingsService.h>
#include <StatefulService.h>

class DummySettings {
  public:
    uint8_t  tx_mode              = 1;
    uint8_t  ems_bus_id           = 0x0B;
    bool     system_heartbeat     = false;
    int8_t   syslog_level         = 1; // uuid::log::Level
    uint32_t syslog_mark_interval = 0;
    String   syslog_host          = "192.168.1.4";
    uint8_t  master_thermostat    = 0;
    bool     shower_timer         = false;
    bool     shower_alert         = false;
    bool     hide_led             = false;
    uint16_t publish_time         = 10; // seconds
    uint8_t  mqtt_format          = 3;  // 1=single, 2=nested, 3=ha, 4=custom
    uint8_t  mqtt_qos             = 0;
    String   hostname             = "ems-esp";
    String   jwtSecret            = "ems-esp";
    String   ssid                 = "ems-esp";
    String   password             = "ems-esp";

    static void read(DummySettings & settings, JsonObject & root){};
    static void read(DummySettings & settings){};

    static StateUpdateResult update(JsonObject & root, DummySettings & settings) {
        return StateUpdateResult::CHANGED;
    }
};

class DummySettingsService : public StatefulService<DummySettings> {
  public:
    DummySettingsService(AsyncWebServer * server, FS * fs, SecurityManager * securityManager){};

    void begin();
    void loop();

  private:
};

#define WiFiSettings DummySettings
#define SecuritySettings DummySettings
#define MqttSettings DummySettings

class ESP8266React {
  public:
    ESP8266React(AsyncWebServer * server, FS * fs)
        : _settings(server, fs, nullptr)
        , _securitySettingsService(server, fs){};

    void begin(){};
    void loop(){};

    SecurityManager * getSecurityManager() {
        return &_securitySettingsService;
    }

    AsyncMqttClient * getMqttClient() {
        return _mqttClient;
    }

    StatefulService<DummySettings> * getWiFiSettingsService() {
        return &_settings;
    }

    StatefulService<DummySettings> * getSecuritySettingsService() {
        return &_settings;
    }

    StatefulService<DummySettings> * getMqttSettingsService() {
        return &_settings;
    }

  private:
    DummySettingsService    _settings;
    SecuritySettingsService _securitySettingsService;

    AsyncMqttClient * _mqttClient;
};

class EMSESPSettingsService {
  public:
    EMSESPSettingsService(AsyncWebServer * server, FS * fs, SecurityManager * securityManager);

    void begin();

  private:
};

#endif
