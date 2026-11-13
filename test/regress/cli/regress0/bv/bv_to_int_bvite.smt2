; COMMAND-LINE: --solve-bv-as-int=sum
; EXPECT: unsat
; bvite, with an ite in one of its branches
(set-logic QF_BV)
(declare-const x (_ BitVec 4))
(declare-const y (_ BitVec 4))
(declare-const z (_ BitVec 4))
(assert (= z (bvite ((_ extract 0 0) (bvadd x y)) (bvmul x y) (ite (bvult x y) x y))))
(assert (not (= z (bvmul x y))))
(assert (not (= z x)))
(assert (not (= z y)))
(check-sat)
