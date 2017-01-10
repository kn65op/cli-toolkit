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

ifndef __CPP_BUILD__
__CPP_BUILD__ = 1


##############
# Default goal
ifndef .DEFAULT_GOAL
.DEFAULT_GOAL = build
endif
.PHONY: cppbuild.default
cppbuild.default: $(.DEFAULT_GOAL) ;


######################
# Preliminary includes
CLI_DIR ?= ../../..
include $(CLI_DIR)/build/make/_vars.mak
include $(CLI_DIR)/cpp/build/make/_vars.mak
include $(CLI_DIR)/build/make/_utils.mak


##############
# Parameters
	# Project
		# Project name. It is generally similar to the makefile name.
		PROJECT ?= $(patsubst %.mak,%,$(firstword $(MAKEFILE_LIST)))
		# Project dependencies. Projects this one relies on. Build and clean rules will be proprgated to them.
		PROJECT_DEPS ?=
		# PRODUCT_TYPE can be one of (BIN|STATIC_LIB|DYN_LIB).
		PRODUCT_TYPE ?= BIN
		# Product file.
		PRODUCT ?= $(OUT_DIR)/$($(PRODUCT_TYPE)_PREFIX)$(PROJECT)$($(PRODUCT_TYPE)_SUFFIX)
	# Input sources
		# Input source directory.
		SRC_DIR ?= .
		# Input source list.
		CPP_FILES ?= $(wildcard $(SRC_DIR)/*.cpp)
		# Corresponding objects.
		OBJS ?= $(patsubst %.cpp,$(INT_DIR)/%.o,$(notdir $(CPP_FILES)))
	# Dependencies (.deps files)
		# Choose whether automatic dependencies should be performed.
		AUTO_DEPS ?= yes
		# Name of the automatic dependencies file.
		AUTO_DEPS_FILE ?= $(PROJECT).deps
	# C++ compilation options
		# Project C++ flags.
		PROJ_CPP_FLAGS ?=
		# Debug and optimization flags.
ifdef _DEBUG
		CPP_OPT_FLAGS ?=
		CPP_DEBUG_FLAG ?= -g -D_DEBUG
else
		CPP_OPT_FLAGS ?= -O2
		CPP_DEBUG_FLAG ?=
endif
ifneq ($(LINUX),)
		CPP_OS_FLAGS = -D_LINUX
endif
ifneq ($(CYGWIN),)
		CPP_OS_FLAGS = -D_CYGWIN
endif
		# All C++ flags
		CPP_FLAGS ?= -Wall $(CPP_OPT_FLAGS) $(CPP_DEBUG_FLAG) $(CPP_OS_FLAGS) $(PROJ_CPP_FLAGS)
		# Project includes
		PROJ_INCLUDES ?=
		# All includes
		INCLUDES ?= -I/usr/include $(PROJ_INCLUDES)
	# Link options
		RANLIB ?= ranlib
		PROJ_LIBS ?=
		LIBS ?= -L/usr/lib $(PROJ_LIBS)
ifneq ($(LINUX),)
		LIBS += -ldl
endif
	# Output
		OUT_DIR ?= $(TARGET)$(CXX)/$(RDX)
		INT_DIR ?= $(TARGET)$(CXX)/$(RDX)/__$(PROJECT)
	# Cleanup
		# Additional cleanup items
		PROJ_CLEAN ?=
		# Final cleanup item list
		CLEAN ?= $(OBJS) $(PRODUCT) $(PROJ_CLEAN)
		CLEAN_DIR ?= $(INT_DIR)


##############
# Includes
CLI_DIR ?= ../../..
include $(CLI_DIR)/build/make/_vars.mak


##############
# Rules

# Build
.PHONY: build
build: build_depends dirs $(PRODUCT) ;

.PHONY: build_depends
build_depends:
	$(call MkDispatch,$(PROJECT_DEPS),)

$(PRODUCT): NO_AUTO_IMPORT_WARNINGS = -Wl,--enable-auto-import
$(PRODUCT): EXPORT_CYGWIN_DLL_SYMBOLS = -Wl,--add-stdcall-alias
$(PRODUCT): $(OBJS)
ifeq ($(PRODUCT_TYPE),BIN)
	$(CXX) $(call IfEquals,$(TARGET),Cygwin,$(NO_AUTO_IMPORT_WARNINGS)) -o $(PRODUCT) $(OBJS) $(LIBS)
endif
ifeq ($(PRODUCT_TYPE),STATIC_LIB)
	$(AR) -cru $(PRODUCT) $(OBJS)
	$(RANLIB) $(PRODUCT)
endif
ifeq ($(PRODUCT_TYPE),DYN_LIB)
	$(call IfEquals,$(TARGET),Cygwin,$(CXX) -shared $(EXPORT_CYGWIN_DLL_SYMBOLS) -o $(PRODUCT) $(OBJS) $(LIBS))
	$(call IfEquals,$(TARGET),Linux,$(CXX) -shared -o $(PRODUCT) $(OBJS) $(LIBS))
endif

%.o:
	$(if $<,$(CXX) $(CPP_FLAGS) $(INCLUDES) -c $< -o $@,@echo "Error: dependency missing for $@. Please update dependencies ('deps' rule)." && false)

# Automatic dependencies
.PHONY: deps
ifeq ($(AUTO_DEPS),yes)
MkDepFile = $(CXX) $(CPP_FLAGS) $(INCLUDES) -MT $(patsubst %.cpp,$$\(INT_DIR\)/%.o,$(notdir $(1))) -MM $(1) >> $(AUTO_DEPS_FILE)
deps:
	rm -f $(AUTO_DEPS_FILE) && touch $(AUTO_DEPS_FILE)
	$(call Map,MkDepFile,$(CPP_FILES))
else
# Do nothing
deps: ;
endif

# Clean up and directories
.PHONY: dirs
dirs:
	$(call CheckDir,$(OUT_DIR))
	$(call CheckDir,$(INT_DIR))

.PHONY: clean
clean:
	$(call MkDispatch,$(PROJECT_DEPS),clean)
	rm -f $(CLEAN)
	$(call RemoveDir,$(CLEAN_DIR))

# Debug and help
.PHONY: $(CLI_DIR)/cpp/build/make/_build.help
help: $(CLI_DIR)/cpp/build/make/_build.help
$(CLI_DIR)/cpp/build/make/_build.help:
	$(call PrintHelp,   build,  Build $(notdir $(PRODUCT)))
	$(call PrintHelp,   dirs,   Check intermediate and output directories)
ifeq ($(AUTO_DEPS),yes)
	$(call PrintHelp,   deps,   Update dependencies file ($(AUTO_DEPS_FILE)))
endif
	$(call PrintHelp,   clean,  Clean intermediate and output files)

.PHONY: $(CLI_DIR)/cpp/build/make/_build.vars
vars: $(CLI_DIR)/cpp/build/make/_build.vars
$(CLI_DIR)/cpp/build/make/_build.vars: MY_VARS += MAKE CXX AR RANLIB
$(CLI_DIR)/cpp/build/make/_build.vars: MY_VARS += PROJECT PROJECT_DEPS PRODUCT_TYPE PRODUCT
$(CLI_DIR)/cpp/build/make/_build.vars: MY_VARS += SRC_DIR CPP_FILES OBJS
$(CLI_DIR)/cpp/build/make/_build.vars: MY_VARS += AUTO_DEPS AUTO_DEPS_FILE
$(CLI_DIR)/cpp/build/make/_build.vars: MY_VARS += PROJ_CPP_FLAGS CPP_OPT_FLAG CPP_DEBUG_FLAG CPP_OS_FLAGS CPP_FLAGS PROJ_INCLUDES INCLUDES
$(CLI_DIR)/cpp/build/make/_build.vars: MY_VARS += PROJ_LIBS LIBS
$(CLI_DIR)/cpp/build/make/_build.vars: MY_VARS += INT_DIR OUT_DIR
$(CLI_DIR)/cpp/build/make/_build.vars: MY_VARS += PROJ_CLEAN CLEAN CLEAN_DIR
$(CLI_DIR)/cpp/build/make/_build.vars:
	$(call ShowVariables,$(MY_VARS))


endif
# __CPP_BUILD__
