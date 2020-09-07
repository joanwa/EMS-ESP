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

#include "controller.h"

namespace emsesp {

REGISTER_FACTORY(Controller, EMSdevice::DeviceType::CONTROLLER);

uuid::log::Logger Controller::logger_{F_(controller), uuid::log::Facility::CONSOLE};

Controller::Controller(uint8_t device_type, uint8_t device_id, uint8_t product_id, const std::string & version, const std::string & name, uint8_t flags, uint8_t brand)
    : EMSdevice(device_type, device_id, product_id, version, name, flags, brand) {
}

void Controller::add_context_menu() {
}

void Controller::device_info_web(JsonArray & root) {
}

// display all values into the shell console
void Controller::show_values(uuid::console::Shell & shell) {
    // EMSdevice::show_values(shell); // always call this to show header
}

// publish values via MQTT
void Controller::publish_values() {
}

// check to see if values have been updated
bool Controller::updated_values() {
    return false;
}

// add console commands
void Controller::console_commands() {
}

} // namespace emsesp