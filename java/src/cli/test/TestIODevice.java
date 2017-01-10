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

package cli.test;


public class TestIODevice {

    public static void main(String J_Args[]) {
        cli.Cli cli_Cli = new TestTools.Cli();
        cli.Shell cli_Shell = new cli.Shell(cli_Cli);

        // Outputs
        cli.StringDevice cli_Err = new cli.StringDevice();
        cli_Shell.setStream(cli.Shell.WELCOME_STREAM, new cli.OutputFileDevice("TestIODevice.welcome.log"));
        cli_Shell.setStream(cli.Shell.OUTPUT_STREAM, new cli.OutputFileDevice("TestIODevice.output.log"));
        cli_Shell.setStream(cli.Shell.PROMPT_STREAM, cli.OutputDevice.getNullDevice());
        cli_Shell.setStream(cli.Shell.ECHO_STREAM, new cli.OutputFileDevice("TestIODevice.echo.log"));
        cli_Shell.setStream(cli.Shell.ERROR_STREAM, cli_Err);

        // Inputs
        cli.IOMux cli_IOMux = new cli.IOMux();
        cli_IOMux.addDevice(new cli.SingleCommand("help", cli.OutputDevice.getNullDevice()));
        cli_IOMux.addDevice(new cli.SingleCommand("error", cli.OutputDevice.getNullDevice()));
        cli_IOMux.addDevice(new cli.InputFileDevice("TestIODevice.input.cli", cli.OutputDevice.getNullDevice()));
        if (! cli_Shell.run(cli_IOMux)) {
            TestTools.error().put("Shell.run() failed").endl();
            System.exit(-1);
        }

        String j_Expected = "Syntax error next to 'error'\n";
        if (! cli_Err.getString().equals(j_Expected)) {
            TestTools.error().put("Error StringDevice does not contain expected value:").endl()
                .put("    contained = " + cli_Err.getString()).endl()
                .put("    expected = " + j_Expected).endl();
            System.exit(-1);
        }
        cli_Err.reset();
        if (cli_Err.getString().length() != 0) {
            TestTools.error().put("StringDevice is not empty after reset()").endl()
                .put("    contained = " + cli_Err.getString()).endl();
            System.exit(-1);
        }

        // Bug(minor)! java.lang.UnsatisfiedLinkError: __Native__getScreenInfo at cli.OutputDevice.__Native__getScreenInfo(Native Method)
        try {
            cli.OutputDevice.ScreenInfo cli_ScreenInfo = cli.OutputDevice.getStdOut().getScreenInfo();
            if (    (cli_ScreenInfo.getWidth() != cli.OutputDevice.ScreenInfo.UNKNOWN)
                    || (cli_ScreenInfo.getSafeWidth() != cli.OutputDevice.ScreenInfo.DEFAULT_WIDTH)) {
                TestTools.error().put("cli.Ouptutdevice.ScreenInfo: Invalid data").endl();
                System.exit(-1);
            }
        } catch (java.lang.UnsatisfiedLinkError j_UnsatisfiedLinkError) {
            TestTools.error().put("cli.Ouptutdevice.getScreenInfo() not implemented in native").endl();
            System.exit(-1);
        }

        runFinalizers();
    }

    @SuppressWarnings("deprecation")
    private static void runFinalizers() {
        // Generates a warning but whatever!
        // The aim here is to check that executing all finalizers does not cause a crash.
        System.runFinalizersOnExit(true);
        // Note: Exception are not thrown anymore when finalizing objects on exit.
    }
}
