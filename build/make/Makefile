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
CLI_DIR := ../..
include _vars.mak
# Dispatch over C++, Java and tools
PROJECTS += $(CLI_DIR)/cpp/build/make/Makefile
PROJECTS += $(CLI_DIR)/java/build/make/Makefile
PROJECTS += $(CLI_DIR)/tools/build/make/Makefile
DEFAULT_HELP = Generate libraries and tests
CLEAN_DIR = $(OUT_DIR)
include _all.mak

# C++ only
.PHONY: cpp
cpp: PROJECTS = $(CLI_DIR)/cpp/build/make/Makefile
cpp: default ;

# Java only
.PHONY: java
java: PROJECTS = $(CLI_DIR)/java/build/make/Makefile
java: default ;

# Documentation
.PHONY: doc
doc: PROJECTS = $(CLI_DIR)/web/build/make/Makefile
doc: default ;

# Debug and help
include $(CLI_DIR)/build/make/_help.mak

.PHONY: $(CLI_DIR)/build/make/Makefile.help
help: $(CLI_DIR)/build/make/Makefile.help
$(CLI_DIR)/build/make/Makefile.help:
	$(call PrintHelp, cpp,      Generate/update C++ items)
	$(call PrintHelp, java,     Generate/update Java items)
	$(call PrintHelp, doc,      Generate/update documentation)
