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

#include "cli/ui_choice.h"

#include "test_error.h"
#include "test_device.h"
#include "test_exec.h"


// cli::ui::Choice unit-tests.
const bool CheckChoiceQuestion(void)
{
    class Test {
    public:
        static const bool Choice(
                const cli::CallInfo& CLI_CallInfo,
                const cli::tk::Queue<cli::ResourceString>& TK_Choices, const int I_Default, const char* const STR_InputKeys, const cli::ResourceString::LANG E_Lang,
                const bool B_ExpectedResult, const int I_ChoiceOutput, const char* const STR_DeviceOutput)
        {
            TestBlockingDevice cli_BlockingDevice(false);
            if (! Choice(CLI_CallInfo, cli_BlockingDevice, cli_BlockingDevice, TK_Choices, I_Default, STR_InputKeys, E_Lang, B_ExpectedResult, I_ChoiceOutput, STR_DeviceOutput)) {
                return false;
            }
            TestNonBlockingDevice cli_NonBlockingDevice(false);
            if (! Choice(CLI_CallInfo, cli_NonBlockingDevice, cli_NonBlockingDevice, TK_Choices, I_Default, STR_InputKeys, E_Lang, B_ExpectedResult, I_ChoiceOutput, STR_DeviceOutput)) {
                return false;
            }
            return true;
        }
    public:
        static const bool Choice(
                const cli::CallInfo& CLI_CallInfo, cli::IODevice& CLI_TestDevice, TestDeviceData& CLI_TestDeviceData,
                const cli::tk::Queue<cli::ResourceString>& TK_Choices, const int I_Default, const char* const STR_InputKeys, const cli::ResourceString::LANG E_Lang,
                const bool B_ExpectedResult, const int I_ChoiceOutput, const char* const STR_DeviceOutput)
        {
            cli::ui::Choice cli_Choice(I_Default, TK_Choices);
            cli_Choice.SetStream(cli::WELCOME_STREAM, cli::OutputDevice::GetNullDevice());
            cli_Choice.SetStream(cli::PROMPT_STREAM, cli::OutputDevice::GetNullDevice());
            cli_Choice.SetLang(E_Lang);
            const bool b_Result = (
                CheckExecuteContext(CLI_CallInfo, cli_Choice, CLI_TestDevice, STR_InputKeys)
                && cli_Choice.GetbExecResult()
            );
            if (b_Result != B_ExpectedResult) {
                TestError(CLI_CallInfo) << "UI::ChoiceQuestion() returned " << b_Result << " (" << B_ExpectedResult << " was expected)" << cli::endl;
                return false;
            }
            if (cli_Choice.GetChoice() != I_ChoiceOutput) {
                TestError(CLI_CallInfo) << "Choice output " << cli_Choice.GetChoice() << " does not match " << I_ChoiceOutput << cli::endl;
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

    // Build test data.
    static const cli::ResourceString::LANG EN = cli::ResourceString::LANG_EN;
    static const cli::ResourceString::LANG FR = cli::ResourceString::LANG_FR;

    cli::tk::Queue<cli::ResourceString> tk_Empty(10);
    cli::tk::Queue<cli::ResourceString> tk_Choices1(10);
    tk_Choices1.AddTail(cli::ResourceString().SetString(EN, "abcdef").SetString(FR, "abcdef"));
    tk_Choices1.AddTail(cli::ResourceString().SetString(EN, "12345").SetString(FR, "12345"));

    // Test invalid input data
    if (! Test::Choice(__CALL_INFO__, tk_Empty, 0, "\n", EN, false, -1, Out().beep().endl())) return false;
    if (! Test::Choice(__CALL_INFO__, tk_Choices1, 2, "\n", EN, false, -1, Out().beep().endl())) return false;

    // Test ENTER validates default choice.
    if (! Test::Choice(__CALL_INFO__, tk_Choices1, 0, "\n", EN, true, 0, Out().txt("abcdef").bsp(6).txt("abcdef").endl())) return false;
    if (! Test::Choice(__CALL_INFO__, tk_Choices1, 1, "\n", EN, true, 1, Out().txt("12345").bsp(5).txt("12345").endl())) return false;

    // Test UP/DOWN navigates within the choice list.
    if (! Test::Choice(__CALL_INFO__, tk_Choices1, 0, "%u\n", EN, true, 0, Out().txt("abcdef").beep().bsp(6).txt("abcdef").bsp(6).txt("abcdef").endl())) return false;
    if (! Test::Choice(__CALL_INFO__, tk_Choices1, 0, "%d\n", EN, true, 1, Out().txt("abcdef").bsp(6).txt("12345").bsp(5).txt("12345").endl())) return false;
    if (! Test::Choice(__CALL_INFO__, tk_Choices1, 0, "%d%d\n", EN, true, 1, Out().txt("abcdef").bsp(6).txt("12345").beep().bsp(5).txt("12345").bsp(5).txt("12345").endl())) return false;
    if (! Test::Choice(__CALL_INFO__, tk_Choices1, 1, "%u%u\n", EN, true, 0, Out().txt("12345").bsp(5).txt("abcdef").beep().bsp(6).txt("abcdef").bsp(6).txt("abcdef").endl())) return false;

    // Test PUP/PDOWN navigates within the choice list.
    if (! Test::Choice(__CALL_INFO__, tk_Choices1, 0, "%U\n", EN, true, 0, Out().txt("abcdef").beep().bsp(6).txt("abcdef").bsp(6).txt("abcdef").endl())) return false;
    if (! Test::Choice(__CALL_INFO__, tk_Choices1, 0, "%D\n", EN, true, 1, Out().txt("abcdef").bsp(6).txt("12345").bsp(5).txt("12345").endl())) return false;
    if (! Test::Choice(__CALL_INFO__, tk_Choices1, 0, "%D%D\n", EN, true, 1, Out().txt("abcdef").bsp(6).txt("12345").beep().bsp(5).txt("12345").bsp(5).txt("12345").endl())) return false;
    if (! Test::Choice(__CALL_INFO__, tk_Choices1, 0, "%D%U%U\n", EN, true, 0, Out().txt("abcdef").bsp(6).txt("12345").bsp(5).txt("abcdef").beep().bsp(6).txt("abcdef").bsp(6).txt("abcdef").endl())) return false;

    // Test LEFT/RIGHT navigates within the edition line.
    if (! Test::Choice(__CALL_INFO__, tk_Choices1, 0, "%lx%r\n", EN, false, 0, Out().txt("abcdef").left(1).txt("xf\b").right("f").beep().bsp(7).txt("abcdef").endl())) return false;
    if (! Test::Choice(__CALL_INFO__, tk_Choices1, 0, "%i%lx%r\n", EN, false, 0, Out().txt("abcdef").left(1).txt("x").beep().beep().bsp(6).txt("abcdef").endl())) return false;

    // Test HOME/END navigates within the edition line.
    if (! Test::Choice(__CALL_INFO__, tk_Choices1, 0, "%H\n", EN, true, 0, Out().txt("abcdef").left(6).del(6).txt("abcdef").endl())) return false;
    if (! Test::Choice(__CALL_INFO__, tk_Choices1, 0, "%H%E\n", EN, true, 0, Out().txt("abcdef").left(6).right("abcdef").bsp(6).txt("abcdef").endl())) return false;

    // Test approximate text being matched with choices.
    cli::tk::Queue<cli::ResourceString> tk_Approx(4);
    tk_Approx.AddTail(cli::ResourceString().SetString(EN, "a"));
    tk_Approx.AddTail(cli::ResourceString().SetString(EN, "ab"));
    tk_Approx.AddTail(cli::ResourceString().SetString(EN, "aB"));
    tk_Approx.AddTail(cli::ResourceString().SetString(EN, "abc"));
    tk_Approx.AddTail(cli::ResourceString().SetString(EN, "de"));
    tk_Approx.AddTail(cli::ResourceString().SetString(EN, "dE"));
    tk_Approx.AddTail(cli::ResourceString().SetString(EN, "F"));
    tk_Approx.AddTail(cli::ResourceString().SetString(EN, "fg"));
        // 'z' matches nothing.
    if (! Test::Choice(__CALL_INFO__, tk_Approx, 0, "z\n", EN, false, 0, Out().txt("a").bsp(1).txt("z").beep().bsp(1).txt("a").endl())) return false;
        // 'a' has got an exact choice matching, even though there are other choices that starts with 'a'.
    if (! Test::Choice(__CALL_INFO__, tk_Approx, 0, "a\n", EN, true, 0, Out().txt("a").bsp(1).txt("a").bsp(1).txt("a").endl())) return false;
        // 'A' has no exact choice matching, and several strings start with 'a' => ambiguous.
    if (! Test::Choice(__CALL_INFO__, tk_Approx, 0, "A\n", EN, false, 0, Out().txt("a").bsp(1).txt("A").beep().bsp(1).txt("a").endl())) return false;
        // 'ab' has got an exact choice matching, even there is another choice 'aB' matching unless a capital letter.
    if (! Test::Choice(__CALL_INFO__, tk_Approx, 0, "ab\n", EN, true, 1, Out().txt("a").bsp(1).txt("ab").bsp(2).txt("ab").endl())) return false;
        // 'aB' has got an exact choice matching, even there is another choice 'ab' matching unless a capital letter.
    if (! Test::Choice(__CALL_INFO__, tk_Approx, 0, "aB\n", EN, true, 2, Out().txt("a").bsp(1).txt("aB").bsp(2).txt("aB").endl())) return false;
        // 'AB' has no exact choice matching, one starting like 'ab' and two others matching unless capital style => ambiguous.
    if (! Test::Choice(__CALL_INFO__, tk_Approx, 0, "AB\n", EN, false, 0, Out().txt("a").bsp(1).txt("AB").beep().bsp(2).txt("a").endl())) return false;
        // 'ABC' has got only one approximate choice unless capital style.
    if (! Test::Choice(__CALL_INFO__, tk_Approx, 0, "ABC\n", EN, true, 3, Out().txt("a").bsp(1).txt("ABC").bsp(3).txt("abc").endl())) return false;
        // 'd' has got two matching choices starting with 'd' => ambiguous.
    if (! Test::Choice(__CALL_INFO__, tk_Approx, 0, "d\n", EN, false, 0, Out().txt("a").bsp(1).txt("d").beep().bsp(1).txt("a").endl())) return false;
        // 'f' has got a matching choice unless capital style, and a choice starting with 'f' => ambiguous.
    if (! Test::Choice(__CALL_INFO__, tk_Approx, 0, "f\n", EN, false, 0, Out().txt("a").bsp(1).txt("f").beep().bsp(1).txt("a").endl())) return false;

    // Test language behaviour.
    cli::tk::Queue<cli::ResourceString> tk_Lang(20);
    tk_Lang.AddTail(cli::ResourceString().SetString(EN, "choice#1").SetString(FR, "choix 1"));
    tk_Lang.AddTail(cli::ResourceString().SetString(EN, "choice#2").SetString(FR, "choix 2"));
    tk_Lang.AddTail(cli::ResourceString().SetString(EN, "choice#3").SetString(FR, "choix 3"));
    tk_Lang.AddTail(cli::ResourceString().SetString(EN, "choice#4").SetString(FR, "choix 4"));
    tk_Lang.AddTail(cli::ResourceString().SetString(EN, "choice#5").SetString(FR, "choix 5"));
    tk_Lang.AddTail(cli::ResourceString().SetString(EN, "choice#6").SetString(FR, "choix 6"));
    tk_Lang.AddTail(cli::ResourceString().SetString(EN, "choice#7").SetString(FR, "choix 7"));
    tk_Lang.AddTail(cli::ResourceString().SetString(EN, "choice#8").SetString(FR, "choix 8"));
    tk_Lang.AddTail(cli::ResourceString().SetString(EN, "choice#9").SetString(FR, "choix 9"));
    tk_Lang.AddTail(cli::ResourceString().SetString(EN, "choice#10").SetString(FR, "choix 10"));
    tk_Lang.AddTail(cli::ResourceString().SetString(EN, "choice#11").SetString(FR, "choix 11"));
    tk_Lang.AddTail(cli::ResourceString().SetString(EN, "choice#12").SetString(FR, "choix 12"));
    tk_Lang.AddTail(cli::ResourceString().SetString(EN, "choice#13").SetString(FR, "choix 13"));
    tk_Lang.AddTail(cli::ResourceString().SetString(EN, "choice#14").SetString(FR, "choix 14"));
    tk_Lang.AddTail(cli::ResourceString().SetString(EN, "choice#15").SetString(FR, "choix 15"));
    tk_Lang.AddTail(cli::ResourceString().SetString(EN, "choice#16").SetString(FR, "choix 16"));
    tk_Lang.AddTail(cli::ResourceString().SetString(EN, "choice#17").SetString(FR, "choix 17"));
    tk_Lang.AddTail(cli::ResourceString().SetString(EN, "choice#18").SetString(FR, "choix 18"));
    tk_Lang.AddTail(cli::ResourceString().SetString(EN, "choice#19").SetString(FR, "choix 19"));
    tk_Lang.AddTail(cli::ResourceString().SetString(EN, "choice#20").SetString(FR, "choix 20"));

    // Test language + PU/PDOWN increments of 10%
    // Test PUP/PDOWN navigates by increments of 10% within the choice list (in English).
    if (! Test::Choice(__CALL_INFO__, tk_Lang, 0, "%D%D\n", EN, true, 4, Out().txt("choice#1").bsp(8).txt("choice#3").bsp(8).txt("choice#5").bsp(8).txt("choice#5").endl())) return false;
    // Test PUP/PDOWN navigates by increments of 10% within the choice list (in French).
    if (! Test::Choice(__CALL_INFO__, tk_Lang, 19, "%u%D\n", FR, true, 19, Out().txt("choix 20").bsp(8).txt("choix 19").bsp(8).txt("choix 20").bsp(8).txt("choix 20").endl())) return false;

    return true;
}
