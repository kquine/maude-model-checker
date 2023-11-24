The Maude Linear Temporal Logic of Rewriting Model Checker
==========================================================

## Introduction

The Linear Temporal Logic of Rewriting (LTLR) is a state/event based extension
of LTL with _spatial action patterns_ representing rewrite events. LTLR describes
a property that involves both events and state predicates, including _mixed_
properties such as fairness. The Maude LTLR model checker is an explicit state
model checker within the [Maude](http://maude.cs.illinois.edu) system.


Our tool can verify LTLR properties under _localized fairness_ assumptions, which
are parameterized fairness conditions over certain system entities. A good
example of parametric fairness is provided by object fairness assumptions such as 

> Each object `o` that is infinitely often enabled to receive a message of the
> form `m(o,q)` will receive it infinitely often, 

which is universally quantified over all the (possibly dynamically changing) 
objects `o` in the system. Such universal quantification is succinctly captured by 
the notion of localized fairness; for example, fairness localized to the parameter 
`o` in object fairness conditions.

The Maude LTLR model checker is implemented at the C++ level by extending the
existing Maude LTL model checker. Unlike [our previous rewriting-based LTLR 
model checker](https://maude.cs.illinois.edu/tools/tlr/old) based on a theory 
and formula transformation, our new implementation do not cause any increase in 
the state space.

## Examples

Below are examples of LTLR model checking with our tool. Some of them have localized 
fairness specifications. More details about the tool and examples can be found 
[here](https://www.sciencedirect.com/science/article/pii/S0167642314000471?via%3Dihub).

- [Evolving Dining Philosophers](src/Main/ltlr-examples/philo-dynamic.maude)
  (c.f., [Classical Dining Philosophers](src/Main/ltlr-examples/simple-dp.maude))
- [Blanced Sliding Window Protocol](src/Main/ltlr-examples/swp.maude)
- [Simple Client-Server model](src/Main/ltlr-examples/client-server.maude)
- [Dekker's Algorithm](src/Main/ltlr-examples/dekker.maude)
- [Bounded retransmission protocol](src/Main/ltlr-examples/brp-ltlr.maude)
- [Unordered-channel](src/Main/ltlr-examples/unordered-channel.maude)
- [Position fairness](src/Main/ltlr-examples/context.maude)

## References

The Maude Fair LTLR Model checker is illustrated in:

- [Model Checking Linear Temporal Logic of Rewriting Formulas under Localized Fairness](https://www.sciencedirect.com/science/article/pii/S0167642314000471?via%3Dihub), 
   by Kyungmin Bae and José Meseguer, 
   _Science of Computer Programming 99_, 2015

- [Model Checking LTLR Formulas under Localized Fairness](https://link.springer.com/chapter/10.1007/978-3-642-34005-5_6), 
  by Kyungmin Bae and José Meseguer, 
  _International Workshop on Rewriting Logic and its Applications (WRLA)_, 2012

The parameterized fair model checking algorithm and localized fairness are presented in:

- [State/Event-Based LTL Model Checking under Parametric Generalized Fairness](https://link.springer.com/chapter/10.1007/978-3-642-22110-1_11), 
  by Kyungmin Bae and José Meseguer, 
  _International Conference on Computer Aided Verification (CAV)_, 2011

- [Localized Fairness: A Rewriting Semantics](https://link.springer.com/chapter/10.1007/978-3-540-32033-3_19), 
  by José Meseguer, 
  _International Conference on Rewriting Techniques and Applications (RTA)_, 2005

The automata-theoretic foundation, design, and implementation of the Maude LTLR Model Checker is illustrated at:

- [The Linear Temporal Logic of Rewriting Maude Model Checker](https://link.springer.com/chapter/10.1007/978-3-642-16310-4_14), 
  by Kyungmin Bae and José Meseguer, 
  _International Workshop on Rewriting Logic and its Applications (WRLA)_, 2010

- [A Rewriting-Based Model Checker for the Linear Temporal Logic of Rewriting](https://www.sciencedirect.com/science/article/pii/S1571066112000795), 
  by Kyungmin Bae and José Meseguer,  
  _International Workshop on Rule-Based Programming_, 2008

The following papers introduces the temporal logic of rewriting, the family of logics incorporating spatial action patterns.

- [The Temporal Logic of Rewriting: A Gentle Introduction](https://link.springer.com/chapter/10.1007/978-3-540-68679-8_22), 
  by José Meseguer, 
  _Concurrency, Graphs and Models, vol 5065 of LNCS_. Springer, 2008

- [The Temporal Logic of Rewriting](https://www.ideals.illinois.edu/items/11320), 
  by José Meseguer,
  Technical report, University of Illinois at Urbana-Champaign, 2007.
  http://hdl.handle.net/2142/11293