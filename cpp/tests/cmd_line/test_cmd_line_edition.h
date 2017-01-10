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


#ifndef __CLI_TEST_CMDLINE_EDITION_H__
#define __CLI_TEST_CMDLINE_EDITION_H__


// Public includes
#include "cli/string_device.h"

// Private includes
#include "command_line_edition.h"
#include "constraints.h"

// Test includes
#include "test_error.h"


static const bool CheckEdition(
    const cli::CallInfo& CLI_CallInfo,
    const cli::CmdLineEdition& CLI_CmdLine, const char* const STR_Left, const char* const STR_Right,
    cli::StringDevice& CLI_Out, const char* const STR_Out)
{
    bool b_Res = true;

    if ((CLI_CmdLine.GetLeft() != STR_Left) || (CLI_CmdLine.GetRight() != STR_Right)
        || (CLI_CmdLine.GetLine() != cli::tk::String::Concat(256, STR_Left, STR_Right)))
    {
        TestError(CLI_CallInfo) << cli::endl
            << " left: '" << CLI_CmdLine.GetLeft() << "' ('" << STR_Left << "' expected)" << cli::endl
            << " right: '" << CLI_CmdLine.GetRight() << "' ('" << STR_Right << "' expected)" << cli::endl;
        b_Res = false;
    }
    if (CLI_Out.GetString() != STR_Out)
    {
        class Print { public: static const cli::tk::String String(const char* const STR_String) {
            const cli::tk::String tk_String(256, STR_String);
            cli::tk::String tk_Res(256);
            for (unsigned int ui = 0; ui < tk_String.GetLength(); ui ++) {
                switch (tk_String[ui]) {
                case '\b': tk_Res.Append("\\b"); break;
                case '\n': tk_Res.Append("\\n"); break;
                default: tk_Res.Append(tk_String[ui]); break;
                }
            }
            return tk_Res;
        }};
        TestError(CLI_CallInfo) << cli::endl
            << " out: '" << Print::String(CLI_Out.GetString()) << "'" << cli::endl
            << " expected: '" << Print::String(STR_Out) << "'" << cli::endl;
        b_Res = false;
    }

    CLI_Out.Reset();
    return b_Res;
}

static const bool CheckEditionWords(
    const cli::CallInfo& CLI_CallInfo,
    const cli::CmdLineEdition& CLI_CmdLine, const char* const STR_Left, const char* const STR_Right)
{
    bool b_Res = true;

    if (CLI_CmdLine.GetPrevWord() != STR_Left)
    {
        TestError(CLI_CallInfo) << "left: '" << CLI_CmdLine.GetPrevWord() << "' ('" << STR_Left << "' expected)" << cli::endl;
        b_Res = false;
    }
    if (CLI_CmdLine.GetNextWord() != STR_Right)
    {
        TestError(CLI_CallInfo) << "right: '" << CLI_CmdLine.GetNextWord() << "' ('" << STR_Right << "' expected)" << cli::endl;
        b_Res = false;
    }

    return b_Res;
}

//! @brief CmdLineEdition unit test.
static const bool TestCmdLineEdition(void)
{
    cli::CmdLineEdition cli_CmdLine;
    cli::StringDevice cli_Out(256, false);

    // Append the command line.
    cli_CmdLine.Put(cli_Out, '0');
        if (! CheckEdition(__CALL_INFO__, cli_CmdLine, "0", "", cli_Out, "0")) return false;
    cli_CmdLine.Put(cli_Out, cli::tk::String(10, "1"));
        if (! CheckEdition(__CALL_INFO__, cli_CmdLine, "01", "", cli_Out, "1")) return false;

    // Move the cursor within the command line.
    cli_CmdLine.Home(cli_Out);
        if (! CheckEdition(__CALL_INFO__, cli_CmdLine, "", "01", cli_Out, "\b\b")) return false;
    cli_CmdLine.MoveCursor(cli_Out, 1);
        if (! CheckEdition(__CALL_INFO__, cli_CmdLine, "0", "1", cli_Out, "0")) return false;
    cli_CmdLine.MoveCursor(cli_Out, 10);
        if (! CheckEdition(__CALL_INFO__, cli_CmdLine, "01", "", cli_Out, "1")) return false;
    cli_CmdLine.MoveCursor(cli_Out, -1);
        if (! CheckEdition(__CALL_INFO__, cli_CmdLine, "0", "1", cli_Out, "\b")) return false;
    cli_CmdLine.MoveCursor(cli_Out, -2);
        if (! CheckEdition(__CALL_INFO__, cli_CmdLine, "", "01", cli_Out, "\b")) return false;
    cli_CmdLine.End(cli_Out);
        if (! CheckEdition(__CALL_INFO__, cli_CmdLine, "01", "", cli_Out, "01")) return false;
    // Next Line.
    cli_CmdLine.Home(cli_Out);
        if (! CheckEdition(__CALL_INFO__, cli_CmdLine, "", "01", cli_Out, "\b\b")) return false;
    cli_CmdLine.NextLine(cli_Out);
        if (! CheckEdition(__CALL_INFO__, cli_CmdLine, "", "01", cli_Out, "01\n")) return false;
    cli_CmdLine.MoveCursor(cli_Out, 1);
        if (! CheckEdition(__CALL_INFO__, cli_CmdLine, "0", "1", cli_Out, "0")) return false;
    cli_CmdLine.NextLine(cli_Out);
        if (! CheckEdition(__CALL_INFO__, cli_CmdLine, "0", "1", cli_Out, "1\n")) return false;
    cli_CmdLine.End(cli_Out);
        if (! CheckEdition(__CALL_INFO__, cli_CmdLine, "01", "", cli_Out, "1")) return false;
    cli_CmdLine.NextLine(cli_Out);
        if (! CheckEdition(__CALL_INFO__, cli_CmdLine, "01", "", cli_Out, "\n")) return false;

    // Insert characters.
    cli_CmdLine.Put(cli_Out, cli::tk::String(10, "89"));
        if (! CheckEdition(__CALL_INFO__, cli_CmdLine, "0189", "", cli_Out, "89")) return false;
    cli_CmdLine.MoveCursor(cli_Out, -2);
        if (! CheckEdition(__CALL_INFO__, cli_CmdLine, "01", "89", cli_Out, "\b\b")) return false;
    cli_CmdLine.Put(cli_Out, cli::tk::String(10, "234567"));
        if (! CheckEdition(__CALL_INFO__, cli_CmdLine, "01234567", "89", cli_Out, "23456789\b\b")) return false;
    cli_CmdLine.MoveCursor(cli_Out, -3);
        if (! CheckEdition(__CALL_INFO__, cli_CmdLine, "01234", "56789", cli_Out, "\b\b\b")) return false;

    // Delete characters.
    cli_CmdLine.Delete(cli_Out, 1);
        if (! CheckEdition(__CALL_INFO__, cli_CmdLine, "01234", "6789", cli_Out, "6789 \b\b\b\b\b")) return false;
    cli_CmdLine.Delete(cli_Out, -1);
        if (! CheckEdition(__CALL_INFO__, cli_CmdLine, "0123", "6789", cli_Out, "\b6789 \b\b\b\b\b")) return false;
    cli_CmdLine.Delete(cli_Out, 2);
        if (! CheckEdition(__CALL_INFO__, cli_CmdLine, "0123", "89", cli_Out, "89  \b\b\b\b")) return false;
    cli_CmdLine.Delete(cli_Out, -2);
        if (! CheckEdition(__CALL_INFO__, cli_CmdLine, "01", "89", cli_Out, "\b\b89  \b\b\b\b")) return false;
    cli_CmdLine.Delete(cli_Out, 3);
        if (! CheckEdition(__CALL_INFO__, cli_CmdLine, "01", "", cli_Out, "  \b\b")) return false;
    cli_CmdLine.Delete(cli_Out, -3);
        if (! CheckEdition(__CALL_INFO__, cli_CmdLine, "", "", cli_Out, "\b\b  \b\b")) return false;
    cli_CmdLine.Put(cli_Out, cli::tk::String(10, "abcdef"));
        if (! CheckEdition(__CALL_INFO__, cli_CmdLine, "abcdef", "", cli_Out, "abcdef")) return false;
    cli_CmdLine.MoveCursor(cli_Out, -1);
        if (! CheckEdition(__CALL_INFO__, cli_CmdLine, "abcde", "f", cli_Out, "\b")) return false;
    cli_CmdLine.CleanAll(cli_Out);
        if (! CheckEdition(__CALL_INFO__, cli_CmdLine, "", "", cli_Out, " \b\b\b\b\b\b     \b\b\b\b\b")) return false;

    // Insert mode.
    cli_CmdLine.Put(cli_Out, cli::tk::String(10, "abcde"));
        if (! CheckEdition(__CALL_INFO__, cli_CmdLine, "abcde", "", cli_Out, "abcde")) return false;
    cli_CmdLine.MoveCursor(cli_Out, -2);
        if (! CheckEdition(__CALL_INFO__, cli_CmdLine, "abc", "de", cli_Out, "\b\b")) return false;
    cli_CmdLine.Put(cli_Out, cli::tk::String(10, "01"));
        if (! CheckEdition(__CALL_INFO__, cli_CmdLine, "abc01", "de", cli_Out, "01de\b\b")) return false;
    cli_CmdLine.SetInsertMode(false);
    cli_CmdLine.Put(cli_Out, '2');
        if (! CheckEdition(__CALL_INFO__, cli_CmdLine, "abc012", "e", cli_Out, "2")) return false;
    cli_CmdLine.SetInsertMode(true);
    cli_CmdLine.Put(cli_Out, '3');
        if (! CheckEdition(__CALL_INFO__, cli_CmdLine, "abc0123", "e", cli_Out, "3e\b")) return false;
    cli_CmdLine.SetInsertMode(false);
    cli_CmdLine.Put(cli_Out, cli::tk::String(10, "45"));
        if (! CheckEdition(__CALL_INFO__, cli_CmdLine, "abc012345", "", cli_Out, "45")) return false;

    // Set & Reset & Display.
    cli_CmdLine.Set(cli::tk::String(10, "012345"), cli::tk::String(10, "abcdef"));
        if (! CheckEdition(__CALL_INFO__, cli_CmdLine, "012345", "abcdef", cli_Out, "")) return false;
    cli_CmdLine.PrintCmdLine(cli_Out);
        if (! CheckEdition(__CALL_INFO__, cli_CmdLine, "012345", "abcdef", cli_Out, "012345abcdef\b\b\b\b\b\b")) return false;
    cli_CmdLine.Reset();
        if (! CheckEdition(__CALL_INFO__, cli_CmdLine, "", "", cli_Out, "")) return false;

    // Word analysis.
    cli_CmdLine.Put(cli_Out, cli::tk::String(256, "This is just a sample sentence."));
        if (! CheckEdition(__CALL_INFO__, cli_CmdLine, "This is just a sample sentence.", "", cli_Out, "This is just a sample sentence.")) return false;
        if (! CheckEditionWords(__CALL_INFO__, cli_CmdLine, "sentence.", "")) return false;
    cli_CmdLine.MoveCursor(cli_Out, -3);
        if (! CheckEdition(__CALL_INFO__, cli_CmdLine, "This is just a sample senten", "ce.", cli_Out, "\b\b\b")) return false;
        if (! CheckEditionWords(__CALL_INFO__, cli_CmdLine, "senten", "ce.")) return false;
    cli_CmdLine.MoveCursor(cli_Out, - (int) cli_CmdLine.GetPrevWord().GetLength());
        if (! CheckEdition(__CALL_INFO__, cli_CmdLine, "This is just a sample ", "sentence.", cli_Out, "\b\b\b\b\b\b")) return false;
        if (! CheckEditionWords(__CALL_INFO__, cli_CmdLine, "sample ", "sentence.")) return false;
    cli_CmdLine.MoveCursor(cli_Out, - 1);
        if (! CheckEdition(__CALL_INFO__, cli_CmdLine, "This is just a sample", " sentence.", cli_Out, "\b")) return false;
        if (! CheckEditionWords(__CALL_INFO__, cli_CmdLine, "sample", " sentence.")) return false;
    cli_CmdLine.MoveCursor(cli_Out, - (int) cli_CmdLine.GetPrevWord().GetLength());
        if (! CheckEdition(__CALL_INFO__, cli_CmdLine, "This is just a ", "sample sentence.", cli_Out, "\b\b\b\b\b\b")) return false;
        if (! CheckEditionWords(__CALL_INFO__, cli_CmdLine, "a ", "sample")) return false;
    cli_CmdLine.MoveCursor(cli_Out, (int) cli_CmdLine.GetNextWord().GetLength());
        if (! CheckEdition(__CALL_INFO__, cli_CmdLine, "This is just a sample", " sentence.", cli_Out, "sample")) return false;
        if (! CheckEditionWords(__CALL_INFO__, cli_CmdLine, "sample", " sentence.")) return false;
    cli_CmdLine.Home(cli_Out);
        if (! CheckEdition(__CALL_INFO__, cli_CmdLine, "", "This is just a sample sentence.", cli_Out, "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b")) return false;
        if (! CheckEditionWords(__CALL_INFO__, cli_CmdLine, "", "This")) return false;

    // Test the command line does not grow without limit.
    cli_CmdLine.Reset();
    cli::tk::String tk_Left(cli::MAX_CMD_LINE_LENGTH + 10);
    for (unsigned int ui = 0; ui < cli::MAX_CMD_LINE_LENGTH - 2; ui ++)
    {
        cli_CmdLine.Put(cli_Out, '.');
        tk_Left.Append(".");
            if (! CheckEdition(__CALL_INFO__, cli_CmdLine, tk_Left, "", cli_Out, ".")) return false;
    }
    cli_CmdLine.Put(cli_Out, cli::tk::String(10, "abcde"));
    tk_Left.Append("ab");
        if (! CheckEdition(__CALL_INFO__, cli_CmdLine, tk_Left, "", cli_Out, "ab")) return false;
    cli_CmdLine.Put(cli_Out, '.');
        if (! CheckEdition(__CALL_INFO__, cli_CmdLine, tk_Left, "", cli_Out, "")) return false;

    return true;
}

#endif // __CLI_TEST_CMDLINE_EDITION_H__
