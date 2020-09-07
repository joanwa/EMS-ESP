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

// code originally written by nomis - https://github.com/nomis

#ifndef EMSESP_SENSORS_H
#define EMSESP_SENSORS_H

#include <string>
#include <vector>

#include "helpers.h"
#include "mqtt.h"
#include "console.h"

#include <uuid/log.h>

#ifndef EMSESP_STANDALONE
#include <OneWire.h>
#endif

namespace emsesp {

class Sensors {
  public:
    class Device {
      public:
        Device(const uint8_t addr[]);
        ~Device() = default;

        uint64_t    id() const;
        std::string to_string() const;

        float temperature_c = NAN;

      private:
        const uint64_t id_;
        bool           registered_ = false;
    };

    Sensors()  = default;
    ~Sensors() = default;

    void start();
    void loop();
    void publish_values();
    void reload();
    bool updated_values();

    const std::vector<Device> devices() const;

  private:
    static constexpr uint8_t MAX_SENSORS = 20;

    enum class State { IDLE, READING, SCANNING };

    static constexpr size_t ADDR_LEN = 8;

    static constexpr size_t SCRATCHPAD_LEN      = 9;
    static constexpr size_t SCRATCHPAD_TEMP_MSB = 1;
    static constexpr size_t SCRATCHPAD_TEMP_LSB = 0;
    static constexpr size_t SCRATCHPAD_CONFIG   = 4;
    static constexpr size_t SCRATCHPAD_CNT_REM  = 6;

    // dallas chips
    static constexpr uint8_t TYPE_DS18B20 = 0x28;
    static constexpr uint8_t TYPE_DS18S20 = 0x10;
    static constexpr uint8_t TYPE_DS1822  = 0x22;
    static constexpr uint8_t TYPE_DS1825  = 0x3B; // also DS1826

    static constexpr uint32_t READ_INTERVAL_MS = 5000; // 5 seconds
    static constexpr uint32_t CONVERSION_MS    = 1000; // 1 seconds
    static constexpr uint32_t READ_TIMEOUT_MS  = 2000; // 2 seconds
    static constexpr uint32_t SCAN_TIMEOUT_MS  = 3000; // 3 seconds

    static constexpr uint8_t CMD_CONVERT_TEMP    = 0x44;
    static constexpr uint8_t CMD_READ_SCRATCHPAD = 0xBE;

    static uuid::log::Logger logger_;

#ifndef EMSESP_STANDALONE
    OneWire bus_;
#endif

    bool  temperature_convert_complete();
    float get_temperature_c(const uint8_t addr[]);

    uint32_t            last_activity_ = uuid::get_uptime();
    uint32_t            last_publish_  = uuid::get_uptime();
    State               state_         = State::IDLE;
    std::vector<Device> found_;
    std::vector<Device> devices_;

    bool registered_ha_[MAX_SENSORS];

    uint8_t mqtt_format_;
    uint8_t retrycnt_    = 0;
    uint8_t dallas_gpio_ = 0;
    bool    changed_     = false;
};

} // namespace emsesp

#endif