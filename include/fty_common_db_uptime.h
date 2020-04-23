/*  =========================================================================
    fty_common_db_uptime - Uptime support function.

    Copyright (C) 2014 - 2020 Eaton

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

#ifndef FTY_COMMON_DB_UPTIME_H_INCLUDED
#define FTY_COMMON_DB_UPTIME_H_INCLUDED

#ifdef __cplusplus
#include <vector>
#include <string>
#include <functional>
#include <tntdb/connect.h>
#include <tntdb/result.h>
#include <tntdb/row.h>
#include <tntdb/error.h>
#include <fty_common_asset_types.h>
#include <fty_common_db_dbpath.h>
#include <fty_log.h>
#include "fty_common_db_asset.h"

namespace DBUptime {
bool
get_dc_upses (const char *asset_name, zhash_t *hash);
}
#endif

#endif
