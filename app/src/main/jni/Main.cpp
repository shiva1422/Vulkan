//
// Created by pspr1 on 4/10/2021.
//


#include <android_native_app_glue.h>
#include "android/log.h"
#include "Commons.h"
#include "AndriodEvents.h"
#include "AppContext.h"
#define Loge(...)((void)__android_log_print(ANDROID_LOG_ERROR,__VA_ARGS__))

void android_main(android_app *app)
{
    AppContext appContext(app);
    app->userData = (void *)&appContext;


    //
    app->onAppCmd=AndriodEvents::onAppCmd;
    app->onInputEvent=AndriodEvents::onInputEvent;
    int32 events,eventId,fdesc;
    android_poll_source* source;

    do{

        //if ? while?
        while((eventId=ALooper_pollAll(0,&fdesc,&events,(void **)&source))>=0)
        {
            Loge("processing","event");

            if(source)
            {
                Loge("processing","event");
                source->process(app,source);

            }
        }
      //  Loge("VulkanMain","hello %f",DisplayParams::screenHeight);

    }while(app->destroyRequested==0);

}
