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
#include <tntdb.h>
#include <czmq.h>

#ifdef __cplusplus
#define INPUT_POWER_CHAIN     1

// all fields called name
#define MAX_NAME_LENGTH         50
// t_bios_asset_ext_attributes.keytag
#define MAX_KEYTAG_LENGTH       40
// t_bios_asset_ext_attributes.value
#define MAX_VALUE_LENGTH        255
// t_bios_asset_device.mac
#define MAX_MAC_LENGTH          17
// t_bios_asset_device.hostname
#define MAX_HOSTNAME_LENGTH     25
// t_bios_asset_device.fullhostname
#define MAX_FULLHOSTNAME_LENGTH 45
#define MAX_DESCRIPTION_LENGTH  255

typedef std::function<void(const tntdb::Row&)> row_cb_f ;

template <typename T>
struct db_reply{
    int status; // ok/fail
    int errtype;
    int errsubtype;
    uint64_t rowid;  // insert/update or http error code if status == 0
    uint64_t affected_rows; // for update/insert/delete
    std::string msg;
    zhash_t *addinfo;
    T item;
};

typedef db_reply<uint64_t> db_reply_t;

inline db_reply_t db_reply_new() {
    db_reply_t val;
    val.status = 1;
    val.errtype = 0;
    val.errsubtype = 0;
    val.rowid = 0;
    val.affected_rows = 0;
    val.msg = "";
    val.addinfo = NULL;
    val.item = 0;

    return val;
}

template <typename T>
inline db_reply<T> db_reply_new(T& item) {
    db_reply<T> val;
    val.status = 1;
    val.errtype = 0;
    val.errsubtype = 0;
    val.rowid = 0;
    val.affected_rows = 0;
    val.msg = "";
    val.addinfo = NULL;
    val.item = item;
    return val;
}

struct db_web_basic_element_t {
    uint32_t    id;
    std::string name;
    std::string status;
    uint16_t    priority;
    uint16_t    type_id;
    std::string type_name;
    uint32_t    parent_id;
    uint16_t    parent_type_id;
    uint16_t    subtype_id;
    // TODO location
    std::string subtype_name;
    std::string asset_tag;
    std::string parent_name;
};

struct db_tmp_link_t {
    uint32_t         src_id;
    uint32_t         dest_id;
    std::string      src_name;
    std::string      src_socket;
    std::string      dest_socket;
};

struct db_web_element_t{
    db_web_basic_element_t basic;
    std::map <uint32_t, std::string> groups;
    std::vector <db_tmp_link_t> powers;
    std::map <std::string, std::pair<std::string, bool> > ext;
    std::vector <std::tuple <uint32_t, std::string, std::string, std::string>> parents;        // list of parents (id, name)
};

struct db_a_elmnt_t {
    uint32_t         id;
    std::string      name;
    std::string      status;
    uint32_t         parent_id;
    uint16_t         priority;
    uint16_t         type_id;
    uint16_t         subtype_id;
    std::string      asset_tag;
    std::map <std::string, std::string> ext;

    db_a_elmnt_t () :
        id{},
        name{},
        status{},
        parent_id{},
        priority{},
        type_id{},
        subtype_id{},
        asset_tag{},
        ext{}
    {}

    db_a_elmnt_t (
        uint32_t         id,
        std::string      name,
        std::string      status,
        uint32_t         parent_id,
        uint8_t          priority,
        uint16_t         type_id,
        uint16_t         subtype_id,
        std::string      asset_tag) :

        id(id),
        name(name),
        status(status),
        parent_id(parent_id),
        priority(priority),
        type_id(type_id),
        subtype_id(subtype_id),
        asset_tag(asset_tag),
        ext{}
    {}
};

// --------------------------------------------------------------------
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

// select_assets_by_container_name_filter: select assets of given types/subtypes from container with a given name
// return 0 on success (even if nothing was found)
// returns -1 if error occurs
    int
    select_assets_by_container_name_filter (tntdb::Connection &conn,
                                            const std::string& container_name,
                                            const std::set <std::string>& filter,
                                            std::vector <std::string>& assets);

// select_assets_by_filter: select assets of given types/subtypes from v_bios_asset_element_super_parent
// return 0 on success (even if nothing was found)
// returns -1 if error occurs
    int
    select_assets_by_filter (tntdb::Connection &conn,
                             const std::set<std::string> &types_and_subtypes,
                             std::vector <std::string>& assets);

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

// select_asset_element_all: select everything from v_web_element
// returns -1 in case of error or 0 for success
    int
    select_asset_element_all (tntdb::Connection& conn,
                              std::function<void(const tntdb::Row&)>& cb);

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


// select_group_names: selects all group names for given element id
// returns -1 in case of error or 0 for success
    int
    select_group_names (tntdb::Connection& conn,
                        uint32_t id,
                        std::vector<std::string>& out);

//select_v_web_asset_power_link_src_byId: selects information about power links for given device id
// returns -1 in case of error or 0 for success
    int
    select_v_web_asset_power_link_src_byId (tntdb::Connection& conn,
                                            uint32_t id,
                                            row_cb_f& cb);

// select_asset_ext_attribute_by_keytag: select all information about ext attribute based on keytag and asset_id[s]
// returns -1 in case of error, 0 on success (even if nothing was found)
    int
    select_asset_ext_attribute_by_keytag (tntdb::Connection &conn,
                                          const std::string &keytag,
                                          const std::set<uint32_t> &element_ids,
                                          std::function<void( const tntdb::Row& )> &cb);

// select_ext_rw_attributes_keytags: select all read-write ext attributes
// returns -1 in case of error or 0 for success
    int
    select_ext_rw_attributes_keytags (tntdb::Connection& conn,
                                      std::function<void(const tntdb::Row&)>& cb);

// select_ext_attributes: select all ext_attributes of asset
// returns -1 in case of error or 0 for success
    db_reply <std::map <std::string, std::pair<std::string, bool>>>
    select_ext_attributes (tntdb::Connection &conn,
                           uint32_t element_id);
    int
    select_ext_attributes (tntdb::Connection &conn,
                           uint32_t element_id,
                           std::map <std::string, std::pair<std::string, bool> >& out);

// select_ext_attributes_cb: select ass ext attributes of asset, process with cb
// returns -1 in case of error or 0 for success
    int
    select_ext_attributes_cb (tntdb::Connection &conn,
                           uint32_t asset_id,
                           std::function<void(const tntdb::Row&)> cb);
// select_asset_element_basic_cb: select all data about asset from v_web_element, process with cb
// return -1 in case of error or asset not found, 0 otherwise

    int
    select_asset_element_basic_cb (tntdb::Connection &conn,
                                 const std::string &asset_name,
                                 std::function<void(const tntdb::Row&)> cb);

// select_assets_cb: select data about all assets from v_bios_asset_element, process with cb
// return -1 in case of error or asset not found, 0 otherwise
    int
    select_assets_cb (tntdb::Connection &conn,
                      std::function<void(const tntdb::Row&)> cb);
// --------------------------------------------------------------------

// select_monitor_device_type_id: select id based on name from v_bios_device_type
// db_reply_t.status == 0 means error or not found, 1 means success

    db_reply_t
    select_monitor_device_type_id (tntdb::Connection &conn,
                                   const char *device_type_name);

// select_asset_element_web_byId: select all data about asset in v_web_element
// db_reply.status == 0 means error or not found, 1 means success

    db_reply <db_web_basic_element_t>
    select_asset_element_web_byId (tntdb::Connection &conn,
                                   uint32_t element_id);

// select_asset_element_web_byName:  select all data about asset in v_web_element based on asset name
// db_reply.status == 0 means error or not found, 1 means success

    db_reply <db_web_basic_element_t>
    select_asset_element_web_byName (tntdb::Connection &conn,
                                     const char *element_name);

// select_asset_device_links_to: get data about the links the specified device belongs to
// db_reply.status == 0 means error or not found, 1 means success

    db_reply <std::vector <db_tmp_link_t>>
    select_asset_device_links_to (tntdb::Connection &conn,
                                  uint32_t element_id,
                                  uint8_t link_type_id);

// select_asset_element_groups: get information about the groups element belongs to
// db_reply.status == 0 means error or not found, 1 means success

    db_reply <std::map <uint32_t, std::string> >
    select_asset_element_groups (tntdb::Connection &conn,
                                 uint32_t element_id);

// select_short_elements: select all devices of certain type/subtype
// db_reply.status == 0 means error or not found, 1 means success

    db_reply <std::map <uint32_t, std::string> >
    select_short_elements (tntdb::Connection &conn,
                           uint16_t type_id,
                           uint16_t subtype_id);

// select_asset_elements_by_type: returns assets for given type
    db_reply <std::vector<db_a_elmnt_t>>
    select_asset_elements_by_type (tntdb::Connection &conn,
                                   uint16_t type_id,
                                   std::string status);

// select_links_by_container: returns power links for given container
    db_reply <std::set <std::pair<uint32_t, uint32_t>>>
    select_links_by_container (tntdb::Connection &conn,
                               uint32_t element_id,
                               std::string status);

// list_devices_with_status: returns active/inactive devices
    std::vector <std::string>
    list_devices_with_status (tntdb::Connection &conn, std::string status);

// get_active_power_devices: get count of active power devices
    int
    get_active_power_devices (tntdb::Connection &conn);

// get_status_from_db: read status (active/nonactive) of given element
    std::string
    get_status_from_db (tntdb::Connection conn,
                        std::string &element_name);
} // namespace

void
fty_common_db_asset_test (bool verbose);

#endif //namespace
#endif // FTY_COMMON_DB_ASSET_H_INCLUDED
