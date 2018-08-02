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
    fty_common_db_asset - Asset related queries
@discuss
@end
*/

#include "fty_common_db_classes.h"
#include <assert.h>
#include <inttypes.h>

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
