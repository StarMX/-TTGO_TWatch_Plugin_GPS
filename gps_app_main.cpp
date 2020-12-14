#include "config.h"
#include <TTGO.h>

#include "gps_app.h"
#include "gps_app_main.h"

#include "gui/mainbar/app_tile/app_tile.h"
#include "gui/mainbar/main_tile/main_tile.h"
#include "gui/mainbar/mainbar.h"
#include "gui/statusbar.h"

#include "hardware/powermgm.h"

lv_obj_t *gps_app_main_tile = NULL;
lv_style_t gps_app_main_style;

lv_style_t datestyle;

lv_obj_t *gps_btn_label;
// lv_obj_t *gps_location_value;
lv_obj_t *gps_latitude_value;
lv_obj_t *gps_longitude_value;
lv_obj_t *gps_satellites_value;
lv_obj_t *gps_speed_value;
lv_obj_t *gps_date_value;
lv_task_t *_gps_app_task;

LV_IMG_DECLARE(exit_32px);
LV_IMG_DECLARE(setup_32px);
LV_IMG_DECLARE(refresh_32px);
LV_FONT_DECLARE(Ubuntu_72px);
LV_FONT_DECLARE(Ubuntu_16px);

TinyGPSPlus *gps;

void gps_app_task(lv_task_t *task);

void gps_app_main_setup(uint32_t tile_num)
{
    gps_app_main_tile = mainbar_get_tile_obj(tile_num);
    lv_style_copy(&gps_app_main_style, mainbar_get_style());

    lv_obj_t *exit_btn = lv_imgbtn_create(gps_app_main_tile, NULL);
    lv_imgbtn_set_src(exit_btn, LV_BTN_STATE_RELEASED, &exit_32px);
    lv_imgbtn_set_src(exit_btn, LV_BTN_STATE_PRESSED, &exit_32px);
    lv_imgbtn_set_src(exit_btn, LV_BTN_STATE_CHECKED_RELEASED, &exit_32px);
    lv_imgbtn_set_src(exit_btn, LV_BTN_STATE_CHECKED_PRESSED, &exit_32px);
    lv_obj_add_style(exit_btn, LV_IMGBTN_PART_MAIN, &gps_app_main_style);
    lv_obj_align(exit_btn, gps_app_main_tile, LV_ALIGN_IN_BOTTOM_LEFT, 10, -10);
    lv_obj_set_event_cb(exit_btn, [](lv_obj_t *obj, lv_event_t event) {
        switch (event)
        {
        case (LV_EVENT_CLICKED):
            mainbar_jump_to_maintile(LV_ANIM_OFF);
            break;
        }
    });

    lv_obj_t *setup_btn = lv_imgbtn_create(gps_app_main_tile, NULL);
    lv_imgbtn_set_src(setup_btn, LV_BTN_STATE_RELEASED, &setup_32px);
    lv_imgbtn_set_src(setup_btn, LV_BTN_STATE_PRESSED, &setup_32px);
    lv_imgbtn_set_src(setup_btn, LV_BTN_STATE_CHECKED_RELEASED, &setup_32px);
    lv_imgbtn_set_src(setup_btn, LV_BTN_STATE_CHECKED_PRESSED, &setup_32px);
    lv_obj_add_style(setup_btn, LV_IMGBTN_PART_MAIN, &gps_app_main_style);
    lv_obj_align(setup_btn, gps_app_main_tile, LV_ALIGN_IN_BOTTOM_RIGHT, -10, -10);
    lv_obj_set_event_cb(setup_btn, [](lv_obj_t *obj, lv_event_t event) {
        switch (event)
        {
        case (LV_EVENT_CLICKED):
            statusbar_hide(true);
            mainbar_jump_to_tilenumber(gps_app_get_app_setup_tile_num(), LV_ANIM_ON);
            break;
        }
    });

    //
    lv_obj_t *gps_btn = lv_btn_create(gps_app_main_tile, NULL);
    // lv_obj_set_size(gps_btn, 50, 50);
    // lv_obj_add_style(gps_btn, LV_IMGBTN_PART_MAIN, &gps_app_main_style);
    lv_obj_align(gps_btn, gps_app_main_tile, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
    gps_btn_label = lv_label_create(gps_btn, NULL);
    lv_label_set_text(gps_btn_label, LV_SYMBOL_REFRESH);
    lv_obj_set_event_cb(gps_btn, [](lv_obj_t *obj, lv_event_t event) {
        switch (event)
        {
        case (LV_EVENT_CLICKED):
            lv_label_set_text(gps_btn_label, LV_SYMBOL_REFRESH);
            break;
        }
    });

    lv_style_set_text_opa(&gps_app_main_style, LV_OBJ_PART_MAIN, LV_OPA_20);
    lv_style_set_text_font(&gps_app_main_style, LV_STATE_DEFAULT, &Ubuntu_72px);
    lv_obj_t *app_label = lv_label_create(gps_app_main_tile, NULL);
    lv_label_set_text(app_label, "GPS");
    lv_obj_reset_style_list(app_label, LV_OBJ_PART_MAIN);
    lv_obj_add_style(app_label, LV_OBJ_PART_MAIN, &gps_app_main_style);
    lv_obj_align(app_label, gps_app_main_tile, LV_ALIGN_CENTER, 0, 0);

    //
    lv_style_copy(&datestyle, &gps_app_main_style);
    lv_style_set_text_font(&datestyle, LV_STATE_DEFAULT, &Ubuntu_16px);
    lv_style_set_text_opa(&datestyle, LV_OBJ_PART_MAIN, LV_OPA_COVER);
    // lv_obj_t *gps_location_cont = lv_obj_create(gps_app_main_tile, NULL);
    // lv_obj_set_size(gps_location_cont, lv_disp_get_hor_res(NULL), 25);
    // lv_obj_add_style(gps_location_cont, LV_OBJ_PART_MAIN, &datestyle);
    // lv_obj_align(gps_location_cont, gps_app_main_tile, LV_ALIGN_IN_TOP_RIGHT, 0, 35);
    // lv_obj_t *gps_location_label = lv_label_create(gps_location_cont, NULL);
    // lv_obj_add_style(gps_location_label, LV_OBJ_PART_MAIN, &datestyle);
    // lv_label_set_text(gps_location_label, "Location");
    // lv_obj_align(gps_location_label, gps_location_cont, LV_ALIGN_IN_LEFT_MID, 5, 0);
    // gps_location_value = lv_label_create(gps_location_cont, NULL);
    // lv_obj_add_style(gps_location_value, LV_OBJ_PART_MAIN, &datestyle);
    // lv_label_set_text(gps_location_value, "00.00,00.00");
    // lv_obj_align(gps_location_value, gps_location_cont, LV_ALIGN_IN_RIGHT_MID, -5, 0);

    //latitude
    lv_obj_t *gps_latitude_cont = lv_obj_create(gps_app_main_tile, NULL);
    lv_obj_set_size(gps_latitude_cont, lv_disp_get_hor_res(NULL), 25);
    lv_obj_add_style(gps_latitude_cont, LV_OBJ_PART_MAIN, &datestyle);
    lv_obj_align(gps_latitude_cont, gps_app_main_tile, LV_ALIGN_IN_TOP_RIGHT, 0, 35);
    lv_obj_t *gps_latitude_label = lv_label_create(gps_latitude_cont, NULL);
    lv_obj_add_style(gps_latitude_label, LV_OBJ_PART_MAIN, &datestyle);
    lv_label_set_text(gps_latitude_label, "latitude");
    lv_obj_align(gps_latitude_label, gps_latitude_cont, LV_ALIGN_IN_LEFT_MID, 5, 0);
    gps_latitude_value = lv_label_create(gps_latitude_cont, NULL);
    lv_obj_add_style(gps_latitude_value, LV_OBJ_PART_MAIN, &datestyle);
    lv_label_set_text(gps_latitude_value, "unknown");
    lv_obj_align(gps_latitude_value, gps_latitude_cont, LV_ALIGN_IN_RIGHT_MID, -5, 0);

    //longitude
    lv_obj_t *gps_longitude_cont = lv_obj_create(gps_app_main_tile, NULL);
    lv_obj_set_size(gps_longitude_cont, lv_disp_get_hor_res(NULL), 22);
    lv_obj_add_style(gps_longitude_cont, LV_OBJ_PART_MAIN, &datestyle);
    lv_obj_align(gps_longitude_cont, gps_latitude_cont, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_t *gps_longitude_label = lv_label_create(gps_longitude_cont, NULL);
    lv_obj_add_style(gps_longitude_label, LV_OBJ_PART_MAIN, &datestyle);
    lv_label_set_text(gps_longitude_label, "longitude");
    lv_obj_align(gps_longitude_label, gps_longitude_cont, LV_ALIGN_IN_LEFT_MID, 5, 0);
    gps_longitude_value = lv_label_create(gps_longitude_cont, NULL);
    lv_obj_add_style(gps_longitude_value, LV_OBJ_PART_MAIN, &datestyle);
    lv_label_set_text(gps_longitude_value, "unknown");
    lv_obj_align(gps_longitude_value, gps_longitude_cont, LV_ALIGN_IN_RIGHT_MID, -5, 0);

    //satellites
    lv_obj_t *gps_satellites_cont = lv_obj_create(gps_app_main_tile, NULL);
    lv_obj_set_size(gps_satellites_cont, lv_disp_get_hor_res(NULL), 22);
    lv_obj_add_style(gps_satellites_cont, LV_OBJ_PART_MAIN, &datestyle);
    lv_obj_align(gps_satellites_cont, gps_longitude_cont, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_t *gps_satellites_label = lv_label_create(gps_satellites_cont, NULL);
    lv_obj_add_style(gps_satellites_label, LV_OBJ_PART_MAIN, &datestyle);
    lv_label_set_text(gps_satellites_label, "satellite");
    lv_obj_align(gps_satellites_label, gps_satellites_cont, LV_ALIGN_IN_LEFT_MID, 5, 0);
    gps_satellites_value = lv_label_create(gps_satellites_cont, NULL);
    lv_obj_add_style(gps_satellites_value, LV_OBJ_PART_MAIN, &datestyle);
    lv_label_set_text(gps_satellites_value, "unknown");
    lv_obj_align(gps_satellites_value, gps_satellites_cont, LV_ALIGN_IN_RIGHT_MID, -5, 0);

    //speed
    lv_obj_t *gps_speed_cont = lv_obj_create(gps_app_main_tile, NULL);
    lv_obj_set_size(gps_speed_cont, lv_disp_get_hor_res(NULL), 22);
    lv_obj_add_style(gps_speed_cont, LV_OBJ_PART_MAIN, &datestyle);
    lv_obj_align(gps_speed_cont, gps_satellites_cont, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_t *gps_speed_label = lv_label_create(gps_speed_cont, NULL);
    lv_obj_add_style(gps_speed_label, LV_OBJ_PART_MAIN, &datestyle);
    lv_label_set_text(gps_speed_label, "speed");
    lv_obj_align(gps_speed_label, gps_speed_cont, LV_ALIGN_IN_LEFT_MID, 5, 0);
    gps_speed_value = lv_label_create(gps_speed_cont, NULL);
    lv_obj_add_style(gps_speed_value, LV_OBJ_PART_MAIN, &datestyle);
    lv_label_set_text(gps_speed_value, "unknown");
    lv_obj_align(gps_speed_value, gps_speed_cont, LV_ALIGN_IN_RIGHT_MID, -5, 0);

    //data
    lv_obj_t *gps_date_cont = lv_obj_create(gps_app_main_tile, NULL);
    lv_obj_set_size(gps_date_cont, lv_disp_get_hor_res(NULL), 22);
    lv_obj_add_style(gps_date_cont, LV_OBJ_PART_MAIN, &datestyle);
    lv_obj_align(gps_date_cont, gps_speed_cont, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_t *gps_date_label = lv_label_create(gps_date_cont, NULL);
    lv_obj_add_style(gps_date_label, LV_OBJ_PART_MAIN, &datestyle);
    lv_label_set_text(gps_date_label, "datetime");
    lv_obj_align(gps_date_label, gps_date_cont, LV_ALIGN_IN_LEFT_MID, 5, 0);
    gps_date_value = lv_label_create(gps_date_cont, NULL);
    lv_obj_add_style(gps_date_value, LV_OBJ_PART_MAIN, &datestyle);
    lv_label_set_text(gps_date_value, "unknown");
    lv_obj_align(gps_date_value, gps_date_cont, LV_ALIGN_IN_RIGHT_MID, -5, 0);

    mainbar_add_tile_activate_cb(tile_num, [](void) {
        // TTGOClass *ttgo = TTGOClass::getWatch();
        // ttgo->enableLDO3();
        _gps_app_task = lv_task_create(gps_app_task, 1000, LV_TASK_PRIO_MID, NULL);
    });
    mainbar_add_tile_hibernate_cb(tile_num, [](void) {
        if (!gps_get_config()->nohup)
        {
            // TTGOClass *ttgo = TTGOClass::getWatch();
            // ttgo->enableLDO3(false);
            gps_app_show_indicator(false);
        }
        else
            gps_app_show_indicator();
        lv_task_del(_gps_app_task);
    });


#ifndef ENABLE_SOUND
    powermgm_register_cb(
        POWERMGM_SILENCE_WAKEUP | POWERMGM_STANDBY | POWERMGM_WAKEUP, [](EventBits_t event, void *arg) {
            if (gps_get_config()->nohup)
            {
                TTGOClass *ttgo = TTGOClass::getWatch();
                switch (event)
                {
                case POWERMGM_STANDBY:
                    ttgo->enableLDO3(false);
                    break;
                case POWERMGM_SILENCE_WAKEUP:
                case POWERMGM_WAKEUP:
                    ttgo->enableLDO3();
                    break;
                }
            }
            return (true);
        },
        "gps");
#endif

#ifndef ENABLE_SOUND
    powermgm_register_loop_cb(
        POWERMGM_WAKEUP | POWERMGM_SILENCE_WAKEUP | POWERMGM_STANDBY, [](EventBits_t event, void *arg) {
            if (gps_get_config()->nohup)
            {
                TTGOClass *ttgo = TTGOClass::getWatch();
                switch (event)
                {
                case POWERMGM_STANDBY:
                    ttgo->enableLDO3(false);
                    break;
                case POWERMGM_SILENCE_WAKEUP:
                case POWERMGM_WAKEUP:
                    ttgo->enableLDO3();
                    break;
                }
            }
            return (true);
        },
        "gps loop");
#endif
    // create an task that runs every secound
    // _gps_app_task = lv_task_create(gps_app_task, 1000, LV_TASK_PRIO_MID, NULL);
}

void gps_app_task(lv_task_t *task)
{
    TTGOClass *ttgo = TTGOClass::getWatch();
    gps = ttgo->gps;
    if (ttgo->gpsHandler())
    {
        
        char gps_value_temp[30] = "";
        if (gps->satellites.isValid() /* || gps->satellites.isUpdated()*/)
        {
            snprintf(gps_value_temp, sizeof(gps_value_temp), "%d", gps->satellites.value());
            lv_label_set_text(gps_satellites_value, gps_value_temp);
            lv_obj_align(gps_satellites_value, lv_obj_get_parent(gps_satellites_value), LV_ALIGN_IN_RIGHT_MID, -5, 0);
            log_i("衛星數 -> %d|%d", gps->satellites.age(), gps->satellites.value());
        }
        if (gps->location.isValid() /* && gps->location.isUpdated()*/)
        {
            snprintf(gps_value_temp, sizeof(gps_value_temp), "%.4f", gps->location.lat());
            lv_label_set_text(gps_latitude_value, gps_value_temp);
            lv_obj_align(gps_latitude_value, lv_obj_get_parent(gps_latitude_value), LV_ALIGN_IN_RIGHT_MID, -5, 0);

            snprintf(gps_value_temp, sizeof(gps_value_temp), "%.4f", gps->location.lng());
            lv_label_set_text(gps_longitude_value, gps_value_temp);
            lv_obj_align(gps_longitude_value, lv_obj_get_parent(gps_longitude_value), LV_ALIGN_IN_RIGHT_MID, -5, 0);
            lv_label_set_text(gps_btn_label, LV_SYMBOL_GPS);
        }else{
            lv_label_set_text(gps_btn_label, LV_SYMBOL_REFRESH);
        }
        log_i("lat: %.2f lng: %.2f", gps->location.lat(), gps->location.lng());
        if (gps->date.isValid() /* || gps->date.isUpdated()*/)
        {
            if (gps->time.isValid())
                snprintf(gps_value_temp, sizeof(gps_value_temp), "%d-%02d-%02d %02d:%02d:%02d", gps->date.year(), gps->date.month(), gps->date.day(), gps->time.hour(), gps->time.minute(), gps->time.second());
            else
                snprintf(gps_value_temp, sizeof(gps_value_temp), "%d-%02d-%02d", gps->date.year(), gps->date.month(), gps->date.day());
            lv_label_set_text(gps_date_value, gps_value_temp);
            lv_obj_align(gps_date_value, lv_obj_get_parent(gps_date_value), LV_ALIGN_IN_RIGHT_MID, -5, 0);
            log_i("時間: %s", gps_value_temp);
        }
        if (gps->altitude.isValid() /* && gps->altitude.isUpdated()*/)
        {
            log_i("高度: %.2f", gps->altitude.meters());
        }
        if (gps->speed.isValid() /* && gps->speed.isUpdated()*/)
        {
            snprintf(gps_value_temp, sizeof(gps_value_temp), "%.2f kmp/h", gps->speed.kmph());
            lv_label_set_text(gps_speed_value, gps_value_temp);
            lv_obj_align(gps_speed_value, lv_obj_get_parent(gps_speed_value), LV_ALIGN_IN_RIGHT_MID, -5, 0);
            log_i("速度: %s", gps_value_temp);
        }
    }
    else
    {
        log_i("No GPS detected: check wiring.");
        lv_label_set_text(gps_btn_label, LV_SYMBOL_WARNING);
    }
}