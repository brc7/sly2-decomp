// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
/* libc (newlib) rand() */
struct _reent { char pad[0x58]; int _next; };
extern struct _reent *_impure_ptr;
int rand(void)
{
    _impure_ptr->_next = _impure_ptr->_next * 1103515245 + 12345;
    return _impure_ptr->_next & 0x7fffffff;
}

/* _reent is newlib's per-thread state; only _next (the LCG state, +0x58 on the EE, seeded by srand; newlib's _REENT_INIT starts it at 1,
   not checked in this ELF's impure data) is modelled here. A port must keep this LCG and seed for the game's random sequence (rand() is
   ((next = next * 1103515245 + 12345) & 0x7fffffff) on a 32-bit int). */
