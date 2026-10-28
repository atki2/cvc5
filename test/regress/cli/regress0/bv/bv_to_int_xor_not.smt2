; COMMAND-LINE: --solve-bv-as-int=sum
; EXPECT: unsat
; bvxor and bvnot
(set-logic QF_BV)
(declare-const x (_ BitVec 4))
(declare-const y (_ BitVec 4))
(assert (bvult x #b0100))
(assert (bvult y #b0100))
(assert (or (not (bvult (bvxor x y) #b0100)) (bvult (bvnot x) #b1100)))
(check-sat)
