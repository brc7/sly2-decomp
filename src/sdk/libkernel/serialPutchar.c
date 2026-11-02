// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
void func_00235780(int c);
void serialPutchar(int c)
{
    if (c == '\n') {
        func_00235780('\r');
        func_00235780('\n');
        return;
    }
    func_00235780(c);
}
