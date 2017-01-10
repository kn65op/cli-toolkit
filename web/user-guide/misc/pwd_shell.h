#include "cli/exec_context.h"
#include "cli/ui_password.h"
#include "cli/shell.h"

//! @brief Authentication for a shell access.
class PwdShellContext : public cli::ExecutionContext, public cli::ExecutionResult
{
public:
    //! @brief Constructor.
    PwdShellContext(const cli::Cli&amp; CLI_Cli, const char* const STR_Pwd)
          : cli::ExecutionContext(),                                    // Create top execution context.
            m_tkPwd(256, STR_Pwd), m_uiPwdAttempsLeft(MAX_PWD_ATTEMPTS)
    {
        m_pcliPwd = new cli::ui::Password(*this, true, -1, -1);         // Create password child execution context.
        m_pcliShell = new cli::Shell(*this, CLI_Cli);                   // Create shell child execution context.
    }
    //! @brief Destructor.
    virtual ~PwdShellContext(void) {
        delete m_pcliPwd;
        delete m_pcliShell;
    }
public:
    //! @brief Password control accessor.
    cli::ui::Password&amp; GetPwdControl(void) { return *m_pcliPwd; }
    //! @brief Shell accessor.
    cli::Shell&amp; GetShell(void) { return *m_pcliShell; }
protected:
    //! @brief OnStartExecution(): start the password control.
    virtual const bool OnStartExecution(void) {
        GetStream(cli::OUTPUT_STREAM) &lt;&lt; "Enter password: ";
        WatchResult(*m_pcliPwd);
        m_pcliPwd->Run();
        return true;
    }
    //! @brief Receives notifications when a child execution context is done.
    virtual void OnResult(const cli::ExecutionContext&amp; CLI_ExecutionContext) {
        if (IsRunning()) {
            // When this is a notification from the password control...
            if (&amp; CLI_ExecutionContext == m_pcliPwd) {
                if (m_pcliPwd->GetbExecResult()) {
                    if (m_uiPwdAttempsLeft > 0) {
                        // Decrement the password attempts left at first.
                        m_uiPwdAttempsLeft --;
                        if (m_pcliPwd->GetPassword() == m_tkPwd) {
                            // Password OK: restore the password attempts left and run the shell.
                            m_uiPwdAttempsLeft = MAX_PWD_ATTEMPTS;
                            WatchResult(*m_pcliShell);
                            m_pcliShell->Run();
                            return;
                        } else {
                            // Wrong password
                            if (m_pcliPwd->GetPassword().IsEmpty()) {
                                m_uiPwdAttempsLeft ++;
                            } else {
                                GetStream(cli::ERROR_STREAM) &lt;&lt; "Wrong password!" &lt;&lt; cli::endl;
                            }
                            if (m_uiPwdAttempsLeft > 0) {
                                // Let the user enter his password again.
                                PwdShellContext::OnStartExecution();
                                return;
                            }
                        }
                    }
                }
            }
            // Default behaviour.
            ExecutionContext::StopExecution();
        }
    }
    //! @brief OnStopExecution(): called when either the user has typed too many wrong passwords, or when the shell is done.. */
    virtual const bool OnStopExecution(void) { return true; }
    //! @brief OnKey(): should not happen.
    virtual void OnKey(const cli::KEY E_Key) {}
private:
    cli::ui::Password* m_pcliPwd;
    cli::Shell* m_pcliShell;
    cli::tk::String m_tkPwd;
    enum { MAX_PWD_ATTEMPTS = 3 };
    unsigned int m_uiPwdAttempsLeft;
};
