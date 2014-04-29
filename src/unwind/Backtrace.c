/* libunwind - a platform-independent unwind library
   Copyright (C) 2003-2004 Hewlett-Packard Co
	Contributed by David Mosberger-Tang <davidm@hpl.hp.com>

This file is part of libunwind.

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.  */

#include "unwind-internal.h"

/* ANDROID support update. */
PROTECTED _Unwind_Reason_Code
_Unwind_Backtrace (_Unwind_Trace_Fn trace, void *trace_parameter)
{
  struct _Unwind_Context context;
  unw_context_t uc;
  int step_ret, ret = _URC_NO_REASON;

  unw_map_local_create ();

  if (_Unwind_InitContext (&context, &uc) < 0)
    ret = _URC_FATAL_PHASE1_ERROR;
  else
    {
      /* Phase 1 (search phase) */

      while (ret == _URC_NO_REASON)
        {
          if ((step_ret = unw_step (&context.cursor)) <= 0)
	    {
	      if (step_ret == 0)
	        ret = _URC_END_OF_STACK;
	      else
	        ret = _URC_FATAL_PHASE1_ERROR;
	    }
          else if ((*trace) (&context, trace_parameter) != _URC_NO_REASON)
	    ret = _URC_FATAL_PHASE1_ERROR;
        }
    }

  unw_map_local_destroy ();

  return ret;
}
/* End of ANDROID update. */

_Unwind_Reason_Code __libunwind_Unwind_Backtrace (_Unwind_Trace_Fn, void *)
     ALIAS (_Unwind_Backtrace);
