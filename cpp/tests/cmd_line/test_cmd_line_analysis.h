/*
    Copyright (c) 2006-2013, Alexis Royer, http://alexis.royer.free.fr/CLI

    All rights reserved.

    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

        * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation
          and/or other materials provided with the distribution.
        * Neither the name of the CLI library project nor the names of its contributors may be used to endorse or promote products derived from this software
          without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
    CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
    EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
    PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
    PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
    LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#ifndef __CLI_TEST_CMDLINE_ANALYSIS_H__
#define __CLI_TEST_CMDLINE_ANALYSIS_H__

#include "cli/menu.h"
#include "cli/cli.h"
#include "cli/command_line.h"

#include "test_error.h"
#include "test_cli.h"


static const bool CheckAnalysis(
    const cli::CallInfo& CLI_CallInfo,
    const char* const STR_CmdLine, const bool B_Res,
    const char* const STR_Error)
{
    if ((STR_CmdLine != NULL) && (STR_Error != NULL))
    {
        cli::CommandLine cli_CmdLine;
        cli::Menu cli_Menu("test", cli::Help());
        cli_Menu.SetCli(GetTestCli());

        const bool b_Res = cli_CmdLine.Parse(cli_Menu, cli::tk::String((unsigned int) strlen(STR_CmdLine), STR_CmdLine), true);
        if (b_Res != B_Res) {
            TestError(CLI_CallInfo) << "Incorrect result: " << cli_CmdLine.GetLastError().GetString(cli::ResourceString::LANG_EN) << cli::endl;
            return false;
        }

        if (cli_CmdLine.GetLastError().GetString(cli::ResourceString::LANG_EN) != STR_Error) {
            TestError(CLI_CallInfo) << "Unexpected error '" << cli_CmdLine.GetLastError().GetString(cli::ResourceString::LANG_EN) << "' "
                                    << "instead of '" << STR_Error << "'" << cli::endl;
            return false;
        }

        return true;
    }

    return false;
}

//! @brief CommandLine unit test.
static const bool TestCmdLineAnalysis(void)
{
    // Regular behaviour
    if (! CheckAnalysis(__CALL_INFO__, "a", false, "Syntax error next to 'a'")) return false;
    if (! CheckAnalysis(__CALL_INFO__, "help\n", true, "")) return false;
    // Too long command line: CLI_MAX_CMD_LINE_LENGTH = 256 // Not managed by the CommandLine class but by the CommandLineEdition class.
    //  if (! CheckAnalysis(__FILE__, __LINE__, "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdefXXX",
    //                                           false, "?"))
    //      return false;
    // Too many words: CLI_MAX_CMD_LINE_WORD_COUNT = 32
    if (! CheckAnalysis(__CALL_INFO__, "1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33", false, "Too many words in command line")) return false;
    // Too long word: CLI_MAX_WORD_LENGTH = 16
    if (! CheckAnalysis(__CALL_INFO__, "a 0123456789abcdefX", false, "Too long word '0123456789...'")) return false;

    return true;
}

#endif // __CLI_TEST_CMDLINE_ANALYSIS_H__
