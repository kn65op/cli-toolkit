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
.PHONY: java.default
java.default: $(.DEFAULT_GOAL) ;

ifeq ($(JAVA_HOME),)
$(warning Please set JAVA_HOME to your SDK installation directory and ensure \$JAVA_DOC/bin is in your PATH environment variable)
.PHONY: default
default: ;
else

# Includes
CLI_DIR := ../../..
include _vars.mak

# Library
PROJECTS += libclijava.mak
PROJECTS += jni.mak
PROJECTS += native.mak
# Samples
PROJECTS += empty.mak
PROJECTS += hello.mak
PROJECTS += clisample.mak
# Tests
check: PROJECTS =
check clean: PROJECTS += test_no_res.mak
check clean: PROJECTS += test_ui.mak
check clean: PROJECTS += test_io_device.mak
check clean: PROJECTS += test_non_blocking_device.mak
check clean: PROJECTS += test_exec_context.mak
check clean: PROJECTS += test_telnet.mak
check clean: PROJECTS += test_all_samples.mak
DEFAULT_HELP = Generate libraries, samples and tests
# Clean
CLEAN_DIR = $(OUT_DIR)/cli $(OUT_DIR)

include $(CLI_DIR)/build/make/_all.mak

endif
