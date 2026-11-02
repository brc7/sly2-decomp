typedef int qword __attribute__((mode(TI)));
typedef struct M4 { qword r[4]; } M4;

/** @sly2-annot addr=0x166428
 * Purpose: Transforms an array of qword points by a 4x4 matrix using VU0 micro-subroutines (vcallms
 *   0x120/0xF0), double-buffered
 * Info: proposed name TransformPointsVu0 (T1); TU geom
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r3_1539A8
 * See: docs/reference/functions/physics-collision.md#geom-0x166270-0x166920
 * @end-sly2-annot */
void TransformPointsVu0(const M4 *matrix, unsigned int count,
                   const qword *in0, qword *output)
{
    if (count) {
        register unsigned int end __asm__("$2") /* register pin */ = count * sizeof(qword);
        register qword first __asm__("vf1");
        register qword second __asm__("vf2");
        register qword third __asm__("vf3");
        register qword fourth __asm__("vf4");
        register qword even __asm__("vf6");
        register qword odd __asm__("vf5");

        const qword *input;
        /* first input row: volatile asm keeps it ahead of the pointer/end updates (Claude agent) */
        __asm__ __volatile__("lqc2 %0,%1" : "=j"(even) : "m"(*in0));
        input = in0 + 1;
        end = (unsigned int)output + end;
        __asm__ __volatile__("lqc2 %0,%4\n\tlqc2 %1,%5\n\tlqc2 %2,%6\n\tlqc2 %3,%7"
                : "=j"(fourth), "=j"(first), "=j"(second), "=j"(third)
                : "m"(matrix->r[3]), "m"(matrix->r[0]),
                  "m"(matrix->r[1]), "m"(matrix->r[2]));
        __asm__ __volatile__("vcallms 0x120"
                : "=j"(even)
                : "j"(first), "j"(second), "j"(third), "j"(fourth), "0"(even));
        do {
            odd = *input++;
            /* VNOP waits for the microprogram before results are stored. */
            __asm__ __volatile__("vcallms 0xF0\n\tvnop"
                    : "=j"(odd)
                    : "j"(first), "j"(second), "j"(third), "j"(fourth), "0"(odd));
            *output++ = even;
            if (output == (qword *)end)
                break;
            even = *input++;
            __asm__ __volatile__("vcallms 0x120\n\tvnop"
                    : "=j"(even)
                    : "j"(first), "j"(second), "j"(third), "j"(fourth), "0"(even));
            *output++ = odd;
        } while (output != (qword *)end);
    }
}

typedef char layout_qword[(sizeof(qword)==16)?1:-1];
typedef char layout_pointer[(sizeof(void*)==4)?1:-1];
typedef char layout_M4[(sizeof(M4)==0x40)?1:-1];
typedef char layout_M4_row0[((unsigned long)&((M4*)0)->r[0]==0x0)?1:-1];
typedef char layout_M4_row1[((unsigned long)&((M4*)0)->r[1]==0x10)?1:-1];
typedef char layout_M4_row2[((unsigned long)&((M4*)0)->r[2]==0x20)?1:-1];
typedef char layout_M4_row3[((unsigned long)&((M4*)0)->r[3]==0x30)?1:-1];
