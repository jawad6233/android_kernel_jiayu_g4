/*----------------------------------------------------

Face Beautification API
Copyright (c) DigitalOptics Corporation
All rights reserved. Confidential.

----------------------------------------------------*/

#ifndef _LIBFB_H_
#define _LIBFB_H_

#ifdef  __cplusplus
extern "C" {
#endif


///////////////////////////////////////////////////////////////////////////
// Status Codes
/** @weakgroup api_Status
  Status codes and interface for obtaining detailed status information. See also @ref gtAPI_Status "the rest" of the status codes.
@{
**************************************************************************/


/// The operation was partly successfull. For now, this code is only returned by the Process function.
/// Detailed information can be obtained using FB_STATUS_INFO object.
#define FB_WARNING                 (GTS_LIB_INFO -1)

/// Operation was cancelled. See ::FB_PROGRESS callback function.
/// Detailed information can be obtained using FB_STATUS_INFO object.
#define FB_CANCELLED               (GTS_LIB_INFO -2)

/// Indicates that no correction were applied to the image.
#define FB_NO_CORRECTION           (GTS_LIB_INFO -3)

/// The operation was not executed. 
/// This code is not returned by the Process function. It only refers to the output of a specific feature
/// thus it is only returned when calling the FB_STATUS_INFO methods.
#define FB_NOT_EXECUTED            (GTS_LIB_INFO -4)

///////////////////////////////////////////////////////////////////////////
// FB_STATUS_INFO
/**
Represents interface for obtaining detailed status information.
**************************************************************************/
typedef struct FB_STATUS_INFO
{
    /** Pointer to function table. */
    struct FB_STATUS_INFO_VFT *vft;
    // private part follows...
} FB_STATUS_INFO;

/**
Features that have been applied.
These values are used by GetStatusForFeature()
*/
enum FBB_STATUS_INFO_FEATURE
{
    /** Skin smoothing */
    FBB_FEAT_CORR_SKIN_SMOOTHING = 0x01,
    /** Eyes enhancement*/
    FBB_FEAT_CORR_EYES_ENHANCEMENT = 0x02,
    /** Teeth whitening */
    FBB_FEAT_CORR_TEETH_WHITENING  = 0x04,
    /** Skin toning */
    FBB_FEAT_CORR_SKIN_TONING  = 0x08,
    /** Eye enlargement */
    FBB_FEAT_CORR_EYES_ENLARGEMENT = 0x10,
    /** Deflash */
    FBB_FEAT_CORR_DEFLASH = 0x20,
    /** Face slimming */
    FBB_FEAT_CORR_FACE_SLIMMING = 0x40  //xiaoyh
};


///////////////////////////////////////////////////////////////////////////
// FB_STATUS_INFO_VFT
/**
Contains pointers to FB_STATUS_INFO functions.
**************************************************************************/
struct FB_STATUS_INFO_VFT
{
    /**
    Destroys the specified status information object.
    @param[in]  si  Pointer to status information object to be destroyed.
    */
    void (GTCALL *Destroy)(
        IN FB_STATUS_INFO* si
        );

    /**
    Get the status for a specified feature.
    @param[in] si        Pointer to status information object to be destroyed.
    @param[in] feature   The feature for which the status is requested. 
                         It is one of the FBB_FEAT_CORR_ enumerated values.
    @return The status for the specified feature, GTS_INVALID_ARG if the requested feature does not exist.
    */
    INT32 (GTCALL *GetStatusForFeature)(
        IN FB_STATUS_INFO* si,
        IN UINT32 feature
        );

    /**
    Get the features that returned a specific status code. Beside a specific status code,
    you can use the generic values IT_LIB_ERROR, IT_LIB_WARNING and IT_LIB_INFO to get the 
    features that returned errors / warnings / informations.
    @param[in] si        Pointer to status information object to be destroyed.
    @return bit field value containing a one or more FBB_STATUS_INFO_FEATURE values OR-ed.
    */
    UINT32 (GTCALL *GetFeaturesForStatus)(
        IN FB_STATUS_INFO* si,
        IN INT32 status
        );
};


/** @} end of api_Status */




/** @weakgroup api_FaceStructure Face Input Information
@{
**************************************************************************/

///////////////////////////////////////////////////////////////////////////
// FB_FACE
/**
Represents the face information structure. An array of FB_FACE is provided to the beautifier.
**************************************************************************/
typedef struct _FB_FACE
{
    /** The size, in bytes, of the ::FB_FACE structure. Set this member to
    <code>sizeof (FB_FACE)</code>. Be sure to set this member before calling any
    function that uses this structure. */
    UINT32  structSize;

    /** The face unique identifier.
    Used to identify the faces between succesive calls to ::AnalyzeFaces or ::Process functions.
    */
    UINT32 id;  //xiaoyh add 2012/11/03

    /** Bounding box surrounding the face area */
    GT_RECT rcFace;

    /** The face roll angle (in-plane orientation) in degrees.
    The valid range is [-179 ... 180].
    A positive value means that the face is rotated clockwise relative
    to the reference image.    */
    INT32   angle;

    /** The face yaw angle  in degrees.
    The valid range is [-179 ... 180].
    */
    INT32 yawAngle;

    /** Bounding box surrounding the left eye */
    GT_RECT rcLeftEye;

    /** Bounding box surrounding the right eye */
    GT_RECT rcRightEye;

    /** Bounding box surrounding the lips */
    GT_RECT rcLips;
} FB_FACE;

/** @} end of api_FaceStructure */

///////////////////////////////////////////////////////////////////////////
// Execution Progress Tracking
/** @weakgroup api_Progress
 Progress tracking mechanism and flags used in progress tracking.
@{
**************************************************************************/
/**
Flags employed in progress tracking
*/
enum FB_PROGRESS_FLAGS
{
	  /** Indicates the beginning of the operation and the parameter uiParam specifies total number of steps this operation has. */
    FB_PROGRESS_BEGIN = 0,
    /** Indicates that the operation is being executed and the parameter uiParam specifies number of completed steps. */
	  FB_PROGRESS_UPDATE,
	  /** Indicates that the operation has finished. */
    FB_PROGRESS_END,
};

///////////////////////////////////////////////////////////////////////////
// FB_PROGRESS
/**
  Progress callback routine.
  The address of this callback is passed to some library functions. It is called
  back when a portion of an operation is completed.
@param[in] context
  The application specific context passed by a library function.
@param[in] reason
  Indicates why the function is being called. This parameter can be one the following values.
  <table>
  <tr><td class="indexkey">Value</td>
      <td class="indexkey">Meaning</td></tr>
  <tr><td>FB_PROGRESS_BEGIN</td>
      <td>Indicates the beginning of the operation and the parameter
      @a uiParam specifies total number of steps this operation has.</td></tr>
  <tr><td>FB_PROGRESS_UPDATE</td>
      <td>Indicates that the operation is being executed and
      the parameter @a uiParam specifies number of completed steps.</td></tr>
  <tr><td>FB_PROGRESS_END</td>
      <td>Indicates that the operation has finished.</td></tr>
  </table>
@param[in] param
  Parameter of callback routine.
@retval 0 Continue current operation.
@retval 1 Cancel current operation. The corresponding library function will
  return status FB_CANCELLED.
@remarks
  For example, an operation consisting of five steps could make the following sequence of callbacks:
  - FB_PROGRESS_BEGIN(5)
  - FB_PROGRESS_UPDATE(1)
  - FB_PROGRESS_UPDATE(1)
  - FB_PROGRESS_UPDATE(1)
  - FB_PROGRESS_UPDATE(2)
  - FB_PROGRESS_END(-)
**************************************************************************/
typedef UINT32
(GTCALL *FB_PROGRESS) (
    IN void* context,
    IN UINT32 reason,
    IN UINT32 param
    );
/** @} end of api_Progress */

///////////////////////////////////////////////////////////////////////////
// Face beautification interface
/**
@weakgroup beautyApi_Core
@{
**************************************************************************/

/**
Flag describing how the correction should be applied. If none of these is set, the library does not apply
correction when face detection data is not available and does not process faces having the width
smaller than 1/5 of the image.
*/
enum FBB_CORRECT
{
    /** Apply correction on the entire image when face detection data is not present */
    FBB_CORRECT_WHEN_NO_FACES         = (1 << 0),
    /** Apply correction on all faces passed to the library regardless of their size */
    FBB_CORRECT_ALL_FACES             = (1 << 1),
};

/**
Optimization level flag values (only applicable to skin smoothing at the moment)
*/
enum FBB_OPT
{
    /** Fastest correction possible */
    FBB_OPT_SPEED_OVER_QUALITY = 1,
    /** Balance between correction quality and speed optimization */
    FBB_OPT_QUALITY_AND_SPEED,
    /** Default mode: best correction quality. */
    FBB_OPT_QUALITY_OVER_SPEED
};

/**
Skin smoothing correction types
*/
enum FBB_CORR_TYPE
{
    /** Removes wrinkles and spots while preserving skin texture */
    FBB_CORR_TYPE_SUBTLE = 1,
    /** Default mode: All skin defects are reduced. */
    FBB_CORR_TYPE_SKINSOFTENING,

	/** Even mode: Even skin coverage, subtle in appearance */
	FBB_CORR_TYPE_EVEN  //xiaoyh
};


/**
Skin processing modes for smoothing and toning
*/
enum FBB_MODE
{
    /** Skin toning is to be applied only to face area */
    FBB_MODE_FACE_ONLY = 1,
    /** Skin toning is to be applied to all skin regions */
    FBB_MODE_FULL_BODY
};

/**
Skin toning types
*/
enum FBB_ST_TYPE
{
    /** Whitens (bleaches) skin*/
    FBB_ST_TYPE_WHITE = 1,
    /** Makes skin look lighter and more pale*/
    FBB_ST_TYPE_PALE,
    /** Warms skin tone */
    FBB_ST_TYPE_WARM,
    /** Darkens skin, making it look naturally tanned */
    FBB_ST_TYPE_TAN,
    /** Colors the skin to a specific make-up color */
    FBB_ST_TYPE_FOUNDATION
};


///////////////////////////////////////////////////////////////////////////
// FB_SKIN_SMOOTHING_CFG
/**
Holds skin smoothing configuration
**************************************************************************/
typedef struct _FB_SKIN_SMOOTHING_CFG
{
	 /** The size, in bytes, of the ::FB_SKIN_SMOOTHING_CFG structure. Set this
    member to <code>sizeof (FB_SKIN_SMOOTHING_CFG)</code>. Be sure to set this
    member before calling any function that uses this structure. */
    UINT32  structSize;

    /**
    Configuration flags for skin smoothing. All other bits that are not specified
    here must be zero. The valid flags are:\n
    - FBB_MODE_FACE_ONLY:\n
     If set, the library will apply correction ONLY on skin regions included in
     faces. \n
    - FBB_MODE_FULL_BODY:\n
     If set, the library will apply correction on most skin regions regardless
     they are linked with a face or not. \n
    */
    UINT32  mode;

    /**
    Type of smoothing to be applied.
    Various beautification modes may be selected (only one of these can be specified at a time):\n
    - FBB_CORR_TYPE_SUBTLE:\n
    The subtle correction removes the wrinkles and spots alone while it keeps
    the texture of the face unchanged.
    - FBB_CORR_TYPE_SKINSOFTENING:\n
    This is the default level of correction for the current version. In this case
    the appearance of the entire skin (including wrinkles, spots, hot spots) is
    changed (softened).
    */
    UINT32 type;

    /**
    Skin smoothing strength level. 
    The valid values are:
      - 0: no skin smoothing is applied
      - 1-255: skin smoothing level
    */
    UINT8 level;

} FB_SKIN_SMOOTHING_CFG;

//////////////////////////////////////////////////////////////////////////
// FOUNDATION_COLOR
/**
Foundation color triple used in skin toning
**************************************************************************/
typedef struct _FOUNDATION_COLOR
{
	UINT8 R; /**< The red component value. */
	UINT8 G; /**< The green component value. */
	UINT8 B; /**< The blue component value. */
} FOUNDATION_COLOR;

//////////////////////////////////////////////////////////////////////////
//FB_SKIN_TONING_CFG
/**
Holds skin toning configuration
**************************************************************************/
typedef struct _FB_SKIN_TONING_CFG
{
	 /** The size, in bytes, of the ::FB_SKIN_TONING_CFG structure. Set this
    member to <code>sizeof (FB_SKIN_TONING_CFG)</code>. Be sure to set this
    member before calling any function that uses this structure. */
    UINT32  structSize;

    /**
    Specifies the area to be processed.
      - FBB_MODE_FACE_ONLY:\n
      Only the face area is to be toned.
      - FBB_MODE_FULL_BODY:\n
      The entire image is scanned and all skin-like regions are toned.
    */
    UINT32 mode;

    /**
    Face toning correction type. The toning method to be applied.
    Specify one of the following three modes:
    - FBB_ST_TYPE_WHITE:\n
    Whitens (bleaches) face. Recommended mainly for darker skin.
      - FBB_ST_TYPE_PALE:\n
      Makes skin look lighter and more pale.
      - FBB_ST_TYPE_WARM:\n
      Warms skin tone.
      - FBB_ST_TYPE_TAN:\n
      Darkens skin, makes it look naturally tanned.
	  - FBB_ST_TYPE_FOUNDATION:\n
      Adjust skin to user defined color.
    */
    UINT32 type;

    /**
    Face toning correction level. The amount of toning to be applied.
    The valid values are:
      - 0: no face toning is applied.
      - 1-255: face toning level.
    */
    UINT8 level;

    /**
    Foundation color in RGB format. 
    The foundation color is applied when the skin toning type is FBB_ST_TYPE_FOUNDATION.
    */
    FOUNDATION_COLOR foundationColor;

} FB_SKIN_TONING_CFG;


/////////////////////////////////////////////////////////////////////////
//FB_TEETH_WHITENING_CFG
/**
Holds teeth whitening configuration
**************************************************************************/
typedef struct _FB_TEETH_WHITENING_CFG
{
    /** The size, in bytes, of the ::FB_TEETH_WHITENING_CFG structure. Set this
    member to <code>sizeof (FB_TEETH_WHITENING_CFG)</code>. Be sure to set this
    member before calling any function that uses this structure. */
    UINT32  structSize;

    /**
    Teeth whitening correction level. The amount of whitening to be applied.
    The valid values are:
      - 0: no teeth whitening is applied.
      - 1-255: teeth whitening level.
    */
    UINT8 level;

} FB_TEETH_WHITENING_CFG;

/////////////////////////////////////////////////////////////////////////
//FB_EYE_ENHANCEMENT_CFG
/**
Holds eye enhancement configuration
**************************************************************************/
typedef struct _FB_EYES_ENHANCEMENT_CFG
{
    /** The size, in bytes, of the ::FB_EYES_ENHANCEMENT_CFG structure. Set this
    member to <code>sizeof (FB_EYES_ENHANCEMENT_CFG)</code>. Be sure to set this
    member before calling any function that uses this structure. */
    UINT32  structSize;

    /**
    Eyes enhancement correction level. The amount of enhancement to be applied.
    The valid values are:
      - 0: no eyes enhancement is applied.
      - 1-255: eyes enhancement level.
    */
    UINT8 level;

} FB_EYES_ENHANCEMENT_CFG;

/////////////////////////////////////////////////////////////////////////
//FB_EYES_ENLARGEMENT_CFG
/**
Holds eye enlargement configuration
**************************************************************************/
typedef struct _FB_EYES_ENLARGEMENT_CFG
{
    /** The size, in bytes, of the ::FB_EYES_ENLARGEMENT_CFG structure. Set this
    member to <code>sizeof (FB_EYES_ENLARGEMENT_CFG)</code>. Be sure to set this
    member before calling any function that uses this structure. */
    UINT32  structSize;

    /**
    Eyes enlargement level. The amount of enlargement to be applied.
    The valid values are:
      - 0: no eyes enlargement.
      - 1-255: eyes enlargement level.
    */    
    UINT8 level;

} FB_EYES_ENLARGEMENT_CFG;

/////////////////////////////////////////////////////////////////////////
//FB_DEFLASH_CFG
/**
Holds deflash configuration
**************************************************************************/
typedef struct _FB_DEFLASH_CFG
{
    /** The size, in bytes, of the ::FB_DEFLASH_CFG structure. Set this
    member to <code>sizeof (FB_DEFLASH_CFG)</code>. Be sure to set this
    member before calling any function that uses this structure. */
    UINT32  structSize;

    /**
    Deflash correction level. The amount of deflash correction to be applied.
    The valid values are:
      - 0: no eyes enlargement.
      - 1-255: eyes enlargement level.
    */
    UINT8 level;

} FB_DEFLASH_CFG;

//xiaoyh
//////////////////////////////////////////////////////////////////////////
// FB_FACE_SLIMMING_CFG
/**
Holds the face slimming configuration
**************************************************************************/
typedef struct _FB_FACE_SLIMMING_CFG
{
    /** The size, in bytes, of the ::FB_FACE_SLIMMING_CFG structure. Set this
    member to <code>sizeof (FB_FACE_SLIMMING_CFG)</code>. Be sure to set this
    member before calling any function that uses this structure. */
    UINT32 structSize;
    /**
    Face slimming correction level. The amount of face slimming correction to be applied.
    The valid values are:
      - 0: no slimming.
      - 1-255: slimming strength.
    */
   UINT8 level;
} FB_FACE_SLIMMING_CFG;
//xiaoyh

///////////////////////////////////////////////////////////////////////////
// FB_BEAUTIFIER_CFG
/**
Holds configuration for the beautification to be applied
**************************************************************************/
typedef struct _FB_BEAUTIFIER_CFG
{
    /** The size, in bytes, of the ::FB_BEAUTIFIER_CFG structure. Set this
    member to <code>sizeof (FB_BEAUTIFIER_CFG)</code>. Be sure to set this
    member before calling any function that uses this structure. */
    UINT32  structSize;

    /** Flag describing how the correction should be applied. 
    - FBB_CORRECT_WHEN_NO_FACES:\n
     If set, the library will apply correction on the entire image when face
     detection data is not present. If the flag is not set, images without faces
     detected will not be changed.\n
    - FBB_CORRECT_ALL_FACES:\n
     If set, the library will apply correction on all faces passed to the
     library no matter their size. By default the library does not apply
     correction to faces with their width smaller than 1/5 of the image
     considering that in such situations the wrinkles, spots or other face
     defects are not so visible that they need reduction. \n
    */
    UINT32 flags;

    /**
    A way to establish balance between speed and quality. If unsure please use
    the recommended value: FBB_OPT_QUALITY_OVER_SPEED.
    - FBB_OPT_SPEED_OVER_QUALITY:\n
    If set, the library will apply fastest correction possible on all faces
    passed to the library. Recommended to be used for slow devices where access
    to memory is a big issue. See datasheet to learn how this parameter impacts
    on speed.
    - FBB_OPT_QUALITY_AND_SPEED:\n
    The library will balance between qualitative correction and speed optimization,
    such as execution time is improved with reduced impact on correction quality.
    Recommended to be used on some embedded devices.
    - FBB_OPT_QUALITY_OVER_SPEED:\n
    Default value. If set, the library will apply most qualitative correction
    on all faces passed to the library. This flag may be used wherever speed
    is not an issue. See datasheet to learn how this parameter impacts on speed.
    */
    UINT32 optimizationLevel;

    /**
    Configuration for skin smoothing
    */
    FB_SKIN_SMOOTHING_CFG skinSmoothingCfg;

    /**
    Configuration for skin toning
    */
    FB_SKIN_TONING_CFG skinToningCfg;

    /**
    Configuration for teeth whitening
    */
    FB_TEETH_WHITENING_CFG teethWhiteningCfg;

    /**
    Configuration for eye enhancement
    */
    FB_EYES_ENHANCEMENT_CFG eyesEnhancementCfg;

    /**
    Configuration for eye enlargement
    */
    FB_EYES_ENLARGEMENT_CFG eyesEnlargementCfg;

    /**
    Configuration for deflash correction
    */
    FB_DEFLASH_CFG deflashCfg;

    /**
    Configuration for the face slimming correction
    */
   FB_FACE_SLIMMING_CFG faceSlimmingCfg;  //xiaoyh


    /**
    Pointer to the hardware data.
    The structure of the hardware data is platform dependent.
    Leave NULL if the platform and the library do not support hardware acceleration.
    */
   void *hardwareData;   //xiaoyh

} FB_BEAUTIFIER_CFG;

///////////////////////////////////////////////////////////////////////////
// FB_BEAUTIFIER
/**
 Represents face beautification interface object, which is the root object 
 in the API. 
 The usage is illustrated by following simplified code:
 
@code
 FB_BEAUTIFIER *btfr = NULL;
 FB_HEAP *heap;
 FB_BEAUTIFIER_CFG cfg;

 ...

 if( (status = FbBeautifierCreate( heap, &cfg, &btfr )) == FB_OK )
 {
      ...
      status = btfr->vft->Process(btfr, &fdImg, &prvImg, 
               &fullImg, faces, numFaces, NULL, NULL, NULL);
      btfr->vft->Destroy( btfr );
}
@endcode	
**************************************************************************/
typedef struct FB_BEAUTIFIER
{
    /** Pointer to function table. */
    struct FB_BEAUTIFIER_VFT *vft;
    // private part follows...
} FB_BEAUTIFIER;

//////////////////////////////////////////////////////////////////////////
// FB_BEAUTIFIER_VFT
/**
 Contains face beautification interface functions.
**************************************************************************/
struct FB_BEAUTIFIER_VFT
{
    ///////////////////////////////////////////////////////////////////////////
    // Destroy
    /**
    Destroys the specified beautification object.
    @param[in] btf
     Pointer to beautification object to be destroyed.
    */
    void (GTCALL *Destroy)(
        IN FB_BEAUTIFIER* btf
        );

    // Process
    /**
	\anchor beautyProcess
     Analyzes the image and then applies face beautification.
    @param[in] btf
     Pointer to the face beautification object.
    @param[in] fdImg
     Pointer to a ::GT_IMAGE structure that specifies very small copy of the
     image to be improved. fdImg is required for the parts of the algorithm
     which require heavy processing power. The recommended resolution is 320x240.
     It should not be bigger than full resolution image ('image' parameter).
     This parameter may be set to NULL.
    @param[in] prvImg
     Pointer to a ::GT_IMAGE structure that specifies a smaller copy of the
     image to be improved. prvImg is required for the parts of the algorithm
     which require medium processing power. The recommended resolution is 1024x768.
     It should not be bigger than full resolution image ('image' parameter).
     This parameter may be set to NULL.
    @param[in] image
     Pointer to a ::GT_IMAGE structure that specifies the full resolution input image which is
     to be modified by the beautification algorithm. The whole image data must be available
     (tiles are not supported in this function).
    @param[in] faces
     Pointer to a ::FB_FACE buffer containing information about the faces. The
     face rectangles must be scaled to match the size of the prvImg. In case
     that prvImg is NULL the size of the faces must match the full resolution
     image.
    @param[in] noFaces
     Specifies the number of faces present in the faces buffer.
    @param[in,out] ppExtStatusInfo
     Detailed status info object returned in case the return status from function is FB_WARNING or FB_CANCELLED.
     If NULL, no detailed status info object is returned. The detailed status is allocated by the library,
     and must be freed when no longer used, calling its Destroy() method.
    @param[in] progress
     Address of an optional callback function of type ::FB_PROGRESS that
     is called to indicate progress of operation. This parameter can be NULL.
    @param[in] context
     Optional argument to be passed to the callback function.
     This parameter can be NULL.
    @return
     If the function succeeds, the return value is GTS_OK.
     If the function succeeds but no correction is performed, the return value is FB_NO_CORRECTION.
     If the function is cancelled, FB_CANCELLED is returned.
     Any other return code specifies an error condition.
    **************************************************************************/
    GTSTATUS (GTCALL *Process) (
        IN FB_BEAUTIFIER* btf,
        IN GT_IMAGE* fdImg OPTIONAL,
        IN GT_IMAGE* prvImg OPTIONAL,
        IN OUT GT_IMAGE* image,
        IN FB_FACE *faces,
        IN INT32 noFaces,
        IN OUT FB_STATUS_INFO **ppExtStatusInfo OPTIONAL,
        IN FB_PROGRESS progress OPTIONAL,
        IN void* context OPTIONAL
        );


    /**
    Analyzes the list of faces to be beautified.
	\anchor beautyAnalyzeFaces
    The library calculates specific characteristics for each face in the input list
    and stores them internally to be used in subsequent calls to the Process function.
	This enables the library to produce consistent results between multiple processing
	rounds (for example in scenarios when preview images are used).
	In case a valid pointer is provided as faceIdArray, the function will populate this
	array with the face IDs ordered by their importance (e.g. largest face first)

    @param[in] btf
     Pointer to the face beautification object.
    @param[in] image
     Pointer to a ::GT_IMAGE structure that specifies the full resolution input image which is
     to be modified by the beautification algorithm. The whole image data must be available
     (tiles are not supported in this function).
    @param[in] faces
     Pointer to a ::FB_FACE buffer containing information about the faces. The
     face rectangles must be scaled to match the size of the input image.
    @param[in] noFaces
     Specifies the number of faces present in the faces buffer.
    @param[out] faceIdArray
     Array filled with face ID's ordered on face quality.
     It needs to be allocated before calling this function.
     This parameter can be NULL.
    @return
     GTS_OK when the function succeeds.
     GTS_NOT_SUPPORTED when the image format is not supported.
     GTS_INVALID_ARG when an invalid argument is supplied (e.g. invalid face rectangles)
     GTS_NO_SYSTEM_RESOURCES when there is no available memory for further processing.

    **************************************************************************/
    GTSTATUS (GTCALL *AnalyzeFaces) (
        IN FB_BEAUTIFIER *btf,
        IN GT_IMAGE* image,
        IN FB_FACE *faces,
        IN INT32 noFaces,
        OUT UINT32 *faceIdArray OPTIONAL
       );//xiaoyh


    /**
    Changes the current configuration of the beautifier.
	\anchor beautySetConfiguration
    @param[in] btf
     Pointer to the face beautification object.
    @param[in] config
     Pointer to the configuration structure.

    @return
     GTS_OK when the function succeeds.
     GTS_NOT_SUPPORTED when one of the features is not supported.
     GTS_INVALID_ARG when an invalid argument is supplied.
     GTS_NO_SYSTEM_RESOURCES when there is no available memory for further processing.
    **************************************************************************/
    GTSTATUS (GTCALL *SetConfiguration) (
        IN FB_BEAUTIFIER     *btf,
        IN FB_BEAUTIFIER_CFG *config
       );//xiaoyh

    /**
    Get the features that can be applied for the face identified by faceId.
    The ::AnalyzeFaces() function must be called before ::GetFeaturesForFace()
    \anchor beautyGetFeaturesForFace
    @param[in] btf
     Pointer to the face beautification object.
    @param[in] faceId
     The face ID
    @param[out] faceFeatures
     Bit field value containing one or more FBB_FEATURE values OR-ed.

    @return 
     GTS_OK when the function succeeds.
     GTS_INVALID_ARG when the input parameters are invalid 
     or ::AnalyzeFaces has not been called previously     
    */
    GTSTATUS (GTCALL *GetFeaturesForFace) (
        IN  FB_BEAUTIFIER   *btf,
        IN  UINT32          faceId,
        OUT UINT32          *faceFeatures
       );//xiaoyh

};

///////////////////////////////////////////////////////////////////////////
// FbBeautifierCreate
/**
  Creates a face beautification config object.
@param[in] heap
  Address of an optional heap object that shall be used for memory management.
  This parameter can be NULL if the library was compiled to use external memory
  allocation functions.
@param[in] cfg
 A pointer ::FB_BEAUTIFIER_CFG configuration structure. Through this structure
 the behavior of the face beautifier can be configured (e.g. correction strength,
 correction area, correction type).
@param[out] btf
  A pointer to a caller supplied variable. On success it will contain the
  pointer to the newly created library context.
@return
  If the function succeeds, the return value is GTS_OK.
**************************************************************************/
GTAPI
GTSTATUS
GTCALL
FbBeautifierCreate(
    IN GT_HEAP* heap OPTIONAL,
    IN FB_BEAUTIFIER_CFG *cfg,
    OUT FB_BEAUTIFIER **btf
    );

/** @} end of beautyApi_Core */

///////////////////////////////////////////////////////////////////////////
// Heap interface
/**
@addtogroup gtAPI_Heap

  Applications can create heap object in two ways:
  - Supply work buffer to ::FbHeapCreate function which will create heap object.
  - Create heap object by hand.

@{
**************************************************************************/

///////////////////////////////////////////////////////////////////////////
// FbHeapCreate
/**
  Creates the heap management object.
@param[in] buffer
  The address to the heap buffer.
@param[in] size
 The size of the heap buffer.
@param[out] heap
  Pointer to the heap object.
@return
  If the function succeeds, the return value is GTS_OK.
**************************************************************************/
GTAPI 
GTSTATUS 
GTCALL
FbHeapCreate(
   IN void *buffer,
   IN UINT32 size,
   OUT GT_HEAP **heap
);

/** @} end of gtAPI_Heap */


#ifdef __cplusplus
}
#endif

#endif // _LIBFB_H_
