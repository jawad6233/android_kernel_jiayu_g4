/*************************************************************
 * 
 * Copyright (C), 2008-2012, OPPO Mobile Comm Corp., Ltd
 * 
 * VENDOR_EDIT
 *
 * Date Created: 2012-06-12 11:10
 * 
 * Author: Lycan.Wang@Prd.Camera.CameraSrv
 * 
 * Description : NULL
 * 
 * ------------------ Revision History: ---------------------
 *      <author>        <date>          <desc>
 *
 *************************************************************/ 
#define LOG_TAG "utils"
#include <utils/Log.h>
#include "utils.h"
#include <fcntl.h>

namespace android {

long long getTimeMs() 
{
	struct timeval t1;
	long long ms;

	gettimeofday(&t1, NULL);
	ms = t1.tv_sec * 1000LL + t1.tv_usec / 1000;

	return ms;
}

void storeToFile(sp<IMemory> mem, const char * name) {
	MY_LOGV("OppoCameraHardware::storeToFile E");
	int file_fd_out = open(name, O_RDWR | O_CREAT, 0777);
	if (file_fd_out < 0) {
		MY_LOGE("cannot open file [%d]\n [%s]", file_fd_out, strerror(errno));

	} else {

		ssize_t offset;
		size_t size;
		sp<IMemoryHeap> heap = mem->getMemory(&offset, &size);

		int written = write(file_fd_out, heap->base() + offset, size);
		if (written < 0) {
			MY_LOGE("error in data write");
		}
	}
	close(file_fd_out);
	MY_LOGV("store to [%s]", name);
	MY_LOGV("OppoCameraHardware::storeToFile X");
}

//image blur out of a circle (AYUV4444 or ARGB8888)
void imageBlur(uint8_t *image, uint8_t level, int width, int height){
    AutoTimer l("imageBlur");
    uint8_t *imageBuf = new uint8_t[width * height * 4];
    int offset[5][5];
    int cnt, i, j;
    int radius = width / 4;

    for (i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            offset[i][j] = ((i - 2) * width + j - 2) * 4;
        }
    }

    for (cnt = 0; cnt < level; cnt++) {
        memcpy(imageBuf, image, width * height * 4);
        //mean filter
        for (i = 2; i < width - 2; i++) {
            for (j = 2; j < height - 2; j++) {
                uint8_t *addr = imageBuf + (j * width + i) * 4;

                //blur out of the circle
                if (((width >> 1) - i) * ((width >> 1) - i) + 
                        ((height >> 1) - j) * ((height >> 1) - j)
                        > radius * radius) {
                    for (int index = 1; index < 4; index++) {
                        *(image +(j * width + i) * 4 + index) = 
                            (
                             *(addr + offset[1][2] + index) +
                             *(addr + offset[2][1] + index) +
                             *(addr + offset[2][3] + index) +
                             *(addr + offset[3][2] + index) 
                            ) >> 2;
                    }
                }
            }
        }
    }
    delete [] imageBuf;
}

void mergeImage(uint8_t * a, uint8_t * b, uint8_t * c, uint8_t * d,
                uint8_t * _out, int width, int height)
{
    uint8_t *srcL0 = a;
    uint8_t *srcR0 = b;
    uint8_t *out = _out;

    for (int i = 0; i < height; ++i) {
        uint8_t *dst, *srcL, *srcR;
        dst = out + (i * width * 8);
        srcL = (i * width * 4) + srcL0;
        srcR = (i * width * 4) + srcR0;
        memcpy(dst, srcL, width * 4);
        memcpy(dst + (width * 4), srcR, width * 4);
    }
    srcL0 = c;
    srcR0 = d;
    out += (width * 2) * (height * 2) * 2;
    for (int i = 0; i < height; ++i) {
        uint8_t *dst, *srcL, *srcR;
        dst = out + (i * width * 8);
        srcL = (i * width * 4) + srcL0;
        srcR = (i * width * 4) + srcR0;
        memcpy(dst, srcL, width * 4);
        memcpy(dst + (width * 4), srcR, width * 4);
    }
    // draw spliter
    const int spliterWidth = 4;
    out = _out;
    // vertical spliter
    for (int i = 0; i < height * 2; ++i) {
        uint8_t *dst;
        dst = out + (i * width * 2 + width - spliterWidth / 2) * 4;
        int* iDst = (int*) dst;
        //memset(dst, 0, spliterWidth * 4);
        for (int j = 0; j < spliterWidth; ++j) {
            iDst[j] = 0x80800000;
        }
    }
    // horizontal spliter
    for (int i = height - spliterWidth / 2; i < height + spliterWidth / 2; ++i) {

        uint8_t *dst = out + i * width * 8;
        //memset(dst, 0, width * 8);
        int* iDst = (int*) dst;
        const int length = width * 2;
        for (int j = 0; j < length; ++j) {
            iDst[j] = 0x80800000;
        }
    }

}

SimpleMemoryHelper::SimpleMemoryHelper() :
    mSize(0), mBuffer(NULL) 
{
}
SimpleMemoryHelper::~SimpleMemoryHelper() {
    freeMemory();
    MY_LOGV("~SimpleMemoryHelper EX");
}

void SimpleMemoryHelper::getMemory(unsigned char ** pp, int size) {
    if (mSize != size) {
        freeMemory();
        mBuffer = new unsigned char[size];
        MY_LOGV("getMemory 0x%x", mBuffer);
        mSize = size;
    }
    *pp = mBuffer;

}
void SimpleMemoryHelper::freeMemory() {
    if (mBuffer != NULL) {
        MY_LOGV("freeMemory 0x%x", mBuffer);
        delete[] mBuffer;
        mBuffer = NULL;
    }
    mSize = 0;
}


}

