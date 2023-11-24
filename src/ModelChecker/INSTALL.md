

LTLR/LMC model checker install instruction
==========================================

The followings are the step-by-step instruction for adding LTLR/LMC
model checker module into the Maude 3.3.1 source. We assume that the 
files are in the directory ModelChecker and the Maude source files 
are in the directory `$MAUDE`.

1. Copy ModelChecker folder into `$MAUDE/src`

2. Modify `$MAUDE/configure` (line 615)

  ```
    PACKAGE_STRING='Maude 3.3.1 LTLR&LMC'
  ```

3. Modify `$MAUDE/configuer.ac`
  - add `src/ModelChecker/Makefile` to line 423 (in `AC_CONFIG_FILES`)

4. Modify `Makefile.am` files
  - `$MAUDE/src/Makefile.am` 
    * add `ModelChecker \ ` to line 25 (in `SUBDIRS`)
  - `$MAUDE/src/Main/Makefile.am`
    * add `-I$(top_srcdir)/src/ModelChecker \ ` to line 32 (in `maude_CPPFLAGS`)
    * add `../ModelChecker/libModelChecker.a \ ` to line 58 (in `maude_LDADD`)
  - `$MAUDE/src/Mixfix/Makefile.am`
    * add `-I$(top_srcdir)/src/ModelChecker \ ` to line 32 (in `libmixfix_a_CPPFLAGS`)

5. Add the declaration for special symbols.
  - `$MAUDE/src/Mixfix/fancySymbols.cc`
    * add the following statements to line 132 (in the middle of the function declaration `MixfixModlue::newFancySymbol`)
  
    ```
      case SymbolType::LTLR_FAIRCHECKER_SYMBOL:
        return new modelChecker::LTLRFairnessCheckerSymbol(name, nrArgs);
      case SymbolType::SYMBOLIC_MODEL_CHECKER_SYMBOL:
        return new modelChecker::SymbolicModelCheckerSymbol(name, nrArgs);
    ```

  - `$MAUDE/src/Mixfix/mixfixModule.cc`
    * add the following includes at line 168

    ```
      //	ltlr and lmc model checkers
      #include `ltlrFairnessCheckerSymbol.hh`
      #include `SymbolicModelCheckerSymbol.hh`
    ```
  
  - `$MAUDE/src/Mixfix/specialSymbolTypes.cc`
    * add the following macro declarations to the last

    ```
      MACRO(LTLRFairnessCheckerSymbol, SymbolType::LTLR_FAIRCHECKER_SYMBOL)
      MACRO(SymbolicModelCheckerSymbol, SymbolType::SYMBOLIC_MODEL_CHECKER_SYMBOL)
    ```
  
  - `$MAUDE/src/Mixfix/symbolType.hh`
    *  add the following to line 88 (in `enum BasicTypes`)

    ```
      LTLR_FAIRCHECKER_SYMBOL,
      SYMBOLIC_MODEL_CHECKER_SYMBOL,
    ```
    
6. Run autoconf and automake again. Sometime you need to execute aclocal.

7. Follow the instruction for Maude sourcecode installation.

------

If you have any question or comments, please contact Kyungmin Bae (kmbae@postech.ac.kr).

