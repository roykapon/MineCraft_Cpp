# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.20

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "G:\My Drive\Projects\MineCraft_Cpp"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "G:\My Drive\Projects\MineCraft_Cpp\build"

# Include any dependencies generated for this target.
include CMakeFiles/MineCraft_cpp.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/MineCraft_cpp.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/MineCraft_cpp.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/MineCraft_cpp.dir/flags.make

CMakeFiles/MineCraft_cpp.dir/MineCraft_cpp.cpp.obj: CMakeFiles/MineCraft_cpp.dir/flags.make
CMakeFiles/MineCraft_cpp.dir/MineCraft_cpp.cpp.obj: ../MineCraft_cpp.cpp
CMakeFiles/MineCraft_cpp.dir/MineCraft_cpp.cpp.obj: CMakeFiles/MineCraft_cpp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="G:\My Drive\Projects\MineCraft_Cpp\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/MineCraft_cpp.dir/MineCraft_cpp.cpp.obj"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/MineCraft_cpp.dir/MineCraft_cpp.cpp.obj -MF CMakeFiles\MineCraft_cpp.dir\MineCraft_cpp.cpp.obj.d -o CMakeFiles\MineCraft_cpp.dir\MineCraft_cpp.cpp.obj -c "G:\My Drive\Projects\MineCraft_Cpp\MineCraft_cpp.cpp"

CMakeFiles/MineCraft_cpp.dir/MineCraft_cpp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MineCraft_cpp.dir/MineCraft_cpp.cpp.i"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "G:\My Drive\Projects\MineCraft_Cpp\MineCraft_cpp.cpp" > CMakeFiles\MineCraft_cpp.dir\MineCraft_cpp.cpp.i

CMakeFiles/MineCraft_cpp.dir/MineCraft_cpp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MineCraft_cpp.dir/MineCraft_cpp.cpp.s"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "G:\My Drive\Projects\MineCraft_Cpp\MineCraft_cpp.cpp" -o CMakeFiles\MineCraft_cpp.dir\MineCraft_cpp.cpp.s

# Object files for target MineCraft_cpp
MineCraft_cpp_OBJECTS = \
"CMakeFiles/MineCraft_cpp.dir/MineCraft_cpp.cpp.obj"

# External object files for target MineCraft_cpp
MineCraft_cpp_EXTERNAL_OBJECTS =

libMineCraft_cpp.a: CMakeFiles/MineCraft_cpp.dir/MineCraft_cpp.cpp.obj
libMineCraft_cpp.a: CMakeFiles/MineCraft_cpp.dir/build.make
libMineCraft_cpp.a: CMakeFiles/MineCraft_cpp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="G:\My Drive\Projects\MineCraft_Cpp\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libMineCraft_cpp.a"
	$(CMAKE_COMMAND) -P CMakeFiles\MineCraft_cpp.dir\cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\MineCraft_cpp.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/MineCraft_cpp.dir/build: libMineCraft_cpp.a
.PHONY : CMakeFiles/MineCraft_cpp.dir/build

CMakeFiles/MineCraft_cpp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\MineCraft_cpp.dir\cmake_clean.cmake
.PHONY : CMakeFiles/MineCraft_cpp.dir/clean

CMakeFiles/MineCraft_cpp.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "G:\My Drive\Projects\MineCraft_Cpp" "G:\My Drive\Projects\MineCraft_Cpp" "G:\My Drive\Projects\MineCraft_Cpp\build" "G:\My Drive\Projects\MineCraft_Cpp\build" "G:\My Drive\Projects\MineCraft_Cpp\build\CMakeFiles\MineCraft_cpp.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/MineCraft_cpp.dir/depend
