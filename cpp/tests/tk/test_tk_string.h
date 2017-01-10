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


#ifndef __CLI_TEST_TK_STRING_H__
#define __CLI_TEST_TK_STRING_H__

// Public includes
#include "cli/tk.h"

// Test includes
#include "test_error.h"


static const bool CheckString(void)
{
    // New string.
    cli::tk::String tk_String(10);
    if (! tk_String.IsEmpty()) { TestError(__CALL_INFO__) << "tk::String: new String is not empty." << cli::endl; return false; }
    if (tk_String.GetLength() != 0) { TestError(__CALL_INFO__) << "tk::String: new String length is not 0." << cli::endl; return false; }
    if ((const char*) tk_String == NULL) { TestError(__CALL_INFO__) << "tk::String: implicit conversion does not work on a new String." << cli::endl; return false; }
    if (strcmp(tk_String, "") != 0) { TestError(__CALL_INFO__) << "tk::String: new String does not equal ''." << cli::endl; return false; }

    // 0 character string.
    if (! tk_String.Set("")) { TestError(__CALL_INFO__) << "tk:String: could not set ''." << cli::endl; return false; }
    if (! tk_String.IsEmpty()) { TestError(__CALL_INFO__) << "tk::String: string is not empty after setting ''." << cli::endl; return false; }
    if (tk_String.GetLength() != 0) { TestError(__CALL_INFO__) << "tk::String: length is not 0 after setting ''." << cli::endl; return false; }
    if (strcmp(tk_String, "") != 0) { TestError(__CALL_INFO__) << "tk:String: tk_String differs from ''." << cli::endl; return false; }

    // 3 characters string.
    if (! tk_String.Set("abc")) { TestError(__CALL_INFO__) << "tk:String: could not set 'abc'." << cli::endl; return false; }
    if (tk_String.IsEmpty()) { TestError(__CALL_INFO__) << "tk::String: string is empty after setting 'abc'." << cli::endl; return false; }
    if (tk_String.GetLength() != 3) { TestError(__CALL_INFO__) << "tk::String: length is not 3 after setting 'abc'." << cli::endl; return false; }
    if (strcmp(tk_String, "abc") != 0) { TestError(__CALL_INFO__) << "tk:String: tk_String differs from 'abc'." << cli::endl; return false; }

    // 10 characters string.
    if (! tk_String.Set("abcdefghij")) { TestError(__CALL_INFO__) << "tk:String: could not set 'abcdefghij'." << cli::endl; return false; }
    if (tk_String.IsEmpty()) { TestError(__CALL_INFO__) << "tk::String: string is empty after setting 'abcdefghij'." << cli::endl; return false; }
    if (tk_String.GetLength() != 10) { TestError(__CALL_INFO__) << "tk::String: length is not 10 after setting 'abcdefghij'." << cli::endl; return false; }
    if (strcmp(tk_String, "abcdefghij") != 0) { TestError(__CALL_INFO__) << "tk:String: tk_String differs from 'abcdefghij'." << cli::endl; return false; }

#ifdef CLI_NO_STL
    // 11 characters string.
    if (tk_String.Set("abcdefghijk")) { TestError(__CALL_INFO__) << "tk:String: 'abcdefghijk' set without trouble." << cli::endl; return false; }
    if (tk_String.IsEmpty()) { TestError(__CALL_INFO__) << "tk::String: string is empty after setting 'abcdefghijk'." << cli::endl; return false; }
    if (tk_String.GetLength() != 10) { TestError(__CALL_INFO__) << "tk::String: length is not 10 after setting 'abcdefghijk'." << cli::endl; return false; }
    if (strcmp(tk_String, "abcdefghij") != 0) { TestError(__CALL_INFO__) << "tk:String: tk_String differs from 'abcdefghij'." << cli::endl; return false; }
#else
    // 11 characters string.
    if (! tk_String.Set("abcdefghijk")) { TestError(__CALL_INFO__) << "tk:String: could not set 'abcdefghijk'." << cli::endl; return false; }
    if (tk_String.IsEmpty()) { TestError(__CALL_INFO__) << "tk::String: string is empty after setting 'abcdefghijk'." << cli::endl; return false; }
    if (tk_String.GetLength() != 11) { TestError(__CALL_INFO__) << "tk::String: length is not 11 after setting 'abcdefghijk'." << cli::endl; return false; }
    if (strcmp(tk_String, "abcdefghijk") != 0) { TestError(__CALL_INFO__) << "tk:String: tk_String differs from 'abcdefghijk'." << cli::endl; return false; }
    // Set back to 'abcdefghij' as in CLI_NO_STL
    if (! tk_String.Set("abcdefghij")) { TestError(__CALL_INFO__) << "tk:String: could not set back to 'abcdefghij'." << cli::endl; return false; }
#endif

    // Sub-string.
    if (strcmp(tk_String.SubString(1, 3), "bcd") != 0) { TestError(__CALL_INFO__) << "tk::String: SubString(1, 3) failure." << cli::endl; return false; }
    if (strcmp(tk_String.SubString(0, 0), "") != 0) { TestError(__CALL_INFO__) << "tk::String: SubString(0, 0) failure." << cli::endl; return false; }

    // Upper / Lower operations.
    if (strcmp(tk_String.ToUpper(), "ABCDEFGHIJ") != 0) { TestError(__CALL_INFO__) << "tk::String: ToUpper() failure." << cli::endl; return false; }
    if (strcmp(tk_String.ToUpper().ToLower(), "abcdefghij") != 0) { TestError(__CALL_INFO__) << "tk::String: ToLower() failure." << cli::endl; return false; }

    // Equal operator.
    bool b_Res = false;
    b_Res = (tk_String == "abcdefghij");
    if (! b_Res) { TestError(__CALL_INFO__) << "tk::String: equal operator failure." << cli::endl; return false; }
    b_Res = (tk_String == "abcdefghijk");
    if (b_Res) { TestError(__CALL_INFO__) << "tk::String: equal operator failure." << cli::endl; return false; }

    // Difference operator.
    b_Res = (tk_String != "abcdefghij");
    if (b_Res) { TestError(__CALL_INFO__) << "tk::String: difference operator failure." << cli::endl; return false; }
    b_Res = (tk_String != "abcdefghijk");
    if (! b_Res) { TestError(__CALL_INFO__) << "tk::String: difference operator failure." << cli::endl; return false; }

    return true;
}

#endif // __CLI_TEST_TK_STRING_H__
