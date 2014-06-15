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
. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

#ifndef CTHDR3_H
#define CTHDR3_H


#ifdef __cplusplus
extern "C" {
#endif

#include "scbdefs.h"
#include "ctbitmap.h"
#include "ctstream.h"

//=============================================================================
/**
    @file cthdr3.h
    @brief Defines the interface of @a CTHDR3
    @ingroup cthdr3
*/
//=============================================================================
/**
    @addtogroup cthdr3
    @brief Creation of High Dynamic Range images.
    
    The CTHDR3 API provides functionality for creating HDR (High Dynamic Range)
    images, given three images taken with different exposure settings, but
    depicting the same scene.

    For optimal result the images should be taken in such a way that
    all areas of the scene look good in at least one of the images.
    E.g. when taking a picture that has both indoors and outdoors areas (windows or
    similar) take one shot where the indoors look good and one where the outdoors
    looks good.

    If exposure intelligence is unavailable it is recommended to use frame average
    exposure and use a slightly overexposed and a slightly underexposed image.
    A simple rule of thumb is that increased exposure step between input images
    gives a more apparent HDR effect.


    <h3>Usage</h3>
    -# Create a new hdr3 object with cthdr3_create()
    -# Add three images to the hdr3 object using cthdr3_addLDRImage()
    -# Render a preview to display on screen using cthdr3_generatePreview(),
       @n
       render the result to a stream with cthdr3_renderToStream()
       @n
       or render the result to an Image using cthdr3_renderToImage()
    -# Destroy the hdr3 object, cthdr3_destroy()
    .
    
    @{
*/
//-----------------------------------------------------------------------------
/**
    @brief Type representing a hdr3 object.
*/
typedef struct CTHDR3 CTHDR3;

typedef struct CTHDRExtendedOptions
{
    /* Internal measure on how low frequency should be considered texture.
     * Low number will lead to only small feature will be considered as texture
     * large number will handle large features as texture.
     * Large number will minimize ghosts and lead to harder distribution of light
     * 0 < textureLevels < 10 */
    CTuint32 textureLevels;
    /* Five different light models are implemented:
     * 0 - Normal
     * 1 - Larger different between dark/light areas.
     * 2 - Halo reduction in light areas.
     * 3 - As 2 but stronger effect.
     * 4 - As 3 but stronger effect.
     *
     */
    CTuint32 lightModel;

}CTHDRExtendedOptions;



//-----------------------------------------------------------------------------
/**
    @brief
    Creates a CTHDR3 object.

    @param[out] ppHDR
    Pointer to the created CTHDR3 object if the function returns CERR_OK.

    @return CERR_OK on success.

    @see
    cthdr3_destroy()

    @since 5.4
*/
SCBIMPORT CRESULT cthdr3_create(CTHDR3 **ppHDR);



//-----------------------------------------------------------------------------

/**
    @brief
    Adds an image, provided via a CTBitmap, to the CTHDR3 object. This is a less general way of adding images and only
    very few color formats are supported (CM_YUV420P, CM_YUV420SP, CM_YVU420SP).

    The three images the created HDR image will be based on must be added before calling
    cthdr3_renderToStream(), cthdr3_renderToImage() or cthdr3_generatePreview().

    Any added images must not be modified or destroyed until after the CTHDR3 object
    has been destroyed.


    @param pHDR
    A valid CTHDR3 object.

    @param pLDRImage
    An image to add.

    @return CERR_OK on success.

    @see
    - cthdr3_generatePreview()

    - cthdr3_renderToStream()
    - cthdr3_renderToImage()
    .

    @since 5.4
*/
SCBIMPORT CRESULT cthdr3_addLDRImage(CTHDR3 *pHDR, CTBitmap *pLDRImage);

//-----------------------------------------------------------------------------
/**
     @brief
     Returns the dimensions of the output of the next call to cthdr3_renderToStream() or cthdr3_renderToImage().

     The images that the created HDR image should be based on must be added before calling cthdr3_getRenderSize().

     @param pHDR
     A valid CTHDR3 object.

     @param[out] pSize
     A valid CTSize object. The dimensions of the next rendered image will be inserted in the provided CTSize object.

     @return CERR_OK on success.

     @since 5.4
 */
SCBIMPORT CRESULT cthdr3_getRenderSize(CTHDR3 *pHDR, CTSize *pSize);

//-----------------------------------------------------------------------------
/**
    @brief
    Creates a HDR image from multiple images, and renders it to the provided image.

    The LDR (Low Dynamic Range) images must have been added with
    cthdr3_addLDRImage() before a HDR image can be created.

    The size of the generated HDR image will correspond to the size of the intersection
    of all the added LDR images after registration. This size must be fetched with cthdr3_getRenderSize().

    The image will be rendered to the color format described by the CTBitmap. For highest performance YUV420P, YUV420SP
    or YVU420SP must be used.

    @param pHDR
    A valid CTHDR3 object.

    @param[in,out] pOutputImage
    An output image to which the HDR image will be rendered.

    @return CERR_OK on success.

    @see
    - cthdr3_addLDRImage()
    - cthdr3_getRenderSize()
    - cthdr3_generatePreview()
    .

    @since 5.4
*/
SCBIMPORT CRESULT cthdr3_renderToImage(CTHDR3 *pHDR, CTBitmap *pOutputImage);

//-----------------------------------------------------------------------------
/**
    @brief
    Creates a HDR image from multiple images, and renders it to a provided stream.

    The LDR (Low Dynamic Range) images must have been added with
    cthdr3_addLDRImage() before a HDR image can be created.

    The size of the generated HDR image will correspond to the size of the intersection
    of all the added LDR images after registration.

    @param pHDR
    A valid CTHDR3 object.

    @param[in,out] pOutputStream
    An output stream to which the HDR image will be written.
    The rendered image will be encoded as a Jpeg.

    @return CERR_OK on success.

    @see
    - cthdr3_addLDRImage()
    - cthdr3_generatePreview()
    .

    @since 5.4
*/
SCBIMPORT CRESULT cthdr3_renderToStream(CTHDR3 *pHDR, CTStream *pOutputStream);

//-----------------------------------------------------------------------------
/**
    @brief
    Creates a HDR image from multiple images, and renders it to a provided
    @c CTBitmap.

    The LDR (Low Dynamic Range) images must have been added with
    cthdr3_addLDRImage() before a HDR image can be created.

    The HDR image will be scaled to fit the dimensions specified in the provided
    CTBitmap; i.e. the resolution given by CTBitmap::dims.

    The output color format is determined by that specified in CTBitmap::colorMode. For highest performance YUV420P,
    YUV420SP or YVU420SP must be used.
    
    @param pHDR
    A valid CTHDR3 object.

    @param[in,out] pPreviewImage
    An image buffer that the HDR image will be written to.

    @return CERR_OK on success.
    
    @see
    - cthdr3_addLDRImage()
    - cthdr3_renderToStream()
    .

    @since 5.4
*/
SCBIMPORT CRESULT cthdr3_generatePreview(CTHDR3 *pHDR, CTBitmap *pPreviewImage);

//-----------------------------------------------------------------------------
/**
    @brief
    Destroys a CTHDR3 object.

    @param pHDR
    The CTHDR3 object to destroy.

    @since 5.4
*/
SCBIMPORT void cthdr3_destroy(CTHDR3 *pHDR);

//-----------------------------------------------------------------------------
/**
    @brief
    Sets rendering options, only for use after instructions from Scalado.

    @param pHDR
    The CTHDR3 object

    @param options

    @since 5.4
*/
SCBEXPORT CRESULT cthdr3_setExtendedOptions(CTHDR3 *pHDR, CTHDRExtendedOptions options);

//-----------------------------------------------------------------------------
/** @} */
// end 'cthdr3'
//==============================================================================


#ifdef __cplusplus
}//extern "C" {
#endif

#endif // CTHDR_3

