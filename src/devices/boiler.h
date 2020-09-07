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

#ifndef EMSESP_BOILER_H
#define EMSESP_BOILER_H

#include <Arduino.h>
#include <ArduinoJson.h>

#include <uuid/log.h>

#include <string>

#include "emsdevice.h"
#include "telegram.h"
#include "emsesp.h"
#include "helpers.h"
#include "mqtt.h"

namespace emsesp {

class Boiler : public EMSdevice {
  public:
    Boiler(uint8_t device_type, int8_t device_id, uint8_t product_id, const std::string & version, const std::string & name, uint8_t flags, uint8_t brand);

    virtual void show_values(uuid::console::Shell & shell);
    virtual void publish_values();
    virtual void device_info_web(JsonArray & root);
    virtual bool updated_values();
    virtual void add_context_menu();

  private:
    static uuid::log::Logger logger_;

    void console_commands(Shell & shell, unsigned int context);
    void register_mqtt_ha_config();
    void check_active();

    uint8_t last_boilerState = 0xFF; // remember last state of heating and warm water on/off
    uint8_t mqtt_format_;            // single, nested or ha
    bool    changed_ = false;

    static constexpr uint8_t EMS_TYPE_UBAParameterWW  = 0x33;
    static constexpr uint8_t EMS_TYPE_UBAFunctionTest = 0x1D;
    static constexpr uint8_t EMS_TYPE_UBAFlags        = 0x35;
    static constexpr uint8_t EMS_TYPE_UBASetPoints    = 0x1A;
    static constexpr uint8_t EMS_TYPE_UBAParameters   = 0x16;

    static constexpr uint8_t EMS_BOILER_SELFLOWTEMP_HEATING = 20; // was originally 70, changed to 30 for issue #193, then to 20 with issue #344

    // UBAParameterWW
    uint8_t wWActivated_     = EMS_VALUE_BOOL_NOTSET; // Warm Water activated
    uint8_t wWSelTemp_       = EMS_VALUE_UINT_NOTSET; // Warm Water selected temperature
    uint8_t wWCircPump_      = EMS_VALUE_BOOL_NOTSET; // Warm Water circulation pump available
    uint8_t wWCircPumpMode_  = EMS_VALUE_UINT_NOTSET; // Warm Water circulation pump mode
    uint8_t wWCircPumpType_  = EMS_VALUE_BOOL_NOTSET; // Warm Water circulation pump type
    uint8_t wWDisinfectTemp_ = EMS_VALUE_UINT_NOTSET; // Warm Water disinfection temperature to prevent infection
    uint8_t wWComfort_       = EMS_VALUE_UINT_NOTSET; // WW comfort mode

    // MC10Status
    uint16_t wwMixTemperature_          = EMS_VALUE_USHORT_NOTSET; // mengertemperatuur
    uint16_t wwBufferBoilerTemperature_ = EMS_VALUE_USHORT_NOTSET; // bufferboilertemperatuur

    // UBAMonitorFast - 0x18 on EMS1
    uint8_t  selFlowTemp_        = EMS_VALUE_UINT_NOTSET;   // Selected flow temperature
    uint16_t curFlowTemp_        = EMS_VALUE_USHORT_NOTSET; // Current flow temperature
    uint16_t wwStorageTemp1_     = EMS_VALUE_USHORT_NOTSET; // warm water storage temp 1
    uint16_t wwStorageTemp2_     = EMS_VALUE_USHORT_NOTSET; // warm water storage temp 2
    uint16_t retTemp_            = EMS_VALUE_USHORT_NOTSET; // Return temperature
    uint8_t  burnGas_            = EMS_VALUE_BOOL_NOTSET;   // Gas on/off
    uint8_t  fanWork_            = EMS_VALUE_BOOL_NOTSET;   // Fan on/off
    uint8_t  ignWork_            = EMS_VALUE_BOOL_NOTSET;   // Ignition on/off
    uint8_t  heatPmp_            = EMS_VALUE_BOOL_NOTSET;   // Boiler pump on/off
    uint8_t  wWHeat_             = EMS_VALUE_BOOL_NOTSET;   // 3-way valve on WW
    uint8_t  wWCirc_             = EMS_VALUE_BOOL_NOTSET;   // Circulation on/off
    uint8_t  selBurnPow_         = EMS_VALUE_UINT_NOTSET;   // Burner max power %
    uint8_t  curBurnPow_         = EMS_VALUE_UINT_NOTSET;   // Burner current power %
    uint16_t flameCurr_          = EMS_VALUE_USHORT_NOTSET; // Flame current in micro amps
    uint8_t  sysPress_           = EMS_VALUE_UINT_NOTSET;   // System pressure
    char     serviceCodeChar_[3] = {'\0'};                  // 2 character status/service code
    uint16_t serviceCode_        = EMS_VALUE_USHORT_NOTSET; // error/service code

    // UBAMonitorSlow - 0x19 on EMS1
    int16_t  extTemp_     = EMS_VALUE_SHORT_NOTSET;  // Outside temperature
    uint16_t boilTemp_    = EMS_VALUE_USHORT_NOTSET; // Boiler temperature
    uint16_t exhaustTemp_ = EMS_VALUE_USHORT_NOTSET; // Exhaust temperature
    uint8_t  pumpMod_     = EMS_VALUE_UINT_NOTSET;   // Pump modulation %
    uint32_t burnStarts_  = EMS_VALUE_ULONG_NOTSET;  // # burner restarts
    uint32_t burnWorkMin_ = EMS_VALUE_ULONG_NOTSET;  // Total burner operating time
    uint32_t heatWorkMin_ = EMS_VALUE_ULONG_NOTSET;  // Total heat operating time
    uint16_t switchTemp_  = EMS_VALUE_USHORT_NOTSET; // Switch temperature

    // UBAMonitorWW
    uint8_t  wWSetTmp_        = EMS_VALUE_UINT_NOTSET;   // Warm Water set temperature
    uint16_t wWCurTmp_        = EMS_VALUE_USHORT_NOTSET; // Warm Water current temperature
    uint16_t wWCurTmp2_       = EMS_VALUE_USHORT_NOTSET; // Warm Water current temperature storage
    uint32_t wWStarts_        = EMS_VALUE_ULONG_NOTSET;  // Warm Water # starts
    uint32_t wWWorkM_         = EMS_VALUE_ULONG_NOTSET;  // Warm Water # minutes
    uint8_t  wWOneTime_       = EMS_VALUE_BOOL_NOTSET;   // Warm Water one time function on/off
    uint8_t  wWDisinfecting_  = EMS_VALUE_BOOL_NOTSET;   // Warm Water disinfection on/off
    uint8_t  wWReadiness_     = EMS_VALUE_BOOL_NOTSET;   // Warm Water readiness on/off
    uint8_t  wWRecharging_    = EMS_VALUE_BOOL_NOTSET;   // Warm Water recharge on/off
    uint8_t  wWTemperatureOK_ = EMS_VALUE_BOOL_NOTSET;   // Warm Water temperature ok on/off
    uint8_t  wWCurFlow_       = EMS_VALUE_UINT_NOTSET;   // Warm Water current flow temp in l/min

    // UBATotalUptime
    uint32_t UBAuptime_ = EMS_VALUE_ULONG_NOTSET; // Total UBA working hours

    // UBAParameters
    uint8_t heating_temp_ = EMS_VALUE_UINT_NOTSET; // Heating temperature setting on the boiler
    uint8_t pump_mod_max_ = EMS_VALUE_UINT_NOTSET; // Boiler circuit pump modulation max. power %
    uint8_t pump_mod_min_ = EMS_VALUE_UINT_NOTSET; // Boiler circuit pump modulation min. power
    uint8_t burnPowermin_ = EMS_VALUE_UINT_NOTSET;
    uint8_t burnPowermax_ = EMS_VALUE_UINT_NOTSET;
    int8_t  boilTemp_off_ = EMS_VALUE_INT_NOTSET;
    int8_t  boilTemp_on_  = EMS_VALUE_INT_NOTSET;
    uint8_t burnPeriod_   = EMS_VALUE_UINT_NOTSET;
    uint8_t pumpDelay_    = EMS_VALUE_UINT_NOTSET;

    // UBASetPoint
    uint8_t setFlowTemp_  = EMS_VALUE_UINT_NOTSET; // boiler setpoint temp
    uint8_t setBurnPow_   = EMS_VALUE_UINT_NOTSET; // max output power in %
    uint8_t setWWPumpPow_ = EMS_VALUE_UINT_NOTSET; // ww pump speed/power?

    // other internal calculated params
    uint8_t tap_water_active_ = EMS_VALUE_BOOL_NOTSET; // Hot tap water is on/off
    uint8_t heating_active_   = EMS_VALUE_BOOL_NOTSET; // Central heating is on/off
    uint8_t pumpMod2_         = EMS_VALUE_UINT_NOTSET; // heatpump modulation from 0xE3 (heatpumps)

    void process_UBAParameterWW(std::shared_ptr<const Telegram> telegram);
    void process_UBAMonitorFast(std::shared_ptr<const Telegram> telegram);
    void process_UBATotalUptime(std::shared_ptr<const Telegram> telegram);
    void process_UBAParameters(std::shared_ptr<const Telegram> telegram);
    void process_UBAMonitorWW(std::shared_ptr<const Telegram> telegram);
    void process_UBAMonitorFastPlus(std::shared_ptr<const Telegram> telegram);
    void process_UBAMonitorSlow(std::shared_ptr<const Telegram> telegram);
    void process_UBAMonitorSlowPlus(std::shared_ptr<const Telegram> telegram);
    void process_UBAMonitorSlowPlus2(std::shared_ptr<const Telegram> telegram);
    void process_UBAOutdoorTemp(std::shared_ptr<const Telegram> telegram);
    void process_UBASetPoints(std::shared_ptr<const Telegram> telegram);
    void process_UBAFlags(std::shared_ptr<const Telegram> telegram);
    void process_MC10Status(std::shared_ptr<const Telegram> telegram);
    void process_UBAMaintenanceStatus(std::shared_ptr<const Telegram> telegram);
    void process_UBAMaintenanceData(std::shared_ptr<const Telegram> telegram);
    void process_UBAErrorMessage(std::shared_ptr<const Telegram> telegram);
    void process_UBADHWStatus(std::shared_ptr<const Telegram> telegram);

    // commands - none of these use the additional id parameter
    void set_warmwater_mode(const char * value, const int8_t id);
    void set_warmwater_activated(const char * value, const int8_t id);
    void set_tapwarmwater_activated(const char * value, const int8_t id);
    void set_warmwater_onetime(const char * value, const int8_t id);
    void set_warmwater_circulation(const char * value, const int8_t id);
    void set_warmwater_temp(const char * value, const int8_t id);
    void set_flow_temp(const char * value, const int8_t id);
    void set_min_power(const char * value, const int8_t id);
    void set_max_power(const char * value, const int8_t id);
    void set_hyst_on(const char * value, const int8_t id);
    void set_hyst_off(const char * value, const int8_t id);
    void set_burn_period(const char * value, const int8_t id);
    void set_pump_delay(const char * value, const int8_t id);
};

} // namespace emsesp

#endif