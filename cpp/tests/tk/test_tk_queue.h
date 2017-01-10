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


#ifndef __CLI_TEST_TK_QUEUE_H__
#define __CLI_TEST_TK_QUEUE_H__


// Public includes
#include "cli/tk.h"

// Test includes
#include "test_error.h"


static const bool CheckQueue(void)
{
    // Regular scenario.
    do {
        // New queue.
        cli::tk::Queue<int> tk_Queue(3);
        if (! tk_Queue.IsEmpty()) { TestError(__CALL_INFO__) << "tk::Queue: new queue is not empty." << cli::endl; return false; }
        if (tk_Queue.GetCount() != 0) { TestError(__CALL_INFO__) << "tk::Queue: new queue element count is not 0." << cli::endl; return false; }

        // First element addition.
        if (! tk_Queue.AddTail(1)) { TestError(__CALL_INFO__) << "tk::Queue: first element addition failure." << cli::endl; return false; }
        if (tk_Queue.IsEmpty()) { TestError(__CALL_INFO__) << "tk::Queue: queue is empty after first element addition." << cli::endl; return false; }
        if (tk_Queue.GetCount() != 1) { TestError(__CALL_INFO__) << "tk::Queue: queue element count is not 1 after first element addition." << cli::endl; return false; }
        if ((tk_Queue.GetHead() != 1) || (tk_Queue.GetTail() != 1)) { TestError(__CALL_INFO__) << "tk::Queue: head is not 1 or tail is not 1 after first element addition." << cli::endl; return false; }

        // Second element addition.
        if (! tk_Queue.AddTail(2)) { TestError(__CALL_INFO__) << "tk::Queue: second element addition failure." << cli::endl; return false; }
        if (tk_Queue.IsEmpty()) { TestError(__CALL_INFO__) << "tk::Queue: queue is empty after second element addition." << cli::endl; return false; }
        if (tk_Queue.GetCount() != 2) { TestError(__CALL_INFO__) << "tk::Queue: queue element count is not 2 after second element addition." << cli::endl; return false; }
        if ((tk_Queue.GetHead() != 1) || (tk_Queue.GetTail() != 2)) { TestError(__CALL_INFO__) << "tk::Queue: head is not 1 or tail is not 2 after second element addition." << cli::endl; return false; }

        // Third element addition.
        if (! tk_Queue.AddHead(0)) { TestError(__CALL_INFO__) << "tk::Queue: third element addition failure." << cli::endl; return false; }
        if (tk_Queue.IsEmpty()) { TestError(__CALL_INFO__) << "tk::Queue: queue is empty after third element addition." << cli::endl; return false; }
        if (tk_Queue.GetCount() != 3) { TestError(__CALL_INFO__) << "tk::Queue: queue element count is not 3 after third element addition." << cli::endl; return false; }
        if ((tk_Queue.GetHead() != 0) || (tk_Queue.GetTail() != 2)) { TestError(__CALL_INFO__) << "tk::Queue: head is not 0 or tail is not 2 after third element addition." << cli::endl; return false; }

#ifdef CLI_NO_STL
        // Fourth element addition.
        if (tk_Queue.AddTail(3)) { TestError(__CALL_INFO__) << "tk::Queue: fourth element addition succeeded." << cli::endl; return false; }
        if (tk_Queue.IsEmpty()) { TestError(__CALL_INFO__) << "tk::Queue: queue is empty after fourth element addition." << cli::endl; return false; }
        if (tk_Queue.GetCount() != 3) { TestError(__CALL_INFO__) << "tk::Queue: queue element count is not 3 after fourth element addition." << cli::endl; return false; }
        if ((tk_Queue.GetHead() != 0) || (tk_Queue.GetTail() != 2)) { TestError(__CALL_INFO__) << "tk::Queue: head is not 0 or tail is not 2 after fourth element addition." << cli::endl; return false; }

        // Fifth element addition.
        if (tk_Queue.AddHead(-1)) { TestError(__CALL_INFO__) << "tk::Queue: fifth element addition succeeded." << cli::endl; return false; }
        if (tk_Queue.IsEmpty()) { TestError(__CALL_INFO__) << "tk::Queue: queue is empty after fifth element addition." << cli::endl; return false; }
        if (tk_Queue.GetCount() != 3) { TestError(__CALL_INFO__) << "tk::Queue: queue element count is not 3 after fifth element addition." << cli::endl; return false; }
        if ((tk_Queue.GetHead() != 0) || (tk_Queue.GetTail() != 2)) { TestError(__CALL_INFO__) << "tk::Queue: head is not 0 or tail is not 2 after fifth element addition." << cli::endl; return false; }
#else
        // Fourth element addition.
        if (! tk_Queue.AddTail(3)) { TestError(__CALL_INFO__) << "tk::Queue: fourth element addition failure." << cli::endl; return false; }
        if (tk_Queue.IsEmpty()) { TestError(__CALL_INFO__) << "tk::Queue: queue is empty after fourth element addition." << cli::endl; return false; }
        if (tk_Queue.GetCount() != 4) { TestError(__CALL_INFO__) << "tk::Queue: queue element count is not 4 after fourth element addition." << cli::endl; return false; }
        if ((tk_Queue.GetHead() != 0) || (tk_Queue.GetTail() != 3)) { TestError(__CALL_INFO__) << "tk::Queue: head is not 0 or tail is not 3 after fourth element addition." << cli::endl; return false; }

        // Fifth element addition.
        if (! tk_Queue.AddHead(-1)) { TestError(__CALL_INFO__) << "tk::Queue: fifth element addition failure." << cli::endl; return false; }
        if (tk_Queue.IsEmpty()) { TestError(__CALL_INFO__) << "tk::Queue: queue is empty after fifth element addition." << cli::endl; return false; }
        if (tk_Queue.GetCount() != 5) { TestError(__CALL_INFO__) << "tk::Queue: queue element count is not 5 after fifth element addition." << cli::endl; return false; }
        if ((tk_Queue.GetHead() != -1) || (tk_Queue.GetTail() != 3)) { TestError(__CALL_INFO__) << "tk::Queue: head is not -1 or tail is not 3 after fifth element addition." << cli::endl; return false; }

        // Get back to three items only.
        if (tk_Queue.RemoveHead() != -1) { TestError(__CALL_INFO__) << "tk::Queue: could not pop back head item." << cli::endl; return false; }
        if (tk_Queue.RemoveTail() != 3) { TestError(__CALL_INFO__) << "tk::Queue: could not pop back tail item." << cli::endl; return false; }
#endif

        // Iteration.
        do {
            int i_Item = 0;
            for (cli::tk::Queue<int>::Iterator it = tk_Queue.GetIterator(); tk_Queue.IsValid(it); tk_Queue.MoveNext(it)) {
                if (tk_Queue.GetAt(it) != i_Item) { TestError(__CALL_INFO__) << "tk::Queue: element mismatch on iteration." << cli::endl; return false; }
                i_Item ++;
            }
            if (i_Item != 3) { TestError(__CALL_INFO__) << "tk::Queue: iteration should have met 3 items." << cli::endl; return false; }
        } while(0);

        // Sort.
        class _ { public:
            static const int cmp(const int& i_1, const int& i_2) {
                if (0) {}
                else if (i_1 < i_2) { return -1; }
                else if (i_1 > i_2) { return 1; }
                else { return 0; }
            }
        };
        if (! tk_Queue.Sort(_::cmp)) { TestError(__CALL_INFO__) << "tk::Queue: sort failure." << cli::endl; return false; }
        if (tk_Queue.GetCount() != 3) { TestError(__CALL_INFO__) << "tk::Queue: queue element count is not 3 after sorting." << cli::endl; return false; }
        do {
            int i_Item = 2;
            for (cli::tk::Queue<int>::Iterator it = tk_Queue.GetIterator(); tk_Queue.IsValid(it); tk_Queue.MoveNext(it)) {
                if (tk_Queue.GetAt(it) != i_Item) { TestError(__CALL_INFO__) << "tk::Queue: element mismatch after sorting." << cli::endl; return false; }
                i_Item --;
            }
        } while(0);

        // First element removal.
        if (tk_Queue.RemoveTail() != 0) { TestError(__CALL_INFO__) << "tk::Queue: first element removal failure." << cli::endl; return false; }
        if (tk_Queue.IsEmpty()) { TestError(__CALL_INFO__) << "tk::Queue: queue is empty after first element removal." << cli::endl; return false; }
        if (tk_Queue.GetCount() != 2) { TestError(__CALL_INFO__) << "tk::Queue: queue element count is not 2 after first element removal." << cli::endl; return false; }
        if ((tk_Queue.GetHead() != 2) || (tk_Queue.GetTail() != 1)) { TestError(__CALL_INFO__) << "tk::Queue: head is not 2 or tail is not 1 after first element removal." << cli::endl; return false; }

        // Second element removal.
        if (tk_Queue.RemoveHead() != 2) { TestError(__CALL_INFO__) << "tk::Queue: second element removal failure." << cli::endl; return false; }
        if (tk_Queue.IsEmpty()) { TestError(__CALL_INFO__) << "tk::Queue: queue is empty after second element removal." << cli::endl; return false; }
        if (tk_Queue.GetCount() != 1) { TestError(__CALL_INFO__) << "tk::Queue: queue element count is not 1 after second element removal." << cli::endl; return false; }
        if ((tk_Queue.GetHead() != 1) || (tk_Queue.GetTail() != 1)) { TestError(__CALL_INFO__) << "tk::Queue: head is not 1 or tail is not 1 after second element removal." << cli::endl; return false; }

        // Third element removal.
        if (tk_Queue.RemoveTail() != 1) { TestError(__CALL_INFO__) << "tk::Queue: third element removal failure." << cli::endl; return false; }
        if (! tk_Queue.IsEmpty()) { TestError(__CALL_INFO__) << "tk::Queue: queue is not empty after third element removal." << cli::endl; return false; }
        if (tk_Queue.GetCount() != 0) { TestError(__CALL_INFO__) << "tk::Queue: queue element count is not 0 after third element removal." << cli::endl; return false; }
    } while(0);

    // Remove items while iterating.
    do {
        cli::tk::Queue<cli::OutputDevice*> tk_Queue(3);
        if (! tk_Queue.AddTail(& cli::OutputDevice::GetStdOut())) { TestError(__CALL_INFO__) << "tk::Queue: unexpected error." << cli::endl; return false; }
        if (! tk_Queue.AddTail(& cli::OutputDevice::GetStdErr())) { TestError(__CALL_INFO__) << "tk::Queue: unexpected error." << cli::endl; return false; }
        if (! tk_Queue.AddTail(& cli::IODevice::GetStdIn())) { TestError(__CALL_INFO__) << "tk::Queue: unexpected error." << cli::endl; return false; }
        // Create an iterator and make it point to the second item.
        cli::tk::Queue<cli::OutputDevice*>::Iterator it = tk_Queue.GetIterator();
        if (! tk_Queue.IsValid(it)) { TestError(__CALL_INFO__) << "tk::Queue: unexpected error." << cli::endl; return false; }
        if (! tk_Queue.MoveNext(it)) { TestError(__CALL_INFO__) << "tk::Queue: unexpected error." << cli::endl; return false; }
        // Remove that second item.
        if (tk_Queue.Remove(it) != & cli::OutputDevice::GetStdErr()) { TestError(__CALL_INFO__) << "tk::Queue: item removed should be stderr." << cli::endl; return false; }
        if (! tk_Queue.IsValid(it)) { TestError(__CALL_INFO__) << "tk::Queue: it should still be valid after removing the second item." << cli::endl; return false; }
        if (tk_Queue.GetAt(it) != & cli::IODevice::GetStdIn()) { TestError(__CALL_INFO__) << "tk::Queue: it should be focused on stdin now." << cli::endl; return false; }
        // Remove the third and last item.
        if (tk_Queue.Remove(it) != & cli::IODevice::GetStdIn()) { TestError(__CALL_INFO__) << "tk::Queue: item removed should be stdin." << cli::endl; return false; }
        if (tk_Queue.IsValid(it)) { TestError(__CALL_INFO__) << "tk::Queue: it should not be valid anymore after removing the last item." << cli::endl; return false; }
        // Remove the first item.
        it = tk_Queue.GetIterator();
        if (! tk_Queue.IsValid(it)) { TestError(__CALL_INFO__) << "tk::Queue: unexpected error." << cli::endl; return false; }
        if (tk_Queue.Remove(it) != & cli::OutputDevice::GetStdOut()) { TestError(__CALL_INFO__) << "tk::Queue: item removed should be stdout." << cli::endl; return false; }
        if (tk_Queue.IsValid(it)) { TestError(__CALL_INFO__) << "tk::Queue: it should not be valid anymore after removing the last item." << cli::endl; return false; }
    } while(0);

    return true;
}

#endif // __CLI_TEST_TK_QUEUE_H__
