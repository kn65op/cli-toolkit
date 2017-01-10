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
.PHONY: test_tk.default
test_tk.default: $(.DEFAULT_GOAL) ;

# Parameters
USE_STL ?=

# Includes
CLI_DIR := ../../..
include $(CLI_DIR)/build/make/_utils.mak
include _vars.mak
ifneq ($(USE_STL),)
PROJECT = $(call IfEquals,$(USE_STL),yes,test_tk_stl,test_tk_inner)
SRC_DIR = $(CLI_DIR)/cpp/tests/tk
PROJ_INCLUDES += -I$(CLI_DIR)/cpp/include
PROJ_INCLUDES += -I$(CLI_DIR)/cpp/tests/include
PROJ_INCLUDES += -I$(CLI_DIR)/cpp/tests/tk
CPP_FILES = $(wildcard $(SRC_DIR)/*.cpp)
CPP_FILES += $(CLI_DIR)/cpp/src/object.cpp
CPP_FILES += $(CLI_DIR)/cpp/src/tk.cpp
CPP_FILES += $(CLI_DIR)/cpp/src/debug.cpp
CPP_FILES += $(CLI_DIR)/cpp/src/traces.cpp
CPP_FILES += $(CLI_DIR)/cpp/src/io_device.cpp
CPP_FILES += $(CLI_DIR)/cpp/src/string_device.cpp
CPP_FILES += $(CLI_DIR)/cpp/src/help.cpp
CPP_FILES += $(CLI_DIR)/cpp/src/resource_string.cpp
CPP_FILES += $(CLI_DIR)/cpp/src/consistency.cpp
PROJ_CPP_FLAGS += $(call IfEquals,$(USE_STL),no,-DCLI_NO_STL)
INT_DIR = $(TARGET)$(CXX)/$(RDX)/__$(PROJECT)
OUT_DIR = $(TARGET)$(CXX)/$(RDX)/__$(PROJECT)
include _build.mak
endif

# Rules
ifeq ($(USE_STL),)
.PHONY: check
check:
	$(MAKE) USE_STL=yes -f $(firstword $(MAKEFILE_LIST)) check
	$(MAKE) USE_STL=no -f $(firstword $(MAKEFILE_LIST)) check

.PHONY: deps
deps:
	$(MAKE) USE_STL=yes -f $(firstword $(MAKEFILE_LIST)) deps
	$(MAKE) USE_STL=no -f $(firstword $(MAKEFILE_LIST)) deps

.PHONY: clean
clean:
	$(MAKE) USE_STL=yes -f $(firstword $(MAKEFILE_LIST)) clean
	$(MAKE) USE_STL=no -f $(firstword $(MAKEFILE_LIST)) clean

else

.PHONY: check
check: build
	$(PRODUCT)

endif

# Debug and help
include $(CLI_DIR)/build/make/_help.mak

.PHONY: $(CLI_DIR)/cpp/build/make/test_tk.help
help: $(CLI_DIR)/cpp/build/make/test_tk.help
$(CLI_DIR)/cpp/build/make/test_tk.help:
	$(call PrintHelp, check, Run unit-test)

.PHONY: $(CLI_DIR)/cpp/build/make/test_tk.vars
vars: $(CLI_DIR)/cpp/build/make/test_tk.vars
$(CLI_DIR)/cpp/build/make/test_tk.vars:
	$(call ShowVariables,USE_STL)
ifeq ($(USE_STL),)
	$(MAKE) USE_STL=yes -f $(firstword $(MAKEFILE_LIST)) vars
	$(MAKE) USE_STL=no -f $(firstword $(MAKEFILE_LIST)) vars
endif

# Dependencies
ifneq ($(USE_STL),)
include $(AUTO_DEPS_FILE)
endif
