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
.PHONY: cli2help.default
cli2help.default: $(.DEFAULT_GOAL) ;

# Includes
CLI_DIR := ../../..
include _vars.mak
include _python.mak
include $(CLI_DIR)/build/make/_utils.mak

# Variables
CLI_XML_FILES = $(CLI_XML_REGULAR_FILES)
CLI_XML_RES = $(CLI_DIR)/samples/clisample/clisample.xml
CLI_HTML_EN_XSL = $(patsubst $(CLI_DIR)/samples/%.xml,$(OUT_DIR)/%.en.xsl.html,$(CLI_XML_RES))
CLI_HTML_FR_XSL = $(patsubst $(CLI_DIR)/samples/%.xml,$(OUT_DIR)/%.fr.xsl.html,$(CLI_XML_RES))
CLI_HTML_EN_PY = $(patsubst $(CLI_DIR)/samples/%.xml,$(OUT_DIR)/%.en.py.html,$(CLI_XML_RES))
CLI_HTML_FR_PY = $(patsubst $(CLI_DIR)/samples/%.xml,$(OUT_DIR)/%.fr.py.html,$(CLI_XML_RES))
OUT_DIFF_EN = $(patsubst $(CLI_DIR)/samples/%.xml, $(OUT_DIR)/%.en.diff,$(CLI_XML_RES))
OUT_DIFF_FR = $(patsubst $(CLI_DIR)/samples/%.xml, $(OUT_DIR)/%.fr.diff,$(CLI_XML_RES))
EXPECTED_DIFF = $(patsubst %.xml,%.cli2help.diff,$(CLI_XML_RES))
TIME = time

# Rules
.PHONY: check
check: MkCallCheck = $(MAKE) -s --no-print-directory -f $(THIS_MAKEFILE) CLI_XML_FILES=_ CLI_XML_RES=$(1) TIME= check.xml
check:
	$(call Map,MkCallCheck,$(CLI_XML_FILES))

.PHONY: check.xml check.en check.fr
check.xml: check.en check.fr ;

check.en: $(CLI_HTML_EN_XSL).lite $(CLI_HTML_EN_PY).lite
ifeq ($(shell ls $(EXPECTED_DIFF) 2> /dev/null),$(EXPECTED_DIFF))
	# Diff expected...
	diff $(CLI_HTML_EN_XSL).lite $(CLI_HTML_EN_PY).lite > $(OUT_DIFF_EN) || true
	diff $(OUT_DIFF_EN) $(EXPECTED_DIFF) | head
	@diff $(OUT_DIFF_EN) $(EXPECTED_DIFF) > /dev/null
else
	# No diff expected...
	diff $(CLI_HTML_EN_XSL).lite $(CLI_HTML_EN_PY).lite | head
	@diff $(CLI_HTML_EN_XSL).lite  $(CLI_HTML_EN_PY).lite > /dev/null
endif

check.fr: $(CLI_HTML_FR_XSL).lite $(CLI_HTML_FR_PY).lite
ifeq ($(shell ls $(EXPECTED_DIFF) 2> /dev/null),$(EXPECTED_DIFF))
	# Diff expected...
	diff $(CLI_HTML_FR_XSL).lite $(CLI_HTML_FR_PY).lite > $(OUT_DIFF_FR) || true
	sed -e "s/Description :/Description:/" -i $(OUT_DIFF_FR)
	sed -e "s/Aucune aide disponible/No help available/" -i $(OUT_DIFF_FR)
	sed -e "s/Valeur enti\xe8re/int value/" -i $(OUT_DIFF_FR)
	diff $(OUT_DIFF_FR) $(EXPECTED_DIFF)
else
	# No diff expected...
	diff $(CLI_HTML_FR_XSL).lite $(CLI_HTML_FR_PY).lite | head
	@diff $(CLI_HTML_FR_XSL).lite  $(CLI_HTML_FR_PY).lite  > /dev/null
endif

%.xsl.html.lite: %.xsl.html
# uniq fails when dealing when latin-1 characters, export LC_ALL fixes that behaviour
	export LC_ALL='C' && uniq $< $@.tmp
	sed -e "s/iso-8859-1/ISO-8859-1/" -i $@.tmp
	sed -e "s/cli2help\.xsl/cli2help/" -i $@.tmp
	sed -e "s/ xmlns:cli=\"http:\/\/alexis\.royer\.free\.fr\/CLI\"//" -i $@.tmp
	mv $@.tmp $@

%.py.html.lite: %.py.html
# uniq fails when dealing when latin-1 characters, export LC_ALL fixes that behaviour
	export LC_ALL='C' && uniq $< $@.tmp
	sed -e "s/cli2help\.py/cli2help/" -i $@.tmp
	mv $@.tmp $@

# Force /bin/bash in order to prevent /bin/dash from failing with command grouping below.
$(OUT_DIR)/%.en.xsl.html: SHELL := /bin/bash
$(OUT_DIR)/%.en.xsl.html: $(CLI_DIR)/samples/%.xml $(CLI_DIR)/tools/cli2help.xsl
	$(call CheckDir,$(dir $@))
	# XSLT transformation of $(CLI_XML_RES)...
	$(TIME) (xsltproc --stringparam STR_Lang 'en' $(CLI_DIR)/tools/cli2help.xsl $< > $@.tmp)
	mv $@.tmp $@

$(OUT_DIR)/%.fr.xsl.html: $(CLI_DIR)/samples/%.xml $(CLI_DIR)/tools/cli2help.xsl
	$(call CheckDir,$(dir $@))
	xsltproc --stringparam STR_Lang 'fr' $(CLI_DIR)/tools/cli2help.xsl $< > $@.tmp
	mv $@.tmp $@

$(OUT_DIR)/%.en.py.html: SHELL := /bin/bash
$(OUT_DIR)/%.en.py.html: $(CLI_DIR)/samples/%.xml $(PYLINT_RESULT) $(wildcard $(CLI_DIR)/tools/*.py)
	$(call CheckDir,$(dir $@))
	# python transformation of $(CLI_XML_RES)...
	$(TIME) ($(PYTHON) $(CLI_DIR)/tools/cli2help.py --lang='en' $< > $@.tmp)
	mv $@.tmp $@

$(OUT_DIR)/%.fr.py.html: $(CLI_DIR)/samples/%.xml $(PYLINT_RESULT) $(wildcard $(CLI_DIR)/tools/*.py)
	$(call CheckDir,$(dir $@))
	$(PYTHON) $(CLI_DIR)/tools/cli2help.py --lang='fr' $< > $@.tmp
	cat $@.tmp | sed -e "s/Adresse réseau/host value/g" > $@

.PHONY: deps
deps: ;

.PHONY: clean
clean: MkCallClean = $(MAKE) -s --no-print-directory -f $(THIS_MAKEFILE) CLI_XML_FILES=_ CLI_XML_RES=$(1) clean
clean:
ifneq ($(CLI_XML_FILES),_)
	$(call Map,MkCallClean,$(CLI_XML_FILES) $(CLI_DIR)/samples/tests/big.xml)
	$(call RemoveDir,$(shell find $(OUT_DIR) -type d | sort -r))
else
	rm -f $(CLI_HTML_EN_XSL) $(CLI_HTML_EN_XSL).tmp $(CLI_HTML_EN_XSL).lite $(CLI_HTML_EN_XSL).lite.tmp
	rm -f $(CLI_HTML_FR_XSL) $(CLI_HTML_FR_XSL).tmp $(CLI_HTML_FR_XSL).lite $(CLI_HTML_FR_XSL).lite.tmp
	rm -f $(CLI_HTML_EN_PY) $(CLI_HTML_EN_PY).tmp $(CLI_HTML_EN_PY).lite $(CLI_HTML_EN_PY).lite.tmp
	rm -f $(CLI_HTML_FR_PY) $(CLI_HTML_FR_PY).tmp $(CLI_HTML_FR_PY).lite $(CLI_HTML_FR_PY).lite.tmp
	rm -f $(OUT_DIFF_EN) $(OUT_DIFF_FR)
	rm -f $(PYLINT_RESULT)
endif

# Debug and help
include $(CLI_DIR)/build/make/_help.mak

.PHONY: $(CLI_DIR)/build/make/cli2help.help
help: $(CLI_DIR)/build/make/cli2help.help
$(CLI_DIR)/build/make/cli2help.help:
	$(call PrintHelp, check,     Dispatch 'check' rule over all XML sample and test files)
	$(call PrintHelp, check.xml, Compare XSL and python transformation results for the given XML resource file (CLI_XML_RES))
	$(call PrintHelp, clean,     Clean all intermediate files)

.PHONY: $(CLI_DIR)/build/make/cli2help.vars
vars: $(CLI_DIR)/build/make/cli2help.vars
$(CLI_DIR)/build/make/cli2help.vars:
	$(call ShowVariables,CLI_XML_FILES CLI_XML_RES \
	                     CLI_HTML_EN_XSL CLI_HTML_FR_XSL CLI_HTML_EN_PY CLI_HTML_FR_PY \
	                     OUT_DIFF_EN OUT_DIFF_FR EXPECTED_DIFF \
	                     TIME)
