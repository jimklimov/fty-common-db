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

// Note: Consumers MUST be built with C++11 or newer standard due to this:
#include <functional>
#include <inttypes.h>

#ifdef __cplusplus

namespace DBAssets {
typedef std::function<void(const tntdb::Row&)> row_cb_f ;

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

// extname_to_asset_name: converts asset name by the name in ext
// returns value < 0 if error ocurres
    int
    extname_to_asset_name (std::string asset_ext_name, std::string &asset_name);

// --------------------------------------------------------------------

// select_asset_element_super_parent: selects parents of given device
// returns 0 if succesful
// returns -1 if error occurs
    int
    select_asset_element_super_parent (tntdb::Connection& conn,
                                       uint32_t id,
                                       std::function<void(const tntdb::Row&)>& cb);

// select_asset_element_all_with_warranty_end: selects assets which
// are close to warranty expiration
// returns 0 if succesful
// returns -1 if error occurs

    int
    select_asset_element_all_with_warranty_end (tntdb::Connection& conn,
                                                std::function<void(const tntdb::Row&)>& cb);

// select_assets_by_container: selects assets from given container (DB, room, rack, ...)
// without - accepted values: "location", "powerchain" or empty string
// returns 0 if succesful
// returns -1 if error occurs

    int
    select_assets_by_container (tntdb::Connection &conn,
                                uint32_t element_id,
                                std::vector<uint16_t> types,
                                std::vector<uint16_t> subtypes,
                                std::string without,
                                std::string status,
                                std::function<void(const tntdb::Row&)> cb);

    int
    select_assets_by_container (tntdb::Connection &conn,
                                uint32_t element_id,
                                std::function<void(const tntdb::Row&)> cb,
                                std::string status);

    int
    select_assets_by_container (tntdb::Connection &conn,
                                uint32_t element_id,
                                std::function<void(const tntdb::Row&)> cb);

// select_assets_without_container: select all assets in all (or without) containers
// return 0 on success (even if nothing was found)
// returns -1 if error occurs
    int
    select_assets_without_container (tntdb::Connection &conn,
                                     std::vector<uint16_t> types,
                                     std::vector<uint16_t> subtypes,
                                     std::function<void(const tntdb::Row&)> cb);

// select_assets_all_container: selects all assets (with and wihout container)
// return 0 on success (even if nothing was found)
// returns -1 if error occurs
    int
    select_assets_all_container (tntdb::Connection &conn,
                                 std::vector<uint16_t> types,
                                 std::vector<uint16_t> subtypes,
                                 std::string without,
                                 std::string status,
                                 std::function<void(const tntdb::Row&)> cb);

// convert_asset_to_monitor: converts asset id to monitor id
// return  0 on success (even if counterpart was not found)
// returns -1 if error occurs
    int
    convert_asset_to_monitor (tntdb::Connection &conn,
                              uint32_t asset_element_id,
                              uint16_t &monitor_element_id);

// count_keytag: how many times is gived a couple keytag/value
// in t_bios_asset_ext_attributes
// returns -1 in case of error otherwise number of instance
    int
    count_keytag (tntdb::Connection& conn,
                  const std::string &keytag,
                  const std::string &value);

// unique_keytag: check if the pair (key, value) is unique
// return -1 in case of error
//         0 if there is no such pair in db yet
//         otherwise number of such pairs
    int
    unique_keytag (tntdb::Connection &conn,
                   const std::string &keytag,
                   const std::string &value,
                   uint32_t       element_id);

// max_number_of_power_links: select maximum number of power sources for device in the system
// return -1 in case of error otherwise number of power sources
    int
    max_number_of_power_links (tntdb::Connection& conn);

// count_of_link_src: how many times asset id occurs as id_asset_device_src
// return -1 in case of error otherwise number of used outlets
    int
    count_of_link_src (tntdb::Connection& conn,
                       uint32_t id);

// max_number_of_asset_groups: select maximal number of groups in the system
// return -1 in case of error otherwise number of groups
    int
    max_number_of_asset_groups (tntdb::Connection& conn);


// brief selects all group names for given element id
// returns -1 in case of error or 0 for success
    int
    select_group_names (tntdb::Connection& conn,
                        uint32_t id,
                        std::vector<std::string>& out);

//selects information about power links for given device id
// returns -1 in case of error or 0 for success
    int
    select_v_web_asset_power_link_src_byId (tntdb::Connection& conn,
                                            uint32_t id,
                                            row_cb_f& cb);

} // namespace


void
fty_common_db_asset_test (bool verbose);

#endif //namespace
#endif // FTY_COMMON_DB_ASSET_H_INCLUDED
