(* Homework due March 26th, 6.30 PM *)
(* Your name and Matric number: *)

(* Instructions: 
    This file can be loaded directly into CoqIDE.

    All but one of the following problems are provable; please replace
    the "admit" tactics below with other tactics that convince Coq.  When you
    find the problem which is not provable, please prove its inverse (so if the 
    problem asks you to prove P -> Q, and you realize that this is impossible, 
    then please prove P -> ~Q instead).  The false problem is one of the first 5.
    Once you are done then there should be no "admit" tactics and Coq should
    be able to check through the end of the file.
    
    Hints: 
      1)  START EARLY.  Among other things, stress is a killer and nothing
            is stressful like starting homework one hour before it is due.  Moreover, 
            if you try that, you will probably not succeed, so you will be both 
            stressed and get a poor grade.
      2)  You can download Coq from http://coq.inria.fr/distrib-eng.html.
      3)  Review the lecture slides and proof script demonstrated in class
      4)  Here are some additional resources: 
               Coq Tutorial (recommended):
                   http://coq.inria.fr/V8.2/doc/html/tutorial.html
               Coq in a hurry (also helpful):
                   http://cel.archives-ouvertes.fr/docs/00/33/44/28/PDF/coq-hurry.pdf
               Coq Library (large, but with an index):
                   http://coq.inria.fr/library-eng.html
      5)  Please feel free to ask Aquinas questions by email, by setting up an 
           appointment, or by dropping by his office if he is around.  Aquinas is
           much happier helping one week before the assignment is due than he
           is helping one hour before the assignment is due (in fact, he will 
           probably be eating dinner at that time).
      6)  Banned tactics: the following tactics do too much work for this 
            assignment, so don't use them if you want credit for the problem:
                admit
                firstorder
                tauto
      7)  You should use previous problems to prove later ones by using the
            "apply" tactic.  You can even do this if you do not how to solve the
            previous problem (e.g., if it is still admit) and you will get credit for
            the later problem if that proof is correct.  However, do not do this
            until you have figured out which problem is false and corrected it.
      8)  You may (and, in fact, are encouraged to) use the "omega" tactic
            to deal with arithmatical goals.
      9)  Don't forget to put your name and Matric number at the top of file, and upload 
          the file into the folder "Take_Home_2_Submissions"  by  6:30 PM March, 26.  The 
          name of the file (or the zipped folder) should contain 
          your matricnumber: For instance, HT666666Z.doc or U444444Y.pdf. 
          Late assignments will be penalized 10% / day.

    A note on partial credit: in general, each problem is binary -- either the proof
    checks or it does not.  If it checks (and you have not used banned tactics or
    subverted the theorem prover in other ways, such as by adding axioms),
    then you will get full credit, even if you have not produced the "best" proof 
    script.  However, if it does not check, most likely you will not get any credit for 
    the problem.  Therefore, when you hand the assignment in you should know
    what grade you will get.

*)

Require Import Classical.
(* This is the axiom that if P = Q then P -> Q and Q -> P *)
Axiom prop_ext: ClassicalFacts.prop_extensionality.
Require Import Omega.

(* Problems in propositional logic *)

(* 5 points *)
Lemma problem1: forall P Q R : Prop, 
  (P -> R) -> 
  (Q -> R) -> 
  (P \/ Q) -> R.
Proof.
intro.
intro.
intro.
intro.
intro.
intro.
destruct H1.
apply H.
apply H1.
apply H0.
apply H1.
Qed.

(* 5 points *)
Lemma problem2: forall P Q : Prop,
  (~ (P \/ Q)) = ((~P) /\ (~Q)).
Proof.
intro.
intro.
apply classical.

Qed.

(* 10 points *)
Lemma problem3: forall P Q : Prop, 
  (P -> Q) = (~Q -> ~P).
Proof.
admit.
Qed.

(* 10 points *)
Lemma problem4: forall P Q R : Prop,
  (P -> R) ->
  (Q -> R) ->
  ~R -> 
  (P /\ Q).
Proof.
admit.
Qed.

(* Problems in first-order logic *)

(* 15 points *)
Lemma problem5: forall (P Q : nat -> Prop),
  (forall n, P n = ~ Q n) ->
  (forall n, P n) \/ (exists n, Q n).
Proof.
admit.
Qed.

(* Add some arithmetic *)
Definition even (n : nat) := exists m, m + m = n.
Definition odd (n : nat) := even (n + 1).

(* 5 points *)
(* Hint: omega is your friend *)
Lemma problem6a: forall n, 
  even n ->
  odd (n + 1).
Proof.
admit.
Qed.

(* 5 points *)
Lemma problem6b: forall n,
  odd n ->
  even (n + 1).
Proof.
admit.
Qed.

(* 10 points *)
(* Hint : prove by induction on n 
    Note that if you have "S n" (sucessor of n) and you would like to have
    instead "n + 1", then you can use the following line to convert between
    the two of them (this goes one way, the other way you flip the line 
    around):
      replace (S n) with (n + 1).
    This will do the replacement and give you an additional goal, which
    is to prove that (S n) = (n + 1).  For this, use omega.
    If you want to do this replacement in a hypothesis instead of the goal,
    use:
      replace (S n) with (n + 1) in H.
    where H is the hypothesis you want to do the replacement in.
*)
Lemma problem6c: forall n,
  even n \/ odd n.
Proof.
admit.
Qed.

(* 5 points *)
(* Hint: you can use the "generalize" tactic to add a hypothesis that
    some other step, such as omega, might need *)
Lemma problem7: forall (P Q : nat -> Prop),
  (forall n, even n -> P n) ->
  (forall n, odd n -> Q n) ->
  forall n, P n \/ Q n.
Proof.
admit.
Qed.

(* Here is a function that returns true if the left side is <= the right *)
Fixpoint bigger (n1 n2 : nat) : bool :=
  match (n1, n2) with
   | (0, n2) => false
   | (S n1', 0) => true
   | (S n1', S n2') => bigger n1' n2'
  end.

(* 5 points *)
Lemma problem8a: forall n1 n2,
bigger n1 n2 = bigger (S n1) (S n2).
Proof.
admit.
Qed.

(* We use it to build up this minimum function *)
Definition maximum (n1 n2 : nat) : nat :=
  if bigger n1 n2 then n1 else n2.

(* Here is an example of working with "minimum".  I suggest you go over it
    to learn techniques you can use for problem 8b-d. Also, you may find
    it useful to use it in your proof of 8c. *)
Lemma example: forall n1 n2,
  maximum n1 n2 = maximum n2 n1.
Proof.
unfold maximum.
induction n1; intros; simpl.
destruct n2; trivial.
destruct n2; trivial.
rewrite <- problem8a.
replace (if bigger n1 n2 then S n1 else S n2) with
  (S (if bigger n1 n2 then n1 else n2)).
replace (if bigger n2 n1 then S n2 else S n1) with
  (S (if bigger n2 n1 then n2 else n1)).
rewrite IHn1.
trivial.
destruct (bigger n2 n1); trivial.
destruct (bigger n1 n2); trivial.
Qed.

(* 15 points *)
(* Hint: you can take cases for a natural number n with
      destruct n.
*)
Lemma problem8b : forall n1 n2,
  n1 <= maximum n1 n2.
Proof.
admit.
Qed.

(* 5 points *)
Lemma problem8c : forall n1 n2,
  n2 <= maximum n1 n2.
Proof.
admit.
Qed.

(* 5 points *)
Lemma problem8d : forall n1 n2,
  maximum n1 n2 = n1 \/
  maximum n1 n2 = n2.
Proof.
admit.
Qed.

(* Now we will define the syntax of propositional logic *)

(* atoms we will just say are natural numbers *)
Definition atom : Type := nat.

Inductive Formula : Type :=
 | Atom : forall a: atom, Formula
 | Not : forall F : Formula, Formula
 | Or : forall F1 F2 : Formula, Formula.

(* Here is a definition of the "size" function from the previous homework *)
Fixpoint size (F : Formula) : nat :=
  match F with
   | Atom a => 1
   | Not F => 1 + size F
   | Or F1 F2 => 1 + size F1 + size F2
  end.

(* 5 points *)
(* Hint: use destruct to break F into its 3 cases *)
Lemma problem9: forall F,
  size F >= 1.
Proof.
admit.
Qed.

(* Here is a similar defintion for Formula "depth" *)
Fixpoint depth (F : Formula) : nat :=
  match F with
   | Atom a => 1
   | Not F => 1 + depth F
   | Or F1 F2 => 1 + maximum (depth F1) (depth F2)
  end.

(* 15 points *)
Lemma problem10: forall F,
  depth F <= size F.
Proof.
admit.
Qed.

