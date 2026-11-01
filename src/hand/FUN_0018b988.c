/* FUN_0018b988 (0x1AABC0): maps (kind 1..21, error code) to a message id.  Error codes are mostly small negatives
   (-1..-8, -403); code 0 is success and gives message 0, unknown codes fall back to 11 (code < -9) or 31.
   Kinds 7..9 and anything outside 1..21 give 31 (jump-table default).

   Compile note (lane 10): kinds 16/17 write `result = 0` first and then branch to the shared `done:` return at the
   end of the function.  Those conditional branches to the function's return label are what leave the label in
   front of the last `jr ra` while dbr runs, so the final "result = 31; return" keeps an unfilled delay slot like
   the original.  Kind 1's `result = 9` is plain.  No register pin or asm is needed. */
static inline int Fallback(int err) { int msg = 11; if (err >= -9) msg = 31; return msg; }

/** @sly2-annot addr=0x1AABC0
 * Purpose: (kind 1..21, sceMc error) -> message id for the UI
 * Info: proposed name MsgFromMcError (EXISTING); TU memcard
 * Notes: agent/verifier proposes MsgFromMcError for this existing name (see existing_name_changes.csv) |
 *   RENAME proposed -> MsgFromMcError (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.60] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r4_17F090+arbitration
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
int FUN_0018b988(int kind, int err)
{
    int result;
    switch (kind) {
    case 1:
        if (err == -2) goto ret10;
        if (err < -1) { if (err == -5) goto ret11; goto fallback; }
        if (err != -1) goto zeroOrFallback;
        result = 9;
        return result;
ret10:  return 10;
ret11:  return 11;
fallback:
        return Fallback(err);
    case 2:
        switch (err) {
        case -3: ret1: return 1;
        case -4: return 13;
        case -5: return 6;
        case -7: return 25;
        case -2: goto ret12;
        }
        goto ordinary;
errOther16:
        if (err != -4) return Fallback(err);
ret16:  return 16;
    case 4:
        if (err == -4) goto ret16;
        goto checkOrdinary;
    case 5:
        switch (err) { case -4: return 16; case -5: return 17; case -2: goto ret12; }
        goto ordinary;
    case 6:
        switch (err) { case -3: return 1; case -4: return 16; case -5: return 18; case -8: return 26; case -2: goto ret12; }
        goto ordinary;
    case 3:
    case 10:
        if (err == -2) goto ret12;
        if (err < -1) goto errOther16;
zeroOrFallback:
        if (err != 0) goto fallback;
zero:   return 0;
    case 11:
        if (err == -3) goto ret1;
        if (err < -2) { if (err == -4) goto ret24; goto fallback; }
        if (err == -2) goto ret12;
        goto zeroOrFallback;
ret24:  return 24;
    case 12:
        if (err == -2) goto ret12;
        if (err >= -1) goto zeroOrFallback;
        if (err != -4) return Fallback(err);
ret21:  return 21;
    case 13:
        if (err == -4) goto ret21;
        goto checkOrdinary;
ret12:  return 12;
    case 14:
        if (err == -2) goto ret12;
        if (err >= -1) goto zeroOrFallback;
errOther22:
        if (err != -4) return Fallback(err);
        return 22;
    case 15:
        switch (err) { case -4: return 14; case -5: return 19; case -6: return 23; case -2: goto ret12; case 0: goto zero; }
        return Fallback(err);
    case 16:
        result = 0;
        if (err == 0) goto done;
        { int msg = 4; if (err >= 0) msg = 31; return msg; }
    case 17:
        result = 0;
        if (err == 0) goto done;
        { int msg = 5; if (err >= 0) msg = 31; return msg; }
    case 18:
checkOrdinary:
        if (err == -2) goto ret12;
ordinary:
        if (err >= 0) goto zero;
        return Fallback(err);
    case 19:
        if (err == -2) goto ret12;
        if (err < -1) goto errOther22;
        goto zeroOrFallback;
    case 20:
        switch (err) { case 0: goto zero; case -403: return 35; }
        return 31;
    case 21:
        if (err >= 0) goto zero;
        if ((unsigned)(err + 100) < 100) return 31;
        { int msg = 30; if (err >= -99) msg = 31; return msg; }
    }
    result = 31;
done:
    return result;
}
