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


#ifndef __CLI_TEST_CLI_H__
#define __CLI_TEST_CLI_H__

#include "cli/assert.h"
#include "cli/cli.h"

#include "test_error.h"


// Forward declarations.
static cli::Cli& GetTestCli(void);
static const cli::Cli& GetXmlResCli(void);


class TestCli : public cli::Cli {
public:
    explicit TestCli(void) : cli::Cli("TestCli", cli::Help()) {}
    explicit TestCli(const char* const STR_Name, const cli::Help& CLI_Help) : Cli(STR_Name, CLI_Help) {}
private:
};


static cli::Cli& GetTestCli(void)
{
    static TestCli cli_TestCli;
    return cli_TestCli;
}

static const cli::Cli& GetXmlResCli(void)
{
    // Retrieve the CLI
    cli::Cli::List cli_List(10);
    const int i_Clis = cli::Cli::FindFromName(cli_List, ".*");
    if (i_Clis > 0)
    {
        CLI_ASSERT(! cli_List.IsEmpty());
        if (const cli::Cli* const pcli_Cli = cli_List.GetHead())
        {
            if (i_Clis > 1)
            {
                TestWarning(__CALL_INFO__) << "Several CLIs found. Executing only the first one." << cli::endl;
            }
            return *pcli_Cli;
        }
    }

    TestError(__CALL_INFO__) << "No CLI found." << cli::endl;
    return GetTestCli();
}

#include "test_no_warning.h"
BEGIN_NO_WARNING(test_cli)
    GetTestCli();
    GetXmlResCli();
END_NO_WARNING(test_cli)

#endif // __CLI_TEST_CLI_H__
