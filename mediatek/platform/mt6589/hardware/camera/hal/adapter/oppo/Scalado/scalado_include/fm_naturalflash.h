/*  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
.                                                                             .
.    (c) 2000-2011 Scalado AB. All rights reserved.                           .
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
. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

#ifndef FM_NATURALFLASH_H
#define FM_NATURALFLASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "caps_base.h"
#include "caps_types.h"

//=============================================================================
/**
    @file fm_naturalflash.h
    @brief Defines the interface of the Caps Feature Module: @a NaturalFlash
    @ingroup fm_naturalflash
*/
//=============================================================================
/**
    @addtogroup fm_naturalflash
    @brief Creation of Natural Flash images.

    The CTNaturalFlash API provides functionality for creating natural-looking images, given two images depicting the
    same scene. One image should be taken without flash to capture natural colors and one taken with flash to capture
    darker areas. The camera should be as stationary as possible in between shots.

    <h3>Usage</h3>
    -# Create a new Natural Flash object with ctnaturalflash_create()
    -# Add two images to the object using ctnaturalflash_addSession() or ctnaturalflash_addImage()
    -# Render a preview to display on screen using ctnaturalflash_generatePreview(),
       @n
       render the result to a stream with ctnaturalflash_renderToStream()
       @n
       or render the result to an Image using ctnaturalflash_renderToImage()
    -# Destroy the Natural Flash object, ctnaturalflash_destroy()
    
    @{
*/

//-----------------------------------------------------------------------------

/**
    @brief Type representing a Natural Flash object.
*/
typedef struct CTNaturalFlash CTNaturalFlash;

//-----------------------------------------------------------------------------

/**
    @brief
    Creates a CTNaturalFlash object.

    @param[out] ppNaturalFlash
    Pointer to the created CTNaturalFlash object if the function returns CERR_OK.

    @return CERR_OK on success.

    @see
    ctnaturalflash_destroy()

    @since 5.3
*/
SCBIMPORT CRESULT ctnaturalflash_create(CTNaturalFlash **ppNaturalFlash);

//-----------------------------------------------------------------------------

/**
    @brief
    Adds an image, provided via a CTSession, to the CTNaturalFlash object.

    Two images that the created image will be based on must be added before calling ctnaturalflash_renderToStream(),
    ctnaturalflash_renderToImage() or ctnaturalflash_generatePreview(). If more than two images are added, only the
    first two will be used.

    Any added session must not be modified until after the CTNaturalFlash object has been destroyed.

    @param pNaturalFlash
    A valid CTNaturalFlash object.

    @param session
    A session providing the image to add.

    @return CERR_OK on success.

    @see
    - ctnaturalflash_generatePreview()
    - ctnaturalflash_renderToStream()
    - ctnaturalflash_renderToImage()

    @since 5.3
*/
SCBIMPORT CRESULT ctnaturalflash_addSession(CTNaturalFlash *pNaturalFlash, CTSession session);

//-----------------------------------------------------------------------------

/**
    @brief
    Adds an image, provided via a CTImage, to the CTNaturalFlash object. This is a less general way of adding images
    and only very few color formats are supported (CM_YUV420P). If other formats are used please refer to the more
    general ctnaturalflash_addSession().

    Two images that the created image will be based on must be added before calling ctnaturalflash_renderToStream(),
    ctnaturalflash_renderToImage() or ctnaturalflash_generatePreview(). If more than two images are added, only the
    first two will be used.

    Any added images must not be modified or destroyed until after the CTNaturalFlash object
    has been destroyed.

    @param pNaturalFlash
    A valid CTNaturalFlash object.

    @param pImage
    An image to add.

    @return CERR_OK on success.

    @see
    - ctnaturalflash_generatePreview()
    - ctnaturalflash_renderToStream()
    - ctnaturalflash_renderToImage()
    .

    @since 5.3
*/
SCBIMPORT CRESULT ctnaturalflash_addImage(CTNaturalFlash *pNaturalFlash, CTBitmap *pImage);

//-----------------------------------------------------------------------------

/**
     @brief
     Returns the dimensions of the output of the next call to ctnaturalflash_renderToStream() or
     ctnaturalflash_renderToImage().

     The two images that the created image should be based on must be added before calling
     ctnaturalflash_getRenderSize().

     @param pNaturalFlash
     A valid CTNaturalFlash object.

     @param[out] pSize
     A valid CTSize object. The dimensions of the next rendered image will be inserted in the provided CTSize object.

     @return CERR_OK on success.

     @since 5.3
 */
SCBIMPORT CRESULT ctnaturalflash_getRenderSize(CTNaturalFlash *pNaturalFlash, CTSize *pSize);

//-----------------------------------------------------------------------------

/**
    @brief
    Creates an image from two images, and renders it to the provided image.

    Two images, one taken with flash and one without flash, must have been added with ctnaturalflash_addSession() or
    ctnaturalflash_addImage() before an image can be created.

    The size of the generated image will correspond to the size of the intersection of the added images after
    registration. This size must be fetched with ctnaturalflash_getRenderSize().

    The result will be rendered to the color format described by the CTImage. For highest performance YUV420P must be
    used.

    @param pNaturalFlash
    A valid CTNaturalFlash object.

    @param[in,out] pOutputImage
    The output image to which the generated image will be rendered.

    @return CERR_OK on success.

    @see
    - ctnaturalflash_addImage()
    - ctnaturalflash_addSession()
    - ctnaturalflash_getRenderSize()
    - ctnaturalflash_generatePreview()

    @since 5.3
*/
SCBIMPORT CRESULT ctnaturalflash_renderToImage(CTNaturalFlash *pNaturalFlash, CTBitmap *pOutputImage);

//-----------------------------------------------------------------------------

/**
    @brief
    Creates an image from two images, and renders it to the provided stream.

    Two images, one taken with flash and one without flash, must have been added with ctnaturalflash_addSession() or
    ctnaturalflash_addImage() before an image can be created.

    The size of the generated image will correspond to the size of the intersection of the added images after
    registration.

    @param pNaturalFlash
    A valid CTNaturalFlash object.

    @param[in,out] pOutputStream
    An output stream to which the generated image will be written.
    The rendered image will be encoded as a Jpeg.

    @return CERR_OK on success.

    @see
    - ctnaturalflash_addImage()
    - ctnaturalflash_addSession()
    - ctnaturalflash_generatePreview()

    @since 5.3
*/
SCBIMPORT CRESULT ctnaturalflash_renderToStream(CTNaturalFlash *pNaturalFlash, CTStream *pOutputStream);

//-----------------------------------------------------------------------------

/**
    @brief
    Creates an image from two images, and renders it to the provided image.

    Two images, one taken with flash and one without flash, must have been added with ctnaturalflash_addSession() or
    ctnaturalflash_addImage() before an image can be created.

    The generated image will be scaled to fit the dimensions specified in the provided CTImage buffer; i.e. the
    resolution given by CTImage::dims.

    The result will be rendered to the color format described by the CTImage. For highest performance YUV420P must be
    used.

    @param pNaturalFlash
    A valid CTNaturalFlash object.

    @param[in,out] pPreviewImage
    An image buffer that the generated image will be written to.

    @return CERR_OK on success.
    
    @see
    - ctnaturalflash_addImage()
    - ctnaturalflash_addSession()

    @since 5.3
*/
SCBIMPORT CRESULT ctnaturalflash_generatePreview(CTNaturalFlash *pNaturalFlash, CTBitmap *pPreviewImage);

//-----------------------------------------------------------------------------

/**
    @brief
    Destroys a CTNaturalFlash object.

    @param pNaturalFlash
    The CTNaturalFlash object to destroy.

    @since 5.3
*/
SCBIMPORT void ctnaturalflash_destroy(CTNaturalFlash *pNaturalFlash);

//-----------------------------------------------------------------------------
/** @} */
// end 'fm_naturalflash'
//==============================================================================


#ifdef __cplusplus
}//extern "C" {
#endif

#endif // FM_NATURALFLASH_H
