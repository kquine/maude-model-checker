noinst_LIBRARIES = libModelChecker.a

libModelChecker_a_CPPFLAGS = \
	-I$(top_srcdir)/src/Utility \
	-I$(top_srcdir)/src/Interface \
	-I$(top_srcdir)/src/Core \
	-I$(top_srcdir)/src/FullCompiler \
	-I$(top_srcdir)/src/Higher \
	-I$(top_srcdir)/src/Temporal \
	-I$(top_srcdir)/src/Mixfix \
	-I$(top_srcdir)/src/StrategyLanguage \
	-I$(top_srcdir)/src/BuiltIn \
	-I$(top_srcdir)/src/Variable \
	-I$(top_srcdir)/src/S_Theory \
	-I$(top_srcdir)/src/NA_Theory \
	-I$(top_srcdir)/src/FreeTheory \
	-I$(top_srcdir)/src/CUI_Theory \
	-I$(top_srcdir)/src/AU_Theory \
	-I$(top_srcdir)/src/ACU_Theory


libModelChecker_a_SOURCES = \
	DataStruct/TermUtil.cc \
	Graph/RewriteTransitionState.cc \
	Graph/StateDagContainer.cc \
	Graph/StateTransitionMetaGraph.cc \
	Graph/StateFoldingGraph.cc \
	Graph/Prop/SimplePropHandler.cc \
	Graph/Prop/StaticPropHandler.cc \
	Interface/PropChecker.cc \
	Interface/PropInterpreter.cc \
	Interface/ProofTermGenerator.cc \
	Interface/SubstConstructor.cc \
	Interface/CounterExampleGenerator.cc \
	Interface/FairnessDecoder.cc \
	Interface/DefinablePropGenerator.cc \
	Interface/FoldingChecker.cc \
	Interface/PrettyPrinter.cc \
	FairSet/WeakFairSet.cc \
	FairSet/StrongFairSet.cc \
	FairSet/ParamRealizedSet.cc \
	FairSet/ParamWeakFairSet.cc \
	FairSet/ParamStrongFairSet.cc \
	FairSet/CompositedFairSet.cc \
	Fairness/paramFairLattice.cc \
	Fairness/FormulaFairnessMap.cc \
	Fairness/SystemWeakFairnessMap.cc \
	Fairness/SystemStrongFairnessMap.cc \
	Fairness/CompositedFairnessMap.cc \
	Search/NDFSModelChecker.cc \
	Search/SCCModelChecker.cc \
	Search/SCCBuchiModelChecker.cc \
	Search/StreettModelChecker.cc \
	ltlrFairnessCheckerSymbol.cc \
	SymbolicModelCheckerSymbol.cc

	
noinst_HEADERS = \
	ltlrFairnessCheckerSymbol.hh \
	SymbolicModelCheckerSymbol.hh





	
