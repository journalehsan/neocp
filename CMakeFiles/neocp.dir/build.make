# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

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
CMAKE_SOURCE_DIR = /home/ehsant/Codes/neocp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ehsant/Codes/neocp

# Include any dependencies generated for this target.
include CMakeFiles/neocp.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/neocp.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/neocp.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/neocp.dir/flags.make

CMakeFiles/neocp.dir/neocp.c.o: CMakeFiles/neocp.dir/flags.make
CMakeFiles/neocp.dir/neocp.c.o: neocp.c
CMakeFiles/neocp.dir/neocp.c.o: CMakeFiles/neocp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/ehsant/Codes/neocp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/neocp.dir/neocp.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/neocp.dir/neocp.c.o -MF CMakeFiles/neocp.dir/neocp.c.o.d -o CMakeFiles/neocp.dir/neocp.c.o -c /home/ehsant/Codes/neocp/neocp.c

CMakeFiles/neocp.dir/neocp.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/neocp.dir/neocp.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ehsant/Codes/neocp/neocp.c > CMakeFiles/neocp.dir/neocp.c.i

CMakeFiles/neocp.dir/neocp.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/neocp.dir/neocp.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ehsant/Codes/neocp/neocp.c -o CMakeFiles/neocp.dir/neocp.c.s

# Object files for target neocp
neocp_OBJECTS = \
"CMakeFiles/neocp.dir/neocp.c.o"

# External object files for target neocp
neocp_EXTERNAL_OBJECTS =

neocp: CMakeFiles/neocp.dir/neocp.c.o
neocp: CMakeFiles/neocp.dir/build.make
neocp: CMakeFiles/neocp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/ehsant/Codes/neocp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable neocp"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/neocp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/neocp.dir/build: neocp
.PHONY : CMakeFiles/neocp.dir/build

CMakeFiles/neocp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/neocp.dir/cmake_clean.cmake
.PHONY : CMakeFiles/neocp.dir/clean

CMakeFiles/neocp.dir/depend:
	cd /home/ehsant/Codes/neocp && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ehsant/Codes/neocp /home/ehsant/Codes/neocp /home/ehsant/Codes/neocp /home/ehsant/Codes/neocp /home/ehsant/Codes/neocp/CMakeFiles/neocp.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/neocp.dir/depend

