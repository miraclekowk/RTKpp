# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.21

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
CMAKE_COMMAND = /opt/cmake-3.21.4/bin/cmake

# The command to remove a file.
RM = /opt/cmake-3.21.4/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/nins/Code/RTKpp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nins/Code/RTKpp/build

# Include any dependencies generated for this target.
include CMakeFiles/RTKpp.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/RTKpp.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/RTKpp.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/RTKpp.dir/flags.make

CMakeFiles/RTKpp.dir/main.cpp.o: CMakeFiles/RTKpp.dir/flags.make
CMakeFiles/RTKpp.dir/main.cpp.o: ../main.cpp
CMakeFiles/RTKpp.dir/main.cpp.o: CMakeFiles/RTKpp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nins/Code/RTKpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/RTKpp.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RTKpp.dir/main.cpp.o -MF CMakeFiles/RTKpp.dir/main.cpp.o.d -o CMakeFiles/RTKpp.dir/main.cpp.o -c /home/nins/Code/RTKpp/main.cpp

CMakeFiles/RTKpp.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RTKpp.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nins/Code/RTKpp/main.cpp > CMakeFiles/RTKpp.dir/main.cpp.i

CMakeFiles/RTKpp.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RTKpp.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nins/Code/RTKpp/main.cpp -o CMakeFiles/RTKpp.dir/main.cpp.s

CMakeFiles/RTKpp.dir/utils.cpp.o: CMakeFiles/RTKpp.dir/flags.make
CMakeFiles/RTKpp.dir/utils.cpp.o: ../utils.cpp
CMakeFiles/RTKpp.dir/utils.cpp.o: CMakeFiles/RTKpp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nins/Code/RTKpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/RTKpp.dir/utils.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RTKpp.dir/utils.cpp.o -MF CMakeFiles/RTKpp.dir/utils.cpp.o.d -o CMakeFiles/RTKpp.dir/utils.cpp.o -c /home/nins/Code/RTKpp/utils.cpp

CMakeFiles/RTKpp.dir/utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RTKpp.dir/utils.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nins/Code/RTKpp/utils.cpp > CMakeFiles/RTKpp.dir/utils.cpp.i

CMakeFiles/RTKpp.dir/utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RTKpp.dir/utils.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nins/Code/RTKpp/utils.cpp -o CMakeFiles/RTKpp.dir/utils.cpp.s

CMakeFiles/RTKpp.dir/rtk_request.cpp.o: CMakeFiles/RTKpp.dir/flags.make
CMakeFiles/RTKpp.dir/rtk_request.cpp.o: ../rtk_request.cpp
CMakeFiles/RTKpp.dir/rtk_request.cpp.o: CMakeFiles/RTKpp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nins/Code/RTKpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/RTKpp.dir/rtk_request.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RTKpp.dir/rtk_request.cpp.o -MF CMakeFiles/RTKpp.dir/rtk_request.cpp.o.d -o CMakeFiles/RTKpp.dir/rtk_request.cpp.o -c /home/nins/Code/RTKpp/rtk_request.cpp

CMakeFiles/RTKpp.dir/rtk_request.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RTKpp.dir/rtk_request.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nins/Code/RTKpp/rtk_request.cpp > CMakeFiles/RTKpp.dir/rtk_request.cpp.i

CMakeFiles/RTKpp.dir/rtk_request.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RTKpp.dir/rtk_request.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nins/Code/RTKpp/rtk_request.cpp -o CMakeFiles/RTKpp.dir/rtk_request.cpp.s

CMakeFiles/RTKpp.dir/rtk_response.cpp.o: CMakeFiles/RTKpp.dir/flags.make
CMakeFiles/RTKpp.dir/rtk_response.cpp.o: ../rtk_response.cpp
CMakeFiles/RTKpp.dir/rtk_response.cpp.o: CMakeFiles/RTKpp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nins/Code/RTKpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/RTKpp.dir/rtk_response.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RTKpp.dir/rtk_response.cpp.o -MF CMakeFiles/RTKpp.dir/rtk_response.cpp.o.d -o CMakeFiles/RTKpp.dir/rtk_response.cpp.o -c /home/nins/Code/RTKpp/rtk_response.cpp

CMakeFiles/RTKpp.dir/rtk_response.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RTKpp.dir/rtk_response.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nins/Code/RTKpp/rtk_response.cpp > CMakeFiles/RTKpp.dir/rtk_response.cpp.i

CMakeFiles/RTKpp.dir/rtk_response.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RTKpp.dir/rtk_response.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nins/Code/RTKpp/rtk_response.cpp -o CMakeFiles/RTKpp.dir/rtk_response.cpp.s

CMakeFiles/RTKpp.dir/rtk_io.cpp.o: CMakeFiles/RTKpp.dir/flags.make
CMakeFiles/RTKpp.dir/rtk_io.cpp.o: ../rtk_io.cpp
CMakeFiles/RTKpp.dir/rtk_io.cpp.o: CMakeFiles/RTKpp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nins/Code/RTKpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/RTKpp.dir/rtk_io.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RTKpp.dir/rtk_io.cpp.o -MF CMakeFiles/RTKpp.dir/rtk_io.cpp.o.d -o CMakeFiles/RTKpp.dir/rtk_io.cpp.o -c /home/nins/Code/RTKpp/rtk_io.cpp

CMakeFiles/RTKpp.dir/rtk_io.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RTKpp.dir/rtk_io.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nins/Code/RTKpp/rtk_io.cpp > CMakeFiles/RTKpp.dir/rtk_io.cpp.i

CMakeFiles/RTKpp.dir/rtk_io.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RTKpp.dir/rtk_io.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nins/Code/RTKpp/rtk_io.cpp -o CMakeFiles/RTKpp.dir/rtk_io.cpp.s

CMakeFiles/RTKpp.dir/rtk_threadpool.cpp.o: CMakeFiles/RTKpp.dir/flags.make
CMakeFiles/RTKpp.dir/rtk_threadpool.cpp.o: ../rtk_threadpool.cpp
CMakeFiles/RTKpp.dir/rtk_threadpool.cpp.o: CMakeFiles/RTKpp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nins/Code/RTKpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/RTKpp.dir/rtk_threadpool.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RTKpp.dir/rtk_threadpool.cpp.o -MF CMakeFiles/RTKpp.dir/rtk_threadpool.cpp.o.d -o CMakeFiles/RTKpp.dir/rtk_threadpool.cpp.o -c /home/nins/Code/RTKpp/rtk_threadpool.cpp

CMakeFiles/RTKpp.dir/rtk_threadpool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RTKpp.dir/rtk_threadpool.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nins/Code/RTKpp/rtk_threadpool.cpp > CMakeFiles/RTKpp.dir/rtk_threadpool.cpp.i

CMakeFiles/RTKpp.dir/rtk_threadpool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RTKpp.dir/rtk_threadpool.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nins/Code/RTKpp/rtk_threadpool.cpp -o CMakeFiles/RTKpp.dir/rtk_threadpool.cpp.s

CMakeFiles/RTKpp.dir/rtk_epoll.cpp.o: CMakeFiles/RTKpp.dir/flags.make
CMakeFiles/RTKpp.dir/rtk_epoll.cpp.o: ../rtk_epoll.cpp
CMakeFiles/RTKpp.dir/rtk_epoll.cpp.o: CMakeFiles/RTKpp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nins/Code/RTKpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/RTKpp.dir/rtk_epoll.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RTKpp.dir/rtk_epoll.cpp.o -MF CMakeFiles/RTKpp.dir/rtk_epoll.cpp.o.d -o CMakeFiles/RTKpp.dir/rtk_epoll.cpp.o -c /home/nins/Code/RTKpp/rtk_epoll.cpp

CMakeFiles/RTKpp.dir/rtk_epoll.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RTKpp.dir/rtk_epoll.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nins/Code/RTKpp/rtk_epoll.cpp > CMakeFiles/RTKpp.dir/rtk_epoll.cpp.i

CMakeFiles/RTKpp.dir/rtk_epoll.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RTKpp.dir/rtk_epoll.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nins/Code/RTKpp/rtk_epoll.cpp -o CMakeFiles/RTKpp.dir/rtk_epoll.cpp.s

CMakeFiles/RTKpp.dir/rtk_timer.cpp.o: CMakeFiles/RTKpp.dir/flags.make
CMakeFiles/RTKpp.dir/rtk_timer.cpp.o: ../rtk_timer.cpp
CMakeFiles/RTKpp.dir/rtk_timer.cpp.o: CMakeFiles/RTKpp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nins/Code/RTKpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/RTKpp.dir/rtk_timer.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RTKpp.dir/rtk_timer.cpp.o -MF CMakeFiles/RTKpp.dir/rtk_timer.cpp.o.d -o CMakeFiles/RTKpp.dir/rtk_timer.cpp.o -c /home/nins/Code/RTKpp/rtk_timer.cpp

CMakeFiles/RTKpp.dir/rtk_timer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RTKpp.dir/rtk_timer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nins/Code/RTKpp/rtk_timer.cpp > CMakeFiles/RTKpp.dir/rtk_timer.cpp.i

CMakeFiles/RTKpp.dir/rtk_timer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RTKpp.dir/rtk_timer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nins/Code/RTKpp/rtk_timer.cpp -o CMakeFiles/RTKpp.dir/rtk_timer.cpp.s

# Object files for target RTKpp
RTKpp_OBJECTS = \
"CMakeFiles/RTKpp.dir/main.cpp.o" \
"CMakeFiles/RTKpp.dir/utils.cpp.o" \
"CMakeFiles/RTKpp.dir/rtk_request.cpp.o" \
"CMakeFiles/RTKpp.dir/rtk_response.cpp.o" \
"CMakeFiles/RTKpp.dir/rtk_io.cpp.o" \
"CMakeFiles/RTKpp.dir/rtk_threadpool.cpp.o" \
"CMakeFiles/RTKpp.dir/rtk_epoll.cpp.o" \
"CMakeFiles/RTKpp.dir/rtk_timer.cpp.o"

# External object files for target RTKpp
RTKpp_EXTERNAL_OBJECTS =

RTKpp: CMakeFiles/RTKpp.dir/main.cpp.o
RTKpp: CMakeFiles/RTKpp.dir/utils.cpp.o
RTKpp: CMakeFiles/RTKpp.dir/rtk_request.cpp.o
RTKpp: CMakeFiles/RTKpp.dir/rtk_response.cpp.o
RTKpp: CMakeFiles/RTKpp.dir/rtk_io.cpp.o
RTKpp: CMakeFiles/RTKpp.dir/rtk_threadpool.cpp.o
RTKpp: CMakeFiles/RTKpp.dir/rtk_epoll.cpp.o
RTKpp: CMakeFiles/RTKpp.dir/rtk_timer.cpp.o
RTKpp: CMakeFiles/RTKpp.dir/build.make
RTKpp: lib/libgtest.a
RTKpp: CMakeFiles/RTKpp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/nins/Code/RTKpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Linking CXX executable RTKpp"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/RTKpp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/RTKpp.dir/build: RTKpp
.PHONY : CMakeFiles/RTKpp.dir/build

CMakeFiles/RTKpp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/RTKpp.dir/cmake_clean.cmake
.PHONY : CMakeFiles/RTKpp.dir/clean

CMakeFiles/RTKpp.dir/depend:
	cd /home/nins/Code/RTKpp/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nins/Code/RTKpp /home/nins/Code/RTKpp /home/nins/Code/RTKpp/build /home/nins/Code/RTKpp/build /home/nins/Code/RTKpp/build/CMakeFiles/RTKpp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/RTKpp.dir/depend

