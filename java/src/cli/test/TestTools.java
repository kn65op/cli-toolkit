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


/** Does nothing but gather test tools. */
public abstract class TestTools {

    // Test arguments
    public static class Args {
        public boolean m_bNoExec;
        public int m_iTelnetPort;
        public cli.OutputDevice.Interface m_cliOut;
        public cli.IODevice.Interface m_cliIn;

        public Args() {
            m_bNoExec = false;
            m_iTelnetPort = -1;
            m_cliOut = null;
            m_cliIn = null;
        }

        public boolean parseArgs(String J_ProgramName, String[] ARJ_Args) {
            // Parse arguments.
            if (J_ProgramName == null) { return false; }
            String j_OutputFileName = null, j_InputFileName = null;
            for (int i=0; i<ARJ_Args.length; i++) {
                if (ARJ_Args[i] == null) {}
                else if (ARJ_Args[i].equals("--help")) { displayHelp(J_ProgramName); }
                else if (ARJ_Args[i].equals("--console")) { m_iTelnetPort = -1; j_OutputFileName = null; j_InputFileName = null; }
                else if (ARJ_Args[i].equals("--telnet")) { if ((++i < ARJ_Args.length) && (ARJ_Args[i] != null)) { m_iTelnetPort = new Integer(ARJ_Args[i]); } }
                else if (ARJ_Args[i].equals("--input")) { if ((++i < ARJ_Args.length) && (ARJ_Args[i] != null)) { j_InputFileName = ARJ_Args[i]; } }
                else if (ARJ_Args[i].equals("--output")) { if ((++i < ARJ_Args.length) && (ARJ_Args[i] != null)) { j_OutputFileName = ARJ_Args[i]; } }
                else { TestTools.error().put("Unknown option '" + ARJ_Args[i] + "'").endl(); displayHelp(J_ProgramName); return false; }
            }

            // Create devices.
            if (m_bNoExec) {}
            else if (m_iTelnetPort > 0) {}
            else if (j_InputFileName != null) {
                if (j_OutputFileName != null) {
                    m_cliOut = new cli.OutputFileDevice(j_OutputFileName);
                } else {
                    m_cliOut = new cli.Console();
                }

                cli.InputFileDevice cli_FileDevice = new cli.InputFileDevice(j_InputFileName, m_cliOut);
                cli_FileDevice.enableSpecialCharacters(true);
                m_cliIn = cli_FileDevice;
            } else {
                m_cliOut = m_cliIn = new cli.Console();
            }

            return true;
        }
        public boolean isNoExec() { return m_bNoExec; }
        public boolean isConsole() { return ((m_iTelnetPort < 0) && (m_cliIn != null)); }
        public boolean isTelnet() { return (m_iTelnetPort > 0); }
        public cli.IODevice.Interface getInput() { return m_cliIn; }
        cli.OutputDevice.Interface getOutput() { return m_cliOut; }

        public void execute(cli.Shell CLI_Shell) {
            try {
                if (m_bNoExec) {}
                else if (m_iTelnetPort > 0) {
                    class TestTelnetServer extends cli.TelnetServer {
                        private cli.Shell m_cliShell;
                        public TestTelnetServer(cli.Shell CLI_Shell, int UL_TcpPort) {
                            super(1, UL_TcpPort, cli.ResourceString.LANG_EN); // because the CLI is allocated once only, allow only one client.
                            m_cliShell = CLI_Shell;
                        }
                        public cli.ExecutionContext.Interface onNewConnection(cli.TelnetConnection CLI_NewConnection) { return m_cliShell; }
                        public void onCloseConnection(cli.TelnetConnection CLI_ClosedConnection, cli.ExecutionContext.Interface CLI_Context) {}
                    };

                    //cli.OutputDevice.getStdOut().put("Running telnet server on port " + m_iTelnetPort).endl();
                    TestTelnetServer cli_Server = new TestTelnetServer(CLI_Shell, m_iTelnetPort);
                    cli_Server.startServer();
                }
                else if (m_cliIn != null) {
                    //cli.OutputDevice.getStdOut().put("Regular execution with device " + m_cliIn.getDebugName()).endl();
                    CLI_Shell.run(m_cliIn);
                }
            } catch (Exception e) {
                TestTools.error().printStackTrace(e);
            }
        }
        private void displayHelp(String J_ProgramName) {
            cli.OutputDevice.getStdOut().put("USAGE").endl();
            cli.OutputDevice.getStdOut().put("   " + J_ProgramName + " --help").endl();
            cli.OutputDevice.getStdOut().put("       Display this help.").endl();
            cli.OutputDevice.getStdOut().put("   " + J_ProgramName + " [--console]").endl();
            cli.OutputDevice.getStdOut().put("       Interactive mode.").endl();
            cli.OutputDevice.getStdOut().put("   " + J_ProgramName + " --telnet <port>").endl();
            cli.OutputDevice.getStdOut().put("       Telnet mode.").endl();
            cli.OutputDevice.getStdOut().put("   " + J_ProgramName + " [--input <input file> [--output <output file>]]").endl();
            cli.OutputDevice.getStdOut().put("       Input from file, output to file or standard output.").endl();
            m_bNoExec = true;
        }
    }

    // Test cli
    public static class Cli extends cli.Cli {
        public Cli() { super("TestCli", new cli.Help()); }
        public void populate() {}
        public boolean execute(cli.CommandLine CLI_CmdLine) { return false; }
    }

    // Test device
    public static class Device extends cli.NonBlockingIODevice.Java { // from TestUI + TestNonBlocking + TestExec
        private cli.OutputDevice.Interface m_cliOut;
        public Device(cli.OutputDevice.Interface CLI_Out) { super("TestDevice"); m_cliOut = CLI_Out; }
        public boolean openDevice() { return m_cliOut.openDevice(); }
        public boolean closeDevice() { return m_cliOut.closeDevice(); }
        public cli.OutputDevice.Interface put(String J_Text) { m_cliOut.put(J_Text); return this; }
        public void beep() { m_cliOut.put("%beep%"); }
        public void cleanScreen() { m_cliOut.put("%cls%"); }
        public cli.OutputDevice.ScreenInfo getScreenInfo() { return new cli.OutputDevice.ScreenInfo(10, 10, false, false); }
        public boolean wouldOutput(cli.OutputDevice.Interface CLI_Device) { return (super.wouldOutput(CLI_Device) || m_cliOut.wouldOutput(CLI_Device)); }
        public void pushKeys(String J_Keys) {
            for (int i=0; i<J_Keys.length(); i++) {
                onKey(J_Keys.charAt(i));
            }
        }
        public cli.ResourceString getLocation() { return new cli.ResourceString(); }

        /** @warning Only for cli.StringDevice outputs. */
        public String getOutput() {
            String j_Output = ((cli.StringDevice) m_cliOut).getString();
            ((cli.StringDevice) m_cliOut).reset();
            return j_Output;
        }
        /** @warning Only for cli.StringDevice outputs. */
        public boolean checkOutput(ExpectedOutput CLI_Out) {
            String j_Output = ((cli.StringDevice) m_cliOut).getString();
            if (! j_Output.equals(CLI_Out.toString())) {
                TestTools.error().put("'" + j_Output + "' does not match '" + CLI_Out.toString() + "'").endl();
                return false;
            } else {
                clearOutput();
                return true;
            }
        }
        /** @warning Only for cli.StringDevice outputs. */
        public void clearOutput() {
            ((cli.StringDevice) m_cliOut).reset();
        }
    }
    public static class ExpectedOutput {
        public ExpectedOutput() { m_jOut = new String(); }
        public ExpectedOutput txt(String J_Txt) { m_jOut += J_Txt; return this; }
        public ExpectedOutput endl() { m_jOut += "\n"; return this; }
        public String toString() { return m_jOut; }
        private String m_jOut;
    }

    // Traces, errors and warnings
    private static cli.TraceClass TRACE_CLASS = new cli.TraceClass("CLI_TEST", new cli.Help());
    public static cli.OutputDevice.Interface error() {
        displayCallLocation(cli.OutputDevice.getStdErr());
        cli.OutputDevice.getStdErr().put("Error! ");
        return cli.OutputDevice.getStdErr();
    }
    public static cli.OutputDevice.Interface warning() {
        displayCallLocation(cli.OutputDevice.getStdErr());
        cli.OutputDevice.getStdErr().put("Warning! ");
        return cli.OutputDevice.getStdErr();
    }
    public static void trace(String J_Trace) {
        cli.Traces.trace(TRACE_CLASS, J_Trace);
    }
    private static void displayCallLocation(cli.OutputDevice.Interface CLI_OutputStream) {
        try {
            throw new Exception();
        } catch (Exception e) {
            for (int i=e.getStackTrace().length-2; i>=0; i--) {
                StackTraceElement j_StackTrace = e.getStackTrace()[i];
                String j_File = j_StackTrace.getFileName();
                String j_Line = new Integer(j_StackTrace.getLineNumber()).toString();
                String j_Class = j_StackTrace.getClassName();
                String j_Method = j_StackTrace.getMethodName();
                if (! j_Class.equals("TestTools")) {
                    CLI_OutputStream.put(j_File);
                    CLI_OutputStream.put(":").put(j_Line);
                    CLI_OutputStream.put("[").put(j_Method).put("()]");
                    CLI_OutputStream.put(": ");
                    return;
                }
            }
        }
        CLI_OutputStream.put("Unknown location: ");
    }

    // Test executions
    public static class Result extends cli.ExecutionResult { // from TestUI
        public Result() { m_cliCtx = null; }
        protected void onResult(cli.ExecutionContext.Interface CLI_Context) { m_cliCtx = CLI_Context; }
        public cli.ExecutionContext.Interface getCtx() { return m_cliCtx; }
        private cli.ExecutionContext.Interface m_cliCtx;
    }

}

class Out extends TestTools.ExpectedOutput {};
