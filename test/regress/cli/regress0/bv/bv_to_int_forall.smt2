; COMMAND-LINE: --solve-bv-as-int=sum --full-saturate-quant
; EXPECT: unsat
; a quantified formula over two bit-vector variables and an integer variable, whose body has a condition over bit-vectors
(set-logic ALL)
(declare-fun f ((_ BitVec 4) Int) (_ BitVec 4))
(declare-const a (_ BitVec 4))
(assert (forall ((x (_ BitVec 4)) (y (_ BitVec 4)) (i Int)) (bvule (ite (bvult x a) a x) (f y i))))
(assert (= a #b0101))
(assert (= (f #b0000 0) #b0000))
(check-sat)
