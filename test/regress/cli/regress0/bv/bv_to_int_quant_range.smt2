; COMMAND-LINE: --solve-bv-as-int=sum --full-saturate-quant
; EXPECT: unsat
; the quantified range constraint of a function
(set-logic ALL)
(declare-fun f ((_ BitVec 4)) (_ BitVec 4))
(declare-const y (_ BitVec 4))
(assert (bvuge y #b1111))
(assert (forall ((x (_ BitVec 4))) (bvult y (f x))))
(check-sat)
