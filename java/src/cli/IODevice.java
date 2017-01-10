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


/** Input/output device management.

    See OutputDevice comment. */
public abstract class IODevice {

    /** Generic input/output device interface. */
    public interface Interface extends OutputDevice.Interface {
        /** Input key capture.
            @return Key captured. */
        public int getKey();

        /** Location accessor.
            @return The location as a resource string. */
        public ResourceString getLocation();

        /** Stack overflow protection.
            @param CLI_Device Other device that the callee device should check it would input characters from.
            @return true if the callee device would redirect input to the given device for reading.

            Determines whether the current device would input the given device in any ways.
            Default implementation checks whether CLI_Device is the self device. */
        public boolean wouldInput(IODevice.Interface CLI_Device);
    }

    /** Native-implemented input/output devices. */
    public static abstract class Native extends OutputDevice.Native implements IODevice.Interface {
        /** Constructor.
            @param I_NativeRef Native instance reference. */
        protected Native(int I_NativeRef) {
            super(I_NativeRef);
        }

        // IODevice.Interface native input/output device implementation.

        public final int getKey() {
            return IODevice.__Native__getKey(this.getNativeRef());
        }

        public ResourceString getLocation() {
            ResourceString cli_Location = (ResourceString) NativeObject.getObject(IODevice.__Native__getLocation(this.getNativeRef()));
            NativeObject.forget(cli_Location);
            return cli_Location;
        }

        public boolean wouldInput(IODevice.Interface CLI_Device) {
            return IODevice.__Native__wouldInput(this.getNativeRef(), CLI_Device.getNativeRef());
        }
    }
    // JNI seems to have trouble at linking following methods when they are embedded in the nested Native class above (at least with java 1.5.0_03).
    // Therefore they are just declared in the scope of the global IODevice class with a __Native prefix.
    private static final native int __Native__getKey(int I_NativeDeviceRef);
    private static final native int __Native__getLocation(int I_NativeDeviceRef);
    private static final native boolean __Native__wouldInput(int I_NativeIODeviceRef, int I_NativeIODevice2Ref);

    /** Java-implemented input/output devices. */
    public static abstract class Java extends OutputDevice.Java implements IODevice.Interface {
        /** Constructor.
            @param J_DbgName Debug name. Useful for traces only. */
        protected Java(String J_DbgName) {
            super(IODevice.__Java__Java(J_DbgName));
        }

        /** Constructor for NonBlockingIODevice classes only.
            @param I_NativeRef Native instance reference. */
        protected Java(int I_NativeRef) {
            super(I_NativeRef);
        }

        // IODevice.Interface Java input/output device implementation.

        public abstract int getKey();
        private final int __getKey() {
            Traces.safeTrace(NativeTraces.CLASS, this, NativeTraces.begin("IODevice.Java.__getKey()"));
            int e_Key = getKey();
            Traces.safeTrace(NativeTraces.CLASS, this, NativeTraces.end("IODevice.Java.__getKey()", new Integer(e_Key).toString()));
            return e_Key;
        }

        public abstract ResourceString getLocation();
        private final void __getLocation(int I_NativeResourceStringRef) {
            Traces.safeTrace(NativeTraces.CLASS, this, NativeTraces.begin("IODevice.Java.__getLocation(I_NativeResourceStringRef)"));
            Traces.safeTrace(NativeTraces.CLASS, this, NativeTraces.param("I_NativeResourceStringRef", new Integer(I_NativeResourceStringRef).toString()));
            // Retrieve the native resource string instance (output parameter).
            ResourceString cli_NativeLocation = (ResourceString) NativeObject.getObject(I_NativeResourceStringRef);
            if (cli_NativeLocation != null) {
                // Make the call to the Java handler.
                ResourceString cli_JavaLocation = getLocation();
                if (cli_JavaLocation != null) {
                    // Report information from Java to native instance.
                    for (int e_Lang = 0; e_Lang < ResourceString.LANG_COUNT; e_Lang++) {
                        Traces.safeTrace(NativeTraces.CLASS, this, NativeTraces.value("e_Lang", new Integer(e_Lang).toString()));
                        String j_Location = cli_JavaLocation.getString(e_Lang);
                        Traces.safeTrace(NativeTraces.CLASS, this, NativeTraces.value("j_Location", j_Location));
                        cli_NativeLocation.setString(e_Lang, j_Location);
                    }
                }
            }
            Traces.safeTrace(NativeTraces.CLASS, this, NativeTraces.end("IODevice.Java.__getLocation()"));
        }

        public boolean wouldInput(IODevice.Interface CLI_Device) {
            return (CLI_Device == this);
        }
        private final boolean __wouldInput(int I_NativeDeviceRef) {
            // Do not trace! for consistency reasons.
            //Traces.safeTrace(NativeTraces.CLASS, this, NativeTraces.begin("OutputDevice.Java.__wouldOutput(CLI_Device)"));
            boolean b_Res = false;
            try {
                IODevice.Interface cli_Device = (IODevice.Interface) NativeObject.getObject(I_NativeDeviceRef);
                if (cli_Device != null) {
                    b_Res = wouldInput(cli_Device);
                }
            } catch (Exception e) {
            }
            //Traces.safeTrace(NativeTraces.CLASS, this, NativeTraces.end("OutputDevice.Java.__wouldOutput()"));
            return b_Res;
        }
    }
    // JNI seems to have trouble at linking following methods when they are embedded in the nested Java class above (at least with java 1.5.0_03).
    // Therefore they are just declared in the scope of the global IODevice class with a __Java prefix.
    private static final native int __Java__Java(String J_DbgName);


    // General static input/output device features.

    /** Null device singleton.
        @return The null output device. */
    public static final IODevice.Interface getNullDevice() {
        class NullDevice extends IODevice.Native {
            public NullDevice() {
                super(__getNullDevice());
            }
        }
        if (m_cliNullDevice == null) {
            m_cliNullDevice = new NullDevice();
        }
        return m_cliNullDevice;
    }
    private static final native int __getNullDevice();
    private static IODevice.Interface m_cliNullDevice = null;

    /** Standard input device singleton.
        @return The standard input device. */
    public static final IODevice.Interface getStdIn() {
        class StdInDevice extends IODevice.Native {
            public StdInDevice() {
                super(__getStdIn());
            }
        }
        if (m_cliStdIn == null) {
            m_cliStdIn = new StdInDevice();
        }
        return m_cliStdIn;
    }
    private static final native int __getStdIn();
    private static IODevice.Interface m_cliStdIn = null;

    /** @brief Common char translation.
        @param I_Char Common char to translate.
        @return KEY code corresponding to the given common char.

        No JNI call to native implementation, but full Java re-implementation in order to avoid strange Java/native character translations. */
    static final int getKey(int I_Char) {
        switch (I_Char) {
        case 10: case 13:   return OutputDevice.ENTER;

        case ' ':   return OutputDevice.SPACE;
        case '\t':  return OutputDevice.TAB;
        case '\b':  return OutputDevice.BACKSPACE;

        case '0':   return OutputDevice.KEY_0;
        case '1':   return OutputDevice.KEY_1;
        case '2':   return OutputDevice.KEY_2;
        case '3':   return OutputDevice.KEY_3;
        case '4':   return OutputDevice.KEY_4;
        case '5':   return OutputDevice.KEY_5;
        case '6':   return OutputDevice.KEY_6;
        case '7':   return OutputDevice.KEY_7;
        case '8':   return OutputDevice.KEY_8;
        case '9':   return OutputDevice.KEY_9;

        case 'a':   return OutputDevice.KEY_a;
        case 'á':   return OutputDevice.KEY_aacute;
        case 'à':   return OutputDevice.KEY_agrave;
        case 'ä':   return OutputDevice.KEY_auml;
        case 'â':   return OutputDevice.KEY_acirc;
        case 'b':   return OutputDevice.KEY_b;
        case 'c':   return OutputDevice.KEY_c;
        case 'ç':   return OutputDevice.KEY_ccedil;
        case 'd':   return OutputDevice.KEY_d;
        case 'e':   return OutputDevice.KEY_e;
        case 'é':   return OutputDevice.KEY_eacute;
        case 'è':   return OutputDevice.KEY_egrave;
        case 'ë':   return OutputDevice.KEY_euml;
        case 'ê':   return OutputDevice.KEY_ecirc;
        case 'f':   return OutputDevice.KEY_f;
        case 'g':   return OutputDevice.KEY_g;
        case 'h':   return OutputDevice.KEY_h;
        case 'i':   return OutputDevice.KEY_i;
        case 'í':   return OutputDevice.KEY_iacute;
        case 'ì':   return OutputDevice.KEY_igrave;
        case 'ï':   return OutputDevice.KEY_iuml;
        case 'î':   return OutputDevice.KEY_icirc;
        case 'j':   return OutputDevice.KEY_j;
        case 'k':   return OutputDevice.KEY_k;
        case 'l':   return OutputDevice.KEY_l;
        case 'm':   return OutputDevice.KEY_m;
        case 'n':   return OutputDevice.KEY_n;
        case 'o':   return OutputDevice.KEY_o;
        case 'ó':   return OutputDevice.KEY_oacute;
        case 'ò':   return OutputDevice.KEY_ograve;
        case 'ö':   return OutputDevice.KEY_ouml;
        case 'ô':   return OutputDevice.KEY_ocirc;
        case 'p':   return OutputDevice.KEY_p;
        case 'q':   return OutputDevice.KEY_q;
        case 'r':   return OutputDevice.KEY_r;
        case 's':   return OutputDevice.KEY_s;
        case 't':   return OutputDevice.KEY_t;
        case 'u':   return OutputDevice.KEY_u;
        case 'ú':   return OutputDevice.KEY_uacute;
        case 'ù':   return OutputDevice.KEY_ugrave;
        case 'ü':   return OutputDevice.KEY_uuml;
        case 'û':   return OutputDevice.KEY_ucirc;
        case 'v':   return OutputDevice.KEY_v;
        case 'w':   return OutputDevice.KEY_w;
        case 'x':   return OutputDevice.KEY_x;
        case 'y':   return OutputDevice.KEY_y;
        case 'z':   return OutputDevice.KEY_z;

        case 'A':   return OutputDevice.KEY_A;
        case 'B':   return OutputDevice.KEY_B;
        case 'C':   return OutputDevice.KEY_C;
        case 'D':   return OutputDevice.KEY_D;
        case 'E':   return OutputDevice.KEY_E;
        case 'F':   return OutputDevice.KEY_F;
        case 'G':   return OutputDevice.KEY_G;
        case 'H':   return OutputDevice.KEY_H;
        case 'I':   return OutputDevice.KEY_I;
        case 'J':   return OutputDevice.KEY_J;
        case 'K':   return OutputDevice.KEY_K;
        case 'L':   return OutputDevice.KEY_L;
        case 'M':   return OutputDevice.KEY_M;
        case 'N':   return OutputDevice.KEY_N;
        case 'O':   return OutputDevice.KEY_O;
        case 'P':   return OutputDevice.KEY_P;
        case 'Q':   return OutputDevice.KEY_Q;
        case 'R':   return OutputDevice.KEY_R;
        case 'S':   return OutputDevice.KEY_S;
        case 'T':   return OutputDevice.KEY_T;
        case 'U':   return OutputDevice.KEY_U;
        case 'V':   return OutputDevice.KEY_V;
        case 'W':   return OutputDevice.KEY_W;
        case 'X':   return OutputDevice.KEY_X;
        case 'Y':   return OutputDevice.KEY_Y;
        case 'Z':   return OutputDevice.KEY_Z;

        case '+':   return OutputDevice.PLUS;
        case '-':   return OutputDevice.MINUS;
        case '*':   return OutputDevice.STAR;
        case '/':   return OutputDevice.SLASH;
        case '<':   return OutputDevice.LOWER_THAN;
        case '>':   return OutputDevice.GREATER_THAN;
        case '=':   return OutputDevice.EQUAL;
        case '%':   return OutputDevice.PERCENT;

        case '_':   return OutputDevice.UNDERSCORE;
        case '@':   return OutputDevice.AROBASE;
        case '#':   return OutputDevice.SHARP;
        case '&':   return OutputDevice.AMPERCENT;
        case '$':   return OutputDevice.DOLLAR;
        case '\\':  return OutputDevice.BACKSLASH;
        case '|':   return OutputDevice.PIPE;
        case '~':   return OutputDevice.TILDE;
        case '²':   return OutputDevice.SQUARE;
        case '€':   return OutputDevice.EURO;
        case '£':   return OutputDevice.POUND;
        case 'µ':   return OutputDevice.MICRO;
        case '§':   return OutputDevice.PARAGRAPH;
        case '°':   return OutputDevice.DEGREE;

        case '?':   return OutputDevice.QUESTION;
        case '!':   return OutputDevice.EXCLAMATION;
        case ':':   return OutputDevice.COLUMN;
        case '.':   return OutputDevice.DOT;
        case ',':   return OutputDevice.COMA;
        case ';':   return OutputDevice.SEMI_COLUMN;
        case '\'':  return OutputDevice.QUOTE;
        case '"':   return OutputDevice.DOUBLE_QUOTE;

        case '(':   return OutputDevice.OPENING_BRACE;
        case ')':   return OutputDevice.CLOSING_BRACE;
        case '{':   return OutputDevice.OPENING_CURLY_BRACE;
        case '}':   return OutputDevice.CLOSING_CURLY_BRACE;
        case '[':   return OutputDevice.OPENING_BRACKET;
        case ']':   return OutputDevice.CLOSING_BRACKET;

        default:
            // Unrecognized character.
            return OutputDevice.NULL_KEY;
        }
    }

}
