#include "config.h"
#include <TTGO.h>

#include "gps_app.h"
#include "gps_app_main.h"
#include "gps_app_setup.h"

#include "gui/mainbar/mainbar.h"
#include "gui/statusbar.h"
#include "gui/app.h"
#include "gui/widget.h"


#include "hardware/json_psram_allocator.h"

gps_config_t gps_config;

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
    gps_load_config();

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



gps_config_t *gps_get_config( void ) {
    return( &gps_config );
}

void gps_load_config( void ){
        fs::File file = SPIFFS.open( GPS_JSON_CONFIG_FILE, FILE_READ );
    if (!file) {
        log_e("Can't open file: %s!", GPS_JSON_CONFIG_FILE );
    }
    else {
        int filesize = file.size();
        SpiRamJsonDocument doc( filesize * 4 );
        DeserializationError error = deserializeJson( doc, file );
        if ( error ) {
            log_e("update check deserializeJson() failed: %s", error.c_str() );
        }
        else {
            // strlcpy( gps_config.string, doc["gps"]["string"], sizeof( gps_config.string ) );
            gps_config.autoconnect = doc["gps"]["autoconnect"] | false;
        }        
        doc.clear();
    }
    file.close();
}


void gps_save_config( void ) {
    fs::File file = SPIFFS.open( GPS_JSON_CONFIG_FILE, FILE_WRITE );

    if (!file) {
        log_e("Can't open file: %s!", GPS_JSON_CONFIG_FILE );
    }
    else {
        SpiRamJsonDocument doc( 1000 );
        doc["gps"]["autoconnect"] = gps_config.autoconnect;
        if ( serializeJsonPretty( doc, file ) == 0) {
            log_e("Failed to write config file");
        }
        doc.clear();
    }
    file.close();
}
