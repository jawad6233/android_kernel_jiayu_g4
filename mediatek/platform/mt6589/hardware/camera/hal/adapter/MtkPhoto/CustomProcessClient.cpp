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
 *    wuhongru        2013-02-18        add docskinbeauty
 *************************************************************/ 
#define LOG_TAG "MtkCam/CustomProcessClient"

#include <inc/CamUtils.h>
using namespace android;
using namespace MtkCamUtils;

#include <inc/ImgBufProvidersManager.h>
#include <hwscenario/IhwScenarioType.h>
using namespace NSHwScenario;

#include <system/camera.h>

#include "camera/MtkCamera.h"
#include <inc/ICustomProcess.h>
#include <ImgBufCustomMgr.h>
using namespace NSCamAdapterImgMgr;

#include <sys/prctl.h>
#include <sys/resource.h>
#include <cutils/properties.h>

#include <drv/hwutils.h>

#include <oppo/ICameraHacker.h>
#include <oppo/PhotoArtHacker.h>
#include <oppo/doc_hacker.h>

//
using namespace NSCustomProcess;
//
/******************************************************************************
*
*******************************************************************************/
#define ENABLE_LOG_PER_FRAME        (1)

/******************************************************************************
 *
 ******************************************************************************/
namespace android {
namespace NSCustomProcess {
/******************************************************************************
 *
 ******************************************************************************/
class CustomProcessClient : public ICustomProcess, public Thread
{

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Operations in base class Thread
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
public:     ////
    // Ask this object's thread to exit. This function is asynchronous, when the
    // function returns the thread might still be running. Of course, this
    // function can be called from a different thread.
    virtual void                    requestExit();

    // Good place to do one-time initializations
    virtual status_t                readyToRun();
protected:
    struct Command
    {
        //  Command ID.
        enum CustomID
        {
            eID_UNKNOWN,
            eID_EXIT, 
            eID_WAKEUP, 
        };
    };
    virtual void                    postCommand(Command::CustomID cmd);
    virtual bool                    getCommand(Command::CustomID &cmd);

    enum CamerHacker
    {
        HACKER_SCALADO,
        HACKER_DOC, 
        HACKER_TOTAL, 
    };
private:
    // Derived class must implement threadLoop(). The thread starts its life
    // here. There are two ways of using the Thread object:
    // 1) loop: if threadLoop() returns true, it will be called again if
    //          requestExit() wasn't called.
    // 2) once: if threadLoop() returns false, the thread will exit upon return.
    virtual bool                    threadLoop();
    
protected:  ////                    Data Members.
    List<Command::CustomID>         mCmdQue;
    Mutex                           mCmdQueMtx;
    Condition                       mCmdQueCond;    //  Condition to wait: [ ! exitPending() && mCmdQue.empty() ]
    int32_t                         mi4ThreadId;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Operations.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++    
public:
    CustomProcessClient(sp<IParamsManager> pParamsMgr, 
                        sp<IPreviewBufMgrHandler> pPreviewBufHandler,
                        int32_t openId,
                        int32_t queueId, 
                        char* queueName,
                        int32_t nodeCnt);
    virtual ~CustomProcessClient();

    bool waitBuffers(sp<IImgBufQueue>const& rpBufQueue, Vector<ImgBufQueNode>& vQueNode);
    bool handleReturnBuffers(Vector<ImgBufQueNode>& rvQueNode);
    
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //  ICustomProcess Interfaces.
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    virtual bool                    init();
    virtual bool                    uninit();

    virtual bool                    setImgBufProviderClient(
                                        sp<IImgBufProviderClient>const& rpClient
                                    );

    virtual void                    setCallbacks(
                                        sp<CamMsgCbInfo> const& rpCamMsgCbInfo
                                    );

    virtual status_t                sendCommand(int32_t cmd, int32_t arg1, int32_t arg2);
    virtual bool                    startPreview();
    virtual bool                    stopPreview();
    virtual bool                    takePicture();
    bool                            isAliveState();
    
private:
    void                            onThreadLoop();
    bool                            initBuffers();
    void                            uninitBuffers();
    void                            getCameraState(ICameraHacker::CamState& camState);
    bool                            splitInt(String8 const& s8Input, Vector<int>& rOutput);
    static bool                     pushBufferToDispo(sp<IPreviewBufMgrHandler> handler,                                            
                                                      sp<ICameraHacker> pCameraHacker,
                                                      nsecs_t nsecs,
                                                      int32_t idx);

private:
    sp<IPreviewBufMgrHandler>       mspPreviewBufHandler;
    sp<CamMsgCbInfo>                mpCamMsgCbInfo; //  
    sp<IParamsManager>              mpParamsMgr;
    int32_t const                   mi4OpenId;
    
     //  Pointer to IImgBufQueue.
    sp<IImgBufQueue>                mpImgBufQueue;
    //  Pointer to the client of Image Buffer Provider (i.e. a client is a provider-IF user of mpImgBufQueue).
    sp<IImgBufProviderClient>       mpImgBufPvdrClient;
    
    int32_t                         miLogLevel; //0: silence, 1: debug1, 2: debug2
    volatile int32_t                mIsProcessAlive;

    int32_t                         mProcessIsused;

    sp<ICameraHacker>               mspCameraHacker[HACKER_TOTAL];

    sp<ImgBufCustomMgr>             mspImgBufCustomMgr;

    int                             mCurrHackerID;

    int32_t const                   mi4QueueId;
    char const*const                mpszQueueName;
    int32_t const                   mi4NodeCnt;
    
    static Mutex                    mPushDispMtx;
    static int32_t                  mFrameOrdNum;
};

};
};

/******************************************************************************
 *
 ******************************************************************************/
sp<ICustomProcess> ICustomProcess::createInstance(sp<IParamsManager> pParamsMgr, 
     sp<IPreviewBufMgrHandler> pPreviewBufHandler, 
     int32_t openId, 
     int32_t queueId, 
     char* queueName,
     int32_t nodeCnt)
{
    return new CustomProcessClient(pParamsMgr, pPreviewBufHandler, openId, queueId, queueName, nodeCnt);
}


Mutex CustomProcessClient::mPushDispMtx;
int32_t CustomProcessClient::mFrameOrdNum = 0;
/******************************************************************************
 *
 ******************************************************************************/
CustomProcessClient::CustomProcessClient(sp<IParamsManager> pParamsMgr, 
                                         sp<IPreviewBufMgrHandler> pPreviewBufHandler, 
                                         int32_t openId, 
                                         int32_t queueId, 
                                         char* queueName,
                                         int32_t nodeCnt )
    : mCmdQue()
    , mCmdQueMtx()
    , mCmdQueCond()
    , mi4ThreadId(0)
    , mspPreviewBufHandler(pPreviewBufHandler)
    , mpCamMsgCbInfo(new CamMsgCbInfo)
    , mpParamsMgr(pParamsMgr)
    , mi4OpenId(openId)
    , mi4QueueId(queueId)
    , mpszQueueName(queueName)
    , mi4NodeCnt(nodeCnt)
    //
    , mpImgBufQueue(NULL)
    , mpImgBufPvdrClient(NULL)
    , miLogLevel(1)
    , mIsProcessAlive(0)
    , mProcessIsused(0)
    //
    , mspImgBufCustomMgr(NULL)
    , mCurrHackerID(0xff)
{
    //MY_LOGD("+");
    //MY_LOGD("+ this(%p)", this);
    //char cLogLevel[10] = {'\0'};
    //::property_get("debug.camera.display.loglevel", cLogLevel, "1");
    //miLogLevel = ::atoi(cLogLevel);
    //MY_LOGD("- debug.camera.display.loglevel=%s", cLogLevel);
    for (int i = 0; i < HACKER_TOTAL; i++)
    {
        mspCameraHacker[i] = NULL;
    }
    
    mspCameraHacker[HACKER_SCALADO] = new PhotoArtHacker(mi4OpenId);
    mspCameraHacker[HACKER_DOC] = new DocHacker(mi4OpenId);
}


/******************************************************************************
 *
 ******************************************************************************/
CustomProcessClient::~CustomProcessClient()
{
    MY_LOGD("+");
    
    mspPreviewBufHandler = NULL;
    mpCamMsgCbInfo = NULL;
    mpParamsMgr = NULL;
    mpImgBufQueue = NULL;

    MY_LOGD("-");
}


/******************************************************************************
 *
 ******************************************************************************/
bool CustomProcessClient::init()
{
    MY_LOGD("+");
    bool ret = true;

    //mpImgBufQueue = new ImgBufQueue(IImgBufProvider::eID_CUSTOM, "CustomProcess@ImgBufQue");
    mpImgBufQueue = new ImgBufQueue(mi4QueueId, mpszQueueName);
    if  ( mpImgBufQueue == 0 )
    {
        MY_LOGE("Fail to new ImgBufQueue");
        ret = false;
        goto lbExit;
    }
    
lbExit:
    MY_LOGD("-");
    return  ret;
}


/******************************************************************************
 *
 ******************************************************************************/
bool CustomProcessClient::uninit()
{
    sp<IImgBufQueue> pImgBufQueue;
    MY_LOGD("+");

    if ((!isAliveState())&&(mProcessIsused == 0))
    {
        MY_LOGW("is not alive state");
        goto lbExit;
    }

    {
        //Mutex::Autolock _l(mModuleMtx);
        pImgBufQueue = mpImgBufQueue;
    }

    if  ( pImgBufQueue != 0 )
    {
        pImgBufQueue->stopProcessor();
        pImgBufQueue = NULL;
    }

    ::android_atomic_write(0, &mIsProcessAlive);
    
    {
        MY_LOGD("getThreadId(%d), getStrongCount(%d), this(%p)", getThreadId(), getStrongCount(), this);
        //  Notes:
        //  requestExitAndWait() in ICS has bugs. Use requestExit()/join() instead.
        requestExit();
        status_t status = join();
        if  ( OK != status )
        {
            MY_LOGW("Not to wait thread(tid:%d), status[%s(%d)]", getThreadId(), ::strerror(-status), -status);
        }
        MY_LOGD("join() exit");
    }   
  
lbExit:    
    if  ( mpImgBufPvdrClient != 0 )
    {
        mpImgBufPvdrClient->onImgBufProviderDestroyed(mpImgBufQueue->getProviderId());
        mpImgBufPvdrClient = NULL;
    }
    
    
    if (mi4QueueId == IImgBufProvider::eID_CUSTOM 
        && mCurrHackerID != 0xff
        && mspCameraHacker[mCurrHackerID] != NULL) 
    {    
        mspCameraHacker[mCurrHackerID]->onCustomCallback(ICameraHacker::ICAMERA_MSG_DO_ON_DESTORY,0,0);
    }

    for (int i = 0; i < HACKER_TOTAL; i++)
    {
        mspCameraHacker[i] = NULL;
    }

    mspImgBufCustomMgr = 0;

    mProcessIsused = 0;

    MY_LOGD("-");
    return  true;
}

/******************************************************************************
 *
 ******************************************************************************/
bool CustomProcessClient::initBuffers()
{
    status_t status;
    ImgBufQueNode * rQueNode = NULL;
    bool ret = false;
    
    MY_LOGD("+");

    if (isAliveState()) 
    {
        MY_LOGW("CustomProcessClient is alive!");
        ret = true;
        goto lbExit;
    }
    int32_t prvWidth, prvHeight;

    mProcessIsused = 1;
    
    mpParamsMgr->getPreviewSize(&prvWidth, &prvHeight);
    mspImgBufCustomMgr = ImgBufCustomMgr::alloc(
        String8(CameraParameters::PIXEL_FORMAT_YUV420P),
        prvWidth, 
        prvHeight,
        mi4NodeCnt, 
        "CustomBuff", 
        mpCamMsgCbInfo->mRequestMemory
    );
    if  ( mspImgBufCustomMgr == 0 )
    {
        MY_LOGE("ImgBufCustomMgr::alloc() fail");
        goto lbExit;
    }
    
    mpImgBufQueue->flushProcessor();
    mpImgBufQueue->stopProcessor(); 
    for (int i=0; i<mi4NodeCnt; i++)
    {
        ret = mpImgBufQueue->enqueProcessor(
            ImgBufQueNode(mspImgBufCustomMgr->getBuf(i), ImgBufQueNode::eSTATUS_TODO));
            
        if  ( ! ret ) 
        {
            MY_LOGW("enqueProcessor() fails");
            goto lbExit;
        }    
    }
    
    status = run();
    if ( INVALID_OPERATION == status)
    {
        MY_LOGW("run(): CustomProcessClient is running");
    }
    else if ( OK != status )
    {
        MY_LOGE("Fail to run CustomProcessClient thread, status[%s(%d)]", ::strerror(-status), -status);
        ret = false;
        goto lbExit;
    }

    ::android_atomic_write(1, &mIsProcessAlive);
    postCommand(Command::eID_WAKEUP);
    
    ret = true;
lbExit:
    MY_LOGD("- ret(%d)", ret);
    return ret;
}

/******************************************************************************
 *
 ******************************************************************************/
void CustomProcessClient::uninitBuffers()
{    
    ::android_atomic_write(0, &mIsProcessAlive);
    sp<IImgBufQueue> pImgBufQueue;
    {
        //Mutex::Autolock _l(mModuleMtx);
        pImgBufQueue = mpImgBufQueue;
    }

    if  ( pImgBufQueue != 0 )
    {
        pImgBufQueue->pauseProcessor(); 
        pImgBufQueue->flushProcessor(); // clear "TODO"
        pImgBufQueue->stopProcessor();  // clear "DONE"
        pImgBufQueue = NULL;
    }

    mspImgBufCustomMgr = 0;
}

/******************************************************************************
 *
 ******************************************************************************/
bool CustomProcessClient::setImgBufProviderClient(sp<IImgBufProviderClient>const& rpClient)
{
    bool ret = false;

    MY_LOGD("+ ImgBufProviderClient(%p)", rpClient.get());

    if  ( rpClient == 0 )
    {
        MY_LOGE("NULL ImgBufProviderClient");
        goto lbExit;
    }

    if  ( mpImgBufQueue == 0 )
    {
        MY_LOGE("NULL ImgBufQueue");
        goto lbExit;
    }

    if  ( ! rpClient->onImgBufProviderCreated(mpImgBufQueue) )
    {
        goto lbExit;
    }
    mpImgBufPvdrClient = rpClient;

    ret = true;
lbExit:
    MY_LOGD("-");
    return  ret;
}

/******************************************************************************
 * Set camera message-callback information.
 ******************************************************************************/
void CustomProcessClient::setCallbacks(sp<CamMsgCbInfo> const& rpCamMsgCbInfo)
{
    //  value copy
    *mpCamMsgCbInfo = *rpCamMsgCbInfo;
    MY_LOGV("setCallBacks");
}

/******************************************************************************
 *
 ******************************************************************************/
status_t CustomProcessClient::sendCommand(int32_t cmd, int32_t arg1, int32_t arg2)
{
    return INVALID_OPERATION;
}

/******************************************************************************
 *
 ******************************************************************************/
bool CustomProcessClient::startPreview()
{
    switch(mpParamsMgr->getShotMode())
    {
    case eShotMode_scaladoLomo:
        mCurrHackerID = HACKER_SCALADO;
        break;
    case eShotMode_docSkinbeauty:
        mCurrHackerID = HACKER_DOC;
        break;
    default:
        mCurrHackerID = 0xff;
        MY_LOGW("there no hacker to process!!");
        break;        
    } 
    if (mCurrHackerID != 0xff) 
    {
        initBuffers();
    }
    
    return true;
}

/******************************************************************************
 *
 ******************************************************************************/
bool CustomProcessClient::stopPreview()
{
    if (mCurrHackerID != 0xff) 
    {
        uninitBuffers();
    }
     
    return true;
}

/******************************************************************************
 *
 ******************************************************************************/
bool CustomProcessClient::takePicture()
{
    if (mi4QueueId == IImgBufProvider::eID_CUSTOM 
        && mCurrHackerID != 0xff
        && mspCameraHacker[mCurrHackerID] != NULL) 
    {
        mspCameraHacker[mCurrHackerID]->onCustomCallback(ICameraHacker::ICAMERA_MSG_DO_ON_CAPTURE,0,0);
    }

    return true;
}

/******************************************************************************
 *
 ******************************************************************************/
// Ask this object's thread to exit. This function is asynchronous, when the
// function returns the thread might still be running. Of course, this
// function can be called from a different thread.
void CustomProcessClient::requestExit()
{
    MY_LOGD("+");
    
    Thread::requestExit();
    postCommand(Command::eID_EXIT);
    
    MY_LOGD("-");
}

/******************************************************************************
 *
 ******************************************************************************/
// Good place to do one-time initializations
status_t CustomProcessClient::readyToRun()
{
    ::prctl(PR_SET_NAME,"CustomProcess@Preview", 0, 0, 0);

    mi4ThreadId = ::gettid();

    //  thread policy & priority
    //  Notes:
    //      Even if pthread_create() with SCHED_OTHER policy, a newly-created thread 
    //      may inherit the non-SCHED_OTHER policy & priority of the thread creator.
    //      And thus, we must set the expected policy & priority after a thread creation.
    int const policy    = SCHED_OTHER;
    int const priority  = 19;

    struct sched_param sched_p;
    ::sched_getparam(0, &sched_p);
    sched_p.sched_priority = priority;  //  Note: "priority" is nice value
    sched_setscheduler(0, policy, &sched_p);    
    setpriority(PRIO_PROCESS, 0, priority); 

    MY_LOGD(
        "policy:(expect, result)=(%d, %d), priority:(expect, result)=(%d, %d)"
        , policy, ::sched_getscheduler(0)
        , priority, getpriority(PRIO_PROCESS, 0)
    );
    
    return NO_ERROR;
}

/******************************************************************************
 *
 ******************************************************************************/
void CustomProcessClient::postCommand(Command::CustomID cmd)
{
    Mutex::Autolock _lock(mCmdQueMtx);
    //
    if  ( ! mCmdQue.empty() )
    {
        Command::CustomID const& rBegCmd = *mCmdQue.begin();
        MY_LOGW("que size:%d > 0 with begin cmd::%d", mCmdQue.size(), rBegCmd);
    }
    //
    mCmdQue.push_back(cmd);
    mCmdQueCond.broadcast();
    //
    MY_LOGD("- new command::%d", cmd);
}

/******************************************************************************
 *
 ******************************************************************************/
bool CustomProcessClient::getCommand(Command::CustomID &cmd)
{
    bool ret = false;
    //
    Mutex::Autolock _lock(mCmdQueMtx);
    //
    MY_LOGD_IF(ENABLE_LOG_PER_FRAME, "+ que size(%d)", mCmdQue.size());
    //
    //  Wait until the queue is not empty or this thread will exit.
    while   ( mCmdQue.empty() && ! exitPending() )
    {
        status_t status = mCmdQueCond.wait(mCmdQueMtx);
        if  ( NO_ERROR != status )
        {
            MY_LOGW("wait status(%d), que size(%d), exitPending(%d)", 
                    status, mCmdQue.size(), exitPending());
        }
    }
    //
    if  ( ! mCmdQue.empty() )
    {
        //  If the queue is not empty, take the first command from the queue.
        ret = true;
        cmd = *mCmdQue.begin();
        mCmdQue.erase(mCmdQue.begin());
        MY_LOGD("command:%d", cmd);
    }
    //
    MY_LOGD_IF(ENABLE_LOG_PER_FRAME, "- que size(%d), ret(%d)", mCmdQue.size(), ret);
    return  ret;
}

/******************************************************************************
 *
 ******************************************************************************/
bool CustomProcessClient::isAliveState()
{
    return  0 != ::android_atomic_release_load(&mIsProcessAlive);
}

/******************************************************************************
 *
 ******************************************************************************/
bool CustomProcessClient::threadLoop()
{
    Command::CustomID cmd;
    
    MY_LOGD("+");
    
    if  ( getCommand(cmd) )
    {
        switch  (cmd)
        {
        case Command::eID_WAKEUP:
            onThreadLoop();
            break;
        //
        case Command::eID_EXIT:            
            break;
        default:
            MY_LOGD("Command::%d", cmd);
            break;
        }
    }
    //
    MY_LOGD("-");
    return  true;
}
/******************************************************************************
 *
 ******************************************************************************/
void CustomProcessClient::onThreadLoop()
{
    MY_LOGD("+");

    sp<IImgBufQueue> pBufQueue = NULL;
    {
        pBufQueue = mpImgBufQueue;
        if ( pBufQueue == 0 )
        {
            MY_LOGE("pBufMgr(%p)", pBufQueue.get());
            return; 
        }
    }
    
    if  ( !pBufQueue->startProcessor() )
    {
        MY_LOGE("startProcessor failed");
        return;
    }
	
    Vector<ImgBufQueNode> rQueNode;
	
    while (isAliveState())
    {
        if ( waitBuffers(pBufQueue, rQueNode) )
        {
            MY_LOGD("No available deque-ed buffer; to be leaving");
            continue;
        }

        handleReturnBuffers(rQueNode); 
    }
	
    rQueNode.clear();

    pBufQueue->pauseProcessor(); 
    pBufQueue->flushProcessor(); // clear "TODO"
    pBufQueue->stopProcessor();  // clear "DONE"
    
    MY_LOGD("-");
}

/******************************************************************************
 *
 ******************************************************************************/
bool CustomProcessClient::pushBufferToDispo(sp<IPreviewBufMgrHandler> handler,                                            
                                            sp<ICameraHacker> pCameraHacker,
                                            nsecs_t nsecs, 
                                            int32_t idx)
{
    Mutex::Autolock _l(mPushDispMtx);
    
    ImgBufQueNode dispNode;

    if (idx == 0) 
    {
        mFrameOrdNum = idx;
    } 
    else if (idx > mFrameOrdNum) 
    {
        mFrameOrdNum = idx;
    }
    else 
    {
        MY_LOGD("This frame is late, just ignore it!!");
        return false;
    }


    if (handler == NULL || pCameraHacker == NULL) 
    {
        MY_LOGE("wrong params!!");
        return false;
    }

    //zhiquan.huang@MMApp.Camera, 2013-04-11 Add for mtk
    MY_LOGD("handler->dequeBuffer(eID_Pass2DISPO, dispNode) add for mtk");

    if (handler->dequeBuffer(eID_Pass2DISPO, dispNode)) 
    {
        //zhiquan.huang@MMApp.Camera, 2013-04-11 Add for mtk
        MY_LOGD("handler->dequeBuffer(eID_Pass2DISPO, dispNode) success! add for mtk"); 

        if (pCameraHacker->getProcessedMem((dispNode.getImgBuf())->getVirAddr(), (dispNode.getImgBuf())->getBufSize()))
        {
            dispNode.getImgBuf()->setTimestamp(nsecs);
        
            globalcacheFlushAll(); 
            handler->enqueBuffer(dispNode); 
            return true;
        }
        else 
        {
            MY_LOGE("get mem fail! bufsize(%d)", (dispNode.getImgBuf())->getBufSize()); 
            handler->enqueBuffer(dispNode); 
        }
    }

    return false;
}

/******************************************************************************
 *
 ******************************************************************************/
bool CustomProcessClient::waitBuffers(sp<IImgBufQueue>const& rpBufQueue, 
                                      Vector<ImgBufQueNode>& vQueNode)
{
    bool ret = false;
    MY_LOGD_IF((2<=miLogLevel), "+");

    rpBufQueue->dequeProcessor(vQueNode);
    if  ( vQueNode.empty() ) 
    {
        MY_LOGE("vQueNode.empty()");
        ret = true;
    }
    
    MY_LOGD_IF((2<=miLogLevel), "- ret(%d)", ret);
    return ret;
}

/******************************************************************************
 * Custom process the node and push to display
 ******************************************************************************/
bool CustomProcessClient::handleReturnBuffers(Vector<ImgBufQueNode>& rvQueNode)
{
    int32_t idxToProcess = 0;
    int32_t const queSize = rvQueNode.size();
    ICameraHacker::CamState camState;
    
    MY_LOGD_IF((2<=miLogLevel), "+");

    for ( idxToProcess = queSize - 1; idxToProcess >= 0; idxToProcess-- )
    {
        if ( rvQueNode[idxToProcess].isDONE() )
        {
            break;
        }
    }

    getCameraState(camState);
    
    for (int32_t i = 0; i < queSize; i++)
    {
        ImgBufQueNode      rQueNode    = rvQueNode[i];
        sp<IImgBuf>const&  rpQueImgBuf = rQueNode.getImgBuf();  
        
        if  ( i == idxToProcess ) 
        {
            ImgBufQueNode dispNode;
            ImgBufQueNode prvCbNode;
            sp<ICameraHacker> pCameraHacker = mspCameraHacker[mCurrHackerID];         

            if (pCameraHacker != NULL) 
            {
                MY_LOGD_IF((2<=miLogLevel), "before process zoom(%d) caps_effect(%d)", 
                    camState.zoom, camState.caps_effect);
                
                int32_t i4startMs = ::ns2ms(::systemTime());
                int32_t i4costMs = 0;
                
                pCameraHacker->onReceivePreviewFrame(rpQueImgBuf->getVirAddr(), camState);
                
                i4costMs = ::ns2ms(::systemTime()) - i4startMs;               
                MY_LOGD_IF((1<=miLogLevel), "after process Idx=%d time=%d", 
                        rQueNode.getCookieED(), i4costMs);
                if (!isAliveState()) 
                {
                    MY_LOGD_IF((1<=miLogLevel), "It is not alive, donot display!");
                } 
                else 
                {
                    if (!pushBufferToDispo(mspPreviewBufHandler, 
                                           pCameraHacker, 
                                           rpQueImgBuf->getTimestamp(), 
                                           rQueNode.getCookieED())) 
                    {
                        MY_LOGW("push to display fail!");
                    }

                    if (mspPreviewBufHandler->dequeBuffer(eID_Pass2VIDO, prvCbNode)) 
                    {
                        pCameraHacker->getProcessedMem((prvCbNode.getImgBuf())->getVirAddr(), 
                                                       (prvCbNode.getImgBuf())->getBufSize());
                        //prvCbNode.getImgBuf()->setTimestamp(rpQueImgBuf->getTimestamp());
                        mspPreviewBufHandler->enqueBuffer(prvCbNode); 
                    }
                }
                pCameraHacker = NULL;
                MY_LOGD_IF((2<=miLogLevel), "after mem copy");
            }
            else 
            {
                MY_LOGE_IF((2<=miLogLevel), "There is no custom process!!");
            }
        }
        
        rQueNode.setStatus(ImgBufQueNode::eSTATUS_TODO);
        if  ( !mpImgBufQueue->enqueProcessor(rQueNode)) 
        {
            MY_LOGE("enqueProcessor() fails");            
        } 
    }

    MY_LOGD_IF((2<=miLogLevel), "-");
    return  true;
}

/******************************************************************************
 *
 ******************************************************************************/
void CustomProcessClient::getCameraState(ICameraHacker::CamState& camState)
{
    mpParamsMgr->getPreviewSize(&camState.prvW, &camState.prvH);
    mpParamsMgr->getPictureSize(&camState.capW, &camState.capH);
    camState.zoom = mpParamsMgr->getZoomRatio();
    camState.caps_effect = mpParamsMgr->getInt(MtkCameraParameters::KEY_CAPS_EFFECT);
    camState.quadEffect = mpParamsMgr->getInt(MtkCameraParameters::KEY_QUAD_EFFECT);
    camState.blurCircleRatio = mpParamsMgr->getFloat(MtkCameraParameters::KEY_BLUR_CIRCLE_RATIO);
    camState.rotation = mpParamsMgr->getInt(CameraParameters::KEY_ROTATION);
    Vector<int> value;
    if (splitInt(mpParamsMgr->getStr(MtkCameraParameters::KEY_SURFACE_SIZE), value)) 
    {
	    camState.surfaceW = value[0];
	    camState.surfaceH = value[1];
	}
    if (splitInt(mpParamsMgr->getStr(MtkCameraParameters::KEY_BLUR_CENTER_POINT), value)) 
    {
    	camState.centerPointX = value[0];
	    camState.centerPointY = value[1];
	}
}

/******************************************************************************
 *
 ******************************************************************************/
bool CustomProcessClient::splitInt(String8 const& s8Input, Vector<int>& rOutput)
{
    rOutput.clear();

    if  ( s8Input.isEmpty() )
    {
        MY_LOGW("empty string");
        return  false;
    }

    char const*start = s8Input.string();
    char *end = NULL;
    do {
        int value = ::strtol(start, &end, 10);
        if  ( start == end ) 
        {
            MY_LOGD("no digits in str:%s", s8Input.string());
            return  false;
        }
        rOutput.push_back(value);
        //MY_LOGD("%d", value);
        start = end + 1;
    } while ( end && *end );

    return  (rOutput.size() > 0);
}
