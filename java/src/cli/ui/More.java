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

package cli.ui;


/** 'more' display object. */
public class More extends UI {

    /** Top execution context constructor. */
    public More() {
        super(__More(0));
        __MoreBody();
    }
    /** Child execution context constructor.
        @param CLI_ParentContext Parent execution context. */
    public More(cli.ExecutionContext.Interface CLI_ParentContext) {
        super(__More(CLI_ParentContext.getNativeRef()));
        __MoreBody();
    }
    private static final native int __More(int I_NativeParentContextRef);
    private final void __MoreBody() {
        // Create a member inner device right away.
        m_cliInnerDevice = new InnerDevice(__getText(this.getNativeRef()));
        // However, do not let Java propagate deletion to native in order not to delete twice the native object.
        m_cliInnerDevice.dontFinalize();
    }

    /** Inner device accessor.
        @return Inner device instance. */
    public cli.OutputDevice.Interface getText() {
        return m_cliInnerDevice;
    }
    private static final native int __getText(int I_NativeLessRef);

    /** Console input/output device. */
    private static class InnerDevice extends cli.OutputDevice.Native
    {
        /** Constructor. */
        public InnerDevice(int I_NativeDeviceRef) {
            super(I_NativeDeviceRef);
        }
    }

    /** Inner device reference. */
    private InnerDevice m_cliInnerDevice;
}
