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
. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

#ifndef FM_HDRIMAGE_H
#define FM_HDRIMAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "caps_base.h"
#include "caps_types.h"

//=============================================================================
/**
    @file fm_hdrimage.h
    @brief Defines the interface of the Caps Feature Module: @a HDR Imaging
    @ingroup fm_hdrimage
*/
//=============================================================================
/**
    @addtogroup fm_hdrimage
    @brief Creation of High Dynamic Range images.
    
    @deprecated This interface is deprecated. Use @ref fm_hdr2 instead.

    The CTHDRImage API provides functionality for creating HDR (High Dynamic Range)
    images, given a set of images taken with different exposure settings, but
    depicting the same scene.

    <h3>Usage</h3>
    -# Create a new HDR object with cthdrimage_create()
    -# Add two, or more, images to the HDR object using cthdrimage_addLdrSession()
    -# Render a preview to display on screen using cthdrimage_generatePreview(),
       @n
       or render the result directly to file with cthdrimage_render()
    -# Destroy the HDR object, cthdrimage_destroy()
    .
    
    @{
*/
//-----------------------------------------------------------------------------
/**
    @brief Type representing a HDR image.
*/
typedef struct CTHDRImage CTHDRImage;

//-----------------------------------------------------------------------------

/**
    @brief Image registration options.
*/
typedef enum CTHDRRegistration
{
    /**
        @brief No image registration will be performed; i.e. the provided images
        will be assumed to be perfectly aligned.
    */
    CTHDRREGISTRATION_NONE,
    /**
        @brief Register image relative to the first added image.
    */
    CTHDRREGISTRATION_LIGHTEST,
    /**
        @brief Register image relative to the previously added image.
    */
    CTHDRREGISTRATION_PREVIOUS
} CTHDRRegistration;

//-----------------------------------------------------------------------------
/**
    @brief Render options for HDR images.

    Note that these values only need to be explicitly set if the automatically
    set/computed values should be overridden.
*/
typedef struct CTHDROptions
{
    /**
        @brief Specifies a manual exposure difference between images.

        Defined as 2<sup>x</sup>, where @c x is the EV-step (Exposure Value step size).
        The difference has to be equal for all images.

        If set to @c 0 (zero), the algorithm will analyze the images to estimate the
        exposure differences. If set to something other than zero, the algorithm is
        forced to use the specified value instead.
    */
    CTfloat exposure;

    /**
        @brief Specifies a pseudo-gamma to use when generating a tone mapped image.

        This will affect the overall brightness of the image. Lower values will
        produce a brighter image.

        Valid range [0,2]. The default value is 0.5.
    */
    CTfloat gamma;

    /**
        @brief Specifies which kind of image registration to use.

        The default registration type is ::CTHDRREGISTRATION_LIGHTEST
    */
    CTHDRRegistration registration;
}CTHDROptions;

//-----------------------------------------------------------------------------
/**
    @brief
    Creates a CTHDRImage object.

    @param[out] ppHdrObject
    Pointer to the created CTHDRImage object if the function returns CERR_OK.

    @return CERR_OK on success.

    @see
    cthdrimage_destroy()

    @since 3.11

    @deprecated This function is deprecated. Use @ref fm_hdr2 instead.
*/
SCBIMPORT CRESULT cthdrimage_create( CTHDRImage **ppHdrObject );

//-----------------------------------------------------------------------------

/**
    @brief
    Adds an image, provided via a CTSession, to the CTHDRImage object.

    All images that the created HDR image will be based on must be added before calling
    cthdrimage_render(), or cthdrimage_generatePreview().

    Any added session must not be modified until after the CTHDRImage object
    has been destroyed.

    Note that the brightest image in the set must be added first.

    @param pHdrObject
    A valid CTHDRImage object.

    @param ldrSession
    A session providing the image to add.

    @return CERR_OK on success.

    @see
    - cthdrimage_generatePreview()
    - cthdrimage_render()
    .

    @since 3.11

    @deprecated This function is deprecated. Use @ref fm_hdr2 instead.
*/
SCBIMPORT CRESULT cthdrimage_addLdrSession( CTHDRImage *pHdrObject, CTSession ldrSession );

//-----------------------------------------------------------------------------
/**
     @brief
     Returns the dimensions of the output of the next call to cthdrimage_render() or cthdrimage_renderRaw().

     All images that the created HDR image should be based on must be added before calling cthdrimage_getRenderSize().

     @param pHDRImage
     A valid CTHDRImage object.

     @param pHDROptions
     If @c NULL, default settings will be used (recommended).
     If a CTHDROptions struct with valid settings are passed, these will be used
     instead.

     @param[out] pSize
     A valid CTSize object. The dimensions of the next rendered image will be inserted in the provided CTSize object.

     @since 5.0

     @deprecated This function is deprecated. Use @ref fm_hdr2 instead.
*/
SCBIMPORT CRESULT cthdrimage_getRenderSize(CTHDRImage *pHDRImage, const CTHDROptions *pHDROptions, CTSize *pSize);

//-----------------------------------------------------------------------------
/**
    @brief
    Creates a HDR image from a set of images, and renders it to the provided image.

    At least two LDR (Low Dynamic Range) images must have been added with
    cthdrimage_addLdrSession() before a HDR image can be created.

    The size of the generated HDR image will correspond to the size of the intersection
    of all the added LDR images after registration. This size can be fetched with cthdrimage_getRenderSize().

    @param pHDRImage
    A valid CTHDRImage object.

    @param pHDROptions
    If @c NULL, default settings will be used (recommended).
    If a CTHDROptions struct with valid settings are passed, these will be used instead.

    @param[in,out] pOutputImage
    An output image to which the HDR image will be rendered.
    The image will be rendered to the color format described by the CTImage. This image should have the size described
    by cthdrimage_getRenderSize().

    @param[in,out] ppIterator
    A pointer to a variable of the type (CTIterator *), or @c NULL.
    @n
    In the latter case, the operation will be completed directly.
    (see: @ref caps_iterators).

    @return CERR_OK on success.

    @see
    - cthdrimage_addLdrSession()
    - cthdrimage_getRenderSize()
    - cthdrimage_generatePreview()
    .

    @since 5.0

    @deprecated This function is deprecated. Use @ref fm_hdr2 instead.
*/
SCBIMPORT CRESULT cthdrimage_renderRaw(CTHDRImage *pHDRImage,
                                       const CTHDROptions *pHDROptions,
                                       CTImage *pOutputImage,
                                       CTIterator **ppIterator);

//-----------------------------------------------------------------------------
/**
    @brief
    Creates a HDR image from a set of images, and renders it to a provided stream.

    At least two LDR (Low Dynamic Range) images must have been added with
    cthdrimage_addLdrSession() before a HDR image can be created.

    The size of the generated HDR image will correspond to the size of the intersection
    of all the added LDR images after registration.

    @param pHdrObject
    A valid CTHDRImage object.

    @param pHdrOptions
    If @c NULL, default settings will be used (recommended).
    If a CTHDROptions struct with valid settings are passed, these will be used
    instead.

    @param[in,out] pHdrStream
    An output stream to which the HDR image will be written.
    The rendered image will be encoded as a Jpeg.

    @param[in,out] ppIterator
    A pointer to a variable of the type (CTIterator *), or @c NULL.
    @n
    In the latter case, the operation will be completed directly.
    (see: @ref caps_iterators).

    @return CERR_OK on success.

    @see
    - cthdrimage_addLdrSession()
    - cthdrimage_generatePreview()
    .

    @since 3.11

    @deprecated This function is deprecated. Use @ref fm_hdr2 instead.
*/
SCBIMPORT CRESULT cthdrimage_render( CTHDRImage *pHdrObject,
                                     const CTHDROptions *pHdrOptions,
                                     CTStream *pHdrStream,
                                     CTIterator **ppIterator );

//-----------------------------------------------------------------------------
/**
    @brief
    Creates a HDR image from a set of images, and renders it to a provided
    @c CTImage buffer.

    At least two LDR (Low Dynamic Range) images must have been added with
    cthdrimage_addLdrSession() before a HDR image can be created.

    The HDR image will be scaled to fit the dimensions specified in the provided
    CTImage buffer; i.e. the resolution given by CTImage::dims.

    The output color format is determined by that specified in CTImage::format.
    
    @param pHdrObject
    A valid CTHDRImage object.

    @param pHdrOptions
    If @c NULL, default settings will be used (recommended).
    If a CTHDROptions struct with valid settings are passed, these will be used
    instead.

    @param[in,out] pHdrImage
    An image buffer that the HDR image will be written to.

    @param[in,out] ppIterator
    A pointer to a variable of the type (CTIterator *), or @c NULL.
    @n
    In the latter case, the operation will be completed directly.
    (see: @ref caps_iterators).

    @return CERR_OK on success.
    
    @see
    - cthdrimage_addLdrSession()
    - cthdrimage_render()
    .

    @since 3.11

    @deprecated This function is deprecated. Use @ref fm_hdr2 instead.
*/
SCBIMPORT CRESULT cthdrimage_generatePreview( CTHDRImage *pHdrObject,
                                              const CTHDROptions *pHdrOptions,
                                              CTImage *pHdrImage,
                                              CTIterator **ppIterator );

//-----------------------------------------------------------------------------
/**
    @brief
    Destroys a CTHDRImage object.

    @param pHdrObject
    The CTHDRImage object to destroy.

    @since 3.11

    @deprecated This function is deprecated. Use @ref fm_hdr2 instead.
*/
SCBIMPORT void cthdrimage_destroy( CTHDRImage *pHdrObject );

//-----------------------------------------------------------------------------
/** @} */
// end 'fm_hdrimage'
//==============================================================================


#ifdef __cplusplus
}//extern "C" {
#endif

#endif // FM_HDRIMAGE_H

