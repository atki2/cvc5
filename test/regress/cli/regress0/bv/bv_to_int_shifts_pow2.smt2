; REQUIRES: unrestricted-mode
; COMMAND-LINE: --solve-bv-as-int=sum --bv-to-int-use-pow2
; EXPECT: unsat
; shifts by a variable amount, translated with int.pow2
(set-logic QF_BV)
(declare-const x (_ BitVec 4))
(declare-const y (_ BitVec 4))
(declare-const z (_ BitVec 4))
(assert (bvult y #b0100))
(assert (or (= (bvshl #b0001 y) #b0000)
            (bvult x (bvlshr x y))
            (and (bvslt x #b0000) (not (bvslt (bvashr x y) #b0000)))))
(check-sat)
