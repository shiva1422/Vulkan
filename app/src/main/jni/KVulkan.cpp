//
// Created by pspr1 on 4/25/2021.
//


#include "vector"
#include "KVulkan.h"
#include "Commons.h"
#include "AppContext.h"

#define VulkanLogI(...)((void)__android_log_print(ANDROID_LOG_INFO,"Vulkan::",__VA_ARGS__))
#define VulkanLogE(...)((void)__android_log_print(ANDROID_LOG_ERROR,"Vulkan::",__VA_ARGS__))

bool KVulkan::initialize(android_app *app)
{
    if(!InitVulkan())
    {
        Loge("KVulkan ::initialize"," failed - KVulkan might be unavailable");
        return false;
    }

    VulkanLogI("Available initializing ...");

    if(!createInstance())
        goto fail;
    if(!createSurface())
        goto fail;
    if(!getGpuAndCheckCapabilities())
        goto fail;
    //break below to diff meths based on req
    if(!findGFXFamilyQueueAndCreateLogicalDevice())
        goto fail;

    VulkanLogI("Initialization success");
    return true;
    fail:
    Loge("Vulkan :","initialize failed");
    return false;
    //assert(false);
    //cleanup?
}
bool KVulkan::createInstance()
{
    VkApplicationInfo  appInfo={.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                                .pNext=nullptr,
                                .pApplicationName="KalaVulkan App",
                                .applicationVersion=VK_MAKE_VERSION(1,0,0),
                                .pEngineName="KalaVulkanEngine",
                                .engineVersion=VK_MAKE_VERSION(1,0,0),
                                .apiVersion=VK_MAKE_VERSION(1,1,0),
                                };

    //check and print extensions
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr , &extensionCount , nullptr);//check res
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr , &extensionCount ,extensions.data());//check res;
    Loge("VULKAN","available Extensions ...");
    for(const auto& extension :extensions)
    {
        VulkanLogI("extension - %s",extension.extensionName);
    }

    std::vector<const char*> instanceExt;
    instanceExt.push_back("VK_KHR_surface");
    instanceExt.push_back("VK_KHR_android_surface");

 //create vulkan Instance
    VkInstanceCreateInfo instanceCreateInfo{.sType=VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                                            .pNext=nullptr,
                                            .pApplicationInfo =&appInfo,
                                            .enabledLayerCount=0,
                                            .ppEnabledLayerNames=nullptr,
                                            .enabledExtensionCount=static_cast<uint32_t>(instanceExt.size()),
                                            .ppEnabledExtensionNames = instanceExt.data(),
                                            };

    VkResult res ;
    if((res = vkCreateInstance(&instanceCreateInfo,nullptr,&vkInstance) )!= VK_SUCCESS)
    {
        VulkanLogE("instance Creation failed - err -  %d",res);
        return false;
    }


return true;

}
bool KVulkan::createSurface()
{
    //create surface
    VkAndroidSurfaceCreateInfoKHR surfaceCreatInfo{.sType=VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR,
            .pNext=nullptr,
            .flags=0,
            .window=AppContext::getApp()->window,
    };
    int res;
    if((res = vkCreateAndroidSurfaceKHR(vkInstance,&surfaceCreatInfo,nullptr,&vkSurface) )!=VK_SUCCESS)
    {
        VulkanLogE("Surface Creation failed -err - %d",res);
        return false;
    }
    VulkanLogI("surface Created");
    return true;
}
bool KVulkan::getGpuAndCheckCapabilities()
{
    /*
     * find gpu to use on android every gpu is equal graphics/compute/preset,here using first available
     */
    uint32_t gpuCount = 0;
    vkEnumeratePhysicalDevices(vkInstance,&gpuCount, nullptr);//check res also count less than 1
    VulkanLogI("Gpu count : %d",gpuCount);
    VkPhysicalDevice tmpGpus[gpuCount];
    vkEnumeratePhysicalDevices(vkInstance,&gpuCount,tmpGpus);//check res//if gpuCount < 1 fails? workaround
    //pick the gpu(1st now)
    vkGpu = tmpGpus[0];//check if null handle;

    //check vulkan info on this gpu device; check tutorial after device selection
    VkPhysicalDeviceProperties gpuProps;
    vkGetPhysicalDeviceProperties(vkGpu,&gpuProps);
    VulkanLogI("Physcial device name : %s",gpuProps.deviceName);
    VulkanLogI("Physical device info : apiVersion: %x \n\t driverVersion : %x",gpuProps.apiVersion,gpuProps.driverVersion);
    VulkanLogI("API version supported: %d.%d.%d",VK_VERSION_MAJOR(gpuProps.apiVersion),VK_VERSION_MINOR(gpuProps.apiVersion),VK_VERSION_PATCH(gpuProps.apiVersion));

    VkSurfaceCapabilitiesKHR surfaceCapabilities;//checkOther versions
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vkGpu,vkSurface,&surfaceCapabilities);
    VulkanLogI("Surface CVapabilities:.....");
    VulkanLogI("\timage count: %u - %u", surfaceCapabilities.minImageCount, surfaceCapabilities.maxImageCount);
    VulkanLogI("\tarray layers: %u", surfaceCapabilities.maxImageArrayLayers);
    VulkanLogI("\timage size (now): %dx%d", surfaceCapabilities.currentExtent.width, surfaceCapabilities.currentExtent.height);//now?
    VulkanLogI("\timage size (extent): %dx%d -%dx%d", surfaceCapabilities.minImageExtent.width, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.width, surfaceCapabilities.maxImageExtent.height);
    VulkanLogI("\tusage: %x", surfaceCapabilities.supportedUsageFlags);
    VulkanLogI("\tcurrent transform : %u", surfaceCapabilities.currentTransform);
    VulkanLogI("\tallowed tranform : %x", surfaceCapabilities.supportedTransforms);
    VulkanLogI("\tcomposite alpha flags: %u", surfaceCapabilities.supportedCompositeAlpha);//currentTransform in doc;

    //find a GFX queue family next;
    VulkanLogI("got gpu");
    return true;
}
bool KVulkan::findGFXFamilyQueueAndCreateLogicalDevice()
{
    //here we need Graphics familyqueue,other include compute,memory related check;
    uint32_t queueFamilyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(vkGpu,&queueFamilyCount, nullptr);
    assert(queueFamilyCount);//
    std::vector<VkQueueFamilyProperties> queueFamilyProps(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(vkGpu,&queueFamilyCount,queueFamilyProps.data());

    uint32_t queueFamilyIndex;
    for(queueFamilyIndex=0; queueFamilyIndex <queueFamilyCount ; queueFamilyIndex++)
    {
        if(queueFamilyProps[queueFamilyIndex].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            //selected queue family at this indx
            break;
        }
    }
    assert(queueFamilyIndex < queueFamilyCount);


    //create logical device from picked gpu;
    std::vector<const char*>  deviceExt;//fromCreateInstace
    deviceExt.push_back("VK_KHR_swapchain");
    float properties[] = {1.0f,};

    VkDeviceQueueCreateInfo  queueCreateInfo{.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                                             .pNext = nullptr,
                                             .flags = 0,
                                             .queueFamilyIndex = queueFamilyIndex,
                                             .queueCount = 1,
                                             .pQueuePriorities = properties};
    VkDeviceCreateInfo deviceCreateInfo{.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
                                        .pNext = nullptr,
                                        .queueCreateInfoCount = 1,
                                        .pQueueCreateInfos = &queueCreateInfo,
                                        .enabledLayerCount = 0,
                                        .ppEnabledLayerNames = nullptr,
                                        .enabledExtensionCount = static_cast<uint32_t>(deviceExt.size()),
                                        .ppEnabledExtensionNames = deviceExt.data(),
                                        .pEnabledFeatures = nullptr,};
    int res;

    if((res = vkCreateDevice(vkGpu,&deviceCreateInfo, nullptr,&vkDevice)) != VK_SUCCESS)
    {
        VulkanLogE("create logical device failed err - %d",res);
        return false;
    }
    bInit = true;
    return true;
}
void KVulkan::destroy()
{
    vkDestroySurfaceKHR(vkInstance,vkSurface, nullptr);
    vkDestroyDevice(vkDevice, nullptr);
    vkDestroyInstance(vkInstance, nullptr);
}
/*
 * Order of exec-
 * Init KVulkan - Load KVulkan and retrieve vulkan api function pointers.
 * create Instance
 *      fill VKApplicationInfo struct(option) with info about the app(may be for driver to optimize our specific application based on info)(OPTIONAL).
 *      tell KVulkan driver which global extension and validation layers we want to use(global means that they apply to entire program and not a specific device).(NOT OPTIONAL).
 *create vulkan instance.
 *      create a surface - to create surface , surface extension is needed
 *Find gpu to use check vulkan capabilites info on gpu,
 *find GFX queueFamily.Graphics compute etc.
 *create a logical device from the picked gpu;,can create multiple lovical devices from same physical device;
 *
 */