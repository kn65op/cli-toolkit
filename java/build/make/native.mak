# Copyright (c) 2006-2013, Alexis Royer, http://alexis.royer.free.fr/CLI
#
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
#
#     * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation
#       and/or other materials provided with the distribution.
#     * Neither the name of the CLI library project nor the names of its contributors may be used to endorse or promote products derived from this software
#       without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


# Default goal
.DEFAULT_GOAL = build
.PHONY: native.default
native.default: $(.DEFAULT_GOAL) ;


# Includes
CLI_DIR := ../../..
include _vars.mak
include $(CLI_DIR)/cpp/build/make/_vars.mak

PROJECT = native
PRODUCT_TYPE = DYN_LIB
PRODUCT = $(JAVA_DYN_LIB)
PROJ_DEPS = jni.mak
SRC_DIR = $(NATIVE_DIR)
ifeq ($(TARGET),Cygwin)
CPP_FILES = $(wildcard $(SRC_DIR)/*.cpp) $(filter-out $(CLI_DIR)/cpp/src/ncurses_console.cpp,$(wildcard $(CLI_DIR)/cpp/src/*.cpp))
deps: CPP_FILES += $(CLI_DIR)/cpp/src/ncurses_console.cpp
endif
ifeq ($(TARGET),Linux)
CPP_FILES = $(wildcard $(SRC_DIR)/*.cpp) $(filter-out $(CLI_DIR)/cpp/src/win_console.cpp,$(wildcard $(CLI_DIR)/cpp/src/*.cpp))
deps: CPP_FILES += $(CLI_DIR)/cpp/src/win_console.cpp
endif
AUTO_DEPS = yes
INT_DIR = $(OUT_DIR)/$(TARGET)$(CXX)/__$(PROJECT)
ifeq ($(TARGET),Linux)
PROJ_LIBS = -lncurses
endif
ifeq ($(TARGET),Cygwin)
PROJ_CPP_FLAGS = -mno-cygwin -DCLI_NO_REGEX -DCLI_WIN_NETWORK
CPP_FLAGS = $(CPP_DEBUG_FLAG) $(CPP_OS_FLAGS) $(PROJ_CPP_FLAGS)
PROJ_LIBS = -L/cygdrive/c/cygwin/lib/gcc/i686-pc-mingw32/3.4.4 -mno-cygwin -lwsock32
endif
PROJ_INCLUDES += -isystem "$(JDK_DIR)/include"
ifeq ($(TARGET),Cygwin)
PROJ_INCLUDES += -isystem "$(JDK_DIR)/include/win32"
deps: PROJ_INCLUDES += -isystem /usr/include
endif
ifeq ($(TARGET),Linux)
PROJ_INCLUDES += -isystem "$(JDK_DIR)/include/linux"
endif
PROJ_INCLUDES += -I$(CLI_DIR)/cpp/include
INCLUDES = $(PROJ_INCLUDES)
CLEAN_DIR = $(INT_DIR) $(OUT_DIR)/$(TARGET)$(CXX)
include $(CLI_DIR)/cpp/build/make/_build.mak

.PHONY: deps.pre
deps.pre:
	$(MAKE) -f libclijava.mak
	$(MAKE) -f jni.mak

# Dependencies.
include $(AUTO_DEPS_FILE)

deps: deps.pre
