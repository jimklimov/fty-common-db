/*  =========================================================================
    fty_common_db_asset_update - Functions updating assets in database.

    Copyright (C) 2014 - 2018 Eaton

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
    =========================================================================
*/

#ifndef FTY_COMMON_DB_ASSET_UPDATE_H_INCLUDED
#define FTY_COMMON_DB_ASSET_UPDATE_H_INCLUDED

#include <tntdb/connect.h>

namespace DBAssetsUpdate {

// update_asset_element:
// name would not be updated, but to provide a functionality for future
// parameter was added

    int
    update_asset_element (tntdb::Connection &conn,
                          uint32_t element_id,
                          const char *element_name,
                          uint32_t parent_id,
                          const char *status,
                          uint16_t priority,
                          const char *asset_tag,
                          int32_t &affected_rows);

// update_asset_status_by_name: updates asset status
    int
    update_asset_status_by_name (const char *element_name,
                                const char *status);

} // end namespace
#endif
