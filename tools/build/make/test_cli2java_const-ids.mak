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


# This aim of this test is to check that cli2cpp transformation is a determinist operation.

# Default goal
.DEFAULT_GOAL = check
.PHONY: cli2java.default
cli2java.default: $(.DEFAULT_GOAL) ;

# Includes
CLI_DIR := ../../..
include _vars.mak
include $(CLI_DIR)/build/make/_utils.mak

# Variables
CLI_XML_RES = $(CLI_DIR)/samples/tests/const-ids.xml
CLI_JAVA = $(patsubst $(CLI_DIR)/samples/%.xml,$(OUT_DIR)/%.java,$(CLI_XML_RES))
AWK_SCRIPT = test_cli2x_const-ids.awk
AWK_OUT1 = $(patsubst $(CLI_DIR)/samples/%.xml,$(OUT_DIR)/%.out1.log,$(CLI_XML_RES))
AWK_OUT2 = $(patsubst $(CLI_DIR)/samples/%.xml,$(OUT_DIR)/%.out2.log,$(CLI_XML_RES))
AWK_LOG = $(patsubst $(CLI_DIR)/samples/%.xml,$(OUT_DIR)/%.awk.log,$(CLI_XML_RES))

# Rules
.PHONY: check
check: $(CLI_JAVA).lite $(AWK_SCRIPT)
	rm -f $(AWK_OUT1) $(AWK_OUT2)
	touch $(AWK_OUT1) $(AWK_OUT2)
	gawk -f $(AWK_SCRIPT) -v outfile1=$(AWK_OUT1) -v outfile2=$(AWK_OUT2) $(CLI_JAVA).lite > $(AWK_LOG)
	diff $(AWK_OUT1) $(AWK_OUT2)

$(CLI_JAVA).lite: $(CLI_JAVA)
	cat $< \
	| grep "private cli\.Keyword m_clikeyword_" \
	| sed -e "s/^.*private cli\.Keyword m_clikeyword_//" \
	| sed -e "s/;//" \
	> $@

$(CLI_JAVA): $(CLI_XML_RES) $(CLI_DIR)/tools/cli2java.xsl
	$(call CheckDir,$(dir $@))
	xsltproc $(CLI_DIR)/tools/cli2java.xsl $< > $@.tmp
	mv $@.tmp $@

.PHONY: clean
clean:
	rm -f $(CLI_JAVA) $(CLI_JAVA).tmp $(CLI_JAVA).lite $(AWK_OUT1) $(AWK_OUT2) $(AWK_LOG)
	$(call RemoveDir,$(shell find $(OUT_DIR) -type d | sort -r))

# Debug and help
include $(CLI_DIR)/build/make/_help.mak

.PHONY: $(CLI_DIR)/java/build/make/cli2java.help
help: $(CLI_DIR)/java/build/make/cli2java.help
$(CLI_DIR)/java/build/make/cli2java.help:
	$(call PrintHelp, check, Check cli2java transformation)
	$(call PrintHelp, clean, Clean all intermediate files)

.PHONY: $(CLI_DIR)/java/build/make/cli2java.vars
vars: $(CLI_DIR)/java/build/make/cli2java.vars
$(CLI_DIR)/java/build/make/cli2java.vars:
	$(call ShowVariables,CLI_XML_RES CLI_JAVA AWK_SCRIPT AWK_LOG)
