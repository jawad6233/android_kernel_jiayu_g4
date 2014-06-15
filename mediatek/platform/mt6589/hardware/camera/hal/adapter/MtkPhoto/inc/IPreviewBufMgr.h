#ifndef I_PRV_BUF_MGR
#define I_PRV_BUF_MGR
//
#include <utils/String8.h>
//
//
namespace android {
namespace NSMtkPhotoCamAdapter {
//
/******************************************************************************
*
*******************************************************************************/
class IPreviewBufMgrHandler : public virtual RefBase
{
public:    
    virtual                ~IPreviewBufMgrHandler() {}

public:
    virtual bool            dequeBuffer(int ePort, ImgBufQueNode &node) = 0;
    virtual bool            enqueBuffer(ImgBufQueNode const& node)= 0;
    virtual void            allocBuffer(int w, int h, const char* format, int cnt) = 0; 
    virtual void            freeBuffer() = 0;
};

/******************************************************************************
 *
 ******************************************************************************/
class IPreviewBufMgr : public IPreviewBufMgrHandler
{
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Member Enum
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
public:
    enum EBufProvider{
        eBuf_Unknown,
        eBuf_Pass1,
        eBuf_Disp,
        eBuf_AP,
        eBuf_FD,
        eBuf_Rec,
        eBuf_Generic,
    /* xinlan.he@Prd.Camera.CameraSrv for: CUSTOM_PROCESS */
    #ifdef VENDOR_EDIT
        eBuf_Custom,
        eBuf_Custom2,
        eBuf_Custom3,
        eBuf_Custom4,
    #endif
    };

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  IPreviewCmdQueThreadHandler Interfaces.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
public:    
    
    virtual bool            dequeBuffer(int ePort, ImgBufQueNode &node) = 0;
    virtual bool            enqueBuffer(ImgBufQueNode const& node)= 0;
    virtual void            allocBuffer(int w, int h, const char* format, int cnt) = 0; 
    virtual void            freeBuffer() = 0;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Operations.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++    
public:

    static IPreviewBufMgr*  createInstance(sp<ImgBufProvidersManager>& rImgBufProvidersMgr);
    virtual void            destroyInstance() = 0;
    virtual                 ~IPreviewBufMgr(){};
};

};  // namespace NSMtkPhotoCamAdapter
};  // namespace android

#endif
