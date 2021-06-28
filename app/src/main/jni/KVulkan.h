//
// Created by pspr1 on 4/25/2021.
//

#ifndef VULKAN_KVULKAN_H
#define VULKAN_KVULKAN_H
#include <android_native_app_glue.h>
#include "vulkan_wrapper.h"
#define CALL_VK(func)                                                 \
  if (VK_SUCCESS != (func)) {                                         \
    __android_log_print(ANDROID_LOG_ERROR, "Tutorial ",               \
                        "Vulkan error. File[%s], line[%d]", __FILE__, \
                        __LINE__);                                    \
    assert(false);                                                    \
  }
class KVulkan{
private:
     bool createInstance();
     bool createSurface();
     bool getGpuAndCheckCapabilities();
     bool findGFXFamilyQueueAndCreateLogicalDevice();

public:
     bool bInit = false;
     VkInstance vkInstance;
     VkPhysicalDevice vkGpu;
     VkDevice vkDevice = VK_NULL_HANDLE;
     VkSurfaceKHR vkSurface;

     bool initialize(android_app *app);
     void destroy();


};


#endif //VULKAN_KVULKAN_H
