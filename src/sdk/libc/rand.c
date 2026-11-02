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
