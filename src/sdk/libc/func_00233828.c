// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
struct _atexit { struct _atexit *_next; int _ind; void (*_fns[32])(void); };
struct _reent { char pad[0x3C]; void (*__cleanup)(struct _reent *); char pad2[0x148 - 0x40]; struct _atexit *_atexit; };
extern struct _reent *D_002C49D4;
void func_00100218(int) __attribute__((noreturn));

void func_00233828(int code)
{
    struct _atexit *p;
    int n;
    for (p = D_002C49D4->_atexit; p; p = p->_next)
        for (n = p->_ind; --n >= 0;)
            (*p->_fns[n])();
    if (D_002C49D4->__cleanup)
        (*D_002C49D4->__cleanup)(D_002C49D4);
    func_00100218(code);
}
