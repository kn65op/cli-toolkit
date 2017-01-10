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

#include "cli/ui_float.h"

#include "cli_ui_Float.h"

#include "NativeObject.h"
#include "NativeExec.h"
#include "NativeTraces.h"


extern "C" JNIEXPORT jint JNICALL Java_cli_ui_Float__1_1Float(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeParentContextRef, jdouble D_DefaultValue, jdouble D_MinValue, jdouble D_MaxValue)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("ui.Float.__Float(I_NativeParentContextRef, D_DefaultValue, D_MinValue, D_MaxValue)") << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt("I_NativeParentContextRef", I_NativeParentContextRef) << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamFloat("D_DefaultValue", D_DefaultValue) << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamFloat("D_MinValue", D_MinValue) << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamFloat("D_MaxValue", D_MaxValue) << cli::endl;
    NativeObject::REF i_FloatRef = 0;
    cli::ui::Float* pcli_Float = NULL;
    if (I_NativeParentContextRef != 0)
    {
        if (cli::ExecutionContext* const pcli_ParentContext = NativeObject::GetNativeObject<cli::ExecutionContext*>(I_NativeParentContextRef))
        {
            pcli_Float = new cli::ui::Float(*pcli_ParentContext, D_DefaultValue, D_MinValue, D_MaxValue);
        }
    }
    else
    {
        pcli_Float = new cli::ui::Float(D_DefaultValue, D_MinValue, D_MaxValue);
    }
    if (pcli_Float != NULL)
    {
        NativeObject::Use(*pcli_Float);
        i_FloatRef = NativeObject::GetNativeRef(*pcli_Float);
    }
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndInt("ui.Float.__Float()", i_FloatRef) << cli::endl;
    return i_FloatRef;
}

extern "C" JNIEXPORT jdouble JNICALL Java_cli_ui_Float__1_1getFloat(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeFloatRef)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("ui.Float.__getFloat(I_NativeFloatRef)") << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt("I_NativeFloatRef", I_NativeFloatRef) << cli::endl;
    double d_Float = 0.0;
    if (const cli::ui::Float* const pcli_Float = NativeObject::GetNativeObject<const cli::ui::Float*>(I_NativeFloatRef))
    {
        d_Float = pcli_Float->GetFloat();
    }
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndFloat("ui.Float.__getFloat()", d_Float) << cli::endl;
    return d_Float;
}
