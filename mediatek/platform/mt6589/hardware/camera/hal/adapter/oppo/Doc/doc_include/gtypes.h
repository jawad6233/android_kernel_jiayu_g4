/*----------------------------------------------------

  Generic Types
  Copyright (c) DigitalOptics Corporation.
  All rights reserved. Confidential.

----------------------------------------------------*/

#ifndef __GTYPES_H__
#define __GTYPES_H__

#ifdef  __cplusplus
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////
// GTAPI
///////////////////////////////////////////////////////////////////////////

#if !defined(GTAPI)
#  define GTAPI
#endif

///////////////////////////////////////////////////////////////////////////
// GTCALL
///////////////////////////////////////////////////////////////////////////

#if !defined(GTCALL)
#  if defined(_STDCALL_SUPPORTED) // GCC, RVCT, etc
#    define GTCALL __stdcall
#  elif defined(_MSC_VER)         // VC++, Intel, etc
#    define GTCALL __stdcall
#  else
#    define GTCALL
#  endif
#endif

///////////////////////////////////////////////////////////////////////////
// UINT*, INT*
///////////////////////////////////////////////////////////////////////////
#if 0
#ifndef GT_CUSTOM_INT
  typedef unsigned char  UINT8;
  typedef unsigned short UINT16;
  typedef unsigned int   UINT32;
  typedef   signed char  INT8;
  typedef   signed short INT16;
  typedef   signed int   INT32;
#endif
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
// BOOL
///////////////////////////////////////////////////////////////////////////

#ifndef GT_CUSTOM_BOOL
  //typedef int BOOL;
#endif

///////////////////////////////////////////////////////////////////////////
// IN, OUT, OPTIONAL function arguments
///////////////////////////////////////////////////////////////////////////

#define IN
#define OUT
#define OPTIONAL

///////////////////////////////////////////////////////////////////////////
// GTSTATUS
/**
  Status of execution of a function.
  For the possible values see @ref gtAPI_Status.
@ingroup gtAPI_Status
**************************************************************************/

typedef INT32 GTSTATUS;

///////////////////////////////////////////////////////////////////////////
// GTS_*
/**
  Status Codes
@weakgroup gtAPI_Status Status Codes
@{
**************************************************************************/

/// @name General error codes
/// @{
#define GTS_OK                      0    ///< No errors.
#define GTS_FAIL                    (-1) ///< Unspecified error.
#define GTS_CANCELLED               (-2) ///< Operation was cancelled.
#define GTS_NO_SYSTEM_RESOURCES     (-3) ///< Not enough system resources.
#define GTS_INVALID_ARG             (-4) ///< One of the arguments passed is invalid.
#define GTS_NOT_SUPPORTED           (-6) ///< Feature is not supported.
#define GTS_FMT_NOT_SUPPORTED       (-7) ///< Image format not supported.
#define GTS_NOT_IMPLEMENTED         (-8) ///< Feature is not implemented.
#define GTS_BAD_STRUCT_SIZE         (-9) ///< Invalid structure size.
#define GTS_BAD_IMAGE_SIZE         (-10) ///< The size of the input image is not valid.
/// @}

#define GTS_LIB_ERROR               (-100) // First library specific error code.
#define GTS_LIB_INFO                ( 100) // First library specific non error code.

/** @} end of gtAPI_Status */

///////////////////////////////////////////////////////////////////////////
// GT_FMT_*
/**
@weakgroup gtAPI_Fmt Color Formats

  Color format code of a @ref GT_IMAGE "raster image".

  Color format is a 32-bit code that consists of the following bit fields.
  - 31-28 (4 bits): @ref gtAPI_FmtFamily "family"
  - 27-22 (6 bits): @ref gtAPI_FmtRange "range"
  - 21-20 (2 bits): number of @ref gtAPI_FmtPlanes "planes" - 1
  - 19-08 (12 bits): reserved, set to 0.
  - 07-00 (8 bits): subcode. It is unique only if other fields are the same.
    For similar formats that differ only by range, it has the same value.

  To extract these fields, use the following macros:
  @verbatim
  range   = GT_FMT_GET_RANGE(fmt);
  family  = GT_FMT_GET_FAMILY(fmt);
  planes  = GT_FMT_GET_PLANES(fmt);
  subcode = GT_FMT_GET_SUBCODE(fmt);
  @endverbatim

  @par Color Format Codes

  @note
    This is the complete list of color formats. A certain library may use only a
    subset of these. Please refer to the spec of the specific library to find out
    which formats it supports.

  <table>

  <tr>
    <th>Name</th>
    <th>Code</th>
    <th>@ref gtAPI_FmtRange "Range"</th>
    <th>@ref gtAPI_FmtPlanes "Planes"</th>
    <th>Layout</th>
  </tr>

  <tr><th colspan="5" >Grayscale formats</th> </tr>

  <tr><td>GT_FMT_GRAY        </td> <td>0x40000001</td> <td>8B      </td> <td>1</td> <td> @ref gtAPI_LayoutGray "Gray Pixel Interleaved" </td></tr>

  <tr><th colspan="5" >YUV 4:4:4 formats</th> </tr>

  <tr><td>GT_FMT_YUV_444     </td> <td>0x10000001</td> <td>YUV_255 </td> <td>1</td> <td> @ref gtAPI_LayoutYUV444 "YUV 4:4:4 Pixel Interleaved" </td></tr>
  <tr><td>GT_FMT_YUV16_444   </td> <td>0x11000001</td> <td>YUV_16B </td> <td>1</td> <td> @ref gtAPI_LayoutYUV444 "YUV 4:4:4 Pixel Interleaved" </td></tr>
  <tr><td>GT_FMT_YUVX_444    </td> <td>0x10000002</td> <td>YUV_255 </td> <td>1</td> <td> @ref gtAPI_LayoutYUV444 "YUV 4:4:4 Pixel Interleaved" </td></tr>
  <tr><td>GT_FMT_YUVA_444    </td> <td>0x10000003</td> <td>YUV_255 </td> <td>1</td> <td> @ref gtAPI_LayoutYUV444 "YUV 4:4:4 Pixel Interleaved" </td></tr>
  <tr><td>GT_FMT_YUV_444_P2  </td> <td>0x10100001</td> <td>YUV_255 </td> <td>2</td> <td> @ref gtAPI_LayoutYUV444P2 "YUV 4:4:4 2 Planes"</td></tr>
  <tr><td>GT_FMT_YUV16_444_P2</td> <td>0x11100001</td> <td>YUV_16B </td> <td>2</td> <td> @ref gtAPI_LayoutYUV444P2 "YUV 4:4:4 2 Planes"</td></tr>
  <tr><td>GT_FMT_YVU_444_P2  </td> <td>0x10100002</td> <td>YUV_255 </td> <td>2</td> <td> @ref gtAPI_LayoutYUV444P2 "YUV 4:4:4 2 Planes"</td></tr>
  <tr><td>GT_FMT_YUV_444_P3  </td> <td>0x10200001</td> <td>YUV_255 </td> <td>3</td> <td> @ref gtAPI_LayoutYUV444P3 "YUV 4:4:4 3(4) Planes"</td></tr>
  <tr><td>GT_FMT_YVU_444_P3  </td> <td>0x10200002</td> <td>YUV_255 </td> <td>3</td> <td> @ref gtAPI_LayoutYUV444P3 "YUV 4:4:4 3(4) Planes"</td></tr>
  <tr><td>GT_FMT_YUV16_444_P3</td> <td>0x11200001</td> <td>YUV_16B </td> <td>3</td> <td> @ref gtAPI_LayoutYUV444P3 "YUV 4:4:4 3(4) Planes"</td></tr>
  <tr><td>GT_FMT_YUVA_444_P4 </td> <td>0x10300001</td> <td>YUV_255 </td> <td>4</td> <td> @ref gtAPI_LayoutYUV444P3 "YUV 4:4:4 3(4) Planes"</td></tr>

  <tr><th colspan="5" >YUV 4:2:2 formats</th> </tr>

  <tr><td>GT_FMT_YUYV_422    </td> <td>0x20000001</td> <td>YUV_255 </td> <td>1</td> <td> @ref gtAPI_LayoutYUV422 "YUV 4:2:2 Pixel Interleaved" </td></tr>
  <tr><td>GT_FMT_YUYV16_422  </td> <td>0x21000001</td> <td>YUV_16B </td> <td>1</td> <td> @ref gtAPI_LayoutYUV422 "YUV 4:2:2 Pixel Interleaved" </td></tr>
  <tr><td>GT_FMT_UYVY_422    </td> <td>0x20000002</td> <td>YUV_255 </td> <td>1</td> <td> @ref gtAPI_LayoutYUV422 "YUV 4:2:2 Pixel Interleaved" </td></tr>
  <tr><td>GT_FMT_YVYU_422    </td> <td>0x20000003</td> <td>YUV_255 </td> <td>1</td> <td> @ref gtAPI_LayoutYUV422 "YUV 4:2:2 Pixel Interleaved" </td></tr>
  <tr><td>GT_FMT_YYUV_422    </td> <td>0x20000004</td> <td>YUV_255 </td> <td>1</td> <td> @ref gtAPI_LayoutYUV422 "YUV 4:2:2 Pixel Interleaved" </td></tr>
  <tr><td>GT_FMT_YUV_422_P2  </td> <td>0x20100001</td> <td>YUV_255 </td> <td>2</td> <td> @ref gtAPI_LayoutYUV422P2 "YUV 4:2:2 2 Planes" </td></tr>
  <tr><td>GT_FMT_YUVC_422_P2 </td> <td>0x20D00001</td> <td>YUV_255C</td> <td>2</td> <td> @ref gtAPI_LayoutYUV422P2 "YUV 4:2:2 2 Planes" </td></tr>
  <tr><td>GT_FMT_YUV16_422_P2</td> <td>0x21100001</td> <td>YUV_16B </td> <td>2</td> <td> @ref gtAPI_LayoutYUV422P2 "YUV 4:2:2 2 Planes" </td></tr>
  <tr><td>GT_FMT_YUV_422_P3  </td> <td>0x20200001</td> <td>YUV_255 </td> <td>3</td> <td> @ref gtAPI_LayoutYUV422P3 "YUV 4:2:2 3(4) Planes" </td></tr>
  <tr><td>GT_FMT_YUV16_422_P3</td> <td>0x21200001</td> <td>YUV_16B </td> <td>3</td> <td> @ref gtAPI_LayoutYUV422P3 "YUV 4:2:2 3(4) Planes" </td></tr>
  <tr><td>GT_FMT_YUVA_422_P4 </td> <td>0x20300001</td> <td>YUV_255 </td> <td>4</td> <td> @ref gtAPI_LayoutYUV422P3 "YUV 4:2:2 3(4) Planes" </td></tr>

  <tr><th colspan="5" >YUV 4:2:0 formats</th> </tr>

  <tr><td>GT_FMT_YUV_420_L3  </td> <td>0x30000001</td> <td>YUV_255 </td> <td>1</td> <td> @ref gtAPI_LayoutYUV420L3 "YUV 4:2:0 Line Interleaved" </td></tr>
  <tr><td>GT_FMT_YUV_420_P2  </td> <td>0x30100001</td> <td>YUV_255 </td> <td>2</td> <td> @ref gtAPI_LayoutYUV420P2 "YUV 4:2:0 2 Planes" </td></tr>
  <tr><td>GT_FMT_YUVC_420_P2 </td> <td>0x30D00001</td> <td>YUV_255C</td> <td>2</td> <td> @ref gtAPI_LayoutYUV420P2 "YUV 4:2:0 2 Planes" </td></tr>
  <tr><td>GT_FMT_YUV16_420_P2</td> <td>0x31100001</td> <td>YUV_16B </td> <td>2</td> <td> @ref gtAPI_LayoutYUV420P2 "YUV 4:2:0 2 Planes" </td></tr>
  <tr><td>GT_FMT_YVU_420_P2  </td> <td>0x30100002</td> <td>YUV_255 </td> <td>2</td> <td> @ref gtAPI_LayoutYUV420P2 "YUV 4:2:0 2 Planes" </td></tr>
  <tr><td>GT_FMT_YUV_420_P3  </td> <td>0x30200001</td> <td>YUV_255 </td> <td>3</td> <td> @ref gtAPI_LayoutYUV420P3 "YUV 4:2:0 3(4) Planes" </td></tr>
  <tr><td>GT_FMT_YUV16_420_P3</td> <td>0x31200001</td> <td>YUV_16B </td> <td>3</td> <td> @ref gtAPI_LayoutYUV420P3 "YUV 4:2:0 3(4) Planes" </td></tr>

  <tr><th colspan="5" >RGB formats</th> </tr>

  <tr><td>GT_FMT_RGB         </td> <td>0x50000001</td> <td>8B      </td> <td>1</td> <td> @ref gtAPI_LayoutRGB "RGB Pixel Interleaved" </td></tr>
  <tr><td>GT_FMT_RGB16       </td> <td>0x51000001</td> <td>16B     </td> <td>1</td> <td> @ref gtAPI_LayoutRGB "RGB Pixel Interleaved" </td></tr>
  <tr><td>GT_FMT_RGBX        </td> <td>0x50000002</td> <td>8B      </td> <td>1</td> <td> @ref gtAPI_LayoutRGB "RGB Pixel Interleaved" </td></tr>
  <tr><td>GT_FMT_ARGB        </td> <td>0x50000003</td> <td>8B      </td> <td>1</td> <td> @ref gtAPI_LayoutRGB "RGB Pixel Interleaved" </td></tr>
  <tr><td>GT_FMT_RGBA        </td> <td>0x50000004</td> <td>8B      </td> <td>1</td> <td> @ref gtAPI_LayoutRGB "RGB Pixel Interleaved" </td></tr>
  <tr><td>GT_FMT_RGBA16      </td> <td>0x51000004</td> <td>16B     </td> <td>1</td> <td> @ref gtAPI_LayoutRGB "RGB Pixel Interleaved" </td></tr>
  <tr><td>GT_FMT_BGR         </td> <td>0x50000009</td> <td>8B      </td> <td>1</td> <td> @ref gtAPI_LayoutRGB "RGB Pixel Interleaved" </td></tr>
  <tr><td>GT_FMT_BGR16       </td> <td>0x51000009</td> <td>16B     </td> <td>1</td> <td> @ref gtAPI_LayoutRGB "RGB Pixel Interleaved" </td></tr>
  <tr><td>GT_FMT_BGRX        </td> <td>0x5000000A</td> <td>8B      </td> <td>1</td> <td> @ref gtAPI_LayoutRGB "RGB Pixel Interleaved" </td></tr>
  <tr><td>GT_FMT_ABGR        </td> <td>0x5000000B</td> <td>8B      </td> <td>1</td> <td> @ref gtAPI_LayoutRGB "RGB Pixel Interleaved" </td></tr>
  <tr><td>GT_FMT_BGRA        </td> <td>0x5000000C</td> <td>8B      </td> <td>1</td> <td> @ref gtAPI_LayoutRGB "RGB Pixel Interleaved" </td></tr>
  <tr><td>GT_FMT_RGB_P3      </td> <td>0x50200001</td> <td>8B      </td> <td>3</td> <td> @ref gtAPI_LayoutRGBP3 "RGB 3(4) Planes" </td></tr>
  <tr><td>GT_FMT_RGB16_P3    </td> <td>0x51200001</td> <td>16B     </td> <td>3</td> <td> @ref gtAPI_LayoutRGBP3 "RGB 3(4) Planes" </td></tr>
  <tr><td>GT_FMT_RGBA_P4     </td> <td>0x50300001</td> <td>8B      </td> <td>4</td> <td> @ref gtAPI_LayoutRGBP3 "RGB 3(4) Planes" </td></tr>

  </table>

  @par Layout: Gray Pixel Interleaved
  @anchor gtAPI_LayoutGray

    Each pixel is represented by a luminance (Y) value with is stored in one or two bytes
    depending on the @ref gtAPI_FmtRange "range".

    For example, a 4x4 image looks like:

    @verbatim
    +-----+-----+-----+-----+
    |Y 0,0|Y 1,0|Y 2,0|Y 3,0|
    +-----+-----+-----+-----+
    |Y 0,1|Y 1,1|Y 2,1|Y 3,1|
    +-----+-----+-----+-----+
    |Y 0,2|Y 1,2|Y 2,2|Y 3,2|
    +-----+-----+-----+-----+
    |Y 0,3|Y 1,3|Y 2,3|Y 3,3|
    +-----+-----+-----+-----+
    @endverbatim

  @par Layout: YUV 4:4:4 Pixel Interleaved
  @anchor gtAPI_LayoutYUV444

    - Each pixel is represented by three color sample values: Y, U, V.
    - There can be a fourth value (if present in the format name):
      - A - alpha channel, or
      - X - ignored, used for aligment.
    - Each color sample Y, U, V, A or X occupies the same number of bytes. This is one or
      two bytes depending on the @ref gtAPI_FmtRange "range".
    - The color samples are stored per each pixel in the order given in the name of the color format.

    For example for GT_FMT_YUVA_444, the bytes are stored in order: Y, U, V, A.
    A 2x4 image for this format would look like:
    @verbatim
    +-----+-----+-----+-----+-----+-----+-----+-----+
    |Y 0,0|U 0,0|V 0,0|A 0,0|Y 1,0|U 1,0|V 1,0|A 1,0|
    +-----+-----+-----+-----+-----+-----+-----+-----+
    |Y 0,1|U 0,1|V 0,1|A 0,1|Y 1,1|U 1,1|V 1,1|A 1,1|
    +-----+-----+-----+-----+-----+-----+-----+-----+
    |Y 0,2|U 0,2|V 0,2|A 0,2|Y 1,2|U 1,2|V 1,2|A 1,2|
    +-----+-----+-----+-----+-----+-----+-----+-----+
    |Y 0,3|U 0,3|V 0,3|A 0,3|Y 1,3|U 1,3|V 1,3|A 1,3|
    +-----+-----+-----+-----+-----+-----+-----+-----+
    @endverbatim

  @par Layout: YUV 4:4:4 2 Planes
  @anchor gtAPI_LayoutYUV444P2

    - Each pixel is represented by three color sample values: Y, U, V.
    - Y samples are stored in the first plane.
    - U, V samples are stored pixel interleaved in the second plane in
      the order they appear in the format name.
    - Each color sample occupies the same number of bytes.
      This is one or two bytes depending on the @ref gtAPI_FmtRange "range".

    A 2x4 image would look like:
    @verbatim
    plane 0         plane 1
    +-----+-----+   +-----+-----+-----+-----+
    |Y 0,0|Y 1,0|   |U 0,0|V 0,0|U 1,0|V 1,0|
    +-----+-----+   +-----+-----+-----+-----+
    |Y 0,1|Y 1,1|   |U 0,1|V 0,1|U 1,1|V 1,1|
    +-----+-----+   +-----+-----+-----+-----+
    |Y 0,2|Y 1,2|   |U 0,2|V 0,2|U 1,2|V 1,2|
    +-----+-----+   +-----+-----+-----+-----+
    |Y 0,3|Y 1,3|   |U 0,3|V 0,3|U 1,3|V 1,3|
    +-----+-----+   +-----+-----+-----+-----+
    @endverbatim

  @par Layout: YUV 4:4:4 3(4) Planes
  @anchor gtAPI_LayoutYUV444P3

    - Each pixel is represented by three color sample values: Y, U, V.
    - Y samples are stored in the first plane, U - in the second, V - in the third.
    - Each color sample occupies the same number of bytes.
      This is one or two bytes depending on the @ref gtAPI_FmtRange "range".
    - There can be a fourth alpha (A) plane (if "A" is present in the format name).

    A 2x4 image would look like:
    @verbatim
    plane 0         plane 1         plane 2         plane 3 (optional)
    +-----+-----+   +-----+-----+   +-----+-----+   +-----+-----+
    |Y 0,0|Y 1,0|   |U 0,0|U 1,0|   |V 0,0|V 1,0|   |A 0,0|A 1,0|
    +-----+-----+   +-----+-----+   +-----+-----+   +-----+-----+
    |Y 0,1|Y 1,1|   |U 0,1|U 1,1|   |V 0,1|V 1,1|   |A 0,1|A 1,1|
    +-----+-----+   +-----+-----+   +-----+-----+   +-----+-----+
    |Y 0,2|Y 1,2|   |U 0,2|U 1,2|   |V 0,2|V 1,2|   |A 0,2|A 1,2|
    +-----+-----+   +-----+-----+   +-----+-----+   +-----+-----+
    |Y 0,3|Y 1,3|   |U 0,3|U 1,3|   |V 0,3|V 1,3|   |A 0,3|A 1,3|
    +-----+-----+   +-----+-----+   +-----+-----+   +-----+-----+
    @endverbatim

  @par Layout: YUV 4:2:2 Pixel Interleaved
  @anchor gtAPI_LayoutYUV422

    - Each two pixels in a row are represented by two Y values, one U and one V value,
      so there are four color samples per two pixels. Each image row looks like this:

    @verbatim
    +---+----+---+-----+ +---+----+---+-----+   +----+--------+----+--------+
    | Y0|U0&1| Y1| V0&1| | Y2|U2&3| Y3| V2&3|...|Yn-2|Un-2&n-1|Yn-1|Vn-2&n-1|
    +---+----+---+-----+ +---+----+---+-----+   +----+--------+----+--------+
    pixel 0,1            pixel 2,3              pixel n-2,n-1
    @endverbatim

    - Each color sample Y, U, V occupy the same number of bytes. This is one or
      two bytes depending on the @ref gtAPI_FmtRange "range".
    - The color samples are stored in the order given in the name of color format:
      - GT_FMT_YUYV_422: order Y0 U0&1 Y1 V0&1
      - GT_FMT_UYVY_422: order U0&1 Y0 V0&1 Y1
      - etc

  @par Layout: YUV 4:2:2 2 Planes
  @anchor gtAPI_LayoutYUV422P2

    - Each two pixels in a row are represented by two Y values, one U and one V value,
      so there are four color samples per two pixels.
    - Y samples are stored in the first plane.
    - U, V samples are stored pixel interleaved in the second plane in
      the order they appear in the format name.
    - Each color sample occupies the same number of bytes.
      This is one or two bytes depending on the @ref gtAPI_FmtRange "range".

    A 4x4 image would look like:
    @verbatim
    plane 0                     plane 1
    +-----+-----+-----+-----+   +-------+-------+-------+-------+
    |Y 0,0|Y 1,0|Y 2,0|Y 3,0|   |U 0&1,0|V 0&1,0|U 2&3,0|V 2&3,0|
    +-----+-----+-----+-----+   +-------+-------+-------+-------+
    |Y 0,1|Y 1,1|Y 2,1|Y 3,1|   |U 0&1,1|V 0&1,1|U 2&3,1|V 2&3,1|
    +-----+-----+-----+-----+   +-------+-------+-------+-------+
    |Y 0,2|Y 1,2|Y 2,2|Y 3,2|   |U 0&1,2|V 0&1,2|U 2&3,2|V 2&3,2|
    +-----+-----+-----+-----+   +-------+-------+-------+-------+
    |Y 0,3|Y 1,3|Y 2,3|Y 3,3|   |U 0&1,3|V 0&1,3|U 2&3,3|V 2&3,3|
    +-----+-----+-----+-----+   +-------+-------+-------+-------+
    @endverbatim

  @par Layout: YUV 4:2:2 3(4) Planes
  @anchor gtAPI_LayoutYUV422P3

    - Each two pixels in a row are represented by two Y values, one U and one V value,
      so there are four color samples per two pixels.
    - Y samples are stored in the first plane, U - in the second, V - in the third.
    - Each color sample occupies the same number of bytes.
      This is one or two bytes depending on the @ref gtAPI_FmtRange "range".
    - There can be a fourth alpha (A) plane (if "A" is present in the format name)
      with the same amount of samples as "Y".
    
    A 4x4 image would look like:
    @verbatim
    plane 0                     plane 1             plane 2             plane 3 (optional)
    +-----+-----+-----+-----+   +-------+-------+   +-------+-------+   +-----+-----+-----+-----+   
    |Y 0,0|Y 1,0|Y 2,0|Y 3,0|   |U 0&1,0|U 2&3,0|   |V 0&1,0|V 2&3,0|   |A 0,0|A 1,0|A 2,0|A 3,0|   
    +-----+-----+-----+-----+   +-------+-------+   +-------+-------+   +-----+-----+-----+-----+   
    |Y 0,1|Y 1,1|Y 2,1|Y 3,1|   |U 0&1,1|U 2&3,1|   |V 0&1,1|V 2&3,1|   |A 0,1|A 1,1|A 2,1|A 3,1|   
    +-----+-----+-----+-----+   +-------+-------+   +-------+-------+   +-----+-----+-----+-----+   
    |Y 0,2|Y 1,2|Y 2,2|Y 3,2|   |U 0&1,2|U 2&3,2|   |V 0&1,2|V 2&3,2|   |A 0,2|A 1,2|A 2,2|A 3,2|   
    +-----+-----+-----+-----+   +-------+-------+   +-------+-------+   +-----+-----+-----+-----+   
    |Y 0,3|Y 1,3|Y 2,3|Y 3,3|   |U 0&1,3|U 2&3,3|   |V 0&1,3|V 2&3,3|   |A 0,3|A 1,3|A 2,3|A 3,3|   
    +-----+-----+-----+-----+   +-------+-------+   +-------+-------+   +-----+-----+-----+-----+   
    @endverbatim

  @par Layout: YUV 4:2:0 Line Interleaved
  @anchor gtAPI_LayoutYUV420L3

    - Each four pixels in each 2x2 region of the image are represented by four Y values,
      one U and one V value, so there are six color samples per four pixels.
    - Each color sample occupies the same number of bytes.
      This is one or two bytes depending on the @ref gtAPI_FmtRange "range".
    - Each two rows of pixels are stored in three lines, where:
      - First line: Y values of the first row of pixels.
      - Second line: U and V values that correspond to both rows of pixels.
        After each U value comes the V value. The order can be UV or VU depending
        on how it appears in the format name (GT_FMT_YUV_420_L3 or GT_FMT_YVU_420_L3).
      - Third line: Y values of the second row of pixels.
    - Each line has the same amount of samples.

    A 4x4 image would look like:
    @verbatim
    plane 0                  
    +---------+---------+---------+---------+
    |Y 0,0    |Y 1,0    |    Y 2,0|    Y 3,0|
    +---------+---------+---------+---------+
    |U 0&1,0&1|V 0&1,0&1|U 2&3,0&1|V 2&3,0&1|
    +---------+---------+---------+---------+
    |Y 0,1    |Y 1,1    |    Y 2,1|    Y 3,1|
    +---------+---------+---------+---------+
    |Y 0,2    |Y 1,2    |    Y 2,2|    Y 3,2|
    +---------+---------+---------+---------+
    |U 0&1,2&3|V 0&1,2&3|U 2&3,2&3|V 2&3,2&3|
    +---------+---------+---------+---------+
    |Y 0,3    |Y 1,3    |    Y 2,3|    Y 3,3|
    +---------+---------+---------+---------+
    @endverbatim

  @par Layout: YUV 4:2:0 2 Planes
  @anchor gtAPI_LayoutYUV420P2

    - Each four pixels in each 2x2 region of the image are represented by four Y values,
      one U and one V value, so there are six color samples per four pixels.
    - Each color sample occupies the same number of bytes.
      This is one or two bytes depending on the @ref gtAPI_FmtRange "range".
    - Y samples are stored in the first plane.
    - U, V samples are stored pixel interleaved in the second plane in
      the order they appear in the format name.

    A 4x4 image would look like:
    @verbatim
    plane 0                     plane 1
    +-----+-----+-----+-----+   +---------+---------+---------+---------+
    |Y 0,0|Y 1,0|Y 2,0|Y 3,0|   |U 0&1,0&1|V 0&1,0&1|U 2&3,0&1|V 2&3,0&1|
    +-----+-----+-----+-----+   +---------+---------+---------+---------+
    |Y 0,1|Y 1,1|Y 2,1|Y 3,1|   |U 0&1,2&3|V 0&1,2&3|U 2&3,2&3|V 2&3,2&3|
    +-----+-----+-----+-----+   +---------+---------+---------+---------+
    |Y 0,2|Y 1,2|Y 2,2|Y 3,2|
    +-----+-----+-----+-----+
    |Y 0,3|Y 1,3|Y 2,3|Y 3,3|
    +-----+-----+-----+-----+
    @endverbatim

  @par Layout: YUV 4:2:0 3(4) Planes
  @anchor gtAPI_LayoutYUV420P3

    - Each four pixels in each 2x2 region of the image are represented by four Y values,
      one U and one V value, so there are six color samples per four pixels.
    - Y samples are stored in the first plane, U - in the second, V - in the third.
    - Each color sample occupies the same number of bytes.
      This is one or two bytes depending on the @ref gtAPI_FmtRange "range".
    - There can be a fourth alpha (A) plane (if "A" is present in the format name)
      with the same amout of samples as "Y".

    A 4x4 image would look like:
    @verbatim
    plane 0                     plane 1                 plane 2                 plane 3 (optional)
    +-----+-----+-----+-----+   +---------+---------+   +---------+---------+   +-----+-----+-----+-----+   
    |Y 0,0|Y 1,0|Y 2,0|Y 3,0|   |U 0&1,0&1|U 2&3,0&1|   |V 0&1,0&1|V 2&3,0&1|   |A 0,0|A 1,0|A 2,0|A 3,0|   
    +-----+-----+-----+-----+   +---------+---------+   +---------+---------+   +-----+-----+-----+-----+   
    |Y 0,1|Y 1,1|Y 2,1|Y 3,1|   |U 0&1,2&3|U 2&3,2&3|   |V 0&1,2&3|V 2&3,2&3|   |A 0,1|A 1,1|A 2,1|A 3,1|   
    +-----+-----+-----+-----+   +---------+---------+   +---------+---------+   +-----+-----+-----+-----+   
    |Y 0,2|Y 1,2|Y 2,2|Y 3,2|                                                   |A 0,2|A 1,2|A 2,2|A 3,2|   
    +-----+-----+-----+-----+                                                   +-----+-----+-----+-----+   
    |Y 0,3|Y 1,3|Y 2,3|Y 3,3|                                                   |A 0,3|A 1,3|A 2,3|A 3,3|   
    +-----+-----+-----+-----+                                                   +-----+-----+-----+-----+   
    @endverbatim

  @par Layout: RGB Pixel Interleaved
  @anchor gtAPI_LayoutRGB

    - Each pixel is represented by three color sample values: R, G, B.
    - There can be a fourth value (if present in the format name):
      - A - alpha channel, or
      - X - ignored, used for aligment.
    - Each color sample R, G, B, A or X occupies the same number of bytes. This is one or
      two bytes depending on the @ref gtAPI_FmtRange "range".
    - The color samples are stored per each pixel in the order given in the name of color format.

    For example for GT_FMT_BGRA, the bytes are stored in order: B, G, R, A.

    A 2x4 image for this format would look like:
    @verbatim
    +-----+-----+-----+-----+-----+-----+-----+-----+
    |B 0,0|G 0,0|R 0,0|A 0,0|B 1,0|G 1,0|R 1,0|A 1,0|
    +-----+-----+-----+-----+-----+-----+-----+-----+
    |B 0,1|G 0,1|R 0,1|A 0,1|B 1,1|G 1,1|R 1,1|A 1,1|
    +-----+-----+-----+-----+-----+-----+-----+-----+
    |B 0,2|G 0,2|R 0,2|A 0,2|B 1,2|G 1,2|R 1,2|A 1,2|
    +-----+-----+-----+-----+-----+-----+-----+-----+
    |B 0,3|G 0,3|R 0,3|A 0,3|B 1,3|G 1,3|R 1,3|A 1,3|
    +-----+-----+-----+-----+-----+-----+-----+-----+
    @endverbatim

  @par Layout: RGB 3(4) Planes
  @anchor gtAPI_LayoutRGBP3
                              
    - Each pixel is represented by three color sample values: R, G, B.
    - R samples are stored in the first plane, G - in the second, B - in the third.
    - Each color sample occupies the same number of bytes.
      This is one or two bytes depending on the @ref gtAPI_FmtRange "range".
    - There can be a fourth alpha (A) plane (if "A" is present in the format name).

    A 2x4 image would look like:
    @verbatim
    plane 0         plane 1         plane 2         plane 3 (optional)
    +-----+-----+   +-----+-----+   +-----+-----+   +-----+-----+
    |R 0,0|Y 1,0|   |G 0,0|U 1,0|   |B 0,0|V 1,0|   |A 0,0|A 1,0|
    +-----+-----+   +-----+-----+   +-----+-----+   +-----+-----+
    |R 0,1|Y 1,1|   |G 0,1|U 1,1|   |B 0,1|V 1,1|   |A 0,1|A 1,1|
    +-----+-----+   +-----+-----+   +-----+-----+   +-----+-----+
    |R 0,2|Y 1,2|   |G 0,2|U 1,2|   |B 0,2|V 1,2|   |A 0,2|A 1,2|
    +-----+-----+   +-----+-----+   +-----+-----+   +-----+-----+
    |R 0,3|Y 1,3|   |G 0,3|U 1,3|   |B 0,3|V 1,3|   |A 0,3|A 1,3|
    +-----+-----+   +-----+-----+   +-----+-----+   +-----+-----+
    @endverbatim

**************************************************************************/

///////////////////////////////////////////////////////////////////////////
// Color Planes
/**
@defgroup gtAPI_FmtPlanes Color Planes
@ingroup gtAPI_Fmt

  Specifies the number of color planes (1 to 4).

  The number of color planes is encoded in the @ref gtAPI_Fmt "color format code".

  To get the number of planes from the format code, use the following macro:
  @verbatim
  planes = GT_FMT_GET_PLANES(fmt);
  @endverbatim

  If a color format has more than one plane, then this is indicated in the
  identifier with the following prefixes:
  - *_P2 - two planes. Usually used with YUV color space, where first plane
           contains Y values and the second plane UV values.
  - *_P3 - three planes, one per each color sample.
  - *_P4 - four planes. Usually used to store three color samples and an alpha
           channel in each plane.

  The order of the planes is same as the order of samples in the format identifier.
  Examples:
  - GT_FMT_YUV_422_P3 - the order of planes is Y, U, V.
  - GT_FMT_RGBA_P4 - the order of planes is R, G, B, A.
  - GT_FMT_YUV_420_P2 - first plane is Y, second plane is interleaved in this
    order: U, V.

  If the number of planes is one, then the color samples are interleaved per each
  pixel, or a more complex layout is used.

**************************************************************************/

#define GT_FMT(family, range, planes, subcode) \
  ( ((family) << 28) | ((range) << 22) | (((planes) - 1) << 20) | (subcode) )

#define GT_FMT_GET_FAMILY(code)  ((((code) >> 28) & 0x0F) + 0)
#define GT_FMT_GET_RANGE(code)   ((((code) >> 22) & 0x3F) + 0)
#define GT_FMT_GET_PLANES(code)  ((((code) >> 20) & 0x03) + 1)
#define GT_FMT_GET_SUBCODE(code) ((code) & 0x000000FF)

///////////////////////////////////////////////////////////////////////////
// GT_FMT_F_*
/**
@defgroup gtAPI_FmtFamily Color Format Families
@ingroup gtAPI_Fmt

  Used as a part of the @ref gtAPI_Fmt "color format code".
  Specifies the color space and, in case of YUV, the subsampling.

  To get the family code from the format code, use the following macro:
  @verbatim
  family = GT_FMT_GET_FAMILY(fmt);
  @endverbatim

  Following are the family codes:
  - 1 GT_FMT_F_YUV444: YUV, 4:4:4 subsampling
  - 2 GT_FMT_F_YUV422: YUV, 4:2:2 subsampling
  - 3 GT_FMT_F_YUV420: YUV, 4:2:0 subsampling
  - 4 GT_FMT_F_GRAY  : Grayscale
  - 5 GT_FMT_F_RGB   : RGB

**************************************************************************/

enum {
  GT_FMT_F_YUV444 = 1,
  GT_FMT_F_YUV422 = 2,
  GT_FMT_F_YUV420 = 3,
  GT_FMT_F_GRAY   = 4,
  GT_FMT_F_RGB    = 5,
};

///////////////////////////////////////////////////////////////////////////
// GT_FMT_R_*
/**
@defgroup gtAPI_FmtRange Color Sample Ranges
@ingroup gtAPI_Fmt

  Used as a part of the @ref gtAPI_Fmt "color format code".
  Specifies the range of the color sample values.

  To get the range code from the format code, use the following macro:
  @verbatim
  range = GT_FMT_GET_RANGE(fmt);
  @endverbatim

  Following are the ranges used with YUV color space:
  - 0x00 GT_FMT_R_YUV_255        : Y =  0 .. 255, UV =    0 .. 255 
  - 0x01 GT_FMT_R_YUV_16_255     : Y = 16 .. 255, UV =   16 .. 255 
  - 0x02 GT_FMT_R_YUV_16_235_240 : Y = 16 .. 235, UV =   16 .. 240 
  - 0x03 GT_FMT_R_YUV_255C       : Y =  0 .. 255, UV = -128 .. 127 
  - 0x04 GT_FMT_R_YUV_16B        : Y =  0 .. 65535, UV = 0 .. 65535

  Following are the ranges used with other color spaces:
  - 0x00 GT_FMT_R_8B             : 8 bits per color sample
  - 0x04 GT_FMT_R_16B            : 16 bits per color sample

**************************************************************************/

enum {
  GT_FMT_R_8B             = 0x00,
  GT_FMT_R_16B            = 0x04,
  GT_FMT_R_YUV_255        = 0x00,
  GT_FMT_R_YUV_16_255     = 0x01,
  GT_FMT_R_YUV_16_235_240 = 0x02,
  GT_FMT_R_YUV_255C       = 0x03,
  GT_FMT_R_YUV_16B        = 0x04,
};

enum {
  GT_FMT_UNKNOWN      = 0,

  GT_FMT_GRAY         = GT_FMT(GT_FMT_F_GRAY,   GT_FMT_R_8B,       1,  1),

  GT_FMT_YUV_444      = GT_FMT(GT_FMT_F_YUV444, GT_FMT_R_YUV_255,  1,  1),
  GT_FMT_YUV16_444    = GT_FMT(GT_FMT_F_YUV444, GT_FMT_R_YUV_16B,  1,  1),
  GT_FMT_YUVX_444     = GT_FMT(GT_FMT_F_YUV444, GT_FMT_R_YUV_255,  1,  2),
  GT_FMT_YUVA_444     = GT_FMT(GT_FMT_F_YUV444, GT_FMT_R_YUV_255,  1,  3),
  GT_FMT_YUV_444_P2   = GT_FMT(GT_FMT_F_YUV444, GT_FMT_R_YUV_255,  2,  1),
  GT_FMT_YUV16_444_P2 = GT_FMT(GT_FMT_F_YUV444, GT_FMT_R_YUV_16B,  2,  1),
  GT_FMT_YVU_444_P2   = GT_FMT(GT_FMT_F_YUV444, GT_FMT_R_YUV_255,  2,  2),
  GT_FMT_YUV_444_P3   = GT_FMT(GT_FMT_F_YUV444, GT_FMT_R_YUV_255,  3,  1),
  GT_FMT_YVU_444_P3   = GT_FMT(GT_FMT_F_YUV444, GT_FMT_R_YUV_255,  3,  2),
  GT_FMT_YUV16_444_P3 = GT_FMT(GT_FMT_F_YUV444, GT_FMT_R_YUV_16B,  3,  1),
  GT_FMT_YUVA_444_P4  = GT_FMT(GT_FMT_F_YUV444, GT_FMT_R_YUV_255,  4,  1),

  GT_FMT_YUYV_422     = GT_FMT(GT_FMT_F_YUV422, GT_FMT_R_YUV_255,  1,  1),
  GT_FMT_YUYV16_422   = GT_FMT(GT_FMT_F_YUV422, GT_FMT_R_YUV_16B,  1,  1),
  GT_FMT_UYVY_422     = GT_FMT(GT_FMT_F_YUV422, GT_FMT_R_YUV_255,  1,  2),
  GT_FMT_YVYU_422     = GT_FMT(GT_FMT_F_YUV422, GT_FMT_R_YUV_255,  1,  3),
  GT_FMT_YYUV_422     = GT_FMT(GT_FMT_F_YUV422, GT_FMT_R_YUV_255,  1,  4),
  GT_FMT_YUV_422_P2   = GT_FMT(GT_FMT_F_YUV422, GT_FMT_R_YUV_255,  2,  1),
  GT_FMT_YUVC_422_P2  = GT_FMT(GT_FMT_F_YUV422, GT_FMT_R_YUV_255C, 2,  1),
  GT_FMT_YUV16_422_P2 = GT_FMT(GT_FMT_F_YUV422, GT_FMT_R_YUV_16B,  2,  1),
  GT_FMT_YUV_422_P3   = GT_FMT(GT_FMT_F_YUV422, GT_FMT_R_YUV_255,  3,  1),
  GT_FMT_YUV16_422_P3 = GT_FMT(GT_FMT_F_YUV422, GT_FMT_R_YUV_16B,  3,  1),
  GT_FMT_YUVA_422_P4  = GT_FMT(GT_FMT_F_YUV422, GT_FMT_R_YUV_255,  4,  1),

  GT_FMT_YUV_420_L3   = GT_FMT(GT_FMT_F_YUV420, GT_FMT_R_YUV_255,  1,  1),
  GT_FMT_YUV_420_P2   = GT_FMT(GT_FMT_F_YUV420, GT_FMT_R_YUV_255,  2,  1),
  GT_FMT_YUVC_420_P2  = GT_FMT(GT_FMT_F_YUV420, GT_FMT_R_YUV_255C, 2,  1),
  GT_FMT_YUV16_420_P2 = GT_FMT(GT_FMT_F_YUV420, GT_FMT_R_YUV_16B,  2,  1),
  GT_FMT_YVU_420_P2   = GT_FMT(GT_FMT_F_YUV420, GT_FMT_R_YUV_255,  2,  2),
  GT_FMT_YUV_420_P3   = GT_FMT(GT_FMT_F_YUV420, GT_FMT_R_YUV_255,  3,  1),
  GT_FMT_YUV16_420_P3 = GT_FMT(GT_FMT_F_YUV420, GT_FMT_R_YUV_16B,  3,  1),

  GT_FMT_RGB          = GT_FMT(GT_FMT_F_RGB,    GT_FMT_R_8B,       1,  1),
  GT_FMT_RGB16        = GT_FMT(GT_FMT_F_RGB,    GT_FMT_R_16B,      1,  1),
  GT_FMT_RGBX         = GT_FMT(GT_FMT_F_RGB,    GT_FMT_R_8B,       1,  2),
  GT_FMT_ARGB         = GT_FMT(GT_FMT_F_RGB,    GT_FMT_R_8B,       1,  3),
  GT_FMT_RGBA         = GT_FMT(GT_FMT_F_RGB,    GT_FMT_R_8B,       1,  4),
  GT_FMT_RGBA16       = GT_FMT(GT_FMT_F_RGB,    GT_FMT_R_16B,      1,  4),
  GT_FMT_BGR          = GT_FMT(GT_FMT_F_RGB,    GT_FMT_R_8B,       1,  9),
  GT_FMT_BGR16        = GT_FMT(GT_FMT_F_RGB,    GT_FMT_R_16B,      1,  9),
  GT_FMT_BGRX         = GT_FMT(GT_FMT_F_RGB,    GT_FMT_R_8B,       1, 10),
  GT_FMT_ABGR         = GT_FMT(GT_FMT_F_RGB,    GT_FMT_R_8B,       1, 11),
  GT_FMT_BGRA         = GT_FMT(GT_FMT_F_RGB,    GT_FMT_R_8B,       1, 12),
  GT_FMT_RGB_P3       = GT_FMT(GT_FMT_F_RGB,    GT_FMT_R_8B,       3,  1),
  GT_FMT_RGB16_P3     = GT_FMT(GT_FMT_F_RGB,    GT_FMT_R_16B,      3,  1),
  GT_FMT_RGBA_P4      = GT_FMT(GT_FMT_F_RGB,    GT_FMT_R_8B,       4,  1),
};

///////////////////////////////////////////////////////////////////////////
// GT_POINT
/**
  Specifies the coordinates (x,y) of a point.
**************************************************************************/

typedef struct
{
    INT32 x; ///< The x-coordinate of the point.
    INT32 y; ///< The y-coordinate of the point.
} GT_POINT;

///////////////////////////////////////////////////////////////////////////
// GT_RECT
/**
  Specifies the coordinates of a rectangle.
**************************************************************************/

typedef struct
{
    INT32 left;   ///< The x-coordinate of the upper-left corner of the rectangle.
    INT32 top;    ///< The y-coordinate of the upper-left corner of the rectangle.
    INT32 width;  ///< The horizontal width of the rectangle.
    INT32 height; ///< The vertical height of the rectangle.
} GT_RECT;

///////////////////////////////////////////////////////////////////////////
// GT_SIZE
/**
  Specifies the size of a rectangle.
**************************************************************************/
 
typedef struct
{
    INT32 width;  ///< The horizontal width of the rectangle.
    INT32 height; ///< The vertical height of the rectangle.
} GT_SIZE;

///////////////////////////////////////////////////////////////////////////
// GT_PLANE
/**
  Specifies the image @ref gtAPI_FmtPlanes "color plane".
**************************************************************************/

typedef struct
{
    /** Offset, in bytes, between consecutive lines of the image plane.
        When negative, the lines are stored in memory in bottom-up fashion.
    */
    INT32 stride;

    /** Pointer to the first line of the image plane.
        To move to the next line, add GT_PLANE::stride bytes to the pointer.
    */
    void* data;
} GT_PLANE;

///////////////////////////////////////////////////////////////////////////
// GT_IMAGE
/**
  Contains information about the raster image.

  Example 1:
  @code
    GT_IMAGE img;
    memset(&img, 0, sizeof(GT_IMAGE));
    img.format = GT_FMT_YUYV_422; // this format uses one plane
    img.width  = 320; // pixels
    img.height = 240; // pixels
    img.planes[0].stride = img.width * 2; // bytes, 2 per pixel
    img.planes[0].data = buffer; // size = stride * height;
  @endcode

  Example 2:
  @code
    GT_IMAGE img;
    memset(&img, 0, sizeof(GT_IMAGE));
    img.format = GT_FMT_YUV_420_P2; // this format uses two planes
    img.width  = 320; // pixels
    img.height = 240; // pixels
    // Y plane:
    img.planes[0].stride = img.width; // bytes
    img.planes[0].data = yBuffer; // size = stride * height;
    // UV plane:
    img.planes[1].stride = img.width; // bytes
    img.planes[1].data = uvBuffer; // size = stride * height/2;
  @endcode
**************************************************************************/

typedef struct 
{
    UINT32   format;    ///< Image @ref gtAPI_Fmt "color format".
    UINT32   width;     ///< Image width in pixels.
    UINT32   height;    ///< Image height in pixels.
    GT_PLANE planes[4]; ///< Image data planes.
} GT_IMAGE;

///////////////////////////////////////////////////////////////////////////
// Heap interface
/**
@weakgroup gtAPI_Heap Heap Interface
  An application can provide memory management for the library by means of a
  heap object. The heap object is represented by the GT_HEAP structure.

  The following code shows how to create a heap object using system memory
  management functions:
  @code
    static void GTCALL heap_destroy(GT_HEAP* heap)
    {
      free(heap);
    }
    static void* GTCALL heap_alloc(GT_HEAP* heap, UINT32 size)
    {
      return malloc(size);
    }
    static void GTCALL heap_free(GT_HEAP* heap, void* mem)
    {
      free(mem);
    }

    static const struct GT_HEAP_VFT vft = {
      heap_destroy,
      heap_alloc,
      heap_free
    };
    GT_HEAP* heap = (GT_HEAP*)malloc(sizeof(GT_HEAP));
    heap->vft = vft;
  @endcode
**************************************************************************/

///////////////////////////////////////////////////////////////////////////
// GT_HEAP
/**
  Represents the @ref gtAPI_Heap "heap interface".
@ingroup gtAPI_Heap
**************************************************************************/
typedef struct 
{
    /** Pointer to function table. */
    struct GT_HEAP_VFT *vft;
    // private part follows...
} GT_HEAP;

///////////////////////////////////////////////////////////////////////////
// GT_HEAP_VFT
/**
  Contains pointers to memory management functions.
  The section @ref gtAPI_Heap "heap interface" has some examples how to define memory management functions.
@ingroup gtAPI_Heap
**************************************************************************/
struct GT_HEAP_VFT
{
    // Destroy()
    //
    /** Destroys the specified heap object.
    @param[in] heap
      Pointer to heap object.
    */
    void (GTCALL* Destroy)(
       GT_HEAP* heap);

    // Alloc()
    //
    /** Allocates a block of memory from the heap.
    @param[in] heap
      Pointer to heap object.
    @param[in] size
      Number of bytes to be allocated.
    @return
      If the function succeeds, the return value is a pointer to the allocated
      memory block. If the function fails, the return value is NULL.
    */
    void* (GTCALL* Alloc)(
        GT_HEAP* heap,
        UINT32   size);

    // Free()
    //
    /** Frees a memory block allocated from a heap by the Alloc function.
    @param[in] heap
      Pointer to the heap object.
    @param[in] mem
      Pointer to the memory block to be freed. This pointer shall be returned
      by the @ref GT_HEAP_VFT::Alloc "Alloc" function.
    */
    void (GTCALL* Free)(
       GT_HEAP* heap,
       void*    mem);
};

#ifdef __cplusplus
}
#endif

#endif // __GTYPES_H__
