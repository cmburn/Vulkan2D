cmake_minimum_required(VERSION 3.17)
project(Vulkan2D LANGUAGES C CXX)

option(VK2D_BUILD_EXAMPLES "Build examples for Vulkan2D" OFF)
option(VK2D_BUILD_SDL "Build SDL3 with VK2D" ON)

# VK2D requires C11 and C++17
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Dependencies
find_package(Vulkan REQUIRED)
set(Slang_ENABLE_GFX OFF)
set(Slang_ENABLE_SLANG_GLSLANG OFF)
set(Slang_ENABLE_SLANG_GLSLANG_SHARED OFF)
set(Slang_ENABLE_SLANGC OFF)
set(Slang_ENABLE_SLANGD OFF)
set(Slang_ENABLE_SLANGRT OFF)
set(Slang_ENABLE_SLANG_TESTS OFF)
set(Slang_ENABLE_EXAMPLES OFF)
set(Slang_ENABLE_TOOLS OFF)
set(Slang_ENABLE_CUDA OFF)
set(Slang_ENABLE_DXC OFF)
set(Slang_ENABLE_OPTIX OFF)
set(Slang_ENABLE_NVAPI OFF)
set(Slang_ENABLE_AFTERMATH OFF)
add_subdirectory(extern/slang)
if(VK2D_BUILD_SDL)
    add_subdirectory(extern/SDL)
    set(EXTRA_INCLUDE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/extern/SDL/include>)
endif()

# Vulkan2D
add_library(Vulkan2D
        VK2D/src/Buffer.c
        VK2D/src/Camera.c
        VK2D/src/Constants.c
        VK2D/src/DescriptorBuffer.c
        VK2D/src/DescriptorControl.c
        VK2D/src/Gui.c
        VK2D/src/Image.c
        VK2D/src/Initializers.c
        VK2D/src/Logger.c
        VK2D/src/LogicalDevice.c
        VK2D/src/Model.c
        VK2D/src/nuklear.c
        VK2D/src/PhysicalDevice.c
        VK2D/src/Pipeline.c
        VK2D/src/Polygon.c
        VK2D/src/Renderer.c
        VK2D/src/RendererMeta.c
        VK2D/src/Shader.c
        VK2D/src/ShadowEnvironment.c
        VK2D/src/Texture.c
        VK2D/src/Util.c
        VK2D/src/Validation.c
        VK2D/src/VulkanInterface.c
        VK2D/src/ShaderCompiler.cpp
        extern/VulkanMemoryAllocator/src/VmaUsage.cpp
)

target_include_directories(Vulkan2D
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/VK2D/include>
        $<INSTALL_INTERFACE:include>
        ${EXTRA_INCLUDE}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/extern/VulkanMemoryAllocator/include
)

target_compile_features(Vulkan2D PUBLIC cxx_std_17)
target_link_libraries(Vulkan2D
        PUBLIC
        Vulkan::Vulkan
        PRIVATE
        slang
        $<$<TARGET_EXISTS:SDL3::SDL3>:SDL3::SDL3>
        $<$<TARGET_EXISTS:SDL3::SDL3-shared>:SDL3::SDL3-shared>
        $<$<TARGET_EXISTS:SDL3::SDL3-static>:SDL3::SDL3-static>
)

if(VK2D_BUILD_EXAMPLES)
    add_subdirectory(examples/main)
    add_subdirectory(examples/gui)
    add_subdirectory(examples/retrolook)
    add_subdirectory(examples/shadowsglsl)
    add_subdirectory(examples/splitscreen)
    add_subdirectory(examples/testing)
endif()
