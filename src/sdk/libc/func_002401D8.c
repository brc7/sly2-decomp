/* func_002401D8 = strcmp (newlib r5900 string routine, like memcmp/memcpy/memmove/memset: hand-written
 * assembly, kept as a top-level asm block). (Claude agent, T1553 for Lilly)
 * Why asm and not C: the 64-bit constants use the dli expansion of the game-era assembler (Sony's libc C
 * compiler ee-gcc 2.9 expands them with lui and cannot compile any TI-mode MMI operation); pcpyud has a
 * don't-care rt ($a0) exactly like memcmp's; and the registers are reused like an asm programmer would
 * ($t0 = s1|s2, then the 0x01 qword mask, then ~x; $a3 = 0x01.. then the psubw result). Every C draft
 * (build/hand_wip/func_002401D8.*) needed a register pin on nearly every variable to come close.
 *
 * Paths: both pointers 16-aligned -> 16 bytes per step (psubw difference, pnor/psubb/pand null test);
 * 8-aligned -> 8 bytes per step with the classic (x - 0x0101..) & ~x & 0x8080.. null test; equal chunk
 * containing a NUL -> return 0; otherwise (or after a differing chunk) compare bytewise and return
 * *(unsigned char *)s1 - *(unsigned char *)s2. */
__asm__(
    ".set noreorder\n"
    ".set noat\n"
    ".text\n"
    ".globl func_002401D8\n"
    ".ent func_002401D8\n"
    "func_002401D8:\n"
    "or $t0, $a0, $a1\n"
    "andi $v0, $t0, 0x7\n"
    "bnel $v0, $zero, L240310\n"            /* not 8-aligned: bytewise */
    "lb $v0, 0x0($a0)\n"
    "andi $t1, $t0, 0xF\n"
    /* $a3 = 0x0101010101010101 */
    "ori $a3, $zero, 0x8080\n"
    "dsll $a3, $a3, 16\n"
    "ori $a3, $a3, 0x8080\n"
    "dsll $a3, $a3, 16\n"
    "ori $a3, $a3, 0x8080\n"
    "dsll $a3, $a3, 9\n"
    "ori $a3, $a3, 0x101\n"
    /* $a2 = 0x8080808080808080 */
    "ori $a2, $zero, 0x8080\n"
    "dsll $a2, $a2, 16\n"
    "ori $a2, $a2, 0x8080\n"
    "dsll $a2, $a2, 16\n"
    "ori $a2, $a2, 0x8080\n"
    "dsll $a2, $a2, 16\n"
    "ori $a2, $a2, 0x8080\n"
    "bnez $t1, L2402A4\n"                   /* 8- but not 16-aligned */
    "ld $v0, 0x0($a1)\n"
    /* 16 bytes at a time */
    "lq $v1, 0x0($a0)\n"
    "pcpyld $t0, $a3, $a3\n"
    "lq $v0, 0x0($a1)\n"
    "pcpyld $t2, $a2, $a2\n"
    "psubw $a3, $v0, $v1\n"
    "pcpyud $a2, $a3, $a0\n"
    "or $v1, $a2, $a3\n"
    "bnel $v1, $zero, L240310\n"            /* first qwords differ */
    "lb $v0, 0x0($a0)\n"
    "lq $v0, 0x0($a0)\n"
    "pnor $v1, $zero, $v0\n"
    "L240258:\n"
    "psubb $v0, $v0, $t0\n"
    "pand $v0, $v0, $v1\n"
    "pand $v0, $v0, $t2\n"
    "pcpyud $v1, $v0, $a0\n"
    "or $a2, $v1, $v0\n"
    "beqz $a2, L24027C\n"
    "addiu $a0, $a0, 0x10\n"
    "jr $ra\n"                              /* equal and contains the NUL */
    "daddu $v0, $zero, $zero\n"
    "L24027C:\n"
    "addiu $a1, $a1, 0x10\n"
    "lq $v0, 0x0($a0)\n"
    "lq $v1, 0x0($a1)\n"
    "psubw $a3, $v0, $v1\n"
    "pcpyud $a2, $a3, $a0\n"
    "or $t1, $a2, $a3\n"
    "beql $t1, $zero, L240258\n"
    "pnor $v1, $zero, $v0\n"
    "b L240310\n"
    "lb $v0, 0x0($a0)\n"
    /* 8 bytes at a time */
    "L2402A4:\n"
    "ld $v1, 0x0($a0)\n"
    "bnel $v1, $v0, L240310\n"
    "lb $v0, 0x0($a0)\n"
    "ld $v0, 0x0($a0)\n"
    "nor $t0, $zero, $v0\n"
    "L2402B8:\n"
    "dsubu $v0, $v0, $a3\n"
    "and $v0, $v0, $t0\n"
    "and $v0, $v0, $a2\n"
    "beqz $v0, L2402D4\n"
    "addiu $a0, $a0, 0x8\n"
    "jr $ra\n"                              /* equal and contains the NUL */
    "daddu $v0, $zero, $zero\n"
    "L2402D4:\n"
    "addiu $a1, $a1, 0x8\n"
    "ld $v0, 0x0($a0)\n"
    "ld $v1, 0x0($a1)\n"
    "beql $v1, $v0, L2402B8\n"
    "nor $t0, $zero, $v0\n"
    "b L240310\n"
    "lb $v0, 0x0($a0)\n"
    /* bytewise; $v0 = *(signed char *)s1 on entry */
    "L2402F0:\n"
    "sll $v0, $v1, 24\n"
    "lb $v1, 0x0($a1)\n"
    "sra $v0, $v0, 24\n"
    "bnel $v0, $v1, L240318\n"
    "lbu $v1, 0x0($a0)\n"
    "addiu $a0, $a0, 0x1\n"
    "addiu $a1, $a1, 0x1\n"
    "lb $v0, 0x0($a0)\n"
    "L240310:\n"
    "bnez $v0, L2402F0\n"
    "lbu $v1, 0x0($a0)\n"
    "L240318:\n"
    "lbu $v0, 0x0($a1)\n"
    "jr $ra\n"
    "subu $v0, $v1, $v0\n"
    ".end func_002401D8\n"
    ".set at\n"
    ".set reorder\n"
);
