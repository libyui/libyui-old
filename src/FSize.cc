/*
  Copyright (C) 2000-2012 Novell, Inc
  This library is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation; either version 2.1 of the
  License, or (at your option) version 3.0 of the License. This library
  is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
  License for more details. You should have received a copy of the GNU
  Lesser General Public License along with this library; if not, write
  to the Free Software Foundation, Inc., 51 Franklin Street, Fifth
  Floor, Boston, MA 02110-1301 USA
*/


/*-/

  File:       FSize.cc

  Author:     Michael Andres <ma@suse.de>
  Maintainer: Michael Andres <ma@suse.de>

  Purpose:

/-*/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "FSize.h"

FSize::FSize( const std::string &sizeStr, const Unit unit_r )
  : _size( atoll( sizeStr.c_str() ) * factor( unit_r ) )
{
}

//
//
//	METHOD NAME : FSize::fillBlock
//	METHOD TYPE : FSize &
//
//	DESCRIPTION :
//
FSize & FSize::fillBlock( FSize blocksize_r )
{
  if ( _size && blocksize_r ) {
    long long diff = _size % blocksize_r;
    if ( diff ) {
      if ( _size > 0 )
	_size += blocksize_r;
      _size -= diff;
    }
  }
  return *this;
}

//
//
//	METHOD NAME : FSize::bestUnit
//	METHOD TYPE : FSize::Unit
//
//	DESCRIPTION :
//
FSize::Unit FSize::bestUnit() const
{
  long long usize( _size < 0 ? -_size : _size );
  if ( usize < KB )
    return B;
  if ( usize < MB )
    return K;
  if ( usize < GB )
    return M;
  if ( usize < TB )
    return G;
  return T;
}

//
//
//	METHOD NAME : FSize::form
//	METHOD TYPE : std::string
//
//	DESCRIPTION :
//
std::string FSize::form( const Unit unit_r, unsigned fw, unsigned prec, const bool showunit ) const
{
  if ( prec == bestPrec ) {
    switch ( unit_r )
    {
      case T:  prec = 3; break;
      case G:  prec = 2; break;
      case M:  prec = 1; break;
      case K:  prec = 1; break;
      case B:  prec = 0; break;
    }
  } else if ( unit_r == B )
    prec = 0; // doesn't make sense for Byte

  char buffer[80]; // should be long enough for any numeric sprintf()
  snprintf( buffer, sizeof( buffer ),
	    "%*.*f",
	    fw, prec, ( double( _size ) / factor( unit_r ) ) );

  std::string ret( buffer );

  if ( showunit )
    ret += std::string(" ") + unit( unit_r );

  return ret;
}


//
//
//	METHOD NAME : FSize::asString
//	METHOD TYPE : std::string
//
//	DESCRIPTION :
//
std::string FSize::asString() const
{
  return form();
}
