# Install script for directory: /home/djshaji/projects/plugins-source/infamousPlugins

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

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/usr/local/bin/infamous-rule" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/bin/infamous-rule")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/usr/local/bin/infamous-rule"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/bin/infamous-rule")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/usr/local/bin" TYPE EXECUTABLE FILES "/home/djshaji/projects/plugins-source/infamousPlugins/infamous-rule")
  if(EXISTS "$ENV{DESTDIR}/usr/local/bin/infamous-rule" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/bin/infamous-rule")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/usr/local/bin/infamous-rule")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/djshaji/projects/plugins-source/infamousPlugins/src/casynth/cmake_install.cmake")
  include("/home/djshaji/projects/plugins-source/infamousPlugins/src/envfollower/cmake_install.cmake")
  include("/home/djshaji/projects/plugins-source/infamousPlugins/src/hip2b/cmake_install.cmake")
  include("/home/djshaji/projects/plugins-source/infamousPlugins/src/powerup/cmake_install.cmake")
  include("/home/djshaji/projects/plugins-source/infamousPlugins/src/powercut/cmake_install.cmake")
  include("/home/djshaji/projects/plugins-source/infamousPlugins/src/cheapdist/cmake_install.cmake")
  include("/home/djshaji/projects/plugins-source/infamousPlugins/src/stuck/cmake_install.cmake")
  include("/home/djshaji/projects/plugins-source/infamousPlugins/src/ewham/cmake_install.cmake")
  include("/home/djshaji/projects/plugins-source/infamousPlugins/src/lushlife/cmake_install.cmake")
  include("/home/djshaji/projects/plugins-source/infamousPlugins/src/bentdelay/cmake_install.cmake")
  include("/home/djshaji/projects/plugins-source/infamousPlugins/src/mindi/cmake_install.cmake")
  include("/home/djshaji/projects/plugins-source/infamousPlugins/src/octolo/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/djshaji/projects/plugins-source/infamousPlugins/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
