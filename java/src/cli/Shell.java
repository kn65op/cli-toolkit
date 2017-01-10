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


/** Shell class.
    A shell basically runs a command line interface with the following arguments at the minimum:

        # a Cli object which defines a set of command lines and the execution for each of them;

        # an IODevice used to receive the input characters and to print out characters. */
public class Shell extends ExecutionContext.Native {

    // Constant definition relay for CLI 2.7 backward compatibility.
    /** All streams constant. */
    public static final int ALL_STREAMS = ExecutionContext.ALL_STREAMS;
    /** Welcome stream constant. Useful for bye too. */
    public static final int WELCOME_STREAM = ExecutionContext.WELCOME_STREAM;
    /** Prompt stream constant. */
    public static final int PROMPT_STREAM = ExecutionContext.PROMPT_STREAM;
    /** Echo stream constant. */
    public static final int ECHO_STREAM = ExecutionContext.ECHO_STREAM;
    /** Output stream constant. */
    public static final int OUTPUT_STREAM = ExecutionContext.OUTPUT_STREAM;
    /** Error stream constant. */
    public static final int ERROR_STREAM = ExecutionContext.ERROR_STREAM;
    /** Number of stream types. */
    public static final int STREAM_TYPES_COUNT = ExecutionContext.STREAM_TYPES_COUNT;

    /** Top execution context constructor.
        @param CLI_Cli Cli structure the shell will run the text based interface for. */
    public Shell(Cli CLI_Cli) {
        super(__Shell(0, CLI_Cli.getNativeRef()));
    }
    /** Child execution context constructor.
        @param CLI_ParentContext Parent execution context.
        @param CLI_Cli Cli structure the shell will run the text based interface for. */
    public Shell(ExecutionContext.Interface CLI_ParentContext, Cli CLI_Cli) {
        super(__Shell(CLI_ParentContext.getNativeRef(), CLI_Cli.getNativeRef()));
    }
    private static final native int __Shell(int I_NativeParentContextRef, int I_NativeCliRef);

    /** Cli accessor.
        @return The CLI instance the shell is running with. */
    public final Cli getCli() {
        return (Cli) NativeObject.getObject(__getCli(this.getNativeRef()));
    }
    private static final native int __getCli(int I_NativeShellRef);

    /** Welcome message setting.
        @param CLI_WelcomeMessage Welcome message. When an empty string is given, the default welcome message is restored. */
    public final void setWelcomeMessage(ResourceString CLI_WelcomeMessage) {
        __setWelcomeMessage(this.getNativeRef(), CLI_WelcomeMessage.getNativeRef());
    }
    private static final native void __setWelcomeMessage(int I_NativeShellRef, int I_NativeWelcomeMessageRef);

    /** Bye message setting.
        @param CLI_ByeMessage Bye message. When an empty string is given, the default bye message is restored. */
    public final void setByeMessage(ResourceString CLI_ByeMessage) {
        __setByeMessage(this.getNativeRef(), CLI_ByeMessage.getNativeRef());
    }
    private static final native void __setByeMessage(int I_NativeShellRef, int I_NativeByeMessageRef);

    /** Prompt message positionning.
        @param CLI_Prompt Prompt string. When an empty string is given, the default prompt (depending on the current menu) is restored. */
    public final void setPrompt(ResourceString CLI_Prompt) {
        __setPrompt(this.getNativeRef(), CLI_Prompt.getNativeRef());
    }
    private static final native void __setPrompt(int I_NativeShellRef, int I_NativePromptRef);

    /** Help margin accessor.
        @return Number of spaces for the help margin. */
    public final int getHelpMargin() {
        return __getHelpMargin(this.getNativeRef());
    }
    private static final native int __getHelpMargin(int I_NativeShellRef);

    /** Help offset accessor.
        @return Number of spaces for the help offset. */
    public final int getHelpOffset() {
        return __getHelpOffset(this.getNativeRef());
    }
    private static final native int __getHelpOffset(int I_NativeShellRef);

    /** Current menu retrieval.
        @param I_MenuIndex Index of the menu in the stack.
                            0: root menu (bottom of the stack).
                            1: menu stacked over the root menu.
                            2: menu stacked over again...
                            -1: current menu (top of the stack)
        @return Reference of the current menu. null when I_MenuIndex is out of bounds. */
    public Menu getCurrentMenu(int I_MenuIndex) {
        return (Menu) NativeObject.getObject(__getCurrentMenu(this.getNativeRef(), I_MenuIndex));
    }
    private static final native int __getCurrentMenu(int I_NativeShellRef, int I_MenuIndex);

    /** Enter a menu.
        @param CLI_Menu Menu to enter. */
    public final void enterMenu(Menu CLI_Menu, boolean B_PromptMenu) {
        if (CLI_Menu != null) {
            __enterMenu(this.getNativeRef(), CLI_Menu.getNativeRef(), B_PromptMenu);
        }
    }
    private static final native void __enterMenu(int I_NativeShellRef, int I_NativeMenuRef, boolean B_PromptMenu);

    /** Exits the current menu. */
    public final void exitMenu(boolean B_PromptMenu) {
        __exitMenu(this.getNativeRef(), B_PromptMenu);
    }
    private static final native void __exitMenu(int I_NativeShellRef, boolean B_PromptMenu);

    /** Terminates the shell.
        Caution! Not thread safe. Call quitThreadSafe() to do so. */
    public final void quit() {
        __quit(this.getNativeRef());
    }
    private static final native void __quit(int I_NativeShellRef);

    /** Displays help depending on the context of the current line. */
    public final void displayHelp() {
        __displayHelp(this.getNativeRef());
    }
    private static final native void __displayHelp(int I_NativeShellRef);

    /** Prints the working menu. */
    public final void printWorkingMenu() {
        __printWorkingMenu(this.getNativeRef());
    }
    private static final native void __printWorkingMenu(int I_NativeShellRef);

    /** Cleans the screen. */
    public final void cleanScreen(boolean B_PromptMenu) {
        __cleanScreen(this.getNativeRef(), B_PromptMenu);
    }
    private static final native void __cleanScreen(int I_NativeShellRef, boolean B_PromptMenu);

}
