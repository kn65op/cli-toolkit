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

ifndef __ROOT_UTILS__
__ROOT_UTILS__ = 1


CLI_DIR ?= ../..

# Functions
IsSilent = $(if $(filter s,$(MAKEFLAGS))$(filter -s,$(MAKEFLAGS)),yes,)
Map =   @echo -n "" \
        $(foreach a,$(2), \
                $(if $(IsSilent),, && echo "$(call $(1),$(a))") \
                && $(call $(1),$(a)) \
        )
Map2 =  @echo -n "" \
        $(foreach a,$(2), \
                $(foreach b,$(3), \
                        $(if $(IsSilent),, && echo "$(call $(1),$(a),$(b))") \
                        && $(call $(1),$(a),$(b)) \
                ) \
        )

MkCallDefault = $(MAKE) -C $(dir $(1)) -f $(notdir $(1))
MkCallRule = $(MkCallDefault) $(2)
MkDispatch = $(if $(2),$(call Map2,MkCallRule,$(1),$(2)),$(call Map,MkCallDefault,$(1)))
IfEquals = $(if $(findstring $(strip $(1)),$(strip $(2))),$(if $(findstring $(strip $(1)),$(strip $(2))),$(strip $(3)),$(strip $(4))),$(strip $(4)))
CheckDir = @test -d $(1) || ($(if $(IsSilent),,echo "mkdir -p $(1)" &&) mkdir -p $(1))
RemoveDir = @echo -n "" \
        $(foreach a,$(1), \
                && ((test -d $(a) $(if $(IsSilent),,&& echo "rmdir $(a)") && rmdir $(a)) || true) \
        )

CheckSh = @((dos2unix $(1) 2> /dev/null) && chmod a+x $(1))

endif
# __ROOT_UTILS__
