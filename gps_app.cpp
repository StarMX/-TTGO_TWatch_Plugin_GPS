/****************************************************************************
 *   Aug 3 12:17:11 2020
 *   Copyright  2020  Dirk Brosswick
 *   Email: dirk.brosswick@googlemail.com
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
#include "config.h"
#include <TTGO.h>

#include "gps_app.h"
#include "gps_app_main.h"
#include "gps_app_setup.h"

#include "gui/mainbar/mainbar.h"
#include "gui/statusbar.h"
#include "gui/app.h"
#include "gui/widget.h"

uint32_t gps_app_main_tile_num;
uint32_t gps_app_setup_tile_num;

// app icon
icon_t *gps_app = NULL;

// widget icon
icon_t *gps_widget = NULL;

// declare you images or fonts you need
LV_IMG_DECLARE(gps_app_64px);
LV_IMG_DECLARE(info_1_16px);

/*
 * setup routine for gps app
 */
void gps_app_setup(void)
{
    TTGOClass *ttgo = TTGOClass::getWatch();
    ttgo->gps_begin();
    // register 2 vertical tiles and get the first tile number and save it for later use
    gps_app_main_tile_num = mainbar_add_app_tile(1, 2, "gps app");
    gps_app_setup_tile_num = gps_app_main_tile_num + 1;

    // register app icon on the app tile
    // set your own icon and register her callback to activate by an click
    // remember, an app icon must have an size of 64x64 pixel with an alpha channel
    // use https://lvgl.io/tools/imageconverter to convert your images and set "true color with alpha" to get fancy images
    // the resulting c-file can put in /app/gpss/images/ and declare it like LV_IMG_DECLARE( your_icon );
    gps_app = app_register("gps", &gps_app_64px, [](lv_obj_t *obj, lv_event_t event) {
        switch (event)
        {
        case (LV_EVENT_CLICKED):
            statusbar_hide(true);
            app_hide_indicator(gps_app);
            mainbar_jump_to_tilenumber(gps_app_main_tile_num, LV_ANIM_OFF);
            break;
        }
    });
    app_set_indicator(gps_app, ICON_INDICATOR_OK);

#ifdef gps_WIDGET

    // register widget icon on the main tile
    // set your own icon and register her callback to activate by an click
    // remember, an widget icon must have an max size of 64x64 pixel
    // use https://lvgl.io/tools/imageconverter to convert your images and set "true color with alpha" to get fancy images
    // the resulting c-file can put in /app/gpss/images/ and declare it like LV_IMG_DECLARE( your_icon );
    gps_widget = widget_register("gps", &gps_app_64px, [](lv_obj_t *obj, lv_event_t event) {
        switch (event)
        {
        case (LV_EVENT_CLICKED):
            statusbar_hide(true);
            widget_hide_indicator(gps_widget);
            mainbar_jump_to_tilenumber(gps_app_main_tile_num, LV_ANIM_OFF);
            break;
        }
    });
    widget_set_indicator(gps_widget, ICON_INDICATOR_UPDATE);
#endif // gps_WIDGET
    // init main and setup tile, see gps_app_main.cpp and gps_app_setup.cpp
    gps_app_main_setup(gps_app_main_tile_num);
    gps_app_setup_setup(gps_app_setup_tile_num);
}

/*
 *
 */
uint32_t gps_app_get_app_main_tile_num(void)
{
    return (gps_app_main_tile_num);
}

/*
 *
 */
uint32_t gps_app_get_app_setup_tile_num(void)
{
    return (gps_app_setup_tile_num);
}
