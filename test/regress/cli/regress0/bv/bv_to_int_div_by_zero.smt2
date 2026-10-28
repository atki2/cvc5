; COMMAND-LINE: --solve-bv-as-int=sum
; EXPECT: unsat
; bvudiv and bvurem by zero
(set-logic QF_BV)
(declare-const x (_ BitVec 4))
(declare-const y (_ BitVec 4))
(assert (bvult y #b0001))
(assert (or (not (= (bvudiv x y) #b1111)) (not (= (bvurem x y) x))))
(check-sat)
