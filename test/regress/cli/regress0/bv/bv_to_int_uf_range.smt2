; COMMAND-LINE: --solve-bv-as-int=sum
; EXPECT: unsat
; the range constraint of a function application
(set-logic QF_UFBV)
(declare-fun f ((_ BitVec 2)) (_ BitVec 2))
(declare-const x (_ BitVec 2))
(assert (bvult #b10 (f x)))
(assert (not (= (f x) #b11)))
(check-sat)
