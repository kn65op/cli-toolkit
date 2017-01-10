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
.DEFAULT_GOAL = default
.PHONY: Makefile.default
Makefile.default: $(.DEFAULT_GOAL) ;

# Includes
CLI_DIR := ../../..
# Tests
check: PROJECTS =
check clean: PROJECTS += test_schema.mak
check clean: PROJECTS += test_cli2cpp_const-ids.mak test_cli2cpp_xsl-py-diff.mak test_cli2cpp_errors.mak test_cli2cpp_perf.mak
check clean: PROJECTS += test_cli2java_const-ids.mak test_cli2java_xsl-py-diff.mak test_cli2java_errors.mak test_cli2java_perf.mak
check clean: PROJECTS += test_cli2help_xsl-py-diff.mak test_cli2help_errors.mak test_cli2help_perf.mak
clean: PROJECTS += compatibility.mak
include $(CLI_DIR)/build/make/_all.mak

# Rules
.PHONY: compatibility
compatibility:
	make -f compatibility.mak

# Debug and help
include $(CLI_DIR)/build/make/_help.mak

.PHONY: $(CLI_DIR)/tools/build/make/Makefile.help
help: $(CLI_DIR)/tools/build/make/Makefile.help
$(CLI_DIR)/tools/build/make/Makefile.help:
	$(call PrintHelp, compatibility, Make copies of tools at their previous location in the archive)
