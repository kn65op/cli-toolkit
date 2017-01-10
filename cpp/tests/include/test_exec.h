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


#ifndef __CLI_TEST_EXEC_H__
#define __CLI_TEST_EXEC_H__

#include "cli/debug.h"
#include "cli/assert.h"
#include "cli/ui_more.h"

#include "test_error.h"
#include "test_device.h"


class TestResult : public cli::ExecutionResult {
public:
    explicit TestResult(void) : cli::ExecutionResult(), m_pcliContext(NULL) {}
    virtual ~TestResult(void) {}
public:
    virtual void OnResult(const cli::ExecutionContext& CLI_Context) { m_pcliContext = & CLI_Context; }
public:
    const cli::ExecutionContext* const GetContext(void) const { return m_pcliContext; }
private:
    const cli::ExecutionContext* m_pcliContext;
};


static const bool CheckExecuteContext(
    const cli::CallInfo& CLI_CallInfo,
    cli::ExecutionContext& CLI_Context,
    cli::IODevice& CLI_TestDevice, const char* const STR_Input
) {
    if (TestBlockingDevice* const pcli_BlockingDevice = dynamic_cast<TestBlockingDevice*>(& CLI_TestDevice)) {
        pcli_BlockingDevice->SetInputString(STR_Input);
        TestResult cli_Result;
        cli_Result.WatchResult(CLI_Context);
        CLI_Context.Run(*pcli_BlockingDevice);
        return (cli_Result.GetContext() == & CLI_Context);
    }

    if (TestNonBlockingDevice* const pcli_NonBlockingDevice = dynamic_cast<TestNonBlockingDevice*>(& CLI_TestDevice)) {
        TestResult cli_Result;
        cli_Result.WatchResult(CLI_Context);
        CLI_Context.Run(*pcli_NonBlockingDevice);

        // First check execution is not done yet, since the call is non-blocking.
        if (cli_Result.GetContext() != NULL) {
            // Unless for ui::More objects, which might terminate right away if there is no text to display
            if (dynamic_cast<const cli::ui::More*>(& CLI_Context) != NULL) { return (cli_Result.GetContext() == & CLI_Context); }
            TestError(CLI_CallInfo) << "Non blocking execution: execution should not be done yet" << cli::endl; return false;
        }

        // Push keys.
        pcli_NonBlockingDevice->SetInputString(STR_Input);
        pcli_NonBlockingDevice->PushKeys();

        if (dynamic_cast<cli::ui::UI*>(& CLI_Context) != NULL) {
            // UI objects only: Eventually push a NULL_KEY for erroneous terminations.
            if (CLI_Context.IsRunning()) { pcli_NonBlockingDevice->OnKey(cli::NULL_KEY); }
        }

        // Check execution is done now.
        if (cli_Result.GetContext() != & CLI_Context) { TestError(CLI_CallInfo) << "Non blocking execution: execution should be done now" << cli::endl; return false; }

        return true;
    }

    CLI_ASSERT(false);
    return false;
}

#endif // __CLI_TEST_EXEC_H__
