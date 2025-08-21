#include "UE/DIGI_XBEE3.h"

#include "my_logger.h"
#include "Configs/Fuzzing_Settings/main_config.h"
#include "Configs/Fuzzing_Settings/timers_config.h"

#include "UE/Communication/AT_Serial_Communication.h"

extern My_Logger my_logger_g;
extern Main_Config main_config_g;
extern Timers_Config timers_config_g;

DIGI_XBEE3::DIGI_XBEE3() {
    dut_communication_ = std::make_unique<AT_Serial_Communication>();
}

bool DIGI_XBEE3::start() {
    return true;
}

bool DIGI_XBEE3::stop() {
    return true;
}

bool DIGI_XBEE3::restart() {
    return true;
}

bool DIGI_XBEE3::connect_to_network() {
    if (!set_command_mode()) {
        my_logger_g.logger->error("Could not set the command mode");
        return false;
    }
    static std::string command = "AT AM 0";
    AT_Response resp = dut_communication_->send_AT_COMMAND_nonblocking(command, 2, OK);
    my_logger_g.logger->debug("Got response {} after sending command {}", resp.response, command);
    if (!apply_at_commands()) {
        my_logger_g.logger->error("Could not apply the at commands");
        return false;
    }
    // Wait until the modem stops initializing
    while (get_association_indication() == "FF") {
        continue;
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return true;
}

bool DIGI_XBEE3::is_connected() {
    if (!set_command_mode()) {
        my_logger_g.logger->error("Could not set the command mode");
        return false;
    }
    std::string ot = get_operation_technology(); // TODO: remove
    std::string response = get_association_indication();
    const std::string connected_ai = "0";
    if (response != connected_ai) return false;
    return true;
}

bool DIGI_XBEE3::disconnect_from_network() {
    if (!set_command_mode()) {
        my_logger_g.logger->error("Could not set the command mode");
        return false;
    }
    static std::string command = "AT AM 1";
    AT_Response resp = dut_communication_->send_AT_COMMAND_nonblocking(command, 2, OK);
    my_logger_g.logger->debug("Got response {} after sending command {}", resp.response, command);
    if (!apply_at_commands()) {
        my_logger_g.logger->error("Could not apply the at commands");
        return false;
    }
    // Wait some time
    std::this_thread::sleep_for(std::chrono::seconds(3));
    return true;
}

bool DIGI_XBEE3::is_alive() {
    if (!set_command_mode()) {
        my_logger_g.logger->error("Could not set the command mode");
        return false;
    }
    static std::string command = "AT";
    AT_Response resp = dut_communication_->send_AT_COMMAND_nonblocking(command, 2, OK);
    my_logger_g.logger->debug("Got response {} after sending command {}", resp.response, command);
    return resp.is_OK;
}

bool DIGI_XBEE3::set_command_mode() {
    std::string response;
    for (int i = 0; i < 3; i++) {
        response = dut_communication_->send_command_nonblocking("+++", 1, "OK", "");
        my_logger_g.logger->debug("Got response {} after setting the command mode", response);
        if (!response.empty()) return true;
        my_logger_g.logger->debug("Trying again");
    }
    my_logger_g.logger->debug("Stop trying");
    return false;
}

bool DIGI_XBEE3::apply_at_commands() {
    static std::string command = "AT AC";
    AT_Response resp = dut_communication_->send_AT_COMMAND_nonblocking(command, 2, OK);
    my_logger_g.logger->debug("Got response {} after sending command {}", resp.response, command);
    if (!resp.is_OK) return false;
    // Wait some time
    return true;
}

std::string DIGI_XBEE3::get_association_indication() {
    static std::string command = "AT AI";
    AT_Response resp = dut_communication_->send_AT_COMMAND_nonblocking(command, 1, "");
    my_logger_g.logger->debug("Got response {} after sending command {}", resp.response, command);
    return resp.response;
}

std::string DIGI_XBEE3::get_operation_technology() {
    static std::string command = "AT OT";
    AT_Response resp = dut_communication_->send_AT_COMMAND_nonblocking(command, 1, "");
    my_logger_g.logger->debug("Got response {} after sending command {}", resp.response, command);
    return resp.response;
}