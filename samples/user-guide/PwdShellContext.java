/** Authentication for a shell access. */
class PwdShellContext extends cli.ExecutionContext.Java
{
    /** Constructor. */
    public PwdShellContext(cli.Cli CLI_Cli, String J_Pwd) {
        super();                                            // Create top execution context.
        m_jPwd = J_Pwd;
        m_cliPwd = new cli.ui.Password(this, true, -1, -1); // Create password child execution context.
        m_cliShell = new cli.Shell(this, CLI_Cli);          // Create shell child execution context.
        m_cliResult = new PwdShellResult();
        m_iPwdAttempsLeft = MAX_PWD_ATTEMPTS;
    }
    /** Password control accessor. */
    public cli.ui.Password getPwdControl() { return m_cliPwd; }
    /** Shell accessor. */
    public cli.Shell getShell() { return m_cliShell; }
    /** onStartExecution(): start the password control. */
    protected boolean onStartExecution() {
        getStream(cli.ExecutionContext.OUTPUT_STREAM).put("Enter password: ");
        m_cliResult.watchResult(m_cliPwd);
        m_cliPwd.run();
        return true;
    }
    /** onStopExecution(): called when either the user has typed too many wrong passwords, or when the shell is done.. */
    protected boolean onStopExecution() { return true; }
    /** onKey(): should not happen. */
    protected void onKey(int E_Key) {}

    /** Receives notifications when a child execution context is done. */
    private class PwdShellResult extends cli.ExecutionResult {
        protected void onResult(cli.ExecutionContext.Interface CLI_ExecutionContext) {
            if (isRunning()) {
                // When this is a notification from the password control...
                if (CLI_ExecutionContext == m_cliPwd) {
                    if (m_cliPwd.getbExecResult()) {
                        if (m_iPwdAttempsLeft > 0) {
                            // Decrement the password attempts left at first.
                            m_iPwdAttempsLeft --;
                            if (m_cliPwd.getPassword().equals(m_jPwd)) {
                                // Password OK: restore the password attempts left and run the shell.
                                m_iPwdAttempsLeft = MAX_PWD_ATTEMPTS;
                                m_cliResult.watchResult(m_cliShell);
                                m_cliShell.run();
                                return;
                            } else {
                                // Wrong password
                                if (m_cliPwd.getPassword().length() <= 0) {
                                    m_iPwdAttempsLeft ++;
                                } else {
                                    getStream(cli.ExecutionContext.ERROR_STREAM).put("Wrong password!").endl();
                                }
                                if (m_iPwdAttempsLeft > 0) {
                                    // Let the user enter his password again.
                                    onStartExecution();
                                    return;
                                }
                            }
                        }
                    }
                }
                // Default behaviour.
                stopExecution();
            }
        }
    }

    private final int MAX_PWD_ATTEMPTS = 3;
    private final String m_jPwd;
    private final cli.ui.Password m_cliPwd;
    private final cli.Shell m_cliShell;
    private final PwdShellResult m_cliResult;
    private int m_iPwdAttempsLeft;
}
