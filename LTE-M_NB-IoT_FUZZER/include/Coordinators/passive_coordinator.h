#pragma once

#include "Coordinators/base_coordinator.h"

class Passive_Coordinator : public Base_Coordinator {
public:
    Passive_Coordinator();

    void thread_dut_communication_func() override;
    bool reset_target() override;
    bool init(const std::vector<std::string>&) override;
    void deinit() override;
    
    std::string get_name() override;

private:
    std::string name_ = "Passive Coordinator";
};