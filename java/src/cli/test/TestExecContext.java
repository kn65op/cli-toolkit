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


public class TestExecContext {

    public static void main(String J_Args[]) {

        boolean b_TestShellAbort = true;
        boolean b_TestShellUI = true;
        boolean b_TestShellUINonBlocking = true;
        boolean b_TestPwdShell = true;
        if (J_Args.length > 0) {
            if (! J_Args[0].equals("ShellAbort")) { b_TestShellAbort = false; }
            if (! J_Args[0].equals("ShellUI")) { b_TestShellUI = false; }
            if (! J_Args[0].equals("ShellUINonBlocking")) { b_TestShellUINonBlocking = false; }
            if (! J_Args[0].equals("PwdShell")) { b_TestPwdShell = false; }
        }

        boolean b_Res = true;
        try {
            if (b_TestShellAbort) {
                if (! Test_Shell_Abort()) {
                    System.exit(-1);
                }
            }
            if (b_TestShellUI) {
                if (! Test_Shell_UI()) {
                    System.exit(-1);
                }
            }
            if (b_TestShellUINonBlocking) {
                if (! Test_Shell_UI_NonBlocking()) {
                    System.exit(-1);
                }
            }
            if (b_TestPwdShell) {
                if (! Test_Pwd_Shell()) {
                    System.exit(-1);
                }
            }
        } catch (Exception e) {
            error().printStackTrace(e);
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

    private static cli.OutputDevice.Interface error() {
        return TestTools.error();
    }

    private static void trace(String J_Trace) {
        TestTools.trace(J_Trace);
    }


    // Non blocking I/O device.
    // Top execution context: Shell
    // Shell is not terminated before returning.
    static boolean Test_Shell_Abort() {
        trace("=== Test_Shell_Abort ===");

        TestTools.Device cli_Device = new TestTools.Device(new cli.StringDevice());
        cli.Cli cli_Cli = new TestTools.Cli();
        cli.Shell cli_Shell = new cli.Shell(cli_Cli);
        cli_Shell.run(cli_Device);
        return true;
    }

    // Non blocking I/O device.
    // Top execution context: Shell
    // Child execution context: ui::Line
    private static boolean Test_Shell_UI() {
        trace("=== Test_Shell_UI ===");

        TestTools.Device cli_Device = new TestTools.Device(new cli.StringDevice());

        cli.Cli cli_Cli = new TestTools.Cli();
        cli.Shell cli_Shell = new cli.Shell(cli_Cli);
        if (cli_Shell.isRunning()) { error().put("Shell should not be running yet.").endl(); return false; }

        cli_Shell.run(cli_Device);
        if (! cli_Shell.isRunning()) { error().put("Shell should be running now.").endl(); return false; }

        cli_Device.clearOutput();
        cli_Device.pushKeys("help\n");
        final String j_Echo = cli_Device.getOutput();
        if (! j_Echo.substring(0, 4).equals("help")) { error().put("Output should start with echo 'help' ('.").put(j_Echo).put("' instead).").endl(); return false; }
        if (! cli_Shell.isRunning()) { error().put("Shell should still be running.").endl(); return false; }

        cli.ui.Line cli_Line = new cli.ui.Line(cli_Shell, "", 0, 256);
        if (! cli_Shell.isRunning()) { error().put("Shell should still be running.").endl(); return false; }
        if (cli_Line.isRunning()) { error().put("Line should not be running yet.").endl(); return false; }

        cli_Line.run();
        if (! cli_Shell.isRunning()) { error().put("Shell should still be running.").endl(); return false; }
        if (! cli_Line.isRunning()) { error().put("Line should be running now.").endl(); return false; }

        cli_Device.pushKeys("foo");
        if (! cli_Shell.isRunning()) { error().put("Shell should still be running.").endl(); return false; }
        if (! cli_Line.isRunning()) { error().put("Line should still be running.").endl(); return false; }

        cli_Device.pushKeys("\n");
        if (! cli_Shell.isRunning()) { error().put("Shell should still be running.").endl(); return false; }
        if (cli_Line.isRunning()) { error().put("Line should not be running anymore.").endl(); return false; }
        if (! cli_Line.getbExecResult()) { error().put("Line should have terminated successfully.").endl(); return false; }
        if (! cli_Line.getLine().equals("foo")) { error().put("Line value should be 'foo'.").endl(); return false; }

        cli_Device.pushKeys("exit\n");
        if (cli_Shell.isRunning()) { error().put("Shell should not be running anymore.").endl(); return false; }
        if (cli_Line.isRunning()) { error().put("Line should still not be running.").endl(); return false; }

        return true;
    }

    // Complementary test to the previous in the following configuration :
    //  - Shell + UI
    //  - Non blocking device
    //  - UI executed from a command line of the shell
    // Let's check the prompt of the shell does not interleave with the UI execution, and is printed out on UI termination only.
    private static boolean Test_Shell_UI_NonBlocking() {
        trace("=== Test_Shell_UI_NonBlocking ===");

        class _TestCli extends cli.Cli {
            public _TestCli() {
                super("TestCli", new cli.Help());
                populate();
            }
            public void populate() {
                cli.Keyword cli_UI = (cli.Keyword) addElement(new cli.Keyword("ui-line", new cli.Help()));
                if (cli_UI != null) {
                    cli_UI.addElement(new cli.Endl(new cli.Help()));
                }
            }
            public boolean execute(cli.CommandLine CLI_CmdLine) {
                try {
                    java.util.Iterator<cli.Element> cli_Elements = CLI_CmdLine.iterator();
                    cli.Element cli_Element = cli_Elements.next();
                    if ((cli_Element instanceof cli.Keyword) && ((cli.Keyword) cli_Element).getKeyword().equals("ui-line")) {
                        cli_Element = cli_Elements.next();
                        if (cli_Element instanceof cli.Endl) {
                            getShell().getStream(cli.ExecutionContext.PROMPT_STREAM).put("Enter line: ");
                            cli.ui.Line m_cliLine = new cli.ui.Line(getShell(), "", 0, 20);
                            m_cliLine.run();
                            return true;
                        }
                    }
                } catch (java.util.NoSuchElementException e1) {
                } catch (java.lang.Exception e) {
                    getErrorStream().printStackTrace(e);
                }
                return false;
            }
        };

        TestTools.Device cli_Device = new TestTools.Device(new cli.StringDevice());
        _TestCli cli_Cli = new _TestCli();
        cli.Shell cli_Shell = new cli.Shell(cli_Cli);
        cli_Shell.run(cli_Device);
        cli_Device.clearOutput();

        cli_Device.pushKeys("ui-line\n");
        String j_Echo1 = cli_Device.getOutput();
        if (! j_Echo1.equals("ui-line\nEnter line: ")) { error().put("Echo '").put(j_Echo1).put("' does not match 'ui-line\nEnter line: '.").endl(); return false; }

        cli_Device.pushKeys("test line\n");
        String j_Echo2 = cli_Device.getOutput();
        if (! j_Echo2.equals("test line\nTestCli>")) { error().put("Echo '").put(j_Echo2).put("' does not match 'test line\nTestCli>'.").endl(); return false; }

        return true;
    }

    // Non blocking I/O device.
    // Top execution context: TestExecContext
    // Child execution context1: ui::Password
    // Child execution context2: Shell
    private static boolean Test_Pwd_Shell() {
        trace("=== Test_Pwd_Shell ===");

        TestTools.Device cli_Device = new TestTools.Device(new cli.StringDevice());

    // First execution: login successfully, then prompt for the user to enter a line, then abort everything.
        trace("--- First execution");
        cli.Cli cli_Cli = new TestTools.Cli();
        PwdShellContext cli_PwdShell = new PwdShellContext(cli_Cli, "hello");
        if (cli_PwdShell.isRunning()) { error().put("PwdShell should not be running.").endl(); return false; }

        cli_PwdShell.run(cli_Device);
        if (! cli_PwdShell.isRunning()) { error().put("PwdShell should be running.").endl(); return false; }
        if (! cli_PwdShell.getPwdControl().isRunning()) { error().put("PwdShell.Pwd should be running.").endl(); return false; }
        if (cli_PwdShell.getShell().isRunning()) { error().put("PwdShell.Shell should not be running.").endl(); return false; }

        trace("--- Type 'hello' password");
        cli_Device.pushKeys("hello\n");
        if (! cli_PwdShell.isRunning()) { error().put("PwdShell should be running.").endl(); return false; }
        if (cli_PwdShell.getPwdControl().isRunning()) { error().put("PwdShell.Pwd should not be running.").endl(); return false; }
        if (! cli_PwdShell.getShell().isRunning()) { error().put("PwdShell.Shell should be running.").endl(); return false; }

        do { // Create the Line object in a block in order to check consistency after abortion.
            trace("--- Execute cli.Line");
            cli.ui.Line cli_Line = new cli.ui.Line(cli_PwdShell, "", 0, 256);
            cli_Line.run();
            if (! cli_PwdShell.isRunning()) { error().put("PwdShell should be running.").endl(); return false; }
            if (cli_PwdShell.getPwdControl().isRunning()) { error().put("PwdShell.Pwd should not be running.").endl(); return false; }
            if (! cli_PwdShell.getShell().isRunning()) { error().put("PwdShell.Shell should be still running.").endl(); return false; }
            if (! cli_Line.isRunning()) { error().put("Line should be running as well.").endl(); return false; }

            trace("--- type 'abc' and abort everything");
            // Abort everthing!
            cli_Device.pushKeys("abc");
            cli_PwdShell.stopAllExecutions();
            if (cli_PwdShell.isRunning()) { error().put("PwdShell should not be running.").endl(); return false; }
            if (cli_PwdShell.getPwdControl().isRunning()) { error().put("PwdShell.Pwd should not be running.").endl(); return false; }
            if (cli_PwdShell.getShell().isRunning()) { error().put("PwdShell.Shell should not be running.").endl(); return false; }
        } while(false);

    // Second execution: failure at login
        trace("--- Second execution");
        cli_Device.clearOutput();
        cli_PwdShell.run(cli_Device);
        if (! cli_Device.checkOutput(new Out()
                .txt("Enter password: ")))
            return false;
        if (! cli_PwdShell.isRunning()) { error().put("PwdShell should be running.").endl(); return false; }
        if (! cli_PwdShell.getPwdControl().isRunning()) { error().put("PwdShell.Pwd should be running.").endl(); return false; }
        if (cli_PwdShell.getShell().isRunning()) { error().put("PwdShell.Shell should not be running.").endl(); return false; }

        trace("--- Type 'bad1' password");
        cli_Device.pushKeys("bad1\n");
        if (! cli_Device.checkOutput(new Out()
                .txt("****").endl()
                .txt("Wrong password!").endl()
                .txt("Enter password: ")))
            return false;
        if (! cli_PwdShell.isRunning()) { error().put("PwdShell should be running.").endl(); return false; }
        if (! cli_PwdShell.getPwdControl().isRunning()) { error().put("PwdShell.Pwd should be running.").endl(); return false; }
        if (cli_PwdShell.getShell().isRunning()) { error().put("PwdShell.Shell should not be running.").endl(); return false; }

        trace("--- Type 'bad2' password");
        cli_Device.pushKeys("bad2\n");
        if (! cli_Device.checkOutput(new Out()
                .txt("****").endl()
                .txt("Wrong password!").endl()
                .txt("Enter password: ")))
            return false;
        if (! cli_PwdShell.isRunning()) { error().put("PwdShell should be running.").endl(); return false; }
        if (! cli_PwdShell.getPwdControl().isRunning()) { error().put("PwdShell.Pwd should be running.").endl(); return false; }
        if (cli_PwdShell.getShell().isRunning()) { error().put("PwdShell.Shell should not be running.").endl(); return false; }

        trace("--- Type 'bad3' password");
        cli_Device.pushKeys("bad3\n");
        if (! cli_Device.checkOutput(new Out()
                .txt("****").endl()
                .txt("Wrong password!").endl()))
            return false;
        if (cli_PwdShell.isRunning()) { error().put("PwdShell should not be running.").endl(); return false; }
        if (cli_PwdShell.getPwdControl().isRunning()) { error().put("PwdShell.Pwd should not be running.").endl(); return false; }
        if (cli_PwdShell.getShell().isRunning()) { error().put("PwdShell.Shell should not be running.").endl(); return false; }

        return true;
    }
}
