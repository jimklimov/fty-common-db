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

/*
@header
    fty_common_db_limitations - Class which implements generic asset creation limitations
@discuss
@end
*/

#include "fty_common_db_classes.h"

namespace fty
{
    uint32_t DBLimitations::activateAsset(const std::string& asset_json)
    {
        m_assets.push_back(asset_json);
        return 0;
    }

    uint32_t DBLimitations::deactivateAsset(const std::string& asset_json)
    {
        auto it = m_assets.begin();
        while (it != m_assets.end())
        {
            if (*it == asset_json)
            {
                it = m_assets.erase (it);
                break;
            }
            it++;
        }

        return 0;
    }

    bool DBLimitations::isOperationAllowed (const std::string& operation)
    {
        return true;
    }
}
