; COMMAND-LINE: --solve-bv-as-int=sum
; EXPECT: unsat
; unsat only because of the range constraints of the 1-bit variables
(set-logic QF_BV)
(declare-const x (_ BitVec 1))
(declare-const y (_ BitVec 1))
(declare-const z (_ BitVec 1))
(assert (bvult x y))
(assert (bvult y z))
(check-sat)
