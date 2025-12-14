/// This file is C++ whereas the rest of Vulkan2D's source code is C. This is because
/// Slang only provides a C++ interface for compiling shaders, so we wrap the C++ code
/// in an extern C interface the rest of the renderer can use.
#include <slang-com-ptr.h>
#include <string>
#include "VK2D/ShaderCompiler.h"
#include "VK2D/Validation.h"

using namespace slang;
static SlangGlobalSessionDesc gGlobalSessionDesc;
static Slang::ComPtr<ISession> gSession;

void _vk2dInitShaderCompiler() {
    // Global session
    Slang::ComPtr<IGlobalSession> globalSession;
    SlangResult result = createGlobalSession(&gGlobalSessionDesc, globalSession.writeRef());
    if (!SLANG_SUCCEEDED(result)) {
        vk2dRaise(VK2D_STATUS_VULKAN_ERROR, "Failed to initialize global Slang session, %s", slang_getLastInternalErrorMessage());
    }

    // Session (not global)
    TargetDesc target = {
            .format = SLANG_SPIRV,
            .profile = globalSession->findProfile("glsl_450"),
    };

    SessionDesc sessionDesc = {
        .targets = &target,
        .targetCount = 1,
    };
    result = globalSession->createSession(sessionDesc, gSession.writeRef());
    if (!SLANG_SUCCEEDED(result)) {
        vk2dRaise(VK2D_STATUS_VULKAN_ERROR, "Failed to initialize Slang session, %s", slang_getLastInternalErrorMessage());
    }

    // TODO: Write script to dump Prologue.slang and Epilogue.slang to a C header so we can use them here
}

void _vk2dQuitShaderCompiler() {
    // TODO: This
}

bool _vk2dShaderCompile(const char *shader, uint8_t **outBuffer, uint32_t *outSize) {
    return false;
}