/*  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
.                                                                             .
.    (c) 2000-2009 Scalado AB. All rights reserved.                           .
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

#ifndef CTFACEDETECTOR_STANDARD_H
#define CTFACEDETECTOR_STANDARD_H

#include "ctfacedetector.h"

#ifdef __cplusplus
extern "C" {
#endif

//=============================================================================
/**
    @file ctfacedetector_standard.h
    @brief Public interface for creating a standard CTFaceDetector
    @ingroup fm_face
*/
//=============================================================================

/**
    @addtogroup fm_face
    @{
*/
//-----------------------------------------------------------------------------

/**
    @brief Create a CTFaceDetector.

    This face detector is capable of detecting the following features:

    - ::CTFEATURETYPE_FACE
    - ::CTFEATURETYPE_EYE
    - ::CTFEATURETYPE_SMILE
    - ::CTFEATURETYPE_BLINK
    .

    It requires images in YUV422_Y1UY2V color format.
    When tracking is enabled each detected face will get a unique ID. This ID
    is used for that face in all following frames until the face is lost.

    @param  tracking true if tracking should be enabled, false otherwise.
    Tracking should be enabled for streams of images. The detector
    will then track faces it has detected so detection in following
    frames will be faster.

    @param[out] ppFaceDetector the created face detector.

    @return @c CERR_OK if successful or a specific error code upon failure.

    @since 3.9
 */
SCBIMPORT CRESULT ctfacedetector_createStandard( CTbool tracking, CTFaceDetector **ppFaceDetector );

//-----------------------------------------------------------------------------

/**
    @brief Create a CTFaceDetector.

    This face detector is capable of detecting the following features:

    - ::CTFEATURETYPE_FACE
    - ::CTFEATURETYPE_EYE
    - ::CTFEATURETYPE_SMILE
    - ::CTFEATURETYPE_BLINK
    .

    It requires images in YUV422_Y1UY2V color format.
    When tracking is enabled each detected face will get a unique ID. This ID
    is used for that face in all following frames until the face is lost.

    @param  tracking true if tracking should be enabled, false otherwise.
    Tracking should be enabled for streams of images. The detector
    will then track faces it has detected so detection in following
    frames will be faster.

    @param minFace Specifies the minimum face size in pixels the library should try to
    detect. If zero is specified then it tries to find down
    to the smallest size possible. Increasing this value increases the
    speed. Negative values will result in default value.

    @param[out] ppFaceDetector the created face detector.

    @return @c CERR_OK if successful or a specific error code upon failure.

    @since 5.2
*/
SCBIMPORT CRESULT ctfacedetector_createStandardEx( CTbool tracking, CTint32 minFace, CTFaceDetector **ppFaceDetector );

//-----------------------------------------------------------------------------

/**
    @brief Change configuration for CTFaceDetector.

    Configuration has to be set directly after creating face detector.

    @param [IN] pFaceDetector Face detector

    @param [IN] boostFD Sets a trade-off between face detection rate and false positives.
    It is a value between -100 ... 100:
    - boostFD = 0 - recommended default trade-off.
    - boostFD > 0 - higher detection rate and higher false positives.
    - boostFD < 0 - lower detection rate and lower false positives.
    
    By changing this value, the execution time is affected to a smaller extent.

    @return @c CERR_OK if successful or a specific error code upon failure.

    @since 5.4
*/

SCBIMPORT CRESULT ctfacedetector_setBoostFD(CTFaceDetector *pFaceDetector, CTint32 boostFD);

//-----------------------------------------------------------------------------

/**
    @brief Change configuration for CTFaceDetector.

    Configuration has to be set directly after creating face detector.

    @param [IN] pFaceDetector Face detector

    @param [IN] limitFaceAngleDetection
    If set then the locking of additional faces is performed much
    faster as the library uses the angle of current faces as a
    reference. Therefore if a new face is added to the scene, and some
    faces are already present, then the new face is not locked
    unless it has about the same angle.
    This flag can be set if more performance is needed. It does not
    work well if there is a group of faces (detected not at once) with
    different orientations.
    Tracking is not affected.

    @return @c CERR_OK if successful or a specific error code upon failure.

    @since 5.4
*/
SCBIMPORT CRESULT ctfacedetector_setLimitFaceAngleDetection(CTFaceDetector *pFaceDetector, CTbool limitFaceAngleDetection);

/**
    @brief Change configuration for CTFaceDetector.

    Configuration has to be set directly after creating face detector.

    @param [IN] pFaceDetector Face detector 

    @param [IN] framesPerLock
    This parameter allows to spread the processing time required for face
    locking across several frames. In turn, if more frames are specified,
    then more processing time is allocated for tracking algorithm. In other
    words, increasing this parameter decreases the priority of locking
    and increases the priority of tracking.

    @return @c CERR_OK if successful or a specific error code upon failure.
*/
SCBIMPORT CRESULT ctfacedetector_setFramesPerLock(CTFaceDetector *pFaceDetector, CTuint32 framesPerLock);

/**
    @brief Change configuration for CTFaceDetector.

    Configuration has to be set directly after creating face detector.
    This method can be called only once after creation of the detector.

    @param [IN] pFaceDetector Face detector

    @param [IN] pDetectAngles
    Sets roll face angles to be used by the locking algorithm.

    - During locking the algorithm tries to find first the faces oriented
      according to the first specified angle, then with the second, etc.
    - The angles should be in the interval [-179, 180].
    - The currently supported angles are: 0, +/-30, +/-45, +/-60, +/-90, +/-120,
      +/-135, +/-150, 180 degrees.
    - If pointer is null the algorithm automatically chooses the default
      angles sequence.

     @param [IN] numberOfAngles
     The number of angle values that are specified by pDetectAngles.

    @return @c CERR_OK if successful or a specific error code upon failure.
*/
SCBIMPORT CRESULT ctfacedetector_setDetectAngles(CTFaceDetector *pFaceDetector, CTint16 *pDetectAngles, CTuint32 numberOfAngles);

/** @} */
// end addtogroup 'fm_face'
//=============================================================================

#ifdef __cplusplus
} // extern "C"
#endif

#endif // CTFACEDETECTOR_STANDARD_H

