/*
 * EMS-ESP - https://github.com/proddy/EMS-ESP
 * Copyright 2019  Paul Derbyshire
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EMSESPStatusService_h
#define EMSESPStatusService_h

#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <ESPAsyncWebServer.h>
#include <SecurityManager.h>
#include <AsyncMqttClient.h>

#define MAX_EMSESP_STATUS_SIZE 1024
#define EMSESP_STATUS_SERVICE_PATH "/rest/emsespStatus"

namespace emsesp {

class EMSESPStatusService {
  public:
    EMSESPStatusService(AsyncWebServer * server, SecurityManager * securityManager);

  private:
    void emsespStatusService(AsyncWebServerRequest * request);

#ifdef ESP32
    static void onStationModeDisconnected(WiFiEvent_t event, WiFiEventInfo_t info);
    static void onStationModeGotIP(WiFiEvent_t event, WiFiEventInfo_t info);
#elif defined(ESP8266)
    WiFiEventHandler _onStationModeDisconnectedHandler;
    WiFiEventHandler _onStationModeGotIPHandler;
    static void      onStationModeDisconnected(const WiFiEventStationModeDisconnected & event);
    static void      onStationModeGotIP(const WiFiEventStationModeGotIP & event);
#endif
};

} // namespace emsesp

#endif
