// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
void func_00234A50(int sema);
int _DelayThreadHandler(int id, unsigned short t, int a2, int sema)
{
    func_00234A50(sema);
    __asm__ volatile("sync");
    __asm__ volatile("ei");
    return 0;
}
