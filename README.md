# fty-common-db

This is a library providing :
* Common methods and functions that require DB

## How to build

To build the `fty-common-db` project run:

```bash
./autogen.sh [clean]
./configure
make
make check # to run self-test
```

## List of Available Headers
* fty\_common\_db\_asset.h
* fty\_common\_db\_asset\_delete.h
* fty\_common\_db\_asset\_insert.h
* fty\_common\_db\_asset\_update.h
* fty\_common\_db.h
* fty\_common\_db\_dbpath.h
* fty\_common\_db\_defs.h
* fty\_common\_db\_uptime.h

## How to compile and test projects using fty-common-db by 42ITy standards

### project.xml
Add this block in the `project.xml` file :

````
<use project = "fty-common-db" libname = "libfty_common_db" header = "fty_common_db.h"
    repository = "https://github.com/42ity/fty-common-db.git"
    release = "master"
    test = "fty_commmon_db_selftest" >

    <use project = "czmq"
        repository = "https://github.com/42ity/czmq.git"
        release = "v3.0.2-FTY-master"
        min_major = "3" min_minor = "0" min_patch = "2" >

        <use project = "libzmq"
            repository = "https://github.com/42ity/libzmq.git"
            release = "4.2.0-FTY-master" >

            <use project = "libsodium" prefix = "sodium"
                repository = "https://github.com/42ity/libsodium.git"
                release = "1.0.5-FTY-master"
                test = "sodium_init"
                />
        </use>
    </use>

    <use project = "tntdb"
        test = "tntdb::Date::gmtime"
        repository = "https://github.com/42ity/tntdb.git"
        release = "1.3-FTY-master"
        builddir = "tntdb">

        <use project = "cxxtools"
            test = "cxxtools::Utf8Codec::Utf8Codec"
            header = "cxxtools/allocator.h"
            repository = "https://github.com/42ity/cxxtools.git"
            release = "2.2-FTY-master"
            />
    </use>

    <use project = "fty-common" libname = "libfty_common" header = "fty_common.h"
        repository = "https://github.com/42ity/fty-common.git"
        release = "master"
        test = "fty_common_selftest" >

        <use project = "fty-common-logging" libname = "libfty_common_logging" header = "fty_log.h"
            repository = "https://github.com/42ity/fty-common-logging.git"
            release = "master"
            test = "fty_common_logging_selftest" >

            <use project = "log4cplus" header = "log4cplus/logger.h" test = "appender_test"
                repository = "https://github.com/42ity/log4cplus.git"
                release = "1.1.2-FTY-master"
                />
        </use>
    </use>
</use>
````

