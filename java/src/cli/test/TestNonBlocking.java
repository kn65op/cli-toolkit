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


public class TestNonBlocking {

    public static void main(String J_Args[]) {
        cli.Cli cli_Cli = new TestTools.Cli();
        cli.Shell cli_Shell = new cli.Shell(cli_Cli);
        TestTools.Device cli_Device = new TestTools.Device(cli.OutputDevice.getNullDevice());
        cli_Shell.run(cli_Device);

        if (! cli_Shell.isRunning()) {
            cli_Device.put("ERROR: cli_Shell should still be running").endl();
            System.exit(-1);
        }

        cli_Device.onKey(cli.OutputDevice.KEY_q);
        cli_Device.onKey(cli.OutputDevice.KEY_u);
        cli_Device.onKey(cli.OutputDevice.KEY_i);
        cli_Device.onKey(cli.OutputDevice.KEY_t);
        cli_Device.onKey(cli.OutputDevice.ENTER);

        if (cli_Shell.isRunning()) {
            cli_Device.put("ERROR: cli_Shell should not be running anymore").endl();
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
