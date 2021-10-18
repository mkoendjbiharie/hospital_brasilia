# Install script for directory: /home/marlon/libharu-RELEASE_2_3_0

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
    set(CMAKE_INSTALL_CONFIG_NAME "")
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

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/home/marlon/libharu-RELEASE_2_3_0/include/hpdf.h"
    "/home/marlon/libharu-RELEASE_2_3_0/include/hpdf_types.h"
    "/home/marlon/libharu-RELEASE_2_3_0/include/hpdf_consts.h"
    "/home/marlon/libharu-RELEASE_2_3_0/include/hpdf_version.h"
    "/home/marlon/libharu-RELEASE_2_3_0/include/hpdf_annotation.h"
    "/home/marlon/libharu-RELEASE_2_3_0/include/hpdf_catalog.h"
    "/home/marlon/libharu-RELEASE_2_3_0/include/hpdf_conf.h"
    "/home/marlon/libharu-RELEASE_2_3_0/include/hpdf_destination.h"
    "/home/marlon/libharu-RELEASE_2_3_0/include/hpdf_doc.h"
    "/home/marlon/libharu-RELEASE_2_3_0/include/hpdf_encoder.h"
    "/home/marlon/libharu-RELEASE_2_3_0/include/hpdf_encrypt.h"
    "/home/marlon/libharu-RELEASE_2_3_0/include/hpdf_encryptdict.h"
    "/home/marlon/libharu-RELEASE_2_3_0/include/hpdf_error.h"
    "/home/marlon/libharu-RELEASE_2_3_0/include/hpdf_ext_gstate.h"
    "/home/marlon/libharu-RELEASE_2_3_0/include/hpdf_font.h"
    "/home/marlon/libharu-RELEASE_2_3_0/include/hpdf_fontdef.h"
    "/home/marlon/libharu-RELEASE_2_3_0/include/hpdf_gstate.h"
    "/home/marlon/libharu-RELEASE_2_3_0/include/hpdf_image.h"
    "/home/marlon/libharu-RELEASE_2_3_0/include/hpdf_info.h"
    "/home/marlon/libharu-RELEASE_2_3_0/include/hpdf_list.h"
    "/home/marlon/libharu-RELEASE_2_3_0/include/hpdf_mmgr.h"
    "/home/marlon/libharu-RELEASE_2_3_0/include/hpdf_objects.h"
    "/home/marlon/libharu-RELEASE_2_3_0/include/hpdf_outline.h"
    "/home/marlon/libharu-RELEASE_2_3_0/include/hpdf_pages.h"
    "/home/marlon/libharu-RELEASE_2_3_0/include/hpdf_page_label.h"
    "/home/marlon/libharu-RELEASE_2_3_0/include/hpdf_streams.h"
    "/home/marlon/libharu-RELEASE_2_3_0/include/hpdf_u3d.h"
    "/home/marlon/libharu-RELEASE_2_3_0/include/hpdf_utils.h"
    "/home/marlon/libharu-RELEASE_2_3_0/include/hpdf_pdfa.h"
    "/home/marlon/libharu-RELEASE_2_3_0/include/hpdf_3dmeasure.h"
    "/home/marlon/libharu-RELEASE_2_3_0/include/hpdf_exdata.h"
    "/home/marlon/WT/include/hpdf_config.h"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/." TYPE FILE FILES
    "/home/marlon/libharu-RELEASE_2_3_0/README"
    "/home/marlon/libharu-RELEASE_2_3_0/CHANGES"
    "/home/marlon/libharu-RELEASE_2_3_0/INSTALL"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/." TYPE DIRECTORY FILES "/home/marlon/libharu-RELEASE_2_3_0/if")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/marlon/WT/src/cmake_install.cmake")
  include("/home/marlon/WT/demo/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/marlon/WT/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
