# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/clion-2020.1.1/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /opt/clion-2020.1.1/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/thirstea/Documents/projects/cpp/E2022_Chess

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/thirstea/Documents/projects/cpp/E2022_Chess

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/opt/clion-2020.1.1/bin/cmake/linux/bin/cmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/opt/clion-2020.1.1/bin/cmake/linux/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/thirstea/Documents/projects/cpp/E2022_Chess/CMakeFiles /home/thirstea/Documents/projects/cpp/E2022_Chess/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/thirstea/Documents/projects/cpp/E2022_Chess/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named best-listener-2022

# Build rule for target.
best-listener-2022: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 best-listener-2022
.PHONY : best-listener-2022

# fast build rule for target.
best-listener-2022/fast:
	$(MAKE) -f CMakeFiles/best-listener-2022.dir/build.make CMakeFiles/best-listener-2022.dir/build
.PHONY : best-listener-2022/fast

#=============================================================================
# Target rules for targets named listener

# Build rule for target.
listener: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 listener
.PHONY : listener

# fast build rule for target.
listener/fast:
	$(MAKE) -f CMakeFiles/listener.dir/build.make CMakeFiles/listener.dir/build
.PHONY : listener/fast

#=============================================================================
# Target rules for targets named chessengine

# Build rule for target.
chessengine: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 chessengine
.PHONY : chessengine

# fast build rule for target.
chessengine/fast:
	$(MAKE) -f CMakeFiles/chessengine.dir/build.make CMakeFiles/chessengine.dir/build
.PHONY : chessengine/fast

src/ai/uci.o: src/ai/uci.cc.o

.PHONY : src/ai/uci.o

# target to build an object file
src/ai/uci.cc.o:
	$(MAKE) -f CMakeFiles/best-listener-2022.dir/build.make CMakeFiles/best-listener-2022.dir/src/ai/uci.cc.o
	$(MAKE) -f CMakeFiles/chessengine.dir/build.make CMakeFiles/chessengine.dir/src/ai/uci.cc.o
.PHONY : src/ai/uci.cc.o

src/ai/uci.i: src/ai/uci.cc.i

.PHONY : src/ai/uci.i

# target to preprocess a source file
src/ai/uci.cc.i:
	$(MAKE) -f CMakeFiles/best-listener-2022.dir/build.make CMakeFiles/best-listener-2022.dir/src/ai/uci.cc.i
	$(MAKE) -f CMakeFiles/chessengine.dir/build.make CMakeFiles/chessengine.dir/src/ai/uci.cc.i
.PHONY : src/ai/uci.cc.i

src/ai/uci.s: src/ai/uci.cc.s

.PHONY : src/ai/uci.s

# target to generate assembly for a file
src/ai/uci.cc.s:
	$(MAKE) -f CMakeFiles/best-listener-2022.dir/build.make CMakeFiles/best-listener-2022.dir/src/ai/uci.cc.s
	$(MAKE) -f CMakeFiles/chessengine.dir/build.make CMakeFiles/chessengine.dir/src/ai/uci.cc.s
.PHONY : src/ai/uci.cc.s

src/best-listener-2022/best-listener-2022.o: src/best-listener-2022/best-listener-2022.cc.o

.PHONY : src/best-listener-2022/best-listener-2022.o

# target to build an object file
src/best-listener-2022/best-listener-2022.cc.o:
	$(MAKE) -f CMakeFiles/best-listener-2022.dir/build.make CMakeFiles/best-listener-2022.dir/src/best-listener-2022/best-listener-2022.cc.o
.PHONY : src/best-listener-2022/best-listener-2022.cc.o

src/best-listener-2022/best-listener-2022.i: src/best-listener-2022/best-listener-2022.cc.i

.PHONY : src/best-listener-2022/best-listener-2022.i

# target to preprocess a source file
src/best-listener-2022/best-listener-2022.cc.i:
	$(MAKE) -f CMakeFiles/best-listener-2022.dir/build.make CMakeFiles/best-listener-2022.dir/src/best-listener-2022/best-listener-2022.cc.i
.PHONY : src/best-listener-2022/best-listener-2022.cc.i

src/best-listener-2022/best-listener-2022.s: src/best-listener-2022/best-listener-2022.cc.s

.PHONY : src/best-listener-2022/best-listener-2022.s

# target to generate assembly for a file
src/best-listener-2022/best-listener-2022.cc.s:
	$(MAKE) -f CMakeFiles/best-listener-2022.dir/build.make CMakeFiles/best-listener-2022.dir/src/best-listener-2022/best-listener-2022.cc.s
.PHONY : src/best-listener-2022/best-listener-2022.cc.s

src/chessengine/board/chessboard-interface-impl.o: src/chessengine/board/chessboard-interface-impl.cc.o

.PHONY : src/chessengine/board/chessboard-interface-impl.o

# target to build an object file
src/chessengine/board/chessboard-interface-impl.cc.o:
	$(MAKE) -f CMakeFiles/best-listener-2022.dir/build.make CMakeFiles/best-listener-2022.dir/src/chessengine/board/chessboard-interface-impl.cc.o
	$(MAKE) -f CMakeFiles/chessengine.dir/build.make CMakeFiles/chessengine.dir/src/chessengine/board/chessboard-interface-impl.cc.o
.PHONY : src/chessengine/board/chessboard-interface-impl.cc.o

src/chessengine/board/chessboard-interface-impl.i: src/chessengine/board/chessboard-interface-impl.cc.i

.PHONY : src/chessengine/board/chessboard-interface-impl.i

# target to preprocess a source file
src/chessengine/board/chessboard-interface-impl.cc.i:
	$(MAKE) -f CMakeFiles/best-listener-2022.dir/build.make CMakeFiles/best-listener-2022.dir/src/chessengine/board/chessboard-interface-impl.cc.i
	$(MAKE) -f CMakeFiles/chessengine.dir/build.make CMakeFiles/chessengine.dir/src/chessengine/board/chessboard-interface-impl.cc.i
.PHONY : src/chessengine/board/chessboard-interface-impl.cc.i

src/chessengine/board/chessboard-interface-impl.s: src/chessengine/board/chessboard-interface-impl.cc.s

.PHONY : src/chessengine/board/chessboard-interface-impl.s

# target to generate assembly for a file
src/chessengine/board/chessboard-interface-impl.cc.s:
	$(MAKE) -f CMakeFiles/best-listener-2022.dir/build.make CMakeFiles/best-listener-2022.dir/src/chessengine/board/chessboard-interface-impl.cc.s
	$(MAKE) -f CMakeFiles/chessengine.dir/build.make CMakeFiles/chessengine.dir/src/chessengine/board/chessboard-interface-impl.cc.s
.PHONY : src/chessengine/board/chessboard-interface-impl.cc.s

src/chessengine/board/chessboard.o: src/chessengine/board/chessboard.cc.o

.PHONY : src/chessengine/board/chessboard.o

# target to build an object file
src/chessengine/board/chessboard.cc.o:
	$(MAKE) -f CMakeFiles/best-listener-2022.dir/build.make CMakeFiles/best-listener-2022.dir/src/chessengine/board/chessboard.cc.o
	$(MAKE) -f CMakeFiles/chessengine.dir/build.make CMakeFiles/chessengine.dir/src/chessengine/board/chessboard.cc.o
.PHONY : src/chessengine/board/chessboard.cc.o

src/chessengine/board/chessboard.i: src/chessengine/board/chessboard.cc.i

.PHONY : src/chessengine/board/chessboard.i

# target to preprocess a source file
src/chessengine/board/chessboard.cc.i:
	$(MAKE) -f CMakeFiles/best-listener-2022.dir/build.make CMakeFiles/best-listener-2022.dir/src/chessengine/board/chessboard.cc.i
	$(MAKE) -f CMakeFiles/chessengine.dir/build.make CMakeFiles/chessengine.dir/src/chessengine/board/chessboard.cc.i
.PHONY : src/chessengine/board/chessboard.cc.i

src/chessengine/board/chessboard.s: src/chessengine/board/chessboard.cc.s

.PHONY : src/chessengine/board/chessboard.s

# target to generate assembly for a file
src/chessengine/board/chessboard.cc.s:
	$(MAKE) -f CMakeFiles/best-listener-2022.dir/build.make CMakeFiles/best-listener-2022.dir/src/chessengine/board/chessboard.cc.s
	$(MAKE) -f CMakeFiles/chessengine.dir/build.make CMakeFiles/chessengine.dir/src/chessengine/board/chessboard.cc.s
.PHONY : src/chessengine/board/chessboard.cc.s

src/chessengine/board/color.o: src/chessengine/board/color.cc.o

.PHONY : src/chessengine/board/color.o

# target to build an object file
src/chessengine/board/color.cc.o:
	$(MAKE) -f CMakeFiles/best-listener-2022.dir/build.make CMakeFiles/best-listener-2022.dir/src/chessengine/board/color.cc.o
	$(MAKE) -f CMakeFiles/chessengine.dir/build.make CMakeFiles/chessengine.dir/src/chessengine/board/color.cc.o
.PHONY : src/chessengine/board/color.cc.o

src/chessengine/board/color.i: src/chessengine/board/color.cc.i

.PHONY : src/chessengine/board/color.i

# target to preprocess a source file
src/chessengine/board/color.cc.i:
	$(MAKE) -f CMakeFiles/best-listener-2022.dir/build.make CMakeFiles/best-listener-2022.dir/src/chessengine/board/color.cc.i
	$(MAKE) -f CMakeFiles/chessengine.dir/build.make CMakeFiles/chessengine.dir/src/chessengine/board/color.cc.i
.PHONY : src/chessengine/board/color.cc.i

src/chessengine/board/color.s: src/chessengine/board/color.cc.s

.PHONY : src/chessengine/board/color.s

# target to generate assembly for a file
src/chessengine/board/color.cc.s:
	$(MAKE) -f CMakeFiles/best-listener-2022.dir/build.make CMakeFiles/best-listener-2022.dir/src/chessengine/board/color.cc.s
	$(MAKE) -f CMakeFiles/chessengine.dir/build.make CMakeFiles/chessengine.dir/src/chessengine/board/color.cc.s
.PHONY : src/chessengine/board/color.cc.s

src/chessengine/board/move-builder.o: src/chessengine/board/move-builder.cc.o

.PHONY : src/chessengine/board/move-builder.o

# target to build an object file
src/chessengine/board/move-builder.cc.o:
	$(MAKE) -f CMakeFiles/best-listener-2022.dir/build.make CMakeFiles/best-listener-2022.dir/src/chessengine/board/move-builder.cc.o
	$(MAKE) -f CMakeFiles/chessengine.dir/build.make CMakeFiles/chessengine.dir/src/chessengine/board/move-builder.cc.o
.PHONY : src/chessengine/board/move-builder.cc.o

src/chessengine/board/move-builder.i: src/chessengine/board/move-builder.cc.i

.PHONY : src/chessengine/board/move-builder.i

# target to preprocess a source file
src/chessengine/board/move-builder.cc.i:
	$(MAKE) -f CMakeFiles/best-listener-2022.dir/build.make CMakeFiles/best-listener-2022.dir/src/chessengine/board/move-builder.cc.i
	$(MAKE) -f CMakeFiles/chessengine.dir/build.make CMakeFiles/chessengine.dir/src/chessengine/board/move-builder.cc.i
.PHONY : src/chessengine/board/move-builder.cc.i

src/chessengine/board/move-builder.s: src/chessengine/board/move-builder.cc.s

.PHONY : src/chessengine/board/move-builder.s

# target to generate assembly for a file
src/chessengine/board/move-builder.cc.s:
	$(MAKE) -f CMakeFiles/best-listener-2022.dir/build.make CMakeFiles/best-listener-2022.dir/src/chessengine/board/move-builder.cc.s
	$(MAKE) -f CMakeFiles/chessengine.dir/build.make CMakeFiles/chessengine.dir/src/chessengine/board/move-builder.cc.s
.PHONY : src/chessengine/board/move-builder.cc.s

src/chessengine/board/move-legality-checker.o: src/chessengine/board/move-legality-checker.cc.o

.PHONY : src/chessengine/board/move-legality-checker.o

# target to build an object file
src/chessengine/board/move-legality-checker.cc.o:
	$(MAKE) -f CMakeFiles/best-listener-2022.dir/build.make CMakeFiles/best-listener-2022.dir/src/chessengine/board/move-legality-checker.cc.o
	$(MAKE) -f CMakeFiles/chessengine.dir/build.make CMakeFiles/chessengine.dir/src/chessengine/board/move-legality-checker.cc.o
.PHONY : src/chessengine/board/move-legality-checker.cc.o

src/chessengine/board/move-legality-checker.i: src/chessengine/board/move-legality-checker.cc.i

.PHONY : src/chessengine/board/move-legality-checker.i

# target to preprocess a source file
src/chessengine/board/move-legality-checker.cc.i:
	$(MAKE) -f CMakeFiles/best-listener-2022.dir/build.make CMakeFiles/best-listener-2022.dir/src/chessengine/board/move-legality-checker.cc.i
	$(MAKE) -f CMakeFiles/chessengine.dir/build.make CMakeFiles/chessengine.dir/src/chessengine/board/move-legality-checker.cc.i
.PHONY : src/chessengine/board/move-legality-checker.cc.i

src/chessengine/board/move-legality-checker.s: src/chessengine/board/move-legality-checker.cc.s

.PHONY : src/chessengine/board/move-legality-checker.s

# target to generate assembly for a file
src/chessengine/board/move-legality-checker.cc.s:
	$(MAKE) -f CMakeFiles/best-listener-2022.dir/build.make CMakeFiles/best-listener-2022.dir/src/chessengine/board/move-legality-checker.cc.s
	$(MAKE) -f CMakeFiles/chessengine.dir/build.make CMakeFiles/chessengine.dir/src/chessengine/board/move-legality-checker.cc.s
.PHONY : src/chessengine/board/move-legality-checker.cc.s

src/chessengine/board/move.o: src/chessengine/board/move.cc.o

.PHONY : src/chessengine/board/move.o

# target to build an object file
src/chessengine/board/move.cc.o:
	$(MAKE) -f CMakeFiles/best-listener-2022.dir/build.make CMakeFiles/best-listener-2022.dir/src/chessengine/board/move.cc.o
	$(MAKE) -f CMakeFiles/chessengine.dir/build.make CMakeFiles/chessengine.dir/src/chessengine/board/move.cc.o
.PHONY : src/chessengine/board/move.cc.o

src/chessengine/board/move.i: src/chessengine/board/move.cc.i

.PHONY : src/chessengine/board/move.i

# target to preprocess a source file
src/chessengine/board/move.cc.i:
	$(MAKE) -f CMakeFiles/best-listener-2022.dir/build.make CMakeFiles/best-listener-2022.dir/src/chessengine/board/move.cc.i
	$(MAKE) -f CMakeFiles/chessengine.dir/build.make CMakeFiles/chessengine.dir/src/chessengine/board/move.cc.i
.PHONY : src/chessengine/board/move.cc.i

src/chessengine/board/move.s: src/chessengine/board/move.cc.s

.PHONY : src/chessengine/board/move.s

# target to generate assembly for a file
src/chessengine/board/move.cc.s:
	$(MAKE) -f CMakeFiles/best-listener-2022.dir/build.make CMakeFiles/best-listener-2022.dir/src/chessengine/board/move.cc.s
	$(MAKE) -f CMakeFiles/chessengine.dir/build.make CMakeFiles/chessengine.dir/src/chessengine/board/move.cc.s
.PHONY : src/chessengine/board/move.cc.s

src/chessengine/board/piece.o: src/chessengine/board/piece.cc.o

.PHONY : src/chessengine/board/piece.o

# target to build an object file
src/chessengine/board/piece.cc.o:
	$(MAKE) -f CMakeFiles/best-listener-2022.dir/build.make CMakeFiles/best-listener-2022.dir/src/chessengine/board/piece.cc.o
	$(MAKE) -f CMakeFiles/chessengine.dir/build.make CMakeFiles/chessengine.dir/src/chessengine/board/piece.cc.o
.PHONY : src/chessengine/board/piece.cc.o

src/chessengine/board/piece.i: src/chessengine/board/piece.cc.i

.PHONY : src/chessengine/board/piece.i

# target to preprocess a source file
src/chessengine/board/piece.cc.i:
	$(MAKE) -f CMakeFiles/best-listener-2022.dir/build.make CMakeFiles/best-listener-2022.dir/src/chessengine/board/piece.cc.i
	$(MAKE) -f CMakeFiles/chessengine.dir/build.make CMakeFiles/chessengine.dir/src/chessengine/board/piece.cc.i
.PHONY : src/chessengine/board/piece.cc.i

src/chessengine/board/piece.s: src/chessengine/board/piece.cc.s

.PHONY : src/chessengine/board/piece.s

# target to generate assembly for a file
src/chessengine/board/piece.cc.s:
	$(MAKE) -f CMakeFiles/best-listener-2022.dir/build.make CMakeFiles/best-listener-2022.dir/src/chessengine/board/piece.cc.s
	$(MAKE) -f CMakeFiles/chessengine.dir/build.make CMakeFiles/chessengine.dir/src/chessengine/board/piece.cc.s
.PHONY : src/chessengine/board/piece.cc.s

src/chessengine/board/position.o: src/chessengine/board/position.cc.o

.PHONY : src/chessengine/board/position.o

# target to build an object file
src/chessengine/board/position.cc.o:
	$(MAKE) -f CMakeFiles/best-listener-2022.dir/build.make CMakeFiles/best-listener-2022.dir/src/chessengine/board/position.cc.o
	$(MAKE) -f CMakeFiles/chessengine.dir/build.make CMakeFiles/chessengine.dir/src/chessengine/board/position.cc.o
.PHONY : src/chessengine/board/position.cc.o

src/chessengine/board/position.i: src/chessengine/board/position.cc.i

.PHONY : src/chessengine/board/position.i

# target to preprocess a source file
src/chessengine/board/position.cc.i:
	$(MAKE) -f CMakeFiles/best-listener-2022.dir/build.make CMakeFiles/best-listener-2022.dir/src/chessengine/board/position.cc.i
	$(MAKE) -f CMakeFiles/chessengine.dir/build.make CMakeFiles/chessengine.dir/src/chessengine/board/position.cc.i
.PHONY : src/chessengine/board/position.cc.i

src/chessengine/board/position.s: src/chessengine/board/position.cc.s

.PHONY : src/chessengine/board/position.s

# target to generate assembly for a file
src/chessengine/board/position.cc.s:
	$(MAKE) -f CMakeFiles/best-listener-2022.dir/build.make CMakeFiles/best-listener-2022.dir/src/chessengine/board/position.cc.s
	$(MAKE) -f CMakeFiles/chessengine.dir/build.make CMakeFiles/chessengine.dir/src/chessengine/board/position.cc.s
.PHONY : src/chessengine/board/position.cc.s

src/chessengine/pgn/pgn-exception.o: src/chessengine/pgn/pgn-exception.cc.o

.PHONY : src/chessengine/pgn/pgn-exception.o

# target to build an object file
src/chessengine/pgn/pgn-exception.cc.o:
	$(MAKE) -f CMakeFiles/best-listener-2022.dir/build.make CMakeFiles/best-listener-2022.dir/src/chessengine/pgn/pgn-exception.cc.o
	$(MAKE) -f CMakeFiles/chessengine.dir/build.make CMakeFiles/chessengine.dir/src/chessengine/pgn/pgn-exception.cc.o
.PHONY : src/chessengine/pgn/pgn-exception.cc.o

src/chessengine/pgn/pgn-exception.i: src/chessengine/pgn/pgn-exception.cc.i

.PHONY : src/chessengine/pgn/pgn-exception.i

# target to preprocess a source file
src/chessengine/pgn/pgn-exception.cc.i:
	$(MAKE) -f CMakeFiles/best-listener-2022.dir/build.make CMakeFiles/best-listener-2022.dir/src/chessengine/pgn/pgn-exception.cc.i
	$(MAKE) -f CMakeFiles/chessengine.dir/build.make CMakeFiles/chessengine.dir/src/chessengine/pgn/pgn-exception.cc.i
.PHONY : src/chessengine/pgn/pgn-exception.cc.i

src/chessengine/pgn/pgn-exception.s: src/chessengine/pgn/pgn-exception.cc.s

.PHONY : src/chessengine/pgn/pgn-exception.s

# target to generate assembly for a file
src/chessengine/pgn/pgn-exception.cc.s:
	$(MAKE) -f CMakeFiles/best-listener-2022.dir/build.make CMakeFiles/best-listener-2022.dir/src/chessengine/pgn/pgn-exception.cc.s
	$(MAKE) -f CMakeFiles/chessengine.dir/build.make CMakeFiles/chessengine.dir/src/chessengine/pgn/pgn-exception.cc.s
.PHONY : src/chessengine/pgn/pgn-exception.cc.s

src/chessengine/pgn/pgn-move.o: src/chessengine/pgn/pgn-move.cc.o

.PHONY : src/chessengine/pgn/pgn-move.o

# target to build an object file
src/chessengine/pgn/pgn-move.cc.o:
	$(MAKE) -f CMakeFiles/best-listener-2022.dir/build.make CMakeFiles/best-listener-2022.dir/src/chessengine/pgn/pgn-move.cc.o
	$(MAKE) -f CMakeFiles/chessengine.dir/build.make CMakeFiles/chessengine.dir/src/chessengine/pgn/pgn-move.cc.o
.PHONY : src/chessengine/pgn/pgn-move.cc.o

src/chessengine/pgn/pgn-move.i: src/chessengine/pgn/pgn-move.cc.i

.PHONY : src/chessengine/pgn/pgn-move.i

# target to preprocess a source file
src/chessengine/pgn/pgn-move.cc.i:
	$(MAKE) -f CMakeFiles/best-listener-2022.dir/build.make CMakeFiles/best-listener-2022.dir/src/chessengine/pgn/pgn-move.cc.i
	$(MAKE) -f CMakeFiles/chessengine.dir/build.make CMakeFiles/chessengine.dir/src/chessengine/pgn/pgn-move.cc.i
.PHONY : src/chessengine/pgn/pgn-move.cc.i

src/chessengine/pgn/pgn-move.s: src/chessengine/pgn/pgn-move.cc.s

.PHONY : src/chessengine/pgn/pgn-move.s

# target to generate assembly for a file
src/chessengine/pgn/pgn-move.cc.s:
	$(MAKE) -f CMakeFiles/best-listener-2022.dir/build.make CMakeFiles/best-listener-2022.dir/src/chessengine/pgn/pgn-move.cc.s
	$(MAKE) -f CMakeFiles/chessengine.dir/build.make CMakeFiles/chessengine.dir/src/chessengine/pgn/pgn-move.cc.s
.PHONY : src/chessengine/pgn/pgn-move.cc.s

src/chessengine/pgn/pgn-parser.o: src/chessengine/pgn/pgn-parser.cc.o

.PHONY : src/chessengine/pgn/pgn-parser.o

# target to build an object file
src/chessengine/pgn/pgn-parser.cc.o:
	$(MAKE) -f CMakeFiles/best-listener-2022.dir/build.make CMakeFiles/best-listener-2022.dir/src/chessengine/pgn/pgn-parser.cc.o
	$(MAKE) -f CMakeFiles/chessengine.dir/build.make CMakeFiles/chessengine.dir/src/chessengine/pgn/pgn-parser.cc.o
.PHONY : src/chessengine/pgn/pgn-parser.cc.o

src/chessengine/pgn/pgn-parser.i: src/chessengine/pgn/pgn-parser.cc.i

.PHONY : src/chessengine/pgn/pgn-parser.i

# target to preprocess a source file
src/chessengine/pgn/pgn-parser.cc.i:
	$(MAKE) -f CMakeFiles/best-listener-2022.dir/build.make CMakeFiles/best-listener-2022.dir/src/chessengine/pgn/pgn-parser.cc.i
	$(MAKE) -f CMakeFiles/chessengine.dir/build.make CMakeFiles/chessengine.dir/src/chessengine/pgn/pgn-parser.cc.i
.PHONY : src/chessengine/pgn/pgn-parser.cc.i

src/chessengine/pgn/pgn-parser.s: src/chessengine/pgn/pgn-parser.cc.s

.PHONY : src/chessengine/pgn/pgn-parser.s

# target to generate assembly for a file
src/chessengine/pgn/pgn-parser.cc.s:
	$(MAKE) -f CMakeFiles/best-listener-2022.dir/build.make CMakeFiles/best-listener-2022.dir/src/chessengine/pgn/pgn-parser.cc.s
	$(MAKE) -f CMakeFiles/chessengine.dir/build.make CMakeFiles/chessengine.dir/src/chessengine/pgn/pgn-parser.cc.s
.PHONY : src/chessengine/pgn/pgn-parser.cc.s

src/listeners/listener-manager.o: src/listeners/listener-manager.cc.o

.PHONY : src/listeners/listener-manager.o

# target to build an object file
src/listeners/listener-manager.cc.o:
	$(MAKE) -f CMakeFiles/best-listener-2022.dir/build.make CMakeFiles/best-listener-2022.dir/src/listeners/listener-manager.cc.o
	$(MAKE) -f CMakeFiles/chessengine.dir/build.make CMakeFiles/chessengine.dir/src/listeners/listener-manager.cc.o
.PHONY : src/listeners/listener-manager.cc.o

src/listeners/listener-manager.i: src/listeners/listener-manager.cc.i

.PHONY : src/listeners/listener-manager.i

# target to preprocess a source file
src/listeners/listener-manager.cc.i:
	$(MAKE) -f CMakeFiles/best-listener-2022.dir/build.make CMakeFiles/best-listener-2022.dir/src/listeners/listener-manager.cc.i
	$(MAKE) -f CMakeFiles/chessengine.dir/build.make CMakeFiles/chessengine.dir/src/listeners/listener-manager.cc.i
.PHONY : src/listeners/listener-manager.cc.i

src/listeners/listener-manager.s: src/listeners/listener-manager.cc.s

.PHONY : src/listeners/listener-manager.s

# target to generate assembly for a file
src/listeners/listener-manager.cc.s:
	$(MAKE) -f CMakeFiles/best-listener-2022.dir/build.make CMakeFiles/best-listener-2022.dir/src/listeners/listener-manager.cc.s
	$(MAKE) -f CMakeFiles/chessengine.dir/build.make CMakeFiles/chessengine.dir/src/listeners/listener-manager.cc.s
.PHONY : src/listeners/listener-manager.cc.s

src/main.o: src/main.cc.o

.PHONY : src/main.o

# target to build an object file
src/main.cc.o:
	$(MAKE) -f CMakeFiles/chessengine.dir/build.make CMakeFiles/chessengine.dir/src/main.cc.o
.PHONY : src/main.cc.o

src/main.i: src/main.cc.i

.PHONY : src/main.i

# target to preprocess a source file
src/main.cc.i:
	$(MAKE) -f CMakeFiles/chessengine.dir/build.make CMakeFiles/chessengine.dir/src/main.cc.i
.PHONY : src/main.cc.i

src/main.s: src/main.cc.s

.PHONY : src/main.s

# target to generate assembly for a file
src/main.cc.s:
	$(MAKE) -f CMakeFiles/chessengine.dir/build.make CMakeFiles/chessengine.dir/src/main.cc.s
.PHONY : src/main.cc.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... best-listener-2022"
	@echo "... listener"
	@echo "... edit_cache"
	@echo "... chessengine"
	@echo "... src/ai/uci.o"
	@echo "... src/ai/uci.i"
	@echo "... src/ai/uci.s"
	@echo "... src/best-listener-2022/best-listener-2022.o"
	@echo "... src/best-listener-2022/best-listener-2022.i"
	@echo "... src/best-listener-2022/best-listener-2022.s"
	@echo "... src/chessengine/board/chessboard-interface-impl.o"
	@echo "... src/chessengine/board/chessboard-interface-impl.i"
	@echo "... src/chessengine/board/chessboard-interface-impl.s"
	@echo "... src/chessengine/board/chessboard.o"
	@echo "... src/chessengine/board/chessboard.i"
	@echo "... src/chessengine/board/chessboard.s"
	@echo "... src/chessengine/board/color.o"
	@echo "... src/chessengine/board/color.i"
	@echo "... src/chessengine/board/color.s"
	@echo "... src/chessengine/board/move-builder.o"
	@echo "... src/chessengine/board/move-builder.i"
	@echo "... src/chessengine/board/move-builder.s"
	@echo "... src/chessengine/board/move-legality-checker.o"
	@echo "... src/chessengine/board/move-legality-checker.i"
	@echo "... src/chessengine/board/move-legality-checker.s"
	@echo "... src/chessengine/board/move.o"
	@echo "... src/chessengine/board/move.i"
	@echo "... src/chessengine/board/move.s"
	@echo "... src/chessengine/board/piece.o"
	@echo "... src/chessengine/board/piece.i"
	@echo "... src/chessengine/board/piece.s"
	@echo "... src/chessengine/board/position.o"
	@echo "... src/chessengine/board/position.i"
	@echo "... src/chessengine/board/position.s"
	@echo "... src/chessengine/pgn/pgn-exception.o"
	@echo "... src/chessengine/pgn/pgn-exception.i"
	@echo "... src/chessengine/pgn/pgn-exception.s"
	@echo "... src/chessengine/pgn/pgn-move.o"
	@echo "... src/chessengine/pgn/pgn-move.i"
	@echo "... src/chessengine/pgn/pgn-move.s"
	@echo "... src/chessengine/pgn/pgn-parser.o"
	@echo "... src/chessengine/pgn/pgn-parser.i"
	@echo "... src/chessengine/pgn/pgn-parser.s"
	@echo "... src/listeners/listener-manager.o"
	@echo "... src/listeners/listener-manager.i"
	@echo "... src/listeners/listener-manager.s"
	@echo "... src/main.o"
	@echo "... src/main.i"
	@echo "... src/main.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

