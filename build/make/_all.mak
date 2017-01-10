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

ifndef __ROOT_ALL__
__ROOT_ALL = 1


# Default goal
ifndef .DEFAULT_GOAL
.DEFAULT_GOAL = default
endif
.PHONY: all.default
all.default: $(.DEFAULT_GOAL) ;

# Parameters
PROJECTS ?=
DEFAULT_HELP ?= Dispatch default rule on sub-projects
DEPS_HELP ?= Dispatch deps rule on sub-projects
CHECK_HELP ?= Dispatch check rule on sub-project
CLEAN_HELP ?= Dispatch clean rule on sub-projects
CLEAN_DIR ?=

# Includes
CLI_DIR ?= ../..
include $(CLI_DIR)/build/make/_utils.mak

# Rules
.PHONY: default
default:
	$(call MkDispatch,$(PROJECTS),)

.PHONY: deps
deps:
	$(call MkDispatch,$(PROJECTS),deps)

.PHONY: check
check:
	$(call MkDispatch,$(PROJECTS),check)

.PHONY: clean
clean:
	$(call MkDispatch,$(PROJECTS),clean)
ifneq ($(CLEAN_DIR),)
	$(call RemoveDir,$(CLEAN_DIR))
endif

# Debug and help
include $(CLI_DIR)/build/make/_help.mak

.PHONY: $(CLI_DIR)/build/make/_all.help
help: $(CLI_DIR)/build/make/_all.help
$(CLI_DIR)/build/make/_all.help:
# Do not display help on the default. Let the user Makefile do it.
	$(call PrintHelp, default,   $(DEFAULT_HELP))
	$(call PrintHelp, deps,      $(DEPS_HELP))
	$(call PrintHelp, check,     $(CHECK_HELP))
	$(call PrintHelp, clean,     $(CLEAN_HELP))

.PHONY: $(CLI_DIR)/build/make/_all.vars
vars: $(CLI_DIR)/build/make/_all.vars
$(CLI_DIR)/build/make/_all.vars:
	$(call ShowVariables,PROJECTS DEFAULT_HELP DEPS_HELP CHECK_HELP CLEAN_HELP CLEAN_DIR)

endif
# __ROOT_ALL__
