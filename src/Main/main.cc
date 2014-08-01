/*

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2010 SRI International, Menlo Park, CA 94025, USA.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.

*/

//
//	main() function and misc functions.
//
#include "sys/param.h"
#ifdef ALPHA
#include <stropts.h>
#endif

//      utility stuff
#include "macros.hh"
#include "vector.hh"
#include "tty.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "higher.hh"
#include "freeTheory.hh"
#include "builtIn.hh"
#include "strategyLanguage.hh"
#include "mixfix.hh"
 
//      interface class definitions
#include "term.hh"
 
//      core class definitions
#include "lineNumber.hh"

//      built in stuff
#include "randomOpSymbol.hh"

//      system class definitions
#include "IO_Manager.hh"

//	mixfix frontend definitions
#include "token.hh"
#include "userLevelRewritingContext.hh"
#include "fileTable.hh"
#include "directoryManager.hh"
#include "mixfixModule.hh"
#include "interpreter.hh"
#include "global.hh"


#include "cvc4/expr/expr_manager.h"
#include "cvc4/smt/smt_engine.h"

using namespace CVC4;

void
testCVC4()
{
  ExprManager em;
  SmtEngine smt(&em);
  Expr onePlusTwo = em.mkExpr(kind::PLUS,
                              em.mkConst(Rational(1)),
                              em.mkConst(Rational(2)));
  std::cout << Expr::setlanguage(language::output::LANG_CVC4)
            << smt.getInfo("name")
            << " says that 1 + 2 = "
            << smt.simplify(onePlusTwo)
            << std::endl;
}

void
test2()
{
  ExprManager em;
  Expr helloworld = em.mkVar("Hello World!", em.booleanType());
  SmtEngine smt(&em);
  std::cout << helloworld << " is " << smt.query(helloworld) << std::endl;
}

void
test3()
{
  ExprManager em;
  SmtEngine smt(&em);
  smt.setOption("incremental", SExpr("true"));  // Enable incremental solving

  Type real = em.realType();
  Type integer = em.integerType();

  Expr x = em.mkVar("x", integer);
  Expr y = em.mkVar("y", real);

  Expr three = em.mkConst(Rational(3));
  Expr neg2 = em.mkConst(Rational(-2));
  Expr two_thirds = em.mkConst(Rational(2,3));

  Expr three_y = em.mkExpr(kind::MULT, three, y);  // 3y
  Expr diff = em.mkExpr(kind::MINUS, y, x);  // y - x

  Expr x_geq_3y = em.mkExpr(kind::GEQ, x, three_y);  // x >= 3y
  Expr x_leq_y = em.mkExpr(kind::LEQ, x, y);  // x <= y
  Expr neg2_lt_x = em.mkExpr(kind::LT, neg2, x);  // -2 < x

  Expr assumptions = em.mkExpr(kind::AND, x_geq_3y, x_leq_y, neg2_lt_x);  // x >= 3y /\ x <= y /\ -2 < x
  smt.assertFormula(assumptions);

  //smt.push();
  Expr diff_leq_two_thirds = em.mkExpr(kind::LEQ, diff, two_thirds);   // y - x <= 2/3
  cout << "Prove that " << diff_leq_two_thirds << " with CVC4." << endl;
  cout << "CVC4 should report VALID." << endl;
  cout << "Result from CVC4 is: " << smt.query(diff_leq_two_thirds) << endl;
  //smt.pop();

  smt.push();
  Expr diff_is_two_thirds = em.mkExpr(kind::EQUAL, diff, two_thirds);  // y - x = 2/3
  smt.assertFormula(diff_is_two_thirds);
  cout << "Show that the asserts are consistent with " << endl;
  cout << diff_is_two_thirds << " with CVC4." << endl;
  cout << "CVC4 should report SAT." << endl;
  cout << "Result from CVC4 is: " << smt.checkSat(em.mkConst(true)) << endl;
  smt.pop();

}


void
test4()
{
  ExprManager em;
  SmtEngine smt(&em);
  smt.setOption("incremental", SExpr("true"));  // Enable incremental solving

  Type integer = em.integerType();

  Expr x = em.mkVar("x", integer);
  Expr y = em.mkVar("y", integer);
  
  Expr x_geq_y = em.mkExpr(kind::GEQ, x, y);
  Expr max_x_y = em.mkExpr(kind::ITE, x_geq_y, x, y);

  Expr max_x_y_geq_x = em.mkExpr(kind::GEQ, max_x_y, x);

  cout << "valid = " << smt.query(max_x_y_geq_x) << endl;

  smt.push();
  smt.assertFormula(max_x_y_geq_x);
  cout << "sat = " << smt.checkSat(em.mkConst(true)) << endl;
  smt.pop();

}



int
main(int argc, char* argv[])
{
  void testSeq();

  //testSeq();
  //testCVC4();
  //test2();
  //test3();
  //test4();
  //
  //	Global function declatations
  //
  void printBanner(ostream& s);
  void printHelp(const char* name);
  void printVersion();
  void createRootBuffer(FILE* fp, bool forceInteractive);
  bool includeFile(const string& directory, const string& fileName, bool silent, int lineNr);
  extern Vector<char*> pendingFiles;
  const char* isFlag(const char* arg, const char* flag);
  bool findExecutableDirectory(string& directory, string& executable);
  bool findPrelude(string& directory, string& fileName);
  void checkForPending();

  bool lineWrapping = true;
  bool handleCtrlC = true;
  bool readPrelude = true;
  bool forceInteractive = false;
  bool outputBanner = true;
  int ansiColor = UNDECIDED;
  int useTecla = UNDECIDED;

  for (int i = 1; i < argc; i++)
    {
      char* arg = argv[i];
      if (arg[0] == '-')
	{
	  if (const char* s = isFlag(arg, "-xml-log="))
	    interpreter.beginXmlLog(s);
	  else if (const char* s = isFlag(arg, "-random-seed="))
	    RandomOpSymbol::setGlobalSeed(strtoul(s, 0, 0));
	  else if (strcmp(arg, "--help") == 0)
	    printHelp(argv[0]);
	  else if (strcmp(arg, "--version") == 0)
	    printVersion();
	  else if (strcmp(arg, "-no-mixfix") == 0)
	    interpreter.setPrintFlag(Interpreter::PRINT_MIXFIX, false);
	  else if (strcmp(arg, "-ansi-color") == 0)
	    ansiColor = true;
	  else if (strcmp(arg, "-no-ansi-color") == 0)
	    ansiColor = false;
	  else if (strcmp(arg, "-tecla") == 0)
	    useTecla = true;
	  else if (strcmp(arg, "-no-tecla") == 0)
	    useTecla = false;
	  else if (strcmp(arg, "-no-prelude") == 0)
	    readPrelude = false;
	  else if (strcmp(arg, "-no-banner") == 0)
	    outputBanner = false;
	  else if (strcmp(arg, "-no-advise") == 0)
	    {
	      if (!alwaysAdviseFlag)
		globalAdvisoryFlag = false;
	    }
	  else if (strcmp(arg, "-always-advise") == 0)
	    {
	      alwaysAdviseFlag = true;
	      globalAdvisoryFlag = true;
	    }
	  else if (strcmp(arg, "-no-wrap") == 0)
	    lineWrapping = false;
	  else if (strcmp(arg, "-batch") == 0)
	    handleCtrlC = false;
	  else if (strcmp(arg, "-interactive") == 0)
	    forceInteractive = true;
	  else
	    {
	      IssueWarning(LineNumber(FileTable::COMMAND_LINE) <<
			   ": unrecognised flag: " << QUOTE(arg));
	    }
	}
      else
	pendingFiles.append(arg);
    }

  if (lineWrapping)
    ioManager.setAutoWrap();

  if (ansiColor == UNDECIDED)
    {
      //
      //	By default we allow ANSI escape codes unless
      //	environment variable TERM is set to dumb; or
      //	our standard output is not a terminal.
      //
      ansiColor = true;
      const char* term = getenv("TERM");
      if ((term != 0 && strcmp("dumb", term) == 0) ||
	  isatty(STDOUT_FILENO) == 0)
	ansiColor = false;
    }
  Tty::setEscapeSequencesAllowed(ansiColor);

  if (useTecla == UNDECIDED)
    {
      //
      //	By default we use tecla for input unless
      //	environment variable TERM is set to emacs or dumb;
      //	or our standard input is not a terminal.
      //
      useTecla = true;
      const char* term = getenv("TERM");
      if ((term != 0 && (strcmp("emacs", term) == 0 ||
			 strcmp("dumb", term) == 0)) ||
	  isatty(STDIN_FILENO) == 0)
	useTecla = false;
    }

  if (outputBanner)
    printBanner(cout);
  createRootBuffer(stdin, forceInteractive);
  UserLevelRewritingContext::setHandlers(handleCtrlC);
  if (useTecla)
    ioManager.setCommandLineEditing();
  directoryManager.initialize();
  string executable(argv[0]);
  findExecutableDirectory(executableDirectory, executable);
  if (readPrelude)
    {
      string directory;
      string fileName(PRELUDE_NAME);
      if (findPrelude(directory, fileName))
	includeFile(directory, fileName, true, FileTable::AUTOMATIC);
    }
  else
    checkForPending();  // because we won't hit an EOF
  (void) UserLevelRewritingContext::commandLoop();
  return 0;
}

ostream& operator<<(ostream& s, const LineNumber& lineNumber)
{
  fileTable.printLineNumber(s, lineNumber.getLineNumber());
  return s;
}

const char*
isFlag(const char* arg, const char* flag)
{
  char f;
  do
    {
      f = *flag++;
      if (f == 0)
        return arg;
    }
  while (f == *arg++);
  return 0;
}

void
printHelp(const char* name)
{
  cout <<
    "Maude interpreter\n" <<
    "Usage: " << name << " [options] [files]\n" <<
    "Options:\n" <<
    "  --help\t\tDisplay this information\n" <<
    "  --version\t\tDisplay version number\n" <<
    "  -no-prelude\t\tDo not read in the standard prelude\n" <<
    "  -no-banner\t\tDo not output banner on startup\n" <<
    "  -no-advise\t\tNo advisories on startup\n" <<
    "  -always-advise\t\tAlways show advisories regardless" <<
    "  -no-mixfix\t\tDo not use mixfix notation for output\n" <<
    "  -no-wrap\t\tDo not automatic line wrapping for output\n" <<
    "  -ansi-color\t\tUse ANSI control sequences\n" <<
    "  -no-ansi-color\tDo not use ANSI control sequences\n" <<
    "  -tecla\t\tUse tecla command line editing\n" <<
    "  -no-tecla\t\tDo not use tecla command line editing\n" <<
    "  -batch\t\tRun in batch mode\n" <<
    "  -interactive\t\tRun in interactive mode\n" <<
    "  -random-seed=<int>\tSet seed for random number generator\n" <<
    "  -xml-log=<filename>\tSet file in which to produce an xml log\n" <<
    "\n" <<
    "Send bug reports to: " << PACKAGE_BUGREPORT << endl;
  exit(0);
}

void
printVersion()
{
  cout << PACKAGE_VERSION << endl;
  exit(0);
}

bool
findExecutableDirectory(string& directory, string& executable)
{
  string::size_type p = executable.rfind('/');
  if (p == string::npos)
    return directoryManager.searchPath("PATH", directory, executable, X_OK);
  else
    {
      directoryManager.realPath(executable.substr(0, p), directory);
      return directoryManager.checkAccess(directory, executable, X_OK);
    }
}

bool
findPrelude(string& directory, string& fileName)
{
  if (directoryManager.searchPath(MAUDE_LIB, directory, fileName, R_OK))
    return true;
  if (!(executableDirectory.empty()) &&
      directoryManager.checkAccess(executableDirectory, fileName, R_OK))
    {
      directory = executableDirectory;
      return true;
    }
  if (directoryManager.checkAccess(".", fileName, R_OK))
    {
      directory = ".";
      return true;
    }
  IssueWarning(LineNumber(FileTable::AUTOMATIC) <<
	       ": unable to locate file: " << QUOTE(fileName));
  return false;
}

#include "sequenceAssignment.hh"

int delannoy(int m, int n);
int countAssignments(int nrLhsVars, int nrRhsVars);

void
testSeq()
{
  int lhsMax = 10;
  int rhsMax = 10;

  for (int i = 0; i < lhsMax; ++i)
    {
      for (int j = 0; j < rhsMax; ++j)
	{
	  int d = delannoy(i, j);
	  int a = countAssignments(i + 1, j + 1);
	  if (a == d)
	    cout << "correct " << d << endl;
	  else
	    cout << "fail " << a << " vs " << d << endl;
	}
      cout << endl;
    }
}


int
countAssignments(int nrLhsVars, int nrRhsVars)
{
  SequenceAssignment s(nrLhsVars, nrRhsVars);

  int nrSols = 0;
  while (s.findNextSolution(nrSols == 0))
    ++nrSols;

  return nrSols;
}

  /*
  int nrLhsVars = 5;
  int nrRhsVars = 4;

  SequenceAssignment s(nrLhsVars, nrRhsVars);

  s.setLhsBound(2, 1);
  s.setRhsBound(1, 1);
  s.setRhsBound(2, 1);

  int solNr = 1;
  while (s.findNextSolution())
    {
      Vector<Vector<int> > lhsAssign(nrLhsVars);
      Vector<Vector<int> > rhsAssign(nrRhsVars);

      int freeVarIndex = 0;
      int lIndex = 0;
      int rIndex = 0;

      lhsAssign[lIndex].append(freeVarIndex);
      rhsAssign[rIndex].append(freeVarIndex);

      const SequenceAssignment::Solution& sol = s.getSolution();
      cout << "(sol " << solNr << ") ";
      FOR_EACH_CONST(i, SequenceAssignment::Solution, sol)
	{
	  cout << " " << *i;
	  ++freeVarIndex;
	  lIndex += SequenceAssignment::leftDelta(*i);
	  rIndex += SequenceAssignment::rightDelta(*i);
	  lhsAssign[lIndex].append(freeVarIndex);
	  rhsAssign[rIndex].append(freeVarIndex);
	}
      cout << endl;
      {
	int v = 0;
	FOR_EACH_CONST(j, Vector<Vector<int> >, lhsAssign)
	  {
	    cout << "L" << v << " <-";
	    FOR_EACH_CONST(k, Vector<int>, *j)
	      cout << " X" << *k;
	    cout << endl;
	    ++v;
	  }
      }
      {
	int v = 0;
	FOR_EACH_CONST(j, Vector<Vector<int> >, rhsAssign)
	  {
	    cout << "R" << v << " <-";
	    FOR_EACH_CONST(k, Vector<int>, *j)
	      cout << " X" << *k;
	    cout << endl;
	    ++v;
	  }
      }

      cout << endl;
      ++solNr;
    }
  */

#include <map>

int
delannoy(int m, int n)
{
  typedef pair<int, int> Pair;
  typedef map<Pair, int> Memo;
  static Memo memo;

  if (m == 0 || n == 0)
    return 1;

  Pair p(m, n);
  Memo::const_iterator i = memo.find(p);
  if (i != memo.end())
    return i->second;

  int d = delannoy(m - 1, n) + delannoy(m - 1, n - 1) + delannoy(m, n - 1);
  memo.insert(Memo::value_type(p, d));
  return d;
}

