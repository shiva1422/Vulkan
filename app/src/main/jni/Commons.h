//
// Created by pspr1 on 4/10/2021.
//

#ifndef VULKAN_COMMONS_H
#define VULKAN_COMMONS_H
#include <stdint.h>
#include "android/log.h"
typedef int32_t int32 ;
#define Loge(...)((void)__android_log_print(ANDROID_LOG_ERROR,__VA_ARGS__))
class DisplayParams{
public:
    static float screenWidth,screenHeight,densityDpi,deviceStableDensity,screenStride,density,scaledDensity,xdpi,ydpi;
    static void print();

};


#endif //VULKAN_COMMONS_H
