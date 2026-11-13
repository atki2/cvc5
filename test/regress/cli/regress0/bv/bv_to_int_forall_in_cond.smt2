; COMMAND-LINE: --solve-bv-as-int=sum
; EXPECT: unsat
; a quantified formula as the condition of an ite
(set-logic BV)
(declare-const a (_ BitVec 4))
(declare-const b (_ BitVec 4))
(declare-const z (_ BitVec 4))
(assert (= z (ite (forall ((y (_ BitVec 4))) (bvult y (bvadd a y))) a b)))
(assert (not (= z a)))
(assert (not (= z b)))
(check-sat)
