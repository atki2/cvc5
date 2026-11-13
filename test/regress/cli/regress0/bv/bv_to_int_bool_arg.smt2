; COMMAND-LINE: --solve-bv-as-int=sum
; EXPECT: unsat
; a function applied to a Boolean condition over bit-vectors
(set-logic QF_UFBV)
(declare-fun f (Bool) (_ BitVec 4))
(declare-const x (_ BitVec 4))
(declare-const y (_ BitVec 4))
(declare-const z (_ BitVec 4))
(assert (bvult x y))
(assert (bvult y z))
(assert (not (= (f (bvult x z)) (f true))))
(check-sat)
