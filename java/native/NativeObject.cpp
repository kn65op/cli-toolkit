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

#include <jni.h>
#include <string.h> // memset

#include "cli/assert.h"
#include "cli/debug.h"
#include "cli/common.h"
// devices
#include "cli/console.h"
#include "cli/file_device.h"
#include "cli/io_mux.h"
#include "cli/single_command.h"
#include "cli/string_device.h"
#include "cli/telnet.h"
// ui package
#include "cli/ui_line.h"
#include "cli/ui_password.h"
#include "cli/ui_int.h"
#include "cli/ui_float.h"
#include "cli/ui_yesno.h"
#include "cli/ui_choice.h"

#include "NativeObject.h"
#include "NativeExec.h"
#include "NativeTraces.h"


NativeObject::ObjectMap NativeObject::m_mapTokens;

const NativeObject::REF NativeObject::GetNativeRef(const cli::Object& CLI_Object)
{
    return (NativeObject::REF) & CLI_Object;
}

cli::Object* const NativeObject::GetNativeObject(const NativeObject::REF I_NativeObjectRef)
{
    return (cli::Object*) I_NativeObjectRef;
}

const jobject NativeObject::GetJavaObject(const cli::Object& CLI_NativeObject, const bool B_MayTrace)
{
    jobject pj_Object = NULL;

    if (B_MayTrace) cli::GetTraces().SafeTrace(TRACE_JNI, CLI_NativeObject) << NativeTraces::Begin("GetJavaObject(CLI_NativeObject)") << cli::endl;
    const REF i_NativeObjectRef = GetNativeRef(CLI_NativeObject);
    if (B_MayTrace) cli::GetTraces().SafeTrace(TRACE_JNI, CLI_NativeObject) << NativeTraces::ParamInt("CLI_NativeObject", i_NativeObjectRef) << cli::endl;
    if (JNIEnv* const pj_Env = NativeExec::GetInstance().GetJNIEnv())
    {
        if (B_MayTrace) cli::GetTraces().SafeTrace(TRACE_JNI, CLI_NativeObject) << NativeTraces::ValuePtr("pj_Env", pj_Env) << cli::endl;
        if (const jclass pj_Class = pj_Env->FindClass("cli/NativeObject"))
        {
            if (B_MayTrace) cli::GetTraces().SafeTrace(TRACE_JNI, CLI_NativeObject) << NativeTraces::ValuePtr("pj_Class", pj_Class) << cli::endl;
            pj_Env->ExceptionClear();
            if (const jmethodID j_MethodID = pj_Env->GetStaticMethodID(pj_Class, "getObject", "(I)Lcli/NativeObject;"))
            {
                if (B_MayTrace) cli::GetTraces().SafeTrace(TRACE_JNI, CLI_NativeObject) << NativeTraces::ValuePtr("j_MethodID", j_MethodID) << cli::endl;
                pj_Object = pj_Env->CallStaticObjectMethod(pj_Class, j_MethodID, (jint) i_NativeObjectRef);
            }
        }
    }
    if (B_MayTrace) cli::GetTraces().SafeTrace(TRACE_JNI, CLI_NativeObject) << NativeTraces::EndPtr("GetJavaObject()", pj_Object) << cli::endl;
    return pj_Object;
}

const bool NativeObject::CreateFromNative(const cli::Object& CLI_Object)
{
    cli::GetTraces().SafeTrace(TRACE_JNI, CLI_Object) << NativeTraces::Begin("NativeObject::CreateFromNative(CLI_Object)") << cli::endl;
    cli::GetTraces().SafeTrace(TRACE_JNI, CLI_Object) << NativeTraces::ParamInt("CLI_Object", NativeObject::GetNativeRef(CLI_Object)) << cli::endl;

    bool b_Res = false;

    if (JNIEnv* const pj_Env = NativeExec::GetInstance().GetJNIEnv())
    {
        cli::GetTraces().SafeTrace(TRACE_JNI, CLI_Object) << NativeTraces::ValuePtr("pj_Env", pj_Env) << cli::endl;
        if (const jclass pj_Class = pj_Env->FindClass(GetJavaClassName(CLI_Object).c_str()))
        {
            cli::GetTraces().SafeTrace(TRACE_JNI, CLI_Object) << NativeTraces::ValuePtr("pj_Class", pj_Class) << cli::endl;

            const jmethodID pj_CreateMethodID = pj_Env->GetStaticMethodID(pj_Class, "createFromNative", "(I)V");
                cli::GetTraces().SafeTrace(TRACE_JNI, CLI_Object) << NativeTraces::ValuePtr("pj_CreateMethodID", pj_CreateMethodID) << cli::endl;
            if (pj_CreateMethodID != NULL)
            {
                // Create a java object from native source.
                pj_Env->CallStaticVoidMethod(pj_Class, pj_CreateMethodID, (jint) NativeObject::GetNativeRef(CLI_Object));
                b_Res = true;
            }
        }
    }

    cli::GetTraces().SafeTrace(TRACE_JNI, CLI_Object) << NativeTraces::EndBool("NativeObject::CreateFromNative()", b_Res) << cli::endl;
    return b_Res;
}

const bool NativeObject::DeleteFromNative(const NativeObject::REF I_ObjectRef)
{
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("NativeObject::DeleteFromNative(I_ObjectRef)") << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt("I_ObjectRef", I_ObjectRef) << cli::endl;

    bool b_Res = false;

    if (JNIEnv* const pj_Env = NativeExec::GetInstance().GetJNIEnv())
    {
        cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ValuePtr("pj_Env", pj_Env) << cli::endl;
        if (const jclass pj_Class = pj_Env->FindClass("cli/NativeObject"))
        {
            cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ValuePtr("pj_Class", pj_Class) << cli::endl;

            const jmethodID pj_DeleteMethodID = pj_Env->GetStaticMethodID(pj_Class, "deleteFromNative", "(I)V");
                cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ValuePtr("pj_DeleteMethodID", pj_DeleteMethodID) << cli::endl;
            if (pj_DeleteMethodID != NULL)
            {
                // Delete the java object from native source.
                // It will not be actually deleted, but will do nothing with native source on finalization.
                pj_Env->CallStaticVoidMethod(pj_Class, pj_DeleteMethodID, (jint) I_ObjectRef);
                b_Res = true;
            }
        }
    }

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndBool("NativeObject::DeleteFromNative()", b_Res) << cli::endl;
    return b_Res;
}

void NativeObject::Use(const NativeObject::REF I_ObjectRef)
{
    if (I_ObjectRef != 0)
    {
        // Find out if the object is already registered.
        ObjectMap::iterator it = m_mapTokens.find(I_ObjectRef);
        if (it != m_mapTokens.end())
        {
            // If it is, simply increment its token number.
            it->second.iTokens ++;
            cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Instance(I_ObjectRef, it->second.iTokens, it->second.bAutoDelete) << cli::endl;
        }
        else
        {
            // If not yet, add a new object info structure.
            ObjectInfo t_Info; memset(& t_Info, '\0', sizeof(t_Info));
            t_Info.iTokens = 1;
            t_Info.bAutoDelete = true;
            m_mapTokens[I_ObjectRef] = t_Info;
            cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Instance(I_ObjectRef, 1, true) << cli::endl;
        }
    }
}

const bool NativeObject::Free(const NativeObject::REF I_ObjectRef)
{
    bool b_AutoDelete = false;

    // Find out if the object is registered.
    ObjectMap::iterator it = m_mapTokens.find(I_ObjectRef);
    if (it != m_mapTokens.end())
    {
        // If it is, decrement the token number.
        it->second.iTokens --;
        cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Instance(I_ObjectRef, it->second.iTokens, it->second.bAutoDelete) << cli::endl;
        // When this number falls down 0, perform deletion.
        if (it->second.iTokens <= 0)
        {
            // Auto-deletion notification.
            b_AutoDelete = it->second.bAutoDelete;
            // Unregistration.
            m_mapTokens.erase(it);
        }
    }

    // Note: Deletion is assumed by the public template version of the Free() method.
    return b_AutoDelete;
}

void NativeObject::Delegate(const NativeObject::REF I_WhatRef, const NativeObject::REF I_WhoRef)
{
    // Find out if the object is registered.
    ObjectMap::iterator it = m_mapTokens.find(I_WhatRef);
    if (it != m_mapTokens.end())
    {
        // Notify no deletion for this object.
        it->second.bAutoDelete = false;
        cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Instance(I_WhatRef, it->second.iTokens, it->second.bAutoDelete) << cli::endl;
    }
}

const std::string NativeObject::GetJavaClassName(const cli::Object& CLI_Object)
{
    cli::GetTraces().SafeTrace(TRACE_JNI, CLI_Object) << NativeTraces::Begin("NativeObject::GetJavaClassName(CLI_Object)") << cli::endl;
    cli::GetTraces().SafeTrace(TRACE_JNI, CLI_Object) << NativeTraces::ParamInt("CLI_Object", NativeObject::GetNativeRef(CLI_Object)) << cli::endl;

    std::string str_ClassName;

    if (dynamic_cast<const cli::Object*>(& CLI_Object)) { str_ClassName = "cli/NativeObject";
        if (dynamic_cast<const cli::CommandLine*>(& CLI_Object)) { str_ClassName = "cli/CommandLine"; }
        if (dynamic_cast<const cli::Element*>(& CLI_Object)) { str_ClassName = "cli/Element";
            if (dynamic_cast<const cli::Endl*>(& CLI_Object)) { str_ClassName = "cli/Endl"; }
            if (dynamic_cast<const cli::MenuRef*>(& CLI_Object)) { str_ClassName = "cli/MenuRef"; }
            if (dynamic_cast<const cli::SyntaxNode*>(& CLI_Object)) { str_ClassName = "cli/SyntaxNode";
                if (dynamic_cast<const cli::Keyword*>(& CLI_Object)) { str_ClassName = "cli/Keyword"; }
                if (dynamic_cast<const cli::Param*>(& CLI_Object)) { str_ClassName = "cli/Param";
                    if (dynamic_cast<const cli::ParamFloat*>(& CLI_Object)) { str_ClassName = "cli/ParamFloat"; }
                    if (dynamic_cast<const cli::ParamHost*>(& CLI_Object)) { str_ClassName = "cli/ParamHost"; }
                    if (dynamic_cast<const cli::ParamInt*>(& CLI_Object)) { str_ClassName = "cli/ParamInt"; }
                    if (dynamic_cast<const cli::ParamString*>(& CLI_Object)) { str_ClassName = "cli/ParamString"; }
                }
                if (dynamic_cast<const cli::SyntaxTag*>(& CLI_Object)) { str_ClassName = "cli/SyntaxTag"; }
            }
            if (dynamic_cast<const cli::SyntaxRef*>(& CLI_Object)) { str_ClassName = "cli/SyntaxRef"; }
        }
        if (dynamic_cast<const cli::ExecutionContext*>(& CLI_Object)) { str_ClassName = "cli/ExecutionContext$Java";
            if (dynamic_cast<const cli::Shell*>(& CLI_Object)) { str_ClassName = "cli/Shell"; }
            if (dynamic_cast<const cli::ui::UI*>(& CLI_Object)) { str_ClassName = "cli/ui/UI";
                if (dynamic_cast<const cli::ui::Line*>(& CLI_Object)) { str_ClassName = "cli/ui/Line";
                    if (dynamic_cast<const cli::ui::Choice*>(& CLI_Object)) { str_ClassName = "cli/ui/Choice";
                        if (dynamic_cast<const cli::ui::YesNo*>(& CLI_Object)) { str_ClassName = "cli/ui/YesNo"; }
                    }
                    if (dynamic_cast<const cli::ui::Float*>(& CLI_Object)) { str_ClassName = "cli/ui/Float"; }
                    if (dynamic_cast<const cli::ui::Int*>(& CLI_Object)) { str_ClassName = "cli/ui/Int"; }
                }
                if (dynamic_cast<const cli::ui::Password*>(& CLI_Object)) { str_ClassName = "cli/ui/Password"; }
            }
        }
        if (dynamic_cast<const cli::ExecutionResult*>(& CLI_Object)) { str_ClassName = "cli/ExecutionResult"; }
        if (dynamic_cast<const cli::Help*>(& CLI_Object)) { str_ClassName = "cli/Help"; }
        if (dynamic_cast<const cli::Menu*>(& CLI_Object)) { str_ClassName = "cli/Menu";
            if (dynamic_cast<const cli::Cli*>(& CLI_Object)) { str_ClassName = "cli/Cli"; }
        }
        if (dynamic_cast<const cli::OutputDevice*>(& CLI_Object)) { str_ClassName = "cli/OutputDevice$Java";
            if (& CLI_Object == & cli::OutputDevice::GetNullDevice()) { str_ClassName = "cli/OutputDevice$Native"; }
            if (& CLI_Object == & cli::OutputDevice::GetStdErr()) { str_ClassName = "cli/OutputDevice$Native"; }
            if (& CLI_Object == & cli::OutputDevice::GetStdOut()) { str_ClassName = "cli/OutputDevice$Native"; }
            if (dynamic_cast<const cli::IODevice*>(& CLI_Object)) { str_ClassName = "cli/IODevice$Java";
                if (& CLI_Object == & cli::IODevice::GetNullDevice()) { str_ClassName = "cli/IODevice$Native"; }
                if (& CLI_Object == & cli::IODevice::GetStdIn()) { str_ClassName = "cli/IODevice$Native"; }
                if (dynamic_cast<const cli::Console*>(& CLI_Object)) { str_ClassName = "cli/Console"; }
                if (dynamic_cast<const cli::InputFileDevice*>(& CLI_Object)) { str_ClassName = "cli/InputFileDevice"; }
                if (dynamic_cast<const cli::IOMux*>(& CLI_Object)) { str_ClassName = "cli/IOMux"; }
                if (dynamic_cast<const cli::NonBlockingIODevice*>(& CLI_Object)) { str_ClassName = "cli/NonBlockingIODevice$Java";
                    if (dynamic_cast<const cli::TelnetConnection*>(& CLI_Object)) { str_ClassName = "cli/TelnetConnection"; }
                }
                if (dynamic_cast<const cli::SingleCommand*>(& CLI_Object)) { str_ClassName = "cli/SingleCommand"; }
            }
            if (dynamic_cast<const cli::OutputFileDevice*>(& CLI_Object)) { str_ClassName = "cli/OutputFileDevice"; }
            if (dynamic_cast<const cli::StringDevice*>(& CLI_Object)) { str_ClassName = "cli/StringDevice"; }
        }
        if (dynamic_cast<const cli::OutputDevice::ScreenInfo*>(& CLI_Object)) { str_ClassName = "cli/OutputDevice$ScreenInfo"; }
        if (dynamic_cast<const cli::ResourceString*>(& CLI_Object)) { str_ClassName = "cli/ResourceString"; }
        if (dynamic_cast<const cli::TelnetServer*>(& CLI_Object)) { str_ClassName = "cli/TelnetServer"; }
        if (dynamic_cast<const cli::TraceClass*>(& CLI_Object)) { str_ClassName = "cli/TraceClass"; }
        if (dynamic_cast<const cli::Traces*>(& CLI_Object)) { str_ClassName = "cli/Traces"; }
    }

    CLI_ASSERT(! str_ClassName.empty());
    cli::GetTraces().SafeTrace(TRACE_JNI, CLI_Object) << NativeTraces::EndStr("NativeObject::GetJavaClassName()", str_ClassName.c_str()) << cli::endl;
    return str_ClassName;
}
