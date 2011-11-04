Section ReadingGroup2.

Inductive List: Type :=
 | nil: List
 | cons: nat -> List -> List.

Fixpoint app L R :=
 match L with
 | nil => R
 | cons l ls => cons l (app ls R)
end.

Lemma app_assoc:
  forall l1 l2 l3,
  app (app l1 l2) l3 = app l1 (app l2 l3).
intros.
induction l1.
trivial.
simpl app.  
rewrite IHl1.
trivial.
Qed.

Fixpoint rev (L: List) : List :=
 match L with
 | nil => nil 
 | cons l ls => app (rev ls) (cons l nil)
 end.

Lemma rev_app: 
  forall (x:nat) (l:List),
  rev (cons x l) = app (rev l) (cons x nil).
intros.
simpl.
trivial.
Qed.

Lemma app_cons :
  forall x l,
  app (cons x nil) l = cons x l.
intros.
simpl.
trivial.
Qed.

Lemma rev_alt:
  forall x l,
  rev (app l (cons x nil)) = cons x (rev l).
intros.
induction l.
simpl (app nil (cons x nil)).
simpl (rev nil).
simpl (rev (cons x nil)).
trivial.
simpl (rev (cons n l)).
simpl (rev (app (cons n l) (cons x nil))).
rewrite IHl.
simpl (app (cons x (rev l)) (cons n nil)).
trivial.
Qed.

Theorem rev2: forall l: List, l = rev (rev l).
Proof.
(* Fill in here *)
intros.
symmetry.
induction l.
trivial.
simpl (rev (cons n l)).
rewrite rev_alt.
rewrite IHl.
trivial.
Qed.