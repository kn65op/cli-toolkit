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
#include "cli/io_device.h"

#include "cli_ExecutionContext.h"

#include "NativeObject.h"
#include "NativeExec.h"
#include "NativeTraces.h"


// NonBlockingIODevice.Common implementation.

extern "C" JNIEXPORT jint JNICALL Java_cli_ExecutionContext__1_1Common_1_1getInput(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeContextRef)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("ExecutionContext.Common.__getInput(I_NativeContextRef)") << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt("I_NativeContextRef", I_NativeContextRef) << cli::endl;
    NativeObject::REF i_InputRef = 0;
    if (const cli::ExecutionContext* const pcli_Context = NativeObject::GetNativeObject<const cli::ExecutionContext*>(I_NativeContextRef))
    {
        const cli::IODevice& cli_Input = pcli_Context->GetInput();
        i_InputRef = NativeObject::GetNativeRef(cli_Input);
    }
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndInt("ExecutionContext.Common.__getInput()", i_InputRef) << cli::endl;
    return i_InputRef;
}

extern "C" JNIEXPORT jint JNICALL Java_cli_ExecutionContext__1_1Common_1_1getStream(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeContextRef, jint E_StreamType)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("ExecutionContext.Common.__getStream(I_NativeContextRef, E_StreamType)") << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt("I_NativeContextRef", I_NativeContextRef) << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt("E_StreamType", E_StreamType) << cli::endl;
    NativeObject::REF i_StreamRef = 0;
    if (const cli::ExecutionContext* const pcli_Context = NativeObject::GetNativeObject<const cli::ExecutionContext*>(I_NativeContextRef))
    {
        const cli::OutputDevice& cli_Stream = pcli_Context->GetStream((cli::STREAM_TYPE) E_StreamType);
        i_StreamRef = NativeObject::GetNativeRef(cli_Stream);
    }
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndInt("ExecutionContext.Common.__getStream()", i_StreamRef) << cli::endl;
    return i_StreamRef;
}

extern "C" JNIEXPORT jboolean JNICALL Java_cli_ExecutionContext__1_1Common_1_1setStream(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeContextRef, jint E_StreamType, jint I_NativeDeviceRef)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("ExecutionContext.Common.__setStream(I_NativeContextRef, E_StreamType, I_NativeDeviceRef)") << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt("I_NativeContextRef", I_NativeContextRef) << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt("E_StreamType", E_StreamType) << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt("I_NativeDeviceRef", I_NativeDeviceRef) << cli::endl;
    bool b_Res = false;
    if (cli::ExecutionContext* const pcli_Context = NativeObject::GetNativeObject<cli::ExecutionContext*>(I_NativeContextRef))
    {
        if (((E_StreamType >= 0) && (E_StreamType < cli::STREAM_TYPES_COUNT))
            || (E_StreamType == cli::ALL_STREAMS))
        {
            if (cli::OutputDevice* const pcli_Device = NativeObject::GetNativeObject<cli::OutputDevice*>(I_NativeDeviceRef))
            {
                b_Res = pcli_Context->SetStream((cli::STREAM_TYPE) E_StreamType, *pcli_Device);
            }
        }
    }
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndBool("ExecutionContext.Common.__setStream()", b_Res) << cli::endl;
    return b_Res;
}

extern "C" JNIEXPORT jboolean JNICALL Java_cli_ExecutionContext__1_1Common_1_1streamEnabled(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeContextRef, jint E_StreamType)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("ExecutionContext.Common.__streamEnabled(I_NativeContextRef, E_StreamType)") << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt("I_NativeContextRef", I_NativeContextRef) << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt("E_StreamType", E_StreamType) << cli::endl;
    bool b_Res = false;
    if (const cli::ExecutionContext* const pcli_Context = NativeObject::GetNativeObject<const cli::ExecutionContext*>(I_NativeContextRef))
    {
        b_Res = pcli_Context->StreamEnabled((cli::STREAM_TYPE) E_StreamType);
    }
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndBool("ExecutionContext.Common.__streamEnabled()", b_Res) << cli::endl;
    return b_Res;
}

extern "C" JNIEXPORT jboolean JNICALL Java_cli_ExecutionContext__1_1Common_1_1enableStream(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeContextRef, jint E_StreamType, jboolean B_Enable)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("ExecutionContext.Common.__enableStream(I_NativeContextRef, E_StreamType, B_Enable)") << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt("I_NativeContextRef", I_NativeContextRef) << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt("E_StreamType", E_StreamType) << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamBool("B_Enable", B_Enable) << cli::endl;
    bool b_Res = false;
    if (cli::ExecutionContext* const pcli_Context = NativeObject::GetNativeObject<cli::ExecutionContext*>(I_NativeContextRef))
    {
        b_Res = pcli_Context->EnableStream((cli::STREAM_TYPE) E_StreamType, B_Enable);
    }
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndBool("ExecutionContext.Common.__enableStream()", b_Res) << cli::endl;
    return b_Res;
}

extern "C" JNIEXPORT void JNICALL Java_cli_ExecutionContext__1_1Common_1_1setLang(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeContextRef, jint E_Lang)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("ExecutionContext.Common.__setLang(I_NativeContextRef, E_Lang)") << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt("I_NativeContextRef", I_NativeContextRef) << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt("E_Lang", E_Lang) << cli::endl;
    if (cli::ExecutionContext* const pcli_Context = NativeObject::GetNativeObject<cli::ExecutionContext*>(I_NativeContextRef))
    {
        if ((E_Lang >= 0) && (E_Lang < cli::ResourceString::LANG_COUNT))
        {
            pcli_Context->SetLang((cli::ResourceString::LANG) E_Lang);
        }
    }
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndVoid("ExecutionContext.Common.__setLang()") << cli::endl;
}

extern "C" JNIEXPORT jint JNICALL Java_cli_ExecutionContext__1_1Common_1_1getLang(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeContextRef)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("ExecutionContext.Common.__getLang(I_NativeContextRef)") << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt("I_NativeContextRef", I_NativeContextRef) << cli::endl;
    cli::ResourceString::LANG e_Lang = cli::ResourceString::LANG_DEFAULT;
    if (const cli::ExecutionContext* const pcli_Context = NativeObject::GetNativeObject<const cli::ExecutionContext*>(I_NativeContextRef))
    {
        e_Lang = pcli_Context->GetLang();
    }
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndInt("ExecutionContext.Common.__getLang()", e_Lang) << cli::endl;
    return e_Lang;
}

extern "C" JNIEXPORT void JNICALL Java_cli_ExecutionContext__1_1Common_1_1setBeep(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeContextRef, jboolean B_Enable)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("ExecutionContext.Common.__setBeep(I_NativeContextRef, B_Enable)") << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt("I_NativeContextRef", I_NativeContextRef) << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamBool("B_Enable", B_Enable) << cli::endl;
    if (cli::ExecutionContext* const pcli_Context = NativeObject::GetNativeObject<cli::ExecutionContext*>(I_NativeContextRef))
    {
        pcli_Context->SetBeep(B_Enable);
    }
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndVoid("ExecutionContext.Common.__setBeep()") << cli::endl;
}

extern "C" JNIEXPORT jboolean JNICALL Java_cli_ExecutionContext__1_1Common_1_1getBeep(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeContextRef)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("ExecutionContext.Common.__getBeep(I_NativeContextRef)") << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt("I_NativeContextRef", I_NativeContextRef) << cli::endl;
    bool b_Beep = false;
    if (const cli::ExecutionContext* const pcli_Context = NativeObject::GetNativeObject<const cli::ExecutionContext*>(I_NativeContextRef))
    {
        b_Beep = pcli_Context->GetBeep();
    }
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndBool("ExecutionContext.Common.__getBeep()", b_Beep) << cli::endl;
    return b_Beep;
}

extern "C" JNIEXPORT jboolean JNICALL Java_cli_ExecutionContext__1_1Common_1_1run(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeContextRef, jint I_NativeIODeviceRef)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("ExecutionContext.Common.__run(I_NativeContextRef, I_NativeIODeviceRef)") << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt("I_NativeContextRef", I_NativeContextRef) << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt("I_NativeIODeviceRef", I_NativeIODeviceRef) << cli::endl;
    bool b_Res = false;
    if (cli::ExecutionContext* const pcli_Context = NativeObject::GetNativeObject<cli::ExecutionContext*>(I_NativeContextRef))
    {
        if (cli::IODevice* const pcli_IODevice = NativeObject::GetNativeObject<cli::IODevice*>(I_NativeIODeviceRef))
        {
            pcli_Context->Run(*pcli_IODevice);
            b_Res = true;
        }
    }
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndBool("ExecutionContext.Common.__run()", b_Res) << cli::endl;
    return b_Res;
}

extern "C" JNIEXPORT jboolean JNICALL Java_cli_ExecutionContext__1_1Common_1_1isRunning(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeContextRef)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("ExecutionContext.Common.__isRunning(I_NativeContextRef)") << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt("I_NativeContextRef", I_NativeContextRef) << cli::endl;
    bool b_IsRunning = false;
    if (const cli::ExecutionContext* const pcli_Context = NativeObject::GetNativeObject<const cli::ExecutionContext*>(I_NativeContextRef))
    {
        b_IsRunning = pcli_Context->IsRunning();
    }
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndBool("ExecutionContext.Common.__isRunning()", b_IsRunning) << cli::endl;
    return b_IsRunning;
}

extern "C" JNIEXPORT void JNICALL Java_cli_ExecutionContext__1_1Common_1_1stopExecution(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeContextRef)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("ExecutionContext.Common.__stopExecution(I_NativeContextRef)") << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt("I_NativeContextRef", I_NativeContextRef) << cli::endl;
    if (cli::ExecutionContext* const pcli_Context = NativeObject::GetNativeObject<cli::ExecutionContext*>(I_NativeContextRef))
    {
        pcli_Context->StopExecution();
    }
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndVoid("ExecutionContext.Common.__stopExecution()") << cli::endl;
}

extern "C" JNIEXPORT void JNICALL Java_cli_ExecutionContext__1_1Common_1_1stopAllExecutions(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeContextRef)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("ExecutionContext.Common.__stopAllExecutions(I_NativeContextRef)") << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt("I_NativeContextRef", I_NativeContextRef) << cli::endl;
    if (cli::ExecutionContext* const pcli_Context = NativeObject::GetNativeObject<cli::ExecutionContext*>(I_NativeContextRef))
    {
        pcli_Context->StopAllExecutions();
    }
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndVoid("ExecutionContext.Common.__stopAllExecutions()") << cli::endl;
}


// NonBlockingIODevice.Java implementation.

//! @brief Class implementing native C++ objects matching with cli.ExecutionContext.Java derived class.
class NativeExecutionContext : public cli::ExecutionContext
{
public:
    //! @brief Top context constructor.
    explicit NativeExecutionContext(void)
      : cli::ExecutionContext()
    {
    }

    //! @brief Child context constructor.
    //!
    //! Makes this execution context run in the same context as the one given for parent.
    explicit NativeExecutionContext(
            ExecutionContext& CLI_ParentContext //!< Parent execution context.
            )
      : cli::ExecutionContext(CLI_ParentContext)
    {
    }

    //! @brief Destructor.
    virtual ~NativeExecutionContext(void)
    {
    }

private:
    //! @brief No copy constructor.
    NativeExecutionContext(const NativeExecutionContext&);
    //! @brief No assignment operator.
    NativeExecutionContext& operator=(const NativeExecutionContext&);

protected:
    // cli::ExecutionContext implementation.

    virtual const bool OnStartExecution(void)
    {
        cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("NativeExecutionContext::OnStartExecution()") << cli::endl;

        bool b_Res = false;
        if (JNIEnv* const pj_Env = NativeExec::GetInstance().GetJNIEnv())
        {
            // Java handler call.
            if (const jclass pj_ContextClass = pj_Env->FindClass(NativeObject::GetJavaClassName(*this).c_str()))
            {
                cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ValuePtr("pj_ContextClass", pj_ContextClass) << cli::endl;
                if (const jmethodID pj_OnStartExecutionMethodID = pj_Env->GetMethodID(pj_ContextClass, "__onStartExecution", "()Z"))
                {
                    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ValuePtr("pj_OnStartExecutionMethodID", pj_OnStartExecutionMethodID) << cli::endl;
                    if (const jobject pj_Object = NativeObject::GetJavaObject(*this, true))
                    {
                        cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ValuePtr("pj_Object", pj_Object) << cli::endl;
                        b_Res = pj_Env->CallBooleanMethod(pj_Object, pj_OnStartExecutionMethodID);
                    }
                }
            }
        }

        cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndBool("NativeExecutionContext::OnStartExecution()", b_Res) << cli::endl;
        return b_Res;
    }

    virtual void OnKey(const cli::KEY E_KeyCode)
    {
        cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("NativeExecutionContext::OnKey(E_KeyCode)") << cli::endl;
        cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt("E_KeyCode", E_KeyCode) << cli::endl;

        if (JNIEnv* const pj_Env = NativeExec::GetInstance().GetJNIEnv())
        {
            // Java handler call.
            if (const jclass pj_ContextClass = pj_Env->FindClass(NativeObject::GetJavaClassName(*this).c_str()))
            {
                cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ValuePtr("pj_ContextClass", pj_ContextClass) << cli::endl;
                if (const jmethodID pj_OnKeyMethodID = pj_Env->GetMethodID(pj_ContextClass, "__onKey", "(I)V"))
                {
                    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ValuePtr("pj_OnKeyMethodID", pj_OnKeyMethodID) << cli::endl;
                    if (const jobject pj_Object = NativeObject::GetJavaObject(*this, true))
                    {
                        cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ValuePtr("pj_Object", pj_Object) << cli::endl;
                        pj_Env->CallVoidMethod(pj_Object, pj_OnKeyMethodID, (jint) E_KeyCode);
                    }
                }
            }
        }

        cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndVoid("NativeExecutionContext::OnKey()") << cli::endl;
    }

    virtual const bool OnStopExecution(void)
    {
        cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("NativeExecutionContext::OnStopExecution()") << cli::endl;

        bool b_Res = false;
        if (JNIEnv* const pj_Env = NativeExec::GetInstance().GetJNIEnv())
        {
            // Java handler call.
            if (const jclass pj_ContextClass = pj_Env->FindClass(NativeObject::GetJavaClassName(*this).c_str()))
            {
                cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ValuePtr("pj_ContextClass", pj_ContextClass) << cli::endl;
                if (const jmethodID pj_OnStopExecutionMethodID = pj_Env->GetMethodID(pj_ContextClass, "__onStopExecution", "()Z"))
                {
                    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ValuePtr("pj_OnStopExecutionMethodID", pj_OnStopExecutionMethodID) << cli::endl;
                    if (const jobject pj_Object = NativeObject::GetJavaObject(*this, true))
                    {
                        cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ValuePtr("pj_Object", pj_Object) << cli::endl;
                        b_Res = pj_Env->CallBooleanMethod(pj_Object, pj_OnStopExecutionMethodID);
                    }
                }
            }
        }

        cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndBool("NativeExecutionContext::OnStopExecution()", b_Res) << cli::endl;
        return b_Res;
    }
};


extern "C" JNIEXPORT jint JNICALL Java_cli_ExecutionContext__1_1Java_1_1ExecutionContext(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeParentContextRef)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("ExecutionContext.Java.__Execution(I_NativeParentContextRef)") << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt("I_NativeParentContextRef", I_NativeParentContextRef) << cli::endl;
    NativeObject::REF i_ContextRef = 0;
    cli::ExecutionContext* pcli_Context = NULL;
    if (I_NativeParentContextRef != 0)
    {
        if (cli::ExecutionContext* const pcli_ParentContext = NativeObject::GetNativeObject<cli::ExecutionContext*>(I_NativeParentContextRef))
        {
            pcli_Context = new NativeExecutionContext(*pcli_ParentContext);
        }
    }
    else
    {
        pcli_Context = new NativeExecutionContext();
    }
    if (pcli_Context != NULL)
    {
        NativeObject::Use(*pcli_Context);
        i_ContextRef = NativeObject::GetNativeRef(*pcli_Context);
    }
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndInt("ExecutionContext.Java.__Execution()", i_ContextRef) << cli::endl;
    return i_ContextRef;
}
