//
// Created by pspr1 on 6/27/2021.
//

#ifndef VULKAN_APPCONTEXT_H
#define VULKAN_APPCONTEXT_H

#include "KVulkan.h"



class KVulkan;

class AppContext{
private:
    //later make app non static and other instance of android_app as active app just in case for future
    static android_app *app;
    bool bFirstTimeOpen = true;
    KVulkan vulkan;

public:
    AppContext(android_app *app);
    static void setApp(android_app *app);
    static android_app* getApp();
    void onWindowInit();
};

#endif //VULKAN_APPCONTEXT_H
