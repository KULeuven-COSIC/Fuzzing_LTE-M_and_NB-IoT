#pragma once

#include "UE/UE.h"
#include "UE/Communication/AT_Communication.h"

#include <memory>

class DIGI_XBEE3 : public UE {
public:
    DIGI_XBEE3();
    bool start() override;
    bool stop() override;
    bool restart() override;
    bool is_alive() override;
    bool connect_to_network() override;
    bool is_connected() override;
    bool disconnect_from_network() override;

private:
    const std::string name_ = "DIGI_XBEE3";
    std::unique_ptr<AT_Communication> dut_communication_;

    bool set_command_mode();
    bool apply_at_commands();
    std::string get_operation_technology();
    std::string get_association_indication();
    
};