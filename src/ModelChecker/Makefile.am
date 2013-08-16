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
	DataStructure/TermUtil.cc \
	Param/RealizedPropGenerator.cc \
	Automaton/KripkeStructure.cc \
	Automaton/LabeledKripkeStructure.cc \
	Graph/RewriteTransitionState.cc \
	Graph/StateDagContainer.cc \
	GraphLabel/PropInterpreter.cc \
	GraphLabel/PropChecker.cc \
	GraphLabel/StatePropHandler.cc \
	GraphLabel/StateEventPropHandler.cc \
    Search/ModelCheckerFactory.cc \
    Search/NDFSModelChecker.cc \
    Search/SCCModelChecker.cc \
    Search/SCCBuchiModelChecker.cc \
    Search/StreettModelChecker.cc \
	Interface/ProofTermGenerator.cc \
	Interface/CounterExampleGenerator.cc \
	Interface/FairnessDecoder.cc \
	ltlrFairnessCheckerSymbol.cc 

	
noinst_HEADERS = \
	ltlrFairnessCheckerSymbol.hh \
	SymbolicModelCheckerSymbol.hh




	
