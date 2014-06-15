/*  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
.                                                                             .
.    Copyright (c) 2000-2012 Scalado AB. All rights reserved.                 .
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

#ifndef CTJPEGDEC_H
#define CTJPEGDEC_H

#include "scbdefs.h"
#include "ctstream.h"
#include "ctbitmap.h"

#ifdef __cplusplus
extern "C" {
#endif

//==============================================================================
/**
    @file ctjpegdec.h
    @brief This header file defines the interface needed to access a ctjpegdec
    object.
    @ingroup ctjpegdec
*/
//==============================================================================

/**
    @addtogroup ctjpegdec
    @brief The ctjpegdec class defines an interface to access decoding
    of pixel data.

    @{
*/
//------------------------------------------------------------------------------

typedef struct CTJpegDec CTJpegDec;

/**
    Internal data type
*/
#ifndef SCBJPEG_HEADER_TYPE
#define SCBJPEG_HEADER_TYPE
    typedef struct scbjpeg_header scbjpeg_header;
#endif

/**
    @brief
    Creates a sequential jpeg decoder.

    @param[in] pInputBuffer
    entire JPEG stream stored as a buffer

    @param[out] ppDecoder
    Decoder

    @return
    @c CERR_OK or a specific error code on failure.

    @since 5.4
*/
SCBIMPORT CRESULT ctjpegdec_createFromStream(CTStream *pStream, CTJpegDec **ppDecoder);
SCBIMPORT CRESULT ctjpegdec_create(CTBuffer* pInputBuffer, CTJpegDec **ppDecoder);

/**
    @brief
    Destroys a sequential decoder instance.

    @param pDecoder A pointer to the decoder to be destroyed.

    @since 5.4
*/
SCBIMPORT void ctjpegdec_destroy(CTJpegDec *pDecoder);

/**
    @brief
    Parses a sequential decoder instance.

    @param pDecoder A pointer to the decoder to parse.

    @return
    @c CERR_OK or a specific error code on failure.

    @since 5.4
*/
SCBIMPORT CRESULT ctjpegdec_parse(CTJpegDec *pSeqDec);

/**
    @brief
    Prepares a sequential decoder instance.

    @param pDecoder A pointer to the decoder to prepare.

    @return
    @c CERR_OK or a specific error code on failure.

    @since 5.4
*/
SCBIMPORT CRESULT ctjpegdec_prepare(CTJpegDec *pSeqDec);

/**
    @brief
    Decodes the image into provided bitmap

    @param pDecoder A pointer to the decoder

    @param pBitmap A pointer to bitmap to decode image into.
                   Supported color formats are:
                   CM_AYUV4444
                   CM_ARGB8888
                   CM_RGBA8888
                   CM_BGRA8888
                   CM_RGB565
                   CM_RGB565D
                   CM_RGB565FD
                   CM_YUV420P (Only 2^n scale)

                   The decoded image will be fit into the size of the image as long
                   as the bitmap is smaller or the same size as the JPEG image. If the bitmap
                   is larger in any dimension than the JPEG image, the JPEG dimension will be used.

    @return
    @c CERR_OK or a specific error code on failure.

    @since 5.4
*/
SCBIMPORT CRESULT ctjpegdec_decode(CTJpegDec *pSeqDec, CTBitmap *pBitmap);

/**
    @brief
    Decodes the image into provided bitmap

    @param pDecoder A pointer to the decoder

    @return
    @c size of image or {0, 0} if error occurred.

    @since 5.4
*/
SCBIMPORT CTSize ctjpegdec_getSize(CTJpegDec *pSeqDec);

/**
    @brief
    Fetch pointer to header in the decoder. Header is still owned by decoder and 
    will be deallocated with the decoder

    @param pDecoder A pointer to the decoder

    @return
    @c Header or NULL on failure.
 
    @note This function is for internal use only, and the output type is on
    purpose not included with the public api.

    @since 5.4
*/
SCBIMPORT scbjpeg_header* ctjpegdec_getHeader(CTJpegDec*pSeqDec);

/**
    @brief
    Decodes the image into provided bitmap using multiple cores

    @param pDecoder A pointer to the decoder, only baseline images are supported

    @param pBitmap A pointer to bitmap to decode image into.
                   Supported color formats are:
                   CM_AYUV4444
                   CM_ARGB8888
                   CM_RGBA8888
                   CM_BGRA8888
                   CM_YUV420P (Only 2^n scale)

                   The decoded image will be fitted into the size of the image as long
                   as the bitmap is smaller or the same size as the JPEG image. If the bitmap
                   is larger in any dimension than the JPEG image, the JPEG dimension will be used.

    @return
    @c CERR_OK or a specific error code on failure.

    @since 5.4
*/
SCBIMPORT CRESULT ctjpegdec_decode_mc(CTJpegDec *pDecoder, CTBitmap *pBitmap);

//==============================================================================
/** @} */
// end addtogroup ctjpegdec


#ifdef __cplusplus
}           // extern "C"
#endif      // __cplusplus
#endif      // CTJPEGDEC_H
