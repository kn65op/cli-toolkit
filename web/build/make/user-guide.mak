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
.DEFAULT_GOAL = html
.PHONY: user-guide.default
user-guide.default: $(.DEFAULT_GOAL) ;

# Directories
CLI_DIR ?= ../../..
include $(CLI_DIR)/build/make/_vars.mak
DB_DIR = $(CLI_DIR)/web/user-guide
MISC_DIR = $(DB_DIR)/misc
SAMPLES_DIR = $(DB_DIR)/samples

# Source
DB_USER_GUIDE = $(DB_DIR)/cli-user-guide.xml
HTML_USER_GUIDE = $(patsubst %.xml, %.html, $(DB_USER_GUIDE))
CLI_SAMPLES = $(wildcard $(CLI_DIR)/samples/user-guide/*.xml)
DB_SAMPLES = $(patsubst %.xml,$(SAMPLES_DIR)/%.db,$(notdir $(CLI_SAMPLES)))
DB_SAMPLES += $(SAMPLES_DIR)/circle-cpp.db $(SAMPLES_DIR)/circle-java.db
DB_SAMPLES += $(SAMPLES_DIR)/ui-text-cpp.db $(SAMPLES_DIR)/ui-text-java.db
MISC_FILES = $(MISC_DIR)/clisample.xml $(MISC_DIR)/clisample.html $(MISC_DIR)/empty.cpp $(MISC_DIR)/Empty.java $(MISC_DIR)/pwd_shell.h $(MISC_DIR)/PwdShellContext.java

# Transformation
XSL_STYLESHEET = $(DOCBOOK_XSL_HTML)

# Configuration
#CSS_FLAGS = --param html.stylesheet "'cli-user-guide.css'"
#JAVASCRIPT_FLAGS =
#TOC_FLAGS = --param generate.toc "'article toc,title,figure'"
TOC_FLAGS = --param generate.toc "''"
AUTOLABEL_FLAGS = --param section.autolabel 1 --param htmlhelp.autolabel 1

XSLT_OPTIONS = $(CSS_FLAGS) $(JAVASCRIPT_FLAGS) $(TOC_FLAGS) $(AUTOLABEL_FLAGS)

# Rules
.PHONY: html
html: dirs $(HTML_USER_GUIDE) ;

# => Main files
$(HTML_USER_GUIDE): $(wildcard $(DB_DIR)/*.xml $(DB_DIR)/*.css) $(DB_SAMPLES) $(MISC_FILES) user-guide.mak
ifneq ($(XSL_STYLESHEET),)
	xsltproc $(XSLT_OPTIONS) "$(XSL_STYLESHEET)" "$(DB_USER_GUIDE)" > $@.tmp \
	&& cp -f $@.tmp $@ \
	&& head -n 1 $@.tmp \
		| sed -e "s/<body/<body onload=\"onLoad();\" onscroll=\"onScroll();\" /" \
		| sed -e "s/<\/head>/\n<script type=\"text\/javascript\" src=\"..\/jquery-1.6.4.js\"><\/script><\/head>/" \
		| sed -e "s/<\/head>/\n<link type=\"text\/css\" rel=\"stylesheet\" href=\"..\/jquery-ui-1.8.16.custom\/css\/smoothness\/jquery-ui-1.8.16.custom.css\"><\/head>/" \
		| sed -e "s/<\/head>/\n<script type=\"text\/javascript\" src=\"..\/jquery-ui-1.8.16.custom\/js\/jquery-ui-1.8.16.custom.min.js\"><\/script><\/head>/" \
		| sed -e "s/<\/head>/\n<link type=\"text\/css\" rel=\"stylesheet\" href=\"..\/blackbirdjs-1.0\/blackbird-lite.css\"><\/head>/" \
		| sed -e "s/<\/head>/\n<script type=\"text\/javascript\" src=\"..\/blackbirdjs-1.0\/blackbird.js\"><\/script><\/head>/" \
		| sed -e "s/<\/head>/\n<script type=\"text\/javascript\" src=\"..\/cli-traces.js\"><\/script><\/head>/" \
		| sed -e "s/<\/head>/\n<script type=\"text\/javascript\" src=\"..\/xml-node.js\"><\/script><\/head>/" \
		| sed -e "s/<\/head>/\n<script type=\"text\/javascript\" src=\"..\/dynamic-menu.js\"><\/script><\/head>/" \
		| sed -e "s/<\/head>/\n<script type=\"text\/javascript\" src=\"..\/dynamic-dialog.js\"><\/script><\/head>/" \
		| sed -e "s/<\/head>/\n<link type=\"text\/css\" rel=\"stylesheet\" href=\"cli-user-guide.css\"><\/head>/" \
		| sed -e "s/<\/head>/\n<script type=\"text\/javascript\" src=\"cli-user-guide.js\"><\/script><\/head>/" \
		> $@ \
	&& tail -n +2 $@.tmp >> $@ \
	&& rm $@.tmp
else
$(warning Please set DOCBOOK_XSL_HTML to a valid UNIX path to have the docbook user-guide being generated)
endif

# => Sample files
DoSrc2Db = cat $(1) | sed -e "s/&/\&amp;/g" | sed -e "s/</\&lt;/g" | sed -e "s/>/\&gt;/g" > $@
$(SAMPLES_DIR)/circle-cpp.db: $(CLI_DIR)/samples/user-guide/circle.h
	$(call DoSrc2Db,$<,$@)

$(SAMPLES_DIR)/circle-java.db: $(CLI_DIR)/samples/user-guide/Circle.java
	$(call DoSrc2Db,$<,$@)

$(SAMPLES_DIR)/ui-text-cpp.db: $(CLI_DIR)/samples/user-guide/ui_sample_text.h
	$(call DoSrc2Db,$<,$@)

$(SAMPLES_DIR)/ui-text-java.db: $(CLI_DIR)/samples/user-guide/UISampleText.java
	$(call DoSrc2Db,$<,$@)

# => Misc files
$(SAMPLES_DIR)/%.db: $(CLI_DIR)/samples/user-guide/%.xml $(SAMPLES_DIR)/cli2db.xsl
	xsltproc $(SAMPLES_DIR)/cli2db.xsl $< > $@

$(MISC_DIR)/empty.cpp: $(CLI_DIR)/samples/user-guide/empty.xml $(CLI_DIR)/tools/cli2cpp.xsl
	xsltproc $(CLI_DIR)/tools/cli2cpp.xsl $< > $@

$(MISC_DIR)/Empty.java: $(CLI_DIR)/samples/user-guide/empty.xml $(CLI_DIR)/tools/cli2java.xsl
	xsltproc --stringparam STR_CliClassName "Empty" $(CLI_DIR)/tools/cli2java.xsl $< > $@

$(MISC_DIR)/clisample.xml: $(CLI_DIR)/samples/clisample/clisample.xml
	cp $< $@

$(MISC_DIR)/clisample.html: $(MISC_DIR)/clisample.xml $(CLI_DIR)/tools/cli2help.xsl
	xsltproc $(CLI_DIR)/tools/cli2help.xsl $< > $@

$(MISC_DIR)/pwd_shell.h: $(CLI_DIR)/samples/user-guide/pwd_shell.h
	cat $< | sed -e "s/\&/\&amp;/g" | sed -e "s/</\&lt;/g" > $@

$(MISC_DIR)/PwdShellContext.java: $(CLI_DIR)/samples/user-guide/PwdShellContext.java
	cat $< | sed -e "s/\&/\&amp;/g" | sed -e "s/</\&lt;/g" > $@

.PHONY: dirs
dirs: ;

.PHONY: deps
deps: ;

.PHONY: clean
clean:
ifneq ($(XSL_STYLESHEET),)
	rm -f $(HTML_USER_GUIDE) $(DB_SAMPLES) $(MISC_FILES)
endif

# Debug and help
include $(CLI_DIR)/build/make/_help.mak

.PHONY: $(CLI_DIR)/web/user-guide/user-guide.help
help: $(CLI_DIR)/web/user-guide/user-guide.help
$(CLI_DIR)/web/user-guide/user-guide.help:
	$(call PrintHelp, html,	 Generate user-guide)
	$(call PrintHelp, clean, Clean up output and intermediate files)

.PHONY: $(CLI_DIR)/web/user-guide/user-guide.vars
vars: $(CLI_DIR)/web/user-guide/user-guide.vars
$(CLI_DIR)/web/user-guide/user-guide.vars:
	$(call ShowVariables, \
		DB_DIR MISC_DIR SAMPLES_DIR \
		DB_USER_GUIDE HTML_USER_GUIDE CLI_SAMPLES DB_SAMPLES MISC_FILES \
		XSL_STYLESHEET \
		CSS_FLAGS JAVASCRIPT_FLAGS TOC_FLAGS AUTOLABEL_FLAGS XSLT_OPTIONS)
