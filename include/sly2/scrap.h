// scrap.h -- SLY2_SCRAP: the dead-code scraps of config/junk.csv, emitted in place.
//
// The ELF's .text has, between some functions of one TU, 8-byte units of one instruction and a nop: an orphaned
// `addiu $sp,$sp,N`, `move $2,$4`, `sw $5,N($4)`, ... -- the kind of instruction that fills the delay slot of a
// function's final `jr $ra` -- with no code that reaches it. Each unit is what is left of a function that is not in
// the ELF: its last instruction and its alignment padding (most likely the original toolchain's removal of unused
// functions cutting each one an instruction short; runs of several units are several removed functions in a row).
// No C produces an instruction without its return, so the scraps are assembly: each SLY2_SCRAP is one unit (8-byte
// aligned, the nop after it is the alignment the next unit or function asks for), written at the place in the source
// where the removed function stood. It goes into the current text section, so the object's code reproduces the
// ELF's run without -ffunction-sections and, with it, the scrap trails the preceding function's section; it defines
// no symbol. See docs/MATCHING_NOTES.md "Dead-code scraps" and docs/guides/full-link.md "Junk scraps and padding".
#ifndef SLY2_SCRAP_H
#define SLY2_SCRAP_H

#define SLY2_SCRAP(insn) __asm__("\t.align 3\n\t.set noreorder\n\t" insn "\n\t.set reorder")

#endif
