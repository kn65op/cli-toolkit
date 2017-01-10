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


/** Result interface for execution contexts.
    This interface implements callbacks on execution contexts termination. */
public abstract class ExecutionResult extends NativeObject {

    /** Default constructor. */
    protected ExecutionResult() {
        super(__ExecutionResult());
    }
    private static final native int __ExecutionResult();

    /** Context execution result interface registration.
        Note: Registration for each interface is discarded as soon as the end of execution is notified. */
    public void watchResult(ExecutionContext.Interface CLI_Context) {
        __watchResult(this.getNativeRef(), CLI_Context.getNativeRef());
    }
    private static final native void __watchResult(int I_NativeResultRef, int I_NativeContextRef);

    /** User interface result handler.
        @param CLI_Context Execution context which execution is done. */
    protected abstract void onResult(ExecutionContext.Interface CLI_Context);
    private final void __onResult(int I_NativeContextRef) {
        Traces.trace(NativeTraces.CLASS, NativeTraces.begin("ExecutionResult.__onResult(I_NativeContextRef)"));
        Traces.trace(NativeTraces.CLASS, NativeTraces.param("I_NativeContextRef", new Integer(I_NativeContextRef).toString()));
        Traces.trace(NativeTraces.CLASS, NativeTraces.value("this", new Integer(this.getNativeRef()).toString()));

        try {
            NativeObject cli_Context = NativeObject.getObject(I_NativeContextRef);
            if (cli_Context != null) {
                Traces.trace(NativeTraces.CLASS, NativeTraces.value("cli_Context", cli_Context.toString()));
                if (cli_Context instanceof ExecutionContext.Interface) {
                    onResult((ExecutionContext.Interface) cli_Context);
                    // Execution done for the CLI lib: let's forget Java result interface here???
                    // Not sure this convenient in case the result interface is used several times.
                    //if (cli_Context != null) { NativeObject.forget(cli_Context); }
                }
            } else {
                OutputDevice.getStdErr().put("Could not find ExecutionContext reference " + new Integer(I_NativeContextRef)).endl();
            }
        } catch (Exception e) {
            OutputDevice.getStdErr().printStackTrace(e);
        }

        Traces.trace(NativeTraces.CLASS, NativeTraces.end("ExecutionResult.__onResult()"));
    }
}
