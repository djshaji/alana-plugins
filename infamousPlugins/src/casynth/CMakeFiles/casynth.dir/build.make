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
include src/casynth/CMakeFiles/casynth.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/casynth/CMakeFiles/casynth.dir/compiler_depend.make

# Include the progress variables for this target.
include src/casynth/CMakeFiles/casynth.dir/progress.make

# Include the compile flags for this target's objects.
include src/casynth/CMakeFiles/casynth.dir/flags.make

src/casynth/CMakeFiles/casynth.dir/casynth.c.o: src/casynth/CMakeFiles/casynth.dir/flags.make
src/casynth/CMakeFiles/casynth.dir/casynth.c.o: src/casynth/casynth.c
src/casynth/CMakeFiles/casynth.dir/casynth.c.o: src/casynth/CMakeFiles/casynth.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/djshaji/projects/plugins-source/infamousPlugins/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/casynth/CMakeFiles/casynth.dir/casynth.c.o"
	cd /home/djshaji/projects/plugins-source/infamousPlugins/src/casynth && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/casynth/CMakeFiles/casynth.dir/casynth.c.o -MF CMakeFiles/casynth.dir/casynth.c.o.d -o CMakeFiles/casynth.dir/casynth.c.o -c /home/djshaji/projects/plugins-source/infamousPlugins/src/casynth/casynth.c

src/casynth/CMakeFiles/casynth.dir/casynth.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/casynth.dir/casynth.c.i"
	cd /home/djshaji/projects/plugins-source/infamousPlugins/src/casynth && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/djshaji/projects/plugins-source/infamousPlugins/src/casynth/casynth.c > CMakeFiles/casynth.dir/casynth.c.i

src/casynth/CMakeFiles/casynth.dir/casynth.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/casynth.dir/casynth.c.s"
	cd /home/djshaji/projects/plugins-source/infamousPlugins/src/casynth && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/djshaji/projects/plugins-source/infamousPlugins/src/casynth/casynth.c -o CMakeFiles/casynth.dir/casynth.c.s

src/casynth/CMakeFiles/casynth.dir/waves.c.o: src/casynth/CMakeFiles/casynth.dir/flags.make
src/casynth/CMakeFiles/casynth.dir/waves.c.o: src/casynth/waves.c
src/casynth/CMakeFiles/casynth.dir/waves.c.o: src/casynth/CMakeFiles/casynth.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/djshaji/projects/plugins-source/infamousPlugins/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object src/casynth/CMakeFiles/casynth.dir/waves.c.o"
	cd /home/djshaji/projects/plugins-source/infamousPlugins/src/casynth && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/casynth/CMakeFiles/casynth.dir/waves.c.o -MF CMakeFiles/casynth.dir/waves.c.o.d -o CMakeFiles/casynth.dir/waves.c.o -c /home/djshaji/projects/plugins-source/infamousPlugins/src/casynth/waves.c

src/casynth/CMakeFiles/casynth.dir/waves.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/casynth.dir/waves.c.i"
	cd /home/djshaji/projects/plugins-source/infamousPlugins/src/casynth && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/djshaji/projects/plugins-source/infamousPlugins/src/casynth/waves.c > CMakeFiles/casynth.dir/waves.c.i

src/casynth/CMakeFiles/casynth.dir/waves.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/casynth.dir/waves.c.s"
	cd /home/djshaji/projects/plugins-source/infamousPlugins/src/casynth && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/djshaji/projects/plugins-source/infamousPlugins/src/casynth/waves.c -o CMakeFiles/casynth.dir/waves.c.s

src/casynth/CMakeFiles/casynth.dir/note.c.o: src/casynth/CMakeFiles/casynth.dir/flags.make
src/casynth/CMakeFiles/casynth.dir/note.c.o: src/casynth/note.c
src/casynth/CMakeFiles/casynth.dir/note.c.o: src/casynth/CMakeFiles/casynth.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/djshaji/projects/plugins-source/infamousPlugins/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object src/casynth/CMakeFiles/casynth.dir/note.c.o"
	cd /home/djshaji/projects/plugins-source/infamousPlugins/src/casynth && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/casynth/CMakeFiles/casynth.dir/note.c.o -MF CMakeFiles/casynth.dir/note.c.o.d -o CMakeFiles/casynth.dir/note.c.o -c /home/djshaji/projects/plugins-source/infamousPlugins/src/casynth/note.c

src/casynth/CMakeFiles/casynth.dir/note.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/casynth.dir/note.c.i"
	cd /home/djshaji/projects/plugins-source/infamousPlugins/src/casynth && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/djshaji/projects/plugins-source/infamousPlugins/src/casynth/note.c > CMakeFiles/casynth.dir/note.c.i

src/casynth/CMakeFiles/casynth.dir/note.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/casynth.dir/note.c.s"
	cd /home/djshaji/projects/plugins-source/infamousPlugins/src/casynth && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/djshaji/projects/plugins-source/infamousPlugins/src/casynth/note.c -o CMakeFiles/casynth.dir/note.c.s

# Object files for target casynth
casynth_OBJECTS = \
"CMakeFiles/casynth.dir/casynth.c.o" \
"CMakeFiles/casynth.dir/waves.c.o" \
"CMakeFiles/casynth.dir/note.c.o"

# External object files for target casynth
casynth_EXTERNAL_OBJECTS =

src/casynth/casynth.so: src/casynth/CMakeFiles/casynth.dir/casynth.c.o
src/casynth/casynth.so: src/casynth/CMakeFiles/casynth.dir/waves.c.o
src/casynth/casynth.so: src/casynth/CMakeFiles/casynth.dir/note.c.o
src/casynth/casynth.so: src/casynth/CMakeFiles/casynth.dir/build.make
src/casynth/casynth.so: src/casynth/CMakeFiles/casynth.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/djshaji/projects/plugins-source/infamousPlugins/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C shared library casynth.so"
	cd /home/djshaji/projects/plugins-source/infamousPlugins/src/casynth && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/casynth.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/casynth/CMakeFiles/casynth.dir/build: src/casynth/casynth.so
.PHONY : src/casynth/CMakeFiles/casynth.dir/build

src/casynth/CMakeFiles/casynth.dir/clean:
	cd /home/djshaji/projects/plugins-source/infamousPlugins/src/casynth && $(CMAKE_COMMAND) -P CMakeFiles/casynth.dir/cmake_clean.cmake
.PHONY : src/casynth/CMakeFiles/casynth.dir/clean

src/casynth/CMakeFiles/casynth.dir/depend:
	cd /home/djshaji/projects/plugins-source/infamousPlugins && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/djshaji/projects/plugins-source/infamousPlugins /home/djshaji/projects/plugins-source/infamousPlugins/src/casynth /home/djshaji/projects/plugins-source/infamousPlugins /home/djshaji/projects/plugins-source/infamousPlugins/src/casynth /home/djshaji/projects/plugins-source/infamousPlugins/src/casynth/CMakeFiles/casynth.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/casynth/CMakeFiles/casynth.dir/depend

