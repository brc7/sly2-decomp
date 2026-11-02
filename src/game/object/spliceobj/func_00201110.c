typedef int qword __attribute__((mode(TI)));
typedef struct CRef { int tagk; union { int n; float g; void *pv; } u; } CRef;
typedef struct CREFA { CRef r; } __attribute__((aligned(16))) CREFA;
typedef struct { qword a[3]; } M3;
typedef struct { int a, b; } V2;
typedef struct { int a, b, c; } V3;
CRef *SetTag__4CRef4TAGK(CRef *pref, int tagk);
void func_00129658(CRef *pref, CRef *prefSrc);
void func_00129EC8(CRef *pref);
CRef *CRef_RefCoerceS32(CRef *ret, CRef *pref);
CRef *CRef_RefCoerceF32(CRef *ret, CRef *pref);

/** @sly2-annot addr=0x201110
 * Purpose: UNKNOWN - no analysis recorded yet (byte-matched only).
 * Info: TU spliceobj
 * Confidence: none - unanalysed; anything inferred from the code here is a guess
 * @end-sly2-annot */
CRef *func_00201110(CRef *ret, int c, CRef *aref, int *aotype, void **apv)
{
    CREFA ref;
    CREFA t;
    int i;
    ref.r.tagk = -1;
    for (i = 0; i < c; i++) {
        int ot;
        void *pv;
        func_00129658(&ref.r, &aref[i]);
        ot = aotype[i];
        pv = apv[i];
        switch (ot) {
        case 5: case 6: case 7: case 8:
        case 0xE: case 0xF: case 0x10: case 0x11:
        case 0x13: case 0x14: case 0x15:
            if (ref.r.tagk == 0x15) {
                *(int *)pv = ref.r.u.n;
            } else {
                CREFA tmp;
                CRef *pt = &t.r;
                pt->tagk = -1;
                CRef_RefCoerceS32(&tmp.r, &ref.r);
                func_00129658(pt, &tmp.r);
                func_00129EC8(&tmp.r);
                *(int *)pv = t.r.u.n;
                func_00129EC8(pt);
            }
            break;
        case 1:
            if (ref.r.tagk == 0x16) {
                *(float *)pv = ref.r.u.g;
            } else {
                CREFA tmp;
                CRef *pt = &t.r;
                pt->tagk = -1;
                CRef_RefCoerceF32(&tmp.r, &ref.r);
                func_00129658(pt, &tmp.r);
                func_00129EC8(&tmp.r);
                *(float *)pv = t.r.u.g;
                func_00129EC8(pt);
            }
            break;
        case 4:
            *(M3 *)pv = *(M3 *)ref.r.u.pv;
            break;
        case 2:
        case 9:
            *(qword *)pv = *(qword *)ref.r.u.pv;
            break;
        case 0xA:
            *(V2 *)pv = *(V2 *)ref.r.u.pv;
            break;
        case 0xB:
            *(V3 *)pv = *(V3 *)ref.r.u.pv;
            break;
        default:
            if (ot & 0x8000)
                func_00129658((CRef *)pv, &ref.r);
            else if (ot & 0x1000)
                goto store;
            else if (ot & 0x2000)
        case 0:
            store:
                *(int *)pv = ref.r.u.n;
            break;
        case 0xD:
        case 0x16:
            break;
        }
    }
    SetTag__4CRef4TAGK(ret, 0x24);
    func_00129EC8(&ref.r);
    return ret;
}
