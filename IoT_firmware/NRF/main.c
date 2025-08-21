/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <dk_buttons_and_leds.h>

#include <modem/nrf_modem_lib.h>
#include <modem/lte_lc.h>

// Reboot
#include <zephyr/sys/reboot.h>

// Watchdog
#include <zephyr/drivers/watchdog.h>

#define WDT_MAX_WINDOW  20000U

static const struct device *wdt = DEVICE_DT_GET(DT_NODELABEL(wdt0));
static int wdt_channel_id;

LOG_MODULE_REGISTER(LTE_FUZZER, LOG_LEVEL_INF);

int crash_detected = 0;
/**@brief Modem fault handler. */
void nrf_modem_fault_handler(struct nrf_modem_fault_info *fault_info)
{
	LOG_ERR("LTE_CRASH reason: %u", fault_info->reason);
	crash_detected = 1;
}

void restart_modem() {
	nrf_modem_shutdown();
	nrf_modem_lib_init();
	crash_detected = 0;
}

int main(void)
{

	int err;

	err = nrf_modem_lib_init();
	if (err) {
		LOG_ERR("Failed to initialize the modem library, error: %d", err);
		return err;
	}
	
	err = lte_lc_init();
	if (err) {
		LOG_ERR("Failed to initialize the LTE link controller, error: %d", err);
		return err;
	}
	
	for (int i = 0; i < 2; i++) {

		LOG_INF("LTE_NEW_ITERATION (iter: %d)", i);
		err = lte_lc_connect();
		if (crash_detected) {
			lte_lc_deinit();
			break;
		}

		if (err) {
			LOG_ERR("LTE_NOT_CONNECTED, error: %d (iter: %d)", err, i);
		} else {
			LOG_INF("LTE_CONNECTED (iter: %d)", i);
		}
		lte_lc_power_off();
	}

	lte_lc_deinit();
	
	LOG_INF("PROGRAM FINISHED");

	return 0;
}
