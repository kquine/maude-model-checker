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
     KW_GENERATE = 274,
     KW_VARIANTS = 275,
     KW_EREWRITE = 276,
     KW_FREWRITE = 277,
     KW_SREWRITE = 278,
     KW_CONTINUE = 279,
     KW_SEARCH = 280,
     KW_SET = 281,
     KW_SHOW = 282,
     KW_ON = 283,
     KW_OFF = 284,
     KW_TRACE = 285,
     KW_BODY = 286,
     KW_BUILTIN = 287,
     KW_WHOLE = 288,
     KW_SELECT = 289,
     KW_DESELECT = 290,
     KW_CONDITION = 291,
     KW_SUBSTITUTION = 292,
     KW_PRINT = 293,
     KW_GRAPH = 294,
     KW_MIXFIX = 295,
     KW_FLAT = 296,
     KW_ATTRIBUTE = 297,
     KW_NEWLINE = 298,
     KW_WITH = 299,
     KW_PARENS = 300,
     KW_ALIASES = 301,
     KW_GC = 302,
     KW_TIME = 303,
     KW_STATS = 304,
     KW_TIMING = 305,
     KW_CMD = 306,
     KW_BREAKDOWN = 307,
     KW_BREAK = 308,
     KW_PATH = 309,
     KW_MODULE = 310,
     KW_MODULES = 311,
     KW_VIEWS = 312,
     KW_ALL = 313,
     KW_SORTS = 314,
     KW_OPS2 = 315,
     KW_VARS = 316,
     KW_MBS = 317,
     KW_EQS = 318,
     KW_RLS = 319,
     KW_SUMMARY = 320,
     KW_KINDS = 321,
     KW_ADVISE = 322,
     KW_VERBOSE = 323,
     KW_DO = 324,
     KW_CLEAR = 325,
     KW_PROTECT = 326,
     KW_EXTEND = 327,
     KW_INCLUDE = 328,
     KW_EXCLUDE = 329,
     KW_CONCEAL = 330,
     KW_REVEAL = 331,
     KW_COMPILE = 332,
     KW_COUNT = 333,
     KW_DEBUG = 334,
     KW_RESUME = 335,
     KW_ABORT = 336,
     KW_STEP = 337,
     KW_WHERE = 338,
     KW_CREDUCE = 339,
     KW_SREDUCE = 340,
     KW_DUMP = 341,
     KW_PROFILE = 342,
     KW_NUMBER = 343,
     KW_RAT = 344,
     KW_COLOR = 345,
     SIMPLE_NUMBER = 346,
     KW_PWD = 347,
     KW_CD = 348,
     KW_PUSHD = 349,
     KW_POPD = 350,
     KW_LS = 351,
     KW_LOAD = 352,
     KW_QUIT = 353,
     KW_EOF = 354,
     KW_ENDM = 355,
     KW_IMPORT = 356,
     KW_ENDV = 357,
     KW_SORT = 358,
     KW_SUBSORT = 359,
     KW_OP = 360,
     KW_OPS = 361,
     KW_MSGS = 362,
     KW_VAR = 363,
     KW_CLASS = 364,
     KW_SUBCLASS = 365,
     KW_MB = 366,
     KW_CMB = 367,
     KW_EQ = 368,
     KW_CEQ = 369,
     KW_RL = 370,
     KW_CRL = 371,
     KW_IS = 372,
     KW_FROM = 373,
     KW_ARROW = 374,
     KW_ARROW2 = 375,
     KW_PARTIAL = 376,
     KW_IF = 377,
     KW_LABEL = 378,
     KW_TO = 379,
     KW_COLON2 = 380,
     KW_ASSOC = 381,
     KW_COMM = 382,
     KW_ID = 383,
     KW_IDEM = 384,
     KW_ITER = 385,
     KW_LEFT = 386,
     KW_RIGHT = 387,
     KW_PREC = 388,
     KW_GATHER = 389,
     KW_METADATA = 390,
     KW_STRAT = 391,
     KW_POLY = 392,
     KW_MEMO = 393,
     KW_FROZEN = 394,
     KW_CTOR = 395,
     KW_LATEX = 396,
     KW_SPECIAL = 397,
     KW_CONFIG = 398,
     KW_OBJ = 399,
     KW_MSG = 400,
     KW_DITTO = 401,
     KW_FORMAT = 402,
     KW_ID_HOOK = 403,
     KW_OP_HOOK = 404,
     KW_TERM_HOOK = 405,
     KW_IN = 406,
     IDENTIFIER = 407,
     NUMERIC_ID = 408,
     ENDS_IN_DOT = 409,
     FORCE_LOOKAHEAD = 410
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
#define KW_GENERATE 274
#define KW_VARIANTS 275
#define KW_EREWRITE 276
#define KW_FREWRITE 277
#define KW_SREWRITE 278
#define KW_CONTINUE 279
#define KW_SEARCH 280
#define KW_SET 281
#define KW_SHOW 282
#define KW_ON 283
#define KW_OFF 284
#define KW_TRACE 285
#define KW_BODY 286
#define KW_BUILTIN 287
#define KW_WHOLE 288
#define KW_SELECT 289
#define KW_DESELECT 290
#define KW_CONDITION 291
#define KW_SUBSTITUTION 292
#define KW_PRINT 293
#define KW_GRAPH 294
#define KW_MIXFIX 295
#define KW_FLAT 296
#define KW_ATTRIBUTE 297
#define KW_NEWLINE 298
#define KW_WITH 299
#define KW_PARENS 300
#define KW_ALIASES 301
#define KW_GC 302
#define KW_TIME 303
#define KW_STATS 304
#define KW_TIMING 305
#define KW_CMD 306
#define KW_BREAKDOWN 307
#define KW_BREAK 308
#define KW_PATH 309
#define KW_MODULE 310
#define KW_MODULES 311
#define KW_VIEWS 312
#define KW_ALL 313
#define KW_SORTS 314
#define KW_OPS2 315
#define KW_VARS 316
#define KW_MBS 317
#define KW_EQS 318
#define KW_RLS 319
#define KW_SUMMARY 320
#define KW_KINDS 321
#define KW_ADVISE 322
#define KW_VERBOSE 323
#define KW_DO 324
#define KW_CLEAR 325
#define KW_PROTECT 326
#define KW_EXTEND 327
#define KW_INCLUDE 328
#define KW_EXCLUDE 329
#define KW_CONCEAL 330
#define KW_REVEAL 331
#define KW_COMPILE 332
#define KW_COUNT 333
#define KW_DEBUG 334
#define KW_RESUME 335
#define KW_ABORT 336
#define KW_STEP 337
#define KW_WHERE 338
#define KW_CREDUCE 339
#define KW_SREDUCE 340
#define KW_DUMP 341
#define KW_PROFILE 342
#define KW_NUMBER 343
#define KW_RAT 344
#define KW_COLOR 345
#define SIMPLE_NUMBER 346
#define KW_PWD 347
#define KW_CD 348
#define KW_PUSHD 349
#define KW_POPD 350
#define KW_LS 351
#define KW_LOAD 352
#define KW_QUIT 353
#define KW_EOF 354
#define KW_ENDM 355
#define KW_IMPORT 356
#define KW_ENDV 357
#define KW_SORT 358
#define KW_SUBSORT 359
#define KW_OP 360
#define KW_OPS 361
#define KW_MSGS 362
#define KW_VAR 363
#define KW_CLASS 364
#define KW_SUBCLASS 365
#define KW_MB 366
#define KW_CMB 367
#define KW_EQ 368
#define KW_CEQ 369
#define KW_RL 370
#define KW_CRL 371
#define KW_IS 372
#define KW_FROM 373
#define KW_ARROW 374
#define KW_ARROW2 375
#define KW_PARTIAL 376
#define KW_IF 377
#define KW_LABEL 378
#define KW_TO 379
#define KW_COLON2 380
#define KW_ASSOC 381
#define KW_COMM 382
#define KW_ID 383
#define KW_IDEM 384
#define KW_ITER 385
#define KW_LEFT 386
#define KW_RIGHT 387
#define KW_PREC 388
#define KW_GATHER 389
#define KW_METADATA 390
#define KW_STRAT 391
#define KW_POLY 392
#define KW_MEMO 393
#define KW_FROZEN 394
#define KW_CTOR 395
#define KW_LATEX 396
#define KW_SPECIAL 397
#define KW_CONFIG 398
#define KW_OBJ 399
#define KW_MSG 400
#define KW_DITTO 401
#define KW_FORMAT 402
#define KW_ID_HOOK 403
#define KW_OP_HOOK 404
#define KW_TERM_HOOK 405
#define KW_IN 406
#define IDENTIFIER 407
#define NUMERIC_ID 408
#define ENDS_IN_DOT 409
#define FORCE_LOOKAHEAD 410




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
#line 370 "surface.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



