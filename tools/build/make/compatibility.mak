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
.DEFAULT_GOAL = restore
.PHONY: compatibility.default
compatibility.default: $(.DEFAULT_GOAL) ;

# Includes
CLI_DIR := ../../..
include $(CLI_DIR)/build/make/_utils.mak

# Rules
.PHONY: restore
# CLI 2.7 schema files were located in a $(CLI_DIR)/schema directory.
restore: $(CLI_DIR)/schema/cli.xsd
restore: $(CLI_DIR)/schema/cli.rng
# CLI 2.7 cli2help XSL stylesheet was located in a global $(CLI_DIR)/xsl directory.
restore: $(CLI_DIR)/xsl/cli2help.xsl
# CLI 2.7 cli2cpp and cli2java XSL stylesheets were named cppclic and javaclic, and located in their respective $(CLI_DIR)/cpp/xsl and $(CLI_DIR)/java/xsl directories.
restore: $(CLI_DIR)/cpp/xsl/cppclic.xsl
restore: $(CLI_DIR)/java/xsl/javaclic.xsl
restore: ;

.PHONY: $(CLI_DIR)/schema/cli.xsd
$(CLI_DIR)/schema/cli.xsd: $(CLI_DIR)/tools/cli.xsd
	$(call CheckDir,$(dir $@))
	cp -f $< $@

.PHONY: $(CLI_DIR)/schema/cli.rng
$(CLI_DIR)/schema/cli.rng: $(CLI_DIR)/tools/cli.rng
	$(call CheckDir,$(dir $@))
	cp -f $< $@

.PHONY: $(CLI_DIR)/xsl/cli2help.xsl
$(CLI_DIR)/xsl/cli2help.xsl: $(CLI_DIR)/tools/cli2help.xsl
	$(call CheckDir,$(dir $@))
	cp -f $< $@

.PHONY: $(CLI_DIR)/cpp/xsl/cppclic.xsl
$(CLI_DIR)/cpp/xsl/cppclic.xsl: $(CLI_DIR)/tools/cli2cpp.xsl
	$(call CheckDir,$(dir $@))
	cp -f $< $@

.PHONY: $(CLI_DIR)/java/xsl/javaclic.xsl
$(CLI_DIR)/java/xsl/javaclic.xsl: $(CLI_DIR)/tools/cli2java.xsl
	$(call CheckDir,$(dir $@))
	cp -f $< $@

.PHONY: clean
clean:
	rm -f $(CLI_DIR)/schema/cli.xsd
	rm -f $(CLI_DIR)/schema/cli.rng
	$(call RemoveDir,$(CLI_DIR)/schema)
	rm -f $(CLI_DIR)/xsl/cli2help.xsl
	$(call RemoveDir,$(CLI_DIR)/xsl)
	rm -f $(CLI_DIR)/cpp/xsl/cppclic.xsl
	$(call RemoveDir,$(CLI_DIR)/cpp/xsl)
	rm -f $(CLI_DIR)/java/xsl/javaclic.xsl
	$(call RemoveDir,$(CLI_DIR)/java/xsl)

# Debug and help
include $(CLI_DIR)/build/make/_help.mak

.PHONY: $(CLI_DIR)/cpp/build/make/compatibility.help
help: $(CLI_DIR)/cpp/build/make/compatibility.help
$(CLI_DIR)/cpp/build/make/compatibility.help:
	$(call PrintHelp, restore, Restore tools at their deprecated location)
	$(call PrintHelp, clean,   Cancel backward compatibility copies)
