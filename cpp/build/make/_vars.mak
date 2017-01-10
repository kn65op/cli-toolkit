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

ifndef __CPP_VARS__
__CPP_VARS__ = 1


CLI_DIR ?= ../../..
include $(CLI_DIR)/build/make/_vars.mak


LINUX ?= $(findstring linux,$(shell $(MAKE) --help))
CYGWIN ?= $(findstring cygwin,$(shell $(MAKE) --help))
ifneq ($(LINUX),)
	TARGET ?= Linux
	BIN_PREFIX ?=
	BIN_SUFFIX ?=
	DYN_LIB_PREFIX ?= lib
	DYN_LIB_SUFFIX ?= .so
	STATIC_LIB_PREFIX ?= lib
	STATIC_LIB_SUFFIX ?= .a
endif
ifneq ($(CYGWIN),)
	TARGET ?= Cygwin
	BIN_PREFIX ?=
	BIN_SUFFIX ?= .exe
	DYN_LIB_PREFIX ?=
	DYN_LIB_SUFFIX ?= .dll
	STATIC_LIB_PREFIX ?= lib
	STATIC_LIB_SUFFIX ?= .a
endif
OUT_DIR ?= $(TARGET)$(CXX)/$(RDX)
CLI_CPP_LIB = $(CLI_DIR)/cpp/build/make/$(TARGET)$(CXX)/$(RDX)/$(STATIC_LIB_PREFIX)clicpp$(STATIC_LIB_SUFFIX)


# Debug and help
include $(CLI_DIR)/build/make/_help.mak

.PHONY: $(CLI_DIR)/cpp/build/make/_vars.vars
vars: $(CLI_DIR)/cpp/build/make/_vars.vars
$(CLI_DIR)/cpp/build/make/_vars.vars:
	$(call ShowVariables,TARGET BIN_PREFIX BIN_SUFFIX DYN_LIB_PREFIX DYN_LIB_SUFFIX STATIC_LIB_PREFIX STATIC_LIB_SUFFIX OUT_DIR CLI_CPP_LIB)

endif
# __CPP_VARS__
