#include "Coordinators/passive_coordinator.h"

#include "libserial/SerialStream.h"

#include "UE/Communication/AT_Serial_Communication.h"

#include "Configs/Fuzzing_Settings/main_config.h"
#include "Configs/Fuzzing_Settings/serial_config.h"
#include "Configs/Fuzzing_Strategies/fuzz_strategy_config.h"
#include "Communication/shm_layer_communication.h"
#include "shared_memory.h"

#include "my_logger.h"
#include "statistics.h"

extern Main_Config main_config_g;
extern Serial_Config serial_config_g;
extern Fuzz_Strategy_Config fuzz_strategy_config_g;
extern My_Logger my_logger_g;
extern std::string dut_log_file_name_g;
extern Statistics statistics_g;

Passive_Coordinator::Passive_Coordinator() {}

bool Passive_Coordinator::init(const std::vector<std::string>& fuzz_strategy_config_names) {
    if (!init_fuzzing_strategies(fuzz_strategy_config_names)) {
        my_logger_g.logger->error("Failed to init the fuzzing strategies");
        return false;
    }
    return true;
}

void Passive_Coordinator::deinit() {
    return;
}

std::string Passive_Coordinator::get_name() {
    return name_;
}

void Passive_Coordinator::thread_dut_communication_func() {

    consecutive_successful_attach_counter = 0;
    consecutive_failed_attach_counter = 0;
    consecutive_empty_iterations = 0;
    
    if (serial_config_g.modem_ports.empty()) {
        printf(RED "Modem port is not specified\n" CRESET);
        throw std::runtime_error("Modem port is not specified");
    }
    LibSerial::SerialStream serial_stream;

    bool port_opened = false;
    for (const std::string& port : serial_config_g.modem_ports) {
        try {
            serial_stream.Open(port);
            port_opened = true;
            my_logger_g.logger->info("Using serial port {}", port);
            break;
        } catch(std::exception& ex) {
            my_logger_g.logger->warn("Port {} is not available: {}", port, ex.what());
            continue;
        }
    }
    if (!port_opened) throw std::runtime_error("Serial port could not be opened");

    LibSerial::BaudRate baudrate = get_libserial_baudrate(serial_config_g.baudrate);
    if (baudrate == LibSerial::BaudRate::BAUD_INVALID) {
        my_logger_g.logger->error("Invalid baudrate: {}", serial_config_g.baudrate);
        throw std::runtime_error("Invalid baudrate");
    }
    serial_stream.SetBaudRate(baudrate);

    const size_t MAX_LINE_LEN = 256;
    char serial_output[MAX_LINE_LEN];

    /* USER CAN CUSTOMIZE THIS PART */
    std::string MODULE_NAME = "";//;"LTE_FUZZER";
    std::string NEW_ITER = "";//"LTE_NEW_ITERATION";
    std::string CONNECTED = "";//"LTE_CONNECTED";
    std::string NOT_CONNECTED = "";//"LTE_NOT_CONNECTED";
    std::string CRASH = "";// "LTE_CRASH";
    std::string WATCHDOG_CALLBACK = "";//"LTE_WATCHDOG_CALLBACK";

    for (const Fuzz_Strategy_Config& fuzz_strategy_config : fuzz_strategy_configs_) {

        if (!prepare_new_fuzzing_sprint(fuzz_strategy_config)) {
            my_logger_g.logger->error("Failed to prepare new fuzzing sprint");
            break;
        }

        while (SHM_Layer_Communication::is_active) {

            my_logger_g.logger->info("================ START OF A NEW FUZZING ITERATION {} ================", global_iteration);
            my_logger_g.logger->flush();

            if (!serial_stream.IsOpen()) {
                my_logger_g.logger->error("Serial stream is closed");
                throw std::runtime_error("Serial stream is closed");
            }
            serial_stream.getline(serial_output, MAX_LINE_LEN);
            std::string serial_output_str(serial_output);
            my_logger_g.logger->debug("SERIAL: {}", serial_output_str);

            if (MODULE_NAME.empty() || serial_output_str.find(MODULE_NAME) != std::string::npos) {
                // That's output from my application running on the microcontroller
                if (!NEW_ITER.empty() && serial_output_str.find(NEW_ITER) != std::string::npos) {
                    continue;
                } else if (!CRASH.empty() && serial_output_str.find(CRASH) != std::string::npos) {
                    iteration_result.attach_result = AttachResult::FAIL;
                    my_logger_g.logger->error("CRASH DETECTED");
                    statistics_g.ue_crash_counter++;
                } else if (!CONNECTED.empty() && serial_output_str.find(CONNECTED) != std::string::npos) {
                    iteration_result.attach_result = AttachResult::SUCCESS;
                    my_logger_g.logger->info("ATTACH SUCCESSFUL");
                    statistics_g.attach_successful_counter++;
                    consecutive_successful_attach_counter++;
                    consecutive_failed_attach_counter = 0;
                    consecutive_empty_iterations = 0;
                } else if (!NOT_CONNECTED.empty() && serial_output_str.find(NOT_CONNECTED) != std::string::npos) {
                    iteration_result.attach_result = AttachResult::FAIL;
                    my_logger_g.logger->info("ATTACH FAILED");
                    statistics_g.attach_failed_counter++;
                    consecutive_failed_attach_counter++;
                    if (Base_Fuzzer::current_seed->is_empty() && (std::find(fuzz_strategy_config_g.fuzzing_strategies.begin(), fuzz_strategy_config_g.fuzzing_strategies.end(), FuzzingStrategy::NONE) == fuzz_strategy_config_g.fuzzing_strategies.end())) { statistics_g.empty_iterations++; consecutive_empty_iterations++;}
                    else consecutive_empty_iterations = 0;
                    consecutive_successful_attach_counter = 0;
                } else if (!WATCHDOG_CALLBACK.empty() && serial_output_str.find(WATCHDOG_CALLBACK) != std::string::npos) {
                    iteration_result.attach_result = AttachResult::FAIL;
                    my_logger_g.logger->error("HANG DETECTED");
                    if (Base_Fuzzer::current_seed->is_empty() && (std::find(fuzz_strategy_config_g.fuzzing_strategies.begin(), fuzz_strategy_config_g.fuzzing_strategies.end(), FuzzingStrategy::NONE) == fuzz_strategy_config_g.fuzzing_strategies.end())) { statistics_g.empty_iterations++; consecutive_empty_iterations++;}
                    else consecutive_empty_iterations = 0;
                    consecutive_successful_attach_counter = 0;
                    consecutive_failed_attach_counter = 0;
                    statistics_g.hang_counter++;
                } else {
                    continue;
                }
            } else {
                continue;
            }

            if (!renew_fuzzing_iteration()) {
                break;
            }
        }

    }
}

bool Passive_Coordinator::reset_target() {
    return true;
}