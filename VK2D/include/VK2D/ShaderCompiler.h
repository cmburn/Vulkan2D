/// \brief Internal interface/abstraction for VK2D to compile slang shaders on the fly
#pragma once
#include "VK2D/Structs.h"

#ifdef __cplusplus
extern "C" {
#endif

/// \brief Initializes the shader compiler subsystem
void _vk2dInitShaderCompiler();

/// \brief Shuts down the shader compiler subsystem
void _vk2dQuitShaderCompiler();

/// \brief Compiles a shader and returns the compiled spir-v in outBuffer and size in outSize
/// \param shader Slang shader source code
/// \param outBuffer Pointer to a pointer that will be given the output spir-v bytecode. Don't forget to free it
/// \param outSize Pointer to size that will be given the size of the spir-v in bytes
/// \return Returns false if this function fails. In the case of a failure, outBuffer will be NULL and outSize will be 0.
bool _vk2dShaderCompile(const char *shader, uint8_t **outBuffer, uint32_t *outSize);

#ifdef __cplusplus
};
#endif