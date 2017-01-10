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


#include "cli/pch.h"

#include "cli/exec_context.h"

#include "cli_ExecutionResult.h"

#include "NativeObject.h"
#include "NativeExec.h"
#include "NativeTraces.h"


//! @brief Class implementing native C++ objects matching with cli.ExecutionResult derived class.
class NativeExecutionResult : public cli::ExecutionResult
{
public:
    //! @brief Default constructor.
    explicit NativeExecutionResult(void)
      : cli::ExecutionResult()
    {
    }

    //! @brief Destructor.
    virtual ~NativeExecutionResult(void)
    {
    }

private:
    //! @brief No copy constructor.
    NativeExecutionResult(const NativeExecutionResult&);
    //! @brief No assignment operator.
    NativeExecutionResult& operator=(const NativeExecutionResult&);

protected:
    // cli::ExecutionResult implementation.

    virtual void OnResult(const cli::ExecutionContext& CLI_Context)
    {
        cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("NativeExecutionResult::OnResult(CLI_Context)") << cli::endl;
        cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt("CLI_Context", NativeObject::GetNativeRef(CLI_Context)) << cli::endl;

        cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ValueInt("this", NativeObject::GetNativeRef(*this)) << cli::endl;
        
        if (JNIEnv* const pj_Env = NativeExec::GetInstance().GetJNIEnv())
        {
            // Java handler call.
            if (const jclass pj_ResultClass = pj_Env->FindClass(NativeObject::GetJavaClassName(*this).c_str()))
            {
                cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ValuePtr("pj_ResultClass", pj_ResultClass) << cli::endl;
                if (const jmethodID pj_OnResultMethodID = pj_Env->GetMethodID(pj_ResultClass, "__onResult", "(I)V"))
                {
                    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ValuePtr("pj_OnResultMethodID", pj_OnResultMethodID) << cli::endl;
                    if (const jobject pj_Object = NativeObject::GetJavaObject(*this, true))
                    {
                        cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ValuePtr("pj_Object", pj_Object) << cli::endl;
                        pj_Env->CallVoidMethod(pj_Object, pj_OnResultMethodID, (jint) NativeObject::GetNativeRef(CLI_Context));
                    }
                }
            }
        }

        cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndVoid("NativeExecutionResult::OnResult()") << cli::endl;
    }
};


extern "C" JNIEXPORT jint JNICALL Java_cli_ExecutionResult__1_1ExecutionResult(
        JNIEnv* PJ_Env, jclass PJ_Class)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("ExecutionResult.__ExecutionResult()") << cli::endl;
    NativeObject::REF i_ResultRef = 0;
    if (cli::ExecutionResult* const pcli_Result = new NativeExecutionResult())
    {
        NativeObject::Use(*pcli_Result);
        i_ResultRef = NativeObject::GetNativeRef(*pcli_Result);
    }
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndInt("ExecutionResult.__ExecutionResult()", i_ResultRef) << cli::endl;
    return i_ResultRef;
}

extern "C" JNIEXPORT void JNICALL Java_cli_ExecutionResult__1_1watchResult(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeResultRef, jint I_NativeContextRef)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("ExecutionContext.Common.__watchResult(I_NativeResultRef, I_NativeContextRef)") << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt("I_NativeResultRef", I_NativeResultRef) << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt("I_NativeContextRef", I_NativeContextRef) << cli::endl;
    if (cli::ExecutionResult* const pcli_Result = NativeObject::GetNativeObject<cli::ExecutionResult*>(I_NativeResultRef))
    {
        if (cli::ExecutionContext* const pcli_Context = NativeObject::GetNativeObject<cli::ExecutionContext*>(I_NativeContextRef))
        {
            pcli_Result->WatchResult(*pcli_Context);
        }
    }
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndVoid("ExecutionContext.Common.__watchResult()") << cli::endl;
}
