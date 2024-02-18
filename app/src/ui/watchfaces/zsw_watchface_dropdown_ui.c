#include "zsw_ui.h"
#include "lvgl.h"
#include "drivers/zsw_display_control.h"

//drop down
lv_obj_t * ui_DropDownPanel;
lv_obj_t * ui_UpBGPanel;
lv_obj_t * ui_NFCButton;
lv_obj_t * ui_NFCLabel;
lv_obj_t * ui_BLEButton;
lv_obj_t * ui_BLELabel;
lv_obj_t * ui_PowerButton;
lv_obj_t * ui_PowerLabel;
lv_obj_t * ui_SetButton;
lv_obj_t * ui_SetLabel;
lv_obj_t * ui_LightSlider;
lv_obj_t * ui_LightLabel;
lv_obj_t * ui_DownBGPanel;

uint8_t ui_LightSliderValue = 30;

static bool is_shown;

LV_FONT_DECLARE(ui_font_iconfont34);

void ui_event_LightSlider(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    //lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_VALUE_CHANGED)
    {
        ui_LightSliderValue = lv_slider_get_value(ui_LightSlider);
        zsw_display_control_set_brightness(ui_LightSliderValue);
        printk("ui_LightSliderValue: %d\n", ui_LightSliderValue);
        //LCD_Set_Light(ui_LightSliderValue);
    }
}

static void on_lvgl_screen_gesture_event_callback(lv_event_t *e)
{
    lv_dir_t  dir;
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_GESTURE) {
        dir = lv_indev_get_gesture_dir(lv_indev_get_act());
         switch (dir) {
            case LV_DIR_BOTTOM: {
                if (!is_shown) {
                    lv_obj_clear_flag(ui_DropDownPanel, LV_OBJ_FLAG_HIDDEN);
                    is_shown = true;
                }
                break;
            }
        }
    }
}

static void on_lvgl_screen_gesture_event_callback_drop(lv_event_t *e)
{
    lv_dir_t  dir;
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_GESTURE) {
        dir = lv_indev_get_gesture_dir(lv_indev_get_act());
         switch (dir) {
            case LV_DIR_TOP: {
                if (is_shown) {
                    lv_obj_add_flag(ui_DropDownPanel, LV_OBJ_FLAG_HIDDEN);
                    is_shown = false;
                }
                break;
            }
        }
    }
}

void zsw_watchface_dropdown_ui_add(lv_obj_t *root_page)
{
    __ASSERT(ui_DropDownPanel == NULL, "ui_DropDownPanel is not NULL");
    ui_DropDownPanel = lv_obj_create(root_page);
    lv_obj_set_width(ui_DropDownPanel, 240);
    lv_obj_set_height(ui_DropDownPanel, 240);
    lv_obj_set_x(ui_DropDownPanel, 0);
    lv_obj_set_y(ui_DropDownPanel, -10);
    lv_obj_set_align(ui_DropDownPanel, LV_ALIGN_TOP_MID);
    //lv_obj_scroll_by(ui_DropDownPanel,0, -160, LV_ANIM_OFF);
    lv_obj_clear_flag(ui_DropDownPanel, LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_CHAIN | LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    //lv_obj_clear_flag(ui_DropDownPanel, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_CHAIN);      /// Flags
    lv_obj_set_scroll_dir(ui_DropDownPanel, LV_DIR_VER);
    lv_obj_set_style_bg_color(ui_DropDownPanel, lv_color_hex(0x323232), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_DropDownPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_DropDownPanel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_DropDownPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_DropDownPanel, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_DropDownPanel, 0, LV_PART_SCROLLBAR | LV_STATE_SCROLLED);
    lv_obj_add_flag(ui_DropDownPanel, LV_OBJ_FLAG_HIDDEN);

    ui_UpBGPanel = lv_obj_create(ui_DropDownPanel);
    
    lv_obj_set_width(ui_UpBGPanel, 240);
    lv_obj_set_height(ui_UpBGPanel, 160);
    lv_obj_set_x(ui_UpBGPanel, 0);
    lv_obj_set_y(ui_UpBGPanel, -10);
    lv_obj_set_align(ui_UpBGPanel, LV_ALIGN_TOP_MID);
    lv_obj_clear_flag(ui_UpBGPanel, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_UpBGPanel, lv_color_hex(0x323232), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_UpBGPanel, 200, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_NFCButton = lv_btn_create(ui_DropDownPanel);
    lv_obj_set_width(ui_NFCButton, 50);
    lv_obj_set_height(ui_NFCButton, 50);
	lv_obj_set_x(ui_NFCButton, 0);
    lv_obj_set_y(ui_NFCButton, 35);
    lv_obj_add_flag(ui_NFCButton, LV_OBJ_FLAG_CHECKABLE | LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_NFCButton, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_NFCButton, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_NFCButton, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_NFCButton, lv_color_hex(0x3264C8), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(ui_NFCButton, 255, LV_PART_MAIN | LV_STATE_CHECKED);
    {lv_obj_add_state(ui_NFCButton, LV_STATE_CHECKED);}

    ui_NFCLabel = lv_label_create(ui_NFCButton);
    lv_obj_set_width(ui_NFCLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_NFCLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_NFCLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_NFCLabel, "");
    lv_obj_set_style_text_color(ui_NFCLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_NFCLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_NFCLabel, &ui_font_iconfont34, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_BLEButton = lv_btn_create(ui_DropDownPanel);
    lv_obj_set_width(ui_BLEButton, 50);
    lv_obj_set_height(ui_BLEButton, 50);
    lv_obj_set_x(ui_BLEButton, 0);
    lv_obj_set_y(ui_BLEButton, 95);
    lv_obj_add_flag(ui_BLEButton, LV_OBJ_FLAG_CHECKABLE | LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_BLEButton, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_BLEButton, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_BLEButton, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_BLEButton, lv_color_hex(0x3264C8), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(ui_BLEButton, 255, LV_PART_MAIN | LV_STATE_CHECKED);
    {lv_obj_add_state(ui_BLEButton, LV_STATE_CHECKED);}

    ui_BLELabel = lv_label_create(ui_BLEButton);
    lv_obj_set_width(ui_BLELabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_BLELabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_BLELabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_BLELabel, "");
    lv_obj_set_style_text_color(ui_BLELabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_BLELabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_BLELabel, &ui_font_iconfont34, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PowerButton = lv_btn_create(ui_DropDownPanel);
    lv_obj_set_width(ui_PowerButton, 50);
    lv_obj_set_height(ui_PowerButton, 50);
	lv_obj_set_x(ui_PowerButton, 0);
    lv_obj_set_y(ui_PowerButton, 35);
    lv_obj_set_align(ui_PowerButton, LV_ALIGN_TOP_MID);
    lv_obj_add_flag(ui_PowerButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_PowerButton, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_PowerButton, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PowerButton, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PowerLabel = lv_label_create(ui_PowerButton);
    lv_obj_set_width(ui_PowerLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_PowerLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_PowerLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_PowerLabel, "");
    lv_obj_set_style_text_color(ui_PowerLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_PowerLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_PowerLabel, &ui_font_iconfont34, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_SetButton = lv_btn_create(ui_DropDownPanel);
    lv_obj_set_width(ui_SetButton, 50);
    lv_obj_set_height(ui_SetButton, 50);
    lv_obj_set_x(ui_SetButton, 0);
    lv_obj_set_y(ui_SetButton, 95);
    lv_obj_set_align(ui_SetButton, LV_ALIGN_TOP_MID);
    lv_obj_add_flag(ui_SetButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_SetButton, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_SetButton, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_SetButton, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_SetLabel = lv_label_create(ui_SetButton);
    lv_obj_set_width(ui_SetLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_SetLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_SetLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_SetLabel, "");
    lv_obj_set_style_text_color(ui_SetLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_SetLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_SetLabel, &ui_font_iconfont34, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LightSlider = lv_slider_create(ui_DropDownPanel);
    lv_slider_set_value(ui_LightSlider, ui_LightSliderValue, LV_ANIM_OFF);
    if(lv_slider_get_mode(ui_LightSlider) == LV_SLIDER_MODE_RANGE) lv_slider_set_left_value(ui_LightSlider, 0, LV_ANIM_OFF);
    lv_obj_set_width(ui_LightSlider, 50);
    lv_obj_set_height(ui_LightSlider, 110);
	lv_obj_set_x(ui_LightSlider, 0);
    lv_obj_set_y(ui_LightSlider, 35);
    lv_obj_set_align(ui_LightSlider, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_style_radius(ui_LightSlider, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_LightSlider, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_LightSlider, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_radius(ui_LightSlider, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_LightSlider, lv_color_hex(0x3264C8), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_LightSlider, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(ui_LightSlider, lv_color_hex(0x000000), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_LightSlider, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_clear_flag(ui_LightSlider, LV_OBJ_FLAG_GESTURE_BUBBLE);      /// Flags


    ui_LightLabel = lv_label_create(ui_LightSlider);
    lv_obj_set_width(ui_LightLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LightLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LightLabel, 0);
    lv_obj_set_y(ui_LightLabel, 30);
    lv_obj_set_align(ui_LightLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LightLabel, "");
    lv_obj_set_style_text_color(ui_LightLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LightLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LightLabel, &ui_font_iconfont34, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_DownBGPanel = lv_obj_create(ui_DropDownPanel);
    lv_obj_set_width(ui_DownBGPanel, 240);
    lv_obj_set_height(ui_DownBGPanel, 160);
    lv_obj_set_x(ui_DownBGPanel, 0);
    lv_obj_set_y(ui_DownBGPanel, 150);
    lv_obj_set_align(ui_DownBGPanel, LV_ALIGN_TOP_MID);
    lv_obj_clear_flag(ui_DownBGPanel, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_DownBGPanel, lv_color_hex(0x323232), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_DownBGPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_DownBGPanel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_DownBGPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
		
		//events
    //lv_obj_add_event_cb(ui_HomePage, ui_event_HomePage, LV_EVENT_ALL, NULL);
    //lv_obj_add_event_cb(ui_NFCButton, ui_event_NFCButton, LV_EVENT_ALL, NULL);
    //lv_obj_add_event_cb(ui_BLEButton, ui_event_BLEButton, LV_EVENT_ALL, NULL);
    //lv_obj_add_event_cb(ui_PowerButton, ui_event_PowerButton, LV_EVENT_ALL, NULL);
    //lv_obj_add_event_cb(ui_SetButton, ui_event_SetButton, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_LightSlider,ui_event_LightSlider, LV_EVENT_ALL, NULL);

    lv_obj_add_event_cb(root_page, on_lvgl_screen_gesture_event_callback, LV_EVENT_GESTURE, NULL);
    lv_obj_add_event_cb(ui_DropDownPanel, on_lvgl_screen_gesture_event_callback_drop, LV_EVENT_GESTURE, NULL);
}

void zsw_watchface_dropdown_ui_remove(void)
{
    lv_obj_del(ui_DropDownPanel);
    ui_DropDownPanel = NULL;
}