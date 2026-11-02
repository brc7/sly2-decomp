typedef struct CRef { int m_tagk; union { int n; float g; } u; } CRef;
int func_00117BD0(void *pbs);
int func_00117DB8(void *pbs);
float func_00117E38(void *pbs);
int func_00117D50(void *pbs);
int func_00117C80(void *pbs);
void CRef_ctor_CLQ(CRef *ret, float *ag);
void CRef_ctor_LM(CRef *ret, float *ag);
void CRef_ctor_VECTOR(CRef *ret, float *ag);
void func_0012A0B0(CRef *ret, int tagk);
/** @sly2-annot addr=0x13E690
 * Purpose: Reads one typed method-option argument into a CRef by otyp: 0 bool(U8), 1 float, 5/6 S32, 7/14..23
 *   S16, 2/9 3xF32 vectors, 10 2xF32, 0x1xxx U32, else null tag 0x24
 * Info: proposed name RefFromBrx (T1); TU brx
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels;
 *   METHOD option argument reader (2.2-2.8k method options per level now parsed)
 * Confidence: high [T1 0.90] - evidence: semantic review; sources: r2_131F70+parse-verified
 * See: docs/engine/world-sw.md#10-key-functions,
 *   docs/formats/level-objects.md#4-common-building-blocks-cparse,
 *   docs/formats/world-file.md#37-options-of-g_sw-and-g_cm-func_0013f810---func_0013e818
 * @end-sly2-annot */
CRef *func_0013E690(CRef *ret, int k, void *pbs)
{
    float ag[3];
    switch (k) {
    case 0:
        {
            int f = func_00117BD0(pbs) != 0;
            ret->m_tagk = 0x17;
            ret->u.n = f;
        }
        break;
    case 5: case 6:
        ret->u.n = func_00117DB8(pbs);
        ret->m_tagk = 0x15;
        break;
    case 1:
        ret->u.g = func_00117E38(pbs);
        ret->m_tagk = 0x16;
        break;
    case 7: case 14: case 15: case 16: case 17: case 19: case 20: case 21: case 23:
        ret->u.n = func_00117D50(pbs);
        ret->m_tagk = 0x15;
        break;
    case 9:
        {
            float *pg = ag;
            int j;
            for (j = 0; j < 3; j++)
                *pg++ = func_00117E38(pbs);
        }
        CRef_ctor_CLQ(ret, ag);
        break;
    case 10:
        ag[0] = func_00117E38(pbs);
        ag[1] = func_00117E38(pbs);
        CRef_ctor_LM(ret, ag);
        break;
    case 2:
        {
            float *pg = ag;
            int j;
            for (j = 0; j < 3; j++)
                *pg++ = func_00117E38(pbs);
        }
        CRef_ctor_VECTOR(ret, ag);
        break;
    default:
        if (((k & ~0x8000) >> 12) == 1) {
            ret->u.n = func_00117C80(pbs);
            ret->m_tagk = 0x15;
        } else {
            func_0012A0B0(ret, 0x24);
        }
        break;
    }
    return ret;
}
