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
#define LOG_TAG "MtkCam/CustomBufMgr"
//
#warning "[Remove] ION not ready"
#include <binder/MemoryHeapBase.h>
//
#include "inc/CamUtils.h"
using namespace android;
using namespace MtkCamUtils;
//
#include "ImgBufCustomMgr.h"
//
using namespace NSCamAdapterImgMgr;
//


/******************************************************************************
*
*******************************************************************************/
#define MY_LOGV(fmt, arg...)        CAM_LOGV("(%d)(%s)[ImgBufCustomMgr::%s] "fmt, ::gettid(), getName(), __FUNCTION__, ##arg)
#define MY_LOGD(fmt, arg...)        CAM_LOGD("(%d)(%s)[ImgBufCustomMgr::%s] "fmt, ::gettid(), getName(), __FUNCTION__, ##arg)
#define MY_LOGI(fmt, arg...)        CAM_LOGI("(%d)(%s)[ImgBufCustomMgr::%s] "fmt, ::gettid(), getName(), __FUNCTION__, ##arg)
#define MY_LOGW(fmt, arg...)        CAM_LOGW("(%d)(%s)[ImgBufCustomMgr::%s] "fmt, ::gettid(), getName(), __FUNCTION__, ##arg)
#define MY_LOGE(fmt, arg...)        CAM_LOGE("(%d)(%s)[ImgBufCustomMgr::%s] "fmt, ::gettid(), getName(), __FUNCTION__, ##arg)
#define MY_LOGA(fmt, arg...)        CAM_LOGA("(%d)(%s)[ImgBufCustomMgr::%s] "fmt, ::gettid(), getName(), __FUNCTION__, ##arg)
#define MY_LOGF(fmt, arg...)        CAM_LOGF("(%d)(%s)[ImgBufCustomMgr::%s] "fmt, ::gettid(), getName(), __FUNCTION__, ##arg)
//
#define MY_LOGV_IF(cond, ...)       do { if ( (cond) ) { MY_LOGV(__VA_ARGS__); } }while(0)
#define MY_LOGD_IF(cond, ...)       do { if ( (cond) ) { MY_LOGD(__VA_ARGS__); } }while(0)
#define MY_LOGI_IF(cond, ...)       do { if ( (cond) ) { MY_LOGI(__VA_ARGS__); } }while(0)
#define MY_LOGW_IF(cond, ...)       do { if ( (cond) ) { MY_LOGW(__VA_ARGS__); } }while(0)
#define MY_LOGE_IF(cond, ...)       do { if ( (cond) ) { MY_LOGE(__VA_ARGS__); } }while(0)
#define MY_LOGA_IF(cond, ...)       do { if ( (cond) ) { MY_LOGA(__VA_ARGS__); } }while(0)
#define MY_LOGF_IF(cond, ...)       do { if ( (cond) ) { MY_LOGF(__VA_ARGS__); } }while(0)


/******************************************************************************
 *
 ******************************************************************************/
ImgBufCustom*
ImgBufCustom::
alloc(
    camera_request_memory   requestMemory, 
    sp<ImgInfo const>const& rpImgInfo
)
{
    ImgBufCustom* pImgBufCustom = NULL;
    camera_memory_t*   camera_memory = NULL;
    sp<MemoryHeapBase> pMemHeapBase = new MemoryHeapBase(rpImgInfo->mImgBufSize, 0, rpImgInfo->ms8ImgName);
    if  ( pMemHeapBase == 0 )
    {
        CAM_LOGE("[ImgBufCustom::alloc] cannot new MemoryHeapBase");
        goto lbExit;
    }
    //
    camera_memory = requestMemory(pMemHeapBase->getHeapID(), rpImgInfo->mImgBufSize, 1, NULL);
    if  ( ! camera_memory )
    {
        CAM_LOGE("[requestMemory] id:%d, size:%d", pMemHeapBase->getHeapID(), rpImgInfo->mImgBufSize);
        goto lbExit;
    }
    //
    pMemHeapBase = 0;
    pImgBufCustom = new ImgBufCustom(*camera_memory, rpImgInfo);
    //
lbExit:
    return  pImgBufCustom;
}


/******************************************************************************
 *
 ******************************************************************************/
ImgBufCustom::
ImgBufCustom(
    camera_memory_t const&  rCamMem, 
    sp<ImgInfo const>const& rpImgInfo, 
    int64_t const timestamp /*= 0*/
)
    : ICameraImgBuf()
    , mpImgInfo(rpImgInfo)
    , mi8Timestamp(timestamp)
    , mCamMem(rCamMem)
{
    CAM_LOGD("[ImgBufCustom::ImgBufCustom]");
}


/******************************************************************************
 *
 ******************************************************************************/
ImgBufCustom::
~ImgBufCustom()
{
    CAM_LOGD(
        "[ImgBufCustom::~ImgBufCustom] "
        "Buffer[%s@0x%08X@%d@%s@(%d)%dx%d@%d@Timestamp(%lld)] - mCamMem.release(%p)", 
        getBufName(), getVirAddr(), getBufSize(), getImgFormat().string(), 
        getImgWidthStride(), getImgWidth(), getImgHeight(), 
        getBitsPerPixel(), getTimestamp(), 
        mCamMem.release
    );
    //
    if  ( mCamMem.release )
    {
        mCamMem.release(&mCamMem);
        mCamMem.release = NULL;
    }
}


/******************************************************************************
 *
 ******************************************************************************/
void
ImgBufCustom::
dump() const
{
    CAM_LOGD(
        "[ImgBufCustom::dump] "
        "Buffer[%s@0x%08X@%d@%s@(%d)%dx%d@%d@Timestamp(%lld)]", 
        getBufName(), getVirAddr(), getBufSize(), getImgFormat().string(), 
        getImgWidthStride(), getImgWidth(), getImgHeight(), 
        getBitsPerPixel(), getTimestamp()
    );
}


/******************************************************************************
 *
 ******************************************************************************/
ImgBufCustomMgr*
ImgBufCustomMgr::
alloc(
    char const*const        szImgFormat, 
    uint32_t const          u4ImgWidth, 
    uint32_t const          u4ImgHeight, 
    uint32_t const          u4BufCount, 
    char const*const        szName, 
    camera_request_memory   requestMemory
)
{
    ImgBufCustomMgr* pMgr = new ImgBufCustomMgr(
        szImgFormat, u4ImgWidth, u4ImgHeight, 
        u4BufCount, szName, requestMemory
    );
    //
    if  ( pMgr && ! pMgr->init() )
    {
        // return NULL due to init failure.
        pMgr = NULL;
    }
    //
    return pMgr;
}


/******************************************************************************
 *
 ******************************************************************************/
ImgBufCustomMgr::
ImgBufCustomMgr(
    char const*const        szImgFormat, 
    uint32_t const          u4ImgWidth, 
    uint32_t const          u4ImgHeight, 
    uint32_t const          u4BufCount, 
    char const*const        szName, 
    camera_request_memory   requestMemory
)
    : RefBase()
    //
    , ms8Name(szName)
    , ms8ImgFormat(szImgFormat)
    , mu4ImgWidth(u4ImgWidth)
    , mu4ImgHeight(u4ImgHeight)
    , mu4BufCount(u4BufCount)
    //
    , mvImgBuf()
    , mRequestMemory(requestMemory)
    //
{
    MY_LOGD("");
}


/******************************************************************************
 *
 ******************************************************************************/
ImgBufCustomMgr::
~ImgBufCustomMgr()
{
    uninit();
    //
    MY_LOGD("");
}


/******************************************************************************
 *
 ******************************************************************************/
bool
ImgBufCustomMgr::
init()
{
    MY_LOGD("+ mu4BufCount(%d)", mu4BufCount);
    //
    mvImgBuf.clear();
    for (size_t i = 0; i < mu4BufCount; i++)
    {
        ImgBufCustom* pImgBufCustom = ImgBufCustom::alloc(
            mRequestMemory, 
            new ImgInfo(
                mu4ImgWidth, 
                mu4ImgHeight, 
                ms8ImgFormat, 
                ms8Name
            )
        );
        if  ( pImgBufCustom == 0 )
        {
            MY_LOGE("cannot allocate pImgBufCustom [%d]", i);
            goto lbExit;
        }
        //
        pImgBufCustom->dump();
        mvImgBuf.push_back(pImgBufCustom);
        if  ( mvImgBuf[i] == 0 )
        {
            MY_LOGE("cannot allocate mvImgBuf[%d]", i);
            goto lbExit;
        }
    }
    //
    //
    MY_LOGD("- ret(1)");
    return true;
lbExit:
    MY_LOGD("- ret(0)");
    uninit();
    return false;
}


/******************************************************************************
 *
 ******************************************************************************/
void
ImgBufCustomMgr::
uninit()
{
    MY_LOGD("+ mu4BufCount(%d)", mu4BufCount);
    //
    for (size_t i = 0; i < mu4BufCount; i++)
    {
        mvImgBuf.editItemAt(i) = NULL;
    }
    //
    MY_LOGD("-");
}

