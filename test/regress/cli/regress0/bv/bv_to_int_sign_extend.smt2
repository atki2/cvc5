; COMMAND-LINE: --solve-bv-as-int=sum
; EXPECT: unsat
; sign_extend of a negative value, and by 0 bits
(set-logic QF_BV)
(declare-const x (_ BitVec 4))
(declare-const y (_ BitVec 6))
(declare-const z (_ BitVec 4))
(assert (bvslt x #b0000))
(assert (= y ((_ sign_extend 2) x)))
(assert (or (bvult y #b111000) (and (bvult x z) (bvult z ((_ sign_extend 0) x)))))
(check-sat)
