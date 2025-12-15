/// \brief Internal interface/abstraction for VK2D to compile slang shaders on the fly
#pragma once
#include "VK2D/Structs.h"
#include "VK2D/Opaque.h"

#ifdef __cplusplus
extern "C" {
#endif

/// \brief Initializes the shader compiler subsystem
void _vk2dInitShaderCompiler();

/// \brief Shuts down the shader compiler subsystem
void _vk2dQuitShaderCompiler();

/// \brief Compiles a shader and returns the compiled spir-v in outBuffer and size in outSize
/// \param shader Slang shader source code (NOT a filename)
/// \param compiledShaders End-result spir-v code for both vertex and fragment shaders
/// \return Returns false if this function fails.
bool _vk2dShaderCompile(const char *shader, VK2DCompiledShaders *compiledShaders);

#ifdef __cplusplus
};
#endif