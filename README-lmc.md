The Maude LTL Logical Model Checker
===================================

## Overview

A concurrent system can be naturally specified as a rewrite theory
$(\Sigma, E, R)$.  Under simple conditions, narrowing with rules $R$ modulo
the equations $E$ can be used both to represent the system's state space by
means of terms with logical variables and for model checking verification
of LTL properties. Folding abstraction can approximate an infinite logical
reachable subsystem into a finite logical system. The Maude LTL logical
model checker (LMC) tool can verify LTL properties over such logical states
from a pattern of (possibly infinite) initial states using the (folded)
logical state space.

Using this method, one of four outcomes is possible: (i) a fixpoint (a
finite state space) is reached and the formula is fully verified; (ii) no
such fixpoint is reached and the formula is  verified only up to a given
bound; (iii) a real counterexample is found and reported; or (iv) a
possibly spurious counterexample is found and reported.

## User Interface

Our tool provides a user interface implemented by extending 
[Full Maude]((https://github.com/maude-team/full-maude/). The main module
`SYMBOLIC-CHECKER` (in the file [`symbolic-checker.maude`](src/Main/symbolic-checker.maude))) 
declares the sorts and operators used by the tool. To use the tool, the user 
should declare a state sort as a subsort of the predefined sort `State` and 
state predicates using the predefined operator `_|= : State Prop -> Bool` in 
the same way as the Maude LTL model checker.

There are the two commands for logical model checking an LTL formula $\varphi$
from an initial pattern $t$ with the optional bound $n$:

- (lmc [$n$] $t$ |= $\varphi$ .)
- (lfmc [$n$] $t$ |= $\varphi$ .)

Each command uses a different folding relation: `lmc` uses the $E$-renaming
equivalence and `lfmc` uses the $E$-subsumption. 


## Example

Consider the following rewrite theory for the reader/writer problem, where
`<R, W>` represents the number of readers and writers in the system:

```maude
mod READERS-WRITERS is 
  sort Natural . 
  op 0 : -> Natural [ctor] . 
  op s : Natural -> Natural [ctor] .  
  sort Config . 
  op <_,_> : Natural Natural -> Config [ctor] .

  vars R W : Natural .

  rl [enter-w] : < 0, 0 > => < 0, s(0) > [narrowing] . 
  rl [leave-w] : < R, s(W) > => < R, W > [narrowing] . 
  rl [enter-r] : < R, 0 > => < s(R), 0 > [narrowing] . 
  rl [leave-r] : < s(R), W > => < R, W > [narrowing] .
endm
```

We consider two state predicates: `reads` indicates that there is at least 
one reader, and `writes` indicates that there is at least one writer. The 
following module in Full Maude declares the state predicates:

```maude
load symbolic-checker

(mod READERS-WRITERS-PROPS is
  pr READERS-WRITERS .
  pr SYMBOLIC-CHECKER .

  subsort Config < State .

  vars N M : Natural .

  ops one-writer : -> Prop .
  eq < N, 0       > |= one-writer = true [variant] .
  eq < N, s(0)    > |= one-writer = true [variant] .
  eq < N, s(s(M)) > |= one-writer = false [variant] .

  op reads : -> Prop .
  eq < s(N), M > |= reads = true [variant] .
  eq < 0,    M > |= reads = false [variant] .

  op writes : -> Prop .
  eq < M, s(N) > |= writes = true [variant] .
  eq < M,    0 > |= writes = false [variant] .
endm)
```

The following command checks the mutual exclusion property up to depth 10
using the $E$-renaming equivalence folding:

```
Maude> (lmc [10] < N, 0 > |= [] ~ (reads /\ writes) .)
logical model check in READERS-WRITERS-PROPS :
  < N:Natural,0 > |= []~(reads /\ writes)
result:
  no counterexample found within bound 10
```

The following command verifies the mutual exclusion property using the
$E$-subsumption folding:

```
Maude> (lfmc < N, 0 > |= [] ~ (reads /\ writes) .)
logical folding model check in READERS-WRITERS-PROPS :
  < N:Natural,0 > |= []~(reads /\ writes)
result:
  true (complete with depth 3)
```

The following command finds a counterexample of the liveness property 
`([]<> reads) /\ ([]<> writes)`:

```
Maude> (lmc < N, 0 > |= []<> reads /\ []<> writes .)
logical model check in READERS-WRITERS-PROPS :
  < N:Natural,0 > |= []<> reads /\[]<> writes
result:
  counterexample found at depth 3
  
prefix
  {< s(%1:Natural),0 >,'%1 <- s(%2:Natural),'leave-r}
loop
  {< s(%1:Natural),0 >,'%1 <- s(%2:Natural),'leave-r}  
```

The following command verifies a different liveness property 
`[]<> (reads \/ writes)` using the $E$-subsumption folding:

```
Maude> (lfmc < N, 0 > |= []<> (reads \/ writes) .)
logical folding model check in READERS-WRITERS-PROPS :
  < N:Natural,0 > |= []<>(reads \/ writes)
result:
  true (complete with depth 3)
```

## Example

- [Lamport's Bakery protocol](src/Main/symbolic-examples/bakery.maude) 
- Readers-Writers problem: 
    [simple](src/Main/symbolic-examples/rw.maude),
    [fair](src/Main/symbolic-examples/rw-fair.maude),
    [shared](src/Main/symbolic-examples/rw-shared.maude) 
- [Dijkstra's mutual exclusion algorithm](src/Main/symbolic-examples/dijkstra-mutex.maude) 
- [Token ring mutual exclusion](src/Main/symbolic-examples/token-mutex.maude)