/*******************************************************************************
 * Copyright (c) 2011, Jean-David Gadina - www.xs-labs.com
 * Distributed under the Boost Software License, Version 1.0.
 * 
 * Boost Software License - Version 1.0 - August 17th, 2003
 * 
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 * 
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 ******************************************************************************/
 
/* $Id$ */

/*!
 * @file        dict.c
 * @copyright   (c) 2011 - Jean-David Gadina - www.xs-labs.com
 * @abstract    ...
 */

#include "lzwm.h"

/*!
 * 
 */
lzwm_dict * lzwm_create_dict( void )
{
    unsigned int  i;
    lzwm_dict   * dict;
    
    if( NULL == ( dict = ( lzwm_dict * )malloc( sizeof( lzwm_dict ) ) ) )
    {
        return NULL;
    }
    
    if( NULL == ( dict->codes = ( lzwm_code * )calloc( sizeof( lzwm_code ), LZWM_DICT_ALLOC ) ) )
    {
        free( dict );
        
        return NULL;
    }
    
    dict->count  = 256;
    dict->length = LZWM_DICT_ALLOC;
    
    for( i = 0; i < 256; i++ )
    {
        dict->codes[ i ].code      = ( uint16_t )i;
        dict->codes[ i ].data[ 0 ] = ( unsigned char )i;
        dict->codes[ i ].length    = 1;
    }
    
    return dict;
}

/*!
 * 
 */
lzwm_code * lzwm_add_dict_entry( lzwm_dict * dict, unsigned char * data, unsigned int length )
{
    lzwm_code  * code;
    lzwm_code  * new_code;
    unsigned int i;
    
    code     = &( dict->codes[ data[ 0 ] ] );
    new_code = NULL;
    
    if( length == 1 )
    {
        return code;
    }
    
    if( dict->count == LZWM_DICT_ALLOC )
    {
        return NULL;
    }
    
    for( i = 1; i < length; i++ )
    {
        if( code->children[ data[ i ] ] == NULL )
        {
            new_code                    = &( dict->codes[ dict->count ] );
            new_code->length            = i + 1;
            new_code->code              = ( uint16_t )dict->count;
            code->children[ data[ i ] ] = new_code;
            
            memcpy( new_code->data, data, i + 1 );
            
            dict->count++;
            
            code = new_code;
        }
        else
        {
            code = code->children[ data[ i ] ];
        }
    }
    
    return code;
}

/*!
 * 
 */
lzwm_code * lzwm_find_dict_entry( lzwm_dict * dict, unsigned char * data, unsigned int length )
{
    unsigned int  i;
    lzwm_code *   code;
    
    code = &( dict->codes[ data[ 0 ] ] );
    
    for( i = 1; i < length; i++ )
    {
        if( code->children[ data[ i ] ] != NULL )
        {
            code = code->children[ data[ i ] ];
        }
        else
        {
            return NULL;
        }
    }
    
    return code;
}

/*!
 * 
 */
void lzwm_delete_dict( lzwm_dict * dict )
{
    free( dict->codes );
    free( dict );
}
