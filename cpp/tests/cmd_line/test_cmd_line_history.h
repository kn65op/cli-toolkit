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


#ifndef __CLI_TEST_CMDLINE_HISTORY_H__
#define __CLI_TEST_CMDLINE_HISTORY_H__


// System includes
#include <stdarg.h>

// Public includes
#include "cli/string_device.h"

// Private includes
#include "command_line_edition.h"
#include "command_line_history.h"

// Test includes
#include "test_error.h"


static const bool CheckHistory(
    const cli::CallInfo& CLI_CallInfo,
    const cli::CmdLineHistory& CLI_History, const unsigned int UI_Count, ...)
{
    if (CLI_History.GetCount() != UI_Count)
    {
        TestError(CLI_CallInfo) << "bad count " << CLI_History.GetCount() << " instead of " << UI_Count << cli::endl;
        return false;
    }

    va_list t_Args;
    va_start(t_Args, UI_Count);
    for (unsigned int ui=0; ui<UI_Count; ui++)
    {
        if (const cli::CmdLineEdition* const pcli_CmdLine = va_arg(t_Args, const cli::CmdLineEdition*))
        {
            if (CLI_History.GetLine(ui).GetLine() != pcli_CmdLine->GetLine())
            {
                TestError(CLI_CallInfo) << "Argument #" << ui << ": "
                                        << "incorrect string '" << CLI_History.GetLine(0).GetLine() << "' instead of '" << pcli_CmdLine->GetLine() << "'" << cli::endl;
                return false;
            }
        }
        else
        {
            TestError(CLI_CallInfo) << "Internal error: invalid argument #" << ui << cli::endl;
            return false;
        }
    }
    va_end(t_Args);

    // Successful return.
    return true;
}

static const bool CheckNavigation(
    const cli::CallInfo& CLI_CallInfo,
    cli::CmdLineHistory& CLI_History, const int I_Navigation,
    const bool B_Res, const cli::CmdLineEdition& CLI_Line)
{
    cli::CmdLineEdition cli_CmdLine;
    cli_CmdLine.Set(cli::tk::String(20, "current line"), cli::tk::String(20));
    cli::StringDevice cli_Output(256, false);

    // Compute the expected output.
    cli::StringDevice cli_ExpectedOutput(256, false);
    cli_ExpectedOutput << "\b\b\b\b\b\b\b\b\b\b\b\b            \b\b\b\b\b\b\b\b\b\b\b\b"; // Backspace - blank - backspace in order to remove "current line".
    CLI_Line.PrintCmdLine(cli_ExpectedOutput);

    // Execute navigation.
    const bool b_Res = CLI_History.Navigate(cli_CmdLine, cli_Output, I_Navigation);

    // Check result.
    if (b_Res != B_Res)
    {
        TestError(CLI_CallInfo) << "Incorrect result" << cli::endl;
        return false;
    }

    // Check the current command line.
    if (cli_CmdLine.GetLine() != CLI_Line.GetLine())
    {
        TestError(CLI_CallInfo) << "Incorrect string '" << cli_CmdLine.GetLine() << "' instead of '" << CLI_Line.GetLine() << "'" << cli::endl;
        return false;
    }

    // Check output.
    if (cli_Output.GetString() != cli_ExpectedOutput.GetString())
    {
        TestError(CLI_CallInfo) << "Incorrect output string '" << cli_Output.GetString() << "' instead of '" << cli_ExpectedOutput.GetString() << "'" << cli::endl;
        return false;
    }

    // Successful return.
    return true;
}

//! @brief CmdLineHistory unit test.
static const bool TestCmdLineHistory(void)
{
    // Create the history line object.
    cli::CmdLineHistory cli_History(5);

    // Create lines.
    cli::CmdLineEdition cli_EmptyLine;
    cli::CmdLineEdition cli_Line[7];
    cli::CmdLineEdition& cli_CurrentLine = cli_Line[0];
    cli_CurrentLine.Set(cli::tk::String(20, "current line"), cli::tk::String(20));
    for (int i=1; i<7; i++)
    {
        const cli::StringDevice cli_Content(20, false);
        cli_Content << "history " << i;
        (cli_Line[i]).Set(cli_Content.GetString(), cli::tk::String(20));
    }

    // Initial state.
    if (! CheckHistory(__CALL_INFO__, cli_History, 1, & cli_EmptyLine)) return false;

    // Set the current line.
    cli_History.SaveCurrentLine(cli_CurrentLine);
    if (! CheckHistory(__CALL_INFO__, cli_History, 1, & cli_CurrentLine)) return false;

    // Push history 1.
    cli_History.Push(cli_Line[1]);
    if (! CheckHistory(__CALL_INFO__, cli_History, 2, & cli_EmptyLine, & cli_Line[1])) return false;

    // Push history 2.
    cli_History.Push(cli_Line[2]);
    if (! CheckHistory(__CALL_INFO__, cli_History, 3, & cli_EmptyLine, & cli_Line[2], & cli_Line[1])) return false;

    // Push history 3.
    cli_History.Push(cli_Line[3]);
    if (! CheckHistory(__CALL_INFO__, cli_History, 4, & cli_EmptyLine, & cli_Line[3], & cli_Line[2], & cli_Line[1])) return false;

    // Set the current line.
    cli_History.SaveCurrentLine(cli_CurrentLine);
    if (! CheckHistory(__CALL_INFO__, cli_History, 4, & cli_CurrentLine, & cli_Line[3], & cli_Line[2], & cli_Line[1])) return false;

    // Push the same line.
    cli_History.Push(cli_Line[3]);
    if (! CheckHistory(__CALL_INFO__, cli_History, 4, & cli_CurrentLine, & cli_Line[3], & cli_Line[2], & cli_Line[1])) return false;

    // Push history 4.
    cli_History.Push(cli_Line[4]);
    if (! CheckHistory(__CALL_INFO__, cli_History, 5, & cli_EmptyLine, & cli_Line[4], & cli_Line[3], & cli_Line[2], & cli_Line[1])) return false;

    // Push history 5.
    cli_History.Push(cli_Line[5]);
    if (! CheckHistory(__CALL_INFO__, cli_History, 6, & cli_EmptyLine, & cli_Line[5], & cli_Line[4], & cli_Line[3], & cli_Line[2], & cli_Line[1])) return false;

    // Push history 6.
    cli_History.Push(cli_Line[6]);
    if (! CheckHistory(__CALL_INFO__, cli_History, 6, & cli_EmptyLine, & cli_Line[6], & cli_Line[5], & cli_Line[4], & cli_Line[3], & cli_Line[2])) return false;

    // Navigation
    cli_History.SaveCurrentLine(cli_CurrentLine);
    if (! CheckNavigation(__CALL_INFO__, cli_History, 3, true, cli_Line[4])) return false;
    if (! CheckNavigation(__CALL_INFO__, cli_History, 1, true, cli_Line[3])) return false;
    if (! CheckNavigation(__CALL_INFO__, cli_History, -2, true, cli_Line[5])) return false;
    if (! CheckNavigation(__CALL_INFO__, cli_History, 0, true, cli_Line[5])) return false;
    if (! CheckNavigation(__CALL_INFO__, cli_History, -5, false, cli_CurrentLine)) return false;
    if (! CheckNavigation(__CALL_INFO__, cli_History, -1, false, cli_CurrentLine)) return false;
    if (! CheckNavigation(__CALL_INFO__, cli_History, 10, false, cli_Line[2])) return false;
    if (! CheckNavigation(__CALL_INFO__, cli_History, 1, false, cli_Line[2])) return false;
    cli_History.EnableNavigationMemory(false);
    if (! CheckNavigation(__CALL_INFO__, cli_History, 1, true, cli_Line[6])) return false;
    cli_History.EnableNavigationMemory(false);
    cli_History.EnableNavigationMemory(true);
    if (! CheckNavigation(__CALL_INFO__, cli_History, 2, true, cli_Line[4])) return false;

    // Clear
    cli_History.Clear();
    if (! CheckHistory(__CALL_INFO__, cli_History, 1, & cli_EmptyLine)) return false;

    // Navigate again
    if (! CheckNavigation(__CALL_INFO__, cli_History, 1, false, cli_EmptyLine)) return false;

    return true;
}

#endif // __CLI_TEST_CMDLINE_HISTORY_H__
