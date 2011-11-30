
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     FILE_NAME_STRING = 258,
     UNINTERPRETED_STRING = 259,
     LATEX_STRING = 260,
     KW_MOD = 261,
     KW_OMOD = 262,
     KW_VIEW = 263,
     KW_PARSE = 264,
     KW_NORMALIZE = 265,
     KW_REDUCE = 266,
     KW_REWRITE = 267,
     KW_LOOP = 268,
     KW_NARROW = 269,
     KW_XG_NARROW = 270,
     KW_MATCH = 271,
     KW_XMATCH = 272,
     KW_UNIFY = 273,
     KW_EREWRITE = 274,
     KW_FREWRITE = 275,
     KW_SREWRITE = 276,
     KW_CONTINUE = 277,
     KW_SEARCH = 278,
     KW_SET = 279,
     KW_SHOW = 280,
     KW_ON = 281,
     KW_OFF = 282,
     KW_TRACE = 283,
     KW_BODY = 284,
     KW_BUILTIN = 285,
     KW_WHOLE = 286,
     KW_SELECT = 287,
     KW_DESELECT = 288,
     KW_CONDITION = 289,
     KW_SUBSTITUTION = 290,
     KW_PRINT = 291,
     KW_GRAPH = 292,
     KW_MIXFIX = 293,
     KW_FLAT = 294,
     KW_ATTRIBUTE = 295,
     KW_NEWLINE = 296,
     KW_WITH = 297,
     KW_PARENS = 298,
     KW_ALIASES = 299,
     KW_GC = 300,
     KW_TIME = 301,
     KW_STATS = 302,
     KW_TIMING = 303,
     KW_CMD = 304,
     KW_BREAKDOWN = 305,
     KW_BREAK = 306,
     KW_PATH = 307,
     KW_MODULE = 308,
     KW_MODULES = 309,
     KW_VIEWS = 310,
     KW_ALL = 311,
     KW_SORTS = 312,
     KW_OPS2 = 313,
     KW_VARS = 314,
     KW_MBS = 315,
     KW_EQS = 316,
     KW_RLS = 317,
     KW_SUMMARY = 318,
     KW_KINDS = 319,
     KW_ADVISE = 320,
     KW_VERBOSE = 321,
     KW_DO = 322,
     KW_CLEAR = 323,
     KW_PROTECT = 324,
     KW_EXTEND = 325,
     KW_INCLUDE = 326,
     KW_EXCLUDE = 327,
     KW_CONCEAL = 328,
     KW_REVEAL = 329,
     KW_COMPILE = 330,
     KW_COUNT = 331,
     KW_DEBUG = 332,
     KW_RESUME = 333,
     KW_ABORT = 334,
     KW_STEP = 335,
     KW_WHERE = 336,
     KW_CREDUCE = 337,
     KW_SREDUCE = 338,
     KW_DUMP = 339,
     KW_PROFILE = 340,
     KW_NUMBER = 341,
     KW_RAT = 342,
     KW_COLOR = 343,
     SIMPLE_NUMBER = 344,
     KW_PWD = 345,
     KW_CD = 346,
     KW_PUSHD = 347,
     KW_POPD = 348,
     KW_LS = 349,
     KW_LOAD = 350,
     KW_QUIT = 351,
     KW_EOF = 352,
     KW_ENDM = 353,
     KW_IMPORT = 354,
     KW_ENDV = 355,
     KW_SORT = 356,
     KW_SUBSORT = 357,
     KW_OP = 358,
     KW_OPS = 359,
     KW_MSGS = 360,
     KW_VAR = 361,
     KW_CLASS = 362,
     KW_SUBCLASS = 363,
     KW_MB = 364,
     KW_CMB = 365,
     KW_EQ = 366,
     KW_CEQ = 367,
     KW_RL = 368,
     KW_CRL = 369,
     KW_IS = 370,
     KW_FROM = 371,
     KW_ARROW = 372,
     KW_ARROW2 = 373,
     KW_PARTIAL = 374,
     KW_IF = 375,
     KW_LABEL = 376,
     KW_TO = 377,
     KW_COLON2 = 378,
     KW_ASSOC = 379,
     KW_COMM = 380,
     KW_ID = 381,
     KW_IDEM = 382,
     KW_ITER = 383,
     KW_LEFT = 384,
     KW_RIGHT = 385,
     KW_PREC = 386,
     KW_GATHER = 387,
     KW_METADATA = 388,
     KW_STRAT = 389,
     KW_POLY = 390,
     KW_MEMO = 391,
     KW_FROZEN = 392,
     KW_CTOR = 393,
     KW_LATEX = 394,
     KW_SPECIAL = 395,
     KW_CONFIG = 396,
     KW_OBJ = 397,
     KW_MSG = 398,
     KW_DITTO = 399,
     KW_FORMAT = 400,
     KW_ID_HOOK = 401,
     KW_OP_HOOK = 402,
     KW_TERM_HOOK = 403,
     KW_IN = 404,
     IDENTIFIER = 405,
     NUMERIC_ID = 406,
     ENDS_IN_DOT = 407,
     FORCE_LOOKAHEAD = 408
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 103 "surface.yy"

  bool yyBool;
  Int64 yyInt64;
  const char* yyString;
  Token yyToken;
  ImportModule::ImportMode yyImportMode;
  Interpreter::Flags yyFlags;
  Interpreter::PrintFlags yyPrintFlags;
  Interpreter::SearchKind yySearchKind;



/* Line 1676 of yacc.c  */
#line 218 "surface.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif




