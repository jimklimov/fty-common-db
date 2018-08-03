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

/*
@header
    fty_common_db_asset - Asset filtering functions
@discuss
@end
*/

#include "fty_common_db_classes.h"
#include <assert.h>
//#include <inttypes.h>

namespace DBAssets {

std::pair <std::string, std::string>
id_to_name_ext_name (uint32_t asset_id)
{
    std::string name;
    std::string ext_name;
    try
    {
        tntdb::Connection conn = tntdb::connectCached(DBConn::url);
        tntdb::Statement st = conn.prepareCached(
            " SELECT asset.name, ext.value "
            " FROM "
            "   t_bios_asset_element AS asset "
            " LEFT JOIN "
            "   t_bios_asset_ext_attributes AS ext "
            " ON "
            "   ext.id_asset_element = asset.id_asset_element "
            " WHERE "
            "   ext.keytag = \"name\" AND asset.id_asset_element = :asset_id ");

        tntdb::Row row = st.set("asset_id", asset_id).selectRow();
        log_debug("[t_bios_asset_element]: were selected %" PRIu32 " rows", 1);

        row [0].get (name);
        row [1].get (ext_name);
    }
    catch (const std::exception &e)
    {
        if (asset_id != 0)
            log_error ("exception caught %s - %" PRIu32, e.what (), asset_id);
        name = "";
        ext_name = "";
    }
    return make_pair (name, ext_name);
}

int64_t
name_to_asset_id (std::string asset_name)
{
    try
    {
        int64_t id = 0;

        tntdb::Connection conn = tntdb::connectCached(DBConn::url);
        tntdb::Statement st = conn.prepareCached(
                " SELECT id_asset_element"
                " FROM"
                "   t_bios_asset_element"
                " WHERE name = :asset_name"
                );

        tntdb::Row row = st.set("asset_name", asset_name).selectRow();
        log_debug("[t_bios_asset_element]: were selected %" PRIu32 " rows", 1);

        row [0].get(id);
        return id;
    }
    catch (const tntdb::NotFound &e) {
        log_error ("element %s not found", asset_name.c_str ());
        return -1;
    }
    catch (const std::exception &e)
    {
        log_error ("exception caught %s for element %s", e.what (), asset_name.c_str ());
        return -2;
    }
}

int64_t
extname_to_asset_id (std::string asset_ext_name)
{
    try
    {
        int64_t id = 0;

        tntdb::Connection conn = tntdb::connectCached(DBConn::url);
        tntdb::Statement st = conn.prepareCached(
                " SELECT a.id_asset_element FROM t_bios_asset_element AS a "
                " INNER JOIN t_bios_asset_ext_attributes AS e "
                " ON a.id_asset_element = e.id_asset_element "
                " WHERE keytag = 'name' and value = :extname "
                );

        tntdb::Row row = st.set("extname", asset_ext_name).selectRow();
        log_debug("[t_bios_asset_element]: were selected %" PRIu32 " rows", 1);

        row [0].get(id);
        return id;
    }
    catch (const tntdb::NotFound &e) {
        log_error ("element %s not found", asset_ext_name.c_str ());
        return -1;
    }
    catch (const std::exception &e)
    {
        log_error ("exception caught %s for element '%s'", e.what (), asset_ext_name.c_str ());
        return -2;
    }
}

int
name_to_extname (std::string asset_name, std::string &ext_name)
{
    try
    {
        tntdb::Connection conn = tntdb::connectCached(DBConn::url);
        tntdb::Statement st = conn.prepareCached(
                " SELECT e.value FROM t_bios_asset_ext_attributes AS e  "
                " INNER JOIN t_bios_asset_element AS a "
                " ON a.id_asset_element = e.id_asset_element "
                " WHERE keytag = 'name' AND a.name = :asset_name"
                );

        tntdb::Row row = st.set("asset_name", asset_name).selectRow();
        log_debug("[t_bios_asset_element]: were selected %" PRIu32 " rows", 1);

        row [0].get(ext_name);
        return 0;
    }
    catch (const tntdb::NotFound &e) {
        log_error ("element %s not found", asset_name.c_str ());
        return -1;
    }
    catch (const std::exception &e)
    {
        log_error ("exception caught %s for element '%s'", e.what (), asset_name.c_str ());
        return -2;
    }
}

int
extname_to_asset_name (std::string asset_ext_name, std::string &asset_name)
{
    try
    {
        tntdb::Connection conn = tntdb::connectCached(DBConn::url);
        tntdb::Statement st = conn.prepareCached(
                " SELECT a.name FROM t_bios_asset_element AS a "
                " INNER JOIN t_bios_asset_ext_attributes AS e "
                " ON a.id_asset_element = e.id_asset_element "
                " WHERE keytag = 'name' and value = :extname "
                );

        tntdb::Row row = st.set("extname", asset_ext_name).selectRow();
        log_debug("[t_bios_asset_element]: were selected %" PRIu32 " rows", 1);

        row [0].get(asset_name);
        return 0;
    }
    catch (const tntdb::NotFound &e) {
        log_error ("element %s not found", asset_ext_name.c_str ());
        return -1;
    }
    catch (const std::exception &e)
    {
        log_error ("exception caught %s for element '%s'", e.what (), asset_ext_name.c_str ());
        return -2;
    }
}

// --------------------------------------------------------------------------

int
select_asset_element_super_parent (
    tntdb::Connection& conn,
    uint32_t id,
    std::function<void(const tntdb::Row&)>& cb)
{
    LOG_START;

    try{
        tntdb::Statement st = conn.prepareCached(
            " SELECT "
            "   v.id_asset_element as id, "
            "   v.id_parent1 as id_parent1, "
            "   v.id_parent2 as id_parent2, "
            "   v.id_parent3 as id_parent3, "
            "   v.id_parent4 as id_parent4, "
            "   v.id_parent5 as id_parent5, "
            "   v.id_parent6 as id_parent6, "
            "   v.id_parent7 as id_parent7, "
            "   v.id_parent8 as id_parent8, "
            "   v.id_parent9 as id_parent9, "
            "   v.id_parent10 as id_parent10, "
            "   v.name_parent1 as parent_name1, "
            "   v.name_parent2 as parent_name2, "
            "   v.name_parent3 as parent_name3, "
            "   v.name_parent4 as parent_name4, "
            "   v.name_parent5 as parent_name5, "
            "   v.name_parent6 as parent_name6, "
            "   v.name_parent7 as parent_name7, "
            "   v.name_parent8 as parent_name8, "
            "   v.name_parent9 as parent_name9, "
            "   v.name_parent10 as parent_name10, "
            "   v.id_type_parent1 as id_type_parent1, "
            "   v.id_type_parent2 as id_type_parent2, "
            "   v.id_type_parent3 as id_type_parent3, "
            "   v.id_type_parent4 as id_type_parent4, "
            "   v.id_type_parent5 as id_type_parent5, "
            "   v.id_type_parent6 as id_type_parent6, "
            "   v.id_type_parent7 as id_type_parent7, "
            "   v.id_type_parent8 as id_type_parent8, "
            "   v.id_type_parent9 as id_type_parent9, "
            "   v.id_type_parent10 as id_type_parent10, "
            "   v.id_subtype_parent1 as id_subtype_parent1, "
            "   v.id_subtype_parent2 as id_subtype_parent2, "
            "   v.id_subtype_parent3 as id_subtype_parent3, "
            "   v.id_subtype_parent4 as id_subtype_parent4, "
            "   v.id_subtype_parent5 as id_subtype_parent5, "
            "   v.id_subtype_parent6 as id_subtype_parent6, "
            "   v.id_subtype_parent7 as id_subtype_parent7, "
            "   v.id_subtype_parent8 as id_subtype_parent8, "
            "   v.id_subtype_parent9 as id_subtype_parent9, "
            "   v.id_subtype_parent10 as id_subtype_parent10, "
            "   v.name as name, "
            "   v.type_name as type_name, "
            "   v.id_asset_device_type as device_type, "
            "   v.status as status, "
            "   v.asset_tag as asset_tag, "
            "   v.priority as priority, "
            "   v.id_type as id_type "
            " FROM v_bios_asset_element_super_parent AS v "
            " WHERE "
            "   v.id_asset_element = :id "
            );

        tntdb::Result res = st.set ("id", id).select ();

        for (const auto& r: res) {
            cb(r);
        }
        LOG_END;
        return 0;
    }
    catch (const std::exception &e) {
        LOG_END_ABNORMAL(e);
        return -1;
    }
}

int
select_asset_element_all_with_warranty_end(
    tntdb::Connection& conn,
    std::function<void(const tntdb::Row&)>& cb)
{
    LOG_START;

    try{
        tntdb::Statement st = conn.prepareCached(
            " SELECT "
            "   v.name as name, t.keytag as keytag, t.value as date "
            " FROM v_web_element v "
            " JOIN t_bios_asset_ext_attributes t "
            " ON "
            "   v.id = t.id_asset_element "
            " WHERE "
            "   t.keytag='end_warranty_date' "
            );

        tntdb::Result res = st.select();

        for (const auto& r: res) {
            cb(r);
        }
        LOG_END;
        return 0;
    }
    catch (const std::exception &e) {
        LOG_END_ABNORMAL(e);
        return -1;
    }
}


int
select_assets_by_container (tntdb::Connection &conn,
                            uint32_t element_id,
                            std::vector<uint16_t> types,
                            std::vector<uint16_t> subtypes,
                            std::string without,
                            std::string status,
                            std::function<void(const tntdb::Row&)> cb)
{
    LOG_START;
    log_debug ("container element_id = %" PRIu32, element_id);

    try {
        std::string select =
            " SELECT "
            "   v.name, "
            "   v.id_asset_element as asset_id, "
            "   v.id_asset_device_type as subtype_id, "
            "   v.type_name as subtype_name, "
            "   v.id_type as type_id "
            " FROM "
            "   v_bios_asset_element_super_parent AS v"
            " WHERE "
            "   :containerid in (v.id_parent1, v.id_parent2, v.id_parent3, "
            "                    v.id_parent4, v.id_parent5, v.id_parent6, "
            "                    v.id_parent7, v.id_parent8, v.id_parent9, "
            "                    v.id_parent10)";
        if (!subtypes.empty()) {
            std::string list;
            for( auto &id: subtypes) list += std::to_string(id) + ",";
            select += " AND v.id_asset_device_type in (" + list.substr(0,list.size()-1) + ")";
        }
        if (!types.empty()) {
            std::string list;
            for( auto &id: types) list += std::to_string(id) + ",";
            select += " AND v.id_type in (" + list.substr(0,list.size()-1) + ")";
        }
        if (status != "") {
            select += " AND v.status = \"" + status + "\"";
        }

        std::string end_select = "" ;
        if (without != "") {
            if(without == "location") {
                select += " AND v.id_parent1 is NULL ";
            } else if (without == "powerchain") {
                end_select += " AND NOT EXISTS "
                        " (SELECT id_asset_device_dest "
                        "  FROM t_bios_asset_link_type as l JOIN t_bios_asset_link as a"
                        "  ON a.id_asset_link_type=l.id_asset_link_type "
                        "  WHERE "
                        "     name=\"power chain\" "
                        "     AND v.id_asset_element=a.id_asset_device_dest)";
            } else {
                end_select += " AND NOT EXISTS "
                        " (SELECT a.id_asset_element "
                        "  FROM "
                        "     t_bios_asset_ext_attributes as a "
                        "  WHERE "
                        "     a.keytag=\"" + without + "\""
                        "     AND v.id_asset_element = a.id_asset_element)";
            }
        }

        select += end_select;

        // Can return more than one row.
        tntdb::Statement st = conn.prepareCached (select);

        tntdb::Result result = st.set("containerid", element_id).
                                  select();
        log_debug("[v_bios_asset_element_super_parent]: were selected %" PRIu32 " rows",
                                                            result.size());
        for ( auto &row: result ) {
            cb(row);
        }
        LOG_END;
        return 0;
    }
    catch (const std::exception& e) {
        LOG_END_ABNORMAL(e);
        return -1;
    }
}

int
select_assets_by_container (tntdb::Connection &conn,
                            uint32_t element_id,
                            std::function<void(const tntdb::Row&)> cb,
                            std::string status)
{
    log_debug ("container element_id = %" PRIu32, element_id);

    try {
        // Can return more than one row.
        tntdb::Statement st = conn.prepareCached(
            " SELECT "
            "   v.name, "
            "   v.id_asset_element as asset_id, "
            "   v.id_asset_device_type as subtype_id, "
            "   v.type_name as subtype_name, "
            "   v.id_type as type_id "
            " FROM "
            "   v_bios_asset_element_super_parent v "
            " WHERE "
            "   :containerid in (v.id_parent1, v.id_parent2, v.id_parent3, "
            "                    v.id_parent4, v.id_parent5, v.id_parent6, "
            "                    v.id_parent7, v.id_parent8, v.id_parent9, "
            "                    v.id_parent10)   AND                      "
            "                    v.status = :vstatus                       "
        );

        tntdb::Result result = st.set("containerid", element_id).
                                  set("vstatus", status).
                                  select();
        log_debug ("[v_bios_asset_element_super_parent]: were selected %" PRIu32 " rows",
                                                            result.size());
        for ( auto &row: result ) {
            cb(row);
        }
        return 0;
    }
    catch (const std::exception& e) {
        log_error ("Error: ",e.what());
        return -1;
    }
}


int
select_assets_by_container (tntdb::Connection &conn,
                            uint32_t element_id,
                            std::function<void(const tntdb::Row&)> cb)
{
    return select_assets_by_container(conn, element_id, {}, {},"","", cb);
}

int
select_assets_without_container (tntdb::Connection &conn,
                                 std::vector<uint16_t> types,
                                 std::vector<uint16_t> subtypes,
                                 std::function<void(const tntdb::Row&)> cb)
{
    LOG_START;

    try {
        std::string select =
            " SELECT "
            "   t.name, "
            "   t.id_asset_element as asset_id, "
            "   t.id_type as type_id, "
            "   t.id_subtype as subtype_id "
            " FROM "
            "   t_bios_asset_element AS t "
            " WHERE "
            "   t.id_parent is NULL";
        if (!subtypes.empty()) {
            std::string list;
            for( auto &id: subtypes) list += std::to_string(id) + ",";
            select += " and t.id_subtype in (" + list.substr(0,list.size()-1) + ")";
        }
        if (!types.empty()) {
            std::string list;
            for( auto &id: types) list += std::to_string(id) + ",";
            select += " and t.id_type in (" + list.substr(0,list.size()-1) + ")";
        }
        // Can return more than one row.
        tntdb::Statement st = conn.prepareCached (select);

        tntdb::Result result = st.select();
        log_debug ("[t_bios_asset_element]: were selected %" PRIu32 " rows",
                                                            result.size());
        for ( auto &row: result ) {
            cb(row);
        }
        LOG_END;
        return 0;
    }
    catch (const std::exception& e) {
        LOG_END_ABNORMAL(e);
        return -1;
    }
}

int
convert_asset_to_monitor (tntdb::Connection &conn,
                          uint32_t asset_element_id,
                          uint16_t &monitor_element_id)
{
    try{
        tntdb::Statement st = conn.prepareCached(
            " SELECT "
            "   v.id_discovered_device "
            " FROM "
            "   v_bios_monitor_asset_relation v "
            " WHERE "
            "   v.id_asset_element = :id "
        );

        tntdb::Value value = st.set("id", asset_element_id).
                                selectValue();

        value.get(monitor_element_id);
        LOG_END;
        return 0;
    }
    catch (const tntdb::NotFound &e){
        log_info("end: counterpart for %" PRIu32 " notfound", asset_element_id);
        monitor_element_id = 0;
        return 0;
    }
    catch (const std::exception &e) {
        LOG_END_ABNORMAL(e);
        return -1;
    }
}

int
count_keytag(
        tntdb::Connection& conn,
        const std::string &keytag,
        const std::string &value)
{
    LOG_START;
    try{
        tntdb::Statement st = conn.prepareCached(
            " SELECT COUNT( * ) "
            " FROM t_bios_asset_ext_attributes "
            " WHERE keytag = :keytag AND"
            "       value = :value"
        );

        tntdb::Row row = st.set("keytag", keytag)
                           .set("value", value)
                           .selectRow();

        int r = 0;
        row[0].get(r);
        LOG_END;
        return r;
    }
    catch (const std::exception &e) {
        LOG_END_ABNORMAL(e);
        return -1;
    }
}

int
unique_keytag (tntdb::Connection &conn,
               const std::string &keytag,
               const std::string &value,
               uint32_t       element_id)
{
    LOG_START;

    try{
        tntdb::Statement st = conn.prepareCached(
            " SELECT "
            "   id_asset_element "
            " FROM "
            "   t_bios_asset_ext_attributes "
            " WHERE keytag = :keytag AND"
            "       value = :value"
        );

        tntdb::Row row = st.set("keytag", keytag)
                           .set("value", value)
                           .selectRow();

        uint32_t r = 0;
        row[0].get(r);

        LOG_END;
        if ( element_id == r )
            return 0; // is ok
        else
            return 1; // is not ok
    }
    catch (const tntdb::NotFound &e) {
        LOG_END_ABNORMAL(e);
        return 0; // ok
    }
    catch (const std::exception &e) {
        LOG_END_ABNORMAL(e);
        return -1;
    }
}

int
max_number_of_power_links (tntdb::Connection& conn)
{
    LOG_START;

    try{
        tntdb::Statement st = conn.prepareCached(
            " SELECT "
            "   MAX(power_src_count) "
            " FROM "
            "   ( SELECT COUNT(*) power_src_count FROM t_bios_asset_link "
            "            GROUP BY id_asset_device_dest) pwr "
        );

        tntdb::Row row = st.selectRow();

        int r = 0;
        row[0].get(r);
        LOG_END;
        return r;
    }
    catch (const std::exception &e) {
        LOG_END_ABNORMAL(e);
        return -1;
    }
}

int
count_of_link_src (tntdb::Connection& conn,
                   uint32_t id)
{
    LOG_START;
    static const int id_asset_link_type = 1;
    try{
        tntdb::Statement st = conn.prepareCached(
            " SELECT COUNT( * ) "
            " FROM v_bios_asset_link "
            " WHERE id_asset_element_src = :id AND"
            "       id_asset_link_type = :lt "
        );

        tntdb::Row row = st.\
            set("id", id).\
            set("lt", id_asset_link_type).\
            selectRow();

        int r = 0;
        row[0].get(r);
        LOG_END;
        return r;
    }
    catch (const std::exception &e) {
        LOG_END_ABNORMAL(e);
        return -1;
    }
}


int
max_number_of_asset_groups (tntdb::Connection& conn)
{
    LOG_START;

    try{
        tntdb::Statement st = conn.prepareCached(
            " SELECT "
            "   MAX(grp_count) "
            " FROM "
            "   ( SELECT COUNT(*) grp_count FROM t_bios_asset_group_relation "
            "            GROUP BY id_asset_element) elmnt_grp "
        );

        tntdb::Row row = st.selectRow();

        int r = 0;
        row[0].get(r);
        LOG_END;
        return r;
    }
    catch (const std::exception &e) {
        LOG_END_ABNORMAL(e);
        return -1;
    }
}

int
select_group_names (tntdb::Connection& conn,
                    uint32_t id,
                    std::function<void(const tntdb::Row&)> cb)
{
    LOG_START;
    log_debug("id: %" PRIu32, id);
    try{
        tntdb::Statement st = conn.prepareCached(
            " SELECT "
            "   v2.name "
            " FROM v_bios_asset_group_relation v1 "
            " JOIN v_bios_asset_element v2 "
            "   ON v1.id_asset_group=v2.id "
            " WHERE v1.id_asset_element=:id "
        );

        tntdb::Result res = st.set("id", id).select();

        for (const auto& r: res) {
            cb(r);
        }
        LOG_END;
        return 0;
    }
    catch (const std::exception &e) {
        LOG_END_ABNORMAL(e);
        return -1;
    }
}

int
select_group_names (tntdb::Connection& conn,
                    uint32_t id,
                    std::vector<std::string>& out)
{
    std::function<void(const tntdb::Row&)> func = \
        [&out](const tntdb::Row& r)
        {
            std::string name;
            r["name"].get(name);
            out.push_back(name);
        };
    return select_group_names(conn, id, func);
}

int
select_v_web_asset_power_link_src_byId (tntdb::Connection& conn,
                                        uint32_t id,
                                        row_cb_f& cb)
{
    LOG_START;
    log_debug("id: %" PRIu32, id);
    try{
        tntdb::Statement st = conn.prepareCached(
            " SELECT "
            "   v.id_link, "
            "   v.id_asset_element_src, "
            "   v.src_name, "
            "   v.id_asset_element_dest, "
            "   v.dest_name, "
            "   v.src_out, "
            "   v.dest_in "
            " FROM v_web_asset_link v "
            " WHERE v.id_asset_element_dest=:id "
            " AND v.link_name = 'power chain' "
        );

        tntdb::Result res = st.set("id", id).select();

        for (const auto& r: res) {
            cb(r);
        }
        LOG_END;
        return 0;
    }
    catch (const std::exception &e) {
        LOG_END_ABNORMAL(e);
        return -1;
    }
}


} // namespace

//  --------------------------------------------------------------------------
//  Self test of this class

// If your selftest reads SCMed fixture data, please keep it in
// src/selftest-ro; if your test creates filesystem objects, please
// do so under src/selftest-rw.
// The following pattern is suggested for C selftest code:
//    char *filename = NULL;
//    filename = zsys_sprintf ("%s/%s", SELFTEST_DIR_RO, "mytemplate.file");
//    assert (filename);
//    ... use the "filename" for I/O ...
//    zstr_free (&filename);
// This way the same "filename" variable can be reused for many subtests.
#define SELFTEST_DIR_RO "src/selftest-ro"
#define SELFTEST_DIR_RW "src/selftest-rw"

void
fty_common_db_asset_test (bool verbose)
{
    printf (" * fty_common_db_asset: ");

    printf ("not tests \n");
}
