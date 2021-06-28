//
// Created by pspr1 on 6/27/2021.
//

#include <android_native_app_glue.h>
#include "AppContext.h"
#include "KVulkan.h"

android_app *AppContext::app= nullptr;
AppContext::AppContext(android_app *papp)
{
    app = papp;
}
void AppContext::onWindowInit()
{
    if(bFirstTimeOpen)
    {
        vulkan.initialize(app);
        bFirstTimeOpen = false;
    }
}
void AppContext::setApp(android_app *papp)
{
    AppContext::app=papp;
}
android_app* AppContext::getApp()
{
    return AppContext::app;
}