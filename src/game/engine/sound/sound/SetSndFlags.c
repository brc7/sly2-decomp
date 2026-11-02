struct S { char pad[0x50]; unsigned short fl; };
void AddSndToDirtyList(struct S *);
void RemoveSndFromDirtyList(struct S *);
/** @sly2-annot addr=0x1FB658
 * Purpose: fl = (fl&~mask)|(val&mask); nonzero -> link into SNDMGR dirty list, zero -> unlink
 * Info: proposed name SetSndFlags (T1); TU sound
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/engine/sound.md#2-sound-instances, docs/engine/sound.md#6-key-functions,
 *   docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void SetSndFlags(struct S *p, unsigned short m, unsigned short v)
{
    p->fl = (p->fl & ~m) | (v & m);
    if (p->fl) AddSndToDirtyList(p); else RemoveSndFromDirtyList(p);
}
