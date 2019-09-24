/*  =========================================================================
    fty_common_db_limitations - Class which implements generic asset creation limitations

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

#ifndef FTY_COMMON_DB_LIMITATIONS_H_INCLUDED
#define FTY_COMMON_DB_LIMITATIONS_H_INCLUDED

/* This class is intended to be overriden by different DB limitations for different
 * agent (e.g. forbidden DELETE, read-only DB layer, etc.)
*/

namespace fty
{
    class DBLimitations
    {
        public:
            virtual uint32_t activateAsset(const std::string& asset_json);
            virtual uint32_t deactivateAsset(const std::string& asset_json);
            virtual bool isOperationAllowed (const std::string& operation);

        private:
            std::vector<std::string> m_assets;
    };
}

#endif
