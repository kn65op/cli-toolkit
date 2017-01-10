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


ifndef __CPP_CHECK__
__CPP_CHECK__ = 1


# Default goal
ifndef .DEFAULT_GOAL
.DEFAULT_GOAL = check
endif
.PHONY: cppcheck.default
cppcheck.default: $(.DEFAULT_GOAL) ;

# Variables
CLI_TEST ?= $(patsubst %.xml,%.test,$(CLI_XML_RES))
CLI_LOG ?= $(patsubst %.xml,$(INT_DIR)/%.log,$(notdir $(CLI_XML_RES)))
CLI_CHECK ?= $(patsubst %.xml,%.check,$(CLI_XML_RES))

# Includes
PROJ_CLEAN += $(CLI_LOG)
include _mkres.mak

# Rules
.PHONY: log
log: build_depends $(CLI_LOG) ;

$(CLI_LOG): $(CLI_BINARY) $(CLI_TEST) $(CLI_DIR)/samples/clean_outlog.sh
	dos2unix $(CLI_TEST) 2> /dev/null
	$(CLI_BINARY) --input $(CLI_TEST) --output $(CLI_LOG) 1> /dev/null 2> /dev/null
	$(call CheckSh,$(CLI_DIR)/samples/clean_outlog.sh)
	$(CLI_DIR)/samples/clean_outlog.sh $(CLI_LOG)
	dos2unix $(CLI_LOG) 2> /dev/null

.PHONY: check check.xml
check check.xml: build_depends $(CLI_LOG) $(CLI_CHECK)
	dos2unix $(CLI_CHECK) 2> /dev/null
	diff $(CLI_LOG) $(CLI_CHECK)

# Debug and help
include $(CLI_DIR)/build/make/_help.mak

.PHONY: $(CLI_DIR)/cpp/build/make/_check.help
help: $(CLI_DIR)/cpp/build/make/_check.help
$(CLI_DIR)/cpp/build/make/_check.help:
	$(call PrintHelp, check.xml, Check test output for the given XML resource file (CLI_XML_RES))
	$(call PrintHelp, check,     Shortcut for check.xml rule)
	$(call PrintHelp, log,       Generate $(notdir $(CLI_LOG)))

.PHONY: $(CLI_DIR)/cpp/build/make/_check.vars
vars: $(CLI_DIR)/cpp/build/make/_check.vars
$(CLI_DIR)/cpp/build/make/_check.vars:
	$(call ShowVariables,CLI_TEST CLI_LOG CLI_CHECK)

endif
# __CPP_CHECK__
