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
	-I$(top_srcdir)/src/ACU_Theory \
	-I$(top_srcdir)/src/3rdParty


libModelChecker_a_SOURCES = \
	Automaton/KripkeStructure.cc \
	Automaton/LabeledKripkeStructure.cc \
	Formula/PropSet.cc \
	Formula/PropositionTable.cc \
	Formula/PropChecker.cc \
	Formula/EnabledPropTransferer.cc \
	Formula/FormulaBuilder.cc \
	Formula/ParamPropSet.cc \
	Formula/ParamPropositionTable.cc \
	Formula/ParamSubstitution.cc \
	Formula/ParamSubstitutionBuilder.cc \
	Formula/ParamPropChecker.cc \
	Formula/ParamEnabledPropTransferer.cc \
	Formula/ParamVarInfo.cc \
	Formula/RealizedPropGenerator.cc \
	Fairness/FairnessTable.cc \
	Fairness/ParamFairnessTable.cc \
	Fairness/FairnessChecker.cc \
	Fairness/WeakFairnessChecker.cc \
	Fairness/ParamWeakFairnessChecker.cc \
	Fairness/StrongFairnessChecker.cc \
	Fairness/ParamStrongFairnessChecker.cc \
	Fairness/CompositeFairnessChecker.cc \
	Fairness/WeakFairSet.cc \
	Fairness/StrongFairSet.cc \
	Fairness/CompositeFairSet.cc \
	Graph/RewriteTransitionState.cc \
	Graph/StateDagContainer.cc \
	Graph/LabelPropHandler.cc \
	Graph/LabelFairHandler.cc \
    Search/ModelCheckerFactory.cc \
    Search/NDFSModelChecker.cc \
    Search/SCCModelChecker.cc \
    Search/SCCBuchiModelChecker.cc \
    Search/StreettModelChecker.cc \
    Interface/PropInterpreter.cc \
    Interface/PropEvaluator.cc \
    Interface/FairnessDecoder.cc \
    Interface/ProofTermGenerator.cc \
    Interface/CounterExampleGenerator.cc \
    Utility/TermUtil.cc \
    ltlrFairnessCheckerSymbol.cc 
	

noinst_HEADERS = \
	ltlrFairnessCheckerSymbol.hh \
	SymbolicModelCheckerSymbol.hh




	
