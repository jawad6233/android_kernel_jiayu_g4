/*----------------------------------------------------

Face Detection API
Copyright (c) FotoNation
All rights reserved

----------------------------------------------------*/

#ifndef __FACE_H__
#define __FACE_H__


#ifdef  __cplusplus
extern "C" {
#endif

//
// Define the calling convention
//

#if !defined(FDAPI)
#  define FDAPI
#endif

#if !defined(FDCALL)
#  if defined(_STDCALL_SUPPORTED)
#    define FDCALL __stdcall
#  elif defined(_MSC_VER)
#    if _MSC_VER >= 800
#      define FDCALL __stdcall
#    endif
#  endif
#endif

#if !defined(FDCALL)
#  define FDCALL
#endif


#ifndef UINT8
typedef unsigned char  UINT8;
#endif
#ifndef UINT16
typedef unsigned short UINT16;
#endif
#ifndef UINT32
typedef unsigned int     UINT32;
#endif

#ifndef INT8
typedef signed char  INT8;
#endif
#ifndef INT16
typedef signed short INT16;
#endif
#ifndef INT32
typedef signed int     INT32;
#endif

///////////////////////////////////////////////////////////////////////////
/**
@weakgroup faceApi_Ver
@{

  The following is the list of API versions and the corresponding changes.
  Use ::FdVersionGet() to retrieve the current version.

@par 0x0001001E
    - Added FD_FACE::PtEyeL and FD_FACE::PtEyeR.

@par 0x0001001D
    - Added native support for FD_FMT_YUV_420SP format.

@par 0x0001001C
    - Added native support for FD_FMT_YUV_420SP_FLAT format.

@par 0x0001001B
  - Added FD_DETECTOR_CFG::framesPerLockWhenNoFaces.

@par 0x0001001A
  - Added @ref FDD_DISABLE_PROFILE_DETECTION flag.

@par 0x00010019
    - Added FD_INFO;
    - Added FdVersionGet;

@par 0x00010018
  - Added FD_DETECTOR_VFT::UpdateConfiguration()

@par 0x00010017
    - Added FD_DETECTOR_CFG::keepFacesOverMoreFrames;
    - Added FD_DETECTOR_CFG::keepLimitingWhenNoFace;

@par 0x00010016
    - Updated @ref FD_PROGRESS callback behavior, the UINT32 param parameter can specify unknown number of steps.

@par 0x00010015
    - Added @ref FDD_STATIC_SECOND_SEARCH flag.

@par 0x00010014
  - Changed type of FD_DETECTOR_CFG::boostFDvsSPEED and FD_DETECTOR_CFG::boostFDvsFP to signed.
  - Added FD_DETECTOR_CFG::minFaceSizeFeaturesDetect and FD_DETECTOR_CFG::maxFaceSizeFeaturesDetect

@par 0x00010013
  - Added FD_FACE::yawAngle
  - Added @ref FDD_DISABLE_SEMIPROFILE_DETECTION flag.

@par 0x00010012
  - Added FD_IMAGE::planes in FD_IMAGE.
  - Added FD_DETECTOR_VFT::SetMainFaceMode() and FD_DETECTOR_VFT::GetMainFace().

@par 0x00010011
  - Added FDD_DETECT_CHILDREN in FD_DETECTOR_CFG::flags.
  - Added FD_FACE::childLevel.

@par 0x00010010
  - Rename FD_DETECTOR_CFG::detectionBoost to FD_DETECTOR_CFG::boostFDvsFP;
  - Added FD_DETECTOR_CFG::boostFDvsSPEED;

@par 0x0001000F
  - Added FD_EXTRA_INFO::regions.
  - Added FD_FACE::regionID.

@par 0x0001000E
  - Added FD_DETECTOR_CFG::maxFaceCount;

@par 0x0001000D
  - Changed functionality and description for @ref FDD_ENABLE_IMAGE_ORIENTATION flag.
  - Added FD_DETECTOR_CFG::detectionBoost;

@par 0x0001000C
  - Added new @ref faceApi_Fmt "color formats":
    FD_FMT_YUV_420P_FLAT
    FD_FMT_YUV_444_FLAT

@par 0x0001000B
  - Added support for a secondary input image, with higher resolution.

@par 0x0001000A
  - Added @ref FDD_DISABLE_AUTO_ORIENTATION flag.

@par 0x00010009
  - Added FD_DETECTOR_VFT::DetectFacesEx().

@par 0x00010008
  - Added FDD_DETECT_MOUTH in FD_DETECTOR_CFG::flags.

@par 0x00010007
  - Added FD_DETECTOR_VFT::ClearFaceList().

@par 0x00010006
  - Added FDD_DETECT_EYES in FD_DETECTOR_CFG::flags.

@par 0x00010005
  - Added FDD_DETECT_BLINK in FD_DETECTOR_CFG::flags.
  - Added FD_IMAGE::hwData.
  - Added FD_FACE::blinkLevel.
  - Added FD_FACE::blinkLevelL.
  - Added FD_FACE::blinkLevelR.
  - Added FD_FACE::eyeL.
  - Added FD_FACE::eyeR.

@par 0x00010004
  - Added FD_DETECTOR_VFT::DetectBlink().

@par 0x00010003
  - Added FD_DETECTOR_VFT::GetApiVersion().
  - Added FDD_NEW_VERSION_CHECK in FD_DETECTOR_CFG::flags.
  - Added FDD_DETECT_SMILE in FD_DETECTOR_CFG::flags.
  - Added FD_FACE::structSize.
  - Added FD_FACE::smileLevel.
**************************************************************************/

///////////////////////////////////////////////////////////////////////////
// FD_INFO
/**
  Contains information about the library.
@remarks
  This structure is only created by the library and should be treated as
  read-only.
@version
  Minimal required @ref faceApi_Ver is 0x00010019.
**************************************************************************/
typedef struct
{
    /** The size, in bytes, of the ::FD_INFO structure. The field is set
    by the library. The application may check this field to decide whether it can
    access the fields added later in API. */
    UINT32 structSize;

    /** Library version number. Usually consists of four numbers:
    - Major version number,
    - Minor version number,
    - Revision number,
    - Platform build number. */
    UINT32 libVer[5];

    /** API version number (same as returned by FD_DETECTOR_VFT::GetApiVersion).
    @remarks
    - The API version is a 32-bit value where the higher 16 bits represent the
      major version and the lower 16 bits represent the minor version.
      When the minor is increased the API remains backward compatible. When
      the major is increased the API may not be backward compatible. For
      the current list of API versions see @ref faceApi_Ver.
    - This is not the library version that for example appears in Windows
      DLL version info resource. This is a separate version of API. The
      API version does not change when there are only changes in
      implementation. The API version changes when the API is modified so
      that the application needs to know if the new added features are available.
    */
    UINT32 apiVer;

    /** A bitmask specifying supported features. Can contain zero or more of
    the following:
    - FD_FEATURE_BLINK: The library supports blink detection.
    - FD_FEATURE_EYE: The library supports eye detection.
    - FD_FEATURE_SMILE: The library supports smile detection.
    */
    UINT32 features;

    /** Pointer to the string specifying build date in form "MMM DD YYYY HH:MM:SS" */
    const char *buildDate;

    /** Pointer to the optional string specifying release label. */
    const char *label;
} FD_INFO;

//
// Define library feature values
//

enum
{
    FD_FEATURE_BLINK        = (1<<0),
    FD_FEATURE_EYE          = (1<<1),
    FD_FEATURE_SMILE        = (1<<2),
    FD_FEATURE_HW           = (1<<3),
};

///////////////////////////////////////////////////////////////////////////
// FdVersionGet
/**
  Retrieves the library version information.
@return
  Pointer to an ::FD_INFO structure that describes version information.
**************************************************************************/
FDAPI
const FD_INFO*
FDCALL
FdVersionGet(
    void
    );

/** @} end of faceApi_Ver */

////////////////////////////////////////////////////////////////////
// Note:
// the following types must be defined prior to including this header file:
// INT32 and UINT32

typedef INT32 FD_BOOL;

///////////////////////////////////////////////////////////////////////////
// Status Codes
/** @weakgroup faceApi_Status
  The following constants indicate the result of an API call.
@{
**************************************************************************/

///////////////////////////////////////////////////////////////////////////
// FDSTATUS
/**
  Type to return the status of execution of a function.
  For the possible values of status see @ref faceApi_Status "FDS_XXX codes".
**************************************************************************/
typedef INT32 FDSTATUS;

/// No errors.
#define FDS_OK                      0

/// New version available. See FdDetectorCreate().
#define FDS_NEW_VERSION             ((FDSTATUS)1)

/// Unspecified error.
#define FDS_FAIL                    ((FDSTATUS)-1)

/// Operation was canceled. See ::FD_PROGRESS callback function.
#define FDS_CANCELLED               ((FDSTATUS)-2)

/// There's not enough system resources.
#define FDS_NO_SYSTEM_RESOURCES     ((FDSTATUS)-3)

/// Indicates that one of the arguments passed to the method was not valid.
#define FDS_INVALID_ARG             ((FDSTATUS)-4)

/// No face has been found.
#define FDS_NOT_FOUND               ((FDSTATUS)-5)

/// Feature not supported.
#define FDS_NOT_SUPPORTED           ((FDSTATUS)-6)

/** @} end of faceApi_Status */

////////////////////////////////////////////////////////////////////
// Defines
////////////////////////////////////////////////////////////////////

#define IN
#define OUT
#define OPTIONAL

///////////////////////////////////////////////////////////////////////////
// Angles ( should be in the interval [-179, 180] )

#define FD_ANGLE_0       0
#define FD_ANGLE_30     30
#define FD_ANGLE_45     45
#define FD_ANGLE_60     60
#define FD_ANGLE_90     90
#define FD_ANGLE_120   120
#define FD_ANGLE_135   135
#define FD_ANGLE_150   150
#define FD_ANGLE_180   180
#define FD_ANGLE_210  -150
#define FD_ANGLE_225  -135
#define FD_ANGLE_240  -120
#define FD_ANGLE_270   -90
#define FD_ANGLE_300   -60
#define FD_ANGLE_315   -45
#define FD_ANGLE_330   -30
#define FD_ANGLE_END   0x1FFF  // Note: The angle array should be finished with FD_ANGLE_END.
#define FD_ANGLE_UNKNOWN 0x1FFF

///////////////////////////////////////////////////////////////////////////
// Image format constants

/** @weakgroup faceApi_Fmt
  The following constants specify various image formats used in the API.
@{ */
enum {

    // unknown format - shouldn't be used
    FD_FMT_UNKNOWN = 0,

    /**
      Grayscale - the format is 8 bits per pixel; 8 bits are used for luminance (Y) component.
      <table> <tr>
        <td>Y<sub>0</sub></td>
        <td>Y<sub>1</sub></td>
        <td>Y<sub>2</sub></td>
        <td>...</td>
      </tr> </table> */
    FD_FMT_GRAY8 = 1,

    /**
      YUV 4:2:2 - the format is 32 bits per two consecutive pixels;
      8 bits each are used for luminance (Y) and chrominance (U,V) components.
      <table> <tr>
        <td>Y<sub>0</sub></td>
        <td>U<sub>01</sub></td>
        <td>Y<sub>1</sub></td>
        <td>V<sub>01</sub></td>
        <td>Y<sub>2</sub></td>
        <td>U<sub>23</sub></td>
        <td>Y<sub>3</sub></td>
        <td>V<sub>23</sub></td>
        <td>...</td>
      </tr> </table> */
    FD_FMT_YUYV_422 = 2,

    /**
      24 bit RGB - the format is 24 bits per pixel; 8 bits each are used
      for the red, green, and blue components.
      <table> <tr>
        <td>B<sub>0</sub></td>
        <td>G<sub>0</sub></td>
        <td>R<sub>0</sub></td>
        <td>B<sub>1</sub></td>
        <td>G<sub>1</sub></td>
        <td>R<sub>1</sub></td>
        <td>...</td>
      </tr> </table> */
    FD_FMT_BGR24 = 3,

    /**
      YUV 4:2:2 - the format is 32 bits per two consecutive pixels;
      8 bits each are used for luminance (Y) and chrominance (U,V) components.
      <table> <tr>
        <td>U<sub>01</sub></td>
        <td>Y<sub>0</sub></td>
        <td>V<sub>01</sub></td>
        <td>Y<sub>1</sub></td>
        <td>U<sub>23</sub></td>
        <td>Y<sub>2</sub></td>
        <td>V<sub>23</sub></td>
        <td>Y<sub>3</sub></td>
        <td>...</td>
      </tr> </table> */
    FD_FMT_UYVY_422 = 4,

    /**
      32 bit RGB - the format is 32 bits per pixel; 8 bits each are used
      for the red, green, and blue components. The remaining 8 bits are not used.
      <table> <tr>
        <td>B<sub>0</sub></td>
        <td>G<sub>0</sub></td>
        <td>R<sub>0</sub></td>
        <td>unused</td>
        <td>B<sub>1</sub></td>
        <td>G<sub>1</sub></td>
        <td>R<sub>1</sub></td>
        <td>unused</td>
        <td>...</td>
      </tr> </table> */
    FD_FMT_BGR32 = 5,

    /**
      24 bit RGB - the format is 24 bits per pixel; 8 bits each are used
      for the red, green, and blue components.
      <table> <tr>
        <td>R<sub>0</sub></td>
        <td>G<sub>0</sub></td>
        <td>B<sub>0</sub></td>
        <td>R<sub>1</sub></td>
        <td>G<sub>1</sub></td>
        <td>B<sub>1</sub></td>
        <td>...</td>
      </tr> </table> */
    FD_FMT_RGB24 = 6,

    /**
      Planar YUV 4:2:2 - the luminance (Y) and chrominance (U,V) components
      are stored in two separate 8-bit sample planes.
      <table> <tr>
        <td>Y<sub>0</sub></td>
        <td>Y<sub>1</sub></td>
        <td>Y<sub>2</sub></td>
        <td>Y<sub>3</sub></td>
        <td>...</td>
        <td>Y<sub>n-2</sub></td>
        <td>Y<sub>n-1</sub></td>
      </tr> <tr>
        <td>U<sub>0</sub></td>
        <td>V<sub>0</sub></td>
        <td>U<sub>1</sub></td>
        <td>V<sub>1</sub></td>
        <td>...</td>
        <td>U<sub>n/2-1</sub></td>
        <td>V<sub>n/2-1</sub></td>
      </tr> </table> */
    FD_FMT_YUV_422P = 7,

    /**
      Planar YUV 4:2:2 - the luminance (Y) and chrominance (U,V) components
      are stored in three separate 8-bit sample planes.
      <table> <tr>
        <td>Y<sub>0</sub></td>
        <td>Y<sub>1</sub></td>
        <td>Y<sub>2</sub></td>
        <td>Y<sub>3</sub></td>
        <td>...</td>
        <td>Y<sub>n-1</sub></td>
      </tr> <tr>
        <td>U<sub>0</sub></td>
        <td>U<sub>1</sub></td>
        <td>U<sub>2</sub></td>
        <td>U<sub>3</sub></td>
        <td>...</td>
        <td>U<sub>n/2-1</sub></td>
      </tr> <tr>
        <td>V<sub>0</sub></td>
        <td>V<sub>1</sub></td>
        <td>V<sub>2</sub></td>
        <td>V<sub>3</sub></td>
        <td>...</td>
        <td>V<sub>n/2-1</sub></td>
      </tr> </table> */
    FD_FMT_YUV_422P2 = 8,

    /**
      Planar YUV 4:2:0 - the luminance (Y) and chrominance (U,V) components
      are stored in three separate 8-bit sample planes. First are stored
      all Y values, then U, then V. The chrominance components are averaged
      for 2x2 pixels. For example, an 4x4 image is stored as:
      @verbatim
              Y                  U                  V
      +---+---+---+---+  +-------+-------+  +-------+-------+
      |0,0|0,1|0,2|0,3|  |       |       |  |       |       |
      |---+---+---+---+  |  0,0  |  0,1  |  |  0,0  |  0,1  |
      |1,0|1,1|1,2|1,3|  |       |       |  |       |       |
      +---+---+---+---+  +-------+-------+  +-------+-------+
      |2,0|2,1|2,2|2,3|  |       |       |  |       |       |
      +---+---+---+---+  |  1,0  |  1,1  |  |  1,0  |  1,1  |
      |3,0|3,1|3,2|3,3|  |       |       |  |       |       |
      +---+---+---+---+  +-------+-------+  +-------+-------+
      @endverbatim

      The input image data field must contain an array of 3 pointers to the
      corresponding 8-bit sample planes

      */
    FD_FMT_YUV_420P = 9,

    /**
      YUV 4:4:4 - the format is 24 bits per pixel; 8 bits each are used
      for the luminance(Y) and chrominance(U,V) components.
      <table> <tr>
        <td>Y<sub>0</sub></td>
        <td>U<sub>0</sub></td>
        <td>V<sub>0</sub></td>
        <td>Y<sub>1</sub></td>
        <td>U<sub>1</sub></td>
        <td>V<sub>1</sub></td>
        <td>...</td>
      </tr> </table> */
    FD_FMT_YUV_444 = 10,

    /**
      Planar YUV 4:2:0 - the luminance (Y) and chrominance (U,V) components
      are stored in three separate 8-bit sample planes. First are stored
      all Y values, then U, then V. The chrominance components are averaged
      for 2x2 pixels. For example, an 4x4 image is stored as:
      @verbatim
              Y                  U                  V
      +---+---+---+---+  +-------+-------+  +-------+-------+
      |0,0|0,1|0,2|0,3|  |       |       |  |       |       |
      |---+---+---+---+  |  0,0  |  0,1  |  |  0,0  |  0,1  |
      |1,0|1,1|1,2|1,3|  |       |       |  |       |       |
      +---+---+---+---+  +-------+-------+  +-------+-------+
      |2,0|2,1|2,2|2,3|  |       |       |  |       |       |
      +---+---+---+---+  |  1,0  |  1,1  |  |  1,0  |  1,1  |
      |3,0|3,1|3,2|3,3|  |       |       |  |       |       |
      +---+---+---+---+  +-------+-------+  +-------+-------+
      @endverbatim

      The input image data field must contain all three 8-bit sample planes
      arranged in the memory with no gap between them.

      @version
        Minimal required @ref faceApi_Ver is 0x00010005.
    */
    FD_FMT_YUV_420P_FLAT = 11,

    /**
      YUV 4:4:4 - the format is 24 bits per pixel; 8 bits each are used
      for the luminance(Y) and chrominance(U,V) components. No gap or
      padding bytes are allowed between pixels.
      <table> <tr>
        <td>Y<sub>0</sub></td>
        <td>U<sub>0</sub></td>
        <td>V<sub>0</sub></td>
        <td>Y<sub>1</sub></td>
        <td>U<sub>1</sub></td>
        <td>V<sub>1</sub></td>
        <td>...</td>
      </tr> </table>

    @version
      Minimal required @ref faceApi_Ver is 0x00010005.
    */
    FD_FMT_YUV_444_FLAT = 12,

    /**
      YUV 4:2:2 - the format is 32 bits per two consecutive pixels;
      8 bits each are used for luminance (Y) and chrominance (U,V) components.
      Please note that Y values are swapped.
      <table> <tr>
        <td>Y<sub>1</sub></td>
        <td>V<sub>01</sub></td>
        <td>Y<sub>0</sub></td>
        <td>U<sub>01</sub></td>
        <td>Y<sub>3</sub></td>
        <td>V<sub>23</sub></td>
        <td>Y<sub>2</sub></td>
        <td>U<sub>23</sub></td>
        <td>...</td>
      </tr> </table> */
    FD_FMT_YVYU_422 = 13,

    /**
      Semi-Planar YUV 4:2:0 FLAT- the luminance (Y) and chrominance (U,V) components
      are stored in two separate 8-bit sample planes. First are stored
      all Y values, then U-V pairs. The chrominance components are averaged
      for 2x2 pixels.
      The input image data field must contain both 8-bit sample planes 
      arranged in the memory with no gap between them.
    @version
      Minimal required @ref faceApi_Ver is 0x0001001C.
    */
    FD_FMT_YUV_420SP_FLAT = 14,


    /**
      Semi-Planar YUV 4:2:0 - the luminance (Y) and chrominance (U,V) components
      are stored in two separate 8-bit sample planes. The first plane stores
      all Y values, and the second stores U-V pairs.
      The chrominance components are averaged
      for 2x2 pixels.
    @version
      Minimal required @ref faceApi_Ver is 0x0001001D.
    */
    FD_FMT_YUV_420SP = 15,

    /**
      Semi-Planar YVU 4:2:0 FLAT- the luminance (Y) and chrominance (V,U) components
      are stored in two separate 8-bit sample planes. First are stored
      all Y values, then V-U pairs. The chrominance components are averaged
      for 2x2 pixels.

      @note
        If FD_IMAGE::data is used to specify the image buffers, then it must point to
        an buffer that contain the two 8-bit sample planes arranged in the
        memory with no gap between the lines or the planes.

    @version
      Minimal required @ref faceApi_Ver is 0x0001001E.
    */
    FD_FMT_YVU_420SP_FLAT = 16,  //xiaoyh

    /**
      Semi-Planar YVU 4:2:0 - the luminance (Y) and chrominance (V,U) components
      are stored in two separate 8-bit sample planes. The first plane stores
      all Y values, and the second stores V-U pairs.
      The chrominance components are averaged for 2x2 pixels.

      @note
        If FD_IMAGE::data is used to specify the image buffers, then it must
        point to an array of 2 pointers for the two used planes.

    @version
      Minimal required @ref faceApi_Ver is 0x0001001E.
    */
    FD_FMT_YVU_420SP = 17,   //xiaoyh

    FD_FMT_MAX
};
/** @} end of faceApi_Fmt */

///////////////////////////////////////////////////////////////////////////
// Main Face Modes

enum {
    FD_MAIN_FACE_MODE_DEFAULT = 0,
    FD_MAIN_FACE_MODE_ADULT = 1,
    FD_MAIN_FACE_MODE_CHILD = 2,
    FD_MAIN_FACE_MODE_MAX
};

///////////////////////////////////////////////////////////////////////////
// Heap interface
/**
@weakgroup faceApi_Heap
  An application can provide memory management for the library by means of heap object.
  The heap object is represented with FD_HEAP structure.

  An application can create heap object in two ways:
  - Supply work buffer to ::FdHeapCreate function which creates heap object.
  - Create heap object by calling the primitives provided by the platform memory management system.
  The following code shows how to do it:
  @code
    static void FDCALL heap_destroy( FD_HEAP* heap )
    {
      free( heap );
    }
    static void* FDCALL heap_alloc( FD_HEAP* heap, UINT32 size )
    {
      return malloc( size );
    }
    static void FDCALL heap_free( FD_HEAP* heap, void* mem )
    {
      free( mem );
    }
    // ...

    static const struct FD_HEAP_VFT vft = {
      heap_destroy,
      heap_alloc,
      heap_free
    };
    FD_HEAP *heap = (FD_HEAP*) malloc( sizeof(FD_HEAP) );
    heap->vft = vft;
    // ...
  @endcode
@{
**************************************************************************/

///////////////////////////////////////////////////////////////////////////
// FD_HEAP
/**
  Represents heap interface.
**************************************************************************/
typedef struct FD_HEAP
{
    /** Pointer to function table. */
    struct FD_HEAP_VFT *vft;
    // private part follows...
} FD_HEAP;

///////////////////////////////////////////////////////////////////////////
// FD_HEAP_VFT
/**
  Contains pointers to FD_HEAP functions.
**************************************************************************/
struct FD_HEAP_VFT
{
    ///////////////////////////////////////////////////////////////////////////
    // Destroy
    /**
      Destroys the specified heap object.
    @param[in] heap
      Pointer to heap to be destroyed.
    */
    void (FDCALL *Destroy)(
        IN FD_HEAP* heap
        );

    ///////////////////////////////////////////////////////////////////////////
    // Alloc
    /**
      Allocates a block of memory from a heap.
    @param[in] heap
      Pointer to heap from which the memory is allocated.
    @param[in] size
      Number of bytes to be allocated.
    @return
      If the function succeeds, the return value is a pointer to the allocated
      memory block. If the function fails, the return value is NULL.
    */
    void* (FDCALL *Alloc)(
        IN FD_HEAP* heap,
        IN UINT32 size
        );

    ///////////////////////////////////////////////////////////////////////////
    // Free
    /**
      Frees a memory block allocated from a heap by the ::Alloc function.
    @param[in] heap
      Pointer to heap whose memory block is to be freed.
    @param[in] mem
      Pointer to the memory block to be freed. This pointer is returned by the
      ::Alloc function.
    */
    void (FDCALL *Free)(
        IN FD_HEAP* heap,
        IN void* mem
        );
};

///////////////////////////////////////////////////////////////////////////
// FdHeapCreate
/**
  Creates a heap object from the input buffer.
@param[in] buffer
  Pointer to a buffer that shall be used as a heap.
@param[in] size
  Size of the buffer, in bytes.
@param[out] heap
  A pointer to a variable supplied by the caller. On success it contains the
  pointer to the newly created heap.
@return
  If the function succeeds, the return value is FDS_OK.
@remarks
  This function is intended for platforms without own memory management. It
  uses provided buffer to create heap that can be supplied to ::FdDetectorCreate
  function.
**************************************************************************/
FDAPI
FDSTATUS
FDCALL
FdHeapCreate(
    IN void *buffer,
    IN UINT32 size,
    OUT FD_HEAP** heap
    );

/** @} end of faceApi_Heap */

///////////////////////////////////////////////////////////////////////////
// Face detector interface
/**
@weakgroup faceApi_Det
  The face detector object is the root object in the API. It provides the
  following functionality:
  - Static face detection.
  - Moving face detection.
  - Face enumeration.

  The face detector object is represented with FD_DETECTOR structure.
  The following code shows how to use the detector:
  <P><B>Static mode:</B></P>
  @code
    FD_DETECTOR *det = NULL;

    {//detector initialization
        FD_DETECTOR_CFG cfg;
        memset(&cfg, 0, sizeof(FD_DETECTOR_CFG));
        cfg.structSize = sizeof(FD_DETECTOR_CFG);

        cfg.flags = FDD_COLOR_FILTER;
        cfg.centralLockAreaPercentageW = 100;
        cfg.centralLockAreaPercentageH = 90;

        if( FdDetectorCreate( NULL, &cfg, &det ) != FDS_OK )
        {
            //failed to create detector, add error handling code
            // ...
        }
    }//end of detector initialization


    //update image structure here
    // ...

    det->vft->DetectFaces( det, image, NULL, NULL );
    //...

    //destroy detector
    det->vft->Destroy( det );

  @endcode

  <P><B>Tracking mode:</B></P>
  @code
    FD_DETECTOR *det = NULL;

    {//detector initialization
        FD_DETECTOR_CFG cfg;
        memset(&cfg, 0, sizeof(FD_DETECTOR_CFG));
        cfg.structSize = sizeof(FD_DETECTOR_CFG);

        cfg.flags = FDD_TRACKING_MODE | FDD_COLOR_FILTER | FDD_FAST_LOCK | FDD_LIMIT_FACE_SIZE | FDD_LIMIT_FACE_ANGLE;
        cfg.centralLockAreaPercentageW = 85;
        cfg.centralLockAreaPercentageH = 80;

        cfg.framesPerLock = 5;
        cfg.smoothing = 50;

        if( FdDetectorCreate( NULL, &cfg, &det ) != FDS_OK )
        {
            //failed to create detector, add error handling code
            // ...
        }
    }//end of detector initialization


    {//tracking loop
        //update image structure here
        // ...

        det->vft->DetectFaces( det, image, NULL, NULL );
        //...
    }

    //destroy detector
    det->vft->Destroy( det );

  @endcode
@{
**************************************************************************/

///////////////////////////////////////////////////////////////////////////
// FD_DETECTOR
/**
  Represents face detector interface.
**************************************************************************/
typedef struct FD_DETECTOR
{
    /** Pointer to function table. */
    struct FD_DETECTOR_VFT *vft;
    // private part follows...
} FD_DETECTOR;

///////////////////////////////////////////////////////////////////////////
// FD_IMAGE_PLANE
/**
Contains information about an image data plane.
**************************************************************************/
typedef struct FD_IMAGE_PLANE
{
    /** Offset, in bytes, between consecutive scan lines of the image plane. */
    INT32   stride;
    /** Pointer to the first scan line of the image plane.*/
    void*   data;
} FD_IMAGE_PLANE;

///////////////////////////////////////////////////////////////////////////
// FD_IMAGE
/**
  Contains information about the raster image.
**************************************************************************/
typedef struct
{
    /** The size, in bytes, of the ::FD_IMAGE structure. Set this member to
    <code>sizeof (FD_IMAGE)</code>. Be sure to set this member before calling any
    function that uses this structure. */
    UINT32  structSize;

    /** Number of pixels in one scan line of the image. */
    UINT32  width;

    /** Number of scan lines in the image. */
    UINT32  height;

    /** Offset, in bytes, between consecutive scan lines of the image. */
    INT32   stride;

    /** Integer that specifies the pixel format of the image.
    For more information see @ref faceApi_Fmt */
    UINT32  format;

    /** Pointer to the first scan line of the image. In case of planar format it is a
    pointer to an array of pointers to separate planes. Needs to be set to NULL when
    FD_IMAGE::planes is used (FD_IMAGE::planes allow more flexible memory planes
    arrangements) */
    void*   data;

    /** Specifies the image orientation angle in degrees [-179, 180].
    A positive value means that the image is rotated clockwise.
    This field is used only if FDD_ENABLE_IMAGE_ORIENTATION is set in
    FD_DETECTOR_CFG::flags, meaning that the application can tell
    the library the current orientation of the scene.
    @par See also:
      - @ref faceApi_Roll "Roll Angle".
      - @ref faceApi_Sensor "Orientation Sensor".
    */
    INT32   imageOrientation;

    /** Optional pointer to hardware data. It must be specified
    when hardware acceleration is used, otherwise it must be NULL.
    @version
      Minimal required @ref faceApi_Ver is 0x00010005.
    */
    void*   hwData;

    /** Image data for planar image formats with independent strides on each plane.
    Set FD_IMAGE::data to NULL when using this field.
    @version
      Minimal required @ref faceApi_Ver is 0x00010012.
    */
    FD_IMAGE_PLANE planes[3];

} FD_IMAGE;

///////////////////////////////////////////////////////////////////////////
// FD_RECT
/**
  Defines the coordinates of the upper-left corner, width and height of a rectangle.
**************************************************************************/
typedef struct
{
    /** Specifies the x-coordinate of the upper-left corner of the rectangle. */
    INT32  left;
    /** Specifies the y-coordinate of the upper-left corner of the rectangle. */
    INT32  top;
    /** Specifies the width of the rectangle. */
    UINT32 width;
    /** Specifies the height of the rectangle. */
    UINT32 height;
} FD_RECT;

///////////////////////////////////////////////////////////////////////////
// FD_POINT
/**
  Defines the coordinates of the point (x,y).
**************************************************************************/
typedef struct
{
    /** Specifies the x-coordinate point. */
    INT32  x;
    /** Specifies the y-coordinate point. */
    INT32  y;
} FD_POINT;

///////////////////////////////////////////////////////////////////////////
// FD_FACE
/**
  Contains information about the face.
@remarks
  This structure is only created by the library and should be treated as
  semi-opaque and read-only. The structure contains private data located
  right after the last member.
**************************************************************************/
typedef struct
{
    /** Specifies the bounding rectangle of a face. */
    FD_RECT boundRect;

    /** The face @ref faceApi_Roll "roll angle".
    */
    INT32   angle;

    /** A numerical parameter describing the confidence attributed
    by the detection algorithm to each face. This value is not normalized
    over a statistical collection of images, thus it is not recommended
    in FP filtering (by using a fixed threshold) or comparing faces from
    different images, different lighting or pose conditions. Its value is bigger
    for more easily detectable faces. Example use cases:
    - compare faces within the same image/frame in order to determine which one
      was the easiest to detect(less occluded, most focused/good contrast, closest
      to the main orientations from 'lockAngles') and has the best output
      rectangle fit/centering.
    - use it in a sequence of frames with the same person, where 'confidence' can
      indicate the "best" instance for the face (best rectangle fit, contrast,
      less occlusion, etc).
    */
    UINT32  confidence;

    /** Indicates if the face was tracked or detected in the current frame.
      - FALSE - means that the presence of the face is confirmed by a more
        precise detection algorithm. In this case the confidence of the
        detection is specified in FD_FACE::confidence.
      - TRUE - means that the presence of the face is confirmed by a less
        precise tracking algorithm (color matching, etc). In this case
        the value of FD_FACE::boundRect may vary more from the actual face.
    */
    FD_BOOL isTracked;

    /** The unique ID for the tracked face. If the algorithm considers that
    this is the same face as previously detected or tracked then the same
    ID is assigned. If the face is lost then its ID is not reused. */
    UINT32  nTrackedFaceID;

    /** The size, in bytes, of the FD_FACE structure. This field is set by the library.
    The application may check this field to decide whether it can access the fields
    added later in API.
    @version
      Minimal required @ref faceApi_Ver is 0x00010003.
    */
    UINT32 structSize;

    /** Indicates the presence of a smile. Following values are used:
      - (-1) - no smile.
      - (0) - not able to detect.
      - (1 ... 100) - the higher is the value the more confidence is
        that a smile is present.
      - a 50 threshold is recommended for a trade-off between detection rate
        and false positives rate.
      .
      In order the smile to be detected the library must be compiled with
      corresponding support and FDD_DETECT_SMILE flag must be set in   
      FD_DETECTOR_CFG::flags.
    @version
      Minimal required @ref faceApi_Ver is 0x00010003.
    */
    INT32 smileLevel;

    /** Indicates the presence of a blink. Following values are used:
      - (-1) - no blink.
      - (0) - not able to detect.
      - (1 ... 100) - the higher is the value the more confidence is
        that a blink is present.
      - a 50 threshold is recommended for a trade-off between detection rate
        and false positives rate.
      .
      In order the blink to be detected the library must be compiled with
      corresponding support and FDD_DETECT_BLINK flag must be set in
      FD_DETECTOR_CFG::flags.
    @version
      Minimal required @ref faceApi_Ver is 0x00010005.
    */
    INT32  blinkLevel;

    /** Same meaning as for FD_FACE::blinkLevel but only
      for the left eye.
    @version
      Minimal required @ref faceApi_Ver is 0x00010005.
    */
    INT32  blinkLevelL;

    /** Same meaning as for FD_FACE::blinkLevel but only
      for the right eye.
    @version
      Minimal required @ref faceApi_Ver is 0x00010005.
    */
    INT32  blinkLevelR;

    /** Specifies the bounding rectangle of the left eye.
      To detect eyes FDD_DETECT_EYES or FDD_DETECT_BLINK must be set
      in FD_DETECTOR_CFG::flags.
    @version
      Minimal required @ref faceApi_Ver is 0x00010005.
    */
    FD_RECT eyeL;

    /** Same meaning as for FD_FACE::eyeL, but for the right eye.
    @version
      Minimal required @ref faceApi_Ver is 0x00010005.
    */
    FD_RECT eyeR;

    /** Specifies the bounding rectangle of the mouth.
      To detect mouth FDD_DETECT_MOUTH must be set
      in FD_DETECTOR_CFG::flags.
    @version
      Minimal required @ref faceApi_Ver is 0x00010008.
    */
    FD_RECT mouth;

    /** Specifies the region ID.
      If zero then it is not specified, otherwise it corresponds to the
      value previously set in FD_REGION::id. It means that the face belongs
      to the specified region.
    @version
      Minimal required @ref faceApi_Ver is 0x0001000F.
    */
    UINT32 regionID;

    /** Indicates that the face detected is a child:
      - (-1) - not a child.
      - (0) - not able to detect.
      - (1 ... 100) - the higher is the value the more confidence is
        that the detected face is a child.
      - a 50 threshold is recommended for a trade-off between detection rate
        and false positives rate.
      .
      In order the children faces to be classified the library must be compiled with
      corresponding support and FDD_DETECT_CHILDREN flag must be set in
      FD_DETECTOR_CFG::flags.
    @version
      Minimal required @ref faceApi_Ver is 0x00010011.
    */
    INT32 childLevel;

    /** The @ref faceApi_Yaw "yaw angle".
        - Minimal required @ref faceApi_Ver is 0x00010013.
    */
    INT32 yawAngle;

    /** Specifies the x,y coordinates of the left eye, in fixed-point 
      arithmetics (10 bit precision).
      To detect eyes FDD_DETECT_EYES or FDD_DETECT_BLINK must be set
      in FD_DETECTOR_CFG::flags.
    @version
      Minimal required @ref faceApi_Ver is 0x0001001E.
    */
    FD_POINT PtEyeL;

    /** Specifies the x,y coordinates of the right eye, in fixed-point 
      arithmetics (10 bit precision).
      To detect eyes FDD_DETECT_EYES or FDD_DETECT_BLINK must be set
      in FD_DETECTOR_CFG::flags.
    @version
      Minimal required @ref faceApi_Ver is 0x0001001E.
    */
    FD_POINT PtEyeR;

    // private part follows...
} FD_FACE;

enum
{
    FD_REGION_TYPE_MASK     = 0x000F,
    FD_REGION_TYPE_NONE     = 0,
    FD_REGION_TYPE_FACE     = 1,
    FD_REGION_TRACK_OUTSIDE = (1 << 8)
};

///////////////////////////////////////////////////////////////////////////
// FD_REGION
/**
  Specifies an image region where the detection must be performed.
@remarks
  Used in FD_EXTRA_INFO when calling FD_DETECTOR_VFT::DetectFacesEx().
@sa
  - @ref faceApi_Regions "Detection on Image Regions".
**************************************************************************/
typedef struct FD_REGION
{
    /** The size, in bytes, of this  structure. Be sure to set this member
        to sizeof(FD_REGION) before calling any function that uses this
        structure.
    */
    UINT32 structSize;

    /** The pointer to the next region. Set to NULL if this is the last
        region in the list.
    */
    struct FD_REGION* next;

    /** Specifies the bounding rectangle of the region. The bounding rectangle
        provides the position and size of the region in the input image
        that is passed to FD_DETECTOR_VFT::DetectFacesEx().
    */
    FD_RECT boundRect;

    /** Region options bit mask. All other bits that are not specified here must be
        zero. The valid options are:

        - FD_REGION_TYPE_XXX values:
          One of the value that specifies the type of region. Only one type can be
          used a time:
          - FD_REGION_TYPE_NONE: The region must be ignored. Used to specify a
            non NULL list without regions.
          - FD_REGION_TYPE_FACE: The region is a face. The library assumes that
            a face is fitted in the region and boundRect approximately specifies
            the position and size.

        - FD_REGION_TRACK_OUTSIDE:
          The library may continue to track the face outside the specified region.

    */
    UINT32 options;

    /** The @ref faceApi_Roll "roll angle". If not known, shall be set to
        FD_ANGLE_UNKNOWN.
    */
    INT32 angle;

    /** The region ID. The library sets this value in FD_FACE::regionID,
        thus it is possible to match a reported face with the region.
        0 - means not specified.
    */
    UINT32 id;

    /** Specifies a ratio between the size of the region and distance
        the eyes.

        - Used when FD_REGION_TYPE_FACE is specified in FD_REGION::options.
        - Set to zero if not specified.
        - Calculated by the formula:
          @verbatim
            eyeFactor = (boundRect.width*256)/eyeDistance;
            where:
              eyeDistance - expected distance in pixels between center of
              the eyes.
          @endverbatim
        - This field provides a clue to the library on how the face is
          fitted in the region.
    */
    UINT32 eyeFactor;
}FD_REGION;

///////////////////////////////////////////////////////////////////////////
// FD_EXTRA_INFO
/**
  Contains additional information passed to the detector.
@remarks
  This structure is passed to the FD_DETECTOR_VFT::DetectFacesEx().
**************************************************************************/
typedef struct
{
    /** The size, in bytes, of this  structure. Be sure to set this member
        to sizeof(FD_EXTRA_INFO) before calling any function that uses this
        structure.
    */
    UINT32 structSize;

    /** The duration in milliseconds of the previous FD_DETECTOR_VFT::DetectFacesEx() call.
        Set to zero if not used.
        See @ref faceApi_AdaptiveFPS "Adaptive Processing".
    */
    UINT32 msPreviousFrame;

    /** The available time in milliseconds for FD_DETECTOR_VFT::DetectFacesEx() call.
        Set to zero if not used.
        See @ref faceApi_AdaptiveFPS "Adaptive Processing".
    */
    UINT32 msAvailable;

    /** Optional hight resolution input image (not supported).
        Set to NULL if not used.
      - Minimal required @ref faceApi_Ver is 0x0001000B.
    */
    FD_IMAGE* hiResImage;

    /** Optional list of regions.
      - Set to NULL if not used (default).
      - If not NULL it specifies a list of rectangular regions where the detection
        (locking) occurs.
      - To specify multiple regions, use FD_REGION::next field to create a
        single-linked list. FD_REGION::next must be NULL for last item.
      - If the regions are specified the library does not try to detect new faces
        outside the regions.
      - To specify an empty list, provide one item with FD_REGION::options set to
        FD_REGION_TYPE_NONE. When doing so, the library does not detect new faces.
        In contrast, when setting regions field to NULL, the library detects
        new faces in the entire input image.
      - Minimal required @ref faceApi_Ver is 0x0001000F.
      .
      @sa
        - @ref faceApi_Regions "Detection on Image Regions".
    */
    FD_REGION* regions;

} FD_EXTRA_INFO;

//
// Define configuration flag values
//

enum
{
    FDD_TRACKING_MODE                        = (1 << 0),
    FDD_COLOR_FILTER                         = (1 << 1),
    FDD_FAST_LOCK                            = (1 << 2),
    FDD_DISABLE_FACE_CONFIRMATION            = (1 << 3),
    FDD_DISABLE_SQUARE_OUTPUT                = (1 << 4),
    FDD_ENABLE_IMAGE_ORIENTATION             = (1 << 5),
    FDD_LIMIT_FACE_SIZE                      = (1 << 6),
    FDD_LIMIT_FACE_ANGLE                     = (1 << 7),
    FDD_NEW_VERSION_CHECK                    = (1 << 8),
    FDD_DETECT_SMILE                         = (1 << 9),
    FDD_DETECT_BLINK                         = (1 <<10),
    FDD_DETECT_EYES                          = (1 <<11),
    FDD_DETECT_MOUTH                         = (1 <<12),
    FDD_DISABLE_AUTO_ORIENTATION             = (1 <<13),
    FDD_DETECT_CHILDREN                      = (1 <<14),
    FDD_DISABLE_SEMIPROFILE_DETECTION        = (1 <<15),
    FDD_STATIC_SECOND_SEARCH                 = (1 <<16),
    FDD_DISABLE_PROFILE_DETECTION            = (1 <<17)
};

///////////////////////////////////////////////////////////////////////////
// FD_DETECTOR_CFG
/**
  Contains detector configuration used when calling FdDetectorCreate().
  See also @ref faceApi_Tuning "Tuning Guide".
**************************************************************************/
typedef struct
{
    /** The size, in bytes, of the ::FD_DETECTOR_CFG structure. Set this member to
    <code>sizeof (FD_DETECTOR_CFG)</code>. Be sure to set this member before calling any
    function that uses this structure. */
    UINT32  structSize;

    /** Configuration flags. All other bits that are not specified here must be
    zero. The valid flags are:

    - FDD_TRACKING_MODE: @anchor FDD_TRACKING_MODE
      If set, the library performs face tracking, treating each image
      as part of time sequence (video frame). Otherwise the library
      performs face detection on each static image independently. See
      @ref faceApi_Modes "Still and Tracking Modes".

    - FDD_COLOR_FILTER: @anchor FDD_COLOR_FILTER
      If set (<B>recommended</B>), the library uses the color filters to reject some false
      positives.

    - FDD_FAST_LOCK: @anchor FDD_FAST_LOCK
      If set (<B>recommended</B>) then:
        - When FD_DETECTOR_CFG::framesPerLock > 1, if the library does not have any face in the list, it tries to reduce
        the number of frames for locking new faces to the value set by FD_DETECTOR_CFG::framesPerLockWhenNoFaces.
        This scans more aggresivelly and increases the chances to find a face sooner (in less calls), but requires more processing power.
        - In case the library found a new face (even if there are already other faces locked),
        it performs a more aggressive face locking on the next frame, having more chances to find additional
        faces sooner (in less calls), with the penalty of incresed processing power.
         Otherwise, if no new face is detected in the last few frames, and there are faces locked, the library spends for locking as many frames as specified
        in FD_DETECTOR_CFG::framesPerLock (to decrease the processing time).
      Considerations if the flag is set:
        - Depending on platform speed this may actually take longer than the
          usual frame rate.
        - Every time a new face is locked there is a "spike" in execution time,
          usually significantly bigger than the average execution time.

    - FDD_DISABLE_FACE_CONFIRMATION: @anchor FDD_DISABLE_FACE_CONFIRMATION
      - If not set then after the library locks a face, it is
        reported to the application at the next frame only and only if
        the frame is confirmed by the tacking process. This ensures less
        FPs (false faces) but delays the locking of a face by at least one frame.
      - If set then the face is reported immediately after the locking is
        complete.

    - FDD_DISABLE_SQUARE_OUTPUT: @anchor FDD_DISABLE_SQUARE_OUTPUT
      If set, the library does not adjust the reported face rectangle to a
      square.

    - FDD_ENABLE_IMAGE_ORIENTATION: @anchor FDD_ENABLE_IMAGE_ORIENTATION
      - If set, the library takes into account FD_IMAGE::imageOrientation,
        indicating that the application can tell the library the current orientation
        of the scene.
      - The library uses this orientation for faster locking of faces.
      - The values in FD_DETECTOR_CFG::lockAngles are used relative to the angle
        specified in FD_IMAGE::imageOrientation. By default, when this flag is set,
        and lockAngles is {0,  30, 0, -30, FD_ANGLE_END}, then for
        FD_IMAGE::imageOrientation = 90 the library will use {90, 120, 90, 60}.
        [internal lockAngles = FD_DETECTOR_CFG::lockAngles + FD_IMAGE::imageOrientation]
      - It is recommended to also add the flag FDD_DISABLE_AUTO_ORIENTATION 
        (disables orientation estimation when the orientation angle is known from 
        the system orientation sensor).
      - See also @ref faceApi_Sensor "Orientation Sensor".

    - FDD_LIMIT_FACE_SIZE: @anchor FDD_LIMIT_FACE_SIZE
      - If set (<B>recommended</B>) then the locking of additional faces is performed much
        faster as the library uses the size of current faces as a
        reference. Therefore if a new face is added to the scene, and some
        faces are already present, then the new face is not locked
        unless it is about the same size.
      - This flag can be set if more performance is needed. It does not
        work well if there is a group of faces (detected not at once) that
        are at different distance to the camera.
      - Tracking is not affected.

    - FDD_LIMIT_FACE_ANGLE: @anchor FDD_LIMIT_FACE_ANGLE
      - If set (<B>recommended</B>) then the locking of additional faces is performed much
        faster as the library uses the angle of current faces as a
        reference. Therefore if a new face is added to the scene, and some
        faces are already present, then the new face is not locked
        unless it has about the same angle.
      - This flag can be set if more performance is needed. It does not
        work well if there is a group of faces (detected not at once) with
        different orientations.
      - Tracking is not affected.

    - FDD_NEW_VERSION_CHECK: @anchor FDD_NEW_VERSION_CHECK
      Needed only for the applications that require compatibility with @ref
      faceApi_Ver "API version" prior to 0x00010003. Since in the older
      versions the FD_DETECTOR_VFT::GetApiVersion() function was not
      present - a way to find that the version is at least 0x00010003, and
      therefore is safe to call GetApiVersion(), is to set this flag and
      check the return of FdDetectorCreate():
        - If the version is at least 0x00010003 then
          FDS_NEW_VERSION (if no other errors) is returned.
        - If the @ref faceApi_Ver "API version" is older than 0x00010003 then
          FDS_OK (if no other errors) is returned.

    - FDD_DETECT_SMILE: @anchor FDD_DETECT_SMILE
      - If set and the library is compiled with smile detection support then
        the library reports the presence of a smile in FD_FACE::smileLevel.
      - If the library does not include the corresponding support then
        FdDetectorCreate() returns FDS_NOT_SUPPORTED when setting this
        flag.
      - To improve the smile detection also set the FDD_DETECT_EYES (or FDD_DETECT_BLINK) flag.
      - Minimal required @ref faceApi_Ver is 0x00010003.

    - FDD_DETECT_BLINK: @anchor FDD_DETECT_BLINK
      - If set and the library is compiled with smile detection support then
        the library reports the presence of a blink in FD_FACE::blinkLevel.
      - If the library does not include the corresponding support then
        FdDetectorCreate() returns FDS_NOT_SUPPORTED when setting this
        flag.
      - This flag is useful when demonstrating the blink detection during
        face tracking, but it should not be used in the production camera.
        In production camera use instead FD_DETECTOR_VFT::DetectBlink() to
        detect blink in a static image.
      - To improve the blink detection also set the FDD_DETECT_SMILE flag.
      - Minimal required @ref faceApi_Ver is 0x00010005.

    - FDD_DETECT_EYES: @anchor FDD_DETECT_EYES
      - If set and the library is compiled with eye detection support then
        the library reports the position of eyes in FD_FACE::eyeL and
        FD_FACE::eyeR.
      - If the library does not include the corresponding support then
        FdDetectorCreate() returns FDS_NOT_SUPPORTED when setting this
        flag.
      - Setting this flag improves the smile detection and the children
        classification, see FDD_DETECT_SMILE and FDD_DETECT_CHILDREN.
      - Minimal required @ref faceApi_Ver is 0x00010006.

    - FDD_DETECT_MOUTH: @anchor FDD_DETECT_MOUTH
      - If set and the library is compiled with mouth detection support then
        the library reports the position of the mouth in FD_FACE::mouth
      - If the library does not include the corresponding support then
        FdDetectorCreate() returns FDS_NOT_SUPPORTED when setting this
        flag.
      - Minimal required @ref faceApi_Ver is 0x00010008.

    - FDD_DISABLE_AUTO_ORIENTATION: @anchor FDD_DISABLE_AUTO_ORIENTATION
      - If not set, the library uses previously locked faces to evaluate
        the camera orientation and use this information for faster locking
        of additional faces.
      - this flag is not related to FDD_ENABLE_IMAGE_ORIENTATION flag.
      - Minimal required @ref faceApi_Ver is 0x0001000A.

    - FDD_DETECT_CHILDREN: @anchor FDD_DETECT_CHILDREN
      - If set and the library is compiled with children detection support then
        the library reports if the face detected belongs to a child
        in FD_FACE::childLevel.
      - If the library does not include the corresponding support then
        FdDetectorCreate() returns FDS_NOT_SUPPORTED when setting this
        flag.
      - To improve the children classification also set the FDD_DETECT_EYES
        (or FDD_DETECT_BLINK) flag.
      - Minimal required @ref faceApi_Ver is 0x00010010.

    - FDD_DISABLE_SEMIPROFILE_DETECTION: @anchor FDD_DISABLE_SEMIPROFILE_DETECTION
      - If not set and the library is compiled with semi-profile detection support then
        the library detects also semi-profiles and report the yaw angle in FD_FACE::yawAngle.
      - See @ref faceApi_Yaw "yaw angle overview".
      - Minimal required @ref faceApi_Ver is 0x00010013.

    - FDD_DISABLE_PROFILE_DETECTION: @anchor FDD_DISABLE_PROFILE_DETECTION
      - If not set and the library is compiled with profile detection support then
        the library detects also profiles and reports the yaw angle in FD_FACE::yawAngle.
      - See @ref faceApi_Yaw "yaw angle overview".
      - Minimal required @ref faceApi_Ver is 0x0001001A.

    - FDD_STATIC_SECOND_SEARCH: @anchor FDD_STATIC_SECOND_SEARCH
      - This flag enables the second (and more relaxed) face detection search
        in situations where faces are not detected in the first stage.
      - By adding this flag, detection rate in the static mode is improved in corner cases
        (such as low/side/back light or wrong WB) adding more cost in terms of execution time.
      - It applies only for @ref faceApi_Modes "static mode".
    */
    UINT32  flags;

    /** Specifies the minimum face size in pixels the library should try to
    detect. If zero is specified (default) then it tries to find down
    till the smallest size possible. Increasing this value increases the
    speed. */
    UINT32  minFace;

    /** Specifies the maximum face size in pixels the library should try to
    detect. If zero is specified (default) then it tries to find up till
    the maximum size possible. Decreasing this value increases the
    speed.*/
    UINT32  maxFace;

    /** The horizontal percentage of the frame size that is considered
    by the library during locking (detection of the new face). It does not
    affect the tracking area (following a detected face). The library
    "crops" the frame so that the specified locking area in the middle is
    used. If zero is specified (default) then a predefined percentage is
    used. If FDD_DISABLE_AUTO_ORIENTATION is not set, lock area percentage
    width and height is related to the main face angle. (0, 90, -90, 180)
    <P><B>Recommended value: 85 (Tracking), 100 (Static)</B></P> */
    UINT32  centralLockAreaPercentageW;

    /** The vertical percentage of the frame size.
    Same meaning as for FD_DETECTOR_CFG::centralLockAreaPercentageW.
    <P><B>Recommended value: 80 (Tracking), 90 (Static)</B></P>*/
    UINT32  centralLockAreaPercentageH;

    /** Specifies the maximum number of faces to be detected. The faces are limited
    giving priority to those closest to the center of the image. The number of faces
    is limited by discarding exceeding faces based on priority, meaning that
    locking of new faces is not disabled even if the number of tracked faces
    exceeds the value of centralMaxFaceNumberLimitation. If disabling of new
    faces locking is needed, use FD_DETECTOR_CFG::maxFaceCount.
    If zero is used (default) then there is no limitation for the number of faces. */
    UINT32  centralMaxFaceNumberLimitation;

    /** Specifies pointer to an array of values that lists the sequence of
    @ref faceApi_Roll "roll face angles" to be used by the locking algorithm.

    - During locking the algorithm tries to find first the faces oriented
      according to the first specified angle, then with the second, etc.
    - The array must be finished with FD_ANGLE_END.
    - The angles should be in the interval [-179, 180].
    - The currently supported angles are: 0, +/-30, +/-45, +/-60, +/-90, +/-120,
      +/-135, +/-150, 180 degrees (subject to included options).
    - If FDD_DISABLE_AUTO_ORIENTATION is not set, current lock angle is
      adjusted by main face angle (0, 90, -90, 180).
    - Each angle in the lockAngles array is processed in framesPerLock number of frames.
      Note: lockAngles array also includes by default semi-profile and profile templates, 
      which are not visible from API.
    - If NULL is specified, the algorithm automatically chooses the default
      angles sequence.
    <P><B>Recommended value: NULL (use the default angles sequence)</B></P>*/
    INT16*  lockAngles;

    /** A value between 1 and 10 specifying the number of frames needed to
    complete the face locking. If 0 is specified (default) the library
    chooses a default number of frames.

    For example if 3 is specified, then during the first frame 1/3 of
    locking task is done; during the second frame the next 1/3 of
    locking is done; and during the third frame the last 1/3 of locking
    is completed.

    This parameter allows to spread the processing time required for face
    locking across several frames. In turn, if more frames are specified,
    then more processing time is allocated for tracking algorithm. In other
    words, increasing this parameter decreases the priority of locking
    and increases the priority of tracking.
    <P><B>Recommended value: 5 (in tracking mode)</B></P> */
    UINT32  framesPerLock;

    /** Specifies the level of smoothing of the face tracking output.
    Smoothing prevents abrupt changes of the reported face position and size
    that can happen because of random factors.
    The valid values are in range [0 ... 100], where:
      - 0 - no smoothing;
      - 50 - medium smoothing;
      - 100 - highest smoothing.
      <P><B>Recommended value: 50 (in tracking mode)</B></P> */
    UINT32  smoothing;

    /** Sets a trade-off between face detection rate and false positives.
    It is a value between -100 ... 100:
    - boostFDvsFP = 0 - recommended default trade-off.
    - boostFDvsFP > 0 - higher detection rate and higher false positives.
    - boostFDvsFP < 0 - lower detection rate and lower false positives.
    .
    By changing this value, the execution time is affected to a smaller extent.
    @version
      Minimal required @ref faceApi_Ver is 0x00010010 (0x00010014 for negative values). */
    INT32  boostFDvsFP;

    /** Specifies the maximum number of faces to be detected and tracked. After
    this number is reached the algorithm stops searching for additional faces
    until the count of tracked faces decreases under this value. If zero
    is used (default) then there is no limitation for the number of faces.
    @version
      Minimal required @ref faceApi_Ver is 0x0001000E.
    */
    UINT32  maxFaceCount;

    /** Sets a trade-off between face detection rate and execution speed.
    It is a value between -100 ... 100:
    - boostFDvsSPEED = 0 - recommended default trade-off.
    - boostFDvsSPEED > 0 - higher detection rate and slower execution (up to several times).
    - boostFDvsSPEED < 0 - lower detection rate and faster execution.
    .
    By changing this value, the false positives are affected to a smaller extent.
    @version
      Minimal required @ref faceApi_Ver is 0x00010010 (0x00010014 for negative values). */
    INT32  boostFDvsSPEED;

    /** Specifies the minimum face size required in order to run features
    detection. If set to zero (default value), then no lower limit is used.
    @remarks
      minFaceSizeFeaturesDetect and #maxFaceSizeFeaturesDetect parameters have
      a purpose to limit the range of face sizes for which features detection
      like eye, blink, smile, age, mouth, etc take place:
      - It can reduce the execution time. For example the accuracy of the
        features detection may decrease with small faces (features are too small
        and difficult to analyze) and, at the same time, it takes more
        additional time on images with many small faces.
      - These parameters does not affect the size of detected faces. For this
        see #minFace and #maxFace.
    @version
      Minimal required @ref faceApi_Ver is 0x00010014.
    */
    UINT32  minFaceSizeFeaturesDetect;

    /** Specifies the maximum face size required in order to run features
    detection. If set to zero (default value), then no upper limit is used.
    See remarks for #minFaceSizeFeaturesDetect.
    @version
      Minimal required @ref faceApi_Ver is 0x00010014.
    */
    UINT32  maxFaceSizeFeaturesDetect;

    /** Specifies the number of frames to keep reporting a face after it is lost.
        - 0 - the library uses a default number of frames.
        - -1 - do not keep reporting the face after it is lost.
        - 1...30 - keep the lost face for specified number of frames more.
    @version
      Minimal required @ref faceApi_Ver is 0x00010017.
    */
    INT32  keepFacesOverMoreFrames;

    /** Specifies the number of frames to keep the limited detection
    after all the faces are lost. This parameter works in conjunction with
    @ref FDD_LIMIT_FACE_SIZE and @ref FDD_LIMIT_FACE_ANGLE. It prolongs
    the effect of these flags after all faces are lost:
        - 0 - the library uses a default number of frames.
        - -1 - do not keep limited detection after all faces are lost.
        - 1...30 - keep limited detection specified number of frames more after all faces are lost.
    @version
      Minimal required @ref faceApi_Ver is 0x00010017.
    */
    INT32  keepLimitingWhenNoFace;

    /** It is used when <B>FDD_FAST_LOCK</B> is set to specify the number of frames used for locking when there are no locked faces in the list.
    The range of this parameter is between 1 and framesPerLock (if higher it is adjusted internally to framesPerLock).
    If 0 is specified for this parameter (default) the library internally sets the parameter to 1.
    @version
      Minimal required @ref faceApi_Ver is 0x0001001B.
    */
    UINT32  framesPerLockWhenNoFaces;

} FD_DETECTOR_CFG;

//
// Define FD_PROGRESS callback routine reason values
//

enum
{
    FD_PROGRESS_BEGIN = 0,
    FD_PROGRESS_UPDATE,
    FD_PROGRESS_END,
};

///////////////////////////////////////////////////////////////////////////
// FD_PROGRESS
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
  <tr><td>FD_PROGRESS_BEGIN</td>
      <td>Indicates the beginning of the operation. The parameter
      @a param if equal to -1(0xFFFFFFFF) specifies an unknown number of steps,
      otherwise it specifies the total number of steps this operation has.</td></tr>
  <tr><td>FD_PROGRESS_UPDATE</td>
      <td>Indicates that the operation is being executed and
      the parameter @a param specifies number of completed steps.</td></tr>
  <tr><td>FD_PROGRESS_END</td>
      <td>Indicates that the operation has finished.</td></tr>
  </table>
@param[in] param
  Parameter of callback routine.
@retval 0 Continue current operation.
@retval 1 Cancel current operation. The corresponding library function returns status FDS_CANCELLED.
@remarks
  For example, an operation consisting of five steps could make the following sequence of callbacks:
  - FD_PROGRESS_BEGIN(5)
  - FD_PROGRESS_UPDATE(1)
  - FD_PROGRESS_UPDATE(1)
  - FD_PROGRESS_UPDATE(1)
  - FD_PROGRESS_UPDATE(2)
  - FD_PROGRESS_END(-)
**************************************************************************/
typedef UINT32
(FDCALL *FD_PROGRESS) (
    IN void* context,
    IN UINT32 reason,
    IN UINT32 param
    );

///////////////////////////////////////////////////////////////////////////
// struct FD_BLINK_INPUT
/**
  Specifies input data for FD_DETECTOR_VFT::DetectBlink() function.
*/
typedef struct
{
    /** The size, in bytes, of the ::FD_BLINK_INPUT structure. Be sure to
    set this member to sizeof(FD_BLINK_INPUT) before calling any function
    that uses this structure.
    */
    UINT32          structSize;

    /** The reference input image. The reference image must always be
    specified and can not be NULL.
    - If the application can provide two different resolutions, then the
      reference image must be the highest resolution and the lower
      resolution must be specified in FD_BLINK_INPUT::imageSmall.
    - If the application can provide only one image resolution, then
      it must be specified in this parameter, no matter what is the
      resolution.
    */
    const FD_IMAGE* image;

    /** The optional low resolution input image.
    - If the application can provide two different resolutions, then this
      parameter must specify the lower resolution and the higher
      resolution must be specified in FD_BLINK_INPUT::image.
    - If the application can provide only one image resolution, then
      this parameter must be NULL.
    - Specifying the low resolution image optimizes the detection on
      large faces.
    */
    const FD_IMAGE* imageSmall;

    /** The bounding rectangle indicating the face location in the
    reference image specified in FD_BLINK_INPUT::image.
    */
    const FD_RECT*  faceRect;

    /** The face roll angle (in-plane orientation) in degrees.
    The valid range is [-179 ... 180].
    A positive value means that the face is rotated clockwise relative
    to the reference image specified in FD_BLINK_INPUT::image.
    */
    INT32           faceAngle;

    /** Address of an optional callback function that is called to indicate
    the progress of operation. This parameter can be NULL.
    */
    FD_PROGRESS     progress;

    /** Optional argument to be passed to the progress callback function.
    This parameter can be NULL.
    */
    void*           context;
}
FD_BLINK_INPUT;

///////////////////////////////////////////////////////////////////////////
// FD_DETECTOR_VFT
/**
  Contains pointers to FD_DETECTOR functions.
**************************************************************************/
struct FD_DETECTOR_VFT
{
    ///////////////////////////////////////////////////////////////////////////
    // Destroy
    /**
      Destroys the specified detector object.
    @param[in] det
      Pointer to detector object to be destroyed.
    */
    void (FDCALL *Destroy)(
        IN FD_DETECTOR* det
        );

    ///////////////////////////////////////////////////////////////////////////
    // DetectFaces
    /**
      Detects faces in an image.
    @param[in] det
      Pointer to detector object.
    @param[in] image
      Pointer to a ::FD_IMAGE structure that specifies the input image.
    @param[in] progress
      Address of an optional callback function of type ::FD_PROGRESS that
      is called to indicate progress of operation. This parameter can be NULL.
    @param[in] context
      Optional argument to be passed to the callback function.
      This parameter can be NULL.
    @return
      If the function succeeds, the return value is FDS_OK. If no faces
      have been found, the function returns the status FDS_NOT_FOUND.
    @remarks
      This function is obsolete, use instead FD_DETECTOR_VFT::DetectFacesEx().
    @par Example:
      @ref faceApi_Examples_DetectFaces "Example"
    */
    FDSTATUS (FDCALL *DetectFaces)(
        IN FD_DETECTOR* det,
        IN const FD_IMAGE* image,
        IN FD_PROGRESS progress OPTIONAL,
        IN void* context OPTIONAL
        );

    ///////////////////////////////////////////////////////////////////////////
    // EnumFaces
    /**
      Enumerates faces in the list of found faces.
    @param[in] det
      Pointer to detector object.
    @param[in] face
      Pointer to the current face.
    @return
      Returns the pointer to a face that follows the face specified by @a face parameter.
    @remarks
      - If @a face is the last face in the list, the function returns NULL.
      - If @a face is NULL, the function returns first face in the list.
      - The returned FD_FACE pointer is valid until next call of
        FD_DETECTOR_VFT::DetectFacesEx(). Calling FD_DETECTOR_VFT::Destroy()
        makes also the pointer invalid.
    @par Example:
      @ref faceApi_Examples_EnumFaces "Example"
    */
    FD_FACE* (FDCALL *EnumFaces)(
        IN FD_DETECTOR* det,
        IN FD_FACE *face
        );

    ///////////////////////////////////////////////////////////////////////////
    // GetApiVersion()
    /**
      Retrieves the version of the API.
    @param [in] det
      Pointer to detector object.
    @return
      The API version.
    @remarks
      - This function is obsolete, use instead FdVersionGet().
      - This is not the library version that for example appears in Windows
        DLL version info resource. This is a separate version of API. The
        API version does not change when there are only changes in
        implementation. The API version changes when the API is modified so
        that the application needs to know if the new added features are available.
      - The API version is a 32-bit value where the higher 16 bits represent the
        major version and the lower 16 bits represent the minor version.
        When the minor is increased the API remains backward compatible. When
        the major is increased the API may not be backward compatible. For
        the current list of API versions see @ref faceApi_Ver.
      - Prior to version 0x00010003 this function was not available. In
        order to check if this function is present see
        FDD_NEW_VERSION_CHECK in FD_DETECTOR_CFG::flags.
    @version
      Minimal required @ref faceApi_Ver is 0x00010003.
    */
    UINT32 (FDCALL* GetApiVersion)(
        IN FD_DETECTOR* det);

    ///////////////////////////////////////////////////////////////////////////
    // DetectBlink()
    /**
      Performs the detection of eye blinking given a static image and an already
      detected face area on it.
    @param [in] det
      Pointer to detector object.
    @param [in] blinkInput
      The structure specifying the input parameters for the blink detector
      algorithm.
    @param [out] blinkLevel
      A pointer to a caller supplied variable that receives on return the
      indication of eye blinking presence.
      Following values are used:
      - (-1) - no blinking.
      - (0) - not able to detect.
      - (1 ... 100) - the higher is the value the more confidence is
        that a blink is present.
    @return
      - FDS_OK - success.
      - FDS_NOT_SUPPORTED - the library is compiled without blink detection support.
      - Other return codes indicate an error.
    @remarks
      - In order the blink detection to work the library must be compiled with the
        corresponding support, otherwise FDS_NOT_SUPPORTED is returned.
      - In case the faces have been detected using same FD_DETECTOR object passed
        to FD_DETECTOR_VFT::DetectFaces() and then to
        FD_DETECTOR_VFT::DetectBlink(), then the library may perform less
        processing as it may use some pre-calculated data available from face
        detection phase. In this case FD_BLINK_INPUT::image or
        FD_BLINK_INPUT::imageSmall must coincide with the image passed to
        FD_DETECTOR_VFT::DetectFaces().
    @version
      Minimal required @ref faceApi_Ver is 0x00010004.
    @par Example:
      @ref faceApi_Examples_DetectBlink "Example"
    */
    FDSTATUS (FDCALL *DetectBlink)(
        IN FD_DETECTOR* det,
        IN const FD_BLINK_INPUT* blinkInput,
        OUT INT32* blinkLevel);

    ///////////////////////////////////////////////////////////////////////////
    // ClearFaceList
    /**
      Empties the list of faces.
    @param[in] det
      Pointer to detector object.
    @return
      - FDS_OK - success.
      - Other return codes indicate an error.
    @version
      Minimal required @ref faceApi_Ver is 0x00010007.
    */
    FDSTATUS (FDCALL *ClearFaceList)(
        IN FD_DETECTOR* det
        );

    ///////////////////////////////////////////////////////////////////////////
    // DetectFacesEx()
    /**
      Detects faces in an image.
    @param[in] det
      Pointer to detector object.
    @param[in] image
      Pointer to a ::FD_IMAGE structure that specifies the input image.
    @param[in] progress
      Address of an optional callback function of type ::FD_PROGRESS that
      is called to indicate progress of operation. This parameter can be NULL.
    @param[in] context
      Optional argument to be passed to the callback function.
      This parameter can be NULL.
    @param[in] extraInfo
      The structure specifying the extra information.
      This parameter can be NULL.
    @return
      If the function succeeds, the return value is FDS_OK. If no faces
      have been found, the function returns the status FDS_NOT_FOUND.
    @version
      Minimal required @ref faceApi_Ver is 0x00010009.
    @par Example:
      @ref faceApi_Examples_DetectFacesEx "Example"
    */
    FDSTATUS (FDCALL *DetectFacesEx)(
        IN FD_DETECTOR* det,
        IN const FD_IMAGE* image,
        IN FD_PROGRESS progress OPTIONAL,
        IN void* context OPTIONAL,
        IN const FD_EXTRA_INFO* extraInfo OPTIONAL
        );

    ///////////////////////////////////////////////////////////////////////////
    // SetMainFaceMode()
    /**
      Sets the criteria used to select the main face from the current face lists.
    @param[in] det
      Pointer to detector object.
    @param[in] mode
      One of the following main face modes:
      - <b>FD_MAIN_FACE_MODE_DEFAULT</b>
        - Selects the main face by trying to choose the bigest and closer to
          the center one. Also the library takes in consideration the face
          confidence, i.e. how certain is the library that it is a face.
        - This mode is used by default.
      - <b>FD_MAIN_FACE_MODE_ADULT</b> or <b>FD_MAIN_FACE_MODE_CHILD</b>
        - Uses similar strategy as FD_MAIN_FACE_MODE_DEFAULT but in addition it
          tries to choose the face among the ones detected as adult or child.
        - The library has to be compiled with 'children detection' support,
          otherwise FDS_NOT_SUPPORTED is returned.
        - Children detection can be enabled by setting @ref FDD_DETECT_CHILDREN flag
          during detector initialization. However, if this flag was not set,
          the library implicitly enables children detection until
          SetMainFace() is called with a different mode.
        - While children detection is enabled explicitly or implicitly, the library
          updates FD_FACE::childLevel for each detected face.
    @return
      If the function succeeds, the return value is FDS_OK. If the selection mode
      is not supported, the return value is FDS_NOT_SUPPORTED.
    @remarks
      - The application can call at any time SetMainFaceMode(). The new mode
        is applied when next face detection is performed, i.e. when calling
        FD_DETECTOR_VFT::DetectFacesEx().
    @version
      Minimal required @ref faceApi_Ver is 0x00010012.
    @sa
      - @ref faceApi_MainFace "Selecting the Main Face".
      - FD_DETECTOR_VFT::GetMainFace().
    */
    FDSTATUS (FDCALL *SetMainFaceMode)(
        IN FD_DETECTOR* det,
        IN UINT32 mode
        );

    ///////////////////////////////////////////////////////////////////////////
    // GetMainFace()
    /**
      Selects a main face from the current face lists.
    @param[in] det
      Pointer to detector object.
    @return
      - The pointer to the selected face
      - NULL - if the face list is empty or the main face cannot be chosen.
    @remarks
      - The main face is chosen during the execution of FD_DETECTOR_VFT::DetectFacesEx().
        This function just returns the already chosen face.
      - The criteria on how to choose the main face can be changed by calling
        FD_DETECTOR_VFT::SetMainFaceMode().
      - If not NULL, then the returned pointer is the one among the faces in the
        list which can be obtained by FD_DETECTOR_VFT::EnumFaces().
      - The returned FD_FACE pointer is valid until next call of
        FD_DETECTOR_VFT::DetectFacesEx() - same life time as for faces returned
        by FD_DETECTOR_VFT::EnumFaces().
    @version
      Minimal required @ref faceApi_Ver is 0x00010012.
    @sa
      - @ref faceApi_MainFace "Selecting the Main Face".
      - FD_DETECTOR_VFT::SetMainFaceMode().
    */
    FD_FACE* (FDCALL *GetMainFace)(
        IN FD_DETECTOR* det
        );

    ///////////////////////////////////////////////////////////////////////////
    // UpdateConfiguration()
    /**
      Updates the detector configuration on the fly.
    @param[in] det
      Pointer to detector object.
    @param[in] cfg
        Pointer to the updated configuration.
    @return
    - FDS_OK - success.
    - Otherwise - error.
    @remarks
      - Not all parameters can be updated using this method. For parameters that cannot
        be updated a detector reset is required (e.g. for switching Still/Tracking mode,
        for setting a new lockAngles array).
      - The configuration fields will be updated or ignored as detailed below:
            - FD_DETECTOR_CFG::flags: Not all flags will be updated; flags that cannot change state are: FDD_TRACKING_MODE,
                    FDD_ENABLE_IMAGE_ORIENTATION, FDD_DISABLE_SEMIPROFILE_DETECTION,
                    FDD_DISABLE_PROFILE_DETECTION. If any of the flags that cannot be updated
                    are set different than initial configuration, all flags will be ignored.
            - FD_DETECTOR_CFG::minFace: Is not updated, ignored
            - FD_DETECTOR_CFG::maxFace: Will not be updated, ignored
            - FD_DETECTOR_CFG::centralLockAreaPercentageW: Will be updated
            - FD_DETECTOR_CFG::centralLockAreaPercentageH: Will be updated
            - FD_DETECTOR_CFG::centralMaxFaceNumberLimitation
            - FD_DETECTOR_CFG::lockAngles: Will not be updated, ignored
            - FD_DETECTOR_CFG::framesPerLock: Will be updated
            - FD_DETECTOR_CFG::smoothing: Will be updated
            - FD_DETECTOR_CFG::boostFDvsFP: Will be updated
            - FD_DETECTOR_CFG::maxFaceCount: Will be updated
            - FD_DETECTOR_CFG::boostFDvsSPEED: Will be updated
            - FD_DETECTOR_CFG::minFaceSizeFeaturesDetect: Will be updated
            - FD_DETECTOR_CFG::maxFaceSizeFeaturesDetect: Will be updated
            - FD_DETECTOR_CFG::keepFacesOverMoreFrames: Will be updated
            - FD_DETECTOR_CFG::keepLimitingWhenNoFace: Will be updated

    @version
      Minimal required @ref faceApi_Ver is 0x00010018.
    */
    FDSTATUS (FDCALL *UpdateConfiguration)(
        IN FD_DETECTOR* det,
        IN const FD_DETECTOR_CFG* cfg
        );

};

///////////////////////////////////////////////////////////////////////////
// FdDetectorCreate
/**
  Creates detector object.
@param[in] heap
  Address of an optional heap object that shall be used for memory
  management.
  - The heap object is owned by the detector object. I.e. when the
    detector object is destroyed via FD_DETECTOR_VFT::Destroy() it
    also calls FD_HEAP_VFT::Destroy(). The heap object is also destroyed
    in case FdDetectorCreate() fails. So the application must not destroy
    the heap explicitly after passing it to this function.
  - This parameter can be NULL, in that case internal library heap shall be
    used.
@param[in] cfg
  Address of an optional configuration. This parameter can be NULL, in that
  case default values shall be used.
@param[out] det
  A pointer to a caller supplied variable. On success it contains the
  pointer to the newly created library context.
@return
  - FDS_OK - success.
  - FDS_NEW_VERSION - success and FDD_NEW_VERSION_CHECK was set and the
    version is at least 0x00010003.
  - Otherwise - error.
**************************************************************************/
FDAPI
FDSTATUS
FDCALL
FdDetectorCreate(
    IN FD_HEAP* heap OPTIONAL,
    IN const FD_DETECTOR_CFG* cfg OPTIONAL,
    OUT FD_DETECTOR** det
    );

/** @} end of faceApi_Det */

#ifdef __cplusplus
}
#endif

///////////////////////////////////////////////////////////////////////////
// TOC:

/// @defgroup faceApi_API API Reference
/// @{

///   @defgroup faceApi_Ver API Version
///   @{
///   @}

///   @defgroup faceApi_Heap Heap Interface
///   @{
///   @}

///   @defgroup faceApi_Det Detector Interface
///   @{
///   @}

///   @defgroup faceApi_Status Status Codes
///   @{
///   @}

///   @defgroup faceApi_Fmt Image format constants
///   @{
///   @}

/// @}

#endif // __FACE_H__
