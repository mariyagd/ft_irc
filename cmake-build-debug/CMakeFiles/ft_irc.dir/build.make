# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/mariyadancheva/Documents/GitHub/ft_irc

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/mariyadancheva/Documents/GitHub/ft_irc/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/ft_irc.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/ft_irc.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/ft_irc.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ft_irc.dir/flags.make

CMakeFiles/ft_irc.dir/main.cpp.o: CMakeFiles/ft_irc.dir/flags.make
CMakeFiles/ft_irc.dir/main.cpp.o: /Users/mariyadancheva/Documents/GitHub/ft_irc/main.cpp
CMakeFiles/ft_irc.dir/main.cpp.o: CMakeFiles/ft_irc.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mariyadancheva/Documents/GitHub/ft_irc/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ft_irc.dir/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ft_irc.dir/main.cpp.o -MF CMakeFiles/ft_irc.dir/main.cpp.o.d -o CMakeFiles/ft_irc.dir/main.cpp.o -c /Users/mariyadancheva/Documents/GitHub/ft_irc/main.cpp

CMakeFiles/ft_irc.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ft_irc.dir/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/mariyadancheva/Documents/GitHub/ft_irc/main.cpp > CMakeFiles/ft_irc.dir/main.cpp.i

CMakeFiles/ft_irc.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ft_irc.dir/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/mariyadancheva/Documents/GitHub/ft_irc/main.cpp -o CMakeFiles/ft_irc.dir/main.cpp.s

CMakeFiles/ft_irc.dir/Server.cpp.o: CMakeFiles/ft_irc.dir/flags.make
CMakeFiles/ft_irc.dir/Server.cpp.o: /Users/mariyadancheva/Documents/GitHub/ft_irc/Server.cpp
CMakeFiles/ft_irc.dir/Server.cpp.o: CMakeFiles/ft_irc.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mariyadancheva/Documents/GitHub/ft_irc/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/ft_irc.dir/Server.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ft_irc.dir/Server.cpp.o -MF CMakeFiles/ft_irc.dir/Server.cpp.o.d -o CMakeFiles/ft_irc.dir/Server.cpp.o -c /Users/mariyadancheva/Documents/GitHub/ft_irc/Server.cpp

CMakeFiles/ft_irc.dir/Server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ft_irc.dir/Server.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/mariyadancheva/Documents/GitHub/ft_irc/Server.cpp > CMakeFiles/ft_irc.dir/Server.cpp.i

CMakeFiles/ft_irc.dir/Server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ft_irc.dir/Server.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/mariyadancheva/Documents/GitHub/ft_irc/Server.cpp -o CMakeFiles/ft_irc.dir/Server.cpp.s

CMakeFiles/ft_irc.dir/Client.cpp.o: CMakeFiles/ft_irc.dir/flags.make
CMakeFiles/ft_irc.dir/Client.cpp.o: /Users/mariyadancheva/Documents/GitHub/ft_irc/Client.cpp
CMakeFiles/ft_irc.dir/Client.cpp.o: CMakeFiles/ft_irc.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mariyadancheva/Documents/GitHub/ft_irc/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/ft_irc.dir/Client.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ft_irc.dir/Client.cpp.o -MF CMakeFiles/ft_irc.dir/Client.cpp.o.d -o CMakeFiles/ft_irc.dir/Client.cpp.o -c /Users/mariyadancheva/Documents/GitHub/ft_irc/Client.cpp

CMakeFiles/ft_irc.dir/Client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ft_irc.dir/Client.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/mariyadancheva/Documents/GitHub/ft_irc/Client.cpp > CMakeFiles/ft_irc.dir/Client.cpp.i

CMakeFiles/ft_irc.dir/Client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ft_irc.dir/Client.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/mariyadancheva/Documents/GitHub/ft_irc/Client.cpp -o CMakeFiles/ft_irc.dir/Client.cpp.s

CMakeFiles/ft_irc.dir/RPL.cpp.o: CMakeFiles/ft_irc.dir/flags.make
CMakeFiles/ft_irc.dir/RPL.cpp.o: /Users/mariyadancheva/Documents/GitHub/ft_irc/RPL.cpp
CMakeFiles/ft_irc.dir/RPL.cpp.o: CMakeFiles/ft_irc.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mariyadancheva/Documents/GitHub/ft_irc/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/ft_irc.dir/RPL.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ft_irc.dir/RPL.cpp.o -MF CMakeFiles/ft_irc.dir/RPL.cpp.o.d -o CMakeFiles/ft_irc.dir/RPL.cpp.o -c /Users/mariyadancheva/Documents/GitHub/ft_irc/RPL.cpp

CMakeFiles/ft_irc.dir/RPL.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ft_irc.dir/RPL.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/mariyadancheva/Documents/GitHub/ft_irc/RPL.cpp > CMakeFiles/ft_irc.dir/RPL.cpp.i

CMakeFiles/ft_irc.dir/RPL.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ft_irc.dir/RPL.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/mariyadancheva/Documents/GitHub/ft_irc/RPL.cpp -o CMakeFiles/ft_irc.dir/RPL.cpp.s

CMakeFiles/ft_irc.dir/Commands.cpp.o: CMakeFiles/ft_irc.dir/flags.make
CMakeFiles/ft_irc.dir/Commands.cpp.o: /Users/mariyadancheva/Documents/GitHub/ft_irc/Commands.cpp
CMakeFiles/ft_irc.dir/Commands.cpp.o: CMakeFiles/ft_irc.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mariyadancheva/Documents/GitHub/ft_irc/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/ft_irc.dir/Commands.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ft_irc.dir/Commands.cpp.o -MF CMakeFiles/ft_irc.dir/Commands.cpp.o.d -o CMakeFiles/ft_irc.dir/Commands.cpp.o -c /Users/mariyadancheva/Documents/GitHub/ft_irc/Commands.cpp

CMakeFiles/ft_irc.dir/Commands.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ft_irc.dir/Commands.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/mariyadancheva/Documents/GitHub/ft_irc/Commands.cpp > CMakeFiles/ft_irc.dir/Commands.cpp.i

CMakeFiles/ft_irc.dir/Commands.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ft_irc.dir/Commands.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/mariyadancheva/Documents/GitHub/ft_irc/Commands.cpp -o CMakeFiles/ft_irc.dir/Commands.cpp.s

CMakeFiles/ft_irc.dir/Channel.cpp.o: CMakeFiles/ft_irc.dir/flags.make
CMakeFiles/ft_irc.dir/Channel.cpp.o: /Users/mariyadancheva/Documents/GitHub/ft_irc/Channel.cpp
CMakeFiles/ft_irc.dir/Channel.cpp.o: CMakeFiles/ft_irc.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mariyadancheva/Documents/GitHub/ft_irc/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/ft_irc.dir/Channel.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ft_irc.dir/Channel.cpp.o -MF CMakeFiles/ft_irc.dir/Channel.cpp.o.d -o CMakeFiles/ft_irc.dir/Channel.cpp.o -c /Users/mariyadancheva/Documents/GitHub/ft_irc/Channel.cpp

CMakeFiles/ft_irc.dir/Channel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ft_irc.dir/Channel.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/mariyadancheva/Documents/GitHub/ft_irc/Channel.cpp > CMakeFiles/ft_irc.dir/Channel.cpp.i

CMakeFiles/ft_irc.dir/Channel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ft_irc.dir/Channel.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/mariyadancheva/Documents/GitHub/ft_irc/Channel.cpp -o CMakeFiles/ft_irc.dir/Channel.cpp.s

CMakeFiles/ft_irc.dir/ChannelMenager.cpp.o: CMakeFiles/ft_irc.dir/flags.make
CMakeFiles/ft_irc.dir/ChannelMenager.cpp.o: /Users/mariyadancheva/Documents/GitHub/ft_irc/ChannelMenager.cpp
CMakeFiles/ft_irc.dir/ChannelMenager.cpp.o: CMakeFiles/ft_irc.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mariyadancheva/Documents/GitHub/ft_irc/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/ft_irc.dir/ChannelMenager.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ft_irc.dir/ChannelMenager.cpp.o -MF CMakeFiles/ft_irc.dir/ChannelMenager.cpp.o.d -o CMakeFiles/ft_irc.dir/ChannelMenager.cpp.o -c /Users/mariyadancheva/Documents/GitHub/ft_irc/ChannelMenager.cpp

CMakeFiles/ft_irc.dir/ChannelMenager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ft_irc.dir/ChannelMenager.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/mariyadancheva/Documents/GitHub/ft_irc/ChannelMenager.cpp > CMakeFiles/ft_irc.dir/ChannelMenager.cpp.i

CMakeFiles/ft_irc.dir/ChannelMenager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ft_irc.dir/ChannelMenager.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/mariyadancheva/Documents/GitHub/ft_irc/ChannelMenager.cpp -o CMakeFiles/ft_irc.dir/ChannelMenager.cpp.s

# Object files for target ft_irc
ft_irc_OBJECTS = \
"CMakeFiles/ft_irc.dir/main.cpp.o" \
"CMakeFiles/ft_irc.dir/Server.cpp.o" \
"CMakeFiles/ft_irc.dir/Client.cpp.o" \
"CMakeFiles/ft_irc.dir/RPL.cpp.o" \
"CMakeFiles/ft_irc.dir/Commands.cpp.o" \
"CMakeFiles/ft_irc.dir/Channel.cpp.o" \
"CMakeFiles/ft_irc.dir/ChannelMenager.cpp.o"

# External object files for target ft_irc
ft_irc_EXTERNAL_OBJECTS =

ft_irc: CMakeFiles/ft_irc.dir/main.cpp.o
ft_irc: CMakeFiles/ft_irc.dir/Server.cpp.o
ft_irc: CMakeFiles/ft_irc.dir/Client.cpp.o
ft_irc: CMakeFiles/ft_irc.dir/RPL.cpp.o
ft_irc: CMakeFiles/ft_irc.dir/Commands.cpp.o
ft_irc: CMakeFiles/ft_irc.dir/Channel.cpp.o
ft_irc: CMakeFiles/ft_irc.dir/ChannelMenager.cpp.o
ft_irc: CMakeFiles/ft_irc.dir/build.make
ft_irc: CMakeFiles/ft_irc.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/mariyadancheva/Documents/GitHub/ft_irc/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX executable ft_irc"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ft_irc.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ft_irc.dir/build: ft_irc
.PHONY : CMakeFiles/ft_irc.dir/build

CMakeFiles/ft_irc.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ft_irc.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ft_irc.dir/clean

CMakeFiles/ft_irc.dir/depend:
	cd /Users/mariyadancheva/Documents/GitHub/ft_irc/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/mariyadancheva/Documents/GitHub/ft_irc /Users/mariyadancheva/Documents/GitHub/ft_irc /Users/mariyadancheva/Documents/GitHub/ft_irc/cmake-build-debug /Users/mariyadancheva/Documents/GitHub/ft_irc/cmake-build-debug /Users/mariyadancheva/Documents/GitHub/ft_irc/cmake-build-debug/CMakeFiles/ft_irc.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ft_irc.dir/depend

