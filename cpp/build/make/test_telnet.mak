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
TELNET_GOAL = check
ifeq ($(TELNET_GOAL),check)
.DEFAULT_GOAL = check
endif
ifeq ($(TELNET_GOAL),server)
.DEFAULT_GOAL = server
endif
ifeq ($(TELNET_GOAL),client)
.DEFAULT_GOAL = client
endif
.PHONY: telnet.default
telnet.default: $(.DEFAULT_GOAL) ;

# Variables
CLI_DIR := ../../..
SRC_DIR = $(CLI_DIR)/cpp/tests/telnet
ifeq ($(TELNET_GOAL),check)
	include _vars.mak
	include $(CLI_DIR)/build/make/_utils.mak
	PROJECT = test_telnet
	INT_DIR = $(TARGET)$(CXX)/$(RDX)/__$(PROJECT)
	OUT_DIR = $(TARGET)$(CXX)/$(RDX)/__$(PROJECT)
	SERVER_BINARY = $(TARGET)$(CXX)/$(RDX)/__test_telnet_server/$(BIN_PREFIX)test_telnet_server$(BIN_SUFFIX)
	CLIENT_BINARY = $(TARGET)$(CXX)/$(RDX)/__test_telnet_client/$(BIN_PREFIX)test_telnet_client$(BIN_SUFFIX)
	TELNET_PORT = 9012
	TELNET_TEST = $(INT_DIR)/empty.test
	TELNET_LOG = $(INT_DIR)/empty.log
	TELNET_CHECK = $(INT_DIR)/empty.check
endif
ifeq ($(TELNET_GOAL),server)
	PROJECT = test_telnet_server
	PROJECT_DEPS = libclicpp.mak
	CLI_XML = $(CLI_DIR)/samples/user-guide/empty.xml
	CLI_H = $(INT_DIR)/empty.h
	CPP_FILES = $(SRC_DIR)/goserver.cpp
	AUTO_DEPS = no
	PROJ_INCLUDES += -I$(CLI_DIR)/cpp/include
	PROJ_INCLUDES += -I$(INT_DIR)
	PROJ_INCLUDES += -I$(CLI_DIR)/samples/user-guide
	PROJ_LIBS = -L$(dir $(CLI_CPP_LIB)) -lclicpp -lncurses
	INT_DIR = $(TARGET)$(CXX)/$(RDX)/__$(PROJECT)
	OUT_DIR = $(TARGET)$(CXX)/$(RDX)/__$(PROJECT)
	PROJ_CLEAN = $(CLI_H)
	include _build.mak
	CPP_FLAGS += -Wno-unused-label
endif
ifeq ($(TELNET_GOAL),client)
	PROJECT = test_telnet_client
	PROJECT_DEPS = libclicpp.mak
	CPP_FILES = $(SRC_DIR)/goclient.cpp
	AUTO_DEPS = no
	PROJ_INCLUDES = -I$(CLI_DIR)/cpp/include
	PROJ_LIBS = -L$(dir $(CLI_CPP_LIB)) -lclicpp -lncurses
	INT_DIR = $(TARGET)$(CXX)/$(RDX)/__$(PROJECT)
	OUT_DIR = $(TARGET)$(CXX)/$(RDX)/__$(PROJECT)
	include _build.mak
endif

# Rules
ifeq ($(TELNET_GOAL),check)
.PHONY: check
check: $(TELNET_LOG) $(TELNET_CHECK)
	dos2unix $(TELNET_LOG) 2> /dev/null
	dos2unix $(TELNET_CHECK) 2> /dev/null
	diff $(TELNET_LOG) $(TELNET_CHECK)

$(TELNET_TEST): $(CLI_DIR)/samples/user-guide/empty.test test_telnet.mak
	rm -f $@
	$(call CheckDir,$(dir $@))
	echo "" >> $@
	echo "bad" >> $@
	echo "hello" >> $@
	cat $< >> $@

$(TELNET_LOG): $(SERVER_BINARY) $(CLIENT_BINARY) $(TELNET_TEST) $(CLI_DIR)/samples/clean_outlog.sh
	dos2unix $(TELNET_TEST) 2> /dev/null
	$(SERVER_BINARY) $(TELNET_PORT) &
	sleep 1
	$(call CheckDir,$(dir $(TELNET_LOG)))
	$(CLIENT_BINARY) $(TELNET_PORT) $(TELNET_TEST) > $(TELNET_LOG)
	$(call CheckSh,$(CLI_DIR)/samples/clean_outlog.sh)
	$(CLI_DIR)/samples/clean_outlog.sh $(TELNET_LOG)
	$(call CheckSh,test_telnet.kill.sh)
	./test_telnet.kill.sh

$(TELNET_CHECK): $(CLI_DIR)/samples/user-guide/empty.check test_telnet.mak
	rm -f $@
	$(call CheckDir,$(dir $@))
	echo "Enter password: " >> $@
	echo "Enter password: ***" >> $@
	echo "Wrong password!" >> $@
	echo "Enter password: *****" >> $@
	cat $< | sed -e "s/[^>]*: Syntax error next to /Syntax error next to /" >> $@

.PHONY: server $(SERVER_BINARY)
server: $(SERVER_BINARY)
$(SERVER_BINARY):
	$(MAKE) -f test_telnet.mak TELNET_GOAL=server

.PHONY: client $(CLIENT_BINARY)
client: $(CLIENT_BINARY)
$(CLIENT_BINARY):
	$(MAKE) -f test_telnet.mak TELNET_GOAL=client

.PHONY: deps
deps:
	$(MAKE) -f test_telnet.mak TELNET_GOAL=server deps
	$(MAKE) -f test_telnet.mak TELNET_GOAL=client deps

.PHONY: clean
clean:
	$(MAKE) -C $(CLI_DIR)/cpp/build/make -f test_telnet.mak TELNET_GOAL=server clean
	$(MAKE) -C $(CLI_DIR)/cpp/build/make -f test_telnet.mak TELNET_GOAL=client clean
	rm -f $(TELNET_TEST) $(TELNET_LOG) $(TELNET_CHECK)
	$(call RemoveDir,$(INT_DIR))
endif

ifeq ($(TELNET_GOAL),server)
.PHONY: server
server: build ;

$(CLI_H): $(CLI_XML)
	xsltproc --stringparam STR_CliClassName "EmptyCli" ../../../tools/cli2cpp.xsl $< > $@.tmp
	mv $@.tmp $@
endif

ifeq ($(TELNET_GOAL),client)
.PHONY: client
client: build ;
endif

# Debug and help
include $(CLI_DIR)/build/make/_help.mak

ifeq ($(TELNET_GOAL),check)
.PHONY: $(CLI_DIR)/cpp/build/make/telnet.help
help: $(CLI_DIR)/cpp/build/make/telnet.help
$(CLI_DIR)/cpp/build/make/telnet.help:
	$(call PrintHelp, check,  Check test output)
	$(call PrintHelp, server, Generate the server program)
	$(call PrintHelp, client, Generate the client program)
	$(call PrintHelp, clean,  Clean outputs)

.PHONY: $(CLI_DIR)/cpp/build/make/telnet.vars
vars: $(CLI_DIR)/cpp/build/make/telnet.vars
$(CLI_DIR)/cpp/build/make/telnet.vars:
	$(call ShowVariables,TELNET_GOAL SRC_DIR INT_DIR SERVER_BINARY CLIENT_BINARY TELNET_PORT TELNET_TEST TELNET_LOG TELNET_CHECK)
endif

# Dependencies
ifeq ($(TELNET_GOAL),server)
$(INT_DIR)/goserver.o: $(SRC_DIR)/goserver.cpp $(CLI_H) $(wildcard $(CLI_DIR)/cpp/include/cli/*.h) $(CLI_DIR)/samples/user-guide/pwd_shell.h
$(PRODUCT): $(CLI_CPP_LIB)
endif
ifeq ($(TELNET_GOAL),client)
$(INT_DIR)/goclient.o: $(SRC_DIR)/goclient.cpp $(wildcard $(CLI_DIR)/cpp/include/cli/*.h)
$(PRODUCT): $(CLI_CPP_LIB)
endif
