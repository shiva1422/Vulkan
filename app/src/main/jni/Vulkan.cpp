//
// Created by pspr1 on 4/25/2021.
//

#include "Vulkan.h"
#include "vulkan_wrapper.h"
#include "Commons.h"

void Vulkan::initialize(android_app *app)
{
    if(!InitVulkan())
    {
        Loge("Vulkan ::initialize"," failed - Vulkan might be unavailable");
    }
    else
    {
        Loge("Vulkan::initialize","success");
    }
}