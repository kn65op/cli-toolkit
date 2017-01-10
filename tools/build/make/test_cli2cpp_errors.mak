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


# The aim of this test is to check that python and XSL transformations behave the same.

# Default goal
.DEFAULT_GOAL = check
.PHONY: cli2cpp.default
cli2cpp.default: $(.DEFAULT_GOAL) ;

# Includes
CLI_DIR := ../../..
include _vars.mak
include _python.mak
include $(CLI_DIR)/build/make/_utils.mak

# Variables
CLI_XML_FILES = $(CLI_XML_ERROR_FILES)
CLI_XML_RES = $(firstword $(CLI_XML_FILES))
CLI_ERROR_LOG = $(patsubst %.xml,%.error.log,$(CLI_XML_RES))
CLI_CPP_XSL = $(patsubst $(CLI_DIR)/samples/%.xml,$(OUT_DIR)/%.xsl.log,$(CLI_XML_RES))
CLI_CPP_PY = $(patsubst $(CLI_DIR)/samples/%.xml,$(OUT_DIR)/%.py.log,$(CLI_XML_RES))

# Rules
.PHONY: check
check: MkCallCheck = $(MAKE) -f $(THIS_MAKEFILE) CLI_XML_FILES=_ CLI_XML_RES=$(1) check.xml
check:
	$(call Map,MkCallCheck,$(CLI_XML_FILES))

.PHONY: check.xml
check.xml: $(CLI_CPP_XSL) $(CLI_CPP_PY) $(CLI_ERROR_LOG)
	diff $(CLI_CPP_XSL) $(CLI_ERROR_LOG)
	diff $(CLI_CPP_PY) $(CLI_ERROR_LOG)

$(CLI_CPP_XSL): $(CLI_XML_RES) $(CLI_DIR)/tools/cli2cpp.xsl
	$(call CheckDir,$(dir $@))
	rm -f $@.error
	(xsltproc $(CLI_DIR)/tools/cli2cpp.xsl $< > /dev/null 2> $@.tmp) || touch $@.error
	test -f $@.error
	mv $@.tmp $@ 2> /dev/null

$(CLI_CPP_PY): $(CLI_XML_RES) $(PYLINT_RESULT) $(wildcard $(CLI_DIR)/tools/*.py)
	$(call CheckDir,$(dir $@))
	rm -f $@.error
	($(PYTHON) $(CLI_DIR)/tools/cli2cpp.py $< > /dev/null 2> $@.tmp) || touch $@.error
	test -f $@.error
	sed -s "s/^Error\!.*\]: /Error: /" -i $@.tmp
	mv $@.tmp $@ 2> /dev/null

.PHONY: deps
deps: ;

.PHONY: clean
clean: MkCallClean = $(MAKE) -f $(THIS_MAKEFILE) CLI_XML_FILES=_ CLI_XML_RES=$(1) clean
clean:
ifneq ($(CLI_XML_FILES),_)
	$(call Map,MkCallClean,$(CLI_XML_FILES) $(CLI_DIR)/samples/tests/big.xml)
	$(call RemoveDir,$(shell find $(OUT_DIR) -type d | sort -r))
else
	rm -f $(PYLINT_RESULT)
	rm -f $(CLI_CPP_XSL) $(CLI_CPP_XSL).tmp $(CLI_CPP_XSL).error
	rm -f $(CLI_CPP_PY) $(CLI_CPP_PY).tmp $(CLI_CPP_PY).error
endif

# Debug and help
include $(CLI_DIR)/build/make/_help.mak

.PHONY: $(CLI_DIR)/build/make/cli2cpp.help
help: $(CLI_DIR)/build/make/cli2cpp.help
$(CLI_DIR)/build/make/cli2cpp.help:
	$(call PrintHelp, check,     Dispatch 'check' rule over all XML sample and test files)
	$(call PrintHelp, check.xml, Compare XSL and python transformation results for the given XML resource file (CLI_XML_RES))
	$(call PrintHelp, clean,     Clean all intermediate files)

.PHONY: $(CLI_DIR)/build/make/cli2cpp.vars
vars: $(CLI_DIR)/build/make/cli2cpp.vars
$(CLI_DIR)/build/make/cli2cpp.vars:
	$(call ShowVariables,CLI_XML_FILES CLI_XML_RES CLI_ERROR_LOG CLI_CPP_XSL CLI_CPP_PY PYLINT_RESULT)
