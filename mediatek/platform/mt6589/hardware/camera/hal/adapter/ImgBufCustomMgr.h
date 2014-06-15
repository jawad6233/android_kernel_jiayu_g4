/*************************************************************
 * 
 * Copyright (C), 2008-2013, OPPO Mobile Comm Corp., Ltd
 *  
 * VENDOR_EDIT   CUSTOM_PROCESS
 *
 * Date Created: 2013-02-18
 * 
 * Author: xinlan.he@Prd.Camera.CameraSrv
 * 
 * Description : NULL
 * 
 * ------------------ Revision History: ---------------------
 *      <author>        <date>          <desc>
 *
 *************************************************************/ 
#ifndef _MTK_CAMERA_HAL_CAM_ADAPTER_IMGBUFMGR_H_
#define _MTK_CAMERA_HAL_CAM_ADAPTER_IMGBUFMGR_H_

namespace android {
namespace NSCamAdapterImgMgr {

/******************************************************************************
*   Image Info
*******************************************************************************/
struct ImgInfo : public LightRefBase<ImgInfo>
{
    String8                         ms8ImgName;
    String8                         ms8ImgFormat;
    uint32_t                        mu4ImgWidth;
    uint32_t                        mu4ImgHeight;
    uint32_t                        mu4BitsPerPixel;
    size_t                          mImgBufSize;

    ImgInfo(
        uint32_t const u4ImgWidth, 
        uint32_t const u4ImgHeight, 
        char const*const ImgFormat, 
        char const*const pImgName = ""
    )
        : ms8ImgName(pImgName)
        , ms8ImgFormat(ImgFormat)
        , mu4ImgWidth(u4ImgWidth)
        , mu4ImgHeight(u4ImgHeight)
        , mu4BitsPerPixel( FmtUtils::queryBitsPerPixel(ms8ImgFormat) )
        , mImgBufSize( FmtUtils::queryImgBufferSize(ms8ImgFormat, mu4ImgWidth, mu4ImgHeight) )
    {
        CAM_LOGD(
            "[ImgInfo::ImgInfo] [%s](%s@%dx%d@%d-bit@%d)", 
            ms8ImgName.string(), ms8ImgFormat.string(), 
            mu4ImgWidth, mu4ImgHeight, mu4BitsPerPixel, mImgBufSize
        );
    }
};


/******************************************************************************
*   image buffer for preview callback
*******************************************************************************/
class ImgBufCustom : public ICameraImgBuf
{
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  IMemBuf Interface.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
public:     ////                    Attributes.
    virtual int64_t                 getTimestamp() const                    { return mi8Timestamp; }
    virtual void                    setTimestamp(int64_t const timestamp)   { mi8Timestamp = timestamp; }
    //
public:     ////                    Attributes.
    virtual const char*             getBufName() const                      { return mpImgInfo->ms8ImgName.string(); }
    virtual size_t                  getBufSize() const                      { return mpImgInfo->mImgBufSize; }
    //
    virtual void*                   getVirAddr() const                      { return mCamMem.data; }
    virtual void*                   getPhyAddr() const                      { return mCamMem.data; }
    //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  IImgBuf Interface.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
public:     ////                    Attributes.
    virtual String8 const&          getImgFormat()      const               { return mpImgInfo->ms8ImgFormat; }
    virtual uint32_t                getImgWidth()       const               { return mpImgInfo->mu4ImgWidth;  }
    virtual uint32_t                getImgHeight()      const               { return mpImgInfo->mu4ImgHeight; }
    virtual uint32_t                getImgWidthStride(
                                        uint_t const uPlaneIndex = 0
                                    )  const
                                    {
                                        return  FmtUtils::queryImgWidthStride(getImgFormat(), getImgWidth(), uPlaneIndex);
                                    }
    virtual uint32_t                getBitsPerPixel()   const               { return mpImgInfo->mu4BitsPerPixel; }
    //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  ICameraBuf Interface.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
public:     ////                    Attributes.
    virtual uint_t                  getBufIndex() const                     { return 0; }
    virtual camera_memory_t*        get_camera_memory()                     { return &mCamMem; }
    //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Interfaces.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
public:     ////                    Operations.
    //
    static ImgBufCustom*             alloc(
                                        camera_request_memory   requestMemory, 
                                        sp<ImgInfo const>const& rpImgInfo
                                    );

public:     ////                    Instantiation.
                                    ImgBufCustom(
                                        camera_memory_t const&  rCamMem, 
                                        sp<ImgInfo const>const& rpImgInfo, 
                                        int64_t const timestamp = 0
                                    );
    virtual                         ~ImgBufCustom();

public:     ////                    Debug.
    void                            dump() const;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Data Members.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
protected:  ////                    Memory.
    sp<ImgInfo const>               mpImgInfo;
    int64_t                         mi8Timestamp;
    camera_memory_t                 mCamMem;
};


/******************************************************************************
*   
*******************************************************************************/
class ImgBufCustomMgr : public RefBase
{
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Interfaces.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
public:     ////                    Operations.
    //
    static ImgBufCustomMgr*           alloc(
                                        char const*const        szImgFormat, 
                                        uint32_t const          u4ImgWidth, 
                                        uint32_t const          u4ImgHeight, 
                                        uint32_t const          u4BufCount, 
                                        char const*const        szName, 
                                        camera_request_memory   requestMemory
                                    );

public:     ////                    Attributes.
    //
    virtual char const*             getName() const             { return ms8Name.string(); }
    sp<ICameraImgBuf>const&         getBuf(size_t index) const  { return mvImgBuf[index]; }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:     ////                    Instantiation.
    virtual                         ~ImgBufCustomMgr();
protected:  ////                    Instantiation.
                                    ImgBufCustomMgr(
                                        char const*const        szImgFormat, 
                                        uint32_t const          u4ImgWidth, 
                                        uint32_t const          u4ImgHeight, 
                                        uint32_t const          u4BufCount, 
                                        char const*const        szName, 
                                        camera_request_memory   requestMemory
                                    );

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Implementations.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
protected:  ////                    Operations.
    //
    bool                            init();
    void                            uninit();

protected:  ////                    Data Members.
    //
    String8                         ms8Name;
    String8                         ms8ImgFormat;
    uint32_t                        mu4ImgWidth;
    uint32_t                        mu4ImgHeight;
    uint32_t                        mu4BufCount;
    //
    Vector< sp<ICameraImgBuf> >     mvImgBuf;
    camera_request_memory           mRequestMemory;
};


}; // namespace NSCamAdapterImgMgr
}; // namespace android
#endif  //_MTK_CAMERA_HAL_CLIENT_CAMCLIENT_PREVIEW_IMGBUFMANAGER_H_

