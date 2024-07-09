/**
 * @file lea_assistant_app.c
 * @author Leonardo Bispo
 *
 * @brief LE Audio Broadcast assistant, a Broadcast Assistant can find Broadcast Audio Streams for a Broadcast Receiver.
 *
 * @see https://github.com/larsgk/web-broadcast-assistant
 * @see https://www.bluetooth.com/learn-about-bluetooth/feature-enhancements/le-audio/le-audio-specifications/
 *
 */
#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/zbus/zbus.h>

#include "lea_assistant_ui.h"
#include "lea_assistant_app.h"
#include "managers/zsw_app_manager.h"
#include "ble/ble_comm.h"
#include "events/ble_event.h"
#include "ui/utils/zsw_ui_utils.h"

#include "message_handler.h"

#define MAX_DEVICES_NUM 16

typedef struct devices_list {
    uint8_t num;
    bt_addr_le_t addr[MAX_DEVICES_NUM];
} devices_list_t;

LOG_MODULE_REGISTER(lea_assistant_app, CONFIG_ZSW_LEA_ASSISTANT_APP_LOG_LEVEL);

// Functions needed for all applications
static void lea_assistant_app_start(lv_obj_t *root, lv_group_t *group);
static void lea_assistant_app_stop(void);

LV_IMG_DECLARE(auracast);

static application_t app = {
    .name = "LEA Assistant",
    .icon = &auracast,
    .start_func = lea_assistant_app_start,
    .stop_func = lea_assistant_app_stop
};

static devices_list_t devices_list = {
    .num = 0,
};

static void close_app(void)
{
    zsw_app_manager_app_close_request(&app);
}

static void on_source_selected(lea_assistant_device_t *device)
{
    LOG_DBG("Source %s selected", device->name);

    // Could use `MESSAGE_SUBTYPE_ADD_SOURCE`, but calling it directly so don't need to fill buffer and parse LTV
    add_source(device->sid, device->pa_interval, device->broadcast_id, &device->addr);
    close_app();
}

static void on_sink_selected(lea_assistant_device_t *device)
{
    LOG_DBG("Sink %s selected", device->name);

    devices_list.num = 0;

    message_handler(&(struct webusb_message ) {
        .sub_type = MESSAGE_SUBTYPE_STOP_SCAN
    }, 0);

    message_handler(&(struct webusb_message ) {
        .sub_type = MESSAGE_SUBTYPE_START_SOURCE_SCAN
    }, 0);

    // Could use `MESSAGE_SUBTYPE_CONNECT_SINK`, but calling it directly so don't need to fill buffer and parse LTV
    connect_to_sink(&device->addr);
    lea_assistant_ui_show_source(on_source_selected);
}

static void lea_assistant_app_start(lv_obj_t *root, lv_group_t *group)
{
    LOG_DBG("LEA Assistant app start");
    message_handler(&(struct webusb_message ) {
        .sub_type = MESSAGE_SUBTYPE_START_SINK_SCAN
    }, 0);
    lea_assistant_ui_show(root, on_sink_selected, close_app);
}

static void lea_assistant_app_stop(void)
{
    lea_assistant_ui_remove();
    message_handler(&(struct webusb_message ) {
        .sub_type = MESSAGE_SUBTYPE_STOP_SCAN
    }, 0);

    devices_list.num = 0;
}

static int lea_assistant_app_add(void)
{
    zsw_app_manager_add_application(&app);

    return 0;
}

void lea_assistant_app_add_entry(lea_assistant_device_t *device)
{
    for (size_t i = 0; i < devices_list.num; i++) {
        if (bt_addr_le_cmp(&device->addr, &devices_list.addr[i]) == 0) {
            LOG_DBG("Device already added (%s)", device->name);
            return;
        }
    }

    if (devices_list.num > MAX_DEVICES_NUM) {
        LOG_WRN("MAX devices reached");
        return;
    }

    bt_addr_le_copy(&devices_list.addr[devices_list.num], &device->addr);
    devices_list.num++;

    lea_assistant_ui_add_list_entry(device);
}

SYS_INIT(lea_assistant_app_add, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
