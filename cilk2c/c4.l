%{  /* -*- C -*- */
    /* Copyright (C) 1989,1990 James A. Roskind, All rights reserved. 
    This lexer description was written by James A.  Roskind.  Copying 
    of  this  file, as a whole, is permitted providing this notice is 
    intact  and  applicable   in   all   complete   copies.    Direct 
    translations  as a whole to other lexer generator input languages 
    (or lexical description languages)  is  permitted  provided  that 
    this  notice  is  intact and applicable in all such copies, along 
    with a disclaimer that  the  contents  are  a  translation.   The 
    reproduction  of derived files or text, such as modified versions 
    of this file, or the output of scanner generators, is  permitted, 
    provided   the  resulting  work  includes  the  copyright  notice 
    "Portions Copyright (c) 1989, 1990 James  A.   Roskind".  Derived 
    products  must  also  provide  the notice "Portions Copyright (c) 
    1989, 1990 James A.  Roskind" in  a  manner  appropriate  to  the 
    utility,   and  in  keeping  with  copyright  law  (e.g.:  EITHER 
    displayed when first invoked/executed; OR displayed  continuously 
    on  display terminal; OR via placement in the object code in form 
    readable in a printout, with or near the title of the work, or at 
    the end of the file).  No royalties, licenses or  commissions  of 
    any  kind  are  required  to copy this file, its translations, or 
    derivative products, when the copies are made in compliance  with 
    this  notice.  Persons  or  corporations  that  do make copies in 
    compliance  with  this  notice  may  charge  whatever  price   is 
    agreeable  to  a buyer, for such copies or derivative works. THIS 
    FILE IS PROVIDED ``AS IS'' AND WITHOUT  ANY  EXPRESS  OR  IMPLIED 
    WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES 
    OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

    James A. Roskind
    Independent Consultant
    516 Latania Palm Drive
    Indialantic FL, 32903
    (407)729-4348
    jar@ileaf.com
    or ...!uunet!leafusa!jar

    ---end of copyright notice---


COMMENTS-

My  goal  is  to  see  software  developers adopt my C++ grammar as a 
standard until such time as a better  standard  is  accessible.   The 
only  way  to  get it to become a standard, is to be sure that people 
know that derivations are based on a specific work.   The  intent  of 
releasing  this Flex input file is to facilitate experimentation with 
my C++ grammar. The intent  of  the  copyright  notice  is  to  allow 
arbitrary  commercial and non-commercial use of this file, as long as 
reference is given to my standardization effort.   Without  reference 
to  a specific standard, many alternative grammars would develop.  By 
referring to the standard, the C++ grammar is given publicity,  which 
should  lead  to further use in compatible products and systems.  The 
benefits  of  such  a  standard  to  commercial  products  (browsers, 
beautifiers,  translators,  compilers,  ...) should be obvious to the 
developers, in that other compatible products will  emerge,  and  the 
value  of  all  conforming  products  will rise.  Most developers are 
aware of the value of acquiring  a  fairly  complete  grammar  for  a 
language,  and  the  copyright  notice (and the resulting affiliation 
with my work) should not be too high a price to pay.  By copyrighting 
my work, I have some minor control over what this standard is, and  I 
can  (hopefully)  keep it from degrading without my approval.  I will 
consistently attempt to provide upgraded grammars that are  compliant 
with  the  current  art, and the ANSI C++ Committee recommendation in 
particular.  A  developer  is  never  prevented  from  modifying  the 
grammar or this file to improve it in whatever way is seen fit. There 
is  also  no  restriction on the sale of copies, or derivative works, 
providing the request in the copyright notice are satisfied.

If you are not "copying" my work, but  are  rather  only  abstracting 
some of my work, an acknowledgment with references to such a standard 
would  be  appreciated.  Specifically, agreements with my grammar and 
its resolution of otherwise ambiguous constructs, should be noted.

Simply put: "make whatever use you would like of the grammar and this 
file, but include the ``portions Copyright ...'' as  a  reference  to 
this standard."


*/

/*************************************************************************
 *
 *  C-to-C Translator
 *
 *  Adapted from Clean ANSI C Parser
 *  Eric A. Brewer, Michael D. Noakes
 *************************************************************************/
/*
 * Copyright (c) 1994-2003 Massachusetts Institute of Technology
 * Copyright (c) 2000 Matteo Frigo
 * Copyright (c) 2002 Bradley C. Kuszmaul
 * Copyright (c) 2003 Jim Sukha
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

/* Last modified 3/7/90, Version 1.00 */


/* This file is a dramatically cut down version  of  the  FLEX  input 
file  used in my ANSI C Preprocessor.  The executable version of my C 
preprocessor is available on many platforms (shareware), but this  is 
the  only source extract that is currently being distributed.  If you 
need  a  full  ANSI  C  preprocessor,   with   extensive   diagnostic 
capabilities  and  customization facilities, please contact me at the 
addresses given above.  Current platforms  include  IBMPC  (DOS/OS2), 
Sun  (SPARC  and  Motorola),  and  IBM R/6000.  ... end of commercial 
announcement.

This file is being distributed to facilitate experimentation and  use 
of my C and C++ grammar.


Comment  removal  must be done during the lexing, as context (such as 
enclosure in string literals) must be observed.   For  this  cut-down 
lexer,  we  will assume that comments have been removed (don't assume 
this if you are writing a compiler or browser!).   The  justification 
for   this   assumption   is   a   view   that   we   are  processing 
post-preprocessed source, and hence comment removal was taken care of 
during that phase. Note that in real life, comments CANNOT always  be 
removed  prior  to  lexing,  as  the  sequence of characters '/' '*' may 
appear within a string literal, and hence NOT indicate the start of a 
comment.

For each IDENTIFIER like string that  is  found,  there  are  several 
distinct interpretations that can be applied:

1)  The  preprocessor  may  interpret  the string as a "keyword" in a 
directive (eg: "pragma" or "include", "defined").

2) The parser may interpret the string as a keyword. (eg: "int").

3) Both parser and preprocessor may interpret the string as a keyword 
(eg: "if").

Since this file is based on source that actually  lexically  analyses 
text  for both preprocessing and parsing, macro definitions were used 
throughout.  The macro definitions supplied here have been customized 
to a C++ parse only, and all  preprocessor  keywords  are  passed  as 
IDENTIFIER  or  TYPEDEFname.  Also, since there is no symbol table to 
interrogate to decide whether a string is  a  TYPEDEFname,  I  simply 
assume  that  any identifier beginning with an upper case letter is a 
TYPEDEFname.  This hack should  allow  you  to  check  out  how  code 
segments  are  parsed using my grammar.  Unfortunately, if you really 
want to parse major league code, you have to write  a  symbol  table, 
and maintain appropriate scoping information.  :-), sorry...

*/


/* Included code before lex code */
/*************** Includes and Defines *****************************/
#include <ctype.h>
#include "ast.h"

FILE_IDENTITY(ident,
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/cilk2c/c4.l $ $LastChangedBy: bradley $ $Rev: 2743 $ $Date: 2006-02-12 13:34:00 -0500 (Sun, 12 Feb 2006) $");

PRIVATE inline void GetCoord(Coord *pcoord);
PRIVATE Node *char_constant(const char *);
PRIVATE Node *int_constant(const char *, Bool decimal, Bool octal);
PRIVATE Node *wide_char_const(const char *);
PRIVATE Node *float_constant(const char *s);
PRIVATE char *convert_escape_sequences(char *string);
PRIVATE void print_token(int token);
/*PRIVATE Bool HasSuffix(const char *filename, const char *suffix);*/
PRIVATE Bool Has_Cilk_Suffix(const char *filename, const char *input_language);
PRIVATE void parse_line_directive(char *s);
PRIVATE void save_pragma_directive(char *s);
PRIVATE void ignore_pragma_directive(char *s);
PRIVATE void EnterC(void);
PRIVATE void ExitC(void);

#define DEBUGLEX

#ifdef DEBUGLEX
#define RETURN_TOKEN(x) return (print_token(x), x)
#else
#define RETURN_TOKEN(x) return (x)
#endif

#define TAB_WIDTH  8

static char *line_start;

#undef YY_INPUT
#define YY_INPUT(buf, result, max_size) \
    if (fgets(buf, max_size, yyin)==0) { result=0; } \
    else { line_start = (char *)buf; result = strlen(buf); }

#undef yywrap

/**************************************************************************\
*  End of New code
\**************************************************************************/

/**************************************************************************\
 * Variables for diverting input to a string
\**************************************************************************/

 PRIVATE int previousParseType;
 YY_BUFFER_STATE lexInputStringBuffer;

 /* list for GetWildcardNames*/
 PRIVATE List* wcNameList;

 /* variables for NameAllWildcards*/
 PRIVATE char* newNamedWCString;
 PRIVATE int newStringIndex;
 PRIVATE int wcNum;
 
#define WC_EXTRA_DIGITS 3
#define WC_NUM_START 100
 
/**************************************************************************\
 * End of variables for string input
\**************************************************************************/ 
 
#if 0
/* y.tab.h now included by ast.h -- rcm */
#include "y.tab.h" /* YACC generated definitions based on C++ parser input*/
#endif


#define WHITE_RETURN(x)  /* do nothing,  */

#define PA_KEYWORD_RETURN(x) \
    RETURN_VAL(x)

/* PreProcessor and C keyword */
#define PPPA_KEYWORD_RETURN(x) \
    RETURN_VAL(x)

#define PP_KEYWORD_RETURN(x) \
   { GetCoord(&yylval.tok);\
     yylval.n = MakeIdCoord(UniqueString(yytext), yylval.tok);\
     if (IsAType(yylval.n->u.id.text)) \
       RETURN_TOKEN(TYPEDEFname); else RETURN_TOKEN(IDENTIFIER); }

/* PreProcess and Parser operator */
#define PPOP_RETURN(x) \
    RETURN_VAL((int)*yytext)

/* error: CPP ONLY operator;  Do nothing */
#define NAMED_PPOP_RETURN(x)

/* a single character operator */
#define ASCIIOP_RETURN(x)  RETURN_VAL((int)*yytext)

/* a multichar operator, with a name */
#define NAMEDOP_RETURN(x)  RETURN_VAL(x)             

#define RETURN_VAL(x) \
  {GetCoord(&yylval.tok); \
   RETURN_TOKEN(x);}


%}

%option nounput

%s C
%s CILK_WILDCARD
%s C_WILDCARD
%x WC_PARSE
%x WC_NAME


identifier ([a-zA-Z_][0-9a-zA-Z_]*)

exponent_part ([eE][-+]?[0-9]+)
fractional_constant ([0-9]*"."[0-9]+)|([0-9]+".")
floating_constant ((({fractional_constant}{exponent_part}?)|([0-9]+{exponent_part}))[FfLl]?)

integer_suffix_opt ([uU]?[lL]?[lL]?)|([lL][uU][lL]?)|([lL][lL][uU])
decimal_constant ("0"{integer_suffix_opt}|[1-9][0-9]*{integer_suffix_opt})
octal_constant ("0"[0-7]*{integer_suffix_opt})
hex_constant ("0"[xX][0-9a-fA-F]+{integer_suffix_opt})

/* the syntax of these escape sequences is checked more carefully below, which allows us to provide better error messages. */
simple_escape ([abfnrtv'"?\\])
octal_escape  ([0-7]+)
hex_escape ("x"[0-9a-fA-F]*)
bad_escape ([\\][^abfnrtv'"?\\x0-7\n ])

escape_sequence ([\\]({simple_escape}|{octal_escape}|{hex_escape}))
c_char ([^'\\\n]|{escape_sequence})
s_char ([^"\\\n]|{escape_sequence})

any_white ([ \011\013\014\015])
hwhite ([ \011])
     
unnamed_wc_d ("%d")
unnamed_wc_e ("%e")
unnamed_wc_t ("%t")

named_wc_d ("%d"[0-9]+)
named_wc_e ("%e"[0-9]+)
named_wc_t ("%t"[0-9]+)

wildcard_d ({unnamed_wc_d}|{named_wc_d})
wildcard_e ({unnamed_wc_e}|{named_wc_e})
wildcard_t ({unnamed_wc_t}|{named_wc_t})

%%

{any_white}+     {  WHITE_RETURN(' ');  }

"\n"             {  Line++; WHITE_RETURN(' ');  }

auto                {PA_KEYWORD_RETURN(AUTO);}
break               {PA_KEYWORD_RETURN(BREAK);}
case                {PA_KEYWORD_RETURN(CASE);}
char                {PA_KEYWORD_RETURN(CHAR);}
__complex__         {PA_KEYWORD_RETURN(COMPLEX);}
_Complex            {PA_KEYWORD_RETURN(COMPLEX);}  /* shows up in redhat 8 headers*/
const |
__const |
__const__           {PA_KEYWORD_RETURN(CONST);}
continue            {PA_KEYWORD_RETURN(CONTINUE);}
default             {PA_KEYWORD_RETURN(DEFAULT);}
define              {PP_KEYWORD_RETURN(DEFINE);}
defined             {PP_KEYWORD_RETURN(OPDEFINED);}
do                  {PA_KEYWORD_RETURN(DO);}
double              {PA_KEYWORD_RETURN(DOUBLE);}
elif                {PP_KEYWORD_RETURN(ELIF);}
else                {PPPA_KEYWORD_RETURN(ELSE);}
endif               {PP_KEYWORD_RETURN(ENDIF);}
enum                {PA_KEYWORD_RETURN(ENUM);}
error               {PP_KEYWORD_RETURN(ERROR);}
extern              {PA_KEYWORD_RETURN(EXTERN);}
float               {PA_KEYWORD_RETURN(FLOAT);}
for                 {PA_KEYWORD_RETURN(FOR);}
goto                {PA_KEYWORD_RETURN(GOTO);}
if                  {PPPA_KEYWORD_RETURN(IF);}
ifdef               {PP_KEYWORD_RETURN(IFDEF);}
ifndef              {PP_KEYWORD_RETURN(IFNDEF);}
__imag__            {PA_KEYWORD_RETURN(IMAG);}
include             {PP_KEYWORD_RETURN(INCLUDE); }
int                 {PA_KEYWORD_RETURN(INT);}
line                {PP_KEYWORD_RETURN(LINE);}
long                {PA_KEYWORD_RETURN(LONG);}
pragma              {PP_KEYWORD_RETURN(PRAGMA);}
__real__            {PA_KEYWORD_RETURN(REAL);} 
register            {PA_KEYWORD_RETURN(REGISTER);}
return              {PA_KEYWORD_RETURN(RETURN);}
short               {PA_KEYWORD_RETURN(SHORT);}
signed   |
__signed |
__signed__          {PA_KEYWORD_RETURN(SIGNED);}
sizeof              {PA_KEYWORD_RETURN(SIZEOF);}
__alignof__         {PA_KEYWORD_RETURN(ALIGNOF);}
__alignof           {PA_KEYWORD_RETURN(ALIGNOF);}
typeof              {PA_KEYWORD_RETURN(TYPEOF);}
__typeof__          {PA_KEYWORD_RETURN(TYPEOF);}
__typeof            {PA_KEYWORD_RETURN(TYPEOF);}
static              {PA_KEYWORD_RETURN(STATIC);}
struct              {PA_KEYWORD_RETURN(STRUCT);}
switch              {PA_KEYWORD_RETURN(SWITCH);}
typedef             {PA_KEYWORD_RETURN(TYPEDEF);}
undef               {PP_KEYWORD_RETURN(UNDEF);}
union               {PA_KEYWORD_RETURN(UNION);}
unsigned            {PA_KEYWORD_RETURN(UNSIGNED);}
void                {PA_KEYWORD_RETURN(VOID);}
volatile |
__volatile |
__volatile__        {PA_KEYWORD_RETURN(VOLATILE);}
while               {PA_KEYWORD_RETURN(WHILE);}
__restrict |
__restrict__        {PA_KEYWORD_RETURN(RESTRICT);}
__nooutput__        {PA_KEYWORD_RETURN(NOOUTPUT);}
__cilkonly__        {PA_KEYWORD_RETURN(CILKONLY);}
__cilksafe__        {PA_KEYWORD_RETURN(CILKSAFE);}
inline   |
__inline   |
__inline__          {PA_KEYWORD_RETURN(INLINE); }
__attribute__  |
__attribute         {PA_KEYWORD_RETURN(ATTRIBUTE); }
__builtin_va_arg    { PA_KEYWORD_RETURN(BUILTIN_VA_ARG); /* this support for stdarg.h is gcc-specific. */ }
"__declspec(dllexport)" {PA_KEYWORD_RETURN(DECLSPEC_EXPORT); /* gcc supports declspec (see bug 203) for microsoft c compatibility.  I am cheating by not doing the whitespace... */}
"__declspec(dllimport)" {PA_KEYWORD_RETURN(DECLSPEC_IMPORT); /* gcc supports declspec (see bug 203) for microsoft c compatibility. */}
asm |
__asm__ |
__asm               {PA_KEYWORD_RETURN(ASM);}
__extension__       {PA_KEYWORD_RETURN(EXTENSION);}

<INITIAL,CILK_WILDCARD>spawn           { PA_KEYWORD_RETURN(SPAWN); }
<INITIAL,CILK_WILDCARD>sync            { PA_KEYWORD_RETURN(SYNC); }
<INITIAL,CILK_WILDCARD>cilk            { PA_KEYWORD_RETURN(PROCEDURE); }
<INITIAL,CILK_WILDCARD>SYNCHED         { PA_KEYWORD_RETURN(SYNCHED); }

<INITIAL,CILK_WILDCARD>inlet           { PA_KEYWORD_RETURN(INLET); }
<INITIAL,CILK_WILDCARD>abort           { PA_KEYWORD_RETURN(ABORT); }
<INITIAL,CILK_WILDCARD>shared          { PA_KEYWORD_RETURN(SHAREDVAR); }
<INITIAL,CILK_WILDCARD>private         { PA_KEYWORD_RETURN(PRIVATEVAR); }
xbegin               {printf("Seeing xbegin \n"); PA_KEYWORD_RETURN(XBEGIN);}
xend                 {printf("Seeing xend   \n"); PA_KEYWORD_RETURN(XEND);}

{identifier} { GetCoord(&yylval.tok);
                          yylval.n = MakeIdCoord(UniqueString(yytext), 
                                                 yylval.tok);
                          if (IsAType(yylval.n->u.id.text))
                            RETURN_TOKEN(TYPEDEFname);
                          else RETURN_TOKEN(IDENTIFIER); }


{decimal_constant}  { yylval.n = int_constant(yytext, TRUE, FALSE);
                      RETURN_TOKEN(INTEGERconstant);}
{octal_constant}    { yylval.n = int_constant(yytext, FALSE, TRUE);
                      RETURN_TOKEN(OCTALconstant);}
{hex_constant}      { yylval.n = int_constant(yytext, FALSE, FALSE);
                      RETURN_TOKEN(HEXconstant);}

{floating_constant}[^.] { yyless(yyleng-1);
                          yylval.n = float_constant(yytext);
                          RETURN_TOKEN(FLOATINGconstant);}

[']{c_char}+[']     { yylval.n = char_constant(yytext);
                      RETURN_TOKEN(CHARACTERconstant); }

[']{c_char}*[\n]    { SyntaxError("Unmatched '");
                      yylval.n = char_constant("'X'");
		      Line++;
		      RETURN_TOKEN(CHARACTERconstant); }


['][^\\\n'][^'\n]+[']           |
['][']                          |
[']{bad_escape}[^'\n]*[']    { /* bad char constant */
                      yylval.n = char_constant("'X'");
                      SyntaxError(
                        "invalid char constant %s, treating as 'X'",
                        yytext);
                      RETURN_TOKEN(CHARACTERconstant);
                    }


"L"[']{c_char}+[']  { yylval.n = wide_char_const(yytext);
                          RETURN_TOKEN(WIDECHARconstant);
                        }


["]{s_char}*["]     { int length = strlen(yytext);
                      char *text = UniqueString(yytext);  /* save original */
                      yytext[length-1] = 0;  /* remove trailing double quote */
		      GetCoord(&yylval.tok);
                      /* remove leading double quote */
                      yylval.n = MakeStringTextCoord(
                        text,
			UniqueString(convert_escape_sequences(&yytext[1])),
					    yylval.tok);
                      RETURN_TOKEN(STRINGliteral);}

"L"["]{s_char}*["]  { int length = strlen(yytext);
                      char *text = UniqueString(yytext);  /* save original */
                      yytext[length-1] = 0;  /* remove trailing double quote */
		      GetCoord(&yylval.tok);
                      /* remove leading double quote */
                      yylval.n = MakeStringTextCoord(
                        text,
			UniqueString(convert_escape_sequences(&yytext[1])),
					    yylval.tok);
                      RETURN_TOKEN(STRINGliteral);}

["]{s_char}*{bad_escape}{s_char}*["]    { /* bad string constant */
    /* (unterminated string constants are caught by preprocessor) */
                      SyntaxError(
                        "string contains invalid escape code",
                        yytext);
		      GetCoord(&yylval.tok);
                      yylval.n = MakeStringTextCoord(UniqueString("\"\""),
                                            UniqueString(""),
                                            yylval.tok);
                      RETURN_TOKEN(STRINGliteral);
                    }


"("                  {PPOP_RETURN(LP);}
")"                  {PPOP_RETURN(RP);}
","                  {PPOP_RETURN(COMMA);}

^{hwhite}*"#"{hwhite}*[0-9].*"\n" |
^{hwhite}*"#"{hwhite}*line.*"\n" {
                     parse_line_directive(yytext); }

^{hwhite}*"#"{hwhite}*pragma{hwhite}+lang{hwhite}+"+"[Cc]{hwhite}*"\n" { EnterC(); Line++; }

^{hwhite}*"#"{hwhite}*pragma{hwhite}+lang{hwhite}+"-"[Cc]{hwhite}*"\n" { ExitC(); Line++; }

^{hwhite}*"#"{hwhite}*pragma{hwhite}+lang{hwhite}+"+"[Cc]"ilk"{hwhite}*"\n" { ExitC(); Line++; }
^{hwhite}*"#"{hwhite}*pragma{hwhite}+lang{hwhite}+"-"[Cc]"ilk"{hwhite}*"\n" { EnterC(); Line++; }

^{hwhite}*"#"{hwhite}*pragma{hwhite}+segments{hwhite}*"+"{hwhite}*"\n" { GenerateSegments = TRUE; Line++; }

^{hwhite}*"#"{hwhite}*pragma{hwhite}+segments{hwhite}*"-"{hwhite}*"\n" { GenerateSegments = FALSE; Line++; }

^{hwhite}*"#"{hwhite}*pragma{hwhite}+nd?{hwhite}+"+"{hwhite}*"\n" { MemoryCheckedp = MemoryChecking ? TRUE : FALSE; Line++; }

^{hwhite}*"#"{hwhite}*pragma{hwhite}+nd?{hwhite}+"-"{hwhite}*"\n" { MemoryCheckedp = FALSE; Line++; }

^{hwhite}*"#"{hwhite}*pragma{hwhite}+ndwarn?{hwhite}+"+"{hwhite}*"\n" { MemoryWarningp = TRUE; Line++; }

^{hwhite}*"#"{hwhite}*pragma{hwhite}+ndwarn?{hwhite}+"-"{hwhite}*"\n" { MemoryWarningp = FALSE; Line++; }

^{hwhite}*"#"{hwhite}*ident{hwhite}+["]{s_char}*["]"\n"                 {save_pragma_directive(yytext); Line++; /* sgi headers and older gcc headers had #ident. */}
^{hwhite}*"#"{hwhite}*pragma{hwhite}+ident{hwhite}+["]{s_char}*["].*"\n"  {save_pragma_directive(yytext); Line++; /* sgi headers and older gcc headers had #pragma ident. */}

^{hwhite}*"#"{hwhite}*pragma{hwhite}+optional{hwhite}+{identifier}{hwhite}*([,]{hwhite}*{identifier}{hwhite}*)*"\n" {save_pragma_directive(yytext); Line++;  /* sgi headers have optional symbols. */}

^{hwhite}*"#"{hwhite}*pragma{hwhite}+int_to_unsigned{hwhite}+{identifier}{hwhite}*"\n" {save_pragma_directive(yytext); Line++;  /* sgi headers have this, which can be ignored, and should be copied through. */}
^{hwhite}*"#"{hwhite}*pragma{hwhite}+intrinsic{hwhite}+"("{identifier}")"{hwhite}*"\n" {save_pragma_directive(yytext); Line++;  /* sgi headers have this, which can be ignored, and should be copied through. */}
^{hwhite}*"#"{hwhite}*pragma{hwhite}+no{hwhite}+side{hwhite}+effects{hwhite}+"("{identifier}")"{hwhite}*"\n" {save_pragma_directive(yytext); Line++;  /* sgi headers have this, which can be ignored, and should be copied through. */}
^{hwhite}*"#"{hwhite}*pragma{hwhite}+pack{hwhite}*"("{hwhite}*[0-9]*{hwhite}*")"{hwhite}*"\n" {save_pragma_directive(yytext); Line++;  /* linux headers have this SYSV pragma, which  should be copied through. */}
^{hwhite}*"#"{hwhite}*pragma{hwhite}+pack{hwhite}*"("{hwhite}*"push"{hwhite}*","{hwhite}*[0-9]*{hwhite}*")"{hwhite}*"\n" {save_pragma_directive(yytext); Line++;  /* linux headers have this SYSV pragma, which  should be copied through. */}
^{hwhite}*"#"{hwhite}*pragma{hwhite}+pack{hwhite}*"("{hwhite}*"pop"{hwhite}*")"{hwhite}*"\n" {save_pragma_directive(yytext); Line++;  /* linux headers have this SYSV pragma, which  should be copied through. */}
^{hwhite}*"#"{hwhite}*pragma{hwhite}+GCC{hwhite}*.*"\n" {save_pragma_directive(yytext); Line++;  /* linux headers have this SYSV pragma, which  should be copied through. */}

^{hwhite}*"#"{hwhite}*(pragma|ident).*"\n"                            {ignore_pragma_directive(yytext); Line++; }

"#"                  {NAMED_PPOP_RETURN('#') ;} 
"##"                 {NAMED_PPOP_RETURN(POUNDPOUND);}

"{"                  {ASCIIOP_RETURN(LC);}
"}"                  {ASCIIOP_RETURN(RC);}

"["                  {ASCIIOP_RETURN(LB);}
"]"                  {ASCIIOP_RETURN(RB);}
"."                  {ASCIIOP_RETURN(DOT);}
"&"                  {ASCIIOP_RETURN(AND);}
"*"                  {ASCIIOP_RETURN(STAR);}
"+"                  {ASCIIOP_RETURN(PLUS);}
"-"                  {ASCIIOP_RETURN(MINUS);}
"~"                  {ASCIIOP_RETURN(NEGATE);}
"!"                  {ASCIIOP_RETURN(NOT);}
"/"                  {ASCIIOP_RETURN(DIV);}
"%"                  {ASCIIOP_RETURN(MOD);}
"<"                  {ASCIIOP_RETURN(LT);}
">"                  {ASCIIOP_RETURN(GT);}
"^"                  {ASCIIOP_RETURN(XOR);}
"|"                  {ASCIIOP_RETURN(PIPE);}
"?"                  {ASCIIOP_RETURN(QUESTION);}
":"                  {ASCIIOP_RETURN(COLON);}
";"                  {ASCIIOP_RETURN(SEMICOLON);}
"="                  {ASCIIOP_RETURN(ASSIGN);}

"->"                 {NAMEDOP_RETURN(ARROW);}
"++"                 {NAMEDOP_RETURN(ICR);}
"--"                 {NAMEDOP_RETURN(DECR);}
"<<"                 {NAMEDOP_RETURN(LS);}
">>"                 {NAMEDOP_RETURN(RS);}
"<="                 {NAMEDOP_RETURN(LE);}
">="                 {NAMEDOP_RETURN(GE);}
"=="                 {NAMEDOP_RETURN(EQ);}
"!="                 {NAMEDOP_RETURN(NE);}
"&&"                 {NAMEDOP_RETURN(ANDAND);}
"||"                 {NAMEDOP_RETURN(OROR);}
"*="                 {NAMEDOP_RETURN(MULTassign);}
"/="                 {NAMEDOP_RETURN(DIVassign);}
"%="                 {NAMEDOP_RETURN(MODassign);}
"+="                 {NAMEDOP_RETURN(PLUSassign);}
"-="                 {NAMEDOP_RETURN(MINUSassign);}
"<<="                {NAMEDOP_RETURN(LSassign);}
">>="                {NAMEDOP_RETURN(RSassign);}
"&="                 {NAMEDOP_RETURN(ANDassign);}
"^="                 {NAMEDOP_RETURN(ERassign);}
"|="                 {NAMEDOP_RETURN(ORassign);}
"..."                {NAMEDOP_RETURN(ELLIPSIS);}


.                    { char tmp[20];
		       CharToText(tmp, *yytext);
                       SyntaxError("Unexpected character: %s", tmp); }

<C_WILDCARD,CILK_WILDCARD>{wildcard_d} { GetCoord(&yylval.tok);
                        yylval.n = MakeWildcardIdCoord(UniqueString(yytext), yylval.tok);
                        RETURN_TOKEN(WILDCARD_D); }

<C_WILDCARD,CILK_WILDCARD>{wildcard_e} { GetCoord(&yylval.tok);
                        yylval.n = MakeWildcardExpCoord(UniqueString(yytext), yylval.tok);
                        RETURN_TOKEN(WILDCARD_E); }

<C_WILDCARD,CILK_WILDCARD>{wildcard_t} { GetCoord(&yylval.tok);
                        yylval.n = MakeWildcardTypeCoord(UniqueString(yytext), yylval.tok);
                        RETURN_TOKEN(WILDCARD_T); }


<WC_PARSE>{wildcard_d}|{wildcard_e}|{wildcard_t} { wcNameList = AppendItem(wcNameList, UniqueString(yytext)); }

<WC_PARSE>[^%]* {}
<WC_PARSE>"%" {}
<WC_PARSE>"%%"  {}



<WC_NAME>{unnamed_wc_d}|{unnamed_wc_e}|{unnamed_wc_t} { int j = 0;
  int tempWildNum = wcNum;
  newNamedWCString[newStringIndex] = '%';
  newNamedWCString[newStringIndex+1] = yytext[1];
  for (j = WC_EXTRA_DIGITS - 1; j >= 0; j--) {
     newNamedWCString[newStringIndex+2+j] = (char)((tempWildNum%10) + '0');
     tempWildNum /= 10;
  }
  wcNum++;
  newStringIndex = newStringIndex + 2 + WC_EXTRA_DIGITS;
}

<WC_NAME>[^%]* {int length = strlen(yytext);
                memcpy(newNamedWCString+newStringIndex,
                       yytext,
                       length);
                newStringIndex = newStringIndex + length;
               }


<WC_NAME>{named_wc_d}|{named_wc_e}|{named_wc_t} { int length = strlen(yytext);
                memcpy(newNamedWCString+newStringIndex,
                       yytext,
                       length);
                newStringIndex = newStringIndex + length;
               }

<WC_NAME>"%"  {newNamedWCString[newStringIndex++] = '%'; }

<WC_NAME>"%%" {newNamedWCString[newStringIndex++] = '%'; 
               newNamedWCString[newStringIndex++] = '%'; }

%%
/*
 * I won't bother to provide any error recovery. I won't even handle 
 * unknown characters 
 */

/*******************************************************************/

GLOBAL extern Bool DebugLex;

PRIVATE inline void GetCoord(Coord * pcoord)
{
     char *s;
     int offset;

     pcoord->file = CurrentFile;
     pcoord->line = Line;

     offset = 0;
     for (s = line_start; s < yytext; ++s) {
	  if (*s == '\t')
	       offset = ((offset / TAB_WIDTH) + 1) * TAB_WIDTH;
	  else
	       ++offset;
     }
     pcoord->offset = offset;
}

PRIVATE void print_token(int token)
{
     if (DebugLex)
	  fprintf(stderr, "Lexer: \"%s\", token=%d  offset=%d\n", yytext, token, yylval.tok.offset);
}

PRIVATE int hex_digit(char in)
{
    if (in >= '0' && in <= '9') return(in - '0');
    if (in >= 'a' && in <= 'f') return(in - 'a' + 10);
    if (in >= 'A' && in <= 'F') return(in - 'A' + 10);
    return -1;
}

PRIVATE int octal_digit(char in)
{
    if (in >= '0' && in <= '7') return(in - '0');
    return -1;
}

PRIVATE int parse_char_constant(const char *s, int *actual_read)
{
     assert(s[0] == '\\');
     *actual_read = 2;

    switch (s[1]) {
      case 'n': return '\n';
      case 't': return '\t';
      case 'v': return '\v';
      case 'b': return '\b';
      case 'r': return '\r';
      case 'f': return '\f';
      case 'a': return '\a';
      case '\\': return '\\';
      case '?': return '\?';
      case '\'': return '\'';
      case '\"': return '\"';
      case 'x': {
	  int val=0, tmp, i;

	  for (i=2; (tmp = hex_digit(s[i])) >= 0; i++) {
	      val = 16 * val + tmp;
	  }
	  *actual_read = i;
	  if (i==2) {
	      SyntaxError("expecting hex digits after \\x");
	      return(-1);
	  }
	  if (val > TARGET_MAX_UCHAR) {
	      SyntaxError("explicit hex character (\\x) out of range");
	      return(-1);
	  }
	  return(val);
      }
      default:
	if (octal_digit(s[1]) >= 0) {  /* octal constant */
	    int val=0, tmp, i;
	    
	    for (i=1; (tmp = octal_digit(s[i])) >= 0  && i<4; i++) {
		val = 8 * val + tmp;
	    }
	    if (val > TARGET_MAX_UCHAR) {
		SyntaxError("explicit octal character out of range");
		return(-1);
	    }
	    *actual_read = i;
	    return(val);
	} else {
	    SyntaxError("unknown special character \"\\%c\"", s[1]);
	    return(-1);
	}
    }
    /* unreachable */
}

PRIVATE Node *char_constant(const char *s)
{
     int nread, value;
     Node *n;

     assert(s[0] == '\'');
     if (s[1] == '\\') {
	  value = parse_char_constant(&s[1], &nread);
	  if (value < 0)
	       value = 'X';

	  if (s[1 + nread] != '\'')
	       Warning(1,
	       "ignoring extra characters at end of char constant %s", s);
     } else {
	  value = s[1];
	  if (s[2] != '\'')
	       Warning(1,
	       "ignoring extra characters at end of char constant %s", s);
     }

     GetCoord(&yylval.tok);
     n = MakeConstSintTextCoord(UniqueString(s), value, yylval.tok);
     return (n);
}

PRIVATE Node *wide_char_const(const char *s)
{
     assert(s[0] == 'L' && s[1] == '\'');
     return (char_constant(&s[1]));
}

PRIVATE char *convert_escape_sequences(char *string)
{
     char *read_this, *write_this;
     int actual_read, val;

     write_this = read_this = string;

     while (*read_this != 0) {
	  if (*read_this == '\\') {
	       val = parse_char_constant(read_this, &actual_read);
	       if (val < 0) {
		    *write_this++ = 'X';
	       } else {
		    *write_this++ = (char) val;
	       }
	       read_this += actual_read;
	  } else {
	       *write_this++ = *read_this++;
	  }
     }
     *write_this = 0;
     return (string);
}

PRIVATE Node *int_constant(const char *s,
			   Bool decimal,
			   UNUSED(Bool octal) /* unused because strtoul understands the octal syntax. */
			   )
{
     Node *c;
     char ch;
     const char *p;
     Bool is_unsigned;
     int longness;		/* 0 == not, 1==long, 2==long long */
     TARGET_ULONG value;

     longness = 0;
     is_unsigned = !decimal;

     p = &s[strlen(s) - 1];	/* address of last char in string */
     while (!isxdigit((int)*p)) {
	  ch = toupper(*p);
	  if (ch == 'U') {
	       is_unsigned = TRUE;
	  } else if (ch == 'L') {
	       ++longness;
	  }
	  p--;
     }

     GetCoord(&yylval.tok);

     /* the following breaks if TARGET_ULONG is 'long long' */
     value = (TARGET_ULONG) strtoul(s, NULL, 0);

     if (longness == 0 && !is_unsigned && value <= TARGET_MAX_INT)
	  c = MakeConstSintTextCoord(UniqueString(s), (TARGET_INT) value, yylval.tok);
     else if (longness == 0 && value <= TARGET_MAX_UINT)
	  c = MakeConstUintTextCoord(UniqueString(s), (TARGET_UINT) value, yylval.tok);
     else if (longness == 1 && !is_unsigned && value <= TARGET_MAX_LONG)
	  c = MakeConstSlongTextCoord(UniqueString(s), (TARGET_LONG) value, yylval.tok);
     else if (longness == 1) {
	  if (value > TARGET_MAX_ULONG) {
	       SyntaxError("Constant `%s' is out of range", s);
	  }
	  c = MakeConstUlongTextCoord(UniqueString(s), (TARGET_ULONG) value, yylval.tok);
     } else {
	  assert(longness == 2);

	  if (ANSIOnly)
	       SyntaxErrorCoord(yylval.tok, "long long constants not allowed with -ansi switch");

	  /* store constant value as unsigned long, but cast it to
	   * appropriate long long type */
	  c = MakeImplicitCastCoord(
			     is_unsigned ? PrimUlonglong : PrimSlonglong,
				 MakeConstUlongTextCoord(UniqueString(s),
				       (TARGET_ULONG) value, yylval.tok),
					yylval.tok);
     }
     return (c);
}

PRIVATE Node *float_constant(const char *s)
{
     Node *c;
     char ch;
     const char *p;
     Bool is_long = FALSE, is_float = FALSE;
     double value;

     p = &s[strlen(s) - 1];	/* address of last char in string */
     while (!isdigit((int)*p)) {
	  ch = toupper(*p);
	  if (ch == 'F') {
	       is_float = TRUE;
	  } else if (ch == 'L') {
	       is_long = TRUE;
	  }
	  p--;
     }

     GetCoord(&yylval.tok);

     value = atof(s);

     if (is_float)
	  c = MakeConstFloatTextCoord(UniqueString(s), (float) value, yylval.tok);
     else if (is_long)
	  /* we don't directly represent long double constants -- store it
	   * as a double constant implicitly cast to long double */
	  c = MakeImplicitCastCoord(
					PrimLongdouble,
				MakeConstDoubleTextCoord(UniqueString(s),
						      value, yylval.tok),
					yylval.tok);
     else
	  c = MakeConstDoubleTextCoord(UniqueString(s), value, yylval.tok);

     return (c);
}

PRIVATE Bool HasSuffix(const char *filename, const char *suffix)
{
     int root_len = strlen(filename);
     int suffix_len = strlen(suffix);

     return (root_len >= suffix_len &&
	     !strcmp(filename + root_len - suffix_len, suffix));
}

PRIVATE Bool Has_Cilk_Suffix(const char *name, const char *input_language)
{
    /* Determine whether to keep cilk processing going, or to go into C mode.  */
    if (input_language) {
	return strcmp(input_language, "cilk")==0;
    } else {
	return HasSuffix(name, ".cilk") || HasSuffix(name, ".cilkp") || HasSuffix(name, ".cilkh") || HasSuffix(name, "cilki");
    }
}

GLOBAL char *Filename;
GLOBAL char *FileNames[MAX_FILES] = {"unknown"};
PRIVATE Bool File_is_C[MAX_FILES] = {FALSE};
PRIVATE Bool Current_is_C = FALSE;           /* Is the current file a C file? (FALSE -> it is a Cilk file).
					     * The "#pragma lang +c"  directive will make the current file be C.
					     * The "#pragma lang -c"  directive will make the current file be Cilk.
					     * Even if we do a #include, the current file state should remain sticky.
					     * However, we don't analyze the patterns of includes.  We only complain
					     * if the first time we run into an #included file, if we include a cilk file
					     * but are in C mode.
					     */
GLOBAL unsigned int CurrentFile = 0;
GLOBAL unsigned int known_files = 1;

GLOBAL void SetFile(const char *name, UNUSED(int line), const char *input_language)
{
     unsigned int i;
     char *uniq = UniqueString(name);

     for (i = 0; i < known_files; i++) {
	  if (uniq == FileNames[i])
	       break;
     }
     if (i == known_files) {
	  if (known_files == MAX_FILES) {
	       Warning(1, "Internal Error!");
	       fprintf(stderr, "too many distinct filenames");
	       i = CurrentFile;	/* keep using current file number */
	  } else {
	       FileNames[i] = uniq;
	       known_files++;

	       /*
		* starting new included file.  This file is included
		* if its parent was included or if it has an
		* included suffix. 
		*/
	       File_is_C[i] = Current_is_C || !Has_Cilk_Suffix(name, input_language);
	       if (Current_is_C && Has_Cilk_Suffix(name, input_language)) {
		    /*
		     * non-includable file included from includable
		     * file.  Issue a warning.
		     */
		    printf("%s:%d %s Has_Cilk_Suffix=%d\n", __FILE__, __LINE__, name, Has_Cilk_Suffix(name, input_language));
		    Warning(1,
			    "Non-includable file %s included from includable"
			    " file %s.", name, FileNames[CurrentFile]);
	       }
	  }
     }

     CurrentFile = i;
     Filename = FileNames[i];

     if (!Current_is_C && File_is_C[i]) {
	  /* entering an C file from Cilk.  Turn cilk processing off. */
	  EnterC();
     } else if (Current_is_C && !File_is_C[i]) {
	  /* leaving an included file.  Turn cilk processing on. */
	  ExitC();
     }
     /*printf("%s Current_is_C=%d\n", name, Current_is_C);*/
}

PRIVATE List *pragmas = NULL;        /* These are pragmas that were found after the most recent token was returned. */

/*
   Parse a traditional or ANSI-C line number specification.

   ANSI-C:   # line <n> \n
             # line <n> "<filename>" \n
   K&R       # <n> "<filename>" \n
*/
PRIVATE void parse_line_directive(char *s)
{
     char *p;
     int val;

     /* Read until a '#' sign */
     for (p = s; *p != '\n' && *p != '#'; p++);
     assert(*p != '\n');

     /* Read spaces and tabs */
     for (p = p + 1; *p != '\n' && (*p == ' ' || *p == '\t'); p++);
     assert(*p != '\n');

     /* Swallow "line" and any trailing whitespace */
     if (strncmp(p, "line", 4) == 0) {
	  p = p + 4;

	  /* There must be whitespace */
	  assert(*p == ' ' || *p == '\t');

	  for (; *p != '\n' && (*p == ' ' || *p == '\t'); p++);
	  assert(*p != '\n');
     }
     /* Read an integer from the string */
     if (isdigit((int)*p))
	  for (val = 0; *p && isdigit((int)*p); p++)
	       val = val * 10 + (*p - '0');
     else {
	  SyntaxError("#line error: expecting line number");
	  Line++;
	  return;
     }

     /* Skip whitespace */
     for (; *p != '\n' && (*p == ' ' || *p == '\t'); p++);

     /* Should be pointing at a '"' to start the filename or end of line */
     if (*p != '\n' && *p != '\"') {
	  SyntaxError("#line error: expecting a filename string");
	  Line++;
	  return;
     } else if (*p == '\"') {
	  char fname[MAX_FILENAME + 1];
	  int f;

	  for (f = 0, p++; *p != '\n' && *p != '\"' && f < MAX_FILENAME; p++, f++)
	       fname[f] = *p;
	  fname[f] = '\0';

	  if (*p != '"') {
	       SyntaxError("#line error: Filename string missing close `\"'");
	       Line++;
	       return;
	  } else if (f == MAX_FILENAME)
	       Warning(1, "#line error: filename truncated to %d characters",
		       MAX_FILENAME);
	  if (!IgnoreLineDirectives)
	       SetFile(fname, val, 0);
     }
     if (IgnoreLineDirectives)
	  Line++;
     else
	  Line = val;
}

/*
 * Save pragma directive for later collection into source tree.
 */
/* 
 * The machinery for dealing with pragmas is not quite right.
 * Before Bradley worked on it it was was `totally broken'
 * For example, the old mechanism transforms
 *
 * #pragma quux
 * int foo(int bar) { return bar; }
 *
 * into
 *
 * int foo(int bar) { 
 *    return bar; 
 * #pragma quux
 * }
 *
 * I don't see any easy fix within the current cilk2c structure.
 *
 * For some pragmas this does not matter, so we just do it.  For others we ignore the pragma and print a warning.
 *
 * (Who wrote that?  I don't know. -Bradley)
 *
 */

PRIVATE void save_pragma_directive(char *s)
{
     Coord coord;
     GetCoord(&coord);
     /*printf("See pragma %s\n", s);*/
     pragmas = AppendItem(pragmas,
			  MakeTextCoord(UniqueString(s), TRUE, coord));
     /*printf("\nPragmas are now: ");  PrintList(stdout, pragmas, 0); printf("\n");*/
}

PRIVATE void ignore_pragma_directive(char *s)
{
     Coord coord;
     GetCoord(&coord);
     s[strlen(s) - 1] = '\0';      /* remove trailing \n */
     WarningCoord(3, coord, "ignoring unrecognized directive: `%s'", s);
}

/* GrabPragmas is called by the parser at appropriate points
 * to gather pragmas into the source tree */
GLOBAL List *GrabPragmas(List * stmts_or_decls)
{
    /*printf("\nGrabbing pragmas:  The pragmas are:\n"); PrintList(stdout, pragmas, 0);*/
    /*printf("\n and statements:\n");                    PrintList(stdout, stmts_or_decls, 0); printf("\n");*/
     stmts_or_decls = JoinLists(pragmas, stmts_or_decls);
     pragmas = NULL;
     /*printf("\nGrabbed pragmas, now have\n"); PrintList(stdout, stmts_or_decls, 0); printf("\n");*/
     return stmts_or_decls;
}

int yywrap()
{
     return 1;
}

PRIVATE void EnterC()
{
     BEGIN(C);
     File_is_C[CurrentFile] = Current_is_C = TRUE;
}

PRIVATE void ExitC()
{
     BEGIN(INITIAL);
     File_is_C[CurrentFile] = Current_is_C = FALSE;
}

/**************************************************************************\
 * Functions for diverting input to a string
\**************************************************************************/

PRIVATE void EnterWildcardMode() {
  previousParseType = YY_START;
  if (previousParseType == C) {
    BEGIN(C_WILDCARD);
  }
  else {
    BEGIN(CILK_WILDCARD);
  }
}

PRIVATE void ExitWildcardMode() {
  BEGIN(previousParseType);
}


PRIVATE inline void EnterWildcardParseMode() {
  previousParseType = YY_START;
  BEGIN(WC_PARSE);
}

PRIVATE inline void ExitWildcardParseMode() {
  BEGIN(previousParseType);
}


PRIVATE inline void EnterWildcardNameMode() {
  previousParseType = YY_START;
  BEGIN(WC_NAME);
}

PRIVATE inline void ExitWildcardNameMode() {
  BEGIN(previousParseType);
}



 /* This function causes the lexer to read its input
    from the string that is passed in.
 */
GLOBAL void SwitchLexInputToString(const char *str) {
   char *tempString;
   /* printf("My string is: %s \n", str);*/
   if (str != NULL) {
     int length = strlen(str);
     /*     printf("The String is %s\n", str);*/
     tempString = (char *)malloc(sizeof(char)*(length + 3));
     /* printf("Tempstring is null? %d \n", (tempString == NULL));*/
     /*  printf("got here. \n");*/

     strcpy(tempString, str);
     tempString[length+1] = YY_END_OF_BUFFER_CHAR;
     tempString[length+2] = YY_END_OF_BUFFER_CHAR;

     line_start = tempString;

     /*     lexInputStringBuffer = yy_scan_string(str);*/

     lexInputStringBuffer = yy_scan_buffer(tempString, length+2);

     /*  printf("Now scanning from string\n");*/
     /*    printf("YYin == NULL? %d \n", (yyin == NULL));*/
     /*    printf("LexInputStringBuffer == current? %d \n", (lexInputStringBuffer == YY_CURRENT_BUFFER));*/


     Line = 1;
     CurrentFile = 0;
     EnterWildcardMode();
   }
 }

 /* After the input string has been parsed, call this function
    to clean up. */
GLOBAL void DeleteLexInputBuffer() {
    /*    printf("Deleting the buffer: \n");*/
   yy_delete_buffer(lexInputStringBuffer);
   ExitWildcardMode();
 }




/* Functions for parsing our format string with
   wildcards.  Portions of this code look quite similar to the
   SwitchLexInputBuffer and DeleteLexInputBuffer
   pair we have above.  
*/
GLOBAL List* GetWildcardNames(const char *str) {
   char *tempString;
   if (str != NULL) {
     int length = strlen(str);
     tempString = (char *)malloc(sizeof(char)*(length + 3));


     strcpy(tempString, str);
     tempString[length+1] = YY_END_OF_BUFFER_CHAR;
     tempString[length+2] = YY_END_OF_BUFFER_CHAR;

     lexInputStringBuffer = yy_scan_buffer(tempString, length+2);


     EnterWildcardParseMode();
     wcNameList = NULL;

     yyparse();

     yy_delete_buffer(lexInputStringBuffer);
     ExitWildcardParseMode();
   }
   return wcNameList;
}
       

/* This procedure takes all the unnamed
   wildcards in the string and assigns them
   unique names.  */
GLOBAL char* NameAllWildcards(const char *str) {
   char *tempString;
   if (str != NULL) {
     int length = strlen(str);
     int i = 0;
     int approxNumWildcards = 0;

     tempString = (char *)malloc(sizeof(char)*(length + 3));

     strcpy(tempString, str);
     tempString[length+1] = YY_END_OF_BUFFER_CHAR;
     tempString[length+2] = YY_END_OF_BUFFER_CHAR;

     lexInputStringBuffer = yy_scan_buffer(tempString, length+2);


     EnterWildcardNameMode();

     newStringIndex = 0;
     wcNum = WC_NUM_START;

     for (i = 0; i < length; i++) {
       if (str[i] == '%') {
	 approxNumWildcards++;
       }
     }

     /* Allocate some space for the new WC string. This
	is actually over-allocating - it assumes
	every % sign is a wildcard that must be named. */
     newNamedWCString = (char *)malloc(WC_EXTRA_DIGITS*approxNumWildcards
				       + length + 1);     
     yyparse();

     newNamedWCString[newStringIndex] = '\0';
     
     yy_delete_buffer(lexInputStringBuffer);
     ExitWildcardNameMode();
   }
   else {
     newNamedWCString = NULL;
   }
   return newNamedWCString;
}
