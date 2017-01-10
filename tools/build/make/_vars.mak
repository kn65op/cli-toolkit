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

ifndef __TOOLS_VARS__
__TOOLS_VARS__ = 1

# Includes
CLI_DIR ?= ../../..
include $(CLI_DIR)/build/make/_vars.mak

# Variables
PROJECT = $(patsubst %.mak,%,$(THIS_MAKEFILE))
THIS_MAKEFILE = $(firstword $(MAKEFILE_LIST))
OUT_DIR = __$(PROJECT)
CLI_XML_ERROR_FILES = $(patsubst %.error.log,%.xml,$(shell find $(CLI_DIR)/samples -name "*.error.log" -print))
CLI_XML_BIG_FILE = $(CLI_DIR)/samples/tests/big.xml
CLI_XML_REGULAR_FILES = $(filter-out $(CLI_XML_ERROR_FILES) $(CLI_XML_BIG_FILE),$(shell find $(CLI_DIR)/samples -name "*.xml" -print))

# Debug and help
include $(CLI_DIR)/build/make/_help.mak

.PHONY: $(CLI_DIR)/tools/build/make/_vars.vars
vars: $(CLI_DIR)/tools/build/make/_vars.vars
$(CLI_DIR)/tools/build/make/_vars.vars:
	$(call ShowVariables,PROJECT THIS_MAKEFILE OUT_DIR CLI_XML_REGULAR_FILES CLI_XML_ERROR_FILES CLI_XML_BIG_FILE)

endif
# __TOOLS_VARS__
