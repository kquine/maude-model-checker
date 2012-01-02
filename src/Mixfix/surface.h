/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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
/* Tokens.  */
#define FILE_NAME_STRING 258
#define UNINTERPRETED_STRING 259
#define LATEX_STRING 260
#define KW_MOD 261
#define KW_OMOD 262
#define KW_VIEW 263
#define KW_PARSE 264
#define KW_NORMALIZE 265
#define KW_REDUCE 266
#define KW_REWRITE 267
#define KW_LOOP 268
#define KW_NARROW 269
#define KW_XG_NARROW 270
#define KW_MATCH 271
#define KW_XMATCH 272
#define KW_UNIFY 273
#define KW_EREWRITE 274
#define KW_FREWRITE 275
#define KW_SREWRITE 276
#define KW_CONTINUE 277
#define KW_SEARCH 278
#define KW_SET 279
#define KW_SHOW 280
#define KW_ON 281
#define KW_OFF 282
#define KW_TRACE 283
#define KW_BODY 284
#define KW_BUILTIN 285
#define KW_WHOLE 286
#define KW_SELECT 287
#define KW_DESELECT 288
#define KW_CONDITION 289
#define KW_SUBSTITUTION 290
#define KW_PRINT 291
#define KW_GRAPH 292
#define KW_MIXFIX 293
#define KW_FLAT 294
#define KW_ATTRIBUTE 295
#define KW_NEWLINE 296
#define KW_WITH 297
#define KW_PARENS 298
#define KW_ALIASES 299
#define KW_GC 300
#define KW_TIME 301
#define KW_STATS 302
#define KW_TIMING 303
#define KW_CMD 304
#define KW_BREAKDOWN 305
#define KW_BREAK 306
#define KW_PATH 307
#define KW_MODULE 308
#define KW_MODULES 309
#define KW_VIEWS 310
#define KW_ALL 311
#define KW_SORTS 312
#define KW_OPS2 313
#define KW_VARS 314
#define KW_MBS 315
#define KW_EQS 316
#define KW_RLS 317
#define KW_SUMMARY 318
#define KW_KINDS 319
#define KW_ADVISE 320
#define KW_VERBOSE 321
#define KW_DO 322
#define KW_CLEAR 323
#define KW_PROTECT 324
#define KW_EXTEND 325
#define KW_INCLUDE 326
#define KW_EXCLUDE 327
#define KW_CONCEAL 328
#define KW_REVEAL 329
#define KW_COMPILE 330
#define KW_COUNT 331
#define KW_DEBUG 332
#define KW_RESUME 333
#define KW_ABORT 334
#define KW_STEP 335
#define KW_WHERE 336
#define KW_CREDUCE 337
#define KW_SREDUCE 338
#define KW_DUMP 339
#define KW_PROFILE 340
#define KW_NUMBER 341
#define KW_RAT 342
#define KW_COLOR 343
#define SIMPLE_NUMBER 344
#define KW_PWD 345
#define KW_CD 346
#define KW_PUSHD 347
#define KW_POPD 348
#define KW_LS 349
#define KW_LOAD 350
#define KW_QUIT 351
#define KW_EOF 352
#define KW_ENDM 353
#define KW_IMPORT 354
#define KW_ENDV 355
#define KW_SORT 356
#define KW_SUBSORT 357
#define KW_OP 358
#define KW_OPS 359
#define KW_MSGS 360
#define KW_VAR 361
#define KW_CLASS 362
#define KW_SUBCLASS 363
#define KW_MB 364
#define KW_CMB 365
#define KW_EQ 366
#define KW_CEQ 367
#define KW_RL 368
#define KW_CRL 369
#define KW_IS 370
#define KW_FROM 371
#define KW_ARROW 372
#define KW_ARROW2 373
#define KW_PARTIAL 374
#define KW_IF 375
#define KW_LABEL 376
#define KW_TO 377
#define KW_COLON2 378
#define KW_ASSOC 379
#define KW_COMM 380
#define KW_ID 381
#define KW_IDEM 382
#define KW_ITER 383
#define KW_LEFT 384
#define KW_RIGHT 385
#define KW_PREC 386
#define KW_GATHER 387
#define KW_METADATA 388
#define KW_STRAT 389
#define KW_POLY 390
#define KW_MEMO 391
#define KW_FROZEN 392
#define KW_CTOR 393
#define KW_LATEX 394
#define KW_SPECIAL 395
#define KW_CONFIG 396
#define KW_OBJ 397
#define KW_MSG 398
#define KW_DITTO 399
#define KW_FORMAT 400
#define KW_ID_HOOK 401
#define KW_OP_HOOK 402
#define KW_TERM_HOOK 403
#define KW_IN 404
#define IDENTIFIER 405
#define NUMERIC_ID 406
#define ENDS_IN_DOT 407
#define FORCE_LOOKAHEAD 408




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 103 "surface.yy"
{
  bool yyBool;
  Int64 yyInt64;
  const char* yyString;
  Token yyToken;
  ImportModule::ImportMode yyImportMode;
  Interpreter::Flags yyFlags;
  Interpreter::PrintFlags yyPrintFlags;
  Interpreter::SearchKind yySearchKind;
}
/* Line 1529 of yacc.c.  */
#line 366 "surface.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



