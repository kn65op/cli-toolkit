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
.PHONY: cli2java.default
cli2java.default: $(.DEFAULT_GOAL) ;

# Includes
CLI_DIR := ../../..
include _vars.mak
include _python.mak
include $(CLI_DIR)/build/make/_utils.mak

# Variables
CLI_XML_FILES = $(CLI_XML_REGULAR_FILES)
CLI_XML_RES = $(CLI_DIR)/samples/clisample/clisample.xml
CLI_JAVA_XSL = $(patsubst $(CLI_DIR)/samples/%.xml,$(OUT_DIR)/%.xsl.java,$(CLI_XML_RES))
CLI_JAVA_PY = $(patsubst $(CLI_DIR)/samples/%.xml,$(OUT_DIR)/%.py.java,$(CLI_XML_RES))
TIME = time

# Rules
.PHONY: check
check: MkCallCheck = $(MAKE) -s --no-print-directory -f $(THIS_MAKEFILE) CLI_XML_FILES=_ CLI_XML_RES=$(1) TIME= check.xml
check:
	$(call Map,MkCallCheck,$(CLI_XML_FILES))

.PHONY: check.xml
check.xml: $(CLI_JAVA_XSL).lite $(CLI_JAVA_PY).lite
	diff $(CLI_JAVA_XSL).lite $(CLI_JAVA_PY).lite | head
	@diff $(CLI_JAVA_XSL).lite  $(CLI_JAVA_PY).lite  > /dev/null

$(CLI_JAVA_XSL).lite: $(CLI_JAVA_XSL)
	cat $< | sed -e "s/cli2java\.xsl/cli2java/g" > $@
	dos2unix $@ 2> /dev/null

$(CLI_JAVA_PY).lite: $(CLI_JAVA_PY)
	cat $< | grep -v "^ \* @date" | sed -e "s/cli2java\.py/cli2java/g" > $@
	dos2unix $@ 2> /dev/null

# Force /bin/bash in order to prevent /bin/dash from failing with command grouping below.
$(CLI_JAVA_XSL): SHELL := /bin/bash
$(CLI_JAVA_XSL): $(CLI_XML_RES) $(CLI_DIR)/tools/cli2java.xsl 
	$(call CheckDir,$(dir $@))
	# XSLT transformation of $(CLI_XML_RES)...
	$(TIME) (xsltproc $(CLI_DIR)/tools/cli2java.xsl $< > $@.tmp)
	mv $@.tmp $@

$(CLI_JAVA_PY): SHELL := /bin/bash
$(CLI_JAVA_PY): $(CLI_XML_RES) $(PYLINT_RESULT) $(wildcard $(CLI_DIR)/tools/*.py)
	$(call CheckDir,$(dir $@))
	# python transformation of $(CLI_XML_RES)...
	$(TIME) ($(PYTHON) $(CLI_DIR)/tools/cli2java.py $< > $@.tmp)
	mv $@.tmp $@

.PHONY: deps
deps: ;

.PHONY: clean
clean: MkCallClean = $(MAKE) -s --no-print-directory -f $(THIS_MAKEFILE) CLI_XML_FILES=_ CLI_XML_RES=$(1) clean
clean:
ifneq ($(CLI_XML_FILES),_)
	$(call Map,MkCallClean,$(CLI_XML_FILES) $(CLI_DIR)/samples/tests/big.xml)
	$(call RemoveDir,$(shell find $(OUT_DIR) -type d | sort -r))
else
	rm -f $(CLI_JAVA_XSL) $(CLI_JAVA_XSL).tmp $(CLI_JAVA_XSL).lite
	rm -f $(CLI_JAVA_PY) $(CLI_JAVA_PY).tmp $(CLI_JAVA_PY).lite
	rm -f $(PYLINT_RESULT)
endif

# Debug and help
include $(CLI_DIR)/build/make/_help.mak

.PHONY: $(CLI_DIR)/build/make/cli2java.help
help: $(CLI_DIR)/build/make/cli2java.help
$(CLI_DIR)/build/make/cli2java.help:
	$(call PrintHelp, check,     Dispatch 'check' rule over all XML sample and test files)
	$(call PrintHelp, check.xml, Compare XSL and python transformation results for the given XML resource file (CLI_XML_RES))
	$(call PrintHelp, clean,     Clean all intermediate files)

.PHONY: $(CLI_DIR)/build/make/cli2java.vars
vars: $(CLI_DIR)/build/make/cli2java.vars
$(CLI_DIR)/build/make/cli2java.vars:
	$(call ShowVariables,CLI_XML_FILES CLI_XML_RES CLI_JAVA_XSL CLI_JAVA_PY PYLINT_RESULT TIME)
