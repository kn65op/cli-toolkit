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


#include "cli/pch.h"

#include "cli/ui_line.h"

#include "test_error.h"
#include "test_device.h"
#include "test_exec.h"


// cli::ui::Line unit-tests.
const bool CheckGetLine(void)
{
    class Test {
    public:
        static const bool Line(
                const cli::CallInfo& CLI_CallInfo,
                const char* const STR_Default, const char* const STR_Input, const unsigned int UI_MinLength, const unsigned int UI_MaxLength,
                const bool B_ExpectedResult, const char* const STR_StringOutput, const char* const STR_DeviceOutput)
        {
            TestBlockingDevice cli_BlockingDevice(false); // Declare at first for consistency reasons.
            cli::ui::Line cli_Line1(cli::tk::String(UI_MaxLength, STR_Default), UI_MinLength, UI_MaxLength);
            if (! Line(CLI_CallInfo, cli_BlockingDevice, cli_BlockingDevice, cli_Line1, STR_Default, STR_Input, UI_MinLength, UI_MaxLength, B_ExpectedResult, STR_StringOutput, STR_DeviceOutput)) {
                return false;
            }
            TestNonBlockingDevice cli_NonBlockingDevice(false); // Declare at first for consistency reasons.
            cli::ui::Line cli_Line2(cli::tk::String(UI_MaxLength, STR_Default), UI_MinLength, UI_MaxLength);
            if (! Line(CLI_CallInfo, cli_NonBlockingDevice, cli_NonBlockingDevice, cli_Line2, STR_Default, STR_Input, UI_MinLength, UI_MaxLength, B_ExpectedResult, STR_StringOutput, STR_DeviceOutput)) {
                return false;
            }
            return true;
        }
    public:
        static const bool Line(
                const cli::CallInfo& CLI_CallInfo, cli::IODevice& CLI_TestDevice, TestDeviceData& CLI_TestDeviceData,
                cli::ui::Line& CLI_Line,
                const char* const STR_Default, const char* const STR_Input, const unsigned int UI_MinLength, const unsigned int UI_MaxLength,
                const bool B_ExpectedResult, const char* const STR_StringOutput, const char* const STR_DeviceOutput)
        {
            CLI_Line.SetStream(cli::WELCOME_STREAM, cli::OutputDevice::GetNullDevice());
            CLI_Line.SetStream(cli::PROMPT_STREAM, cli::OutputDevice::GetNullDevice());

            const bool b_Result = (
                CheckExecuteContext(CLI_CallInfo, CLI_Line, CLI_TestDevice, STR_Input)
                && CLI_Line.GetbExecResult()
            );

            if (b_Result != B_ExpectedResult) {
                TestError(CLI_CallInfo) << "UI::GetLine() returned " << b_Result << " (" << B_ExpectedResult << " was expected)" << cli::endl;
                return false;
            }
            if (strcmp((const char* const) CLI_Line.GetLine(), STR_StringOutput) != 0) {
                TestError(CLI_CallInfo) << "String output '" << CLI_Line.GetLine() << "' does not match '" << STR_StringOutput << "'" << cli::endl;
                return false;
            }
            const cli::tk::String cli_OutputString = CLI_TestDeviceData.GetOutput();
            if (strcmp((const char* const) cli_OutputString, STR_DeviceOutput) != 0) {
                TestError(CLI_CallInfo) << "Device output '" << cli_OutputString << "' does not match '" << STR_DeviceOutput << "'" << cli::endl;
                return false;
            }

            return true;
        }
    };

    // TEST basic character inputs.
    if (! Test::Line(__CALL_INFO__, "", "012345\n", 0, 10, true, "012345", Out().txt("012345").endl())) return false;
    if (! Test::Line(__CALL_INFO__, "", "0123456789\n", 0, 10, true, "0123456789", Out().txt("0123456789").endl())) return false;
    // TEST maximum line length.
    if (! Test::Line(__CALL_INFO__, "", "0123456789a\n", 0, 10, true, "0123456789", Out().txt("0123456789").beep().endl())) return false;
    if (! Test::Line(__CALL_INFO__, "", "012345\b6789a\n", 0, 10, true, "012346789a", Out().txt("012345").bsp(1).txt("6789a").endl())) return false;
    //      ... even though a wrong backspace at first.
    if (! Test::Line(__CALL_INFO__, "", "\b0123456789a\n", 0, 10, true, "0123456789", Out().beep().txt("0123456789").beep().endl())) return false;
    // TEST default line is discarded with typing over.
    // TEST minimum line length.
    if (! Test::Line(__CALL_INFO__, "-----", "0\n", 5, 10, false, "-----", Out().txt("-----").bsp(5).txt("0").beep().bsp(1).txt("-----").endl())) return false;

    // Test LEFT/RIGHT moves the cursor in the line (insert mode).
    if (! Test::Line(__CALL_INFO__, "abcdef", "%lx%r\n", 0, 10, true, "abcdexf", Out().txt("abcdef").left(1).txt("xf\b").right("f").endl())) return false;
    // Test LEFT/RIGHT moves the cursor in the line (replace mode).
    if (! Test::Line(__CALL_INFO__, "abcdef", "%i%lx%r\n", 0, 10, true, "abcdex", Out().txt("abcdef").left(1).txt("x").beep().endl())) return false;
    // Test UP/DOWN/PUP/PDOWN does nothing.
    if (! Test::Line(__CALL_INFO__, "abcdef", "%u%d\n", 0, 10, true, "abcdef", Out().txt("abcdef").endl())) return false;
    if (! Test::Line(__CALL_INFO__, "abcdef", "%U%D\n", 0, 10, true, "abcdef", Out().txt("abcdef").endl())) return false;
    // Test HOME/END keys.
    if (! Test::Line(__CALL_INFO__, "abcdef", "%H%E\n", 0, 10, true, "abcdef", Out().txt("abcdef").left(6).right("abcdef").endl())) return false;

    // Test ESCAPE  breaks the current input.
    if (! Test::Line(__CALL_INFO__, "abcdef", "\b\b%]", 0, 10, false, "abcdef", Out().txt("abcdef").bsp(1).bsp(1).bsp(4).txt("abcdef").endl())) return false;
    // Test CTRL+C breaks the current input.
    if (! Test::Line(__CALL_INFO__, "abcdef", "\b\b%!", 0, 10, false, "abcdef", Out().txt("abcdef").bsp(1).bsp(1).bsp(4).txt("abcdef").endl())) return false;

    // Bug! "When 'Insert' is pressed while the line is already full, characters are not taken in account"
    if (! Test::Line(__CALL_INFO__, "abcdef", "%l%l%i5\n", 0, 6, true, "abcd5f", Out().txt("abcdef").left(2).txt("5").right("f").endl())) return false;

    // Bug! "When a Line is used twice, text is deleted backward
    {
        TestBlockingDevice cli_BlockingDevice(false); // Declare at first for consistency reasons.
        TestNonBlockingDevice cli_NonBlockingDevice(false); // Declare at first for consistency reasons.
        cli::ui::Line cli_Line(cli::tk::String(10), 0, 10);
        if (! Test::Line(__CALL_INFO__, cli_BlockingDevice, cli_BlockingDevice, cli_Line, "", "012345\n", 0, 10, true, "012345", Out().txt("012345").endl())) return false;
        if (! Test::Line(__CALL_INFO__, cli_NonBlockingDevice, cli_NonBlockingDevice, cli_Line, "", "012345\n", 0, 10, true, "012345", Out().txt("012345").endl())) return false;
    }

    return true;
}
