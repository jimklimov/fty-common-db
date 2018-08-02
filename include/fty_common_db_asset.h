/*  =========================================================================
    fty_common_db_asset - Asset related queries

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

#ifndef FTY_COMMON_DB_ASSET_H_INCLUDED
#define FTY_COMMON_DB_ASSET_H_INCLUDED

#ifdef __cplusplus
namespace DBAssets {

// id_to_name_ext_name: converts database id to internal name and extended (unicode) name
// returns empty pair of names if error
    std::pair <std::string, std::string>
    id_to_name_ext_name (uint32_t asset_id);

// extname_to_asset_id: converts extended name to database id
// returns value < 0 if error ocurres
    int64_t
    extname_to_asset_id (std::string asset_ext_name);

// name_to_extname: converts internal name to extended name
// returns value < 0 if error ocurres
    int
    name_to_extname (std::string asset_name, std::string &ext_name);

// name_to_asset_id: converts asset internal name to database id
// returns value < 0 if error ocurres
    int64_t
    name_to_asset_id (std::string asset_name);

} // namespace


void
fty_common_db_asset_test (bool verbose);

#endif //namespace
#endif // FTY_COMMON_DB_ASSET_H_INCLUDED
