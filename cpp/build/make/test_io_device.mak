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


# Includes
CLI_DIR := ../../..
PROJECT = test_io_device
SRC_DIR = $(CLI_DIR)/cpp/tests/io_device
CLI_XML_RES = $(SRC_DIR)/io_device.xml
CLI_MAIN_CPP = $(SRC_DIR)/io_device_impl.cpp
INT_DIR = $(TARGET)$(CXX)/$(RDX)/__$(PROJECT)
OUT_DIR = $(TARGET)$(CXX)/$(RDX)/__$(PROJECT)
include _check.mak
PROJ_CLEAN += $(CLI_LOG).lite

# Rules
$(CLI_LOG).lite: $(CLI_LOG) test_io_device.mak
	cat $< \
	| sed -e "s/\(<CLI_IO_DEVICE_INSTANCES>\|<CLI_IO_DEVICE_OPENING>\)\([^,]*\)\(\/[^,]*\)\(,\(.*\)\|\)/\1\2\4/" \
	| sed -e "s/ at .*:.*$$//g" \
	> $@
	cp -f $@ $<
	touch $@

# Dependencies
check: $(CLI_LOG).lite
