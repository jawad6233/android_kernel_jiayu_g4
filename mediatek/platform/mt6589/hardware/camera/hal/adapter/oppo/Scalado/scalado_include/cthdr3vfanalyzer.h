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

#ifndef CTHDR3VFANALYZER_H
#define CTHDR3VFANALYZER_H


//-----------------------------------------------------------------------------
// User includes

#include "scbdefs.h"
#include "ctbitmap.h"


//-----------------------------------------------------------------------------
// System includes


//-----------------------------------------------------------------------------
// Guard for c++

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------
/**
    @file cthdr3vfanalyzer.h
    @brief
    Public interface for CTHDR3VFAnalyzer
    @ingroup cthdrvfanalyzer
*/
//-----------------------------------------------------------------------------
/**
    @addtogroup cthdrvfanalyzer
    @brief
    Provides functionality for selecting exposure values to use
    for the HDR input images. This is achieved by performing an analysis on a
    view frame with normal exposure.

    HDR3 accepts three images with different exposures. Images are normally
    selected at normal EV (exposure value) and then at e.g. -2 EV, +2 EV relative to
    the normal EV. However, there are situations when doing it this way will produce
    one image with very little, or no additional information that can be of value to
    the algorithm.
    By performing an analysis on a view finder frame taken just before the image is
    captured we are able to find the proper directions to go with the EV for the images i.e.
    [-,0 ,+], [-,-,0] or [0,+,+] (0 indicates normal exposure)
    The analyzer is configured with different criterias for the images. Each
    criteria is defined as a pixel value range and an amount of pixels given as
    fraction of the total pixels in an intensity histogram. If the range contains
    the amount of pixels (or more) in the range the criteria is fulfilled.
    Based on the fulfilled criterias a decision is made how the images should be
    captured.

    - Usage
    The picture below depicts the different ranges in a histogram
    when using the default parameters suggested in @c cthdr3vfanalyzer_create

    @verbatim
        +
        +--------------+ -------------+
        +              + +            +
        +----          + +        +---+
        +   +          + +        +   +
        + A +    B     + +   C    + D +
        +   +          + +        +   +
        +++++++++...+++++++...++++++++++++
        0   10        127        245  256
    @endverbatim

    There are four ranges:
    - Under exposed defined by range A, [0-10, 0.01] and a fraction of 0.01.
    - Light image defined by range B, [0-128, 0.80] and a fraction of 0.80.
    - Dark image defined by range C, [128-256, 0.80] and a fraction of 0.80.
    - Over exposed defined by range D, [245-256, 0.01) and a fraction of 0.01.

    When the analyzer has been created with the wanted criterias @c cthdr3vfanalyzer_analyze
    may be called. Typically the last view frame that was captured before the snapshot
    is passed. It is possible to reuse the same analyzer for several frames.

    After an analysis have been performed it is possible to get the result by
    calling either @c cthdr3vfanalyzer_getResult or @c cthdr3vfanalyzer_getResultEx.
    the first one implements a decision algorithm based on which criterias that were
    fulfilled. The second operation is provided in order to allow a user to
    implement a customized decision algorithm based on the fulfilled criterias.
    @{
*/
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type definitions for public and hidden objects

/**
    @brief Type representing a HDR3 view finder analyzer object.
*/
typedef struct CTHDR3VFAnalyzer CTHDR3VFAnalyzer;

//-----------------------------------------------------------------------------
// Public type definitions

/**
 * @brief
 * :CTHDR3VFCriteria defines a range of pixel values in
 * an intensity histogram together with a fraction of pixels in this range.
 *
 * If the fraction of the pixels are contained in the range the criteria is
 * considered to be fulfilled.
 */
typedef struct CTHDR3VFCriteria
{
    /**
     * @brief
     * Lowest inclusive pixel value part of this criteria.
     */
    CTuint32 lowValue;

    /**
     * @brief
     * Highest exclusive pixel value part of this criteria.
     */
    CTuint32 highValue;

    /**
     * @brief
     * Fraction of pixels in the range lowValue to highValue for this criteria
     * to be considered true
     */
    CTfloat fraction;
} CTHDR3VFCriteria;

/**
 * @brief
 * Enumerator for analysis result used together with @c cthdr3vfanalyzer_getResult
 */
typedef enum CTHDR3VFAnalysisResult
{
    /**
     * @brief
     * The ranges defined for over exposed and under exposed criteria are
     * not fulfilled.
     * In this case HDR3 is not feasible, a better option is to use
     * single image HDR @see ctsingleimagehdr_apply(). However if HDR3 is
     * used anyway the images should alternate @see CTHDR3VFANALYZER_ALTERNATE_EV.
     */
    CTHDR3VFANALYZER_IN_RANGE,

    /**
     * @brief
     * Capture two images with increasing EV relative to normal exposure.
     * For example: +2 and +4
     */
    CTHDR3VFANALYZER_INCR_EV,

    /**
     * @brief
     * Capture two images with decreasing EV relative to normal exposure.
     * For example: -2 and -4
     */
    CTHDR3VFANALYZER_DECR_EV,

    /**
     * @brief
     * Capture two images by alterning with incraesed/decreased EV compared to normal
     * exposure.
     * For example: -2 and +2.
     */
    CTHDR3VFANALYZER_ALTERNATE_EV,

} CTHDR3VFAnalysisResult;

//-----------------------------------------------------------------------------
// Declaration of global const variables


//-----------------------------------------------------------------------------
// Declaration of public functions

/**
 * @brief
 * Creates a CTHDR3Analyzer object.
 *
 * It should be noted that due to differences between platforms and limitations of
 * the analysis no absolute correct values for the criteras can be given.
 * Each application must be calibrated to find values that fit that
 *  particular integration. With that in mind the following values could be
 *  used as a start:
 * - Over exposed: range(245-256) fraction(0.01)
 * - Under exposed: range(0-11) fraction(0.01)
 * - Light image: range(128-256) fraction(0.80)
 * - Dark image: range(0-128) fraction(o.80)
 *
 * @param overExposed
 * Criteria to consider an image over exposed.
 *
 * @param underExposed
 * Criteria to consider an image under exposed.
 *
 * @param darkImage
 * Criteria for when an image is considered to be dark.
 *
 * @param lightImage
 * Criteria when an image is considered to be light.
 *
 * @param ppAnalyzer
 * The created HDR3VFAnalyzer.
 *
 * @return CERR_OK on success.
 *
 * @since 5.4
 */
CRESULT cthdr3vfanalyzer_create(CTHDR3VFCriteria overExposed,
                                CTHDR3VFCriteria underExposed,
                                CTHDR3VFCriteria darkImage,
                                CTHDR3VFCriteria lightImage,
                                CTHDR3VFAnalyzer **ppAnalyzer);

/**
 * @brief
 * Destroys a CTHDR3VFAnalyzer object.
 *
 * @param pAnalyzer
 * View finder analyzer object to destroy.
 *
 * @since 5.4
 */
void cthdr3vfanalyzer_destroy(CTHDR3VFAnalyzer *pAnalyzer);

/**
 * @brief
 * Performs analysis on one frame using the parameters used at creation.
 * It is allowed to perform consecutive analyzes with the same analyzer.
 *
 * @param pAnalyzer
 * View finder analyzer object.
 *
 * @param pBitmap
 * The view finder frame to analyze, supported color formats
 * - @c CM_YUV420SP
 * - @c CM_YUV420P
 *
 * @return CERR_OK on success.
 *
 * @since 5.4
 */
CRESULT cthdr3vfanalyzer_analyze(CTHDR3VFAnalyzer *pAnalyzer, CTBitmap *pBitmap);


/**
 * @brief
 * Gets the result from the latest analysis as a CTHDR3VFAnalysisResult.
 *
 * This operation implements a default decision algorithm based on the
 * fulfilled criterias. If this algorithm does not fit your particular
 * application it is possible to use @ccthdr3vfanalyzer_getResultEx
 * in order to implement a customized decision algorithm.
 *
 * @param pAnalyzer
 * View finder analyzer object.
 *
 * @note A pre-requisite is that operation @c cthdr3vfanalyzer_analyze has been
 *       called before invoking this operation
 *
 * @param[out] pResult
 * The result of the most recent call to @ccthdr3vfaanalyzer_analyze.
 * - CTHDR3VFANALYZER_IN_RANGE,
 *      - all pixels are in the range [underExposed.highValue,overExposed.lowValue]
 * - CTHDR3VFANALYZER_DECR_EV
 *      - image is partially under exposed.
 * - CTHDR3VFANALYZER_INCR_EV
 *      - image is partially over exposed.
 * - CTHDR3VFANALYZER_ALTERNATE_EV,
 *      - image is partially under exposed and partially over exposed or
 *      - over exposed and dark or
 *      - under exposed and light
 *
 * @return CERR_OK on success.
 *
 * @since 5.4
 */
CRESULT cthdr3vfanalyzer_getResult(CTHDR3VFAnalyzer *pAnalyzer, CTHDR3VFAnalysisResult *pResult);

/**
 * @brief
 * Gets the result from the latest analysis as fulfilled criterias.
 *
 * This operation allows an application to implement their own decision
 * algorithm instead of using the one provided by @cthdr3vfanalyzer_getResult.
 *
 * @note A prerequisite is that operation @c cthdr3vfanalyzer_analyze has been
 *       called before invoking this operation
 *
 * @param pAnalyzer
 * View finder analyzer object.
 *
 * @param[out] pIsOverExposed
 * Indication whether over exposed criteria is fulfilled.
 *
 * @param[out] pIsUnderExposed
 * Indication whether under exposed criteria is fulfilled.
 *
 * @param[out] pIsDarkImage
 * Indication whether the dark image criteria is fulfilled.
 *
 * @param[out] pIsLightImage
 * Indication whether the light image criteria is fulfilled.
 *
 * @return CERR_OK on success.
 *
 * @since 5.4
 */
CRESULT cthdr3vfanalyzer_getResultEx(CTHDR3VFAnalyzer *pAnalyzer,
                                     CTbool *pIsOverExposed,
                                     CTbool *pIsUnderExposed,
                                     CTbool *pIsDarkImage,
                                     CTbool *pIsLightImage);

//-----------------------------------------------------------------------------
/** @} */
// end 'cthdrvfanalyzer'
//==============================================================================

#ifdef __cplusplus
}      // extern "C"
#endif // __cplusplus
#endif // CTHDR3VFANALYZER_H
