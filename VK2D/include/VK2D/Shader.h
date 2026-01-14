/// \file Shader.h
/// \author Paolo Mazzon
/// \brief Makes shaders possible in VK2D
#pragma once
#include "VK2D/Structs.h"
#include "VK2D/Constants.h"

#ifdef __cplusplus
extern "C" {
#endif

/// \brief Creates a shader from a slang shader file.
/// \param slangFile Path to the slang source file
/// \return Returns a shader or NULL if it fails
///
/// Check https://paolomazzon.github.io/Vulkan2D/md_docs_2QuickStart.html for information
/// on how to properly create a slang shader for VK2D.
VK2DShader vk2dSlangLoad(const char *slangFile);

/// \brief Creates a shader from a slang shader file in memory.
/// \param slangFile Slang file as a string (not null-terminated)
/// \param slangFileSize Size of the slang file in bytes
/// \return Returns a shader or NULL if it fails
///
/// Check https://paolomazzon.github.io/Vulkan2D/md_docs_2QuickStart.html for information
/// on how to properly create a slang shader for VK2D.
VK2DShader vk2dSlangFrom(const char *slangFile, int slangFileSize);

/// \brief Frees a shader from memory
/// \param shader Shader to free
void vk2dShaderFree(VK2DShader shader);

#ifdef __cplusplus
}
#endif