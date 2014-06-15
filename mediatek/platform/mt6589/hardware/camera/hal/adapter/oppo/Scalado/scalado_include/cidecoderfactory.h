/*  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
 .                                                                             .
 .    (c) 2000-2010 Scalado AB. All rights reserved.                           .
 .                                                                             .
 .    Technologies used in this source code are patented or patent pending     .
 .    by Scalado AB Swedish Org. Number, 556594-6885.                          .
 .                                                                             .
 .    All Intellectual Property Rights related to this source code,            .
 .    belongs to Scalado AB.                                                   .
 .                                                                             .
 .    This source code is furnished under license agreement and may be used    .
 .    or copied only in accordance with terms of such license agreement.       .
 .                                                                             .
 .    Except as permitted by such license agreement, no part of this source    .
 .    code may be reproduced, stored in a retrieval system, or transmitted,    .
 .    in any form or by any means, electronic, mechanical, recording, or       .
 .    otherwise, without the prior written permission of Scalado.              .
 .                                                                             .
 .    Scalado assumes no responsibility or liability for any errors or         .
 .    inaccuracies in this source code or any consequential, incidental or     .
 .    indirect damage arising out of the use of this source code.              .
 .                                                                             .
 .    Scalado and the Scalado logo are either registered trademarks or         .
 .    trademarks of Scalado AB in Sweden and/or other countries.               .
 .                                                                             .
 . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/

#ifndef CIDECODERFACTORY_H
#define CIDECODERFACTORY_H

#include "scbdefs.h"
#include "ctstream.h"
#include "ctdecoder.h"
#include "ctdecoderfactory.h"

#ifdef __cplusplus
extern "C" {
#endif

//==============================================================================
/**
    @file cidecoderfactory.h
    @brief This header file describes the interface of a CTDecoderFactory to
    make it possible to create new CTDecoderFactory objects.
    @ingroup cidecoderfactory
*/
//==============================================================================

/**
    @addtogroup cidecoderfactory
    @brief The CIDecoderFactory allows an implementation to detect image
    type based on a stream, and to create a decoder suitable to parse that
    stream.

    @{
*/
//------------------------------------------------------------------------------

/**
    @interface CIDecoderFactory
    @brief The purpose of the decoder factory is to create a suitable decoder for a 
    certain image format and implementation.
*/
typedef struct
{
    /**
        @brief
        Determines, by a simple check, whether the decoder factory can create 
        a decoder suitable for parsing (and possible decoding) the input stream.

        @param pFactory
        A valid pointer to a decoder factory.

        @param pStream 
        Input stream containing the image format to parse.

        @param[out] pCanParse 
        Boolean to indicate suitability of this factory to create a decoder to 
        parse the stream.

        @returns
        @c CERR_OK to indicate success, or a suitable error code if an error 
        occurred.

        @remarks
        The input stream position must not be altered by this operation.

        @remarks
        This operation is optional and may not be supported by this particular
        decoder factory.

        @remarks
        Many factory implementations are purely static, ie. they do not require
        a running context to operate. Thus, a factory implementing this 
        interface is required to make a best effort to handle a null pContext
        passed to it.

    */
    CRESULT (*canParse)( CTDecoderFactory *pFactory, 
                         CTStream *pStream, 
                         CTbool *pCanParse );

    /**
        @brief
        Creates a decoder attached to the provided stream.
        
        After a call to this function, the decoder is considered to be owning 
        the stream until the decoder is destroyed. Decoder destruction will 
        not automatically release resources associated with the stream, and 
        this will remain the callers responsibility.

        @param pFactory
        A valid pointer to a decoder factory.

        @param pStream 
        Input stream to connect to the decoder.

        @param[out] ppDecoder
        The created decoder object.

        @returns
        @c CERR_OK if successful or a suitable error code if an error occurred.

        @remarks
        Many factory implementations are purely static, ie. they do not require
        a running context to operate. Thus, a factory implementing this 
        interface is required to make a best effort to handle a null pContext
        passed to it.

    */
    CRESULT (*createDecoder)( CTDecoderFactory *pFactory, 
                              CTStream *pStream, 
                              CTDecoder **ppDecoder );

    /**
        @brief
        Releases internal allocations in the context, but not the memory 
        allocated for the context itself.

        @param pFactory
        A valid pointer to a decoder factory.
    */
    void (*deinit)( CTDecoderFactory *pFactory );
} CIDecoderFactory;

//------------------------------------------------------------------------------

/**
    @brief Creates an instance of a decoder factory.

    @param pInterface A valid implementation of the CIDecoderFactory interface.
    @param sizeOfContext Size of the context to be created.
    @param[out] ppFactory Pointer to a reference to the newly created factory.

    @returns @c CERR_OK if successful or an error code on failure.
    
    @since 3.8.2 R2
*/
SCBIMPORT CRESULT
ctdecoderfactory_create( const CIDecoderFactory *pInterface,
                         CTuint32 sizeOfContext, 
                         CTDecoderFactory **ppFactory );

/**
    @brief
    Retrieves the decoder factory context allocated in ctdecoderfactory_create().
    
    @param pFactory A pointer to a valid reference of an decoder factory.
    
    @return The interface context for the encoder object, or NULL if the
    encoder is invalid.
    
    @since 3.8.2 R2
*/
SCBIMPORT void *ctdecoderfactory_context( CTDecoderFactory *pFactory );

//==============================================================================
/** @} */
// end addtogroup cidecoderfactory

#ifdef __cplusplus
}           // extern "C"
#endif      // __cplusplus
#endif // CIDECODERFACTORY_H
