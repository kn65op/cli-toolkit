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


# The aim of this test is to check that python tranformation runs faster than XSL transformations.

# Default goal
.DEFAULT_GOAL = check
.PHONY: cli2cpp.default
cli2cpp.default: $(.DEFAULT_GOAL) ;

# Includes
CLI_DIR := ../../..

# Rules
.PHONY: check
check:
	$(MAKE) -f test_cli2cpp_xsl-py-diff.mak CLI_XML_FILES=_ CLI_XML_RES=$(CLI_DIR)/samples/tests/big.xml check.xml

.PHONY: deps
deps: ;

.PHONY: clean
clean:
	$(MAKE) -f test_cli2cpp_xsl-py-diff.mak CLI_XML_FILES=_ CLI_XML_RES=$(CLI_DIR)/samples/tests/big.xml clean

# Debug and help
include $(CLI_DIR)/build/make/_help.mak

.PHONY: $(CLI_DIR)/build/make/cli2cpp.help
help: $(CLI_DIR)/build/make/cli2cpp.help
$(CLI_DIR)/build/make/cli2cpp.help:
	$(call PrintHelp, check, Compare XSL and python transformation time executions)
	$(call PrintHelp, clean, Clean all intermediate files)
