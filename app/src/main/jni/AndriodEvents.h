//
// Created by pspr1 on 4/20/2021.
//

#ifndef VULKAN_ANDRIODEVENTS_H
#define VULKAN_ANDRIODEVENTS_H
#include <android_native_app_glue.h>
#include "Commons.h"

class AndriodEvents {

public:
    static int32_t onInputEvent(android_app* papp, AInputEvent* event);
    static void onAppCmd(android_app* app, int32_t cmd);

};


#endif //VULKAN_ANDRIODEVENTS_H
