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
.PHONY: test_io_device.default
test_io_device.default: $(.DEFAULT_GOAL) ;

# Includes
CLI_DIR := ../../..
include _vars.mak
PRODUCT = test_io_device
SRC_DIR = $(CLI_DIR)/java/src/cli/test
JAVA_FILES += $(CLI_DIR)/java/src/cli/test/TestIODevice.java
JAVA_FILES += $(CLI_DIR)/java/src/cli/test/TestTools.java
PROJECT_DEPS = libclijava.mak jni.mak native.mak
include _build.mak
JAVAC_OPTS += -nowarn
PROJ_CLEAN += $(OUT_DIR)/TestIODevice.input.cli
PROJ_CLEAN += $(OUT_DIR)/TestIODevice.welcome.log
PROJ_CLEAN += $(OUT_DIR)/TestIODevice.output.log
PROJ_CLEAN += $(OUT_DIR)/TestIODevice.echo.log

# Rules
.PHONY: check
check: build $(OUT_DIR)/TestIODevice.input.cli $(SRC_DIR)/TestIODevice.welcome.log $(SRC_DIR)/TestIODevice.output.log $(SRC_DIR)/TestIODevice.echo.log
	cd $(OUT_DIR) && java -Djava.library.path=. cli.test.TestIODevice
	dos2unix $(OUT_DIR)/TestIODevice.welcome.log $(SRC_DIR)/TestIODevice.welcome.log
	diff $(OUT_DIR)/TestIODevice.welcome.log $(SRC_DIR)/TestIODevice.welcome.log
	cat $(OUT_DIR)/TestIODevice.output.log | grep -v "^  traces          Traces menu" > $(OUT_DIR)/TestIODevice.output.log.tmp
	mv $(OUT_DIR)/TestIODevice.output.log.tmp $(OUT_DIR)/TestIODevice.output.log
	dos2unix $(OUT_DIR)/TestIODevice.output.log $(SRC_DIR)/TestIODevice.output.log
	diff $(OUT_DIR)/TestIODevice.output.log $(SRC_DIR)/TestIODevice.output.log
	dos2unix $(OUT_DIR)/TestIODevice.echo.log $(SRC_DIR)/TestIODevice.echo.log
	diff $(OUT_DIR)/TestIODevice.echo.log $(SRC_DIR)/TestIODevice.echo.log

$(OUT_DIR)/TestIODevice.input.cli: $(SRC_DIR)/TestIODevice.input.cli
	cp -f $< $@

.PHONY: deps
deps: ;

# Debug and help
include $(CLI_DIR)/build/make/_help.mak

.PHONY: $(CLI_DIR)/java/build/make/test_io_device.help
help: $(CLI_DIR)/java/build/make/test_io_device.help
$(CLI_DIR)/java/build/make/test_io_device.help:
	$(call PrintHelp, check, Run the IO device sample)

.PHONY: $(CLI_DIR)/java/build/make/test_io_device.vars
vars: $(CLI_DIR)/java/build/make/test_io_device.vars
$(CLI_DIR)/java/build/make/test_io_device.vars: ;

# Dependencies
build: $(JAVA_FILES)
