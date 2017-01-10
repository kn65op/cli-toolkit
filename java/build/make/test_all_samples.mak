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
.PHONY: javatests.default
javatests.default: $(.DEFAULT_GOAL) ;


# Variables
CLI_DIR := ../../..
include _vars.mak

JAVA_SAMPLE_FILES = $(shell find $(CLI_DIR)/samples -name "*.java")

XML_FILES = $(patsubst %.check,%.xml,$(CHECK_FILES))
TEST_FILES = $(patsubst %.xml,%.test,$(XML_FILES))
CHECK_FILES = $(shell find $(CLI_DIR)/samples -name "*.check")


# Rules
DoTestDefault = $(MAKE) -s --no-print-directory -f test_sample.mak CLI_XML_RES=$(1) check.xml
.PHONY: check
check:
	@$(MAKE) --no-print-directory -f test_all_samples.mak java
	$(call Map,DoTestDefault,$(XML_FILES))

SRC_DIR = $(JAVA_SRC_DIR)
JAVA_FILES = $(patsubst %.java,$(SRC_DIR)/cli/test/%.java,$(notdir $(JAVA_SAMPLE_FILES)))
PROJ_CLEAN = $(JAVA_FILES)
include _build.mak

.PHONY: java
java: $(JAVA_OBJS) ;

$(JAVA_SRC_DIR)/cli/test/%.java: JAVA_SRC_FILE = $@
$(JAVA_SRC_DIR)/cli/test/%.java: JAVA_SAMPLE_FILE = $(CLI_DIR)/samples/user-guide/$(notdir $@)
$(JAVA_SRC_DIR)/cli/test/%.java: $(CLI_DIR)/samples/user-guide/%.java
	rm -f $(JAVA_SRC_FILE)
	echo "// Copy of 'samples/user-guide/$(notdir $@)'" >> $(JAVA_SRC_FILE)
	echo "package cli.test;" >> $(JAVA_SRC_FILE)
	cat $(JAVA_SAMPLE_FILE) >> $(JAVA_SRC_FILE)

DoTestClean = $(MAKE) -s --no-print-directory -f test_sample.mak CLI_XML_RES=$(1) clean
.PHONY: javatests.clean
javatests.clean:
	$(call Map,DoTestClean,$(XML_FILES))
	rm -f $(OUT_DIR)/cli/test/*.class
	$(call RemoveDir,$(OUT_DIR)/cli/test)

.PHONY: deps
deps: ;

# Debug and help
include $(CLI_DIR)/build/make/_help.mak

.PHONY: $(CLI_DIR)/java/build/make/tests.help
help: $(CLI_DIR)/java/build/make/tests.help
$(CLI_DIR)/java/build/make/tests.help:
	$(call PrintHelp, check, Launch tests on each sample file)
	$(call PrintHelp, clean, Clean intermediate files)

.PHONY: $(CLI_DIR)/java/build/make/tests.vars
vars: $(CLI_DIR)/java/build/make/tests.vars
$(CLI_DIR)/java/build/make/tests.vars:
	$(call ShowVariables,JAVA_SAMPLE_FILES XML_FILES TEST_FILES CHECK_FILES)

# Dependencies
clean: javatests.clean
