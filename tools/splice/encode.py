#!/usr/bin/env python3
"""Compile Splice text (as printed by dump.py) or a dump.py JSON AST back to the serialized form of the world file.

Usage:
  python3 tools/splice/encode.py DISC LEVEL FILE.scm [-o OUT.bin] [--check]
  python3 tools/splice/encode.py --json FILE.json [-o OUT.bin] [--check DISC]

  FILE.scm  a dump.py listing, possibly edited. Structure comes from its header comments: each
            ';;; ==== world entry K NAME' line starts world entry K and each ';; -- entry I of NAME' line starts
            script entry I; the S-expressions after it (normally one (begin ...)) are the entry's top-level cells.
            Annotations (#|...|#) and ';' comments are ignored. DISC and LEVEL are needed for the level's
            save-schema symbol names and, with --check, for the original bytes.
  -o OUT    write the encoded script tables: per world entry with scripts, U16 world-entry index, U32 byte count,
            then the table bytes exactly as LoadSwSpliceFromBrx reads them (U32 count + entries)
  --check   compare every encoded table with the one in the level's world file and report

Writing the tables back into a playable world file is not implemented: the file has to be re-laid out (everything
after a changed table shifts, per-entry Align(0x10)) and LZSS-compressed again. See docs/reference/splice-dump.md.
"""
import argparse, json, os, re, struct, sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import dump as D                                                   # noqa: E402

TOKEN = re.compile(r'''\s+|;[^\n]*|\#\|.*?\|\#|(\#<[^>]*>)|(\#[a-z]+\()|([()'])|([^\s()';]+)''', re.S)
FORM_IDS = dict((n, t) for t, n in D.FORMS.items() if t != 0)
EVENT_IDS = dict(('evt-' + n, k) for k, n in D.EVENT_NAMES.items())
BOX_IDS = dict((n, t) for t, (n, _) in D.BOXES.items())


class ParseError(Exception):
    pass


def tokenize(text):
    pos, out = 0, []
    while pos < len(text):
        m = TOKEN.match(text, pos)
        if not m:
            raise ParseError('cannot tokenize at %r' % text[pos:pos + 30])
        pos = m.end()
        tok = m.group(1) or m.group(2) or m.group(3) or m.group(4)
        if tok:
            out.append(tok)
    return out


def f32(text):
    if text.startswith('#f32:'):
        return bytes.fromhex(text[5:])[::-1]
    b = struct.pack('<f', float(text))
    if struct.unpack('<f', b)[0] != float(text) and abs(float(text)) > 3.4e38:
        raise ParseError('float out of range: %s' % text)
    return b


class Compiler(object):
    def __init__(self, names):
        self.n = names

    def atom(self, t):
        n = self.n
        if re.match(r'^[+-]?\d+$', t):
            v = int(t)
            if not -2 ** 31 <= v < 2 ** 31:
                raise ParseError('integer out of range: %s' % t)
            return [D.T_INT, v]
        if D.NUMBER.match(t) or t.startswith('#f32:'):
            return [D.T_FLOAT, f32(t)]
        if t == '#t': return [D.T_BOOL, 1]
        if t == '#f': return [D.T_BOOL, 0]
        if t == '#null-obj': return [D.T_BASIC, 0]
        if t == '#nil': return [D.T_LIST, None]
        if t == '#cut': return [D.T_CUT, None]
        m = re.match(r'^#(bool|obj|bif):(\d+)$', t)
        if m:
            return [{'bool': D.T_BOOL, 'obj': D.T_BASIC, 'bif': D.T_BIF}[m.group(1)], int(m.group(2))]
        m = re.match(r'^#<unknown tag (\d+)>$', t)
        if m:
            return [int(m.group(1)), None]
        if t in FORM_IDS:
            return [FORM_IDS[t], None]
        if t in n.bifid:
            return [D.T_BIF, n.bifid[t]]
        if t in n.symid:
            return [D.T_SYM, n.symid[t]]
        m = re.match(r'^(?:s|evt)(\d+)$', t)
        if m:
            return [D.T_SYM, int(m.group(1))]
        raise ParseError('unknown name %r (not a builtin spelling, a named symbol, sN or evtN)' % t)

    def datum(self, toks, i):
        """Parse one datum at toks[i]; returns (cell, next index)."""
        if i >= len(toks):
            raise ParseError('unexpected end of input')
        t = toks[i]
        if t == "'":
            x, i = self.datum(toks, i + 1)
            return [D.T_LIST, [[18, None], x]], i
        if t == '(':
            cells, i = [], i + 1
            while i < len(toks) and toks[i] != ')':
                c, i = self.datum(toks, i)
                cells.append(c)
            if i >= len(toks):
                raise ParseError('missing )')
            return ([0, None] if not cells else [D.T_LIST, cells]), i + 1
        if t == ')':
            raise ParseError('unexpected )')
        if t.startswith('#') and t.endswith('('):
            kind = t[1:-1]
            if kind not in BOX_IDS:
                raise ParseError('unknown literal #%s(' % kind)
            tag = BOX_IDS[kind]
            j = i + 1
            gs = []
            while j < len(toks) and toks[j] != ')':
                gs.append(f32(toks[j]))
                j += 1
            if len(gs) != D.BOXES[tag][1]:
                raise ParseError('#%s( needs %d floats' % (kind, D.BOXES[tag][1]))
            return [tag, b''.join(gs)], j + 1
        return self.atom(t), i + 1

    def entry(self, text):
        """Top-level cells of one script entry (one per datum); '#nil' alone = the null list."""
        toks = tokenize(text)
        if toks == ['#nil']:
            return None
        cells, i = [], 0
        while i < len(toks):
            c, i = self.datum(toks, i)
            cells.append(c)
        return cells


def split_listing(text):
    """{world entry index: {entry index: text}} from a dump.py listing's header comments."""
    wes, cur, buf, key = {}, None, [], None

    def flush():
        if key is not None:
            wes[key[0]][key[1]] = '\n'.join(buf)

    for line in text.split('\n'):
        m = re.match(r'^;;; ==== world entry (\d+) ', line)
        if m:
            flush()
            key, buf, cur = None, [], int(m.group(1))
            wes[cur] = {}
            continue
        m = re.match(r'^;; -- entry (\d+) of ', line)
        if m:
            flush()
            if cur is None:
                raise ParseError('entry header before any world entry header')
            key, buf = (cur, int(m.group(1))), []
            continue
        if key is not None:
            buf.append(line)
    flush()
    return wes


def compile_listing(text, names):
    """{world entry index: [cells per entry]}"""
    c = Compiler(names)
    out = {}
    for we, entries in split_listing(text).items():
        if sorted(entries) != list(range(len(entries))):
            raise ParseError('world entry %d: entry numbers must be 0..%d' % (we, len(entries) - 1))
        out[we] = []
        for k in range(len(entries)):
            try:
                out[we].append(c.entry(entries[k]))
            except ParseError as e:
                raise ParseError('world entry %d, entry %d: %s' % (we, k, e))
    return out


def write_tables(tables, path):
    with open(path, 'wb') as f:
        for we in sorted(tables):
            b = D.encode_table(tables[we])
            f.write(struct.pack('<HI', we, len(b)) + b)


def check(tables, dec):
    """(identical, compared, missing) against the decoded level."""
    ok = tot = 0
    for we in dec['wes']:
        if not we['entries']:
            continue
        tot += 1
        orig = dec['data'][we['splice_start']:we['splice_end']]
        if we['index'] in tables and D.encode_table(tables[we['index']]) == orig:
            ok += 1
        elif we['index'] in tables:
            sys.stderr.write('world entry %d %s differs\n' % (we['index'], we['name']))
    return ok, tot


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0],
                                 formatter_class=argparse.RawDescriptionHelpFormatter, epilog=__doc__)
    ap.add_argument('args', nargs='*')
    ap.add_argument('--json')
    ap.add_argument('-o', '--out')
    ap.add_argument('--check', nargs='?', const=True)
    a = ap.parse_args(argv)
    dec = None
    try:
        if a.json:
            with open(a.json) as f:
                j = json.load(f)
            tables = dict((we['index'], [D.list_from_json(e) for e in we['entries']])
                          for we in j['world_entries'] if we['entries'])
            if isinstance(a.check, str):
                game = D.Game(D.Disc(a.check))
                dec = D.decode_level(game, game.levels[j['level']])
        else:
            if len(a.args) != 3:
                ap.error('give DISC LEVEL FILE.scm (or --json FILE.json)')
            game = D.Game(D.Disc(a.args[0]))
            dec = D.decode_level(game, game.levels[int(a.args[1])])
            with open(a.args[2]) as f:
                tables = compile_listing(f.read(), dec['names'])
    except (ParseError, D.FormatError, OSError, KeyError, ValueError) as e:
        sys.exit('encode.py: %s' % e)
    if a.out:
        write_tables(tables, a.out)
        sys.stderr.write('wrote %d script tables to %s\n' % (len(tables), a.out))
    if a.check:
        if dec is None:
            sys.exit('encode.py: --check with --json needs the disc: --check DISC')
        ok, tot = check(tables, dec)
        print('%d/%d script tables byte-identical to the world file' % (ok, tot))
        return 0 if ok == tot else 1
    return 0


if __name__ == '__main__':
    sys.exit(main())
