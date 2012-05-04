/*============================================================================*/
/*     ____    _        _    ____    _        _   _     _         __          */
/*    / __ \   \\      //   / __ \   \\      //  | |   / /       /  \         */
/*   / /  \_\   \\    //   / /  \_\   \\    //   | |  / /       / /\ \        */
/*  / /____      \\  //   / /____      \\  //    | | / /       / /  \ \       */
/*  |  ___ \      \\//    |  ___ \      \\//     | |/ /       / /____\ \      */
/*  | /   \ \      //     | /   \ \      //      |   |       / ________ \     */
/*  \ \___/ /     //      \ \___/ /     //       | |\ \     / /        \ \    */
/*   \_____/    _//        \_____/    _//        | | \ \   / /          \ \   */
/*             |_/                   |_/         |_|  \_\ /_/            \_\  */
/*                                                                            */
/*============================================================================*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "b_lisp.h"

/*============================================================================*/
/* ��������� ����������� ������� �� ���������� ��������.                      */
/*============================================================================*/
#pragma warning(push)
#pragma warning(disable : 4996)

/*============================================================================*/
/* ������������ ����                                                          */
/*============================================================================*/
typedef int                  bint   ;
typedef unsigned int         buint  ;
typedef double               bdouble;
typedef void                 bvoid  ;
typedef wchar_t              bchar  ;
typedef enum {bfalse, btrue} bbool  ;

/*============================================================================*/
/* bLisp_Function                                                             */
/*============================================================================*/
typedef struct bLisp_Function
{
	buint arg_num;
	buint ptr    ;
	buint end_ptr;
} bLisp_Function;
/*============================================================================*/
/* bLisp_NativeFunction                                                       */
/*============================================================================*/
typedef struct bLisp_NativeFunction
{
	bvoid               *ptr    ;
	bchar               *name   ;
	enum bLisp_TypeClass rt     ;
	enum bLisp_TypeClass at[16] ;
	buint                at_size;
} bLisp_NativeFunction;
/*============================================================================*/
/* bLisp_TypeClassList                                                        */
/*============================================================================*/
typedef struct bLisp_TypeClassList
{
	enum bLisp_TypeClass        type;
	struct bLisp_TypeClassList *next;
} bLisp_TypeClassList;
/*============================================================================*/
/* bLisp_TypeClass                                                            */
/*============================================================================*/
typedef enum
{
	bLisp_IntType    ,
	bLisp_FloatType  ,
	bLisp_StringType ,
	bLisp_LambdaType ,
	bLisp_NativeType ,
	bLisp_DataType   ,
	bLisp_ListType   ,
	bLisp_VoidType   ,
	bLisp_UnknownType
} bLisp_TypeClass;
/*============================================================================*/
/* bLisp_TypeValue                                                            */
/*============================================================================*/
union bLisp_TypeValue
{
	int                    int_value     ;
	bdouble                float_value   ;
	bchar                 *string_value  ;
	bLisp_Function        *lambda_value  ;
	bLisp_NativeFunction  *native_value  ;
	bvoid                 *data_value    ;
	struct bLisp_TypeList *list_value    ;
};
/*============================================================================*/
/* bLisp_Type                                                                 */
/*============================================================================*/
typedef struct bLisp_Type
{
	bLisp_TypeClass       type ;
	union bLisp_TypeValue value;
} bLisp_Type;
/*============================================================================*/
/* bLisp_TypeList                                                             */
/*============================================================================*/
typedef struct bLisp_TypeList
{
	bLisp_Type value           ;
	struct bLisp_TypeList *next;
} bLisp_TypeList;
/*============================================================================*/
/* bLisp_TokenClass                                                           */
/*============================================================================*/
enum bLisp_TokenClass
{
	bLisp_NameToken       ,
	bLisp_LeftBraceToken  ,
	bLisp_RightBraceToken ,
	bLisp_SharpToken      ,
	bLisp_IncludeToken    ,
	bLisp_DigitToken      ,
	bLisp_FloatToken      ,
	bLisp_StringToken     ,
	bLisp_CommentToken    ,
	bLisp_PlusToken       ,
	bLisp_MinusToken      ,
	bLisp_MultiplyToken   ,
	bLisp_DivideToken     ,
	bLisp_EqualToken      ,
	bLisp_LessToken       ,
	bLisp_MoreToken       ,
	bLisp_DefineToken     ,
	bLisp_IfToken         ,
	bLisp_BeginToken      ,
	bLisp_SetToken        ,
	bLisp_LambdaToken     ,
	bLisp_ListToken       ,
	bLisp_CarToken        ,
	bLisp_CdrToken        ,
	bLisp_WhileToken      ,
	bLisp_NativeToken     ,
	bLisp_LineToken       ,
	bLisp_SpaceToken      ,
	bLisp_ErrorToken      ,
	bLisp_EofToken
};
/*============================================================================*/
/* bLisp_TokenValue                                                           */
/*============================================================================*/
union bLisp_TokenValue
{
	int     int_value   ;
	bdouble float_value ;
	bchar  *string_value;
	bchar  *error_value ;
	bchar  *name_value  ;
};
/*============================================================================*/
/* bLisp_TokenKey                                                             */
/*============================================================================*/
typedef struct
{
	enum bLisp_TokenClass type ;
	bchar                *error;
	bchar                *end  ;
} bLisp_TokenKey;
/*============================================================================*/
/* bLisp_Token                                                                */
/*============================================================================*/
typedef struct 
{
	enum  bLisp_TokenClass type ;
	union bLisp_TokenValue value;
	buint                  line ;
	bchar                 *file ;
} bLisp_Token;
/*============================================================================*/
/* bLisp_Variable                                                             */
/*============================================================================*/
typedef bLisp_Type bLisp_Variable;
/*============================================================================*/
/* bLisp_VariableList                                                         */
/*============================================================================*/
typedef struct bLisp_VariableList
{
	bchar                     *name    ;
	bLisp_Variable             variable;
	struct bLisp_VariableList *next    ;
} bLisp_VariableList;
/*============================================================================*/
/* bLisp_FunctionList                                                         */
/*============================================================================*/
typedef struct bLisp_FunctionList
{
	bLisp_Function             function;
	struct bLisp_FunctionList *next    ;
} bLisp_FunctionList;
/*============================================================================*/
/* bLisp_NativeFunctionList                                                   */
/*============================================================================*/
typedef struct bLisp_NativeFunctionList
{
	bLisp_NativeFunction             native_function ;
	struct bLisp_NativeFunctionList *next            ;
} bLisp_NativeFunctionList;
/*============================================================================*/
/* bLisp_IfMap                                                                */
/*============================================================================*/
typedef struct bLisp_IfMap
{
	buint second_pos;
	buint end_pos   ;
} bLisp_IfMap;
/*============================================================================*/
/* bLisp_CodeClass                                                            */
/*============================================================================*/
enum bLisp_CodeClass
{
	bLisp_ConstCode    ,
	bLisp_VariableCode ,
	bLisp_LocalVarCode ,
	bLisp_LambdaCode   ,
	bLisp_NativeCode   ,
	bLisp_CallCode     ,
	bLisp_PlusCode     ,
	bLisp_MinusCode    ,
	bLisp_MultiplyCode ,
	bLisp_DivideCode   ,
	bLisp_EqualCode    ,
	bLisp_LessCode     ,
	bLisp_MoreCode     ,
	bLisp_SetCode      ,
	bLisp_IfCode       ,
	bLisp_BeginCode    ,
	bLisp_ListCode     ,
	bLisp_CarCode      ,
	bLisp_CdrCode      ,
	bLisp_WhileCode    ,
	bLisp_EndCode
};
/*============================================================================*/
/* bLisp_CodeValue                                                            */
/*============================================================================*/
union bLisp_CodeValue
{
	bLisp_Type  const_value    ;
	buint       variable_value ;
	buint       lambda_value   ;
	buint       native_value   ;
	buint       localvar_value ;
	buint       call_value     ;
	buint       while_value    ;
	bLisp_IfMap if_value       ;
};
/*============================================================================*/
/* bLisp_Code                                                                 */
/*============================================================================*/
typedef struct
{
	enum  bLisp_CodeClass type ;
	union bLisp_CodeValue value;
} bLisp_Code;
/*============================================================================*/
/* bLisp_CodeList                                                             */
/*============================================================================*/
typedef struct bLisp_CodeList
{
	bLisp_Code             code;
	struct bLisp_CodeList *next;
} bLisp_CodeList;
/*============================================================================*/
/* bLisp_LocalVarList                                                         */
/*============================================================================*/
typedef struct bLisp_LocalVarList
{
	bchar                     *name;
	struct bLisp_LocalVarList *next;
} bLisp_LocalVarList;
/*============================================================================*/
/* bLisp_StringList                                                           */
/*============================================================================*/
typedef struct bLisp_StringList
{
	bchar                   *string;
	struct bLisp_StringList *next  ;
} bLisp_StringList;
/*============================================================================*/
/* bLisp_SymbolTable                                                          */
/*============================================================================*/
typedef struct
{
	bLisp_CodeList           *code           ;
	bLisp_VariableList       *variable       ;
	bLisp_FunctionList       *function       ;
	bLisp_LocalVarList       *local_var      ;
	bLisp_NativeFunctionList *native_function;
	bLisp_StringList         *strings        ;
} bLisp_SymbolTable;
/*============================================================================*/
/* bLisp_IncludeList                                                          */
/*============================================================================*/
typedef struct bLisp_IncludeList
{
	bchar                    *file;
	bchar                    *code;
	bchar                    *pos ;
	buint                     line;
	struct bLisp_IncludeList *next;
} bLisp_IncludeList;
/*============================================================================*/
/* bLisp_ListOfLists                                                          */
/*============================================================================*/
typedef struct bLisp_ListOfLists
{
	bLisp_TypeList           *list;
	struct bLisp_ListOfLists *next;
} bLisp_ListOfLists;
/*============================================================================*/
/* bLisp_Script                                                               */
/*============================================================================*/
struct bLisp_Script
{
	// ������� �������� � �������.
	buint                 code_size  ;
	buint                 var_size   ;
	buint                 func_size  ;
	buint                 nfunc_size ;
	buint                 string_size;
	// ��� ���������� �������� ���.
	bLisp_Code           *code       ;
	bLisp_Variable       *var        ;
	bLisp_Function       *func       ;
	bLisp_NativeFunction *native_func;
	bchar                *string     ;
	// ������ ��� ��������.
	bLisp_Type           *stack      ;
	bLisp_Type           *local_vars ;
	bLisp_Code           *code_start ;
	bLisp_ListOfLists    *lists      ;
};

/*============================================================================*/
/* ������� �������                                                            */
/*============================================================================*/
#define bLisp_LexerFree     (bchar *)1
#define bLisp_LexerContinue (bchar *)0
/*============================================================================*/
/* ������ � ����� � ��������� ���������                                       */
/*============================================================================*/
#define bLisp_ReturnTail    (bchar *)1
/*============================================================================*/
/* ����, ���������� ��������� ���������                                       */
/*============================================================================*/
#define bLisp_EvalCode _code->type == bLisp_CallCode    ||\
	                   _code->type == bLisp_IfCode      ||\
		               _code->type == bLisp_BeginCode   ||\
				       _code->type == bLisp_SetCode     ||\
				       _code->type == bLisp_LambdaCode  ||\
				       _code->type == bLisp_PlusCode    ||\
				       _code->type == bLisp_MinusCode   ||\
				       _code->type == bLisp_MultiplyCode||\
				       _code->type == bLisp_DivideCode  ||\
				       _code->type == bLisp_LessCode    ||\
				       _code->type == bLisp_MoreCode    ||\
				       _code->type == bLisp_EqualCode

/*============================================================================*/
/* bLisp_GetToken                                                             */
/*                                                                            */
/* ��������:                                                                  */
/*     ��������� ��������� ����� �� ������ � �����.                           */
/* ���������:                                                                 */
/*     _script - ��������� �� ������ � �����.                                 */
/*      ����� ��������� ��������� ��������:                                   */
/*	       1. ���������� ��������� �� ���.                                    */
/*         2. bLisp_LexerFree - ����������� ��� ���������� ������ �������.    */
/*         3. bLisp_LexerContinue - ���������� ������ � ���������� ������.    */
/*============================================================================*/
bLisp_Token bLisp_GetToken(bchar *_script);
/*============================================================================*/
/* bLisp_EvalToken                                                            */
/*                                                                            */
/* ��������:                                                                  */
/*     ��������� �������� �������, ������� ���������� ��� ���������� ������.  */
/*============================================================================*/
bLisp_TokenKey bLisp_EvalToken(bchar *_code);
/*============================================================================*/
/* bLisp_IsSomething                                                          */
/*                                                                            */
/* ��������:                                                                  */
/*     ������ ������� ����������� ��������� ��������� ��������.               */
/*============================================================================*/
bLisp_TokenKey bLisp_IsDigit(bchar *_code);
/*============================================================================*/
bLisp_TokenKey bLisp_IsFloat(bchar *_code);
/*============================================================================*/
bLisp_TokenKey bLisp_IsString(bchar *_code);
/*============================================================================*/
bLisp_TokenKey bLisp_IsKeyWord(bchar *_code);
/*============================================================================*/
bLisp_TokenKey bLisp_IsName(bchar *_code);
/*============================================================================*/
bLisp_TokenKey bLisp_IsCarCdr(bchar *_code);
/*============================================================================*/
bLisp_TokenKey bLisp_IsCar(bchar *_code);
/*============================================================================*/
bLisp_TokenKey bLisp_IsCdr(bchar *_code);
/*============================================================================*/
bLisp_TokenKey bLisp_IsWhile(bchar *_code);
/*============================================================================*/
bLisp_TokenKey bLisp_IsIfInclude(bchar *_code);
/*============================================================================*/
bLisp_TokenKey bLisp_IsInclude(bchar *_code);
/*============================================================================*/
bLisp_TokenKey bLisp_IsIf(bchar *_code);
/*============================================================================*/
bLisp_TokenKey bLisp_IsBegin(bchar *_code);
/*============================================================================*/
bLisp_TokenKey bLisp_IsDefine(bchar *_code);
/*============================================================================*/
bLisp_TokenKey bLisp_IsLambdaList(bchar *_code);
/*============================================================================*/
bLisp_TokenKey bLisp_IsList(bchar *_code);
/*============================================================================*/
bLisp_TokenKey bLisp_IsLambda(bchar *_code);
/*============================================================================*/
bLisp_TokenKey bLisp_IsNative(bchar *_code);
/*============================================================================*/
bLisp_TokenKey bLisp_IsSet(bchar *_code);
/*============================================================================*/
/* bLisp_IsChuckNorris                                                        */
/*                                                                            */
/* ��������:                                                                  */
/*     ���������� �������� �� ������ ������������.                            */
/*============================================================================*/
bbool bLisp_IsChuckNorris(bchar _chuck);
/*============================================================================*/
/* bLisp_SuckSomething                                                        */
/*                                                                            */
/* ��������:                                                                  */
/*     ������ ������� ����������� �������� �� ������ �������.                 */
/*============================================================================*/
bchar *bLisp_SuckString(bchar *_begin, bchar *_end);
/*============================================================================*/
bchar *bLisp_SuckName(bchar *_begin, bchar *_end);
/*============================================================================*/
bint bLisp_SuckDigit(bchar *_begin);
/*============================================================================*/
bdouble bLisp_SuckFloat(bchar *_begin);
/*============================================================================*/
/* bLisp_FreeIncludeList                                                      */
/*                                                                            */
/* ��������:                                                                  */
/*     ����������� ������ ������� ������� ��������.                           */
/*============================================================================*/
bvoid bLisp_FreeIncludeList(bLisp_IncludeList *_include);
/*============================================================================*/
/* bLisp_AddInclude                                                           */
/*                                                                            */
/* ��������:                                                                  */
/*     ��������� ����� ������ � ������ ��������.                              */
/*============================================================================*/
bLisp_IncludeList *bLisp_AddInclude(bLisp_IncludeList *_include);
/*============================================================================*/
/* bLisp_LastInclude                                                          */
/*                                                                            */
/* ��������:                                                                  */
/*     ���������� ��������� ������������� ������.                             */
/*============================================================================*/
bLisp_IncludeList *bLisp_LastInclude(bLisp_IncludeList *_include);
/*============================================================================*/
/* bLisp_ReadScriptFile                                                       */
/*                                                                            */
/* ��������:                                                                  */
/*     ������ ���� � �������� �����.                                          */
/*============================================================================*/
bchar *bLisp_ReadScriptFile(bchar *_filename);
/*============================================================================*/
/* bLisp_ParseScript                                                          */
/*                                                                            */
/* ��������:                                                                  */
/*     ��������� ��������� ������� � ���������� ���.                          */
/*============================================================================*/
bLisp_Token bLisp_ParseScript(bLisp_Script *_script, bchar *_code);
/*============================================================================*/
/* bLisp_ParseSomething                                                       */
/*                                                                            */
/* ��������:                                                                  */
/*     ������ ������� ����������� ������� �������.                            */
/*============================================================================*/
bbool bLisp_ParseExpression(bLisp_SymbolTable *_st, bLisp_Token *_token);
/*============================================================================*/
bbool bLisp_ParseOperator(bLisp_SymbolTable *_st, bLisp_Token *_token);
/*============================================================================*/
bbool bLisp_ParseDefine(bLisp_SymbolTable *_st, bLisp_Token *_token);
/*============================================================================*/
bbool bLisp_ParseSet(bLisp_SymbolTable *_st, bLisp_Token *_token);
/*============================================================================*/
bbool bLisp_ParseIf(bLisp_SymbolTable *_st, bLisp_Token *_token);
/*============================================================================*/
bbool bLisp_ParseBegin(bLisp_SymbolTable *_st, bLisp_Token *_token);
/*============================================================================*/
bbool bLisp_ParseList(bLisp_SymbolTable *_st, bLisp_Token *_token);
/*============================================================================*/
bbool bLisp_ParseCarCdr(bLisp_SymbolTable *_st, bLisp_Token *_token);
/*============================================================================*/
bbool bLisp_ParseWhile(bLisp_SymbolTable *_st, bLisp_Token *_token);
/*============================================================================*/
bbool bLisp_ParseLambda(bLisp_SymbolTable *_st, bLisp_Token *_token);
/*============================================================================*/
bbool bLisp_ParseNative(bLisp_SymbolTable *_st, bLisp_Token *_token);
/*============================================================================*/
/* bLisp_AddSomething                                                         */
/*                                                                            */
/* ��������:                                                                  */
/*     ������ ������� ����������� ����� �������� � ��������������� ������.    */
/*============================================================================*/
bvoid bLisp_AddCode(bLisp_CodeList **_code_list, bLisp_Code *_code);
/*============================================================================*/
buint bLisp_AddVariable(bLisp_SymbolTable *_st, bchar *_name);
/*============================================================================*/
bchar *bLisp_AddString(bLisp_SymbolTable *_st, bchar *_string);
/*============================================================================*/
bvoid bLisp_AddLocalVariable(bLisp_LocalVarList **_lvl, bchar *_name);
/*============================================================================*/
bvoid bLisp_FreeLocalVariable(bLisp_LocalVarList **_lvl, buint _size);
/*============================================================================*/
buint bLisp_AddFunction(bLisp_SymbolTable *_st, bLisp_Function *_f);
/*============================================================================*/
buint bLisp_AddNativeFunction(bLisp_SymbolTable *_st);
/*============================================================================*/
/* bLisp_FindSomething                                                        */
/*                                                                            */
/* ��������:                                                                  */
/*     ������ ������� ������ �������� � �� �������.                           */
/*============================================================================*/
bLisp_NativeFunction *bLisp_FindNativeFunction(bLisp_Script *_script, bchar *_name);
/*============================================================================*/
bbool bLisp_FindVariable(bLisp_SymbolTable *_st, bLisp_Token *_token);
/*============================================================================*/
buint bLisp_FindGlobalVariable(bLisp_SymbolTable *_st, bchar *_name);
/*============================================================================*/
buint bLisp_FindLocalVariable(bLisp_SymbolTable *_st, bchar *_name);
/*============================================================================*/
/* bLisp_Compress                                                             */
/*                                                                            */
/* ��������:                                                                  */
/*     ������� ������ � ����������� �������, ��������� ��� ������ � ����.     */
/*============================================================================*/
bvoid bLisp_Compress(bLisp_Script *_script, bLisp_SymbolTable *_st);
/*============================================================================*/
bvoid bLisp_CompressString(bLisp_Script *_script, bLisp_SymbolTable *_st);
/*============================================================================*/
/* bLisp_GetCodeSize                                                          */
/*                                                                            */
/* ��������:                                                                  */
/*     ���������� ����� ������ ����.                                          */
/*============================================================================*/
buint bLisp_GetCodeSize(bLisp_CodeList *_code);
/*============================================================================*/
/* bLisp_Eval                                                                 */
/*                                                                            */
/* ��������:                                                                  */
/*     ��������� ���������.                                                   */
/*============================================================================*/
bchar *bLisp_Eval(bLisp_Script *_script);
/*============================================================================*/
bchar *bLisp_EvalPlus(bLisp_Script *_script);
/*============================================================================*/
bchar *bLisp_EvalMinus(bLisp_Script *_script);
/*============================================================================*/
bchar *bLisp_EvalMultiply(bLisp_Script *_script);
/*============================================================================*/
bchar *bLisp_EvalDivide(bLisp_Script *_script);
/*============================================================================*/
bchar *bLisp_EvalEqual(bLisp_Script *_script);
/*============================================================================*/
bchar *bLisp_EvalLess(bLisp_Script *_script);
/*============================================================================*/
bchar *bLisp_EvalMore(bLisp_Script *_script);
/*============================================================================*/
bchar *bLisp_EvalSet(bLisp_Script *_script);
/*============================================================================*/
bchar *bLisp_EvalBegin(bLisp_Script *_script);
/*============================================================================*/
bchar *bLisp_EvalIf(bLisp_Script *_script);
/*============================================================================*/
bchar *bLisp_EvalWhile(bLisp_Script *_script);
/*============================================================================*/
bchar *bLisp_EvalCall(bLisp_Script *_script);
/*============================================================================*/
bchar *bLisp_EvalCallLispFunction(bLisp_Script *_script);
/*============================================================================*/
bchar *bLisp_EvalCallNativeFunction(bLisp_Script *_script);
/*============================================================================*/
bchar *bLisp_EvalLambda(bLisp_Script *_script);
/*============================================================================*/
bchar *bLisp_EvalNative(bLisp_Script *_script);
/*============================================================================*/
bchar *bLisp_EvalLocalVar(bLisp_Script *_script);
/*============================================================================*/
bchar *bLisp_EvalVariable(bLisp_Script *_script);
/*============================================================================*/
bchar *bLisp_EvalConst(bLisp_Script *_script);
/*============================================================================*/
bchar *bLisp_EvalList(bLisp_Script *_script);
/*============================================================================*/
bchar *bLisp_EvalCar(bLisp_Script *_script);
/*============================================================================*/
bchar *bLisp_EvalCdr(bLisp_Script *_script);
/*============================================================================*/
/* bLisp_Puck/UnpackScript                                                    */
/*                                                                            */
/* ��������:                                                                  */
/*     ������� ������ ������������� ������ �� �������� � �������.             */
/*============================================================================*/
bvoid bLisp_PackScript(bLisp_Script *_script);
/*============================================================================*/
bvoid bLisp_UnpackScript(bLisp_Script *_script);
/*============================================================================*/
/* bLisp_GetStringPtr\Number                                                  */
/*                                                                            */
/* ��������:                                                                  */
/*     ��������� ������������� � �������� ����� ������.                       */
/*============================================================================*/
bchar *bLisp_GetStringPtr(bLisp_Script *_script, buint _number);
/*============================================================================*/
buint bLisp_GetStringNumber(bLisp_Script *_script, bchar *_string);
/*============================================================================*/
/* bLisp_AddList                                                              */
/*                                                                            */
/* ��������:                                                                  */
/*     ��������� ����� ������ � ������ �������.                               */
/*============================================================================*/
bvoid bLisp_AddList(bLisp_Script *_script, bLisp_TypeList *_list);
/*============================================================================*/
/* bLisp_FreeLists                                                            */
/*                                                                            */
/* ��������:                                                                  */
/*     ����������� ������ �������.                                            */
/*============================================================================*/
bvoid bLisp_FreeLists(bLisp_Script *_script);
/*============================================================================*/
/* bLisp_FreeToken                                                            */
/*                                                                            */
/* ��������:                                                                  */
/*     ������� ��� ������ �� ������, ���� ���� ��� ������� �������.           */
/*============================================================================*/
bvoid bLisp_FreeToken(bLisp_Token *_token);
/*============================================================================*/
/* bLisp_PrntType                                                             */
/*                                                                            */
/* ��������:                                                                  */
/*     ��������� ������������ ��� ����� � ������.                             */
/*============================================================================*/
bchar *bLisp_PrintType(bLisp_Type *_type);
/*============================================================================*/
/* bLisp_RegFunc_Something                                                    */
/*                                                                            */
/* ��������:                                                                  */
/*     ������ ������� ��� �������� ������ � ����� ���������.                  */
/*============================================================================*/
bbool bLisp_RegFunc_IsSeparator(bchar _c);
/*============================================================================*/
bbool bLisp_RegFunc_StrCmp(bchar *_code, bchar *_str);
/*============================================================================*/
bLisp_TypeClass bLisp_RegFunc_GetToken(bchar *_code);
/*============================================================================*/
bchar *bLisp_RegFunc_SkipCode(bchar *_code);
/*============================================================================*/
/* bLisp_FindLastExpressionInIf/Begin                                         */
/*                                                                            */
/* ��������:                                                                  */
/*     ������� ���������� ������� ��������� ��������� � If ��� Begin.         */
/*============================================================================*/
bLisp_Code *bLisp_FindLastExpressionInIf(bLisp_Code *_code);
/*============================================================================*/
bLisp_Code *bLisp_FindLastExpressionInBegin(bLisp_Code *_code);
/*============================================================================*/
/* bLisp_OptimizeTailRecursion                                                */
/*                                                                            */
/* ��������:                                                                  */
/*     ������������ ��������� ��������.                                       */
/*============================================================================*/
bvoid bLisp_OptimizeTailRecursion(bLisp_Script *_script);
/*============================================================================*/
/* bLisp_FindLastExpression                                                   */
/*                                                                            */
/* ��������:                                                                  */
/*     ������� ����� ����� ��������� ���������.                               */
/*============================================================================*/
bLisp_Code *bLisp_FindLastExpression(bLisp_Code *_code);
/*============================================================================*/
/* bLisp_Optimizer                                                            */
/*                                                                            */
/* ��������:                                                                  */
/*     ������������ ��� �������.                                              */
/*============================================================================*/
bvoid bLisp_Optimizer(bLisp_Script *_script);
/*============================================================================*/
/* bLisp_Error                                                                */
/*                                                                            */
/* ��������:                                                                  */
/*     ��������� ��������� �� ������.                                         */
/*============================================================================*/
bbool bLisp_Error(bLisp_Token *_token, bchar *_text);

/*============================================================================*/
/* bLisp_Open                                                                 */
/*============================================================================*/
bLisp_Script *bLisp_Open(bchar *_code, bchar **_error)
{
	bLisp_Script *script     ;
	static bchar  error[256] ;
	bLisp_Token   error_token;

	script = malloc(sizeof(bLisp_Script));

	error_token = bLisp_ParseScript(script, _code);
	if(error_token.type == bLisp_ErrorToken)
	{
		swprintf(error, 256, L"6y6yka Lisp error in file: %s, line: %d, text: %s",
			error_token.file, error_token.line, error_token.value.error_value);
		free(error_token.file);

		if(_error)
			*_error = error;

		free(script);

		return 0;
	}

    bLisp_Optimizer(script);

	bLisp_UnpackScript(script);

	// ������ ���
	*error = 0;

	return script;
}
/*============================================================================*/
/* bLisp_OpenFile                                                             */
/*============================================================================*/
bLisp_Script *bLisp_OpenFile(bchar *_filename)
{
	bLisp_Script *script;
	FILE *file;

	script = malloc(sizeof(bLisp_Script));

	file = _wfopen(_filename, L"rb");

	fread(script, 1, 5 * sizeof(buint), file);

	script->code = malloc(script->code_size * sizeof(bLisp_Code));
	fread(script->code, 1, script->code_size * sizeof(bLisp_Code), file);
	script->code_start = script->code;

	script->var = malloc(script->var_size * sizeof(bLisp_Variable));
	fread(script->var, 1, script->var_size * sizeof(bLisp_Variable), file);

	script->func = malloc(script->func_size * sizeof(bLisp_Function));
	fread(script->func, 1, script->func_size * sizeof(bLisp_Function), file);

	script->native_func = malloc(script->nfunc_size * sizeof(bLisp_NativeFunction));
	fread(script->native_func, 1, script->nfunc_size * sizeof(bLisp_NativeFunction), file);

	script->string = malloc(script->string_size);
	fread(script->string, 1, script->string_size, file);

	fclose(file);

	bLisp_UnpackScript(script);

	return script;
}
/*============================================================================*/
/* bLisp_WriteFile                                                            */
/*============================================================================*/
bbool bLisp_WriteFile(bLisp_Script *_script, bchar *_filename)
{
	buint size_buf[5];
	FILE *file;

	bLisp_PackScript(_script);

	size_buf[0] = _script->code_size;
	size_buf[1] = _script->var_size;
	size_buf[2] = _script->func_size;
	size_buf[3] = _script->nfunc_size;
	size_buf[4] = _script->string_size;

	file = _wfopen(_filename, L"wb");

	fwrite(size_buf, 1, 5 * sizeof(buint), file);
	fwrite(_script->code, 1, size_buf[0] * sizeof(bLisp_Code), file);
	fwrite(_script->var, 1, size_buf[1] * sizeof(bLisp_Variable), file);
	fwrite(_script->func, 1, size_buf[2] * sizeof(bLisp_Function), file);
	fwrite(_script->native_func, 1, size_buf[3] * sizeof(bLisp_NativeFunction), file);
	fwrite(_script->string, 1, size_buf[4], file);

	fclose(file);

	bLisp_UnpackScript(_script);

	return btrue;
}
/*============================================================================*/
/* bLisp_Run                                                                  */
/*============================================================================*/
bchar *bLisp_Run(bLisp_Script *_script, bchar **result)
{
	bchar *error_text;
	bLisp_Type *stack_start;

	stack_start = _script->stack = malloc(2048 * sizeof(bLisp_Type));
	_script->lists = 0;

	while(_script->code->type != bLisp_EndCode)
	{
		if(error_text = bLisp_Eval(_script))
		{
			free(stack_start);
			return error_text;
		}

		_script->stack--;
	}

	*result = bLisp_PrintType(_script->stack);

	free(_script->stack);
	bLisp_FreeLists(_script);

	return 0;
}
/*============================================================================*/
/* bLisp_RegFunc                                                              */
/*============================================================================*/
bbool bLisp_RegFunc(bLisp_Script *_script, bchar *_name, bvoid *_ptr, bchar *_arg)
{
	bLisp_TypeClassList *al = 0, *nl;
	bchar *arg_str, *arg_str_ptr;
	buint i;
    bLisp_NativeFunction *new_function = 0;

	if(!(new_function = bLisp_FindNativeFunction(_script, _name)))
		return bfalse;

	new_function->ptr = _ptr;

	// �������� ������ ��������� ��� �������
	arg_str_ptr = arg_str = malloc(wcslen(_arg) * sizeof(bchar) + sizeof(bchar));
	for(i = 0; *_arg; _arg++)
	{
		if(!iswspace(*_arg))
			arg_str[i++] = *_arg;
	}
	arg_str[i] = L'\0';

	// ������������ ��������
	new_function->rt = bLisp_RegFunc_GetToken(arg_str);
	arg_str = bLisp_RegFunc_SkipCode(arg_str);

	if(*arg_str == L'*')
	{
		new_function->rt = bLisp_DataType;
		arg_str++;
	}

	// ���������� (*)(
	arg_str += 4;

	// �������� ������ ����������
	while(*arg_str != L')')
	{
		nl = malloc(sizeof(bLisp_TypeClassList));
	    nl->type = bLisp_RegFunc_GetToken(arg_str);

		arg_str = bLisp_RegFunc_SkipCode(arg_str);

		if(*arg_str == L'*')
		{
			if(nl->type != bLisp_StringType)
				nl->type = bLisp_DataType;
			arg_str++;
		}
		else
			if(nl->type == bLisp_UnknownType)
				return bfalse;

		nl->next = al;
		al = nl;

		if(*arg_str == L',')
			arg_str++;
	}

	// ������ ������ ������ ����������
	for(i = 0, nl = al; nl; nl = nl->next, i++);

	new_function->at_size = i;

	for(i = 0, nl = al; nl; nl = nl->next, i++)
		new_function->at[i] = nl->type;

	while(al)
	{
		nl = al->next;
		free(al);
		al = nl;
	}

	free(arg_str_ptr);

	return btrue;
}
/*============================================================================*/
/* bLisp_Close                                                                */
/*============================================================================*/
bbool bLisp_Close(bLisp_Script *_script)
{
	free(_script->code_start);
	free(_script->var);
	free(_script->func);
	free(_script->native_func);
	free(_script->string);
	free(_script);

	return btrue;
}
/*============================================================================*/
/* bLisp_EvalPlus                                                             */
/*============================================================================*/
bchar *bLisp_EvalPlus(bLisp_Script *_script)
{
	bLisp_Type result;

	// ���������� ����
	_script->code++;

	// ��������� ���� ����������
	bLisp_Eval(_script);
	bLisp_Eval(_script);

	// ����������� ��������� �� �����
	_script->stack--;
	result = *_script->stack;
	_script->stack--;

	// ��������� �� ����
	if((result.type != bLisp_IntType && result.type != bLisp_FloatType) ||
		(_script->stack->type != bLisp_IntType && _script->stack->type != bLisp_FloatType))
		return L"Wrong types in '+' operator";

	// ����������
	if(result.type == bLisp_IntType)
	{
		if(_script->stack->type == bLisp_IntType)
			result.value.int_value += _script->stack->value.int_value;
		else
		{
			result.type = bLisp_FloatType;
			result.value.float_value = (bdouble)result.value.int_value;
			result.value.float_value += (bdouble)_script->stack->value.float_value;
		}
	}
	else
	{
		if(_script->stack->type == bLisp_IntType)
			result.value.float_value += (bdouble)_script->stack->value.int_value;
		else
			result.value.float_value += _script->stack->value.float_value;
	}

	// ������ ��������� � ����
	*_script->stack = result;
	_script->stack++;

	// ���������� ��������
	_script->code++;

	return 0;
}
/*============================================================================*/
/* bLisp_EvalMinus                                                            */
/*============================================================================*/
bchar *bLisp_EvalMinus(bLisp_Script *_script)
{
	bLisp_Type result;

	// ���������� �����
	_script->code++;

	// ��������� ���� ����������
	bLisp_Eval(_script);
	bLisp_Eval(_script);

	// ����������� ��������� �� �����
	_script->stack--;
	result = *_script->stack;
	_script->stack--;

	// ��������� �� ����
	if((result.type != bLisp_IntType && result.type != bLisp_FloatType) ||
		(_script->stack->type != bLisp_IntType && _script->stack->type != bLisp_FloatType))
		return L"Wrong types in '-' operator";

	// ��������
	if(result.type == bLisp_IntType)
	{
		if(_script->stack->type == bLisp_IntType)
			result.value.int_value                     =
			_script->stack->value.int_value            -
			result.value.int_value                     ;
		else
		{
			result.type = bLisp_FloatType;
			result.value.float_value = (bdouble)result.value.int_value;
			result.value.float_value                   = 
			(bdouble)_script->stack->value.float_value - 
			result.value.float_value                   ;
		}
	}
	else
	{
		if(_script->stack->type == bLisp_IntType)
			result.value.float_value                   =
			(bdouble)_script->stack->value.int_value   - 
			result.value.float_value                   ;
		else
			result.value.float_value                   = 
			_script->stack->value.float_value          - 
			result.value.float_value                   ;
	}

	// ������ ��������� � ����
	*_script->stack = result;
	_script->stack++;

	// ���������� ��������
	_script->code++;

	return 0;
}
/*============================================================================*/
/* bLisp_EvalMultiply                                                         */
/*============================================================================*/
bchar *bLisp_EvalMultiply(bLisp_Script *_script)
{
	bLisp_Type result;

	// ���������� *
	_script->code++;

	// ��������� ���� ����������
	bLisp_Eval(_script);
	bLisp_Eval(_script);

	// ����������� ��������� �� �����
	_script->stack--;
	result = *_script->stack;
	_script->stack--;

	// ��������� �� ����
	if((result.type != bLisp_IntType && result.type != bLisp_FloatType) ||
		(_script->stack->type != bLisp_IntType && _script->stack->type != bLisp_FloatType))
		return L"Wrong types in '*' operator";

	// ��������
	if(result.type == bLisp_IntType)
	{
		if(_script->stack->type == bLisp_IntType)
			result.value.int_value *= _script->stack->value.int_value;
		else
		{
			result.type = bLisp_FloatType;
			result.value.float_value = (bdouble)result.value.int_value;
			result.value.float_value *= (bdouble)_script->stack->value.float_value;
		}
	}
	else
	{
		if(_script->stack->type == bLisp_IntType)
			result.value.float_value *= (bdouble)_script->stack->value.int_value;
		else
			result.value.float_value *= _script->stack->value.float_value;
	}

	// ������ ��������� � ����
	*_script->stack = result;
	_script->stack++;

	// ���������� ��������
	_script->code++;

	return 0;
}
/*============================================================================*/
/* bLisp_EvalDivide                                                           */
/*============================================================================*/
bchar *bLisp_EvalDivide(bLisp_Script *_script)
{
	bLisp_Type result;

	// ���������� /
	_script->code++;

	// ��������� ���� ����������
	bLisp_Eval(_script);
	bLisp_Eval(_script);

	// ����������� ��������� �� �����
	_script->stack--;
	result = *_script->stack;
	_script->stack--;

	// ��������� �� ����
	if((result.type != bLisp_IntType && result.type != bLisp_FloatType) ||
		(_script->stack->type != bLisp_IntType && _script->stack->type != bLisp_FloatType))
		return L"Wrong types in '/' operator";

	// ��������
	if(result.type == bLisp_IntType)
	{
		if(_script->stack->type == bLisp_IntType)
			result.value.int_value                     = 
			_script->stack->value.int_value            /
			result.value.int_value                     ;
		else
		{
			result.type = bLisp_FloatType;
			result.value.float_value = (bdouble)result.value.int_value;
			result.value.float_value                   = 
			(bdouble)_script->stack->value.float_value / 
			result.value.float_value                   ;
		}
	}
	else
	{
		if(_script->stack->type == bLisp_IntType)
			result.value.float_value                   = 
			(bdouble)_script->stack->value.int_value   /
			result.value.float_value                   ;
		else
			result.value.float_value                   =
			_script->stack->value.float_value          / 
			result.value.float_value                   ;
	}

	// ������ ��������� � ����
	*_script->stack = result;
	_script->stack++;

	// ���������� ��������
	_script->code++;

	return 0;
}
/*============================================================================*/
/* bLisp_EvalEqual                                                            */
/*============================================================================*/
bchar *bLisp_EvalEqual(bLisp_Script *_script)
{
	bLisp_Type result;
	bchar *error;

	// ���������� =
	_script->code++;

	// ��������� ���� ����������
	if(error = bLisp_Eval(_script))
		return error;
	if(error = bLisp_Eval(_script))
		return error;

	// ����������� ��������� �� �����
	_script->stack--;
	result = *_script->stack;
	_script->stack--;

	
	if(_script->stack->type == bLisp_FloatType)
	{
		if(result.type == bLisp_FloatType)
			_script->stack->value.int_value   = 
			result.value.float_value          == 
			_script->stack->value.float_value ;
		else
			_script->stack->value.int_value   =
			result.value.int_value            ==
			_script->stack->value.float_value ;
	}
	else
	{
		if(result.type == bLisp_FloatType)
			_script->stack->value.int_value   = 
			result.value.float_value          == 
			_script->stack->value.int_value   ;
		else
			_script->stack->value.int_value   = 
			result.value.int_value            == 
			_script->stack->value.int_value   ;
	}
	_script->stack->type = bLisp_IntType;
	_script->stack++;

	// ���������� ��������
	_script->code++;

	return 0;
}
/*============================================================================*/
/* bLisp_EvalLess                                                             */
/*============================================================================*/
bchar *bLisp_EvalLess(bLisp_Script *_script)
{
	bLisp_Type result;
	bchar *error;

	// ���������� <
	_script->code++;

	// ��������� ���� ����������
	if(error = bLisp_Eval(_script))
		return error;
	if(error = bLisp_Eval(_script))
		return error;

	// ����������� ��������� �� �����
	_script->stack--;
	result = *_script->stack;
	_script->stack--;

	
	if(_script->stack->type == bLisp_FloatType)
	{
		if(result.type == bLisp_FloatType)
			_script->stack->value.int_value   = 
			result.value.float_value          > 
			_script->stack->value.float_value ;
		else
			_script->stack->value.int_value   = 
			result.value.int_value            > 
			_script->stack->value.float_value ;
	}
	else
	{
		if(result.type == bLisp_FloatType)
			_script->stack->value.int_value   = 
			result.value.float_value          >
			_script->stack->value.int_value   ;
		else
			_script->stack->value.int_value   = 
			result.value.int_value            > 
			_script->stack->value.int_value   ;
	}
	_script->stack->type = bLisp_IntType;
	_script->stack++;

	// ���������� ��������
	_script->code++;

	return 0;
}
/*============================================================================*/
/* bLisp_EvalMore                                                             */
/*============================================================================*/
bchar *bLisp_EvalMore(bLisp_Script *_script)
{
	bLisp_Type result;
	bchar *error;

	// ���������� >
	_script->code++;

	// ��������� ���� ����������
	if(error = bLisp_Eval(_script))
		return error;
	if(error = bLisp_Eval(_script))
		return error;

	// ����������� ��������� �� �����
	_script->stack--;
	result = *_script->stack;
	_script->stack--;
	
	if(_script->stack->type == bLisp_FloatType)
	{
		if(result.type == bLisp_FloatType)
			_script->stack->value.int_value    =
			result.value.float_value           <                                  
			_script->stack->value.float_value  ;
		else
			_script->stack->value.int_value    = 
			result.value.int_value             <                             
			_script->stack->value.float_value  ;
	}
	else
	{
		if(result.type == bLisp_FloatType)
			_script->stack->value.int_value    = 
			result.value.float_value           < 
			_script->stack->value.int_value    ;
		else
			_script->stack->value.int_value    = 
			result.value.int_value             < 
			_script->stack->value.int_value    ;
	}
	_script->stack->type = bLisp_IntType;
	_script->stack++;

	// ���������� ��������
	_script->code++;

	return 0;
}
/*============================================================================*/
/* bLisp_EvalSet                                                              */
/*============================================================================*/
bchar *bLisp_EvalSet(bLisp_Script *_script)
{
	bLisp_Variable *var;
	bchar *error;

	_script->code++;

	if(_script->code->type == bLisp_VariableCode)
		var = &_script->var[_script->code->value.variable_value];
	else if(_script->code->type == bLisp_CarCode)
	{
		_script->code++;
		if(error = bLisp_Eval(_script))
			return error;
		_script->stack--;
		var = &_script->stack->value.list_value->value;
	}

	_script->code++;

	if(error = bLisp_Eval(_script))
			return error;

	_script->stack--;
	*var = *_script->stack;

	*_script->stack = *var;
	_script->stack++;

	_script->code++;

	return 0;
}
/*============================================================================*/
/* bLisp_EvalBegin                                                            */
/*============================================================================*/
bchar *bLisp_EvalBegin(bLisp_Script *_script)
{
	bLisp_Type result;
	bchar *error;

	result.type = bLisp_VoidType;

	// ���������� begin
	_script->code++;
	// ��������� ��� ���������
	while(_script->code->type != bLisp_EndCode)
	{
		if(error = bLisp_Eval(_script))
			return error;

		_script->stack--;
	}

	result = *_script->stack;

	*_script->stack = result;
	_script->stack++;

	_script->code++;

	return 0;
}
/*============================================================================*/
/* bLisp_EvalIf                                                               */
/*============================================================================*/
bchar *bLisp_EvalIf(bLisp_Script *_script)
{
	bchar *error;
	bLisp_IfMap *if_map = &_script->code->value.if_value;

	// ���������� if
	_script->code++;
	// ��������� �������
	if(error = bLisp_Eval(_script))
		return error;
	// ��������� �������
	_script->stack--;
	if(_script->stack->value.int_value)
	{
		// ��������� ������ ��������� � ���������� ������
		if(error = bLisp_Eval(_script))
			return error;
		_script->code = &_script->code_start[if_map->end_pos];
	}
	else
	{
		// ���������� ������ ��������� � ��������� ������
		_script->code = &_script->code_start[if_map->second_pos];
		if(_script->code->type == bLisp_EndCode)
		{
			_script->stack->type = bLisp_IntType;
			_script->stack->value.int_value = 0;
			_script->stack++;
		}
		else
		{
			if(error = bLisp_Eval(_script))
				return error;
		}
	}

	_script->code++;

	return 0;
}
/*============================================================================*/
/* bLisp_FreeToken                                                            */
/*============================================================================*/
bvoid bLisp_FreeToken(bLisp_Token *_token)
{
	if(_token->type == bLisp_NameToken)
		free(_token->value.name_value);

	if(_token->type == bLisp_StringToken)
		free(_token->value.string_value);
}
/*============================================================================*/
/* bLisp_GetStringPtr                                                         */
/*============================================================================*/
bchar *bLisp_GetStringPtr(bLisp_Script *_script, buint _number)
{
	buint i ;
	bchar *s;

	for(i = 0, s = _script->string; i != _number; i++)
		s += wcslen(s) + 1;

	return s;
}
/*============================================================================*/
/* bLisp_GetStringNumber                                                      */
/*============================================================================*/
buint bLisp_GetStringNumber(bLisp_Script *_script, bchar *_string)
{
	buint i;
	bchar *s;

	for(i = 0, s = _script->string; _string != s; i++)
		s += wcslen(s) + 1;

	return i;
}
/*============================================================================*/
/* bLisp_AddList                                                              */
/*============================================================================*/
bvoid bLisp_AddList(bLisp_Script *_script, bLisp_TypeList *_list)
{
	bLisp_ListOfLists *new_lol;

	new_lol = malloc(sizeof(bLisp_ListOfLists));
	new_lol->list = _list;
	new_lol->next = _script->lists;
	_script->lists = new_lol;
}
/*============================================================================*/
/* bLisp_FreeLists                                                            */
/*============================================================================*/
bvoid bLisp_FreeLists(bLisp_Script *_script)
{
	void *t1, *t2;

	while(_script->lists)
	{
		t1 = _script->lists->next;

		while(_script->lists->list)
		{
			t2 = _script->lists->list->next;
			free(_script->lists->list);
			_script->lists->list = t2;
		}

		free(_script->lists);
		_script->lists = t1;
	}
}
/*============================================================================*/
/* bLisp_PackScript                                                           */
/*============================================================================*/
bvoid bLisp_PackScript(bLisp_Script *_script)
{
	buint i;

	for(i = 0; i < _script->nfunc_size; i++)
		_script->native_func[i].name = (bchar *)bLisp_GetStringNumber(_script, _script->native_func[i].name);

	for(i = 0; i < _script->code_size; i++)
	{
		if(_script->code[i].type == bLisp_ConstCode)
		{
			if(_script->code[i].value.const_value.type == bLisp_StringType)
			{
				_script->code[i].value.const_value.value.string_value = 
					(bchar *)bLisp_GetStringNumber(_script, _script->code[i].value.const_value.value.string_value);
			}
		}
	}
}
/*============================================================================*/
/* bLisp_UnpackScript                                                         */
/*============================================================================*/
bvoid bLisp_UnpackScript(bLisp_Script *_script)
{
	buint i;

	for(i = 0; i < _script->nfunc_size; i++)
		_script->native_func[i].name = bLisp_GetStringPtr(_script, (buint)_script->native_func[i].name);

	for(i = 0; i < _script->code_size; i++)
	{
		if(_script->code[i].type == bLisp_ConstCode)
		{
			if(_script->code[i].value.const_value.type == bLisp_StringType)
			{
				_script->code[i].value.const_value.value.string_value = 
					bLisp_GetStringPtr(_script, (buint)_script->code[i].value.const_value.value.string_value);
			}
		}
	}
}
/*============================================================================*/
/* bLisp_EvalCall                                                             */
/*============================================================================*/
bchar *bLisp_EvalCall(bLisp_Script *_script)
{
	bchar *error;
	bbool tail;

	tail = _script->code->value.lambda_value;

	_script->code++;
	if(error = bLisp_Eval(_script))
		return error;
	_script->stack--;

	if(_script->stack->type == bLisp_LambdaType && !tail)
		return bLisp_EvalCallLispFunction(_script);
	else if(_script->stack->type == bLisp_LambdaType && tail)
		return (bchar *)1;
	else if(_script->stack->type == bLisp_NativeType)
		return bLisp_EvalCallNativeFunction(_script);
	else 
		return L"Can't call this variable";
}
/*============================================================================*/
/* bLisp_EvalBadTail                                                          */
/*============================================================================*/
bchar *bLisp_EvalBadTail(bLisp_Script *_script)
{
	if(_script->stack->type == bLisp_LambdaType)
		return bLisp_EvalCallLispFunction(_script);
	else if(_script->stack->type == bLisp_NativeType)
		return bLisp_EvalCallNativeFunction(_script);
	else 
		return L"Can't call this variable";
}
/*============================================================================*/
/* bLisp_EvalCallLispFunction                                                 */
/*============================================================================*/
bchar *bLisp_EvalCallLispFunction(bLisp_Script *_script)
{
	bLisp_Script fake_script;
	bLisp_Type result, f;
	bLisp_Code *ret;
	bchar *error;
	buint i;

	result.type = bLisp_IntType;
	result.value.int_value = 0;

	f = *_script->stack;

    fake_script = *_script;

	// ���������� ��������� � ����
	for(i = 0; fake_script.code->type != bLisp_EndCode; i++)
	{
		if(error = bLisp_Eval(&fake_script))
			return error;
	}

	if(f.value.lambda_value->arg_num != i)
		return f.value.lambda_value->arg_num > i ? L"lisp function get so few arguments" :
		                                           L"lisp function get so much arguments";

	ret = fake_script.code;
	fake_script.local_vars = fake_script.stack;

bLisp_Tail:	
	fake_script.code = &_script->code_start[f.value.lambda_value->ptr];

	while(fake_script.code->type != bLisp_EndCode)
	{
		if(error = bLisp_Eval(&fake_script))
			if(error == (bchar *)1)
			{
				if(fake_script.stack->value.lambda_value == f.value.lambda_value)
				{
					// ���������� ��������� � ����
					for(i = 0; fake_script.code->type != bLisp_EndCode; i++)
					{
						if(error = bLisp_Eval(&fake_script))
							return error;
					}

					memcpy(fake_script.local_vars - i, fake_script.stack - i, i * sizeof(bLisp_Type));
					fake_script.stack = fake_script.local_vars;

					goto bLisp_Tail;
				}
				else
				{
					if(error = bLisp_EvalBadTail(&fake_script))
						return error;
					
					fake_script.stack--;
					break;
				}
			}
			else
				return error;

			fake_script.stack--;
	}

	result = *fake_script.stack;

	for(i = 0; i < f.value.lambda_value->arg_num; i++)
		fake_script.stack--;

	_script->code = ret + 1;

	*_script->stack = result;
	_script->stack++;

	return 0;
}
/*============================================================================*/
/* bLisp_EvalCallNativeFunction                                               */
/*============================================================================*/
bchar *bLisp_EvalCallNativeFunction(bLisp_Script *_script)
{
	bLisp_Type result;
	bLisp_NativeFunction *f;
	bLisp_Type *arg;
	union bLisp_TypeValue arg_value;
	enum bLisp_TypeClass arg_type;
	buint i, arg_num;
	bvoid *p;
	bchar *error;

	f = _script->stack->value.native_value;

	// �������� ��������� � ����
	for(i = 0; _script->code->type != bLisp_EndCode; i++)
	{
		if(error = bLisp_Eval(_script))
			return error;
	}

	// ��������� ���������� ����������
	if(i != f->at_size)
		return i > i != f->at_size ? L"native function gives so mutch arguments" :
			                         L"native function gives so few arguments"   ;

	for(i = 0, arg_num = 0; i < f->at_size; i++, arg_num++)
	{
		arg = ((_script->stack - 1) - i);
		arg_value = arg->value;
		arg_type = arg->type;

		if(arg_type != f->at[i])
		{
			if(f->at[i] == bLisp_IntType && arg_type == bLisp_FloatType)
				arg_value.int_value = (int)arg->value.float_value;
			else if(f->at[i] == bLisp_FloatType && arg_type == bLisp_IntType)
				arg_value.float_value = (bdouble)arg->value.int_value;
			else
			{
				while(arg_num--) __asm pop ebx;

				return L"native function argument type conflict";
			}
		}

		switch(f->at[i])
		{
		case bLisp_IntType:
			__asm push arg_value.int_value;
			break;

		case bLisp_DataType:
			__asm push arg_value.data_value;
			break;

		case bLisp_StringType:
			__asm push arg_value.string_value;
			break;

		case bLisp_FloatType:
			__asm lea eax, arg_value;
			__asm push [eax + 4];
			__asm push [eax];
			arg_num++;
			break;
		}
	}

	p = f->ptr;
	result.type = f->rt;

	switch(f->rt)
	{
	case bLisp_IntType:
		__asm call p;
		__asm mov result.value.int_value, eax;
		break;

	case bLisp_DataType:
		__asm call p;
		__asm mov result.value.data_value, eax;
		break;

	case bLisp_StringType:
		__asm call p;
		__asm mov result.value.string_value, eax;
		break;

	case bLisp_FloatType:
		__asm call p;
		__asm fstp result.value.float_value;
		break;

	case bLisp_VoidType:
		__asm call p;
		result.type = bLisp_IntType;
		result.value.int_value = 0;
		break;
	}

	// ������ ����
	while(arg_num--) __asm pop ebx;

	for(i = 0; i < f->at_size; i++, _script->stack--);

	_script->code++;
	*_script->stack = result;
	_script->stack++;

	return 0;
}
/*============================================================================*/
/* bLisp_EvalLambda                                                           */
/*============================================================================*/
bchar *bLisp_EvalLambda(bLisp_Script *_script)
{
	_script->stack->type = bLisp_LambdaType;

	// ������ � ���� ��� ������ ������
	_script->stack->value.lambda_value = &_script->func[_script->code->value.lambda_value - 1];
	
	// ������� � ����� ������ ��������� � ���������� ��������
	_script->code = &_script->code_start[_script->stack->value.lambda_value->end_ptr + 1];

	_script->stack++;

	return 0;
}
/*============================================================================*/
/* bLisp_EvalNative                                                           */
/*============================================================================*/
bchar *bLisp_EvalNative(bLisp_Script *_script)
{
	// ������ � ���� ������ ����� �������
	_script->stack->type = bLisp_NativeType;
	_script->stack->value.native_value = &_script->native_func[_script->code->value.native_value - 1];
	_script->stack++;

	// ���������� native
	_script->code++;

	// ��������� ��������
	_script->code++;
	
	return 0;
}
/*============================================================================*/
/* bLisp_EvalLocalVar                                                         */
/*============================================================================*/
bchar *bLisp_EvalLocalVar(bLisp_Script *_script)
{
	// ������ � ���� �������� ����������
	*_script->stack = *((_script->local_vars - 1) - _script->code->value.localvar_value);
	_script->stack++;

	// ���������� ��������
	_script->code++;

	return 0;
}
/*============================================================================*/
/* bLisp_EvalVariable                                                         */
/*============================================================================*/
bchar *bLisp_EvalVariable(bLisp_Script *_script)
{
	// ������ � ���� �������� ����������
	*_script->stack = _script->var[_script->code->value.variable_value];
	_script->stack++;

	// ���������� ����������
	_script->code++;

	return 0;
}
/*============================================================================*/
/* bLisp_EvalConst                                                            */
/*============================================================================*/
bchar *bLisp_EvalConst(bLisp_Script *_script)
{
	// ������ � ���� �������� ���������
	*_script->stack = _script->code->value.const_value;
	_script->stack++;

	// ���������� ���������
	_script->code++;

	return 0;
}
/*============================================================================*/
/* bLisp_EvalList                                                             */
/*============================================================================*/
bchar *bLisp_EvalList(bLisp_Script *_script)
{
	bchar *error;
	bLisp_TypeList *tl = 0, *nl;

	// ���������� list
	_script->code++;

	// ������� ������
	while(_script->code->type != bLisp_EndCode)
	{
		if(error = bLisp_Eval(_script))
			return error;
		
		_script->stack--;

		nl = malloc(sizeof(bLisp_TypeList));
		nl->value = *_script->stack;
		nl->next = tl;
		tl = nl;
	}

	_script->stack->type = bLisp_ListType;
	_script->stack->value.list_value = tl;
	_script->stack++;

	bLisp_AddList(_script, tl);

	_script->code++;

	return 0;
}
/*============================================================================*/
/* bLisp_EvalCar                                                              */
/*============================================================================*/
bchar *bLisp_EvalCar(bLisp_Script *_script)
{
	bchar *error;

	// ���������� car
	_script->code++;

	if(error = bLisp_Eval(_script))
		return error;

	_script->stack--;
	if(_script->stack->type != bLisp_ListType)
		return L"car working only with lists!";

	*_script->stack = _script->stack->value.list_value->value;
	_script->stack++;

	_script->code++;

	return 0;
}
/*============================================================================*/
/* bLisp_EvalCdr                                                              */
/*============================================================================*/
bchar *bLisp_EvalCdr(bLisp_Script *_script)
{
	bchar *error;

	// ���������� cdr
	_script->code++;

	if(error = bLisp_Eval(_script))
		return error;

	_script->stack--;
	if(_script->stack->type != bLisp_ListType)
		return L"cdr working only with lists!";

	if(_script->stack->value.list_value)
		_script->stack->value.list_value = _script->stack->value.list_value->next;
	else
		_script->stack->value.list_value = 0;
	_script->stack->type = bLisp_ListType;
	_script->stack++;

	_script->code++;

	return 0;
}
/*============================================================================*/
/* bLisp_EvalWhile                                                            */
/*============================================================================*/
bchar *bLisp_EvalWhile(bLisp_Script *_script)
{
	bchar *error;
	bLisp_Code *begin, *end;

	end = &_script->code_start[_script->code->value.while_value];

	// ���������� while
	_script->code++;

	begin = _script->code;

	while(L"I love C")
	{
		// ��������� �������
		if(error = bLisp_Eval(_script))
			return error;
	
		_script->stack--;
		if(_script->stack->value.int_value)
		{
			// ��������� ���� �����
			if(error = bLisp_Eval(_script))
				return error;

			_script->stack--;
			
			// ������� � ������ �����
			_script->code = begin;

			continue;
		}
		else
		{
			_script->code = end;

			break;
		}
	}

	_script->stack++;
	_script->code++;

	return 0;
}
/*============================================================================*/
/* bLisp_Eval                                                                 */
/*============================================================================*/
bchar *bLisp_Eval(bLisp_Script *_script)
{
	switch(_script->code->type)
	{
	case bLisp_CallCode    :
		return bLisp_EvalCall(_script);

	case bLisp_LambdaCode  :
		return bLisp_EvalLambda(_script);

	case bLisp_NativeCode  :
		return bLisp_EvalNative(_script);

	case bLisp_LocalVarCode:
		return bLisp_EvalLocalVar(_script);

	case bLisp_ListCode    :
		return bLisp_EvalList(_script);

	case bLisp_CarCode     :
		return bLisp_EvalCar(_script);

	case bLisp_CdrCode     :
		return bLisp_EvalCdr(_script);

	case bLisp_WhileCode   :
		return bLisp_EvalWhile(_script);

	case bLisp_PlusCode    :
		return bLisp_EvalPlus(_script);

	case bLisp_MinusCode   :
		return bLisp_EvalMinus(_script);

	case bLisp_MultiplyCode:
		return bLisp_EvalMultiply(_script);

	case bLisp_DivideCode  :
		return bLisp_EvalDivide(_script);

	case bLisp_EqualCode   :
		return bLisp_EvalEqual(_script);

	case bLisp_LessCode    :
		return bLisp_EvalLess(_script);

	case bLisp_MoreCode    :
		return bLisp_EvalMore(_script);

	case bLisp_SetCode     :
		return bLisp_EvalSet(_script);

	case bLisp_IfCode      :
		return bLisp_EvalIf(_script);

	case bLisp_BeginCode   :
		return bLisp_EvalBegin(_script);

	case bLisp_ConstCode   :
		return bLisp_EvalConst(_script);

	case bLisp_VariableCode:
		return bLisp_EvalVariable(_script);

	default:
		return L"6y6yka Lisp runtime error: Unknown code found!";
	}
}
/*============================================================================*/
/* bLisp_GetToken                                                             */
/*============================================================================*/
bLisp_Token bLisp_GetToken(bchar *_script)
{
	static bLisp_IncludeList *current_include;
	static bLisp_IncludeList *include = 0;
	bchar *begin;
	bLisp_TokenKey token_type;
	bLisp_Token    token;

	if(_script == bLisp_LexerFree)
	{
		bLisp_FreeIncludeList(include);
		include = 0;

		token.type = 0;
		return token;
	}

	if(_script)
	{
		bLisp_FreeIncludeList(include);
		include = 0;
		include = bLisp_AddInclude(include);
		include->file = L"main script";
		include->code = _script;
		include->pos  = _script;
		include->line = 1;
		current_include = include;
	}

	token_type = bLisp_EvalToken(current_include->pos);
	begin = current_include->pos;
	current_include->pos = token_type.end;

	switch(token_type.type)
	{
	case bLisp_EofToken:
		token.type = token_type.type;
		token.file = current_include->file;
		token.line = current_include->line;

		current_include = bLisp_LastInclude(current_include);

		if(current_include)
			return bLisp_GetToken(bLisp_LexerContinue);

		bLisp_FreeIncludeList(include);
		include = 0;

		return token;

	case bLisp_SpaceToken:
		return bLisp_GetToken(bLisp_LexerContinue);

	case bLisp_LineToken:
		current_include->line++;
		return bLisp_GetToken(bLisp_LexerContinue);

	case bLisp_SharpToken:
		token_type = bLisp_EvalToken(current_include->pos);
		current_include->pos = token_type.end;
		if(token_type.type != bLisp_IncludeToken)
		{
			token.type = token_type.type;
			token.file = current_include->file;
			token.line = current_include->line;
			bLisp_Error(&token, L"Missing 'include' in macro expression!!!");

			return token;
		}

		token_type = bLisp_EvalToken(current_include->pos);
		while(token_type.type == bLisp_SpaceToken)
		{
			current_include->pos = token_type.end;
			token_type = bLisp_EvalToken(current_include->pos);
		}
		if(token_type.type != bLisp_StringToken)
		{
			token.type = token_type.type;
			token.file = current_include->file;
			token.line = current_include->line;
			bLisp_Error(&token, L"Missing filename in include expression!!!");

			return token;
		}

		include              = bLisp_AddInclude(include);
		include->file        = bLisp_SuckString(current_include->pos, token_type.end);
		include->code        = bLisp_ReadScriptFile(include->file);

		if(!include->code)
		{
			token.type = bLisp_ErrorToken;
			token.file = current_include->file;
			token.line = current_include->line;
			bLisp_Error(&token, L"Can't read source file. Use UTF-16!!");

			return token;
		}

		include->pos         = include->code;
		include->line        = 1;
		current_include->pos = token_type.end;
		current_include      = include;

		return bLisp_GetToken(bLisp_LexerContinue);

	case bLisp_DigitToken:
		token.value.int_value = bLisp_SuckDigit(begin);
		break;

	case bLisp_FloatToken:
		token.value.float_value = bLisp_SuckFloat(begin);
		break;

	case bLisp_StringToken:
		token.value.string_value = bLisp_SuckString(begin, token_type.end);
		break;

	case bLisp_NameToken:
		token.value.name_value = bLisp_SuckName(begin, token_type.end);
		break;

	case bLisp_ErrorToken:
		token.type              = bLisp_ErrorToken                   ;
		token.value.error_value = token_type.error                   ;
		token.line              = current_include->line              ;
		token.file              = current_include->file              ;

		return token;

	default:
		break;
	}

	token.type = token_type.type;
	token.file = current_include->file;
	token.line = current_include->line;

	return token;
}
/*============================================================================*/
/* bLisp_FreeIncludeList                                                      */
/*============================================================================*/
bvoid bLisp_FreeIncludeList(struct bLisp_IncludeList *_include)
{
	struct bLisp_IncludeList *temp_include;

	while(_include)
	{
		if(_include->next)
		{
			free(_include->code);
			free(_include->file);
		}

		temp_include = _include->next;
		free(_include);
		_include = temp_include;
	}
}
/*============================================================================*/
/* bLisp_AddInclude                                                           */
/*============================================================================*/
bLisp_IncludeList *bLisp_AddInclude(bLisp_IncludeList *_include)
{
	bLisp_IncludeList *new_include;

	new_include = malloc(sizeof(struct bLisp_IncludeList));
	new_include->next = _include;

	return new_include;
}
/*============================================================================*/
/* bLisp_LastInclude                                                          */
/*============================================================================*/
bLisp_IncludeList *bLisp_LastInclude(bLisp_IncludeList *_include)
{
	while(_include = _include->next)
	{
		if(*_include->pos != L'\0') 
			break;
	}

	return _include;
}
/*============================================================================*/
/* bLisp_EvalToken                                                            */
/*============================================================================*/
bLisp_TokenKey bLisp_EvalToken(bchar *_code)
{
	bLisp_TokenKey result;

	if(*_code == L'\0')
	{
		result.type = bLisp_EofToken;
		result.end = _code;
		return result;
	}

	if(*_code == L' ' || *_code == L'\r' || *_code == L'\t')
	{
		result.type = bLisp_SpaceToken;
		result.end = _code + 1;
		return result;
	}

	if(*_code == L'\n')
	{
		result.type = bLisp_LineToken;
		result.end = _code + 1;
		return result;
	}

	if(*_code == L'#')
	{
		result.type = bLisp_SharpToken;
		result.end = _code + 1;
		return result;
	}

	if(*_code == L'(')
	{
		result.type = bLisp_LeftBraceToken;
		result.end = _code + 1;
		return result;
	}
	
	if(*_code == L')')
	{
		result.type = bLisp_RightBraceToken;
		result.end = _code + 1;
		return result;
	}

	if(*_code == L'"')
	{
		return bLisp_IsString(_code);
	}

	if(*_code == L'+' && bLisp_IsChuckNorris(*(_code + 1)))
	{
		result.type = bLisp_PlusToken;
		result.end = _code + 1;
		return result;
	}

	if(*_code == L'-' && bLisp_IsChuckNorris(*(_code + 1)))
	{
		result.type = bLisp_MinusToken;
		result.end = _code + 1;
		return result;
	}

	if(*_code == L'*' && bLisp_IsChuckNorris(*(_code + 1)))
	{
		result.type = bLisp_MultiplyToken;
		result.end = _code + 1;
		return result;
	}

	if(*_code == L'/' && bLisp_IsChuckNorris(*(_code + 1)))
	{
		result.type = bLisp_DivideToken;
		result.end = _code + 1;
		return result;
	}

	if(*_code == L'=')
	{
		result.type = bLisp_EqualToken;
		result.end = _code + 1;
		return result;
	}

	if(*_code == L'<')
	{
		result.type = bLisp_LessToken;
		result.end = _code + 1;
		return result;
	}

	if(*_code == L'>')
	{
		result.type = bLisp_MoreToken;
		result.end = _code + 1;
		return result;
	}

	if(iswdigit(*_code) || *_code == L'+' || *_code == L'-')
	{
		return bLisp_IsDigit(_code + 1);
	}

	if(iswalpha(*_code))
	{
		return bLisp_IsKeyWord(_code);
	}

	result.type = bLisp_ErrorToken;
	result.error = L"We find a new life form";

	return result;
}
/*============================================================================*/
/* bLisp_IsDigit                                                              */
/*============================================================================*/
bLisp_TokenKey bLisp_IsDigit(bchar *_code)
{
	bLisp_TokenKey result;

	if(iswdigit(*_code))
		return bLisp_IsDigit(_code + 1);

	if(*_code == L'.')
		return bLisp_IsFloat(_code + 1);

	if(bLisp_IsChuckNorris(*_code))
	{
		result.type = bLisp_DigitToken;
		result.end = _code;
		return result;
	}

	result.type = bLisp_ErrorToken;
	result.error = L"Wrong format of number!!!";
	return result;
}
/*============================================================================*/
/* bLisp_IsFloat                                                              */
/*============================================================================*/
bLisp_TokenKey bLisp_IsFloat(bchar *_code)
{
	bLisp_TokenKey result;

	if(iswdigit(*_code))
		return bLisp_IsFloat(_code + 1);

	if(bLisp_IsChuckNorris(*_code))
	{
		result.type = bLisp_FloatToken;
		result.end = _code;
		return result;
	}

	result.type = bLisp_ErrorToken;
	result.error = L"Wrong  format of float number!!!";
	return result;
}
/*============================================================================*/
/* bLisp_IsChuckNorris                                                        */
/*============================================================================*/
bbool bLisp_IsChuckNorris(bchar _chuck)
{
	return (_chuck == L' ' || _chuck == L'(' || _chuck == L')' ||
		    _chuck == L'\r'|| _chuck == L'\n'|| _chuck == L'\t'||
			_chuck == L'\0');
}
/*============================================================================*/
/* bLisp_IsKeyWord                                                            */
/*============================================================================*/
bLisp_TokenKey bLisp_IsKeyWord(bchar *_code)
{
	switch(*_code)
	{
	case L'b':
		return bLisp_IsBegin(_code);
	case L'c':
		return bLisp_IsCarCdr(_code);
	case L'i':
		return bLisp_IsIfInclude(_code);
	case L'd':
		return bLisp_IsDefine(_code);
	case L'l':
		return bLisp_IsLambdaList(_code);
	case L'n':
		return bLisp_IsNative(_code);
	case L's':
		return bLisp_IsSet(_code);
	case L'w':
		return bLisp_IsWhile(_code);
	default:
		return bLisp_IsName(_code + 1);
	};
}
/*============================================================================*/
/* bLisp_IsName                                                               */
/*============================================================================*/
bLisp_TokenKey bLisp_IsName(bchar *_code)
{
	bLisp_TokenKey result;

	if(iswalpha(*_code) || iswdigit(*_code) || *_code == L'_')
		return bLisp_IsName(_code + 1);
	if(bLisp_IsChuckNorris(*_code))
	{
		result.type = bLisp_NameToken;
		result.end = _code;
		return result;
	}

	result.type = bLisp_ErrorToken;
	result.error = L"Omg! Wrong name syntax detected!!!";

	return result;
}
/*============================================================================*/
/* bLisp_IsCarCdr                                                             */
/*============================================================================*/
bLisp_TokenKey bLisp_IsCarCdr(bchar *_code)
{
	_code++;

	if(*_code == L'a')
		return bLisp_IsCar(_code);
	else if(*_code == L'd')
		return bLisp_IsCdr(_code);
	else
		return bLisp_IsName(_code);
}
/*============================================================================*/
/* bLisp_IsCar                                                                */
/*============================================================================*/
bLisp_TokenKey bLisp_IsCar(bchar *_code)
{
	int i;
	bLisp_TokenKey result;
	bchar key_word[] = L"ar";

	for(i = 0; i < (sizeof(key_word) / sizeof(bchar)) - 1; i++)
	{
		if(*_code != key_word[i])
			return bLisp_IsName(_code);

		_code++;
	}

	result.type = bLisp_CarToken;
	result.end = _code;

	return result;
}
/*============================================================================*/
/* bLisp_IsCdr                                                                */
/*============================================================================*/
bLisp_TokenKey bLisp_IsCdr(bchar *_code)
{
	int i;
	bLisp_TokenKey result;
	bchar key_word[] = L"dr";

	for(i = 0; i < (sizeof(key_word) / sizeof(bchar)) - 1; i++)
	{
		if(*_code != key_word[i])
			return bLisp_IsName(_code);

		_code++;
	}

	result.type = bLisp_CdrToken;
	result.end = _code;

	return result;
}
/*============================================================================*/
/* bLisp_IsWhile                                                              */
/*============================================================================*/
bLisp_TokenKey bLisp_IsWhile(bchar *_code)
{
	int i;
	bLisp_TokenKey result;
	bchar key_word[] = L"while";
	
	for(i = 0; i < (sizeof(key_word) / sizeof(bchar)) - 1; i++)
	{
		if(*_code != key_word[i])
			return bLisp_IsName(_code);

		_code++;
	}

	result.type = bLisp_WhileToken;
	result.end = _code;

	return result;
}
/*============================================================================*/
/* bLisp_IsIfInclude                                                          */
/*============================================================================*/
bLisp_TokenKey bLisp_IsIfInclude(bchar *_code)
{
	_code++;

	if(*_code == L'n')
		return bLisp_IsInclude(_code);
	else if(*_code == L'f')
		return bLisp_IsIf(_code);
	else
		return bLisp_IsName(_code);
}
/*============================================================================*/
/* bLisp_IsInclude                                                            */
/*============================================================================*/
bLisp_TokenKey bLisp_IsInclude(bchar *_code)
{
	int i;
	bLisp_TokenKey result;
	bchar key_word[] = L"nclude";

	for(i = 0; i < (sizeof(key_word) / sizeof(bchar)) - 1; i++)
	{
		if(*_code != key_word[i])
			return bLisp_IsName(_code);

		_code++;
	}

	result.type = bLisp_IncludeToken;
	result.end = _code;

	return result;
}
/*============================================================================*/
/* bLisp_IsDefine                                                             */
/*============================================================================*/
bLisp_TokenKey bLisp_IsDefine(bchar *_code)
{
	int i;
	bLisp_TokenKey result;
	bchar key_word[] = L"define";

	for(i = 0; i < (sizeof(key_word) / sizeof(bchar)) - 1; i++)
	{
		if(*_code != key_word[i])
			return bLisp_IsName(_code);

		_code++;
	}

	result.type = bLisp_DefineToken;
	result.end = _code;

	return result;
}
/*============================================================================*/
/* bLisp_IsLambdaList                                                         */
/*============================================================================*/
bLisp_TokenKey bLisp_IsLambdaList(bchar *_code)
{
	_code++;

	if(*_code == L'a')
		return bLisp_IsLambda(_code);
	else if(*_code == L'i')
		return bLisp_IsList(_code);
	else
		return bLisp_IsName(_code);
}
/*============================================================================*/
/* bLisp_IsList                                                               */
/*============================================================================*/
bLisp_TokenKey bLisp_IsList(bchar *_code)
{
	int i;
	bLisp_TokenKey result;
	bchar key_word[] = L"ist";

	for(i = 0; i < (sizeof(key_word) / sizeof(bchar)) - 1; i++)
	{
		if(*_code != key_word[i])
			return bLisp_IsName(_code);

		_code++;
	}

	result.type = bLisp_ListToken;
	result.end = _code;

	return result;
}
/*============================================================================*/
/* bLisp_IsLambda                                                             */
/*============================================================================*/
bLisp_TokenKey bLisp_IsLambda(bchar *_code)
{
	int i;
	bLisp_TokenKey result;
	bchar key_word[] = L"ambda";

	for(i = 0; i < (sizeof(key_word) / sizeof(bchar)) - 1; i++)
	{
		if(*_code != key_word[i])
			return bLisp_IsName(_code);

		_code++;
	}

	result.type = bLisp_LambdaToken;
	result.end = _code;

	return result;
}
/*============================================================================*/
/* bLisp_IsNative                                                             */
/*============================================================================*/
bLisp_TokenKey bLisp_IsNative(bchar *_code)
{
	int i;
	bLisp_TokenKey result;
	bchar key_word[] = L"native";

	for(i = 0; i < (sizeof(key_word) / sizeof(bchar)) - 1; i++)
	{
		if(*_code != key_word[i])
			return bLisp_IsName(_code);

		_code++;
	}

	result.type = bLisp_NativeToken;
	result.end = _code;

	return result;
}
/*============================================================================*/
/* bLisp_IsSet                                                                */
/*============================================================================*/
bLisp_TokenKey bLisp_IsSet(bchar *_code)
{
	int i;
	bLisp_TokenKey result;
	bchar key_word[] = L"set";

	for(i = 0; i < (sizeof(key_word) / sizeof(bchar)) - 1; i++)
	{
		if(*_code != key_word[i])
			return bLisp_IsName(_code);

		_code++;
	}

	result.type = bLisp_SetToken;
	result.end = _code;

	return result;
}
/*============================================================================*/
/* bLisp_IsIf                                                                 */
/*============================================================================*/
bLisp_TokenKey bLisp_IsIf(bchar *_code)
{
	int i;
	bLisp_TokenKey result;
	bchar key_word[] = L"f";

	for(i = 0; i < (sizeof(key_word) / sizeof(bchar)) - 1; i++)
	{
		if(*_code != key_word[i])
			return bLisp_IsName(_code);

		_code++;
	}

	result.type = bLisp_IfToken;
	result.end = _code;

	return result;
}
/*============================================================================*/
/* bLisp_IsBegin                                                              */
/*============================================================================*/
bLisp_TokenKey bLisp_IsBegin(bchar *_code)
{
	int i;
	bLisp_TokenKey result;
	bchar key_word[] = L"begin";

	for(i = 0; i < (sizeof(key_word) / sizeof(bchar)) - 1; i++)
	{
		if(*_code != key_word[i])
			return bLisp_IsName(_code);

		_code++;
	}

	result.type = bLisp_BeginToken;
	result.end = _code;

	return result;
}
/*============================================================================*/
/* bLisp_IsString                                                             */
/*============================================================================*/
bLisp_TokenKey bLisp_IsString(bchar *_code)
{
	bLisp_TokenKey result;

	while(_code++)
	{
		if(*_code == L'\0' || *_code == L'\n')
		{
			result.type = bLisp_ErrorToken;
			result.error = L"Missing '\"'";
			return result;
		}

		if(*_code == L'"')
			break;
	}

	result.type = bLisp_StringToken;
	result.end = _code + 1;
	return result;
}
/*============================================================================*/
/* bLisp_SuckString                                                           */
/*============================================================================*/
bchar *bLisp_SuckString(bchar *_begin, bchar *_end)
{
	bchar *new_string;
	buint size;

	size = (_end - _begin) - 2;

	new_string = malloc(size * sizeof(bchar) + sizeof(bchar));
	wcsncpy(new_string, _begin + 1, size);
	new_string[size] = L'\0';

	return new_string;
}
/*============================================================================*/
/* bLisp_SuckName                                                             */
/*============================================================================*/
bchar *bLisp_SuckName(bchar *_begin, bchar *_end)
{
	bchar *new_string;
	buint size;

	size = _end - _begin;

	new_string = malloc(size * sizeof(bchar) + sizeof(bchar));
	wcsncpy(new_string, _begin, size);
	new_string[size] = L'\0';

	return new_string;
}
/*============================================================================*/
/* bLisp_ReadScript                                                           */
/*============================================================================*/
bchar *bLisp_ReadScriptFile(bchar *_filename)
{
	FILE *file;
	bchar *new_script;
	unsigned char buffer[2], utf16[2] = {255, 254};
	long size, real_size;

	if(!(file = _wfopen(_filename, L"rb")))
		return 0;

	fread(buffer, 1, sizeof(utf16), file);

	if(memcmp(buffer, utf16, sizeof(utf16)) != 0)
		return 0;

	fseek(file, 0, SEEK_END);
	size = ftell(file);
	fseek(file, sizeof(utf16), SEEK_SET);

	new_script = malloc(size + sizeof(bchar));

	real_size = fread(new_script, 1, size, file);
	new_script[real_size / sizeof(bchar)] = L'\0';

	fclose(file);

	return new_script;
}
/*============================================================================*/
/* bLisp_PrintType                                                            */
/*============================================================================*/
bchar *bLisp_PrintType(bLisp_Type *_type)
{
	static bchar text[256];

	switch(_type->type)
	{
	case bLisp_IntType:
		swprintf(text, 256, L"%d", _type->value.int_value);
		break;

	case bLisp_FloatType:
		swprintf(text, 256, L"%g", _type->value.float_value);
		break;

	case bLisp_StringType:
		swprintf(text, 256, L"%s", _type->value.string_value);
		break;

	case bLisp_DataType:
		swprintf(text, 256, L"Data type with adress: %p", _type->value.data_value);
		break;

	case bLisp_LambdaType:
		swprintf(text, 256, L"#lambda");
		break;

	case bLisp_NativeType:
		swprintf(text, 256, L"#native function");
		break;

	case bLisp_ListType:
		swprintf(text, 256, L"#list");
		break;

	default:
		swprintf(text, 256, L"Unknown value");
		break;
	}

	return text;
}
/*============================================================================*/
/* bLisp_SuckDigit                                                            */
/*============================================================================*/
bint bLisp_SuckDigit(bchar *_begin)
{
	return _wtoi(_begin);
} 
/*============================================================================*/
/* bLisp_SuckFloat                                                            */
/*============================================================================*/
bdouble bLisp_SuckFloat(bchar *_begin)
{
	return _wtof(_begin);
}
/*============================================================================*/
/* bLisp_FreeSymbolTable                                                      */
/*============================================================================*/
bvoid bLisp_FreeSymbolTable(bLisp_SymbolTable *_st)
{
	bvoid *temp;

	while(_st->code)
	{
		temp = _st->code;
		_st->code = _st->code->next;
		free(temp);
	}

	while(_st->variable)
	{
		temp = _st->variable->next;
		free(_st->variable->name);
		free(_st->variable);
		_st->variable = temp;
			
	}

	while(_st->function)
	{
		temp = _st->function;
		_st->function = _st->function->next;
		free(temp);
	}

	while(_st->native_function)
	{
		temp = _st->native_function;
		_st->native_function = _st->native_function->next;
		free(temp);
	}

	while(_st->strings)
	{
		temp = _st->strings;
		_st->strings = _st->strings->next;
		free(((bLisp_StringList *)temp)->string);
		free(temp);
	}
}
/*============================================================================*/
/* bLisp_ParseScript                                                          */
/*============================================================================*/
bLisp_Token bLisp_ParseScript(bLisp_Script *_script, bchar *_code)
{
	bLisp_Token       token       ;
	bLisp_SymbolTable symbol_table;

	// ��������� ������� �������� ������
	memset(&symbol_table, 0, sizeof(bLisp_SymbolTable));

	token = bLisp_GetToken(_code);

	while(bLisp_ParseExpression(&symbol_table, &token))
	{
		token = bLisp_GetToken(bLisp_LexerContinue);
	}

	if(token.type == bLisp_ErrorToken)
	{
		bLisp_FreeSymbolTable(&symbol_table);
		return token;
	}

	bLisp_Compress(_script, &symbol_table);
	bLisp_FreeSymbolTable(&symbol_table);

	return token;
}
/*============================================================================*/
/* bLisp_ParseOperator                                                        */
/*============================================================================*/
bbool bLisp_ParseOperator(bLisp_SymbolTable *_st, bLisp_Token *_token)
{
	bLisp_Code new_code;

	switch(_token->type)
	{
	case bLisp_PlusToken:
		new_code.type =  bLisp_PlusCode;
		break;
	case bLisp_MinusToken:
		new_code.type = bLisp_MinusCode;
		break;
	case bLisp_MultiplyToken:
		new_code.type = bLisp_MultiplyCode;
		break;
	case bLisp_DivideToken:
		new_code.type = bLisp_DivideCode;
		break;
	case bLisp_EqualToken:
		new_code.type = bLisp_EqualCode;
		break;
	case bLisp_LessToken:
		new_code.type = bLisp_LessCode;
		break;
	case bLisp_MoreToken:
		new_code.type = bLisp_MoreCode;
		break;
	}

	bLisp_AddCode(&_st->code, &new_code);

	if((*_token = bLisp_GetToken(bLisp_LexerContinue)).type == bLisp_RightBraceToken)
		return bLisp_Error(_token, L"Operator needs only two arguments");
	if(!bLisp_ParseExpression(_st, _token))
		return bfalse;
	if((*_token = bLisp_GetToken(bLisp_LexerContinue)).type == bLisp_RightBraceToken)
		return bLisp_Error(_token, L"Operator needs only two arguments");
	if(!bLisp_ParseExpression(_st, _token))
		return bfalse;

	if((*_token = bLisp_GetToken(bLisp_LexerContinue)).type != bLisp_RightBraceToken)
		return bLisp_Error(_token, L"Missing ')' or so many arguments");

	new_code.type = bLisp_EndCode;
	bLisp_AddCode(&_st->code, &new_code);

	return btrue;
}
/*============================================================================*/
/* bLisp_ParseDefine                                                          */
/*============================================================================*/
bbool bLisp_ParseDefine(bLisp_SymbolTable *_st, bLisp_Token *_token)
{
	bLisp_Code new_code;

	new_code.type = bLisp_SetCode;
	bLisp_AddCode(&_st->code, &new_code);

	*_token = bLisp_GetToken(bLisp_LexerContinue);
	if(_token->type != bLisp_NameToken)
		return bLisp_Error(_token, L"Missing variable name in define expression");

	new_code.type = bLisp_VariableCode;
	new_code.value.variable_value = bLisp_AddVariable(_st, _token->value.name_value);
	bLisp_AddCode(&_st->code, &new_code);
	bLisp_FreeToken(_token);

	*_token = bLisp_GetToken(bLisp_LexerContinue);
	if(_token->type == bLisp_RightBraceToken)
		return bLisp_Error(_token, L"Missing variable value");

	if(!bLisp_ParseExpression(_st, _token))
		return bfalse;

	*_token = bLisp_GetToken(bLisp_LexerContinue);
	if(_token->type != bLisp_RightBraceToken)
		return bLisp_Error(_token, L"Missing ')' in define");

	new_code.type = bLisp_EndCode;
	bLisp_AddCode(&_st->code, &new_code);

	return btrue;
}
/*============================================================================*/
/* bLisp_ParseSet                                                             */
/*============================================================================*/
bbool bLisp_ParseSet(bLisp_SymbolTable *_st, bLisp_Token *_token)
{
	bLisp_Code new_code;

	new_code.type = bLisp_SetCode;
	bLisp_AddCode(&_st->code, &new_code);

	*_token = bLisp_GetToken(bLisp_LexerContinue);
	if(_token->type == bLisp_DigitToken)
		return bLisp_Error(_token, L"Missing variable name in set expression");
	if(!bLisp_ParseExpression(_st, _token))
		return bfalse;

	*_token = bLisp_GetToken(bLisp_LexerContinue);
	if(_token->type == bLisp_RightBraceToken)
		return bLisp_Error(_token, L"Missing second argument in set expression");
	if(!bLisp_ParseExpression(_st, _token))
		return bfalse;

	*_token = bLisp_GetToken(bLisp_LexerContinue);
	if(_token->type != bLisp_RightBraceToken)
		return bLisp_Error(_token, L"Missing ')'");

	new_code.type = bLisp_EndCode;
	bLisp_AddCode(&_st->code, &new_code);

	return btrue;
}
/*============================================================================*/
/* bLisp_ParseIf                                                              */
/*============================================================================*/
bbool bLisp_ParseIf(bLisp_SymbolTable *_st, bLisp_Token *_token)
{
	bLisp_Code new_code, *if_code;

	new_code.type = bLisp_IfCode;
	bLisp_AddCode(&_st->code, &new_code);
	if_code = &_st->code->code;

	*_token = bLisp_GetToken(bLisp_LexerContinue);
	if(_token->type == bLisp_RightBraceToken)
		return bLisp_Error(_token, L"If expression needs two or three arguments");
	if(!bLisp_ParseExpression(_st, _token))
		return bfalse;

	*_token = bLisp_GetToken(bLisp_LexerContinue);
	if(_token->type == bLisp_RightBraceToken)
		return bLisp_Error(_token, L"If expression needs two or three arguments");
	if(!bLisp_ParseExpression(_st, _token))
		return bfalse;

	// ���������� ������� ������� ���������
	if_code->value.if_value.second_pos = bLisp_GetCodeSize(_st->code);

	*_token = bLisp_GetToken(bLisp_LexerContinue);
	if(_token->type != bLisp_RightBraceToken)
	{
		if(!bLisp_ParseExpression(_st, _token))
			return bfalse;
		*_token = bLisp_GetToken(bLisp_LexerContinue);
	}

	// ���������� ������� ����� ��������� ���������
	if_code->value.if_value.end_pos = bLisp_GetCodeSize(_st->code);

	if(_token->type != bLisp_RightBraceToken)
		return bLisp_Error(_token, L"Missing ')' in if");

	new_code.type = bLisp_EndCode;
	bLisp_AddCode(&_st->code, &new_code);

	return btrue;
}
/*============================================================================*/
/* bLisp_ParseBegin                                                           */
/*============================================================================*/
bbool bLisp_ParseBegin(bLisp_SymbolTable *_st, bLisp_Token *_token)
{
	bLisp_Code new_code;

	new_code.type = bLisp_BeginCode;
	bLisp_AddCode(&_st->code, &new_code);

	while((*_token = bLisp_GetToken(bLisp_LexerContinue)).type != bLisp_RightBraceToken)
	{
		if(!bLisp_ParseExpression(_st, _token))
			return bfalse;
	}

	new_code.type = bLisp_EndCode;
	bLisp_AddCode(&_st->code, &new_code);

	return btrue;
}
/*============================================================================*/
/* bLisp_ParseList                                                            */
/*============================================================================*/
bbool bLisp_ParseList(bLisp_SymbolTable *_st, bLisp_Token *_token)
{
	bLisp_Code new_code;

	new_code.type = bLisp_ListCode;
	bLisp_AddCode(&_st->code, &new_code);

	while((*_token = bLisp_GetToken(bLisp_LexerContinue)).type != bLisp_RightBraceToken)
	{
		bLisp_ParseExpression(_st, _token);
	}

	new_code.type = bLisp_EndCode;
	bLisp_AddCode(&_st->code, &new_code);

	return btrue;
}
/*============================================================================*/
/* bLisp_ParseCarCdr                                                          */
/*============================================================================*/
bbool bLisp_ParseCarCdr(bLisp_SymbolTable *_st, bLisp_Token *_token)
{
	bLisp_Code new_code;

	new_code.type = _token->type == bLisp_CarToken ? bLisp_CarCode : bLisp_CdrCode;
	bLisp_AddCode(&_st->code, &new_code);

	*_token = bLisp_GetToken(bLisp_LexerContinue);
	if(_token->type == bLisp_RightBraceToken)
		return bLisp_Error(_token, L"Car/cdr needs one argument");
	if(!bLisp_ParseExpression(_st, _token))
		return bfalse;

	*_token = bLisp_GetToken(bLisp_LexerContinue);
	if(_token->type != bLisp_RightBraceToken)
		return bLisp_Error(_token, L"Missing ')' or so many arguments");

	new_code.type = bLisp_EndCode;
	bLisp_AddCode(&_st->code, &new_code);

	return btrue;
}
/*============================================================================*/
/* bLisp_ParseWhile                                                           */
/*============================================================================*/
bbool bLisp_ParseWhile(bLisp_SymbolTable *_st, bLisp_Token *_token)
{
	bLisp_Code new_code, *while_code;

	new_code.type = bLisp_WhileCode;
	bLisp_AddCode(&_st->code, &new_code);
	while_code = &_st->code->code;

	*_token = bLisp_GetToken(bLisp_LexerContinue);
	if(_token->type == bLisp_RightBraceToken)
		return bLisp_Error(_token, L"Missing condition");
	if(!bLisp_ParseExpression(_st, _token))
		return bfalse;

	*_token = bLisp_GetToken(bLisp_LexerContinue);
	if(_token->type == bLisp_RightBraceToken)
		return bLisp_Error(_token, L"Missing body");
	if(!bLisp_ParseExpression(_st, _token))
		return bfalse;

	while_code->value.while_value = bLisp_GetCodeSize(_st->code);

	*_token = bLisp_GetToken(bLisp_LexerContinue);
	if(_token->type != bLisp_RightBraceToken)
		return bLisp_Error(_token, L"Missing ')' or so many arguments");

	new_code.type = bLisp_EndCode;
	bLisp_AddCode(&_st->code, &new_code);

	return btrue;
}
/*============================================================================*/
/* bLisp_ParseLambda                                                          */
/*============================================================================*/
bbool bLisp_ParseLambda(bLisp_SymbolTable *_st, bLisp_Token *_token)
{
	bLisp_Code new_code;
	buint end_pos, arg_num, error_line;
	bchar *error_file;
	bLisp_Function f, *f_ptr;

	error_file = _token->file;
	error_line = _token->line;

	*_token = bLisp_GetToken(bLisp_LexerContinue);
	if(_token->type != bLisp_LeftBraceToken)
		return bLisp_Error(_token, L"Missing lambda argument list");

	for(arg_num = 0; (*_token = bLisp_GetToken(bLisp_LexerContinue)).type != bLisp_RightBraceToken; arg_num++)
	{
		if(_token->type != bLisp_NameToken)
			return bLisp_Error(_token, L"Wrong function parameter list");

		bLisp_AddLocalVariable(&_st->local_var, _token->value.name_value);
		bLisp_FreeToken(_token);
	}

	f.arg_num = arg_num;
	f.ptr = bLisp_GetCodeSize(_st->code);
	new_code.type = bLisp_LambdaCode;
	new_code.value.lambda_value = bLisp_AddFunction(_st, &f);
	f_ptr = &_st->function->function;
	bLisp_AddCode(&_st->code, &new_code);
	f_ptr->ptr = bLisp_GetCodeSize(_st->code);

	while((*_token = bLisp_GetToken(bLisp_LexerContinue)).type != bLisp_RightBraceToken)
	{
		if(_token->type == bLisp_EofToken)
		{
			_token->file = error_file;
			_token->line = error_line;

			return bLisp_Error(_token, L"Missing ')' in lambda");
		}

		if(!bLisp_ParseExpression(_st, _token))
			return bfalse;
	}

	end_pos = bLisp_GetCodeSize(_st->code);
	f_ptr->end_ptr = end_pos;

	bLisp_FreeLocalVariable(&_st->local_var, arg_num);

	new_code.type = bLisp_EndCode;
	bLisp_AddCode(&_st->code, &new_code);

	return btrue;
}
/*============================================================================*/
/* bLisp_ParseNative                                                          */
/*============================================================================*/
bbool bLisp_ParseNative(bLisp_SymbolTable *_st, bLisp_Token *_token)
{
	bLisp_Code new_code;

	new_code.type = bLisp_NativeCode;
	new_code.value.native_value = bLisp_AddNativeFunction(_st);
	bLisp_AddCode(&_st->code, &new_code);

	*_token = bLisp_GetToken(bLisp_LexerContinue);
	if(_token->type != bLisp_StringToken)
		return bLisp_Error(_token, L"Missing function name");

	_st->native_function->native_function.name = bLisp_AddString(_st, _token->value.string_value);
	free(_token->value.string_value);

	*_token = bLisp_GetToken(bLisp_LexerContinue);
	if(_token->type != bLisp_RightBraceToken)
		return bLisp_Error(_token, L"Missing ')' in native");

	new_code.type = bLisp_EndCode;
	bLisp_AddCode(&_st->code, &new_code);

	return btrue;
}
/*============================================================================*/
/* bLisp_ParseCall                                                            */
/*============================================================================*/
bbool bLisp_ParseCall(bLisp_SymbolTable *_st, bLisp_Token *_token)
{
	bLisp_Code new_code;
	bchar *error_file;
	buint  error_line;

	error_file = _token->file;
	error_line = _token->line;

	if(_token->type == bLisp_LeftBraceToken)
	{
		new_code.type = bLisp_CallCode;
		new_code.value.lambda_value = bfalse;
		bLisp_AddCode(&_st->code, &new_code);
	}
	else
	{
		new_code.type = bLisp_CallCode;
		new_code.value.lambda_value = bfalse;
		bLisp_AddCode(&_st->code, &new_code);
		if(!bLisp_FindVariable(_st, _token))
			return bLisp_Error(_token, L"Unknown function found");
	}

	while((*_token = bLisp_GetToken(bLisp_LexerContinue)).type != bLisp_RightBraceToken)
	{
		if(_token->type == bLisp_EofToken)
		{
			_token->file = error_file;
			_token->line = error_line;

			return bLisp_Error(_token, L"Missing ')' in call");
		}

		if(!bLisp_ParseExpression(_st, _token))
			return bfalse;
	} 

	new_code.type = bLisp_EndCode;
	bLisp_AddCode(&_st->code, &new_code);

	return btrue;
}
/*============================================================================*/
/* bLisp_ParseExpression                                                      */
/*============================================================================*/
bbool bLisp_ParseExpression(bLisp_SymbolTable *_st, bLisp_Token *_token)
{
	bLisp_Code new_code;

	if(_token->type == bLisp_LeftBraceToken)
	{
		*_token = bLisp_GetToken(bLisp_LexerContinue);

		switch(_token->type)
		{
		case bLisp_PlusToken:
		case bLisp_MinusToken:
		case bLisp_MultiplyToken:
		case bLisp_DivideToken:
		case bLisp_EqualToken:
		case bLisp_LessToken:
		case bLisp_MoreToken:
			return bLisp_ParseOperator(_st, _token);
		
		case bLisp_CarToken:
		case bLisp_CdrToken:
			return bLisp_ParseCarCdr(_st, _token);

		case bLisp_DefineToken:
			return bLisp_ParseDefine(_st, _token);

		case bLisp_SetToken:
			return bLisp_ParseSet(_st, _token);

		case bLisp_IfToken:
			return bLisp_ParseIf(_st, _token);

		case bLisp_BeginToken:
			return bLisp_ParseBegin(_st, _token);

		case bLisp_ListToken:
			return bLisp_ParseList(_st, _token);

		case bLisp_WhileToken:
			return bLisp_ParseWhile(_st, _token);

		case bLisp_LambdaToken:
			return bLisp_ParseLambda(_st, _token);

		case bLisp_NativeToken:
			return bLisp_ParseNative(_st, _token);

		case bLisp_NameToken:
			return bLisp_ParseCall(_st, _token);

		case bLisp_LeftBraceToken:
			return bLisp_ParseCall(_st, _token);

		default:
			return bLisp_Error(_token, L"Wrong function token");
		}
	}
	else
	{
		switch(_token->type)
		{
		case bLisp_RightBraceToken:
			new_code.type = bLisp_EndCode;
			bLisp_AddCode(&_st->code, &new_code);
			break;

		case bLisp_EofToken:
			new_code.type = bLisp_EndCode;
			bLisp_AddCode(&_st->code, &new_code);
			return bfalse;

		case bLisp_NameToken:
			return bLisp_FindVariable(_st, _token);

		case bLisp_DigitToken:
			new_code.value.const_value.type = bLisp_IntType;
			new_code.value.const_value.value.int_value = _token->value.int_value;
			new_code.type = bLisp_ConstCode;
			bLisp_AddCode(&_st->code, &new_code);
			break;

		case bLisp_FloatToken:
			new_code.value.const_value.type = bLisp_FloatType;
			new_code.value.const_value.value.float_value = _token->value.float_value;
			new_code.type = bLisp_ConstCode;
			bLisp_AddCode(&_st->code, &new_code);
			break;

		case bLisp_StringToken:
			new_code.value.const_value.type = bLisp_StringType;
			new_code.value.const_value.value.string_value = bLisp_AddString(_st, _token->value.string_value);
			bLisp_FreeToken(_token);
			new_code.type = bLisp_ConstCode;
			bLisp_AddCode(&_st->code, &new_code);
			break;

		case bLisp_LambdaToken:
			return bLisp_ParseLambda(_st, _token);

		case bLisp_ErrorToken:
			return bLisp_Error(_token, _token->value.error_value);

		default:
			return bLisp_Error(_token, L"Wrong expression token");
		}
	}

	return btrue;
}
/*============================================================================*/
/* bLisp_AddCode                                                              */
/*============================================================================*/
bvoid bLisp_AddCode(bLisp_CodeList **_code_list, bLisp_Code *_code)
{
	bLisp_CodeList *new_code;

	new_code = malloc(sizeof(bLisp_CodeList));

	new_code->code = *_code;
	new_code->next = *_code_list;
	*_code_list = new_code;
}
/*============================================================================*/
/* bLisp_AddVariable                                                          */
/*============================================================================*/
buint bLisp_AddVariable(bLisp_SymbolTable *_st, bchar *_name)
{
	bLisp_VariableList *new_variable;
	buint index;

	new_variable = malloc(sizeof(bLisp_VariableList));

	new_variable->name = wcsdup(_name);
	new_variable->next = _st->variable;
	_st->variable = new_variable;

	for(index = 0; new_variable; new_variable = new_variable->next, index++);

	return index - 1;
}
/*===========================================================================*/
/* bLisp_AddString                                                           */
/*===========================================================================*/
bchar *bLisp_AddString(bLisp_SymbolTable *_st, bchar *_string)
{
	bLisp_StringList *new_string;
	buint index;

	new_string = malloc(sizeof(bLisp_StringList));

	new_string->string = wcsdup(_string);
	new_string->next = _st->strings;
	_st->strings = new_string;

	for(index = 0; new_string; new_string = new_string->next, index++);

	return (bchar *)(index - 1);
}
/*============================================================================*/
/* bLisp_AddLocalVariable                                                     */
/*============================================================================*/
bvoid bLisp_AddLocalVariable(bLisp_LocalVarList **_lvl, bchar *_name)
{
	bLisp_LocalVarList *new_local_var;

	new_local_var = malloc(sizeof(bLisp_LocalVarList));

	new_local_var->name = wcsdup(_name);
	new_local_var->next = *_lvl;
	*_lvl = new_local_var;
}
/*============================================================================*/
/* bLisp_FreeLocalVariable                                                    */
/*============================================================================*/
bvoid bLisp_FreeLocalVariable(bLisp_LocalVarList **_lvl, buint _size)
{
	bLisp_LocalVarList *temp_var;

	while(_size)
	{
		temp_var = (*_lvl)->next;
		free((*_lvl)->name);
		free(*_lvl);
		*_lvl = temp_var;
		_size--;
	}
}
/*============================================================================*/
/* bLisp_AddFunction                                                          */
/*============================================================================*/
buint bLisp_AddFunction(bLisp_SymbolTable *_st, bLisp_Function *_f)
{
	bLisp_FunctionList *new_function;
	buint index;

	new_function = malloc(sizeof(bLisp_FunctionList));

	new_function->function = *_f;
	new_function->next = _st->function;
	_st->function = new_function;

	for(index = 0; new_function; new_function = new_function->next, index++);

	return index;
}
/*============================================================================*/
/* bLisp_AddNativeFunction                                                    */
/*============================================================================*/
buint bLisp_AddNativeFunction(bLisp_SymbolTable *_st)
{
	bLisp_NativeFunctionList *new_native_function;
	buint index;

	new_native_function = malloc(sizeof(bLisp_NativeFunctionList));

	new_native_function->next = _st->native_function;
	_st->native_function = new_native_function;

	for(index = 0; new_native_function; new_native_function = new_native_function->next, index++);

	return index;
}
/*============================================================================*/
/* bLisp_FindNativeFunction                                                   */
/*============================================================================*/
bLisp_NativeFunction *bLisp_FindNativeFunction(bLisp_Script *_script, bchar *_name)
{
	buint i;

	for(i = 0; i < _script->nfunc_size; i++)
	{
		if(wcscmp(_script->native_func[i].name, _name) == 0)
			return &_script->native_func[i];
	}

	return 0;
}
/*============================================================================*/
/* bLisp_FindVariable                                                         */
/*============================================================================*/
bbool bLisp_FindVariable(bLisp_SymbolTable *_st, bLisp_Token *_token)
{
	bLisp_Code new_code;
	buint result;
	
	if((result = bLisp_FindLocalVariable(_st, _token->value.name_value)) != -1)
		new_code.type = bLisp_LocalVarCode;
	else
		if((result = bLisp_FindGlobalVariable(_st, _token->value.name_value)) != -1)
			new_code.type = bLisp_VariableCode;
		else
		{
			bLisp_Error(_token, L"Unknown variable found");
			return bfalse;
		}

	new_code.value.variable_value = result;
	bLisp_AddCode(&_st->code, &new_code);

	bLisp_FreeToken(_token);

	return btrue;
}
/*============================================================================*/
/* bLisp_FindGlobalVariable                                                   */
/*============================================================================*/
buint bLisp_FindGlobalVariable(bLisp_SymbolTable *_st, bchar *_name)
{
	bLisp_VariableList *var;
	buint i, k;

	for(var = _st->variable, i = 0; var; i++, var = var->next)
		if(wcscmp(var->name, _name) == 0)
		{
			for(var = _st->variable, k = 0; var; k++, var = var->next);

			return k - i - 1;
		}

	return -1;
}
/*============================================================================*/
/* bLisp_FindLocalVariable                                                    */
/*============================================================================*/
buint bLisp_FindLocalVariable(bLisp_SymbolTable *_st, bchar *_name)
{
	bLisp_LocalVarList *lvar;
	buint i;

	for(lvar = _st->local_var, i = 0; lvar; i++, lvar = lvar->next)
		if(wcscmp(lvar->name, _name) == 0)
			return i;

	return -1;
}
/*============================================================================*/
/* bLisp_Compress                                                             */
/*============================================================================*/
bvoid bLisp_Compress(bLisp_Script *_script, bLisp_SymbolTable *_st)
{
	buint size;
	bLisp_CodeList *code;
	bLisp_VariableList *var;
	bLisp_FunctionList *func;
	bLisp_NativeFunctionList *native_func;

	// compress code
	for(size = 0, code = _st->code; code; code = code->next, size++);

	_script->code = malloc(size * sizeof(bLisp_Code));
	_script->code_size = size;

	for(code = _st->code; code; code = code->next, size--)
		_script->code[size - 1] = code->code;
	_script->code_start = _script->code;

	//compress variables
	for(size = 0, var = _st->variable; var; var = var->next, size++);

	_script->var = malloc(size * sizeof(bLisp_Variable));
	_script->var_size = size;

	for(var = _st->variable; var; var = var->next, size--)
		_script->var[size - 1] = var->variable;

	//compress functions
	for(size = 0, func = _st->function; func; func = func->next, size++);

	_script->func = malloc(size * sizeof(bLisp_Function));
	_script->func_size = size;

	for(func = _st->function; func; func = func->next, size--)
		_script->func[size - 1] = func->function;

	//compress native functions
	for(size = 0, native_func = _st->native_function; native_func; native_func = native_func->next, size++);

	_script->native_func = malloc(size * sizeof(bLisp_NativeFunction));
	_script->nfunc_size = size;
	for(native_func = _st->native_function; native_func; native_func = native_func->next, size--)
		_script->native_func[size - 1] = native_func->native_function;

	bLisp_CompressString(_script, _st);
}
/*============================================================================*/
/* bLisp_CompressString                                                       */
/*============================================================================*/
bvoid bLisp_CompressString(bLisp_Script *_script, bLisp_SymbolTable *_st)
{
	buint size;
	bLisp_StringList *string;

	for(size = 0, string = _st->strings; string; string = string->next)
		size += wcslen(string->string) * sizeof(bchar) + sizeof(bchar);

	_script->string_size = size;
	_script->string = malloc(size);

	for(string = _st->strings; string; string = string->next)
	{
		memcpy(&_script->string[(size / sizeof(bchar)) - wcslen(string->string) - 1], string->string,
		wcslen(string->string) * sizeof(bchar) + sizeof(bchar));
		size -= wcslen(string->string) * sizeof(bchar) + sizeof(bchar);
	}
}
/*============================================================================*/
/* bLisp_GetCodeSize                                                          */
/*============================================================================*/
buint bLisp_GetCodeSize(bLisp_CodeList *_code)
{
	buint i;

	for(i = 0; _code; _code = _code->next, i++);

	return i;
}
/*============================================================================*/
/* bLisp_RegFunc_IsSeparator                                                  */
/*============================================================================*/
bbool bLisp_RegFunc_IsSeparator(bchar _c)
{
	if(_c == L'*' || _c == L'(' || 
	   _c == L')' || _c == L',')
	   return btrue;

	return bfalse;
}
/*============================================================================*/
/* bLisp_RegFunc_StrCmp                                                       */
/*============================================================================*/
bbool bLisp_RegFunc_StrCmp(bchar *_code, bchar *_str)
{
	while(*_str)
	{
		if(*_code++ != *_str++)
			return bfalse;
	}

	if(bLisp_RegFunc_IsSeparator(*_code))
	   return btrue;

	return bfalse;
}
/*============================================================================*/
/* bLisp_RegFunc_GetToken                                                     */
/*============================================================================*/
bLisp_TypeClass bLisp_RegFunc_GetToken(bchar *_code)
{
	if(bLisp_RegFunc_StrCmp(_code, L"int"    ))        
		return bLisp_IntType;

	if(bLisp_RegFunc_StrCmp(_code, L"double" ))
		return bLisp_FloatType;

	if(bLisp_RegFunc_StrCmp(_code, L"void"   ))
		return bLisp_VoidType;

	if(bLisp_RegFunc_StrCmp(_code, L"wchar_t")||
	   bLisp_RegFunc_StrCmp(_code, L"char"   ))
		return bLisp_StringType;

	return bLisp_IntType;
}
/*============================================================================*/
/* bLisp_RegFunc_SkipCode                                                     */
/*============================================================================*/
bchar *bLisp_RegFunc_SkipCode(bchar *_code)
{
	while(*_code)
	{
		if(bLisp_RegFunc_IsSeparator(*_code))
			return _code;

		_code++;
	}

	return _code;
}
/*============================================================================*/
/* bLisp_SkipExpression                                                       */
/*============================================================================*/
bLisp_Code *bLisp_SkipExpression(bLisp_Code *_code)
{
	if(bLisp_EvalCode)
	{
		for(_code++; _code->type != bLisp_EndCode; _code++)
			_code = bLisp_SkipExpression(_code);
	}

	return _code;
}
/*============================================================================*/
/* bLisp_FindLastExpression                                                   */
/*============================================================================*/
bLisp_Code *bLisp_FindLastExpression(bLisp_Code *_code)
{
	bLisp_Code *expression;

	while(_code->type != bLisp_EndCode)
	{
		expression = _code;
		if(bLisp_EvalCode)
			_code = bLisp_SkipExpression(_code);
		_code++;
	}

	return expression;
}
/*============================================================================*/
/* bLisp_FindLastExpressionInBeginAndIf                                       */
/*============================================================================*/
bLisp_Code *bLisp_FindLastExpressionInBeginAndIf(bLisp_Code *_code)
{
	if(_code->type == bLisp_BeginCode)
		_code = bLisp_FindLastExpressionInBegin(_code);
	if(_code->type == bLisp_IfCode)
		_code = bLisp_FindLastExpressionInIf(_code);
	else if(_code->type == bLisp_CallCode)
		_code->value.lambda_value = btrue;

	return _code;
}
/*============================================================================*/
/* bLisp_FindLastExpressionInBegin                                            */
/*============================================================================*/
bLisp_Code *bLisp_FindLastExpressionInBegin(bLisp_Code *_code)
{
	_code = bLisp_FindLastExpression(_code +  1);

	return bLisp_FindLastExpressionInBeginAndIf(_code);
}
/*============================================================================*/
/* _FindLastExpressionInIf                                                    */
/*============================================================================*/
bLisp_Code *bLisp_FindLastExpressionInIf(bLisp_Code *_code)
{
	_code = bLisp_SkipExpression(_code + 1);

	_code = bLisp_FindLastExpressionInBeginAndIf(_code + 1);

	_code = bLisp_SkipExpression(_code);

	return bLisp_FindLastExpressionInBeginAndIf(_code + 1);
}
/*============================================================================*/
/* bLisp_OptimizerTailRecursion                                               */
/*============================================================================*/
bvoid bLisp_OptimizeTailRecursion(bLisp_Script *_script)
{
	bLisp_Code *new_code, *code = _script->code;

	while((buint)(code - _script->code_start) < _script->code_size - 1)
	{
		code++;

		if(code->type == bLisp_LambdaCode)
		{
			new_code = code;

			code = bLisp_FindLastExpression(code + 1);

			code = bLisp_FindLastExpressionInBeginAndIf(code);

			code = bLisp_SkipExpression(new_code);
		}
	}
}
/*============================================================================*/
/* bLisp_Optimizer                                                            */
/*============================================================================*/
bvoid bLisp_Optimizer(bLisp_Script *_script)
{
	bLisp_OptimizeTailRecursion(_script);
}
/*============================================================================*/
/* bLisp_Error                                                                */
/*============================================================================*/
bbool bLisp_Error(bLisp_Token *_token, bchar *_text)
{
	// ������� ������������ ������
	bLisp_FreeToken(_token);

	_token->type = bLisp_ErrorToken;
	_token->value.error_value = _text;

	_token->file = wcsdup(_token->file);

	// ������� ��� ������ �������
	bLisp_GetToken(bLisp_LexerFree);

	// ������ ���������� 0, ������ ��� � ��� ������ :-(
	return bfalse;
}
/*============================================================================*/
/* �������� ������� ��� ��������                                              */
/*============================================================================*/
#pragma warning(pop)