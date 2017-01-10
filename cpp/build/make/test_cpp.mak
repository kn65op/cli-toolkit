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
.DEFAULT_GOAL = check
.PHONY: test_cpp.default
test_cpp.default: $(.DEFAULT_GOAL) ;

# Includes
CLI_DIR := ../../..
PROJECT = test_cpp
PROJECT_DEPS = libclicpp.mak
SRC_DIR = $(CLI_DIR)/cpp/tests/cpp
PROJ_INCLUDES += -I$(CLI_DIR)/cpp/include
PROJ_INCLUDES += -I$(CLI_DIR)/cpp/tests/include
PROJ_LIBS = -L$(TARGET)$(CXX)/$(RDX) -lclicpp -lncurses
INT_DIR = $(TARGET)$(CXX)/$(RDX)/__$(PROJECT)
OUT_DIR = $(TARGET)$(CXX)/$(RDX)/__$(PROJECT)
include _build.mak

# Rules
.PHONY: check
check: build ;

# Dependencies
include $(AUTO_DEPS_FILE)
$(PRODUCT): $(CLI_CPP_LIB)

# Debug and help
include $(CLI_DIR)/build/make/_help.mak

.PHONY: $(CLI_DIR)/cpp/build/make/test_cpp.help
help: $(CLI_DIR)/cpp/build/make/test_cpp.help
$(CLI_DIR)/cpp/build/make/test_cpp.help:
	$(call PrintHelp, check, Run unit-test)
