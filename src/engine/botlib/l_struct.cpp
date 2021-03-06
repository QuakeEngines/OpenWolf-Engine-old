////////////////////////////////////////////////////////////////////////////////////////
// Copyright(C) 1999 - 2010 id Software LLC, a ZeniMax Media company.
// Copyright(C) 2011 - 2018 Dusan Jocic <dusanjocic@msn.com>
//
// This file is part of the OpenWolf GPL Source Code.
// OpenWolf Source Code is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// OpenWolf Source Code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with OpenWolf Source Code.  If not, see <http://www.gnu.org/licenses/>.
//
// In addition, the OpenWolf Source Code is also subject to certain additional terms.
// You should have received a copy of these additional terms immediately following the
// terms and conditions of the GNU General Public License which accompanied the
// OpenWolf Source Code. If not, please request a copy in writing from id Software
// at the address below.
//
// If you have questions concerning this license or the applicable additional terms,
// you may contact in writing id Software LLC, c/o ZeniMax Media Inc.,
// Suite 120, Rockville, Maryland 20850 USA.
//
// -------------------------------------------------------------------------------------
// File name:   l_struct.cpp
// Version:     v1.00
// Created:
// Compilers:   Visual Studio 2015
// Description: structure reading / writing
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifdef BOTLIB
#ifdef DEDICATED
#include <null/null_precompiled.h>
#else
#include <OWLib/precompiled.h>
#endif
#endif //BOTLIB

#ifdef BSPC
//include files for usage in the BSP Converter
#include <bspc/qbsp.h>
#include <bspc/l_log.h>
#include <bspc/l_mem.h>
#include <botlib/l_precomp.h>
#include <botlib/l_struct.h>
#endif //BSPC

//===========================================================================
//
// Parameter:				-
// Returns:					-
// Changes Globals:		-
//===========================================================================
fielddef_t* FindField( fielddef_t* defs, UTF8* name )
{
    S32 i;
    
    for( i = 0; defs[i].name; i++ )
    {
        if( !strcmp( defs[i].name, name ) ) return &defs[i];
    } //end for
    return NULL;
} //end of the function FindField
//===========================================================================
//
// Parameter:				-
// Returns:					-
// Changes Globals:		-
//===========================================================================
bool ReadNumber( source_t* source, fielddef_t* fd, void* p )
{
    token_t token;
    S32 negative = false;
    S64 intval, intmin = 0, intmax = 0;
    F64 floatval;
    
    if( !PC_ExpectAnyToken( source, &token ) ) return 0;
    
    //check for minus sign
    if( token.type == TT_PUNCTUATION )
    {
        if( fd->type & FT_UNSIGNED )
        {
            SourceError( source, "expected unsigned value, found %s", token.string );
            return 0;
        } //end if
        //if not a minus sign
        if( strcmp( token.string, "-" ) )
        {
            SourceError( source, "unexpected punctuation %s", token.string );
            return 0;
        } //end if
        negative = true;
        //read the number
        if( !PC_ExpectAnyToken( source, &token ) ) return 0;
    } //end if
    //check if it is a number
    if( token.type != TT_NUMBER )
    {
        SourceError( source, "expected number, found %s", token.string );
        return 0;
    } //end if
    //check for a float value
    if( token.subtype & TT_FLOAT )
    {
        if( ( fd->type & FT_TYPE ) != FT_FLOAT )
        {
            SourceError( source, "unexpected float" );
            return 0;
        } //end if
        floatval = token.floatvalue;
        if( negative ) floatval = -floatval;
        if( fd->type & FT_BOUNDED )
        {
            if( floatval < fd->floatmin || floatval > fd->floatmax )
            {
                SourceError( source, "float out of range [%f, %f]", fd->floatmin, fd->floatmax );
                return 0;
            } //end if
        } //end if
        *( F32* ) p = ( F32 ) floatval;
        return 1;
    } //end if
    //
    intval = token.intvalue;
    if( negative ) intval = -intval;
    //check bounds
    if( ( fd->type & FT_TYPE ) == FT_CHAR )
    {
        if( fd->type & FT_UNSIGNED )
        {
            intmin = 0;
            intmax = 255;
        }
        else
        {
            intmin = -128;
            intmax = 127;
        }
    } //end if
    if( ( fd->type & FT_TYPE ) == FT_INT )
    {
        if( fd->type & FT_UNSIGNED )
        {
            intmin = 0;
            intmax = 65535;
        }
        else
        {
            intmin = -32768;
            intmax = 32767;
        }
    } //end else if
    if( ( fd->type & FT_TYPE ) == FT_CHAR || ( fd->type & FT_TYPE ) == FT_INT )
    {
        if( fd->type & FT_BOUNDED )
        {
            intmin = Maximum( intmin, fd->floatmin );
            intmax = Minimum( intmax, fd->floatmax );
        } //end if
        if( intval < intmin || intval > intmax )
        {
            SourceError( source, "value %d out of range [%d, %d]", intval, intmin, intmax );
            return 0;
        } //end if
    } //end if
    else if( ( fd->type & FT_TYPE ) == FT_FLOAT )
    {
        if( fd->type & FT_BOUNDED )
        {
            if( intval < fd->floatmin || intval > fd->floatmax )
            {
                SourceError( source, "value %d out of range [%f, %f]", intval, fd->floatmin, fd->floatmax );
                return 0;
            } //end if
        } //end if
    } //end else if
    //store the value
    if( ( fd->type & FT_TYPE ) == FT_CHAR )
    {
        if( fd->type & FT_UNSIGNED ) *( U8* ) p = ( U8 ) intval;
        else *( UTF8* ) p = ( UTF8 ) intval;
    } //end if
    else if( ( fd->type & FT_TYPE ) == FT_INT )
    {
        if( fd->type & FT_UNSIGNED ) *( U32* ) p = ( U32 ) intval;
        else *( S32* ) p = ( S32 ) intval;
    } //end else
    else if( ( fd->type & FT_TYPE ) == FT_FLOAT )
    {
        *( F32* ) p = ( F32 ) intval;
    } //end else
    return 1;
} //end of the function ReadNumber
//===========================================================================
//
// Parameter:				-
// Returns:					-
// Changes Globals:		-
//===========================================================================
bool ReadChar( source_t* source, fielddef_t* fd, void* p )
{
    token_t token;
    
    if( !PC_ExpectAnyToken( source, &token ) ) return 0;
    
    //take literals into account
    if( token.type == TT_LITERAL )
    {
        StripSingleQuotes( token.string );
        *( UTF8* ) p = token.string[0];
    } //end if
    else
    {
        PC_UnreadLastToken( source );
        if( !ReadNumber( source, fd, p ) ) return 0;
    } //end if
    return 1;
} //end of the function ReadChar
//===========================================================================
//
// Parameter:				-
// Returns:					-
// Changes Globals:		-
//===========================================================================
S32 ReadString( source_t* source, fielddef_t* fd, void* p )
{
    token_t token;
    
    if( !PC_ExpectTokenType( source, TT_STRING, 0, &token ) ) return 0;
    //remove the double quotes
    StripDoubleQuotes( token.string );
    //copy the string
    strncpy( ( UTF8* ) p, token.string, MAX_STRINGFIELD );
    //make sure the string is closed with a zero
    ( ( UTF8* )p )[MAX_STRINGFIELD - 1] = '\0';
    //
    return 1;
} //end of the function ReadString
//===========================================================================
//
// Parameter:				-
// Returns:					-
// Changes Globals:		-
//===========================================================================
S32 ReadStructure( source_t* source, structdef_t* def, UTF8* structure )
{
    token_t token;
    fielddef_t* fd;
    void* p;
    S32 num;
    
    if( !PC_ExpectTokenString( source, "{" ) ) return 0;
    while( 1 )
    {
        if( !PC_ExpectAnyToken( source, &token ) ) return false;
        //if end of structure
        if( !strcmp( token.string, "}" ) ) break;
        //find the field with the name
        fd = FindField( def->fields, token.string );
        if( !fd )
        {
            SourceError( source, "unknown structure field %s", token.string );
            return false;
        } //end if
        if( fd->type & FT_ARRAY )
        {
            num = fd->maxarray;
            if( !PC_ExpectTokenString( source, "{" ) ) return false;
        } //end if
        else
        {
            num = 1;
        } //end else
        p = ( void* )( structure + fd->offset );
        while( num-- > 0 )
        {
            if( fd->type & FT_ARRAY )
            {
                if( PC_CheckTokenString( source, "}" ) ) break;
            } //end if
            switch( fd->type & FT_TYPE )
            {
                case FT_CHAR:
                {
                    if( !ReadChar( source, fd, p ) ) return false;
                    p = ( UTF8* ) p + sizeof( UTF8 );
                    break;
                } //end case
                case FT_INT:
                {
                    if( !ReadNumber( source, fd, p ) ) return false;
                    p = ( UTF8* ) p + sizeof( S32 );
                    break;
                } //end case
                case FT_FLOAT:
                {
                    if( !ReadNumber( source, fd, p ) ) return false;
                    p = ( UTF8* ) p + sizeof( F32 );
                    break;
                } //end case
                case FT_STRING:
                {
                    if( !ReadString( source, fd, p ) ) return false;
                    p = ( UTF8* ) p + MAX_STRINGFIELD;
                    break;
                } //end case
                case FT_STRUCT:
                {
                    if( !fd->substruct )
                    {
                        SourceError( source, "BUG: no sub structure defined" );
                        return false;
                    } //end if
                    ReadStructure( source, fd->substruct, ( UTF8* ) p );
                    p = ( UTF8* ) p + fd->substruct->size;
                    break;
                } //end case
            } //end switch
            if( fd->type & FT_ARRAY )
            {
                if( !PC_ExpectAnyToken( source, &token ) ) return false;
                if( !strcmp( token.string, "}" ) ) break;
                if( strcmp( token.string, "," ) )
                {
                    SourceError( source, "expected a comma, found %s", token.string );
                    return false;
                } //end if
            } //end if
        } //end while
    } //end while
    return true;
} //end of the function ReadStructure
//===========================================================================
//
// Parameter:				-
// Returns:					-
// Changes Globals:		-
//===========================================================================
S32 WriteIndent( FILE* fp, S32 indent )
{
    while( indent-- > 0 )
    {
        if( fprintf( fp, "\t" ) < 0 ) return false;
    } //end while
    return true;
} //end of the function WriteIndent
//===========================================================================
//
// Parameter:				-
// Returns:					-
// Changes Globals:		-
//===========================================================================
S32 WriteFloat( FILE* fp, F32 value )
{
    UTF8 buf[128];
    S32 l;
    
    sprintf( buf, "%f", value );
    l = strlen( buf );
    //strip any trailing zeros
    while( l-- > 1 )
    {
        if( buf[l] != '0' && buf[l] != '.' ) break;
        if( buf[l] == '.' )
        {
            buf[l] = 0;
            break;
        } //end if
        buf[l] = 0;
    } //end while
    //write the float to file
    if( fprintf( fp, "%s", buf ) < 0 ) return 0;
    return 1;
} //end of the function WriteFloat
//===========================================================================
//
// Parameter:				-
// Returns:					-
// Changes Globals:		-
//===========================================================================
S32 WriteStructWithIndent( FILE* fp, structdef_t* def, UTF8* structure, S32 indent )
{
    S32 i, num;
    void* p;
    fielddef_t* fd;
    
    if( !WriteIndent( fp, indent ) ) return false;
    if( fprintf( fp, "{\r\n" ) < 0 ) return false;
    
    indent++;
    for( i = 0; def->fields[i].name; i++ )
    {
        fd = &def->fields[i];
        if( !WriteIndent( fp, indent ) ) return false;
        if( fprintf( fp, "%s\t", fd->name ) < 0 ) return false;
        p = ( void* )( structure + fd->offset );
        if( fd->type & FT_ARRAY )
        {
            num = fd->maxarray;
            if( fprintf( fp, "{" ) < 0 ) return false;
        } //end if
        else
        {
            num = 1;
        } //end else
        while( num-- > 0 )
        {
            switch( fd->type & FT_TYPE )
            {
                case FT_CHAR:
                {
                    if( fprintf( fp, "%d", *( UTF8* ) p ) < 0 ) return false;
                    p = ( UTF8* ) p + sizeof( UTF8 );
                    break;
                } //end case
                case FT_INT:
                {
                    if( fprintf( fp, "%d", *( S32* ) p ) < 0 ) return false;
                    p = ( UTF8* ) p + sizeof( S32 );
                    break;
                } //end case
                case FT_FLOAT:
                {
                    if( !WriteFloat( fp, *( F32* )p ) ) return false;
                    p = ( UTF8* ) p + sizeof( F32 );
                    break;
                } //end case
                case FT_STRING:
                {
                    if( fprintf( fp, "\"%s\"", ( UTF8* ) p ) < 0 ) return false;
                    p = ( UTF8* ) p + MAX_STRINGFIELD;
                    break;
                } //end case
                case FT_STRUCT:
                {
                    if( !WriteStructWithIndent( fp, fd->substruct, structure, indent ) ) return false;
                    p = ( UTF8* ) p + fd->substruct->size;
                    break;
                } //end case
            } //end switch
            if( fd->type & FT_ARRAY )
            {
                if( num > 0 )
                {
                    if( fprintf( fp, "," ) < 0 ) return false;
                } //end if
                else
                {
                    if( fprintf( fp, "}" ) < 0 ) return false;
                } //end else
            } //end if
        } //end while
        if( fprintf( fp, "\r\n" ) < 0 ) return false;
    } //end for
    indent--;
    
    if( !WriteIndent( fp, indent ) ) return false;
    if( fprintf( fp, "}\r\n" ) < 0 ) return false;
    return true;
} //end of the function WriteStructWithIndent
//===========================================================================
//
// Parameter:				-
// Returns:					-
// Changes Globals:		-
//===========================================================================
S32 WriteStructure( FILE* fp, structdef_t* def, UTF8* structure )
{
    return WriteStructWithIndent( fp, def, structure, 0 );
} //end of the function WriteStructure

