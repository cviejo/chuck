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
// file: chuck_otf.h
// desc: on-the-fly programming utilities
//
// author: Ge Wang (ge@ccrma.stanford.edu | gewang@cs.princeton.edu)
// date: Autumn 2004
//-----------------------------------------------------------------------------
#ifndef __CHUCK_OTF_H__
#define __CHUCK_OTF_H__

#include "chuck_def.h"
#include "util_network.h"
#include <memory.h>


// defines
#define CK_NET_HEADER      0x8c8cc8c8
// buffer size
#define CK_NET_BUFFER_SIZE 512
// error value
#define CK_NET_ERROR       0xffffffff
// forward
struct Chuck_VM;
struct Chuck_Compiler;


//-----------------------------------------------------------------------------
// name: struct OTF_Net_Msg()
// desc: a network message for on-the-fly programming
//-----------------------------------------------------------------------------
struct OTF_Net_Msg
{
    t_CKUINT header;
    t_CKUINT type;
    t_CKUINT param;
    t_CKUINT param2;
    t_CKUINT param3;
    t_CKUINT length;
    char buffer[CK_NET_BUFFER_SIZE];

    OTF_Net_Msg() { this->clear(); }
    void clear() { header = CK_NET_HEADER; type = param = param2 = param3 = length = 0;
                   memset( buffer, 0, sizeof(buffer) ); }

    // copy constructor | 1.5.1.5
    OTF_Net_Msg( const OTF_Net_Msg & rhs )
    {
        header = rhs.header;
        type = rhs.type;
        param = rhs.param;
        param2 = rhs.param2;
        param3 = rhs.param3;
        length = rhs.length;
        memcpy( buffer, rhs.buffer, sizeof(rhs.buffer) );
    }
};


// host to network
void otf_hton( OTF_Net_Msg * msg );
// network to host
void otf_ntoh( OTF_Net_Msg * msg );

// process incoming message
t_CKUINT otf_process_msg( Chuck_VM * vm, Chuck_Compiler * compiler,
                          OTF_Net_Msg * msg, t_CKBOOL immediate, void * data );

// send command
t_CKINT otf_send_cmd( t_CKINT argc, const char ** argv, t_CKINT & i, const char * host, t_CKINT port, t_CKINT * is_otf = NULL );
// send file to remote host
t_CKINT otf_send_file( const char * filename, OTF_Net_Msg & msg, const char * op, ck_socket sock );
// connect
ck_socket otf_send_connect( const char * host, t_CKINT port );

// callback
void * otf_recv_cb( void * p );

// --poop
void uh();

// yes
extern const char * poop[];
extern long poop_size;
extern t_CKUINT g_otf_log;




#endif
