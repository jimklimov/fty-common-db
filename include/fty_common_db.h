/*  =========================================================================
    fty-common-db - Provides common database tools for agents

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

#ifndef FTY_COMMON_DB_H_H_INCLUDED
#define FTY_COMMON_DB_H_H_INCLUDED

//  Include the project library file
#include "fty_common_db_library.h"

//  Add your own public definitions here, if you need them

//! Possible error types
enum errtypes {
    //! First error should be UNKNOWN as it maps to zero and zero is weird
    UNKNOWN_ERR,
    DB_ERR,
    BAD_INPUT,
    INTERNAL_ERR,
    LICENSING_ERR,
};

//! Constants for database errors
enum db_err_nos {
    //! First error should be UNKNOWN as it maps to zero and zero is weird
    DB_ERROR_UNKNOWN,
    DB_ERROR_INTERNAL,
    // Probably should be removed at some point and replaced with bad_input_err
    DB_ERROR_BADINPUT,
    DB_ERROR_NOTFOUND,
    DB_ERROR_NOTIMPLEMENTED,
    DB_ERROR_DBCORRUPTED,
    DB_ERROR_NOTHINGINSERTED,
    DB_ERROR_DELETEFAIL,
    DB_ERROR_CANTCONNECT,
};

//! Constants for bad input type of error
enum bad_input_err {
    //! First error should be UNKNOWN as it maps to zero and zero is weird
    BAD_INPUT_UNKNOWN,
    BAD_INPUT_WRONG_INPUT,
    BAD_INPUT_OUT_OF_BOUNDS,
};

//! Constants for internal errors
enum internal_err {
    //! First error should be UNKNOWN as it maps to zero and zero is weird
    INTERNAL_UNKNOWN,
    INTERNAL_NOT_IMPLEMENTED
};


#endif
