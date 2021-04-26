//
// Created by pspr1 on 4/10/2021.
//


#include <android_native_app_glue.h>
#include "android/log.h"
#include "Commons.h"
#include "vulkan/vulkan.h"
#include "AndriodEvents.h"
#define Loge(...)((void)__android_log_print(ANDROID_LOG_ERROR,__VA_ARGS__))

void android_main(android_app *app)
{
app->onAppCmd=&AndriodEvents::onCmd;
int32 events,eventId,fdesc;
android_poll_source* source;

    do {
        if((eventId=ALooper_pollAll(0,&fdesc,&events,(void **)&source))>=0)
        {
            if(source!=nullptr)
            {
                source->process(app,source);

            }
        }
      //  Loge("VulkanMain","hello %f",DisplayParams::screenHeight);

    }while(app->destroyRequested==0);

}
