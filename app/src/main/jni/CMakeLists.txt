cmake_minimum_required(VERSION 3.10.2)
add_library(glue STATIC ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)
target_include_directories(glue PUBLIC ${ANDROID_NDK}/sources/android/native_app_glue)
include_directories(${ANDROID_NDK}/sources/android/native_app_glue)
find_library(log-lib log)
set(CMAKE_SHARED_LINKER_FLAGS  "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

#vulkan
set(VULKAN_DIR ${ANDROID_NDK}/sources/third_party/vulkan)
add_definitions(-DVULKAN_HPP_TYPESAFE_CONVERSION)



add_library(main SHARED Main.cpp  ${VULKAN_DIR}/src/common/vulkan_wrapper.cpp)
add_library(comlib STATIC AppContext.cpp KVulkan.cpp Commons.cpp Graphics.cpp AndriodEvents.cpp)
target_include_directories(main PUBLIC ${VULKAN_DIR}/src/include/vulkan ${VULKAN_DIR}/src/common)
target_include_directories(comlib PUBLIC ${VULKAN_DIR}/src/include/vulkan ${VULKAN_DIR}/src/common)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -std=c++11 \
                     -DVK_USE_PLATFORM_ANDROID_KHR")

target_link_libraries(main comlib glue android ${log-lib})