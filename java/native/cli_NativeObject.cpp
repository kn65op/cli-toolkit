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

#include "cli/cli.h"
#include "cli/shell.h"

#include "cli_NativeObject.h"

#include "NativeExec.h"
#include "NativeTraces.h"
#include "NativeDevice.h"


extern "C" JNIEXPORT void JNICALL Java_cli_NativeObject__1_1finalize(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeRef)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    if (const cli::Object* const pcli_Object = NativeObject::GetNativeObject<const cli::Object*>(I_NativeRef))
    {
        // Remember whether traces are effective (or not, either because TRACE_JNI traces are off, or because pcli_Object is unsafe for traces).
        const cli::OutputDevice& cli_TracesDevice = (
            cli::GetTraces().SafeTrace(TRACE_JNI, *pcli_Object) << NativeTraces::Begin("NativeObject.__finalize(I_NativeRef)") << cli::endl
        );
        if (& cli_TracesDevice != & cli::OutputDevice::GetNullDevice()) {
            cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt("I_NativeRef", I_NativeRef) << cli::endl;
        }

        if (const cli::Shell* const pcli_Shell = dynamic_cast<const cli::Shell*>(pcli_Object))
        {
            cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ValuePtr("pcli_Shell", pcli_Shell) << cli::endl;

            const cli::Cli& cli_Cli = pcli_Shell->GetCli();
            NativeObject::Free(*pcli_Shell);
            NativeObject::Free(cli_Cli);
        }
        else
        {
            NativeObject::Free(*pcli_Object);
        }

        // At this point, since pcli_Object may be deleted, do not use Traces::SafeTrace() anymore,
        // but rely on the traces device information reminded previously.
        if (& cli_TracesDevice != & cli::OutputDevice::GetNullDevice()) {
            cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndVoid("NativeObject.__finalize()") << cli::endl;
        }
        else
        {
            // Make the call even if not used, for traces indentation.
            const cli::tk::String tk_LostTrace = NativeTraces::EndVoid("NativeObject.__finalize()");
        }
    }
}
