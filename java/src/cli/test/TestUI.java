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


public class TestUI {

    public static void main(String J_Args[]) {
        // Create resources and launch a cli in a non-blocking mode.
        final cli.Cli cli_Cli = new TestTools.Cli();
        final cli.Shell cli_Shell = new cli.Shell(cli_Cli);
        cli_Shell.setStream(cli.Shell.WELCOME_STREAM, cli.OutputDevice.getNullDevice());
        cli_Shell.setStream(cli.Shell.PROMPT_STREAM, cli.OutputDevice.getNullDevice());
        final TestTools.Device cli_Device = new TestTools.Device(new cli.OutputFileDevice("TestUI.output.log"));
        cli_Shell.run(cli_Device);

        // Line
        if (true) {
            cli_Device.put("Please enter a line: ");
            cli.ui.Line cli_Line = new cli.ui.Line(cli_Shell, "", 0, 10);
            TestTools.Result cli_Result = new TestTools.Result();
            cli_Result.watchResult(cli_Line);
            cli_Line.run();
            if (cli_Result.getCtx() != null) { System.exit(-1); }
            cli_Device.onKey(cli.OutputDevice.KEY_H);
            cli_Device.onKey(cli.OutputDevice.KEY_e);
            cli_Device.onKey(cli.OutputDevice.KEY_l);
            cli_Device.onKey(cli.OutputDevice.KEY_l);
            cli_Device.onKey(cli.OutputDevice.KEY_o);
            cli_Device.onKey(cli.OutputDevice.ENTER);
            if (cli_Result.getCtx() != cli_Line) { System.exit(-1); }
            if (! cli_Line.getbExecResult()) { System.exit(-1); }
            cli_Device.put("User entered: ").put(cli_Line.getLine()).endl();
            if (! cli_Line.getLine().equals("Hello")) { System.exit(-1); }
            cli_Device.endl();
        }

        // Password
        if (true) {
            cli_Device.put("Please enter a password: ");
            cli.ui.Password cli_Password = new cli.ui.Password(cli_Shell, true, 5, 10);
            TestTools.Result cli_Result = new TestTools.Result();
            cli_Result.watchResult(cli_Password);
            cli_Password.run();
            if (cli_Result.getCtx() != null) { System.exit(-1); }
            cli_Device.onKey(cli.OutputDevice.KEY_a);
            cli_Device.onKey(cli.OutputDevice.KEY_b);
            cli_Device.onKey(cli.OutputDevice.KEY_c);
            cli_Device.onKey(cli.OutputDevice.KEY_d);
            cli_Device.onKey(cli.OutputDevice.KEY_e);
            cli_Device.onKey(cli.OutputDevice.KEY_f);
            cli_Device.onKey(cli.OutputDevice.ENTER);
            if (cli_Result.getCtx() != cli_Password) { System.exit(-1); }
            if (! cli_Password.getbExecResult()) { System.exit(-1); }
            cli_Device.put("User entered: ").put(cli_Password.getPassword()).endl();
            if (! cli_Password.getPassword().equals("abcdef")) { System.exit(-1); }
            cli_Device.endl();
        }

        // Int
        if (true) {
            cli_Device.put("Please enter an integer number: ");
            cli.ui.Int cli_Int = new cli.ui.Int(cli_Shell, 0, -10, 10);
            TestTools.Result cli_Result = new TestTools.Result();
            cli_Result.watchResult(cli_Int);
            cli_Int.run();
            if (cli_Result.getCtx() != null) { System.exit(-1); }
            cli_Device.onKey(cli.OutputDevice.KEY_UP);
            cli_Device.onKey(cli.OutputDevice.KEY_UP);
            cli_Device.onKey(cli.OutputDevice.KEY_UP);
            cli_Device.onKey(cli.OutputDevice.ENTER);
            if (cli_Result.getCtx() != cli_Int) { System.exit(-1); }
            if (! cli_Int.getbExecResult()) { System.exit(-1); }
            cli_Device.put("User entered: ").put(cli_Int.getInt()).endl();
            if (cli_Int.getInt() != 3) { System.exit(-1); }
            cli_Device.endl();
        }

        // Float
        if (true) {
            cli_Device.put("Please enter a float number: ");
            cli.ui.Float cli_Float = new cli.ui.Float(cli_Shell, 0, -10, 10);
            TestTools.Result cli_Result = new TestTools.Result();
            cli_Result.watchResult(cli_Float);
            cli_Float.run();
            if (cli_Result.getCtx() != null) { System.exit(-1); }
            cli_Device.onKey(cli.OutputDevice.DOT);
            cli_Device.onKey(cli.OutputDevice.KEY_3);
            cli_Device.onKey(cli.OutputDevice.ENTER);
            if (cli_Result.getCtx() != cli_Float) { System.exit(-1); }
            if (! cli_Float.getbExecResult()) { System.exit(-1); }
            cli_Device.put("User entered: ").put(cli_Float.getFloat()).endl();
            if (cli_Float.getFloat() != 0.3) { System.exit(-1); }
            cli_Device.endl();
        }

        // YesNo
        if (true) {
            cli.ui.YesNo cli_YesNo = new cli.ui.YesNo(cli_Shell, true);

            cli_Shell.setLang(cli.ResourceString.LANG_EN);
            cli_Device.put("Answer [YES/no]: ");
            TestTools.Result cli_Result_EN = new TestTools.Result();
            cli_Result_EN.watchResult(cli_YesNo);
            cli_YesNo.run();
            if (cli_Result_EN.getCtx() != null) { System.exit(-1); }
            cli_Device.onKey(cli.OutputDevice.KEY_n);
            cli_Device.onKey(cli.OutputDevice.ENTER);
            if (cli_Result_EN.getCtx() != cli_YesNo) { System.exit(-1); }
            if (! cli_YesNo.getbExecResult()) { System.exit(-1); }
            cli_Device.put("User entered: ").put(new Boolean(cli_YesNo.getYesNo()).toString()).put("/").put(cli_YesNo.getstrChoice().getString(cli_Shell.getLang())).endl();
            if (cli_YesNo.getYesNo() != false) { System.exit(-1); }

            cli_Shell.setLang(cli.ResourceString.LANG_FR);
            cli_Device.put("Répondez [OUI/non] : ");
            TestTools.Result cli_Result_FR = new TestTools.Result();
            cli_Result_FR.watchResult(cli_YesNo);
            cli_YesNo.run();
            if (cli_Result_FR.getCtx() != null) { System.exit(-1); }
            cli_Device.onKey(cli.OutputDevice.KEY_o);
            cli_Device.onKey(cli.OutputDevice.ENTER);
            if (cli_Result_FR.getCtx() != cli_YesNo) { System.exit(-1); }
            if (! cli_YesNo.getbExecResult()) { System.exit(-1); }
            cli_Device.put("User entered: ").put(new Boolean(cli_YesNo.getYesNo()).toString()).put("/").put(cli_YesNo.getstrChoice().getString(cli_Shell.getLang())).endl();
            if (cli_YesNo.getYesNo() != true) { System.exit(-1); }
    
            cli_Device.endl();
        }

        // Choice
        if (true) {
            java.util.Vector<cli.ResourceString> j_Choices = new java.util.Vector<cli.ResourceString>();
            j_Choices.add(new cli.ResourceString().setString(cli.ResourceString.LANG_EN, "choice#1").setString(cli.ResourceString.LANG_FR, "choix 1"));
            j_Choices.add(new cli.ResourceString().setString(cli.ResourceString.LANG_EN, "choice#2").setString(cli.ResourceString.LANG_FR, "choix 2"));
            cli.ui.Choice cli_Choice = new cli.ui.Choice(cli_Shell, 0, j_Choices);

            cli_Shell.setLang(cli.ResourceString.LANG_EN);
            cli_Device.put("Please make a choice: ");
            TestTools.Result cli_Result_EN = new TestTools.Result();
            cli_Result_EN.watchResult(cli_Choice);
            cli_Choice.run();
            if (cli_Result_EN.getCtx() != null) { System.exit(-1); }
            cli_Device.onKey(cli.OutputDevice.KEY_DOWN);
            cli_Device.onKey(cli.OutputDevice.ENTER);
            if (cli_Result_EN.getCtx() != cli_Choice) { System.exit(-1); }
            if (! cli_Choice.getbExecResult()) { System.exit(-1); }
            cli_Device.put("User entered: ").put(cli_Choice.getChoice()).put("/").put(cli_Choice.getstrChoice().getString(cli_Shell.getLang())).endl();
            if (cli_Choice.getChoice() != 1) { System.exit(-1); }

            cli_Shell.setLang(cli.ResourceString.LANG_FR);
            cli_Device.put("Merci de faire un choix : ");
            TestTools.Result cli_Result_FR = new TestTools.Result();
            cli_Result_FR.watchResult(cli_Choice);
            cli_Choice.run();
            if (cli_Result_FR.getCtx() != null) { System.exit(-1); }
            cli_Device.onKey(cli.OutputDevice.ESCAPE);
            if (cli_Result_FR.getCtx() != cli_Choice) { System.exit(-1); }
            if (cli_Choice.getbExecResult()) { System.exit(-1); }
            cli_Device.put("User entered: ").put(cli_Choice.getChoice()).put("/").put(cli_Choice.getstrChoice().getString(cli_Shell.getLang())).endl();
            cli_Device.endl();
        }

        // More
        if (true) {
            cli.ui.More cli_More = new cli.ui.More(cli_Shell);
            cli.test.UISampleText.fillText(cli_More.getText(), 10);
            TestTools.Result cli_Result = new TestTools.Result();
            cli_Result.watchResult(cli_More);
            cli_More.run();
            if (cli_Result.getCtx() != null) { System.exit(-1); }
            cli_Device.onKey(cli.OutputDevice.KEY_q);
            if (cli_Result.getCtx() != cli_More) { System.exit(-1); }
            cli_Device.endl();
        }

        // Less
        if (true) {
            cli.ui.Less cli_Less = new cli.ui.Less(cli_Shell);
            cli.test.UISampleText.fillText(cli_Less.getText(), 10);
            TestTools.Result cli_Result = new TestTools.Result();
            cli_Result.watchResult(cli_Less);
            cli_Less.run();
            if (cli_Result.getCtx() != null) { System.exit(-1); }
            cli_Device.onKey(cli.OutputDevice.BREAK);
            if (cli_Result.getCtx() != cli_Less) { System.exit(-1); }
            cli_Device.endl();
        }

        // One last test to check handler setting through an anonymous class derived from the cli.ExecutionResult interface (for user-guide example).
        if (true) {
            cli_Device.put("Enter line: ");
            final cli.ui.Line ui_Line = new cli.ui.Line(cli_Shell, "", 0, 1024);
            ui_Line.watchResult(
                new cli.ExecutionResult() {
                    protected void onResult(cli.ExecutionContext.Interface CLI_Context) {
                        cli_Device.put("End of execution: ").put(ui_Line.getLine()).endl();
                    }
                }
            );
            ui_Line.run();
            cli_Device.onKey(cli.OutputDevice.KEY_h);
            cli_Device.onKey(cli.OutputDevice.KEY_e);
            cli_Device.onKey(cli.OutputDevice.KEY_l);
            cli_Device.onKey(cli.OutputDevice.KEY_l);
            cli_Device.onKey(cli.OutputDevice.KEY_o);
            cli_Device.onKey(cli.OutputDevice.ENTER);
        }

        // Eventually quit the shell.
        cli_Shell.quit();

        runFinalizers();
    }

    @SuppressWarnings("deprecation")
    private static void runFinalizers() {
        // Generates a warning but whatever!
        // The aim here is to check that executing all finalizers does not cause a crash.
        System.runFinalizersOnExit(true);
        // Note: Exception are not thrown anymore when finalizing objects on exit.
    }
}
