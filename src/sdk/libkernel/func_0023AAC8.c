// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
extern int D_0044EF80[];
int func_0023AAC8(int i) { return D_0044EF80[i]; }

// ---- Common symbols defined here (docs/guides/tu-merging.md "Data", address names): g++ 2.95 emits a class or
// template static data member as COMMON (C: a tentative definition), and the linker allocates commons at the end
// of .bss (small ones in .sbss), which is why these addresses are not in TU order. Sizes are the gaps to the next
// name in the ELF; a 16-aligned start with a 16-multiple size is a qword array (commons take the type's alignment).
typedef int qword __attribute__((mode(TI)));
qword c_0044EF80[264] __asm__("D_0044EF80");   /* .bss 0x1080 */
