# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/djshaji/djshaji/projects/plugins-source/rt-plugins-0.0.6

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/djshaji/djshaji/projects/plugins-source/rt-plugins-0.0.6/build

# Include any dependencies generated for this target.
include src/CMakeFiles/RTlowshelf.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/CMakeFiles/RTlowshelf.dir/compiler_depend.make

# Include the progress variables for this target.
include src/CMakeFiles/RTlowshelf.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/RTlowshelf.dir/flags.make

src/CMakeFiles/RTlowshelf.dir/RTlowshelf.c.o: src/CMakeFiles/RTlowshelf.dir/flags.make
src/CMakeFiles/RTlowshelf.dir/RTlowshelf.c.o: /mnt/djshaji/djshaji/projects/plugins-source/rt-plugins-0.0.6/src/RTlowshelf.c
src/CMakeFiles/RTlowshelf.dir/RTlowshelf.c.o: src/CMakeFiles/RTlowshelf.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/djshaji/djshaji/projects/plugins-source/rt-plugins-0.0.6/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/CMakeFiles/RTlowshelf.dir/RTlowshelf.c.o"
	cd /mnt/djshaji/djshaji/projects/plugins-source/rt-plugins-0.0.6/build/src && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/CMakeFiles/RTlowshelf.dir/RTlowshelf.c.o -MF CMakeFiles/RTlowshelf.dir/RTlowshelf.c.o.d -o CMakeFiles/RTlowshelf.dir/RTlowshelf.c.o -c /mnt/djshaji/djshaji/projects/plugins-source/rt-plugins-0.0.6/src/RTlowshelf.c

src/CMakeFiles/RTlowshelf.dir/RTlowshelf.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/RTlowshelf.dir/RTlowshelf.c.i"
	cd /mnt/djshaji/djshaji/projects/plugins-source/rt-plugins-0.0.6/build/src && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/djshaji/djshaji/projects/plugins-source/rt-plugins-0.0.6/src/RTlowshelf.c > CMakeFiles/RTlowshelf.dir/RTlowshelf.c.i

src/CMakeFiles/RTlowshelf.dir/RTlowshelf.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/RTlowshelf.dir/RTlowshelf.c.s"
	cd /mnt/djshaji/djshaji/projects/plugins-source/rt-plugins-0.0.6/build/src && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/djshaji/djshaji/projects/plugins-source/rt-plugins-0.0.6/src/RTlowshelf.c -o CMakeFiles/RTlowshelf.dir/RTlowshelf.c.s

# Object files for target RTlowshelf
RTlowshelf_OBJECTS = \
"CMakeFiles/RTlowshelf.dir/RTlowshelf.c.o"

# External object files for target RTlowshelf
RTlowshelf_EXTERNAL_OBJECTS =

src/RTlowshelf.so: src/CMakeFiles/RTlowshelf.dir/RTlowshelf.c.o
src/RTlowshelf.so: src/CMakeFiles/RTlowshelf.dir/build.make
src/RTlowshelf.so: src/CMakeFiles/RTlowshelf.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/djshaji/djshaji/projects/plugins-source/rt-plugins-0.0.6/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C shared module RTlowshelf.so"
	cd /mnt/djshaji/djshaji/projects/plugins-source/rt-plugins-0.0.6/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/RTlowshelf.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/RTlowshelf.dir/build: src/RTlowshelf.so
.PHONY : src/CMakeFiles/RTlowshelf.dir/build

src/CMakeFiles/RTlowshelf.dir/clean:
	cd /mnt/djshaji/djshaji/projects/plugins-source/rt-plugins-0.0.6/build/src && $(CMAKE_COMMAND) -P CMakeFiles/RTlowshelf.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/RTlowshelf.dir/clean

src/CMakeFiles/RTlowshelf.dir/depend:
	cd /mnt/djshaji/djshaji/projects/plugins-source/rt-plugins-0.0.6/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/djshaji/djshaji/projects/plugins-source/rt-plugins-0.0.6 /mnt/djshaji/djshaji/projects/plugins-source/rt-plugins-0.0.6/src /mnt/djshaji/djshaji/projects/plugins-source/rt-plugins-0.0.6/build /mnt/djshaji/djshaji/projects/plugins-source/rt-plugins-0.0.6/build/src /mnt/djshaji/djshaji/projects/plugins-source/rt-plugins-0.0.6/build/src/CMakeFiles/RTlowshelf.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/RTlowshelf.dir/depend

