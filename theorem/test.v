Section ReadingGroup2.


Inductive List: Type :=
 | nil: List
 | cons: nat -> List -> List.
Check List.


Fixpoint app L R :=
 match L with
 | nil => R
 | cons l ls => cons l (app ls R)
end.
Check app.


Lemma app_assoc:
    forall l1 l2 l3,
    app (app l1 l2) l3 = app l1 (app l2 l3).
Proof.
intros.
induction l1.
simpl.
trivial.
simpl.
rewrite IHl1.
trivial.
Qed.
Check app_assoc.

Fixpoint rev (L: List) : List :=
 match L with
 | nil => nil
 | cons l ls => app (rev ls) (cons l nil)
end.
Check rev.

Lemma rev_app:
    forall (x:nat) (l:list),
    rev (cons x l) = 