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
#ifndef _gps_app_H
    #define _gps_app_H

    #include <TTGO.h>

    #define GPS_JSON_CONFIG_FILE        "/gps.json"

    typedef struct {
        bool autoconnect = false;
    } gps_config_t;

//    #define EXAMPLE_WIDGET    // uncomment if an widget need

    void gps_app_setup( void );
    uint32_t gps_app_get_app_setup_tile_num( void );
    uint32_t gps_app_get_app_main_tile_num( void );
    gps_config_t *gps_get_config( void );
    void gps_save_config( void );
    void gps_load_config( void );
    
#endif // _gps_app_H