/*  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
.                                                                             .
.    (c) 2000-2012 Scalado AB. All rights reserved.                           .
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

#ifndef CISTREAM_H
#define CISTREAM_H

#include "scbdefs.h"
#include "ctstream.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct CIStream CIStream;


//=============================================================================
/**
    @ingroup scb_ctstream_interface
    @file cistream.h
    @brief Header for the stream interface.
*/
//=============================================================================
/**
    @addtogroup scb_ctstream_interface
    @brief Low level stream interface
    
    The CIStream interface is the virtual low-level stream interface. If
    support is needed for other stream types than file/buffer
    (see  @ref scb_ctstream_file/@ref scb_ctstream_buffer), an
    implementation of this interface must be provided.

    For creation of file- and buffer-based streams, see:
    - @ref ctstream_createBufferStream()
    - @ref ctstream_createFileStream()
    - @ref ctstream_createFileStreamEx()
    .

    @{
*/
//-----------------------------------------------------------------------------
/**
    @brief
    Creates a @c CTStream object.

    Creates a @c CTStream object using the provided interface.

    @param pInterface Pointer to an stream interface. @sa CIStream.
    @param pContext Pointer to user data related to one specific stream.
    @param[out] ppStream Pointer to a newly created stream.

    When the stream object (returned by this function) is no longer needed, it
    should be destroyed using: ctstream_destroy()

    @return A @c CRESULT result code.

    @since 3.8
*/
SCBIMPORT CRESULT ctstream_create(const CIStream *pInterface,
                                  void *pContext,
                                  CTStream **ppStream);
//-----------------------------------------------------------------------------
/**
    @interface CIStream
    @brief Low level stream interface.

    This interface provides a mean to extend stream handling to generic types.

    The CIStream interface is the virtual low-level stream interface. To add
    support for a new stream type, an implementation of this interface must be
    provided. It is the responsibility of the provider of this implementation
    to assure that each function executes the @em proper operation on the
    underlying data (stream); i.e. @em proper meaning that it exactly match
    the description/behavior specified below.

    @since 3.8
*/
struct CIStream
{
    /**
        @brief Advances the stream position.

        @param pContext Pointer to user specific data for this stream.
        @param numbytes Number of bytes to skip. A stream implementation must
        correctly handle attempts to skip past the end of the stream data.

        @return A @c CRESULT result code. Attempting to skip past the end of
        stream data is valid and results in skipping to the end of stream data.
    */
    CRESULT (*skip)(void *pContext, CTuint32 numBytes);
    //.........................................................................
    /**
        @brief Seeks to a determined offset in the stream.
     
        This function is optional.

        @param pContext Pointer to user specific data for this stream.
        @param offset Global offset where to move the stream position.
       
        @return A @c CRESULT result code. Seeking past the end of stream data
        is valid and results in seeking to the end of stream data.
    */
    CRESULT (*seek)(void *pContext, CTuint32 offset);
    //.........................................................................
    /**
        @brief Destroys the underlying stream.

        @param pContext Pointer to user specific data for this stream.
    */
    void (*destroy)(void *pContext);
    //.........................................................................
    /**
        @brief
        Reads data from the stream and moves the stream position forwards.
     
        @param pContext Stream user data
        @param bytes Number of bytes to read
        @param pData Buffer where the bytes are stored. This buffer must be at
        least bytes large, even if that many bytes are not available in the
        stream.
        @param pReadBytes Actual number of bytes read from the stream.
     
        @return CERR_OK or an error code, typically CERR_STREAM_END_OF_DATA.
     
        @note pData is guaranteed to contain the requested amount of data when
        CERR_OK is returned. If CERR_STREAM_END_OF_DATA is returned, pData does
        not contain the entire request, or maybe even zero valid bytes.
    */
    CRESULT (*read)(void *pContext, CTuint32 bytes, CTuint8 *pData, CTuint32 *pReadBytes);
    //.........................................................................
    /**
        @brief
        Returns a pointer to an underlying data representation of the stream
        contents without advancing the stream position.

        @param pContext Stream user data
        @param bytes Number of bytes to peek
        @param ppData Pointer to (or a copy of) the stream contents.
        @param pReadBytes Actual number of bytes read from the stream.
     
        @return CERR_OK or an error code, typically CERR_STREAM_END_OF_DATA.
     
        @note *pData is guaranteed to contain the requested amount of data when
        CERR_OK is returned. If CERR_STREAM_END_OF_DATA is returned, *pData does
        not contain the entire request. If *pReadBytes is set to zero, *ppData is
        set to null.
        
        @note *ppData remains valid until the next call to a stream function. The
        caller is not responsible for cleaning up *ppData.
    */
    CRESULT (*peek)(void *pContext, CTuint32 bytes, CTuint8 **ppData, CTuint32 *pReadBytes);
    //.........................................................................
    /**
        @brief
        Writes bytes to the underlying stream implementation.
     
        @param pContext Stream user data
        @param bytes Number of bytes to write
        @param pData Buffer of bytes to write to the stream.
        @param pWrittenBytes[out] Actual number of bytes written.
     
        @return CERR_OK if successful. In this case bytes == pWrittenBytes.
    */
    CRESULT (*write)(void *pContext, CTuint32 bytes, CTuint8 *pData, CTuint32 *pWrittenBytes);
    //.........................................................................
    /**
        @brief
        Tells the underlying stream implementation to flush all written output.

        @param pContext Stream user data

        @return CERR_OK if successful, or an error code on failure.
    */
    CRESULT (*flush)(void *pContext);
};
// end of 'struct CIStream'
//-----------------------------------------------------------------------------
/**
    @}
*/ // end of addtogroup 'scb_ctstream_interface'
//-----------------------------------------------------------------------------

#ifdef __cplusplus
} // extern "C"
#endif

#endif
