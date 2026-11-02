// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0 -fexceptions
// SLY2_ASM: gnu-reorder-db
struct eh_context
{
  void *handler_label;
  void **dynamic_handler_chain;
  void *info;
  void *table_index;
};
extern struct eh_context *(*get_eh_context) (void);
extern void __terminate (void);

extern void __sjthrow (void) __attribute__ ((__noreturn__));

void
__sjpopnthrow ()
{
  struct eh_context *eh = (*get_eh_context) ();
  void ***dhc = &eh->dynamic_handler_chain;
  void (*func)(void *, int);
  void *arg;
  void ***cleanup;

  cleanup = (void***)&(*dhc)[1];

  if (cleanup[0])
    {
      double store[200];
      void **buf = (void**)store;
      buf[1] = 0;
      buf[0] = (*dhc);

      if (! __builtin_setjmp (&buf[2]))
	{
	  *dhc = buf;
	  while (cleanup[0])
	    {
	      func = (void(*)(void*, int))cleanup[0][1];
	      arg = (void*)cleanup[0][2];

	      cleanup[0] = (void **)cleanup[0][0];

	      (*func)(arg, 2);
	    }
	  *dhc = buf[0];
	}
      else
	{
	  __terminate ();
	}
    }

  *dhc = (void**)(*dhc)[0];

  __sjthrow ();
}
