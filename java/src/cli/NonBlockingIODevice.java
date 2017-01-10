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


/** Non-blocking input device. */
public abstract class NonBlockingIODevice {

    /** Generic input/output device interface. */
    public interface Interface extends IODevice.Interface {
        /** Handler to call when a key is received.
            @param E_Key Input key. */
        public void onKey(int E_Key);
    }

    private static final native int __Common__getExecutionContext(int I_NativeDeviceRef);
    private static final native void __Common__onKey(int I_NativeDeviceRef, int E_Key);

    /** Native-implemented input/output devices. */
    public static abstract class Native extends IODevice.Native implements NonBlockingIODevice.Interface {
        /** Constructor.
            @param I_NativeRef Native instance reference. */
        public Native(int I_NativeRef) {
            super(I_NativeRef);
        }

        /** Returns the current execution context.
            @return Current execution context if any, NULL otherwise. */
        protected ExecutionContext.Interface getExecutionContext() {
            NativeObject cli_Context = NativeObject.getObject(NonBlockingIODevice.__Common__getExecutionContext(this.getNativeRef()));
            if (cli_Context instanceof ExecutionContext.Common) {
                return (ExecutionContext.Common) cli_Context;
            } else {
                return null;
            }
        }

        public void onKey(int E_Key) {
            NonBlockingIODevice.__Common__onKey(this.getNativeRef(), E_Key);
        }
    }

    /** Java-implemented non blocking input/output devices. */
    public static abstract class Java extends IODevice.Java implements NonBlockingIODevice.Interface {
        /** Constructor.
            @param STR_DbgName Debug name. */
        public Java(String STR_DbgName) {
            super(NonBlockingIODevice.__Java__Java(STR_DbgName));
        }

        // NonBlockingIODevice.Interface Java input/output device implementation.

        public int getKey() {
            return NonBlockingIODevice.__Java__getKey(this.getNativeRef());
        }

        /** Returns the current execution context.
            @return Current execution context if any, NULL otherwise. */
        protected ExecutionContext.Interface getExecutionContext() {
            NativeObject cli_Context = NativeObject.getObject(NonBlockingIODevice.__Common__getExecutionContext(this.getNativeRef()));
            if (cli_Context instanceof ExecutionContext.Common) {
                return (ExecutionContext.Common) cli_Context;
            } else {
                return null;
            }
        }

        public void onKey(int E_Key) {
            NonBlockingIODevice.__Common__onKey(this.getNativeRef(), E_Key);
        }
    }
    // JNI seems to have trouble at linking following methods when they are embedded in the nested Native class above (at least with java 1.5.0_03).
    // Therefore they are just declared in the scope of the global NonBlockingIODevice class with a __Native prefix.
    private static final native int __Java__Java(String STR_DbgName);
    private static final native int __Java__getKey(int I_NativeDeviceRef);
}
