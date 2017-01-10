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
.PHONY: exec_context.default
exec_context.default: $(.DEFAULT_GOAL) ;

# Includes
CLI_DIR := ../../..
PROJECT = test_exec_context
CLI_XML_RES = $(CLI_DIR)/samples/user-guide/empty.xml
SRC_DIR = $(CLI_DIR)/cpp/tests/exec_context
CLI_MAIN_CPP = $(SRC_DIR)/exec_context_test.cpp
INT_DIR = $(TARGET)$(CXX)/$(RDX)/__$(PROJECT)
OUT_DIR = $(TARGET)$(CXX)/$(RDX)/__$(PROJECT)
include _mkres.mak

# Rules
.PHONY: check
check: build $(PRODUCT)
	$(PRODUCT)

# Debug and help
include $(CLI_DIR)/build/make/_help.mak

.PHONY: $(CLI_DIR)/cpp/build/make/exec_context.help
help: $(CLI_DIR)/cpp/build/make/exec_context.help
$(CLI_DIR)/cpp/build/make/exec_context.help:
	$(call PrintHelp, check, Run unit-test)

# Dependencies
$(CLI_MAIN_OBJ): $(wildcard $(CLI_DIR)/cpp/tests/include/*.h) $(wildcard $(CLI_DIR)/samples/user-guide/*.h)
#$(PRODUCT): $(CLI_CPP_LIB) # Already done in _mkres.mak
