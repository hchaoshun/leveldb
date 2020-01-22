# Install script for directory: /Users/hcs/Documents/version-control/leveldb

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/hcs/Documents/version-control/leveldb/cmake-build-debug/libleveldb.a")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libleveldb.a" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libleveldb.a")
    execute_process(COMMAND "/Library/Developer/CommandLineTools/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libleveldb.a")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/leveldb" TYPE FILE FILES
    "/Users/hcs/Documents/version-control/leveldb/include/leveldb/c.h"
    "/Users/hcs/Documents/version-control/leveldb/include/leveldb/cache.h"
    "/Users/hcs/Documents/version-control/leveldb/include/leveldb/comparator.h"
    "/Users/hcs/Documents/version-control/leveldb/include/leveldb/db.h"
    "/Users/hcs/Documents/version-control/leveldb/include/leveldb/dumpfile.h"
    "/Users/hcs/Documents/version-control/leveldb/include/leveldb/env.h"
    "/Users/hcs/Documents/version-control/leveldb/include/leveldb/export.h"
    "/Users/hcs/Documents/version-control/leveldb/include/leveldb/filter_policy.h"
    "/Users/hcs/Documents/version-control/leveldb/include/leveldb/iterator.h"
    "/Users/hcs/Documents/version-control/leveldb/include/leveldb/options.h"
    "/Users/hcs/Documents/version-control/leveldb/include/leveldb/slice.h"
    "/Users/hcs/Documents/version-control/leveldb/include/leveldb/status.h"
    "/Users/hcs/Documents/version-control/leveldb/include/leveldb/table_builder.h"
    "/Users/hcs/Documents/version-control/leveldb/include/leveldb/table.h"
    "/Users/hcs/Documents/version-control/leveldb/include/leveldb/write_batch.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/leveldb/leveldbTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/leveldb/leveldbTargets.cmake"
         "/Users/hcs/Documents/version-control/leveldb/cmake-build-debug/CMakeFiles/Export/lib/cmake/leveldb/leveldbTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/leveldb/leveldbTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/leveldb/leveldbTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/leveldb" TYPE FILE FILES "/Users/hcs/Documents/version-control/leveldb/cmake-build-debug/CMakeFiles/Export/lib/cmake/leveldb/leveldbTargets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/leveldb" TYPE FILE FILES "/Users/hcs/Documents/version-control/leveldb/cmake-build-debug/CMakeFiles/Export/lib/cmake/leveldb/leveldbTargets-debug.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/leveldb" TYPE FILE FILES
    "/Users/hcs/Documents/version-control/leveldb/cmake-build-debug/cmake/leveldbConfig.cmake"
    "/Users/hcs/Documents/version-control/leveldb/cmake-build-debug/cmake/leveldbConfigVersion.cmake"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/Users/hcs/Documents/version-control/leveldb/cmake-build-debug/third_party/googletest/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/Users/hcs/Documents/version-control/leveldb/cmake-build-debug/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
