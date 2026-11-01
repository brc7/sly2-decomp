int func_001AAB48(void *p);
int func_00244450(int n, int *a, int *b);
int func_001AAB68(int n);
int func_001AABC0(void *p, int n);
/** @sly2-annot addr=0x1AB098
 * Purpose: Poll sceMcSync(0,&cmd,&res) and map to result; on error map through MsgFromMcError
 * Info: proposed name McSyncResult (EXISTING); TU memcard
 * Notes: agent/verifier proposes McSyncResult for this existing name (see existing_name_changes.csv) | RENAME
 *   proposed -> McSyncResult (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.60] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r4_17F090+arbitration
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
int FUN_0018bc90(void *a0, void *a1, void *a2, int *a3)
{
    int buf[2];
    int r = func_001AAB48(a1);
    if (r != 0x1D)
        return r;
    r = func_001AAB68(func_00244450(0, &buf[0], &buf[1]));
    if (a3)
        *a3 = buf[1];
    if (r == 0x22)
        r = func_001AABC0(a2, buf[1]);
    return r;
}
