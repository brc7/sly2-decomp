#!/usr/bin/env python3
"""Self-test of dump.py / encode.py on synthetic data (no game files needed): python3 tools/splice/test_splice.py

The script below is invented for the test; it is not from the game.
"""
import os, struct, sys, unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import dump as D       # noqa: E402
import encode as E     # noqa: E402

SYNTHETIC = r"""
(begin
  (define counter 0)                                   ; a save-schema-named symbol (id 40 below)
  (define evt-post-load
    (lambda (self)
      (ensureo self 1300 #|sidebag|# 0)
      (set! counter (+ counter 1))))
  (define evt-trigger
    (lambda (self s2000)
      (if (= (geto self 1300) 3)
        ((geto (find-object 4242 #null-obj) 235) self)
        (schedule-callback 2.5 (lambda () (seto! self 1300 0))))
      (case (geto self 1300)
        ((1 2) 'low)
        (else '()))))
  (define s2001 (list #t #f -7 0.1 -0.0 1e+20 #f32:7fc00000 #vector(1.0 2.0 3.0) #lm(0.0 1.5)))
  (define s2002 #nil)
  (define s2003 (#bif:150 #<unknown tag 3> #obj:5)))
"""


def names():
    return D.Names({40: 'counter', 41: 'low', 42: 'car'})     # 'car' collides with a builtin: printed s42


def roundtrip_text(text):
    n = names()
    cells = E.Compiler(n).entry(text)
    b = D.encode_table([cells])
    r = D.Reader(b)
    assert r.u32() == 1
    back = D.read_list(r, 0)
    assert r.pos == len(b)
    return n, cells, b, back


class Test(unittest.TestCase):
    def test_bytes_of_a_tiny_entry(self):
        cells = E.Compiler(names()).entry('(begin (define counter 1))')
        b = D.encode_table([cells])
        want = struct.pack('<I', 1) + bytes([0x1F, 0x90, 0x1F, 0x82, 0x98]) + struct.pack('<I', 40) + \
            bytes([0x15]) + struct.pack('<i', 1)
        self.assertEqual(b, want)

    def test_synthetic_roundtrip(self):
        n, cells, b, back = roundtrip_text(SYNTHETIC)
        self.assertEqual(back, cells)
        ctx = D.Ctx(n)
        text2 = '\n'.join(D.render_entry(back, ctx, 80))
        self.assertEqual(D.encode_table([E.Compiler(n).entry(text2)]), b)    # print -> compile is stable
        self.assertIn('#<unknown tag 3>', text2)
        self.assertIn('#f32:7fc00000', text2)
        self.assertIn('evt-trigger', text2)
        st = D.Stats()
        st.visit(back, n)
        self.assertEqual(st.unknown, 1)

    def test_json_roundtrip(self):
        n, cells, b, back = roundtrip_text(SYNTHETIC)
        self.assertEqual(D.list_from_json(D.list_to_json(back, n)), cells)

    def test_symbol_collision(self):
        n = names()
        self.assertEqual(n.sym(42), 's42')
        self.assertEqual(n.sym(40), 'counter')
        self.assertEqual(n.sym(5), 'evt-trigger')
        self.assertEqual(n.sym(7), 'evt7')
        self.assertEqual(E.Compiler(n).atom('car'), [D.T_BIF, 33])

    def test_floats(self):
        for g in (0.1, 300.0, -0.0, 1e-30, 3.4e38, 0.05):
            raw = struct.pack('<f', g)
            self.assertEqual(E.f32(D.f32_text(raw)), raw)

    def test_lzss(self):
        # flags 0b0111: three literals 'abc', then a reference to ring index 0, length 3 -> 'abcabc'
        src = bytes([0x07]) + b'abc' + struct.pack('<H', (0 << 13) | 0)
        self.assertEqual(D.lzss(src), b'abcabc')
        # a reference to an unwritten slot of the zero-filled ring reads zeros
        self.assertEqual(D.lzss(bytes([0x00]) + struct.pack('<H', 0x100)), b'\0\0\0')

    def test_listing_split(self):
        text = (';;; ==== world entry 3 x  root oid 1\n;; -- entry 0 of x [..]\n(begin (define counter 1))\n'
                ';; -- entry 1 of x [..]\n#nil\n')
        t = E.compile_listing(text, names())
        self.assertEqual(list(t), [3])
        self.assertEqual(t[3][1], None)

    def test_builtin_table(self):
        n = names()
        self.assertEqual(len(n.bifs), 169)
        self.assertEqual(n.problems, [])
        self.assertEqual(n.bif(112), 'find-object')


if __name__ == '__main__':
    unittest.main()
