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

package cli;


/** Telnet server class.

    Virtual object that shall be overridden for ExecutionContext (shell & cli basically) instance creations.

    Note:
    With the redesign of telnet connections, made as non blocking, this class had been redesigned in CLI 2.7,
    made virtual so that the user could create as many shells and clis for each connection.
    With the redesign of execution contexts, this class has been redesigned again in CLI 2.8,
    so that the user can create shells and clis, but also any other kind of execution contexts. */
public abstract class TelnetServer extends NativeObject
{
    /** Constructor.
        @param I_MaxConnections Maximum number of connections managed at the same time.
        @param I_TcpPort TCP port to listen onto.
        @param E_Lang Debugging language. */
    public TelnetServer(int I_MaxConnections, int I_TcpPort, int E_Lang) {
        super(__TelnetServer(I_MaxConnections, I_TcpPort, E_Lang));
    }
    private static final native int __TelnetServer(int I_MaxConnections, int I_TcpPort, int E_Lang);

    /** Starts the server.
        Warning: blocking call. */
    public void startServer() {
        try {
            __startServer(this.getNativeRef());
        } catch (Exception e) {
            cli.OutputDevice.getStdErr().printStackTrace(e);
        }
    }
    private static final native void __startServer(int I_NativeServerRef);

    /** Stops the server */
    public void stopServer() {
        try {
            __stopServer(this.getNativeRef());
        } catch (Exception e) {
            cli.OutputDevice.getStdErr().printStackTrace(e);
        }
    }
    private static final native void __stopServer(int I_NativeServerRef);

    protected abstract ExecutionContext.Interface onNewConnection(TelnetConnection CLI_NewConnection);
    private final int __onNewConnection(int I_NativeConnectionRef) {
        Traces.safeTrace(NativeTraces.CLASS, I_NativeConnectionRef, NativeTraces.begin("TelnetServer.__onNewConnection(I_NativeConnectionRef)"));
        Traces.safeTrace(NativeTraces.CLASS, I_NativeConnectionRef, NativeTraces.param("I_NativeConnectionRef", new Integer(I_NativeConnectionRef).toString()));

        int i_NativeContextRef = 0;
        NativeObject cli_Connection = NativeObject.getObject(I_NativeConnectionRef);
        if ((cli_Connection != null) && (cli_Connection instanceof TelnetConnection)) {
            ExecutionContext.Interface cli_Context = onNewConnection((TelnetConnection) cli_Connection);
            if (cli_Context != null) {
                // Let's remember the new execution context, in case it is not referenced anywhere else, and could be destroyed by the garbage collector.
                // Forget will be done in __onCloseConnection().
                //NativeObject.remember((ExecutionContext.Common) cli_Context); // Note: No need to force remembering of the Java object for the CLI lib, already done by construction.
                i_NativeContextRef = cli_Context.getNativeRef();
            }
        }

        Traces.safeTrace(NativeTraces.CLASS, I_NativeConnectionRef, NativeTraces.end("TelnetServer.__onNewConnection()", new Integer(i_NativeContextRef).toString()));
        return i_NativeContextRef;
    }

    /** Execution context release handler.
        @param CLI_ConnectionClosed Telnet connection being closed.
        @param CLI_Context Execution context to be released. */
    protected abstract void onCloseConnection(TelnetConnection CLI_ConnectionClosed, ExecutionContext.Interface CLI_Context);
    private final void __onCloseConnection(int I_NativeConnectionRef, int I_NativeContextRef) {
        Traces.safeTrace(NativeTraces.CLASS, I_NativeConnectionRef, NativeTraces.begin("TelnetServer.__onCloseConnection(I_NativeConnectionRef, I_NativeContextRef)"));
        Traces.safeTrace(NativeTraces.CLASS, I_NativeConnectionRef, NativeTraces.param("I_NativeConnectionRef", new Integer(I_NativeConnectionRef).toString()));
        Traces.safeTrace(NativeTraces.CLASS, I_NativeConnectionRef, NativeTraces.param("I_NativeContextRef", new Integer(I_NativeContextRef).toString()));

        NativeObject cli_ConnectionClosed = NativeObject.getObject(I_NativeConnectionRef);
        NativeObject cli_Context = NativeObject.getObject(I_NativeContextRef);
        onCloseConnection((TelnetConnection) cli_ConnectionClosed, (ExecutionContext.Interface) cli_Context);
        // Execution done for the CLI lib: let's forget Java objects here.
        if (cli_Context != null) { NativeObject.forget(cli_Context); }

        Traces.safeTrace(NativeTraces.CLASS, I_NativeConnectionRef, NativeTraces.end("TelnetServer.__onCloseConnection()"));
    }
}
