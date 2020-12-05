#include "config.h"
#include <TTGO.h>

#include "gps_app.h"
#include "gps_app_setup.h"

#include "gui/mainbar/mainbar.h"
#include "gui/statusbar.h"

lv_obj_t *gps_app_setup_tile = NULL;
lv_style_t gps_app_setup_style;

lv_obj_t *gps_app_foobar_switch = NULL;

LV_IMG_DECLARE(exit_32px);

void gps_app_setup_setup(uint32_t tile_num)
{

    gps_app_setup_tile = mainbar_get_tile_obj(tile_num);
    lv_style_copy(&gps_app_setup_style, mainbar_get_style());

    lv_style_set_bg_color(&gps_app_setup_style, LV_OBJ_PART_MAIN, LV_COLOR_BLACK);
    lv_style_set_bg_opa(&gps_app_setup_style, LV_OBJ_PART_MAIN, LV_OPA_100);
    lv_style_set_border_width(&gps_app_setup_style, LV_OBJ_PART_MAIN, 0);
    lv_obj_add_style(gps_app_setup_tile, LV_OBJ_PART_MAIN, &gps_app_setup_style);

    lv_obj_t *exit_cont = lv_obj_create(gps_app_setup_tile, NULL);
    lv_obj_set_size(exit_cont, lv_disp_get_hor_res(NULL), 40);
    lv_obj_add_style(exit_cont, LV_OBJ_PART_MAIN, &gps_app_setup_style);
    lv_obj_align(exit_cont, gps_app_setup_tile, LV_ALIGN_IN_TOP_MID, 0, 10);

    lv_obj_t *exit_btn = lv_imgbtn_create(exit_cont, NULL);
    lv_imgbtn_set_src(exit_btn, LV_BTN_STATE_RELEASED, &exit_32px);
    lv_imgbtn_set_src(exit_btn, LV_BTN_STATE_PRESSED, &exit_32px);
    lv_imgbtn_set_src(exit_btn, LV_BTN_STATE_CHECKED_RELEASED, &exit_32px);
    lv_imgbtn_set_src(exit_btn, LV_BTN_STATE_CHECKED_PRESSED, &exit_32px);
    lv_obj_add_style(exit_btn, LV_IMGBTN_PART_MAIN, &gps_app_setup_style);
    lv_obj_align(exit_btn, exit_cont, LV_ALIGN_IN_TOP_LEFT, 10, 0);
    lv_obj_set_event_cb(exit_btn, [](lv_obj_t *obj, lv_event_t event) {
        switch (event)
        {
        case (LV_EVENT_CLICKED):
            mainbar_jump_to_tilenumber(gps_app_get_app_main_tile_num(), LV_ANIM_ON);
            break;
        }
    });

    lv_obj_t *exit_label = lv_label_create(exit_cont, NULL);
    lv_obj_add_style(exit_label, LV_OBJ_PART_MAIN, &gps_app_setup_style);
    lv_label_set_text(exit_label, "gps setup");
    lv_obj_align(exit_label, exit_btn, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

    lv_obj_t *gps_app_foobar_switch_cont = lv_obj_create(gps_app_setup_tile, NULL);
    lv_obj_set_size(gps_app_foobar_switch_cont, lv_disp_get_hor_res(NULL), 40);
    lv_obj_add_style(gps_app_foobar_switch_cont, LV_OBJ_PART_MAIN, &gps_app_setup_style);
    lv_obj_align(gps_app_foobar_switch_cont, exit_cont, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

    gps_app_foobar_switch = lv_switch_create(gps_app_foobar_switch_cont, NULL);
    lv_obj_add_protect(gps_app_foobar_switch, LV_PROTECT_CLICK_FOCUS);
    lv_obj_add_style(gps_app_foobar_switch, LV_SWITCH_PART_INDIC, mainbar_get_switch_style());
    lv_switch_off(gps_app_foobar_switch, LV_ANIM_ON);
    lv_obj_align(gps_app_foobar_switch, gps_app_foobar_switch_cont, LV_ALIGN_IN_RIGHT_MID, -5, 0);
    lv_obj_set_event_cb(gps_app_foobar_switch, [](lv_obj_t *obj, lv_event_t event) {
        switch (event)
        {
        case (LV_EVENT_VALUE_CHANGED):
            Serial.printf("switch value = %d\r\n", lv_switch_get_state(obj));
            break;
        }
    });

    lv_obj_t *gps_app_foobar_switch_label = lv_label_create(gps_app_foobar_switch_cont, NULL);
    lv_obj_add_style(gps_app_foobar_switch_label, LV_OBJ_PART_MAIN, &gps_app_setup_style);
    lv_label_set_text(gps_app_foobar_switch_label, "Power");
    lv_obj_align(gps_app_foobar_switch_label, gps_app_foobar_switch_cont, LV_ALIGN_IN_LEFT_MID, 5, 0);
}