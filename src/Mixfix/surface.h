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
     KW_GET = 274,
     KW_VARIANTS = 275,
     KW_VARIANT = 276,
     KW_EREWRITE = 277,
     KW_FREWRITE = 278,
     KW_SREWRITE = 279,
     KW_CONTINUE = 280,
     KW_SEARCH = 281,
     KW_SET = 282,
     KW_SHOW = 283,
     KW_ON = 284,
     KW_OFF = 285,
     KW_TRACE = 286,
     KW_BODY = 287,
     KW_BUILTIN = 288,
     KW_WHOLE = 289,
     KW_SELECT = 290,
     KW_DESELECT = 291,
     KW_CONDITION = 292,
     KW_SUBSTITUTION = 293,
     KW_PRINT = 294,
     KW_GRAPH = 295,
     KW_MIXFIX = 296,
     KW_FLAT = 297,
     KW_ATTRIBUTE = 298,
     KW_NEWLINE = 299,
     KW_WITH = 300,
     KW_PARENS = 301,
     KW_ALIASES = 302,
     KW_GC = 303,
     KW_TIME = 304,
     KW_STATS = 305,
     KW_TIMING = 306,
     KW_CMD = 307,
     KW_BREAKDOWN = 308,
     KW_BREAK = 309,
     KW_PATH = 310,
     KW_MODULE = 311,
     KW_MODULES = 312,
     KW_VIEWS = 313,
     KW_ALL = 314,
     KW_SORTS = 315,
     KW_OPS2 = 316,
     KW_VARS = 317,
     KW_MBS = 318,
     KW_EQS = 319,
     KW_RLS = 320,
     KW_SUMMARY = 321,
     KW_KINDS = 322,
     KW_ADVISE = 323,
     KW_VERBOSE = 324,
     KW_DO = 325,
     KW_CLEAR = 326,
     KW_PROTECT = 327,
     KW_EXTEND = 328,
     KW_INCLUDE = 329,
     KW_EXCLUDE = 330,
     KW_CONCEAL = 331,
     KW_REVEAL = 332,
     KW_COMPILE = 333,
     KW_COUNT = 334,
     KW_DEBUG = 335,
     KW_RESUME = 336,
     KW_ABORT = 337,
     KW_STEP = 338,
     KW_WHERE = 339,
     KW_CREDUCE = 340,
     KW_SREDUCE = 341,
     KW_DUMP = 342,
     KW_PROFILE = 343,
     KW_NUMBER = 344,
     KW_RAT = 345,
     KW_COLOR = 346,
     SIMPLE_NUMBER = 347,
     KW_PWD = 348,
     KW_CD = 349,
     KW_PUSHD = 350,
     KW_POPD = 351,
     KW_LS = 352,
     KW_LOAD = 353,
     KW_QUIT = 354,
     KW_EOF = 355,
     KW_ENDM = 356,
     KW_IMPORT = 357,
     KW_ENDV = 358,
     KW_SORT = 359,
     KW_SUBSORT = 360,
     KW_OP = 361,
     KW_OPS = 362,
     KW_MSGS = 363,
     KW_VAR = 364,
     KW_CLASS = 365,
     KW_SUBCLASS = 366,
     KW_MB = 367,
     KW_CMB = 368,
     KW_EQ = 369,
     KW_CEQ = 370,
     KW_RL = 371,
     KW_CRL = 372,
     KW_IS = 373,
     KW_FROM = 374,
     KW_ARROW = 375,
     KW_ARROW2 = 376,
     KW_PARTIAL = 377,
     KW_IF = 378,
     KW_LABEL = 379,
     KW_TO = 380,
     KW_COLON2 = 381,
     KW_ASSOC = 382,
     KW_COMM = 383,
     KW_ID = 384,
     KW_IDEM = 385,
     KW_ITER = 386,
     KW_LEFT = 387,
     KW_RIGHT = 388,
     KW_PREC = 389,
     KW_GATHER = 390,
     KW_METADATA = 391,
     KW_STRAT = 392,
     KW_POLY = 393,
     KW_MEMO = 394,
     KW_FROZEN = 395,
     KW_CTOR = 396,
     KW_LATEX = 397,
     KW_SPECIAL = 398,
     KW_CONFIG = 399,
     KW_OBJ = 400,
     KW_MSG = 401,
     KW_DITTO = 402,
     KW_FORMAT = 403,
     KW_ID_HOOK = 404,
     KW_OP_HOOK = 405,
     KW_TERM_HOOK = 406,
     KW_IN = 407,
     IDENTIFIER = 408,
     NUMERIC_ID = 409,
     ENDS_IN_DOT = 410,
     FORCE_LOOKAHEAD = 411
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
#define KW_GET 274
#define KW_VARIANTS 275
#define KW_VARIANT 276
#define KW_EREWRITE 277
#define KW_FREWRITE 278
#define KW_SREWRITE 279
#define KW_CONTINUE 280
#define KW_SEARCH 281
#define KW_SET 282
#define KW_SHOW 283
#define KW_ON 284
#define KW_OFF 285
#define KW_TRACE 286
#define KW_BODY 287
#define KW_BUILTIN 288
#define KW_WHOLE 289
#define KW_SELECT 290
#define KW_DESELECT 291
#define KW_CONDITION 292
#define KW_SUBSTITUTION 293
#define KW_PRINT 294
#define KW_GRAPH 295
#define KW_MIXFIX 296
#define KW_FLAT 297
#define KW_ATTRIBUTE 298
#define KW_NEWLINE 299
#define KW_WITH 300
#define KW_PARENS 301
#define KW_ALIASES 302
#define KW_GC 303
#define KW_TIME 304
#define KW_STATS 305
#define KW_TIMING 306
#define KW_CMD 307
#define KW_BREAKDOWN 308
#define KW_BREAK 309
#define KW_PATH 310
#define KW_MODULE 311
#define KW_MODULES 312
#define KW_VIEWS 313
#define KW_ALL 314
#define KW_SORTS 315
#define KW_OPS2 316
#define KW_VARS 317
#define KW_MBS 318
#define KW_EQS 319
#define KW_RLS 320
#define KW_SUMMARY 321
#define KW_KINDS 322
#define KW_ADVISE 323
#define KW_VERBOSE 324
#define KW_DO 325
#define KW_CLEAR 326
#define KW_PROTECT 327
#define KW_EXTEND 328
#define KW_INCLUDE 329
#define KW_EXCLUDE 330
#define KW_CONCEAL 331
#define KW_REVEAL 332
#define KW_COMPILE 333
#define KW_COUNT 334
#define KW_DEBUG 335
#define KW_RESUME 336
#define KW_ABORT 337
#define KW_STEP 338
#define KW_WHERE 339
#define KW_CREDUCE 340
#define KW_SREDUCE 341
#define KW_DUMP 342
#define KW_PROFILE 343
#define KW_NUMBER 344
#define KW_RAT 345
#define KW_COLOR 346
#define SIMPLE_NUMBER 347
#define KW_PWD 348
#define KW_CD 349
#define KW_PUSHD 350
#define KW_POPD 351
#define KW_LS 352
#define KW_LOAD 353
#define KW_QUIT 354
#define KW_EOF 355
#define KW_ENDM 356
#define KW_IMPORT 357
#define KW_ENDV 358
#define KW_SORT 359
#define KW_SUBSORT 360
#define KW_OP 361
#define KW_OPS 362
#define KW_MSGS 363
#define KW_VAR 364
#define KW_CLASS 365
#define KW_SUBCLASS 366
#define KW_MB 367
#define KW_CMB 368
#define KW_EQ 369
#define KW_CEQ 370
#define KW_RL 371
#define KW_CRL 372
#define KW_IS 373
#define KW_FROM 374
#define KW_ARROW 375
#define KW_ARROW2 376
#define KW_PARTIAL 377
#define KW_IF 378
#define KW_LABEL 379
#define KW_TO 380
#define KW_COLON2 381
#define KW_ASSOC 382
#define KW_COMM 383
#define KW_ID 384
#define KW_IDEM 385
#define KW_ITER 386
#define KW_LEFT 387
#define KW_RIGHT 388
#define KW_PREC 389
#define KW_GATHER 390
#define KW_METADATA 391
#define KW_STRAT 392
#define KW_POLY 393
#define KW_MEMO 394
#define KW_FROZEN 395
#define KW_CTOR 396
#define KW_LATEX 397
#define KW_SPECIAL 398
#define KW_CONFIG 399
#define KW_OBJ 400
#define KW_MSG 401
#define KW_DITTO 402
#define KW_FORMAT 403
#define KW_ID_HOOK 404
#define KW_OP_HOOK 405
#define KW_TERM_HOOK 406
#define KW_IN 407
#define IDENTIFIER 408
#define NUMERIC_ID 409
#define ENDS_IN_DOT 410
#define FORCE_LOOKAHEAD 411




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
#line 372 "surface.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



