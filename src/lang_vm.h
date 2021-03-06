//-------------------------------------------------------------------
//
// THANKS TO:
// ----------------------------------------------
//
//   01 : God the creator of the heavens and the earth in the name of Jesus Christ.
//
// ----------------------------------------------
//
// THIS FILE IS PART OF APPLICATION API:
//
// VM - The Virtual Machine Header:
//
// START DATE: 04/11/2018 - 07:00
//
//-------------------------------------------------------------------
//
#ifndef _VM_H_
#define _VM_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------
//---------------  DEFINE / ENUM  ---------------
//-----------------------------------------------
//
#define LIBIMPORT       extern
#define VM_DEFAULT_SIZE 50000
#define UCHAR           unsigned char
#define TYPE_NO_RETURN  100
#define GVAR_SIZE       255

enum {
    OP_PUSH_LONG = 0,
    OP_PUSH_FLOAT,
    OP_PUSH_VAR,
    OP_PUSH_STRING,
    OP_POP_VAR,
		OP_POP_LOCAL,
		OP_PUSH_ARG,
		OP_PUSH_LOCAL,
		//
		OP_INC_VAR_LONG,
		OP_INC_LOCAL_LONG,
		OP_DEC_VAR_LONG,
		OP_DEC_LOCAL_LONG,
    //
    OP_MUL_LONG,
    OP_DIV_LONG,
    OP_ADD_LONG,
    OP_SUB_LONG,
    //
    OP_MUL_FLOAT,
    OP_DIV_FLOAT,
    OP_ADD_FLOAT,
    OP_SUB_FLOAT,
    OP_POP_EAX,
    OP_PRINT_EAX,
    OP_MOV_EAX_VAR,
		// compare long:
		OP_CMP_LONG,
		OP_JUMP_JMP,
		OP_JUMP_JE,
		OP_JUMP_JG,
		OP_JUMP_JL,
		OP_JUMP_JNE,
		OP_JUMP_JLE,
		OP_JUMP_JGE,
    //
    OP_CALL,
    OP_CALL_VM,
    //
    OP_HALT
};
enum {
    TYPE_LONG = 0,
    TYPE_FLOAT,
    TYPE_FLOATP,  // float *fp;
    TYPE_STRING,
    TYPE_POINTER,
    TYPE_STRUCT,
    TYPE_PSTRUCT,  // struct data *p;
    TYPE_UNKNOW
};

//-----------------------------------------------
//-------------------  STRUCT  ------------------
//-----------------------------------------------
//
typedef struct VM       VM;
typedef struct VM_label VM_label;
typedef struct VM_jump  VM_jump;
typedef union  VALUE    VALUE;
typedef struct TVar     TVar;

union VALUE {
    long    l;  //: type integer
    float   f;  //: type float
    char    *s; //: type pointer of char
    void    *p; //: type pointer
};
struct TVar {
    char    *name;
    int     type;
    VALUE   value;
    void    *info;  // any information ... struct type use this
};
struct VM { // opaque struct
    UCHAR     *p;
    UCHAR     *code;
    VM_label  *label;
    VM_jump   *jump;
    int       size;
    int       ip;
    VALUE     arg [10];
    //
    TVar      *local;
    int       local_count;
};
struct VM_label {
    char      *name;
    int       pos;
    VM_label  *next;
};
struct VM_jump {
    char      *name;
    int       pos;
    VM_jump   *next;
};

//-----------------------------------------------
//---------------  GLOBAL VARIABLE  -------------
//-----------------------------------------------
//
LIBIMPORT TVar Gvar [GVAR_SIZE]; // global in file: "core.c"

//------------------------------------------------
//-----------------  PUBLIC API  -----------------
//------------------------------------------------
//
LIBIMPORT VM    * vm_New    (unsigned int size);
LIBIMPORT VALUE * vm_Run    (VM *vm);
LIBIMPORT void    vm_Reset  (VM *vm);
LIBIMPORT int     vm_GetLen (VM *vm);
LIBIMPORT void 		vm_Label	(VM *vm, char *name);
//
LIBIMPORT void vm_simule_push_long (long value);
//
LIBIMPORT void emit_begin (VM *vm);
LIBIMPORT void emit_end (VM *vm);
LIBIMPORT void emit_halt (VM *vm);
//
LIBIMPORT void emit_push_long (VM *vm, long value);
LIBIMPORT void emit_push_float (VM *vm, float value);
LIBIMPORT void emit_push_float (VM *vm, float value);
LIBIMPORT void emit_push_var (VM *vm, UCHAR i);
LIBIMPORT void emit_pop_var (VM *vm, UCHAR i);
LIBIMPORT void emit_pop_local (VM *vm, UCHAR i);
//
LIBIMPORT void emit_inc_var_long 		(VM *vm, UCHAR c);
LIBIMPORT void emit_inc_local_long	(VM *vm, UCHAR c);
LIBIMPORT void emit_dec_var_long		(VM *vm, UCHAR c);
LIBIMPORT void emit_dec_local_long	(VM *vm, UCHAR c);
//
LIBIMPORT void emit_mul_long (VM *vm);
LIBIMPORT void emit_div_long (VM *vm);
LIBIMPORT void emit_add_long (VM *vm);
LIBIMPORT void emit_sub_long (VM *vm);
//
LIBIMPORT void emit_mul_float (VM *vm);
LIBIMPORT void emit_div_float (VM *vm);
LIBIMPORT void emit_add_float (VM *vm);
LIBIMPORT void emit_sub_float (VM *vm);
//
LIBIMPORT void emit_pop_eax (VM *vm);
LIBIMPORT void emit_print_eax (VM *vm, UCHAR type);
LIBIMPORT void emit_mov_eax_var (VM *vm, UCHAR index);
LIBIMPORT void emit_push_string (VM *vm, char *s);
LIBIMPORT void emit_push_arg (VM *vm, UCHAR i);
LIBIMPORT void emit_push_local (VM *vm, UCHAR i);
//
LIBIMPORT void emit_cmp_long (VM *vm);
LIBIMPORT void emit_jump_jmp (VM *vm, char *name);
LIBIMPORT void emit_jump_je (VM *vm, char *name);
LIBIMPORT void emit_jump_jne (VM *vm, char *name);
LIBIMPORT void emit_jump_jle (VM *vm, char *name);
LIBIMPORT void emit_jump_jge (VM *vm, char *name);
LIBIMPORT void emit_jump_jl (VM *vm, char *name);
LIBIMPORT void emit_jump_jg (VM *vm, char *name);

//
LIBIMPORT void emit_call (VM *vm, void *func, UCHAR arg_count, UCHAR return_type);
LIBIMPORT void emit_call_vm (VM *vm, void *func, UCHAR arg_count, UCHAR return_type);

#ifdef __cplusplus
}
#endif
#endif // ! _VM_H_

