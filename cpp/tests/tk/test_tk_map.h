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


#ifndef __CLI_TEST_TK_MAP_H__
#define __CLI_TEST_TK_MAP_H__


// Public includes
#include "cli/tk.h"

// Test includes
#include "test_error.h"


static const bool CheckMap(void)
{
    // New map.
    cli::tk::Map<cli::tk::String, int> tk_Map(3);
    if (! tk_Map.IsEmpty()) { TestError(__CALL_INFO__) << "tk::Map: new map is not empty." << cli::endl; return false; }
    if (tk_Map.GetCount() != 0) { TestError(__CALL_INFO__) << "tk::Map: new map element count is not 0." << cli::endl; return false; }

    // First element setting.
    if (! tk_Map.SetAt(cli::tk::String(5, "1"), 1)) { TestError(__CALL_INFO__) << "tk::Map: SetAt(1) failure" << cli::endl; return false; }
    if (tk_Map.IsEmpty()) { TestError(__CALL_INFO__) << "tk::Map: tk_Map is empty after SetAt(1)" << cli::endl; return false; }
    if (tk_Map.GetCount() != 1) { TestError(__CALL_INFO__) << "tk::Map: tk_Map count is not 1 after SetAt(1)" << cli::endl; return false; }
    if (! tk_Map.IsSet(cli::tk::String(3, "1"))) { TestError(__CALL_INFO__) << "tk::Map: '1' key is not set after SetAt(1)" << cli::endl; return false; }
    if (tk_Map.IsSet(cli::tk::String(3, "2"))) { TestError(__CALL_INFO__) << "tk::Map: '2' key is set after SetAt(2)" << cli::endl; return false; }

    // Second element setting.
    if (! tk_Map.SetAt(cli::tk::String(5, "2"), 2)) { TestError(__CALL_INFO__) << "tk::Map: SetAt(2) failure" << cli::endl; return false; }
    if (tk_Map.IsEmpty()) { TestError(__CALL_INFO__) << "tk::Map: tk_Map is empty after SetAt(2)" << cli::endl; return false; }
    if (tk_Map.GetCount() != 2) { TestError(__CALL_INFO__) << "tk::Map: tk_Map count is not 2 after SetAt(2)" << cli::endl; return false; }
    if (! tk_Map.IsSet(cli::tk::String(3, "1"))) { TestError(__CALL_INFO__) << "tk::Map: '1' key is not set after SetAt(2)" << cli::endl; return false; }
    if (! tk_Map.IsSet(cli::tk::String(3, "2"))) { TestError(__CALL_INFO__) << "tk::Map: '2' key is set after SetAt(2)" << cli::endl; return false; }

    // Iteration.
    cli::tk::Queue<cli::tk::String> tk_Keys(10);
    cli::tk::Queue<int> tk_Values(10);
    for (cli::tk::Map<cli::tk::String, int>::Iterator it = tk_Map.GetIterator(); tk_Map.IsValid(it); tk_Map.MoveNext(it)) {
        tk_Keys.AddTail(tk_Map.GetKey(it));
        tk_Values.AddTail(tk_Map.GetAt(it));
    }
    class _ { public:
        static const int CmpInt(const int& I1, const int& I2) {
            if (0) {}
            else if (I1 < I2) return 1;
            else if (I1 > I2) return -1;
            else return 0;
        }
        static const int CmpString(const cli::tk::String& STR1, const cli::tk::String& STR2) {
            if (0) {}
            else if (STR1 < STR2) return 1;
            else if (STR2 > STR1) return -1;
            else return 0;
        }
    };
    tk_Keys.Sort(_::CmpString);
    tk_Values.Sort(_::CmpInt);

    // GetAt.
    if (const int* const pi_1 = tk_Map.GetAt(cli::tk::String(3, "1"))) {
        if (*pi_1 != 1) { TestError(__CALL_INFO__) << "tk::Map: GetAt(1) does not match the element 1." << cli::endl; return false; }
    } else { TestError(__CALL_INFO__) << "tk::Map: GetAt(1) failure." << cli::endl; return false; }
    if (const int* const pi_2 = tk_Map.GetAt(cli::tk::String(3, "2"))) {
        if (*pi_2 != 2) { TestError(__CALL_INFO__) << "tk::Map: GetAt(2) does not match the element 2." << cli::endl; return false; }
    } else { TestError(__CALL_INFO__) << "tk::Map: GetAt(2) failure." << cli::endl; return false; }

    // First element removal.
    if (! tk_Map.Unset(cli::tk::String(3, "1"))) { TestError(__CALL_INFO__) << "tk::Map: Unset(1) failure." << cli::endl; return false; }
    if (! tk_Map.Unset(cli::tk::String(3, "1"))) { TestError(__CALL_INFO__) << "tk::Map: second call to Unset(1) failure." << cli::endl; return false; }
    if (tk_Map.IsEmpty()) { TestError(__CALL_INFO__) << "tk::Map: map is empty after Unset(1)." << cli::endl; return false; }
    if (tk_Map.GetCount() != 1) { TestError(__CALL_INFO__) << "tk::Map: map element count is not 1 after Unset(1)." << cli::endl; return false; }
    if (tk_Map.IsSet(cli::tk::String(3, "1"))) { TestError(__CALL_INFO__) << "tk::Map: '1' is still set after Unset(1)." << cli::endl; return false; }
    if (! tk_Map.IsSet(cli::tk::String(3, "2"))) { TestError(__CALL_INFO__) << "tk::Map: '2' is not set anymore after Unset(1)." << cli::endl; return false; }

    // Second element removal.
    cli::tk::Map<cli::tk::String, int>::Iterator head = tk_Map.GetIterator();
    if (! tk_Map.IsValid(head)) { TestError(__CALL_INFO__) << "tk::Map: head iterator is not valid while there are 2 elements in the map." << cli::endl; return false; }
    if (! tk_Map.Remove(head)) { TestError(__CALL_INFO__) << "tk::Map: Remove(head) failure." << cli::endl; return false; }
    if (! tk_Map.IsEmpty()) { TestError(__CALL_INFO__) << "tk::Map: map is not empty after Remove(head)." << cli::endl; return false; }
    if (tk_Map.GetCount() != 0) { TestError(__CALL_INFO__) << "tk::Map: map element count is not 0 after Remove(head)." << cli::endl; return false; }
    if (tk_Map.IsSet(cli::tk::String(3, "1"))) { TestError(__CALL_INFO__) << "tk::Map: '1' is still set after Remove(head)." << cli::endl; return false; }
    if (tk_Map.IsSet(cli::tk::String(3, "2"))) { TestError(__CALL_INFO__) << "tk::Map: '2' is still set anymore after Remove(head)." << cli::endl; return false; }

    return true;
}

#endif // __CLI_TEST_TK_MAP_H__
