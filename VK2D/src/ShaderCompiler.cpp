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

bool _vk2dShaderCompile(const char *shader, VK2DCompiledShaders *compiledShaders) {
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
    Slang::ComPtr<IModule> module(session->loadModuleFromSourceString("user_shader", "", shader, diagnostics.writeRef()));
    if (diagnostics) {
        vk2dLogWarn("%s", (const char*) diagnostics->getBufferPointer());
    }

    // Find frag and vertex entry points
    Slang::ComPtr<IEntryPoint> fragEntryPoint;
    result = module->findEntryPointByName("PixelShader", fragEntryPoint.writeRef());

    if (!SLANG_SUCCEEDED(result)) {
        vk2dLogWarn("Failed to get entrypoint for pixel shader, %s", slang_getLastInternalErrorMessage());
        return false;
    }

    Slang::ComPtr<IEntryPoint> vertEntryPoint;
    result = module->findEntryPointByName("VertexShader", vertEntryPoint.writeRef());

    if (!SLANG_SUCCEEDED(result)) {
        vk2dLogWarn("Failed to get entrypoint for vertex shader, %s", slang_getLastInternalErrorMessage());
        return false;
    }

    IComponentType* components[] = { module, fragEntryPoint, vertEntryPoint };
    Slang::ComPtr<IComponentType> program;
    result = session->createCompositeComponentType(components, 3, program.writeRef());

    if (!SLANG_SUCCEEDED(result)) {
        vk2dLogWarn("Failed to compose Slang components, %s", slang_getLastInternalErrorMessage());
        return false;
    }

    // TODO: Automatic user buffer reflection

    // Link the shader
    Slang::ComPtr<IComponentType> linkedProgram;
    Slang::ComPtr<ISlangBlob> diagnosticBlob;
    result = program->link(linkedProgram.writeRef(), diagnosticBlob.writeRef());

    if (!SLANG_SUCCEEDED(result)) {
        vk2dLogWarn("Failed to link Slang components, %s", slang_getLastInternalErrorMessage());
        return false;
    }

    // Get final spir-v
    Slang::ComPtr<IBlob> fragKernelBlob;
    Slang::ComPtr<IBlob> vertKernelBlob;
    result = linkedProgram->getEntryPointCode(
            0, // frag
            0,
            fragKernelBlob.writeRef(),
            diagnostics.writeRef());

    if (!SLANG_SUCCEEDED(result)) {
        vk2dLogWarn("Failed to get fragment SPIR-V, %s", (const char*) diagnostics->getBufferPointer());
        return false;
    }

    result = linkedProgram->getEntryPointCode(
            1, // vert
            0,
            vertKernelBlob.writeRef(),
            diagnostics.writeRef());

    if (!SLANG_SUCCEEDED(result)) {
        vk2dLogWarn("Failed to get vertex SPIR-V, %s", (const char*) diagnostics->getBufferPointer());
        return false;
    }

    // Copy over the new spir-v
    compiledShaders->fragmentSpirvSize = fragKernelBlob->getBufferSize();
    compiledShaders->vertexSpirvSize = vertKernelBlob->getBufferSize();
    compiledShaders->fragmentSpirv = static_cast<uint32_t*>(malloc(fragKernelBlob->getBufferSize()));
    compiledShaders->vertexSpirv = static_cast<uint32_t*>(malloc(vertKernelBlob->getBufferSize()));
    memcpy(compiledShaders->fragmentSpirv, fragKernelBlob->getBufferPointer(), compiledShaders->fragmentSpirvSize);
    memcpy(compiledShaders->vertexSpirv, vertKernelBlob->getBufferPointer(), compiledShaders->vertexSpirvSize);

    return false;
}