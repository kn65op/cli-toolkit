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

#include "cli/traces.h"
#include "cli/shell.h"
#include "cli/keyword.h"
#include "cli/endl.h"
#include "cli/command_line.h"
#include "cli/ui_line.h"

#include "test_cli.h"
#include "test_device.h"


static const bool Test_Shell_Abort(void);
static const bool Test_Shell_UI(void);
static const bool Test_Shell_UI_NonBlocking(void);
static const bool Test_Pwd_Shell(void);

int main(void)
{
    bool b_Res = true;
    b_Res = b_Res && Test_Shell_Abort();
    b_Res = b_Res && Test_Shell_UI();
    b_Res = b_Res && Test_Shell_UI_NonBlocking();
    b_Res = b_Res && Test_Pwd_Shell();

    return (b_Res ? 0 : -1);
}



// Shell stand alone.
// Start execution, then destroy directly.
static const bool Test_Shell_Abort(void)
{
    TestNonBlockingDevice cli_Device(false); // Declare at first for consistency on errors.
    cli::Shell cli_Shell(GetXmlResCli());
    cli_Shell.Run(cli_Device);
    return true;
}

// Non blocking I/O device.
// Top execution context: Shell
// Child execution context: ui::Line
static const bool Test_Shell_UI(void)
{
    TestNonBlockingDevice cli_Device(false); // Declare at first for consistency on errors.

    cli::Shell cli_Shell(GetXmlResCli());
    if (cli_Shell.IsRunning()) { TestError(__CALL_INFO__) << "Shell should not be running yet." << cli::endl; return false; }

    cli_Shell.Run(cli_Device);
    if (! cli_Shell.IsRunning()) { TestError(__CALL_INFO__) << "Shell should be running now." << cli::endl; return false; }

    cli_Device.ClearOutput();
    cli_Device.PushKeys("help\n");
    const cli::tk::String tk_Echo = cli_Device.GetOutput();
    if (tk_Echo.SubString(0, 4) != "help") { TestError(__CALL_INFO__) << "Output should start with echo 'help' ('." << tk_Echo << "' instead)." << cli::endl; return false; }
    if (! cli_Shell.IsRunning()) { TestError(__CALL_INFO__) << "Shell should still be running." << cli::endl; return false; }

    cli::ui::Line cli_Line(cli_Shell, cli::tk::String(256, ""), 0, 256);
    if (! cli_Shell.IsRunning()) { TestError(__CALL_INFO__) << "Shell should still be running." << cli::endl; return false; }
    if (cli_Line.IsRunning()) { TestError(__CALL_INFO__) << "Line should not be running yet." << cli::endl; return false; }

    if (& cli_Shell.GetContextManager() != & cli_Line.GetContextManager()) { TestError(__CALL_INFO__) << "Shell and Line should have the same execution context manager." << cli::endl; return false; }

    cli_Line.Run();
    if (! cli_Shell.IsRunning()) { TestError(__CALL_INFO__) << "Shell should still be running." << cli::endl; return false; }
    if (! cli_Line.IsRunning()) { TestError(__CALL_INFO__) << "Line should be running now." << cli::endl; return false; }

    cli_Device.PushKeys("foo");
    if (! cli_Shell.IsRunning()) { TestError(__CALL_INFO__) << "Shell should still be running." << cli::endl; return false; }
    if (! cli_Line.IsRunning()) { TestError(__CALL_INFO__) << "Line should still be running." << cli::endl; return false; }

    cli_Device.PushKeys("\n");
    if (! cli_Shell.IsRunning()) { TestError(__CALL_INFO__) << "Shell should still be running." << cli::endl; return false; }
    if (cli_Line.IsRunning()) { TestError(__CALL_INFO__) << "Line should not be running anymore." << cli::endl; return false; }
    if (! cli_Line.GetbExecResult()) { TestError(__CALL_INFO__) << "Line should have terminated successfully." << cli::endl; return false; }
    if (cli_Line.GetLine() != "foo") { TestError(__CALL_INFO__) << "Line value should be 'foo'." << cli::endl; return false; }

    cli_Device.PushKeys("exit\n");
    if (cli_Shell.IsRunning()) { TestError(__CALL_INFO__) << "Shell should not be running anymore." << cli::endl; return false; }
    if (cli_Line.IsRunning()) { TestError(__CALL_INFO__) << "Line should still not be running." << cli::endl; return false; }

    return true;
}

// Complementary test to the previous in the following configuration :
//  - Shell + UI
//  - Non blocking device
//  - UI executed from a command line of the shell
// Let's check the prompt of the shell does not interleave with the UI execution, and is printed out on UI termination only.
static const bool Test_Shell_UI_NonBlocking(void)
{
    class _TestCli : public cli::Cli, public cli::ExecutionResult {
        private: mutable cli::ui::Line* m_pcliLine;
        public: explicit _TestCli(void) : cli::Cli("TestCli", cli::Help()) {
            Populate();
            m_pcliLine = NULL;
        }
        public: virtual ~_TestCli(void) {
            if (m_pcliLine != NULL) { delete m_pcliLine; m_pcliLine = NULL; }
        }
        public: void Populate(void) {
            if (cli::Keyword* const pcli_UI = dynamic_cast<cli::Keyword*>(& AddElement(new cli::Keyword("ui-line", cli::Help())))) {
                pcli_UI->AddElement(new cli::Endl(cli::Help()));
            }
        }
        public: virtual const bool Execute(const cli::CommandLine& CLI_CmdLine) const {
            cli::CommandLineIterator cli_Elements(CLI_CmdLine);
            if (! cli_Elements.StepIt()) return false;
            if (const cli::Keyword* const pcli_Keyword = dynamic_cast<const cli::Keyword*>(*cli_Elements)) {
            if (pcli_Keyword->GetKeyword() == "ui-line") {
                if (! cli_Elements.StepIt()) return false;
                if (dynamic_cast<const cli::Endl*>(*cli_Elements) != NULL) {
                    GetShell().GetStream(cli::PROMPT_STREAM) << "Enter line: ";
                    if (m_pcliLine = new cli::ui::Line(GetShell(), cli::tk::String(10, ""), 0, 20)) {
                        const_cast<_TestCli*>(this)->WatchResult(*m_pcliLine);
                        m_pcliLine->Run();
                    }
                    return true;
                }
            }}
            return false;
        }
        public: virtual void OnResult(const cli::ExecutionContext& CLI_Context) {
            if (& CLI_Context == m_pcliLine) {
                if (m_pcliLine != NULL) { delete m_pcliLine; m_pcliLine = NULL; }
            }
        }
    };

    TestNonBlockingDevice cli_Device(false); // Declare at first for consistency on errors.
    _TestCli cli_Cli;
    cli::Shell cli_Shell(cli_Cli);
    cli_Shell.Run(cli_Device);
    cli_Device.ClearOutput();

    cli_Device.PushKeys("ui-line\n");
    const cli::tk::String tk_Echo1 = cli_Device.GetOutput();
    if (tk_Echo1 != "ui-line\nEnter line: ") { TestError(__CALL_INFO__) << "Echo '" << tk_Echo1 << "' does not match 'ui-line\nEnter line: '." << cli::endl; return false; }

    cli_Device.PushKeys("test line\n");
    const cli::tk::String tk_Echo2 = cli_Device.GetOutput();
    if (tk_Echo2 != "test line\nTestCli>") { TestError(__CALL_INFO__) << "Echo '" << tk_Echo2 << "' does not match 'test line\nTestCli>'." << cli::endl; return false; }

    return true;
}

// Non blocking I/O device.
// Top execution context: TestExecContext
// Child execution context1: ui::Password
// Child execution context2: Shell
#include "pwd_shell.h"
static const bool Test_Pwd_Shell(void)
{
    TestNonBlockingDevice cli_Device(false); // Declare at first for consistency on errors.

// First execution: login successfully, then prompt for the user to enter a line, then abort everything.
    PwdShellContext cli_PwdShell(GetXmlResCli(), "hello");
    if (cli_PwdShell.IsRunning()) { TestError(__CALL_INFO__) << "PwdShell should not be running." << cli::endl; return false; }
    if (& cli_PwdShell.GetPwdControl().GetContextManager() != & cli_PwdShell.GetContextManager()) { TestError(__CALL_INFO__) << "PwdShell.Pwd and PwdShell's execution context manager should be the same." << cli::endl; return false; }
    if (& cli_PwdShell.GetShell().GetContextManager() != & cli_PwdShell.GetContextManager()) { TestError(__CALL_INFO__) << "PwdShell.Shell and PwdShell's execution context manager should be the same." << cli::endl; return false; }

    cli_PwdShell.Run(cli_Device);
    if (! cli_PwdShell.IsRunning()) { TestError(__CALL_INFO__) << "PwdShell should be running." << cli::endl; return false; }
    if (! cli_PwdShell.GetPwdControl().IsRunning()) { TestError(__CALL_INFO__) << "PwdShell.Pwd should be running." << cli::endl; return false; }
    if (cli_PwdShell.GetShell().IsRunning()) { TestError(__CALL_INFO__) << "PwdShell.Shell should not be running." << cli::endl; return false; }

    cli_Device.PushKeys("hello\n");
    if (! cli_PwdShell.IsRunning()) { TestError(__CALL_INFO__) << "PwdShell should be running." << cli::endl; return false; }
    if (cli_PwdShell.GetPwdControl().IsRunning()) { TestError(__CALL_INFO__) << "PwdShell.Pwd should not be running." << cli::endl; return false; }
    if (! cli_PwdShell.GetShell().IsRunning()) { TestError(__CALL_INFO__) << "PwdShell.Shell should be running." << cli::endl; return false; }

    do { // Create the Line object in a block in order to check consistency after abortion.
        cli::ui::Line cli_Line(cli_PwdShell, cli::tk::String(256, ""), 0, 256);
        cli_Line.Run();
        if (! cli_PwdShell.IsRunning()) { TestError(__CALL_INFO__) << "PwdShell should be running." << cli::endl; return false; }
        if (cli_PwdShell.GetPwdControl().IsRunning()) { TestError(__CALL_INFO__) << "PwdShell.Pwd should not be running." << cli::endl; return false; }
        if (! cli_PwdShell.GetShell().IsRunning()) { TestError(__CALL_INFO__) << "PwdShell.Shell should be still running." << cli::endl; return false; }
        if (! cli_Line.IsRunning()) { TestError(__CALL_INFO__) << "Line should be running as well." << cli::endl; return false; }

        // Abort everthing!
        cli_Device.PushKeys("abc");
        cli_PwdShell.StopAllExecutions();
        if (cli_PwdShell.IsRunning()) { TestError(__CALL_INFO__) << "PwdShell should not be running." << cli::endl; return false; }
        if (cli_PwdShell.GetPwdControl().IsRunning()) { TestError(__CALL_INFO__) << "PwdShell.Pwd should not be running." << cli::endl; return false; }
        if (cli_PwdShell.GetShell().IsRunning()) { TestError(__CALL_INFO__) << "PwdShell.Shell should not be running." << cli::endl; return false; }
    } while(0);

// Second execution: failure at login
    cli_Device.ClearOutput();
    cli_PwdShell.Run(cli_Device);
    if (! cli_Device.CheckOutput(__CALL_INFO__, Out()
            .txt("Enter password: ")))
        return false;
    if (! cli_PwdShell.IsRunning()) { TestError(__CALL_INFO__) << "PwdShell should be running." << cli::endl; return false; }
    if (! cli_PwdShell.GetPwdControl().IsRunning()) { TestError(__CALL_INFO__) << "PwdShell.Pwd should be running." << cli::endl; return false; }
    if (cli_PwdShell.GetShell().IsRunning()) { TestError(__CALL_INFO__) << "PwdShell.Shell should not be running." << cli::endl; return false; }

    cli_Device.PushKeys("bad1\n");
    if (! cli_Device.CheckOutput(__CALL_INFO__, Out()
            .txt("****").endl()
            .txt("Wrong password!").endl()
            .txt("Enter password: ")))
        return false;
    if (! cli_PwdShell.IsRunning()) { TestError(__CALL_INFO__) << "PwdShell should be running." << cli::endl; return false; }
    if (! cli_PwdShell.GetPwdControl().IsRunning()) { TestError(__CALL_INFO__) << "PwdShell.Pwd should be running." << cli::endl; return false; }
    if (cli_PwdShell.GetShell().IsRunning()) { TestError(__CALL_INFO__) << "PwdShell.Shell should not be running." << cli::endl; return false; }

    cli_Device.PushKeys("bad2\n");
    if (! cli_Device.CheckOutput(__CALL_INFO__, Out()
            .txt("****").endl()
            .txt("Wrong password!").endl()
            .txt("Enter password: ")))
        return false;
    if (! cli_PwdShell.IsRunning()) { TestError(__CALL_INFO__) << "PwdShell should be running." << cli::endl; return false; }
    if (! cli_PwdShell.GetPwdControl().IsRunning()) { TestError(__CALL_INFO__) << "PwdShell.Pwd should be running." << cli::endl; return false; }
    if (cli_PwdShell.GetShell().IsRunning()) { TestError(__CALL_INFO__) << "PwdShell.Shell should not be running." << cli::endl; return false; }

    cli_Device.PushKeys("bad3\n");
    if (! cli_Device.CheckOutput(__CALL_INFO__, Out()
            .txt("****").endl()
            .txt("Wrong password!").endl()))
        return false;
    if (cli_PwdShell.IsRunning()) { TestError(__CALL_INFO__) << "PwdShell should not be running." << cli::endl; return false; }
    if (cli_PwdShell.GetPwdControl().IsRunning()) { TestError(__CALL_INFO__) << "PwdShell.Pwd should not be running." << cli::endl; return false; }
    if (cli_PwdShell.GetShell().IsRunning()) { TestError(__CALL_INFO__) << "PwdShell.Shell should not be running." << cli::endl; return false; }

    return true;
}
