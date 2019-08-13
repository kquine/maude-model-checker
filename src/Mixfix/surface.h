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
     KW_CHECK = 274,
     KW_GET = 275,
     KW_VARIANTS = 276,
     KW_VARIANT = 277,
     KW_EREWRITE = 278,
     KW_FREWRITE = 279,
     KW_SREWRITE = 280,
     KW_CONTINUE = 281,
     KW_SEARCH = 282,
     KW_SET = 283,
     KW_SHOW = 284,
     KW_ON = 285,
     KW_OFF = 286,
     KW_TRACE = 287,
     KW_BODY = 288,
     KW_BUILTIN = 289,
     KW_WHOLE = 290,
     KW_SELECT = 291,
     KW_DESELECT = 292,
     KW_CONDITION = 293,
     KW_SUBSTITUTION = 294,
     KW_PRINT = 295,
     KW_GRAPH = 296,
     KW_MIXFIX = 297,
     KW_FLAT = 298,
     KW_ATTRIBUTE = 299,
     KW_NEWLINE = 300,
     KW_WITH = 301,
     KW_PARENS = 302,
     KW_ALIASES = 303,
     KW_GC = 304,
     KW_TIME = 305,
     KW_STATS = 306,
     KW_TIMING = 307,
     KW_CMD = 308,
     KW_BREAKDOWN = 309,
     KW_BREAK = 310,
     KW_PATH = 311,
     KW_MODULE = 312,
     KW_MODULES = 313,
     KW_VIEWS = 314,
     KW_ALL = 315,
     KW_SORTS = 316,
     KW_OPS2 = 317,
     KW_VARS = 318,
     KW_MBS = 319,
     KW_EQS = 320,
     KW_RLS = 321,
     KW_SUMMARY = 322,
     KW_KINDS = 323,
     KW_ADVISE = 324,
     KW_VERBOSE = 325,
     KW_DO = 326,
     KW_CLEAR = 327,
     KW_PROTECT = 328,
     KW_EXTEND = 329,
     KW_INCLUDE = 330,
     KW_EXCLUDE = 331,
     KW_CONCEAL = 332,
     KW_REVEAL = 333,
     KW_COMPILE = 334,
     KW_COUNT = 335,
     KW_DEBUG = 336,
     KW_IRREDUNDANT = 337,
     KW_RESUME = 338,
     KW_ABORT = 339,
     KW_STEP = 340,
     KW_WHERE = 341,
     KW_CREDUCE = 342,
     KW_SREDUCE = 343,
     KW_DUMP = 344,
     KW_PROFILE = 345,
     KW_NUMBER = 346,
     KW_RAT = 347,
     KW_COLOR = 348,
     SIMPLE_NUMBER = 349,
     KW_PWD = 350,
     KW_CD = 351,
     KW_PUSHD = 352,
     KW_POPD = 353,
     KW_LS = 354,
     KW_LOAD = 355,
     KW_QUIT = 356,
     KW_EOF = 357,
     KW_TEST = 358,
     KW_SMT_SEARCH = 359,
     KW_VU_NARROW = 360,
     KW_FVU_NARROW = 361,
     KW_ENDM = 362,
     KW_IMPORT = 363,
     KW_ENDV = 364,
     KW_SORT = 365,
     KW_SUBSORT = 366,
     KW_OP = 367,
     KW_OPS = 368,
     KW_MSGS = 369,
     KW_VAR = 370,
     KW_CLASS = 371,
     KW_SUBCLASS = 372,
     KW_MB = 373,
     KW_CMB = 374,
     KW_EQ = 375,
     KW_CEQ = 376,
     KW_RL = 377,
     KW_CRL = 378,
     KW_IS = 379,
     KW_FROM = 380,
     KW_ARROW = 381,
     KW_ARROW2 = 382,
     KW_PARTIAL = 383,
     KW_IF = 384,
     KW_LABEL = 385,
     KW_TO = 386,
     KW_COLON2 = 387,
     KW_ASSOC = 388,
     KW_COMM = 389,
     KW_ID = 390,
     KW_IDEM = 391,
     KW_ITER = 392,
     KW_LEFT = 393,
     KW_RIGHT = 394,
     KW_PREC = 395,
     KW_GATHER = 396,
     KW_METADATA = 397,
     KW_STRAT = 398,
     KW_POLY = 399,
     KW_MEMO = 400,
     KW_FROZEN = 401,
     KW_CTOR = 402,
     KW_LATEX = 403,
     KW_SPECIAL = 404,
     KW_CONFIG = 405,
     KW_OBJ = 406,
     KW_MSG = 407,
     KW_DITTO = 408,
     KW_FORMAT = 409,
     KW_ID_HOOK = 410,
     KW_OP_HOOK = 411,
     KW_TERM_HOOK = 412,
     KW_IN = 413,
     IDENTIFIER = 414,
     NUMERIC_ID = 415,
     ENDS_IN_DOT = 416,
     FORCE_LOOKAHEAD = 417
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
#define KW_CHECK 274
#define KW_GET 275
#define KW_VARIANTS 276
#define KW_VARIANT 277
#define KW_EREWRITE 278
#define KW_FREWRITE 279
#define KW_SREWRITE 280
#define KW_CONTINUE 281
#define KW_SEARCH 282
#define KW_SET 283
#define KW_SHOW 284
#define KW_ON 285
#define KW_OFF 286
#define KW_TRACE 287
#define KW_BODY 288
#define KW_BUILTIN 289
#define KW_WHOLE 290
#define KW_SELECT 291
#define KW_DESELECT 292
#define KW_CONDITION 293
#define KW_SUBSTITUTION 294
#define KW_PRINT 295
#define KW_GRAPH 296
#define KW_MIXFIX 297
#define KW_FLAT 298
#define KW_ATTRIBUTE 299
#define KW_NEWLINE 300
#define KW_WITH 301
#define KW_PARENS 302
#define KW_ALIASES 303
#define KW_GC 304
#define KW_TIME 305
#define KW_STATS 306
#define KW_TIMING 307
#define KW_CMD 308
#define KW_BREAKDOWN 309
#define KW_BREAK 310
#define KW_PATH 311
#define KW_MODULE 312
#define KW_MODULES 313
#define KW_VIEWS 314
#define KW_ALL 315
#define KW_SORTS 316
#define KW_OPS2 317
#define KW_VARS 318
#define KW_MBS 319
#define KW_EQS 320
#define KW_RLS 321
#define KW_SUMMARY 322
#define KW_KINDS 323
#define KW_ADVISE 324
#define KW_VERBOSE 325
#define KW_DO 326
#define KW_CLEAR 327
#define KW_PROTECT 328
#define KW_EXTEND 329
#define KW_INCLUDE 330
#define KW_EXCLUDE 331
#define KW_CONCEAL 332
#define KW_REVEAL 333
#define KW_COMPILE 334
#define KW_COUNT 335
#define KW_DEBUG 336
#define KW_IRREDUNDANT 337
#define KW_RESUME 338
#define KW_ABORT 339
#define KW_STEP 340
#define KW_WHERE 341
#define KW_CREDUCE 342
#define KW_SREDUCE 343
#define KW_DUMP 344
#define KW_PROFILE 345
#define KW_NUMBER 346
#define KW_RAT 347
#define KW_COLOR 348
#define SIMPLE_NUMBER 349
#define KW_PWD 350
#define KW_CD 351
#define KW_PUSHD 352
#define KW_POPD 353
#define KW_LS 354
#define KW_LOAD 355
#define KW_QUIT 356
#define KW_EOF 357
#define KW_TEST 358
#define KW_SMT_SEARCH 359
#define KW_VU_NARROW 360
#define KW_FVU_NARROW 361
#define KW_ENDM 362
#define KW_IMPORT 363
#define KW_ENDV 364
#define KW_SORT 365
#define KW_SUBSORT 366
#define KW_OP 367
#define KW_OPS 368
#define KW_MSGS 369
#define KW_VAR 370
#define KW_CLASS 371
#define KW_SUBCLASS 372
#define KW_MB 373
#define KW_CMB 374
#define KW_EQ 375
#define KW_CEQ 376
#define KW_RL 377
#define KW_CRL 378
#define KW_IS 379
#define KW_FROM 380
#define KW_ARROW 381
#define KW_ARROW2 382
#define KW_PARTIAL 383
#define KW_IF 384
#define KW_LABEL 385
#define KW_TO 386
#define KW_COLON2 387
#define KW_ASSOC 388
#define KW_COMM 389
#define KW_ID 390
#define KW_IDEM 391
#define KW_ITER 392
#define KW_LEFT 393
#define KW_RIGHT 394
#define KW_PREC 395
#define KW_GATHER 396
#define KW_METADATA 397
#define KW_STRAT 398
#define KW_POLY 399
#define KW_MEMO 400
#define KW_FROZEN 401
#define KW_CTOR 402
#define KW_LATEX 403
#define KW_SPECIAL 404
#define KW_CONFIG 405
#define KW_OBJ 406
#define KW_MSG 407
#define KW_DITTO 408
#define KW_FORMAT 409
#define KW_ID_HOOK 410
#define KW_OP_HOOK 411
#define KW_TERM_HOOK 412
#define KW_IN 413
#define IDENTIFIER 414
#define NUMERIC_ID 415
#define ENDS_IN_DOT 416
#define FORCE_LOOKAHEAD 417




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 105 "surface.yy"
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
#line 384 "surface.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



