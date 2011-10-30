#Number of actors
param n, >0;

#number of processors
param p, >0;

#actors
set A := 1..n;

#processors
set P := 1..p;

# Actor groupings, any actor in the same group cant
#	be assigned to same processor
param AG{A,A}, integer;

# Runtime of an actor on a given processor
param PI{A,P} >= 0;

# Communication cost between two actors (units in bytes)
param CA{A,A} >= 0;

# Communication bandwitdh between two processors
#	(units in bytes per second)
param CP{P,P} >= 0;


param CAP{i in A, j in A, k in P, l in P} :=
	(CA[i,j] * CP[k,l])/2 + (PI[i,k] + PI[j,l])/(2*n);
 
#assignment matrix
var x{A,P}, binary;

#linearisation of term x[a,p] x[b,q]
var y{A,A,P,P}, binary;

#target solution
minimize totalcost:
     sum {i in A, j in A, k in P, l in P}
		CAP[i,j,k,l] * y[i,j,k,l];

# actor allocation
subject to

actor_allocation {i in A}:
    sum{j in P} x[i,j] =1;

primary_pairing{i in A, j in A, k in P, l in P}:
    y[i,j,k,l] <= x[i,k];

secondary_pairing{i in A, j in A, k in P, l in P}:
    y[i,j,k,l] <= x[j,l];

sum_pairing{i in A, j in A, k in P, l in P}:
    x[i,k] + x[j,l] -1 <= y[i,j,k,l];

non_overlap{i in A, j in A, k in P, l in P}:
if(k == l) then AG[i,j]*y[i,j,k,l] = 0;


