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

#include "cli/non_blocking_io_device.h"
#include "cli/shell.h"

#include "cli_NonBlockingIODevice.h"

#include "NativeObject.h"
#include "NativeTraces.h"
#include "NativeExec.h"
#include "NativeDevice.h"


// NonBlockingIODevice.Common implementation.

//extern "C" JNIEXPORT jint JNICALL Java_cli_NonBlockingIODevice_Common__1_1getExecutionContext(
extern "C" JNIEXPORT jint JNICALL Java_cli_NonBlockingIODevice__1_1Common_1_1getExecutionContext(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeDeviceRef)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    NativeObject::REF i_ContextRef = 0;
    if (const cli::NonBlockingIODevice* const pcli_NonBlockingIODevice = NativeObject::GetNativeObject<const cli::NonBlockingIODevice*>(I_NativeDeviceRef))
    {
        cli::GetTraces().SafeTrace(TRACE_JNI, *pcli_NonBlockingIODevice) << NativeTraces::Begin("NonBlockingIODevice.Common.__getExecutionContext(I_NativeDeviceRef)") << cli::endl;
        cli::GetTraces().SafeTrace(TRACE_JNI, *pcli_NonBlockingIODevice) << NativeTraces::ParamInt("I_NativeDeviceRef", I_NativeDeviceRef) << cli::endl;

        class BypassProtected : public cli::NonBlockingIODevice {
            public: const cli::ExecutionContext* const _GetExecutionContext(void) const {
                return GetExecutionContext();
            }
        };
        if (const cli::ExecutionContext* const pcli_Context = reinterpret_cast<const BypassProtected*>(pcli_NonBlockingIODevice)->_GetExecutionContext())
        {
            i_ContextRef = NativeObject::GetNativeRef(*pcli_Context);
        }

        cli::GetTraces().SafeTrace(TRACE_JNI, *pcli_NonBlockingIODevice) << NativeTraces::EndInt("NonBlockingIODevice.Common.__getExecutionContext()", i_ContextRef) << cli::endl;
    }
    return i_ContextRef;
}

//extern "C" JNIEXPORT void JNICALL Java_cli_NonBlockingIODevice_Common__1_1onKey(
extern "C" JNIEXPORT void JNICALL Java_cli_NonBlockingIODevice__1_1Common_1_1onKey(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeDeviceRef, jint E_Key)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    if (const cli::NonBlockingIODevice* const pcli_NonBlockingIODevice = NativeObject::GetNativeObject<const cli::NonBlockingIODevice*>(I_NativeDeviceRef))
    {
        cli::GetTraces().SafeTrace(TRACE_JNI, *pcli_NonBlockingIODevice) << NativeTraces::Begin("NonBlockingIODevice.Common.__onKey(I_NativeDeviceRef, E_Key)") << cli::endl;
        cli::GetTraces().SafeTrace(TRACE_JNI, *pcli_NonBlockingIODevice) << NativeTraces::ParamInt("I_NativeDeviceRef", I_NativeDeviceRef) << cli::endl;
        cli::GetTraces().SafeTrace(TRACE_JNI, *pcli_NonBlockingIODevice) << NativeTraces::ParamInt("E_Key", E_Key) << cli::endl;

        // Java does not necessarily use C++ encoding for "\n".
        if (((E_Key == '\r') || (E_Key == '\n')) && (E_Key != cli::ENTER))
        {
            E_Key = cli::ENTER;
            cli::GetTraces().SafeTrace(TRACE_JNI, *pcli_NonBlockingIODevice) << NativeTraces::ValueInt("E_Key", E_Key) << cli::endl;
        }
        pcli_NonBlockingIODevice->OnKey((cli::KEY) E_Key);

        cli::GetTraces().SafeTrace(TRACE_JNI, *pcli_NonBlockingIODevice) << NativeTraces::EndVoid("NonBlockingIODevice.Common.__onKey()") << cli::endl;
    }
}


// NonBlockingIODevice.Native implementation.


// NonBlockingIODevice.Java implementation.

//extern "C" JNIEXPORT jint JNICALL Java_cli_NonBlockingIODevice_Java__1_1NonBlockingIODevice(
extern "C" JNIEXPORT jint JNICALL Java_cli_NonBlockingIODevice__1_1Java_1_1Java(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jstring PJ_DbgName)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("NonBlockingIODevice.Java.__Java(PJ_DbgName)") << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamStr("PJ_DbgName", NativeExec::Java2Native(PJ_DbgName).c_str()) << cli::endl;

    NativeObject::REF i_NonBlockingIODeviceRef = 0;
    if (cli::NonBlockingIODevice* const pcli_NonBlockingIODevice = new NativeDevice<cli::NonBlockingIODevice>(NativeExec::Java2Native(PJ_DbgName).c_str()))
    {
        NativeObject::Use(*pcli_NonBlockingIODevice);
        i_NonBlockingIODeviceRef = NativeObject::GetNativeRef(*pcli_NonBlockingIODevice);
    }

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndInt("NonBlockingIODevice.Java.__Java()", i_NonBlockingIODeviceRef) << cli::endl;
    return i_NonBlockingIODeviceRef;
}
