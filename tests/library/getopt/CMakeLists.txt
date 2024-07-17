add_ydb_test(NAME library-getopt-last_getopt_ut
  SOURCES
    last_getopt_ut.cpp
  INCLUDE_DIRS
    ${YDB_SDK_SOURCE_DIR}/src/library/getopt
  LINK_LIBRARIES
    library-getopt
    cpp-testing-unittest_main
)

add_ydb_test(NAME library-getopt-modchooser_ut
  SOURCES
    modchooser_ut.cpp
  INCLUDE_DIRS
    ${YDB_SDK_SOURCE_DIR}/src/library/getopt
  LINK_LIBRARIES
    library-getopt
    cpp-testing-unittest_main
)

add_ydb_test(NAME library-getopt-opt2_ut
  SOURCES
    opt2_ut.cpp
  INCLUDE_DIRS
    ${YDB_SDK_SOURCE_DIR}/src/library/getopt
  LINK_LIBRARIES
    library-getopt
    cpp-testing-unittest_main
)

add_ydb_test(NAME library-getopt-opt_ut
  SOURCES
    opt_ut.cpp
  INCLUDE_DIRS
    ${YDB_SDK_SOURCE_DIR}/src/library/getopt
  LINK_LIBRARIES
    library-getopt
    cpp-testing-unittest_main
)

add_ydb_test(NAME library-getopt-posix_getopt_ut
  SOURCES
    posix_getopt_ut.cpp
  INCLUDE_DIRS
    ${YDB_SDK_SOURCE_DIR}/src/library/getopt
  LINK_LIBRARIES
    library-getopt
    cpp-testing-unittest_main
)

add_ydb_test(NAME library-getopt-wrap_ut
  SOURCES
    wrap_ut.cpp
  INCLUDE_DIRS
    ${YDB_SDK_SOURCE_DIR}/src/library/getopt/small
  LINK_LIBRARIES
    library-getopt-small
    cpp-testing-unittest_main
)

add_ydb_test(NAME library-getopt-ygetopt_ut
  SOURCES
    ygetopt_ut.cpp
  INCLUDE_DIRS
    ${YDB_SDK_SOURCE_DIR}/src/library/getopt
  LINK_LIBRARIES
    library-getopt
    cpp-testing-unittest_main
)
