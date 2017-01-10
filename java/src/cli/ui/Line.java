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


/** Simple line user interface object. */
public class Line extends UI {

    /** Top execution context constructor.
        @param J_DefaultLine Default value.
        @param I_MinLineLength Minimum line length required. -1 if not set.
        @param I_MaxLineLength Maximum line length required. -1 if not set. */
    public Line(String J_DefaultLine, int I_MinLineLength, int I_MaxLineLength) {
        super(__Line(0, J_DefaultLine, I_MinLineLength, I_MaxLineLength));
    }
    /** Child execution context constructor.
        @param CLI_ParentContext Parent execution context.
        @param J_DefaultLine Default value.
        @param I_MinLineLength Minimum line length required. -1 if not set.
        @param I_MaxLineLength Maximum line length required. -1 if not set. */
    public Line(cli.ExecutionContext.Interface CLI_ParentContext, String J_DefaultLine, int I_MinLineLength, int I_MaxLineLength) {
        super(__Line(CLI_ParentContext.getNativeRef(), J_DefaultLine, I_MinLineLength, I_MaxLineLength));
    }
    private static final native int __Line(int I_NativeParentContextRef, String J_DefaultLine, int I_MinLineLength, int I_MaxLineLength);

    /** Contructor for derived classes only.
        @param I_NativeRef Native object reference. */
    protected Line(int I_NativeRef) {
        super(I_NativeRef);
    }

    /** Line retrieval.
        @return Line entered by the user. */
    public String getLine() {
        return __getLine(this.getNativeRef());
    }
    private static final native String __getLine(int I_NativeLineRef);

}
