/// This file is C++ whereas the rest of Vulkan2D's source code is C. This is because
/// Slang only provides a C++ interface for compiling shaders, so we wrap the C++ code
/// in an extern C interface the rest of the renderer can use.
#include <slang-com-ptr.h>
#include <string>
#include "VK2D/ShaderCompiler.h"
#include "VK2D/Validation.h"
#include "VK2D/Logger.h"

using namespace slang;
static Slang::ComPtr<IGlobalSession> gGlobalSession;

void _vk2dInitShaderCompiler() {
    // Create the global session
    SlangGlobalSessionDesc globalSessionDesc = {0};
    SlangResult result = createGlobalSession(&globalSessionDesc, gGlobalSession.writeRef());
    if (!SLANG_SUCCEEDED(result)) {
        vk2dRaise(VK2D_STATUS_VULKAN_ERROR, "Failed to initialize global Slang session, %s", slang_getLastInternalErrorMessage());
    }
}

void _vk2dQuitShaderCompiler() {
    // nothing yet
}

bool _vk2dShaderCompile(const char *shader, uint32_t shaderSize, VK2DCompiledShaders *compiledShaders) {
    memset(compiledShaders, 0, sizeof(VK2DCompiledShaders));

    // Create the local session
    TargetDesc target = {
            .format = SLANG_SPIRV,
            .profile = gGlobalSession->findProfile("glsl_450"),
    };
    SessionDesc sessionDesc = {
            .targets = &target,
            .targetCount = 1,
    };
    Slang::ComPtr<ISession> session;
    SlangResult result = gGlobalSession->createSession(sessionDesc, session.writeRef());
    if (!SLANG_SUCCEEDED(result)) {
        vk2dRaise(VK2D_STATUS_VULKAN_ERROR, "Failed to initialize Slang session, %s", slang_getLastInternalErrorMessage());
    }

    // Load user shader module
    Slang::ComPtr<IBlob> diagnostics;
    Slang::ComPtr<ISlangBlob> blob(slang_createBlob(shader, shaderSize));
    Slang::ComPtr<IModule> module(session->loadModuleFromSource("user_shader", "user_shader", blob, diagnostics.writeRef()));
    if (diagnostics) {
        vk2dLogInfo("%s", (const char*) diagnostics->getBufferPointer());
    }
    if (module.get() == nullptr) {
        return false;
    }

    // Find frag entry point
    Slang::ComPtr<IEntryPoint> fragEntryPoint;
    result = module->findEntryPointByName("PixelShader", fragEntryPoint.writeRef());

    if (!SLANG_SUCCEEDED(result)) {
        vk2dLogInfo("Failed to get entrypoint for pixel shader, %s", slang_getLastInternalErrorMessage());
        return false;
    }


    IComponentType* components[] = { module, fragEntryPoint };
    Slang::ComPtr<IComponentType> program;
    result = session->createCompositeComponentType(components, 2, program.writeRef());

    if (!SLANG_SUCCEEDED(result)) {
        vk2dLogInfo("Failed to compose Slang components, %s", slang_getLastInternalErrorMessage());
        return false;
    }

    // Detect if the user has a buffer in the shader
    auto programLayout = program->getLayout();
    auto globalVarLayout = programLayout->getGlobalParamsVarLayout();
    auto globalTypeLayout = globalVarLayout->getTypeLayout();
    size_t userDataSize = 0;

    int fieldCount = globalTypeLayout->getFieldCount();
    for (int i = 0; i < fieldCount; i++) {
        auto fieldVarLayout = globalTypeLayout->getFieldByIndex(i);
        if (strcmp(fieldVarLayout->getName(), "userData") == 0) {
            auto fieldTypeLayout = fieldVarLayout->getTypeLayout();
            userDataSize = fieldTypeLayout->getSize();
            break;
        }
    }

    // Link the shader
    Slang::ComPtr<IComponentType> linkedProgram;
    Slang::ComPtr<ISlangBlob> diagnosticBlob;
    result = program->link(linkedProgram.writeRef(), diagnosticBlob.writeRef());

    if (!SLANG_SUCCEEDED(result)) {
        vk2dLogInfo("Failed to link Slang components, %s", slang_getLastInternalErrorMessage());
        return false;
    }

    // Get final spir-v
    Slang::ComPtr<IBlob> fragKernelBlob;
    result = linkedProgram->getEntryPointCode(
            0, // frag
            0,
            fragKernelBlob.writeRef(),
            diagnostics.writeRef());

    if (!SLANG_SUCCEEDED(result)) {
        vk2dLogInfo("Failed to get fragment SPIR-V, %s", (const char*) diagnostics->getBufferPointer());
        return false;
    }

    // Copy over the new spir-v
    compiledShaders->fragmentSpirvSize = fragKernelBlob->getBufferSize();
    compiledShaders->fragmentSpirv = static_cast<uint32_t*>(malloc(fragKernelBlob->getBufferSize()));
    memcpy(compiledShaders->fragmentSpirv, fragKernelBlob->getBufferPointer(), compiledShaders->fragmentSpirvSize);
    compiledShaders->userDataSize = userDataSize;

    return true;
}