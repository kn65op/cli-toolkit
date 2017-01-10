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

#include "cli/ui_password.h"

#include "test_error.h"
#include "test_device.h"
#include "test_exec.h"


// cli::ui::Password unit-tests.
const bool CheckGetPassword(void)
{
    class Test {
    public:
        static const bool Password(
                const cli::CallInfo& CLI_CallInfo,
                const bool B_DisplayStars,
                const char* const STR_Input, const unsigned int UI_MinLength, const unsigned int UI_MaxLength,
                const bool B_ExpectedResult, const char* const STR_StringOutput, const char* const STR_DeviceOutput)
        {
            TestBlockingDevice cli_BlockingDevice(false);
            if (! Password(CLI_CallInfo, cli_BlockingDevice, cli_BlockingDevice, B_DisplayStars, STR_Input, UI_MinLength, UI_MaxLength, B_ExpectedResult, STR_StringOutput, STR_DeviceOutput)) {
                return false;
            }
            TestNonBlockingDevice cli_NonBlockingDevice(false);
            if (! Password(CLI_CallInfo, cli_NonBlockingDevice, cli_NonBlockingDevice, B_DisplayStars, STR_Input, UI_MinLength, UI_MaxLength, B_ExpectedResult, STR_StringOutput, STR_DeviceOutput)) {
                return false;
            }
            return true;
        }
    private:
        static const bool Password(
                const cli::CallInfo& CLI_CallInfo, cli::IODevice& CLI_TestDevice, TestDeviceData& CLI_TestDeviceData,
                const bool B_DisplayStars,
                const char* const STR_Input, const unsigned int UI_MinLength, const unsigned int UI_MaxLength,
                const bool B_ExpectedResult, const char* const STR_StringOutput, const char* const STR_DeviceOutput)
        {
            cli::ui::Password cli_Password(B_DisplayStars, UI_MinLength, UI_MaxLength);
            cli_Password.SetStream(cli::WELCOME_STREAM, cli::OutputDevice::GetNullDevice());
            cli_Password.SetStream(cli::PROMPT_STREAM, cli::OutputDevice::GetNullDevice());
            const bool b_Result = (
                CheckExecuteContext(CLI_CallInfo, cli_Password, CLI_TestDevice, STR_Input)
                && cli_Password.GetbExecResult()
            );

            if (b_Result != B_ExpectedResult) {
                TestError(CLI_CallInfo) << "UI::GetPassword() returned " << b_Result << " (" << B_ExpectedResult << " was expected)" << cli::endl;
                return false;
            }
            if (strcmp((const char* const) cli_Password.GetPassword(), STR_StringOutput) != 0) {
                TestError(CLI_CallInfo) << "String output '" << cli_Password.GetPassword() << "' does not match '" << STR_StringOutput << "'" << cli::endl;
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

    // Test basic character inputs (no star).
    if (! Test::Password(__CALL_INFO__, false, "012345\n", 0, 10, true, "012345", Out().endl())) return false;
    // Test basic character inputs (star).
    if (! Test::Password(__CALL_INFO__, true, "012345\n", 0, 10, true, "012345", Out().txt("******").endl())) return false;
    // Test maximum password length.
    if (! Test::Password(__CALL_INFO__, true, "0123456789\n", 0, 10, true, "0123456789", Out().txt("**********").endl())) return false;
    if (! Test::Password(__CALL_INFO__, true, "0123456789a\n", 0, 10, true, "0123456789", Out().txt("**********").beep().endl())) return false;
    if (! Test::Password(__CALL_INFO__, true, "012345\b6789a\n", 0, 10, true, "012346789a", Out().txt("******").bsp(1).txt("*****").endl())) return false;
    if (! Test::Password(__CALL_INFO__, true, "\b0123456789a\n", 0, 10, true, "0123456789", Out().beep().txt("**********").beep().endl())) return false;
    // Test minimum password length.
    if (! Test::Password(__CALL_INFO__, true, "0\n", 5, 10, false, "", Out().txt("*").beep().bsp(1).endl())) return false;

    // Test LEFT/RIGHT moves the cursor in the line (insert mode).
    if (! Test::Password(__CALL_INFO__, true, "abcdef%lx%r\n", 0, 10, true, "abcdexf", Out().txt("******").left(1).txt("**\b").right("*").endl())) return false;
    // Test LEFT/RIGHT moves the cursor in the line (replace mode).
    if (! Test::Password(__CALL_INFO__, true, "abcdef%i%lx%r\n", 0, 10, true, "abcdex", Out().txt("******").left(1).txt("*").beep().endl())) return false;
    // Test UP/DOWN/PUP/PDOWN does nothing.
    if (! Test::Password(__CALL_INFO__, true, "abcdef%u%d\n", 0, 10, true, "abcdef", Out().txt("******").endl())) return false;
    if (! Test::Password(__CALL_INFO__, true, "abcdef%U%D\n", 0, 10, true, "abcdef", Out().txt("******").endl())) return false;
    // Test HOME/END keys.
    if (! Test::Password(__CALL_INFO__, true, "abcdef%H%E\n", 0, 10, true, "abcdef", Out().txt("******").left(6).right("******").endl())) return false;

    // Test ESCAPE breaks the current input.
    if (! Test::Password(__CALL_INFO__, true, "abcdef\b\b%]", 0, 10, false, "", Out().txt("******").bsp(1).bsp(1).bsp(4).endl())) return false;
    // Test CTRL+C breaks the current input.
    if (! Test::Password(__CALL_INFO__, true, "abcdef\b\b%!", 0, 10, false, "", Out().txt("******").bsp(1).bsp(1).bsp(4).endl())) return false;

    return true;
}
