; COMMAND-LINE: --solve-bv-as-int=sum
; EXPECT: unsat
; an ite whose condition is an ite over a bit-vector atom
(set-logic QF_BV)
(declare-const x (_ BitVec 4))
(declare-const y (_ BitVec 4))
(declare-const z (_ BitVec 4))
(declare-const p Bool)
(assert (= z (ite (ite p (bvult x y) (= y z)) x y)))
(assert (not (= z x)))
(assert (not (= z y)))
(check-sat)
