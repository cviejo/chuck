/*----------------------------------------------------------------------------
  ChucK Strongly-timed Audio Programming Language
    Compiler, Virtual Machine, and Synthesis Engine

  Copyright (c) 2003 Ge Wang and Perry R. Cook. All rights reserved.
    http://chuck.stanford.edu/
    http://chuck.cs.princeton.edu/

  This program is free software; you can redistribute it and/or modify
  it under the dual-license terms of EITHER the MIT License OR the GNU
  General Public License (the latter as published by the Free Software
  Foundation; either version 2 of the License or, at your option, any
  later version).

  This program is distributed in the hope that it will be useful and/or
  interesting, but WITHOUT ANY WARRANTY; without even the implied warranty
  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  MIT Licence and/or the GNU General Public License for details.

  You should have received a copy of the MIT License and the GNU General
  Public License (GPL) along with this program; a copy of the GPL can also
  be obtained by writing to the Free Software Foundation, Inc., 59 Temple
  Place, Suite 330, Boston, MA 02111-1307 U.S.A.
-----------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------
// file: chuck_emit.h
// desc: chuck instruction emitter
//
// author: Ge Wang (ge@ccrma.stanford.edu | gewang@cs.princeton.edu)
// date: Autumn 2002 - first version
//       Autumn 2004 - redesign
//-----------------------------------------------------------------------------
#ifndef __CHUCK_EMIT_H__
#define __CHUCK_EMIT_H__

#include "chuck_def.h"
#include "chuck_oo.h"
#include "chuck_type.h"
#include "chuck_frame.h"


// forward references
struct Chuck_Instr;
struct Chuck_Instr_Goto;
struct Chuck_Instr_Stmt_Start;
struct Chuck_VM_Code;
struct Chuck_VM_Shred;




//-----------------------------------------------------------------------------
// name: struct Chuck_Code
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Code
{
public:
    // name
    std::string name;
    // stack depth (in bytes)
    t_CKUINT stack_depth;
    // need this
    t_CKBOOL need_this;
    // is static (in class def) | 1.4.1.0 (ge) added
    t_CKBOOL is_static;
    // frame
    Chuck_Frame * frame;
    // code
    std::vector<Chuck_Instr *> code;

    // continue stack
    std::vector<Chuck_Instr_Goto *> stack_cont;
    // break stack
    std::vector<Chuck_Instr_Goto *> stack_break;
    // return stack
    std::vector<Chuck_Instr_Goto *> stack_return;

    // filename this code came from (added 1.3.0.0)
    std::string filename;

    // constructor
    Chuck_Code( )
    {
        stack_depth = 0;
        need_this = FALSE;
        is_static = FALSE; // 1.4.1.0 (ge) added
        frame = new Chuck_Frame;
    }

    // destructor
    ~Chuck_Code()
    {
        // clean up frame
        CK_SAFE_DELETE( frame );
        // TODO: what about contents of code?
        // TODO: what about contents of stack_*?
        // (these may also depend on success/error during emission)
    }
};




//-----------------------------------------------------------------------------
// name: struct Chuck_Emitter
// desc: ...
//-----------------------------------------------------------------------------
struct Chuck_Emitter : public Chuck_VM_Object
{
    // reference to the type checker environment
    Chuck_Env * env;

    // current code
    Chuck_Code * code;
    // current context
    Chuck_Context * context;
    // expression namespace
    Chuck_Namespace * nspc;
    // current function definition
    Chuck_Func * func;

    // locals
    std::vector<Chuck_Local *> locals;
    // code stack
    std::vector<Chuck_Code *> code_stack;
    // stmt stack
    std::vector<Chuck_Instr_Stmt_Start *> stmt_stack;

    // dump
    t_CKBOOL dump;

public:
    // constructor
    Chuck_Emitter()
    { env = NULL; code = NULL; context = NULL;
      nspc = NULL; func = NULL; dump = FALSE;
      should_replace_dac = FALSE; }

    // destructor
    ~Chuck_Emitter() { }

    // append instruction
    void append( Chuck_Instr * instr )
    { assert( code != NULL ); code->code.push_back( instr ); }
    // next instruction index
    t_CKUINT next_index()
    { assert( code != NULL ); return code->code.size(); }

    // push scope
    void push_scope( )
    { assert( code != NULL ); code->frame->push_scope(); }
    // alloc local (ge: added is_obj 2012 april | added 1.3.0.0)
    Chuck_Local * alloc_local( t_CKUINT size, const std::string & name,
        t_CKBOOL is_ref, t_CKBOOL is_obj, t_CKBOOL is_global )
    { assert( code != NULL ); return code->frame->alloc_local( size, name,
        is_ref, is_obj, is_global ); }
    // add references to locals on current scope (added 1.3.0.0)
    void addref_on_scope();
    // pop scope
    void pop_scope();
    // traverse scope on jumps such as 'return' 'break' 'continue' | 1.5.0.0 (ge)
    void traverse_scope_on_jump();

    // default durations
    t_CKBOOL find_dur( const std::string & name, t_CKDUR * out );

public:
    // post REFACTOR-2017: replace-dac
    std::string dac_replacement;
    t_CKBOOL should_replace_dac;

public:
    // codestr context (for instruction dump) | 1.5.0.8 (ge) added
    std::vector<std::string> codestr_context;
    // push codestr context
    void codestr_context_push( const std::string & prefix )
    { codestr_context.push_back(prefix); }
    // pop codestr context
    void codestr_context_pop()
    { codestr_context.pop_back(); }
    // get current codestr context
    std::string codestr_context_top() const
    { return codestr_context.size() > 0 ? codestr_context.back() : ""; }
};




// allocate the emitter
Chuck_Emitter * emit_engine_init( Chuck_Env * env );
// shutdown and free the emitter
t_CKBOOL emit_engine_shutdown( Chuck_Emitter *& emit );
// emit a program into vm code
Chuck_VM_Code * emit_engine_emit_prog( Chuck_Emitter * emit,
                                       a_Program prog,
                                       te_HowMuch how_much );
// helper function to emit code
Chuck_VM_Code * emit_to_code( Chuck_Code * in,
                              Chuck_VM_Code * out = NULL,
                              t_CKBOOL dump = FALSE );

// NOT USED: ...
t_CKBOOL emit_engine_addr_map( Chuck_Emitter * emit, Chuck_VM_Shred * shred );
t_CKBOOL emit_engine_resolve( );




#endif
