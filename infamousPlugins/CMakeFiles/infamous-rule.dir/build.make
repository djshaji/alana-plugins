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
CMAKE_SOURCE_DIR = /home/djshaji/projects/plugins-source/infamousPlugins

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/djshaji/projects/plugins-source/infamousPlugins

# Include any dependencies generated for this target.
include CMakeFiles/infamous-rule.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/infamous-rule.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/infamous-rule.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/infamous-rule.dir/flags.make

CMakeFiles/infamous-rule.dir/src/rule.c.o: CMakeFiles/infamous-rule.dir/flags.make
CMakeFiles/infamous-rule.dir/src/rule.c.o: src/rule.c
CMakeFiles/infamous-rule.dir/src/rule.c.o: CMakeFiles/infamous-rule.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/djshaji/projects/plugins-source/infamousPlugins/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/infamous-rule.dir/src/rule.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/infamous-rule.dir/src/rule.c.o -MF CMakeFiles/infamous-rule.dir/src/rule.c.o.d -o CMakeFiles/infamous-rule.dir/src/rule.c.o -c /home/djshaji/projects/plugins-source/infamousPlugins/src/rule.c

CMakeFiles/infamous-rule.dir/src/rule.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/infamous-rule.dir/src/rule.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/djshaji/projects/plugins-source/infamousPlugins/src/rule.c > CMakeFiles/infamous-rule.dir/src/rule.c.i

CMakeFiles/infamous-rule.dir/src/rule.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/infamous-rule.dir/src/rule.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/djshaji/projects/plugins-source/infamousPlugins/src/rule.c -o CMakeFiles/infamous-rule.dir/src/rule.c.s

# Object files for target infamous-rule
infamous__rule_OBJECTS = \
"CMakeFiles/infamous-rule.dir/src/rule.c.o"

# External object files for target infamous-rule
infamous__rule_EXTERNAL_OBJECTS =

infamous-rule: CMakeFiles/infamous-rule.dir/src/rule.c.o
infamous-rule: CMakeFiles/infamous-rule.dir/build.make
infamous-rule: CMakeFiles/infamous-rule.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/djshaji/projects/plugins-source/infamousPlugins/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable infamous-rule"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/infamous-rule.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/infamous-rule.dir/build: infamous-rule
.PHONY : CMakeFiles/infamous-rule.dir/build

CMakeFiles/infamous-rule.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/infamous-rule.dir/cmake_clean.cmake
.PHONY : CMakeFiles/infamous-rule.dir/clean

CMakeFiles/infamous-rule.dir/depend:
	cd /home/djshaji/projects/plugins-source/infamousPlugins && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/djshaji/projects/plugins-source/infamousPlugins /home/djshaji/projects/plugins-source/infamousPlugins /home/djshaji/projects/plugins-source/infamousPlugins /home/djshaji/projects/plugins-source/infamousPlugins /home/djshaji/projects/plugins-source/infamousPlugins/CMakeFiles/infamous-rule.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/infamous-rule.dir/depend

