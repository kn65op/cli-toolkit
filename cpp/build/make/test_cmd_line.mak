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
.PHONY: test_cmd_line.default
test_cmd_line.default: $(.DEFAULT_GOAL) ;

# Includes
CLI_DIR := ../../..
PROJECT = test_cmd_line
SRC_DIR = $(CLI_DIR)/cpp/tests/cmd_line
PROJ_INCLUDES += -I$(CLI_DIR)/cpp/include
PROJ_INCLUDES += -I$(CLI_DIR)/cpp/src
PROJ_INCLUDES += -I$(CLI_DIR)/cpp/tests/include
PROJ_INCLUDES += -I$(CLI_DIR)/cpp/tests/cmd_line
CPP_FILES = $(wildcard $(SRC_DIR)/*.cpp)
CPP_FILES += $(CLI_DIR)/cpp/src/cli.cpp
CPP_FILES += $(CLI_DIR)/cpp/src/consistency.cpp
CPP_FILES += $(CLI_DIR)/cpp/src/command_line.cpp
CPP_FILES += $(CLI_DIR)/cpp/src/command_line_edition.cpp
CPP_FILES += $(CLI_DIR)/cpp/src/command_line_history.cpp
CPP_FILES += $(CLI_DIR)/cpp/src/config_menu.cpp
CPP_FILES += $(CLI_DIR)/cpp/src/debug.cpp
CPP_FILES += $(CLI_DIR)/cpp/src/element.cpp
CPP_FILES += $(CLI_DIR)/cpp/src/endl.cpp
CPP_FILES += $(CLI_DIR)/cpp/src/exec_context.cpp
CPP_FILES += $(CLI_DIR)/cpp/src/exec_context_manager.cpp
CPP_FILES += $(CLI_DIR)/cpp/src/help.cpp
CPP_FILES += $(CLI_DIR)/cpp/src/io_device.cpp
CPP_FILES += $(CLI_DIR)/cpp/src/keyword.cpp
CPP_FILES += $(CLI_DIR)/cpp/src/menu.cpp
CPP_FILES += $(CLI_DIR)/cpp/src/non_blocking_io_device.cpp
CPP_FILES += $(CLI_DIR)/cpp/src/object.cpp
CPP_FILES += $(CLI_DIR)/cpp/src/param.cpp
CPP_FILES += $(CLI_DIR)/cpp/src/param_string.cpp
CPP_FILES += $(CLI_DIR)/cpp/src/resource_string.cpp
CPP_FILES += $(CLI_DIR)/cpp/src/shell.cpp
CPP_FILES += $(CLI_DIR)/cpp/src/string_device.cpp
CPP_FILES += $(CLI_DIR)/cpp/src/syntax_node.cpp
CPP_FILES += $(CLI_DIR)/cpp/src/syntax_tag.cpp
CPP_FILES += $(CLI_DIR)/cpp/src/tk.cpp
CPP_FILES += $(CLI_DIR)/cpp/src/traces.cpp
CPP_FILES += $(CLI_DIR)/cpp/src/traces_menu.cpp

PROJ_CPP_FLAGS += -DCLI_NO_STL
PROJ_CPP_FLAGS += -DCLI_MAX_CMD_LINE_LENGTH=256
PROJ_CPP_FLAGS += -DCLI_MAX_CMD_LINE_WORD_COUNT=32
PROJ_CPP_FLAGS += -DCLI_MAX_WORD_LENGTH=16

INT_DIR = $(TARGET)$(CXX)/$(RDX)/__$(PROJECT)
OUT_DIR = $(TARGET)$(CXX)/$(RDX)/__$(PROJECT)

include _build.mak

# Rules
.PHONY: check
check: build
	$(PRODUCT)

# Debug and help
include $(CLI_DIR)/build/make/_help.mak

.PHONY: $(CLI_DIR)/cpp/build/make/test_cmd_line.help
help: $(CLI_DIR)/cpp/build/make/test_cmd_line.help
$(CLI_DIR)/cpp/build/make/test_cmd_line.help:
	$(call PrintHelp, check, Run unit-test)

# Dependencies
include $(AUTO_DEPS_FILE)
