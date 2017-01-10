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
.DEFAULT_GOAL = doxygen
.PHONY: doxygen.default
doxygen.default: $(.DEFAULT_GOAL) ;

ifeq ($(shell which doxygen 2> /dev/null),)
$(warning Please install doxygen and make your PATH environment variable is up to date)
.PHONY: doxygen.default deps clean
doxygen.default: ;
deps: ;
clean: ;
else

# Variables
CLI_DIR := ../../..
include $(CLI_DIR)/build/make/_vars.mak
DOXY_FILE = $(CLI_DIR)/web/doxygen/cli.doxygen
OUT_DIR = $(dir $(DOXY_FILE))/html

# Rules
.PHONY: doxygen
doxygen: $(DOXY_FILE)
	cd $(dir $(DOXY_FILE)) && doxygen $(notdir $(DOXY_FILE))
ifeq ($(shell which dot 2> /dev/null),)
	@echo "Warning! In order to enhance the documentation with class diagrams, install dot and make your PATH environment variable is up to date"
endif
ifeq ($(shell which mscgen 2> /dev/null),)
	@echo "Warning! In order to enhance the documentation with flow chart diagrams, install mscgen and make your PATH environment variable is up to date"
endif

.PHONY: deps
deps: ;

.PHONY: clean
clean:
	rm -rf $(OUT_DIR)

# Debug and help
include $(CLI_DIR)/build/make/_help.mak

.PHONY: $(CLI_DIR)/web/build/make/doxygen.help
help: $(CLI_DIR)/web/build/make/doxygen.help
$(CLI_DIR)/web/build/make/doxygen.help:
	$(call PrintHelp, doxygen,   Generate doxygen documentation)
	$(call PrintHelp, clean,     Clean up output documentation)

.PHONY: $(CLI_DIR)/web/build/make/doxygen.vars
vars: $(CLI_DIR)/web/build/make/doxygen.vars
$(CLI_DIR)/web/build/make/doxygen.vars:
	$(call ShowVariables,DOXY_FILE OUT_DIR)

endif
