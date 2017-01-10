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

#include "cli/single_command.h"

#include "cli_SingleCommand.h"

#include "NativeObject.h"
#include "NativeExec.h"
#include "NativeTraces.h"


extern "C" JNIEXPORT jint JNICALL Java_cli_SingleCommand__1_1SingleCommand(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jstring PJ_CommandLine, jint I_NativeOutputDeviceRef)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("SingleCommand.__SingleCommand(PJ_CommandLine, I_NativeOutputDeviceRef)") << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamStr("PJ_CommandLine", NativeExec::Java2Native(PJ_CommandLine).c_str()) << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt("I_NativeOutputDeviceRef", I_NativeOutputDeviceRef) << cli::endl;
    NativeObject::REF i_DeviceRef = 0;
    if (cli::OutputDevice* const pcli_OutputDevice = NativeObject::GetNativeObject<cli::OutputDevice*>(I_NativeOutputDeviceRef))
    {
        if (cli::SingleCommand* const pcli_Device = new cli::SingleCommand(NativeExec::Java2Native(PJ_CommandLine).c_str(), *pcli_OutputDevice, true))
        {
            NativeObject::Use(*pcli_Device);
            i_DeviceRef = NativeObject::GetNativeRef(*pcli_Device);
        }
    }
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndInt("SingleCommand.__SingleCommand()", i_DeviceRef) << cli::endl;
    return i_DeviceRef;
}
