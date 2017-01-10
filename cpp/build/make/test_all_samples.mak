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
.PHONY: tests.default
tests.default: $(.DEFAULT_GOAL) ;


# Variables
CLI_DIR := ../../..
include _vars.mak
include $(CLI_DIR)/build/make/_utils.mak
XML_FILES = $(patsubst %.check,%.xml,$(CHECK_FILES))
TEST_FILES = $(patsubst %.xml,%.test,$(XML_FILES))
CHECK_FILES = $(shell find $(CLI_DIR)/samples -name "*.check")


# Rules
.PHONY: check
check: MkCallDefault = $(MAKE) -s --no-print-directory -f test_sample.mak CLI_XML_RES=$(1) check.xml
check: dirs
	$(call Map,MkCallDefault,$(XML_FILES))

.PHONY: dirs
dirs:
	$(call CheckDir,$(OUT_DIR)/__test_sample)

.PHONY: clean
clean: MkCallClean = $(MAKE) -s --no-print-directory -f test_sample.mak CLI_XML_RES=$(1) CLEAN_DIR= clean
clean:
	$(call Map,MkCallClean,$(XML_FILES))
	$(call RemoveDir,$(OUT_DIR)/__test_sample)

.PHONY: deps
deps: ;

# Debug and help
include $(CLI_DIR)/build/make/_help.mak

.PHONY: $(CLI_DIR)/cpp/build/make/tests.help
help: $(CLI_DIR)/cpp/build/make/tests.help
$(CLI_DIR)/cpp/build/make/tests.help:
	$(call PrintHelp, check, Launch tests on each sample file)
	$(call PrintHelp, dirs,  Create intermediate directories)
	$(call PrintHelp, clean, Clean intermediate files)

.PHONY: $(CLI_DIR)/cpp/build/make/tests.vars
vars: $(CLI_DIR)/cpp/build/make/tests.vars
$(CLI_DIR)/cpp/build/make/tests.vars:
	$(call ShowVariables,XML_FILES TEST_FILES CHECK_FILES)

