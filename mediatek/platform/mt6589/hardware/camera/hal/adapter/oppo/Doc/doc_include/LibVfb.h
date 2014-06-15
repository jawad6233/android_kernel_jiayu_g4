/*----------------------------------------------------

Video Face Beautification library

Copyright (c) DigitalOptics Corporation
All rights reserved

----------------------------------------------------*/

#ifndef __LIBVFB_H__
#define __LIBVFB_H__


#include "gtypes.h"


#ifdef  __cplusplus
extern "C" {
#endif


///////////////////////////////////////////////////////////////////////////
/**
@defgroup api_ver API Versions
  API version list and history of changes.
@{
  The API version is a 32-bit value where the higher 16 bits represent the
  major version and the lower 16 bits represent the minor version.
  When the minor is increased the API remains backward compatible. When
  the major is increased the API may not be backward compatible.

  To retrieve the API version, call VFB_LIB_VFT::GetLibInfo() and read
  the VFB_LIB_INFO::apiVer field. It is different from the library version
  (VFB_LIB_INFO::libVer). Library version refers to the implementation,
  however API version refers only to the API definition.

  Below is the list of API versions, and the corresponding changes which
  were introduced in every version.

  - 0x00020000
    - General API redesign.

  - 0x00010000
    - Initial version.
*/
/// @} end of api_ver


///////////////////////////////////////////////////////////////////////////
/**
@defgroup api_supfmts Supported Image Formats
  List of supported color formats.
@{
  The Video Face Beautification library supports only the formats below:
  - GT_FMT_YUV_444_P3
  - GT_FMT_YUV_422_P3
  - GT_FMT_YUV_420_P3
  - GT_FMT_UYVY_422
  - GT_FMT_YUV16_444
  - GT_FMT_YUV16_444_P3
  - GT_FMT_YUV_420_P2
  - GT_FMT_YVU_420_P2

  These formats are all supported natively by the library.

  For more details about these color formats, please see @ref gtAPI_Fmt
*/
/// @} end of api_supfmts


///////////////////////////////////////////////////////////////////////////
// VFBS_*
/**
@weakgroup gtAPI_Status Status Codes
  These constants indicate the result of an API function call.
@{
**************************************************************************/

/** Can be returned by VFB_ENGINE_VFT::Process to indicate that it executed
  without errors, but did not yet apply VFB correction. */
#define VFBS_NO_CORRECTION      ((GTSTATUS)1)

/** Tests if the status code returned by a function indicates an error. */
#define VFB_FAILED( status )    ((status) < 0)

/// @} end of gtAPI_Status



///////////////////////////////////////////////////////////////////////////
// Video Face Beautification Engine
/**
@defgroup api_eng Video Face Beautification Engine
@{
**************************************************************************/


///////////////////////////////////////////////////////////////////////////
// VFB_CONFIG_OPTIONS
/**
  Configuration option flags that regulate the type of processing applied.
*/
enum VFB_CONFIG_OPTIONS
{
    /** If set, VFB corrects all the image, no matter if there is any face detection
      information. In this case, it may happen for other skin-like objects to be
      modified. Correction will be lighter until at least information of one face
      is sent to VFB.
      Without this parameter, VFB will run in the default mode and operate based
      on face detection information. */
    VFB_CORRECT_WHEN_NO_FACES   = (1 << 0)
};

///////////////////////////////////////////////////////////////////////////
// VFB_CORR_LEVELS
/**
  Correction strength flags.
*/
enum VFB_CORR_LEVELS
{
    /** Level 1 - Lowest Level */
    VFB_CORR_LVL1 = 1,
    /** Level 2 */
    VFB_CORR_LVL2,
    /** Level 3 */
    VFB_CORR_LVL3,
    /** Level 4 */
    VFB_CORR_LVL4,
    /** Level 5 - Default Level*/
    VFB_CORR_LVL5,
    /** Level 6 */
    VFB_CORR_LVL6,
    /** Level 7 */
    VFB_CORR_LVL7,
    /** Level 8 */
    VFB_CORR_LVL8,
    /** Level 9 */
    VFB_CORR_LVL9,
    /** Level 10 - Maximum Level */
    VFB_CORR_LVL10,
    /** Alias for maximum Level */
    VFB_CORR_LVL_MAX = VFB_CORR_LVL10
};


///////////////////////////////////////////////////////////////////////////
// VFB_CONFIG
/**
  VFB engine configuration structure.
*/
typedef struct
{
    /** The size, in bytes, of the ::VFB_CONFIG structure. Set this 
      member to <code>sizeof (VFB_CONFIG)</code>. Be sure to set this
      member before calling any function that uses this structure. */
    UINT32  structSize;

    /** Configuration options. Can be one of the values in @ref VFB_CONFIG_OPTIONS. */
    UINT32  options;

    /** Correction strength level. Strength can be picked from values between
      VFB_CORR_LVL1 (most subtle correction) to VFB_CORR_LVL10 (strongest correction).
      If this parameter is 0, the default value of VFB_CORR_LVL5 will be used. */
    UINT32  correctionStrength;

    /** Accuracy of corrected area. It is a value from 1 to 255. The recommended value is 128.
      A smaller value would make the corrected area more inclusive; this implies that all
      possible skin areas will be corrected but also it may have a lot of FALSE POSITIVES.
      A higher value implies less correction but also less FALSE POSITIVES.
      If this parameter is 0, the default value of 128 will be used. */
    UINT32  correctionAccuracy;

    /** The number of frames in which to increase from correction strength 0 (start of a video)
      up to the decided correction strength. The increase is linear.
      If this parameter is 0, the default value of 30 will be used. */
    UINT32  framesInertia;

} VFB_CONFIG;


///////////////////////////////////////////////////////////////////////////
// VFB_FACE
/**
  Structure containing data about a detected face.
*/
typedef struct
{
    /** The size, in bytes, of the ::VFB_FACE structure. Set this 
      member to <code>sizeof (VFB_FACE)</code>. Be sure to set this
      member before calling any function that uses this structure. */
    UINT32  structSize;

    /** The bounding rectangle of the face. The coordinates are expressed
      in pixels and are relative to the size of the current frame. */
    GT_RECT boundRect;

    /** Unique ID assigned to the current face. */
    INT32   trackingID;

    /** Face orientation angle. Can have the following values:
      - if the face is tilted to the right, it is a positive between [0, 180]
      - if the face is tilted to the left, it is a negative between (-179, 0]. */
    INT32   angle;

    /** Indicates whether the current face is detected with very high confidence,
      and it's position not simply estimated across frames. */
    INT32    isTracked;//wuhongu

} VFB_FACE;

///////////////////////////////////////////////////////////////////////////
// VFB_ENGINE
/**
  Engine object.
  Contains the video face beautification processing functionality.
*/
typedef struct
{
    /** Pointer to the function table. */
    struct VFB_ENGINE_VFT *vft;

} VFB_ENGINE;

///////////////////////////////////////////////////////////////////////////
// VFB_ENGINE_VFT
/**
  Engine object interface definition.
  Contains pointers to the object's functions. Please note that these functions
  are not thread safe.
*/
struct VFB_ENGINE_VFT
{
    ///////////////////////////////////////////////////////////////////////////
    // Destroy
    /**
    Destroys the VFB engine object.
    @param[in] eng
      Pointer to beautification object to be destroyed.
    */
    void (GTCALL *Destroy)(
        IN OUT VFB_ENGINE* eng
        );

    ///////////////////////////////////////////////////////////////////////////
    // Process
    /**
    Applies Video Face Beautification on the frame.
    @param[in] eng
      Pointer to the Video Face Beautification engine object.
    @param[in,out] image
      Pointer to the GT_IMAGE structure representing the current frame. Upon the function's
      return, the specified faces will have had beautification applied over them.
    @param[in] faces
      Pointer to the array of faces detected in the current frame.
    @param[in] numFaces
      Number of elements in the 'faces' array.
    @return
      If the function succeeds, the return value is GTS_OK.
      If the function executes with no errors, but correction has not yet been applied, the
      function returns VFBS_NO_CORRECTION.
      If an error occurs, a relevant GTS_* error code is returned.
    @remarks
      - The library will apply VFB on the first 4 largest faces, even if more than 4
      are specified.
    */
    GTSTATUS (GTCALL *Process)(
        IN     VFB_ENGINE*     eng,
        IN OUT GT_IMAGE*       image,
        IN     const VFB_FACE* faces,
        IN     UINT32          numFaces
        );

    ///////////////////////////////////////////////////////////////////////////
    // SetConfig
    /**
    Sets new parameters for the VFB engine object.
    @param[in] eng
      Pointer to the Video Face Beautification engine object.
    @param[in] cfg
      Pointer to the engine configuration structure.
    @return
      If the function succeeds, it returns GTS_OK, and a relevant GTS_* error code otherwise.
    */
    GTSTATUS (GTCALL *SetConfig)(
        IN VFB_ENGINE*       eng,
        IN const VFB_CONFIG* cfg
        );
};


/// @} end of api_eng */



///////////////////////////////////////////////////////////////////////////
// Library Interface
/**
@defgroup api_lib Library Interface
  Applications can access the library's functions through the library object.
@{
**************************************************************************/


///////////////////////////////////////////////////////////////////////////
// VFB_LIB_INFO
/**
  Library information structure.
*/
typedef struct
{
    /** The size, in bytes, of the ::VFB_LIB_INFO structure. The field is set
      by the library. The application may check this field to decide whether it can
      access the fields added later in API. */
    UINT32        structSize;

    /** Library version, represented by an array of numbers.
      The first four numbers in the array represent the main version numbers,
      from the major version number (first number in the array, element [0])
      to the minor version (fourth number, element [3]).
      The last number (element [4]) is an optional revision number. */
    UINT32        libVer[5];

    /** API version, represented as a 32-bit value, where the higher 16 bits
      represent the major version and the lower 16 bits represent the minor version.
      This is the version of library API, and does not change when there are only
      changes in implementation. The API version changes when the API is modified,
      so an application might query whether newly added features are available.
      For the current list of API versions see @ref api_ver. */
    UINT32        apiVer;

    /** String specifying build date in form "MMM DD YYYY HH:MM:SS". */
    const char*   buildDate;

    /** Optional release label. */
    const char*   label;

    /** An array of @ref api_supfmts "supported color formats". The array is
      terminated with a zero item (GT_FMT_UNKNOWN). */
    const UINT32* supportedFmts;

} VFB_LIB_INFO;


///////////////////////////////////////////////////////////////////////////
// VFB_USAGE_INFO
/**
  This structure is used as input to @ref VFB_LIB_VFT::GetRequiredHeapSize,
  and all its contained parameters are used to calculate the maximum amount
  of memory which a @ref VFB_ENGINE object might allocate during its lifetime.
*/
typedef struct
{
    /** The size, in bytes, of the VFB_USAGE_INFO structure. Set this member
      to <code>sizeof(VFB_USAGE_INFO)</code>. Be sure to set this member before calling
      any function that uses this structure. */
    UINT32  structSize;

    /** Width in pixels of the captured input images used for processing. */
    UINT32  imageWidth;

    /** Height in pixels of the captured input images used for processing. */
    UINT32  imageHeight;

    /** Color format of the captured images, indicated by one of the
      @ref api_supfmts "supported format" constants. */
    UINT32  imageFormat;

} VFB_USAGE_INFO;


///////////////////////////////////////////////////////////////////////////
// VFB_LIB
/**
  Library object.
*/
typedef struct
{
    /** Pointer to the function table. */
    struct VFB_LIB_VFT* vft;

} VFB_LIB;

///////////////////////////////////////////////////////////////////////////
// VFB_LIB
/**
  Library object interface definition.
*/
struct VFB_LIB_VFT
{
    ///////////////////////////////////////////////////////////////////////////
    // GetLibInfo
    /**
    Retrieves versioning information about the library.
    @param[in] lib
      Pointer to this library object.
    @return
      Pointer to an VFB_LIB_INFO structure with library version information.
    */
    const VFB_LIB_INFO* (GTCALL *GetLibInfo)(
        IN  VFB_LIB* lib
        );

    ///////////////////////////////////////////////////////////////////////////
    // GetRequiredHeapSize
    /**
    Calculates with a very close approximation the total amount of memory which
    the library will require for all its internal allocations.
    @param[in] lib
      Pointer to this library object.
    @param[in] usageInfo
      Pointer to a @ref VFB_USAGE_INFO structure describing the way the engine will be used.
    @return
      The approximate amount of heap memory, expressed in bytes, required by the library.
      The amount returned will never be less than the total number of bytes the library
      could need to allocate.
      In an invalid parameter is specified (eg, <i>lib</i> is set to NULL, <i>usageInfo->format</i>
      is not supported), the function will return 0xFFFFFFFF.
    */
    UINT32 (GTCALL *GetRequiredHeapSize)(
        IN  VFB_LIB*              lib,
        IN  const VFB_USAGE_INFO* usageInfo
        );

    ///////////////////////////////////////////////////////////////////////////
    // CreateHeap
    /**
    Creates a heap object, managed by the library, and backed by the specified
    memory buffer.
    @param[in] lib
      Pointer to this library object.
    @param[in] buf
      Pointer to the buffer which will back the heap.
    @param[in] size
      Size in bytes of the user buffer.
    @param[out] heap
      Pointer to the created heap object.
    @return
      Returns GTS_OK if successfull, and an error code otherwise.
    */
    GTSTATUS (GTCALL *CreateHeap)(
        IN  VFB_LIB*  lib,
        IN  void*     buf,
        IN  UINT32    size,
        OUT GT_HEAP** heap
        );

    ///////////////////////////////////////////////////////////////////////////
    // CreateVfbEngine
    /**
    Creates a VFB engine object, which contains the VFB processing functionality.
    @param[in] lib
      Pointer to this library object.
    @param[in] heap
      Pointer to a @ref GT_HEAP object which will be used for memory requests.
    @param[in] cfg
      Pointer to a @ref VFB_CONFIG structure with configuration parameters.
      If this parameter is NULL, default values will be chosen. Please see @ref VFB_CONFIG
      for more details.
    @param[out] eng
      Points to the created VFB engine object.
    @return
      Returns GTS_OK if successfull, or an error code otherwise.
    @remarks
      - If the calling application specifies an application-defined heap object
      in the <i>heap</i> paramater, the VFB engine will not own that heap object.
      This means that it won't destroy the heap by calling @ref GT_HEAP_VFT::Destroy
      on it when the VFB engine itself is destroyed, the calling application is
      responsible for destroying the heap object it created.
    */
    GTSTATUS (GTCALL *CreateVfbEngine)(
        IN  VFB_LIB*          lib,
        IN  GT_HEAP*          heap,
        IN  const VFB_CONFIG* cfg,
        OUT VFB_ENGINE**      eng
        );
};


///////////////////////////////////////////////////////////////////////////
// VfbLibGet
/**
Retrieves a pointer to the library object.
@return
  Pointer to a VFB_LIB structure which represents the library object,
  through which the library functions are accessible.
*/
GTAPI VFB_LIB* GTCALL VfbLibGet( void );


/// @} end of api_lib



#ifdef __cplusplus
}
#endif


#endif // __LIBVFB_H__
