/// \file Validation.h
/// \author Paolo Mazzon
/// \brief Some tools for managing errors
#pragma once
#include <vulkan/vulkan.h>
#include <stdbool.h>
#include "VK2D/Structs.h"

#ifdef __cplusplus
extern "C" {
#endif

/// \brief Raises a status problem of some sort
void vk2dRaise(VK2DStatus result, const char* fmt, ...);

/// \brief Gets the current renderer status to check for errors and the like
VK2DStatus vk2dStatus();

/// \brief Returns whether or not there is a critical status (so the renderer knows to stop)
bool vk2dStatusFatal();

/// \brief Creates validation synchronization primitives
void vk2dValidationBegin(const char *errorFile, bool quitOnError);

/// \brief Cleans up validation synchronization primitives
void vk2dValidationEnd();

/// \brief Writes a header to the error file (if there is one) at the start of the program
void vk2dValidationWriteHeader();

/// \brief Used internally to handle debugging callbacks
VKAPI_ATTR VkBool32 VKAPI_CALL _vk2dDebugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t sourceObject, size_t location, int32_t messageCode, const char* layerPrefix, const char* message, void* data);

#ifdef __cplusplus
};
#endif