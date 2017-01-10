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
.DEFAULT_GOAL = javadoc
.PHONY: javadoc.default
javadoc.default: $(.DEFAULT_GOAL) ;

ifeq ($(JAVA_HOME),)
$(warning Please set JAVA_HOME to your SDK installation directory and ensure \$JAVA_DOC/bin is in your PATH environment variable)
.PHONY: javadoc deps clean
javadoc: ;
deps: ;
clean: ;
else

# Variables
CLI_DIR := ../../..
include $(CLI_DIR)/build/make/_vars.mak
JAVA_FILES = $(wildcard $(CLI_DIR)/java/src/cli/*.java) $(wildcard $(CLI_DIR)/java/src/cli/ui/*.java)
OUT_DIR = $(CLI_DIR)/web/javadoc/html

# Rules
.PHONY: javadoc
javadoc:
	$(call CheckDir,$(OUT_DIR))
	javadoc -quiet -d $(OUT_DIR) $(JAVA_FILES)

.PHONY: deps
deps: ;

.PHONY: clean
clean:
	rm -rf $(OUT_DIR)

# Debug and help
include $(CLI_DIR)/build/make/_help.mak

.PHONY: $(CLI_DIR)/web/build/make/javadoc.help
help: $(CLI_DIR)/web/build/make/javadoc.help
$(CLI_DIR)/web/build/make/javadoc.help:
	$(call PrintHelp, javadoc,   Generate javadoc documentation)
	$(call PrintHelp, clean,     Clean up output documentation)

.PHONY: $(CLI_DIR)/web/build/make/javadoc.vars
vars: $(CLI_DIR)/web/build/make/javadoc.vars
$(CLI_DIR)/web/build/make/javadoc.vars:
	$(call ShowVariables,JAVA_FILES OUT_DIR)

endif
