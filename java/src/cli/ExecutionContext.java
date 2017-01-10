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


/** Execution context.

    An execution context, in the CLI library, is something that manages:

        # a set of input/output devices,

        # settings: language, beep...

        # input character processing

    It may be either a top or child execution context, depending on the kind of constructor actually called.
    A child execution context uses the same context as its parent. */
public abstract class ExecutionContext {

    /** All streams constant. */
    public static final int ALL_STREAMS = -1;
    /** Welcome stream constant. Useful for bye too. */
    public static final int WELCOME_STREAM = 0;
    /** Prompt stream constant. */
    public static final int PROMPT_STREAM = 1;
    /** Echo stream constant. */
    public static final int ECHO_STREAM = 2;
    /** Output stream constant. */
    public static final int OUTPUT_STREAM = 3;
    /** Error stream constant. */
    public static final int ERROR_STREAM = 4;
    /** Number of stream types. */
    public static final int STREAM_TYPES_COUNT = 5;

    /** Generic execution context interface. */
    public interface Interface {
        /** Input stream accessor.
            @return The input stream attached to the context for its execution. */
        public IODevice.Interface getInput();

        /** Output stream accessor.
            @param E_StreamType Output stream identifier.
            @return The required stream. null if an error occured. */
        public OutputDevice.Interface getStream(int E_StreamType);

        /** Output stream positionning.
            @param E_StreamType Output stream identifier.
            @param CLI_Stream Stream reference.
            @return true: success, false: failure. */
        public boolean setStream(int E_StreamType, OutputDevice.Interface CLI_Stream);

        /** Enabled/disabled stream status accessor.
            @param E_StreamType Output stream identifier.
            @return true: the stream is enabled, false: the stream is disabled. */
        public boolean streamEnabled(int E_StreamType);

        /** Enabling/disabling output streams.
            @param E_StreamType Output stream identifier.
            @param B_Enable Enable flag.
            @return true: success, false: failure. */
        public boolean enableStream(int E_StreamType, boolean B_Enable);

        /** Language setting.
            @param E_Lang New value. */
        public void setLang(int E_Lang);

        /** Language access.
            @return The language currently set. */
        public int getLang();

        /** Beep configuration setting.
            @param B_Enable New value. */
        public void setBeep(boolean B_Enable);

        /** Beep configuration access.
            @return The current beep configuration. true if enabled, false otherwise. */
        public boolean getBeep();

        /** Context execution result interface registration.
            Note: Registration for each interface is discarded as soon as the end of execution is notified. */
        public void watchResult(ExecutionResult CLI_Result);

        /** Runs the execution context onto the corresponding input/output device.
            Call this runner method, for top execution contexts.
            @param CLI_IODevice Input/output device to run onto.
            @return true: success, false: failure. */
        public boolean run(IODevice.Interface CLI_IODevice);

        /** Runs the execution context as a child context of the parent given by the construction.
            Call this runner method, for child execution contexts.
            @return true: success, false: failure. */
        public boolean run();

        /** Tells whether this execution context is running or not.
            @return The running status. */
        public boolean isRunning();

        /** Terminates this execution context's execution.
            Warning: Not thread safe. Implement a non blocking device if your CLI execution should interact with external events. */
        public void stopExecution();

        /** Terminates execution for all execution contexts attached to the same top execution context.
            Warning: Not thread safe. Implement a non blocking device if your CLI execution should interact with external events. */
        public void stopAllExecutions();

        /** Native object interface compliance.
            @return The native reference of the instance. */
        public int getNativeRef();
    }

    /** Common behaviours of non-blocking intput/output devices, whatever their location of implementation. */
    public static abstract class Common extends NativeObject implements ExecutionContext.Interface {
        /** Top and child execution context constructor.
            @param I_NativeRef Native instance referenced corresponding to this Java instance. */
        protected Common(int I_NativeRef) {
            super(I_NativeRef);
        }

        public final IODevice.Interface getInput() {
            return (IODevice.Interface) NativeObject.getObject(ExecutionContext.__Common__getInput(this.getNativeRef()));
        }

        public final OutputDevice.Interface getStream(int E_StreamType) {
            return (OutputDevice.Interface) NativeObject.getObject(ExecutionContext.__Common__getStream(this.getNativeRef(), E_StreamType));
        }
        public final boolean setStream(int E_StreamType, OutputDevice.Interface CLI_Stream) {
            return ExecutionContext.__Common__setStream(this.getNativeRef(), E_StreamType, CLI_Stream.getNativeRef());
        }

        public final boolean streamEnabled(int E_StreamType) {
            return ExecutionContext.__Common__streamEnabled(this.getNativeRef(), E_StreamType);
        }
        public final boolean enableStream(int E_StreamType, boolean B_Enable) {
            return ExecutionContext.__Common__enableStream(this.getNativeRef(), E_StreamType, B_Enable);
        }

        public final void setLang(int E_Lang) {
            ExecutionContext.__Common__setLang(this.getNativeRef(), E_Lang);
        }
        public final int getLang() {
            return ExecutionContext.__Common__getLang(this.getNativeRef());
        }

        public final void setBeep(boolean B_Enable) {
            ExecutionContext.__Common__setBeep(this.getNativeRef(), B_Enable);
        }
        public final boolean getBeep() {
            return ExecutionContext.__Common__getBeep(this.getNativeRef());
        }

        public final void watchResult(ExecutionResult CLI_Result) {
            CLI_Result.watchResult(this);
        }

        public final boolean run(IODevice.Interface CLI_IODevice) {
            try {
                return ExecutionContext.__Common__run(this.getNativeRef(), CLI_IODevice.getNativeRef());
            } catch (Exception e) {
                cli.OutputDevice.Interface cli_ErrorStream = getStream(ERROR_STREAM);
                if (cli_ErrorStream == null) {
                    cli_ErrorStream = cli.OutputDevice.getStdErr();
                }
                if (cli_ErrorStream != null) {
                    cli_ErrorStream.printStackTrace(e);
                }
                return false;
            }
        }
        public final boolean run() {
            return run(getInput());
        }

        public final boolean isRunning() {
            return ExecutionContext.__Common__isRunning(this.getNativeRef());
        }

        public final void stopExecution() {
            ExecutionContext.__Common__stopExecution(this.getNativeRef());
        }

        public final void stopAllExecutions() {
            ExecutionContext.__Common__stopAllExecutions(this.getNativeRef());
        }
    }
    // JNI seems to have trouble at linking following methods when they are embedded in the nested Common class above (at least with java 1.5.0_03).
    // Therefore they are just declared in the scope of the global ExecutionContext class with a __Common prefix.
    private static final native int __Common__getInput(int I_NativeContextRef);
    private static final native int __Common__getStream(int I_NativeContextRef, int E_StreamType);
    private static final native boolean __Common__setStream(int I_NativeContextRef, int E_StreamType, int I_NativeDeviceRef);
    private static final native boolean __Common__streamEnabled(int I_NativeContextRef, int E_StreamType);
    private static final native boolean __Common__enableStream(int I_NativeContextRef, int E_StreamType, boolean B_Enable);
    private static final native void __Common__setLang(int I_NativeContextRef, int E_Lang);
    private static final native int __Common__getLang(int I_NativeContextRef);
    private static final native void __Common__setBeep(int I_NativeContextRef, boolean B_Enable);
    private static final native boolean __Common__getBeep(int I_NativeContextRef);
    private static final native boolean __Common__run(int I_NativeContextRef, int I_NativeIODeviceRef);
    private static final native boolean __Common__isRunning(int I_NativeContextRef);
    private static final native void __Common__stopExecution(int I_NativeContextRef);
    private static final native void __Common__stopAllExecutions(int I_NativeContextRef);

    /** Native-implemented execution contexts. */
    public static abstract class Native extends Common {
        /** Top and child execution context constructor.
            @param I_NativeRef Native instance referenced corresponding to this Java instance. */
        protected Native(int I_NativeRef) {
            super(I_NativeRef);
        }
    }

    /** Java-implemented execution contexts. */
    public static abstract class Java extends ExecutionContext.Native implements ExecutionContext.Interface {
        /** Top execution context constructor. */
        protected Java() {
            super(__Java__ExecutionContext(0));
        }

        /** Top and child execution context constructor.
            Makes this execution context run in the same context as the one given for parent.
            @param CLI_ParentContext Parent execution context. */
        protected Java(ExecutionContext.Interface CLI_ParentContext) {
            super(__Java__ExecutionContext(CLI_ParentContext.getNativeRef()));
        }

        /** Beginning of execution handler.
            @return true for success, false otherwise. */
        protected abstract boolean onStartExecution();
        private final boolean __onStartExecution() {
            Traces.trace(NativeTraces.CLASS, NativeTraces.begin("ExecutionContext.Java.__onStartExecution()"));
            boolean b_Res = onStartExecution();
            Traces.trace(NativeTraces.CLASS, NativeTraces.end("ExecutionContext.Java.__onStartExecution()", new Boolean(b_Res).toString()));
            return b_Res;
        }

        /** Handler called on character input.
            @param E_KeyCode Input key. */
        protected abstract void onKey(int E_KeyCode);
        private final void __onKey(int E_KeyCode) {
            Traces.trace(NativeTraces.CLASS, NativeTraces.begin("ExecutionContext.Java.__onKey(E_KeyCode)"));
            Traces.trace(NativeTraces.CLASS, NativeTraces.param("E_KeyCode", new Integer(E_KeyCode).toString()));
            onKey(E_KeyCode);
            Traces.trace(NativeTraces.CLASS, NativeTraces.end("ExecutionContext.Java.__onKey()"));
        }

        /** Execution termination handler.
            @return true for success, false otherwise. */
        protected abstract boolean onStopExecution();
        private final boolean __onStopExecution() {
            Traces.trace(NativeTraces.CLASS, NativeTraces.begin("ExecutionContext.Java.__onStopExecution()"));
            boolean b_Res = onStopExecution();
            Traces.trace(NativeTraces.CLASS, NativeTraces.end("ExecutionContext.Java.__onStopExecution()", new Boolean(b_Res).toString()));
            return b_Res;
        }
    }
    // JNI seems to have trouble at linking following methods when they are embedded in the nested Java class above (at least with java 1.5.0_03).
    // Therefore they are just declared in the scope of the global ExecutionContext class with a __Java prefix.
    private static final native int __Java__ExecutionContext(int I_NativeParentContextRef);
}
