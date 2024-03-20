#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/init.h>
#include <zephyr/zbus/zbus.h>

#include "battery/battery.h"
#include "battery/battery_ui.h"
#include "events/battery_event.h"
#include "managers/zsw_app_manager.h"
#include "ui/utils/zsw_ui_utils.h"
#include "pmic_ui.h"

LOG_MODULE_REGISTER(pmic_app, LOG_LEVEL_WRN);

static void pmic_app_start(lv_obj_t *root, lv_group_t *group);
static void pmic_app_stop(void);

static void zbus_battery_sample_data_callback(const struct zbus_channel *chan);

typedef struct battery_sample_t {
    struct battery_sample_event sample;
    int64_t timestamp;
} battery_sample_t;

ZBUS_CHAN_DECLARE(battery_sample_data_chan);
ZBUS_LISTENER_DEFINE(pmic_app_battery_event, zbus_battery_sample_data_callback);
ZBUS_CHAN_ADD_OBS(battery_sample_data_chan, pmic_app_battery_event, 1);

LV_IMG_DECLARE(battery_app_icon);

static battery_sample_t battery_samples[CONFIG_DEFAULT_CONFIGURATION_BATTERY_NUM_SAMPLES_MAX];
static int next_battery_sample_index;

static application_t app = {
    .name = "nPM1300",
    .icon = &(battery_app_icon),
    .start_func = pmic_app_start,
    .stop_func = pmic_app_stop
};

static void pmic_app_start(lv_obj_t *root, lv_group_t *group)
{
    struct battery_sample_event *sample;
    pmic_ui_show(root);

    for (int i = 0; i < CONFIG_DEFAULT_CONFIGURATION_BATTERY_NUM_SAMPLES_MAX; i++) {
        if (battery_samples[(next_battery_sample_index + i) % CONFIG_DEFAULT_CONFIGURATION_BATTERY_NUM_SAMPLES_MAX].timestamp !=
            0) {
            sample = &battery_samples[(next_battery_sample_index + i) %
                                      CONFIG_DEFAULT_CONFIGURATION_BATTERY_NUM_SAMPLES_MAX].sample;
            pmic_ui_add_measurement(sample->percent, sample->mV, sample->is_charging, sample->status, sample->temperature);
        }
    }
}

static void pmic_app_stop(void)
{
    pmic_ui_remove();
}

static void zbus_battery_sample_data_callback(const struct zbus_channel *chan)
{
    const struct battery_sample_event *event = zbus_chan_const_msg(chan);
    int previous_sample_index;

    if (next_battery_sample_index == 0) {
        previous_sample_index = CONFIG_DEFAULT_CONFIGURATION_BATTERY_NUM_SAMPLES_MAX - 1;
    } else {
        previous_sample_index = next_battery_sample_index - 1;
    }
    if (battery_samples[previous_sample_index].timestamp == 0 ||
        ((k_uptime_get() - battery_samples[previous_sample_index].timestamp) >=
         (1000))) {
        battery_samples[next_battery_sample_index].sample = *event;
        battery_samples[next_battery_sample_index].timestamp = k_uptime_get();
        LOG_DBG("Add %d\n", battery_samples[next_battery_sample_index].sample.mV);
        next_battery_sample_index = (next_battery_sample_index + 1) % CONFIG_DEFAULT_CONFIGURATION_BATTERY_NUM_SAMPLES_MAX;
    } else {
        LOG_DBG("Discard sample: %d, %d\n", battery_samples[previous_sample_index].sample.mV,
                (int)(k_uptime_get() - battery_samples[previous_sample_index].timestamp));
    }
    pmic_ui_add_measurement(event->percent, event->mV, event->is_charging, event->status, event->temperature);
    pmic_ui_update(event->tte, event->ttf, event->status, event->error, event->is_charging);
}

static int pmic_app_add(void)
{
    zsw_app_manager_add_application(&app);
    return 0;
}

SYS_INIT(pmic_app_add, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);