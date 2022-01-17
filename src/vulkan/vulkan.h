#ifndef __RDTY_RENDERERS_VULKAN__
#define __RDTY_RENDERERS_VULKAN__



// size_t
#include <cstddef>
#include <vector>
#include <map>

#if defined(__linux__)
	#define GLFW_EXPOSE_NATIVE_X11
	#define VK_USE_PLATFORM_XLIB_KHR
	#include <X11/Xlib.h>
	#include <dlfcn.h>
#elif defined(_WIN64)
	#define GLFW_EXPOSE_NATIVE_WIN32
	#define VK_USE_PLATFORM_WIN32_KHR
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#endif

#include "renderers/src/glfw/glfw-3.3.5/include/GLFW/glfw3.h"
#include "renderers/src/glfw/glfw-3.3.5/include/GLFW/glfw3native.h"
#include "renderers/src/base/renderer.h"

#define VK_NO_PROTOTYPES
#include "vulkan/vulkan.h"

#include "glslang/Include/glslang_c_interface.h"

#include "wrappers/src/renderer/renderer.h"
#include "wrappers/src/uniform/uniform.h"
#include "wrappers/src/uniform-block/uniform-block.h"
#include "wrappers/src/descriptor-set/descriptor-set.h"
#include "wrappers/src/material/material.h"
#include "wrappers/src/object/object.h"
#include "wrappers/src/scene/scene.h"



#if defined(__GNUC__)
	#define INLINE __attribute__((always_inline)) inline
#elif defined(_MSC_VER)
	#define INLINE __forceinline
#endif



// #include "vulkan/vulkan.h"



#include <iostream>
using std::cout;
using std::endl;

// REQUIRED HEADERS
///////////////////
///////////////////

// #include <cstddef>
// #include <cstdint>
// #include <cstring>
// #include <cstdio>
// #include <vector>

// #if defined(__linux__)

//   #define VK_USE_PLATFORM_XLIB_KHR
//   #include <dlfcn.h>
//   #include <X11/Xlib.h>
// #elif defined(_WIN64)

//   #define VK_USE_PLATFORM_WIN32_KHR
//   #define WIN32_LEAN_AND_MEAN
//   #include <Windows.h>
// #endif

// #define VK_NO_PROTOTYPES
// #include "vulkan.h"

///////////////////
///////////////////
///////////////////



#if defined(__linux__)
	#define SHARED_LIBRARY_MODULE_TYPE void*
	#define SHARED_LIBRARY_MODULE_INIT_VALUE nullptr
	#define SHARED_LIBRARY_LOAD dlopen("libvulkan.so.1", RTLD_LAZY)
	#define SHARED_LIBRARY_LOAD_FUNCTION dlsym
	#define SHARED_LIBRARY_FREE dlclose
#elif defined(_WIN64)
	#define SHARED_LIBRARY_MODULE_TYPE HMODULE
	#define SHARED_LIBRARY_MODULE_INIT_VALUE 0
	#define SHARED_LIBRARY_LOAD LoadLibrary("vulkan-1.dll")
	#define SHARED_LIBRARY_LOAD_FUNCTION GetProcAddress
	#define SHARED_LIBRARY_FREE FreeLibrary
#endif



// #define RS_VULKAN_MACRO_DECLARE_DEBUG_REPORT_CALLBACKS\
// 	VkDebugReportCallbackEXT report_error { VK_NULL_HANDLE };\
// 	// VkDebugReportCallbackEXT report_info { VK_NULL_HANDLE };\
// 	// VkDebugReportCallbackEXT report_warn { VK_NULL_HANDLE };\
// 	// VkDebugReportCallbackEXT report_perf { VK_NULL_HANDLE };\
// 	// VkDebugReportCallbackEXT report_debug { VK_NULL_HANDLE };

// #define RS_VULKAN_MACRO_CREATE_DEBUG_REPORT_CALLBACKS(instance)\
// 	report_error = DebugReportCallbackEXT(&reportError, VK_DEBUG_REPORT_ERROR_BIT_EXT);\
// 	// report_info = DebugReportCallbackEXT(&reportInfo, VK_DEBUG_REPORT_INFORMATION_BIT_EXT);\
// 	// report_warn = DebugReportCallbackEXT(&reportWarn, VK_DEBUG_REPORT_WARNING_BIT_EXT);\
// 	// report_perf = DebugReportCallbackEXT(&reportPerf, VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT);\
// 	// report_debug = DebugReportCallbackEXT(&reportDebug, VK_DEBUG_REPORT_DEBUG_BIT_EXT);

// #define RS_VULKAN_MACRO_DESTROY_DEBUG_REPORT_CALLBACKS(instance)\
// 	vkDestroyDebugReportCallbackEXT(instance, report_error, nullptr);\
// 	// vkDestroyDebugReportCallbackEXT(instance, report_debug, nullptr);\
// 	// vkDestroyDebugReportCallbackEXT(instance, report_perf, nullptr);\
// 	// vkDestroyDebugReportCallbackEXT(instance, report_warn, nullptr);\
// 	// vkDestroyDebugReportCallbackEXT(instance, report_info, nullptr);



// vulkan api function declarations
#define DECL_PROC(name) extern PFN_##name name

DECL_PROC(vkGetInstanceProcAddr);

DECL_PROC(vkCreateInstance);
DECL_PROC(vkEnumerateInstanceLayerProperties);
DECL_PROC(vkEnumerateInstanceExtensionProperties);

DECL_PROC(vkDestroyInstance);

DECL_PROC(vkEnumeratePhysicalDevices);
DECL_PROC(vkEnumerateDeviceExtensionProperties);
DECL_PROC(vkGetPhysicalDeviceProperties);
DECL_PROC(vkGetPhysicalDeviceFeatures);
DECL_PROC(vkGetPhysicalDeviceQueueFamilyProperties);
DECL_PROC(vkGetPhysicalDeviceMemoryProperties);
DECL_PROC(vkGetPhysicalDeviceFormatProperties);

DECL_PROC(vkGetDeviceProcAddr);
DECL_PROC(vkCreateDevice);

DECL_PROC(vkCreateDebugReportCallbackEXT);
DECL_PROC(vkDestroyDebugReportCallbackEXT);

#if defined(VK_USE_PLATFORM_XLIB_KHR)
	DECL_PROC(vkCreateXlibSurfaceKHR);
#elif defined(VK_USE_PLATFORM_WIN32_KHR)
	DECL_PROC(vkCreateWin32SurfaceKHR);
#endif

DECL_PROC(vkGetPhysicalDeviceSurfaceSupportKHR);
DECL_PROC(vkGetPhysicalDeviceSurfaceCapabilitiesKHR);
DECL_PROC(vkGetPhysicalDeviceSurfaceFormatsKHR);
DECL_PROC(vkGetPhysicalDeviceSurfacePresentModesKHR);
DECL_PROC(vkDestroySurfaceKHR);

DECL_PROC(vkDeviceWaitIdle);
DECL_PROC(vkDestroyDevice);

DECL_PROC(vkGetDeviceQueue);
DECL_PROC(vkQueueSubmit);
DECL_PROC(vkQueuePresentKHR);
DECL_PROC(vkQueueWaitIdle);

DECL_PROC(vkCreateSwapchainKHR);
DECL_PROC(vkGetSwapchainImagesKHR);
DECL_PROC(vkAcquireNextImageKHR);
DECL_PROC(vkDestroySwapchainKHR);

DECL_PROC(vkCreateImage);
DECL_PROC(vkDestroyImage);

DECL_PROC(vkCreateImageView);
DECL_PROC(vkDestroyImageView);

DECL_PROC(vkCreateSampler);
DECL_PROC(vkDestroySampler);

DECL_PROC(vkCreateRenderPass);
DECL_PROC(vkDestroyRenderPass);

DECL_PROC(vkCreateFramebuffer);
DECL_PROC(vkDestroyFramebuffer);

DECL_PROC(vkBeginCommandBuffer);
DECL_PROC(vkCmdPipelineBarrier);
DECL_PROC(vkCmdClearColorImage);
DECL_PROC(vkCmdBeginRenderPass);
DECL_PROC(vkCmdBindPipeline);
DECL_PROC(vkCmdBindVertexBuffers);
DECL_PROC(vkCmdBindIndexBuffer);
DECL_PROC(vkCmdBindDescriptorSets);
DECL_PROC(vkCmdDraw);
DECL_PROC(vkCmdEndRenderPass);
DECL_PROC(vkCmdSetViewport);
DECL_PROC(vkCmdPushConstants);
DECL_PROC(vkCmdSetScissor);
DECL_PROC(vkCmdDrawIndexed);
DECL_PROC(vkCmdCopyBufferToImage);
DECL_PROC(vkCmdUpdateBuffer);
DECL_PROC(vkEndCommandBuffer);
DECL_PROC(vkResetCommandBuffer);
DECL_PROC(vkCmdCopyImageToBuffer);

DECL_PROC(vkCreateCommandPool);
DECL_PROC(vkResetCommandPool);
DECL_PROC(vkAllocateCommandBuffers);
DECL_PROC(vkFreeCommandBuffers);
DECL_PROC(vkDestroyCommandPool);

DECL_PROC(vkCreateSemaphore);
DECL_PROC(vkDestroySemaphore);

DECL_PROC(vkCreateFence);
DECL_PROC(vkWaitForFences);
DECL_PROC(vkResetFences);
DECL_PROC(vkDestroyFence);

DECL_PROC(vkCreateShaderModule);
DECL_PROC(vkDestroyShaderModule);

DECL_PROC(vkCreatePipelineLayout);
DECL_PROC(vkDestroyPipelineLayout);

DECL_PROC(vkCreateGraphicsPipelines);
DECL_PROC(vkDestroyPipeline);

DECL_PROC(vkCreateBuffer);
DECL_PROC(vkGetBufferMemoryRequirements);
DECL_PROC(vkGetImageMemoryRequirements);
DECL_PROC(vkDestroyBuffer);

DECL_PROC(vkAllocateMemory);
DECL_PROC(vkBindBufferMemory);
DECL_PROC(vkMapMemory);
DECL_PROC(vkFlushMappedMemoryRanges);
DECL_PROC(vkUnmapMemory);
DECL_PROC(vkFreeMemory);
DECL_PROC(vkBindImageMemory);

DECL_PROC(vkCreateDescriptorPool);
DECL_PROC(vkDestroyDescriptorPool);

DECL_PROC(vkCreateDescriptorSetLayout);
DECL_PROC(vkUpdateDescriptorSets);
DECL_PROC(vkAllocateDescriptorSets);
DECL_PROC(vkDestroyDescriptorSetLayout);

#undef DECL_PROC



std::vector<uint32_t> QWE (const char*, glslang_stage_t);
// std::vector<uint32_t> (* QWE) (const char*, glslang_stage_t);



// namespace RDTY::VULKAN::WRAPPERS {};

// using RDTY::VULKAN::WRAPPERS::shared_library_module_handle;
// extern SHARED_LIBRARY_MODULE_TYPE RDTY::VULKAN::WRAPPERS::shared_library_module_handle;



namespace RDTY::VULKAN::HELPERS
{
	// SHARED_LIBRARY_MODULE_TYPE shared_library_module_handle { SHARED_LIBRARY_MODULE_INIT_VALUE };
	extern SHARED_LIBRARY_MODULE_TYPE shared_library_module_handle;
	// SHARED_LIBRARY_MODULE_TYPE shared_library_module_handle = SHARED_LIBRARY_MODULE_INIT_VALUE;



	INLINE void loadSharedLibrary (void)
	{
		// if (shared_library_module_handle) {

		//   // cout << "LIB " << shared_library_module_handle << endl;

		//   // SHARED_LIBRARY_FREE(shared_library_module_handle);

		//   cout << "DLCLOSE " << SHARED_LIBRARY_FREE(shared_library_module_handle) << endl;

		//   // shared_library_module_handle = SHARED_LIBRARY_MODULE_INIT_VALUE;
		// }

		shared_library_module_handle = SHARED_LIBRARY_LOAD;

		// cout << "LIB " << shared_library_module_handle << endl;
	}

	INLINE void freeSharedLibrary (void)
	{
		// SHARED_LIBRARY_FREE(shared_library_module_handle);

		// cout << "DLCLOSE " << SHARED_LIBRARY_FREE(shared_library_module_handle) << endl;

		shared_library_module_handle = SHARED_LIBRARY_MODULE_INIT_VALUE;
	}

	INLINE void loadGlobalFunctions (void)
	{
		vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr) SHARED_LIBRARY_LOAD_FUNCTION(shared_library_module_handle, "vkGetInstanceProcAddr");

		#define GET_PROC_ADDR(name) name = (PFN_##name) vkGetInstanceProcAddr(nullptr, #name)

		GET_PROC_ADDR(vkCreateInstance);
		GET_PROC_ADDR(vkEnumerateInstanceLayerProperties);
		GET_PROC_ADDR(vkEnumerateInstanceExtensionProperties);

		#undef GET_PROC_ADDR
	}

	INLINE void loadInstanceFunctions (VkInstance instance)
	{
		#define GET_PROC_ADDR(name) name = (PFN_##name) vkGetInstanceProcAddr(instance, #name)

		GET_PROC_ADDR(vkDestroyInstance);

		GET_PROC_ADDR(vkEnumeratePhysicalDevices);
		GET_PROC_ADDR(vkGetPhysicalDeviceProperties);
		GET_PROC_ADDR(vkGetPhysicalDeviceFeatures);
		GET_PROC_ADDR(vkGetPhysicalDeviceQueueFamilyProperties);
		GET_PROC_ADDR(vkGetPhysicalDeviceMemoryProperties);
		GET_PROC_ADDR(vkGetPhysicalDeviceFormatProperties);

		GET_PROC_ADDR(vkGetDeviceProcAddr);
		GET_PROC_ADDR(vkCreateDevice);

		GET_PROC_ADDR(vkCreateDebugReportCallbackEXT);
		GET_PROC_ADDR(vkDestroyDebugReportCallbackEXT);

		#if defined(__linux__)
			GET_PROC_ADDR(vkCreateXlibSurfaceKHR);
		#elif defined(_WIN64)
			GET_PROC_ADDR(vkCreateWin32SurfaceKHR);
		#endif

		GET_PROC_ADDR(vkGetPhysicalDeviceSurfaceSupportKHR);
		GET_PROC_ADDR(vkGetPhysicalDeviceSurfaceCapabilitiesKHR);
		GET_PROC_ADDR(vkGetPhysicalDeviceSurfaceFormatsKHR);
		GET_PROC_ADDR(vkGetPhysicalDeviceSurfacePresentModesKHR);
		GET_PROC_ADDR(vkDestroySurfaceKHR);

		#undef GET_PROC_ADDR
	}

	INLINE void loadDeviceFunctions (VkDevice device)
	{
		#define GET_PROC_ADDR(name) name = (PFN_##name) vkGetDeviceProcAddr(device, #name)

		GET_PROC_ADDR(vkDeviceWaitIdle);
		GET_PROC_ADDR(vkDestroyDevice);

		GET_PROC_ADDR(vkGetDeviceQueue);
		GET_PROC_ADDR(vkQueueSubmit);
		GET_PROC_ADDR(vkQueuePresentKHR);
		GET_PROC_ADDR(vkQueueWaitIdle);

		GET_PROC_ADDR(vkCreateSwapchainKHR);
		GET_PROC_ADDR(vkGetSwapchainImagesKHR);
		GET_PROC_ADDR(vkAcquireNextImageKHR);
		GET_PROC_ADDR(vkDestroySwapchainKHR);

		GET_PROC_ADDR(vkCreateImage);
		GET_PROC_ADDR(vkDestroyImage);

		GET_PROC_ADDR(vkCreateImageView);
		GET_PROC_ADDR(vkDestroyImageView);

		GET_PROC_ADDR(vkCreateSampler);
		GET_PROC_ADDR(vkDestroySampler);

		GET_PROC_ADDR(vkCreateRenderPass);
		GET_PROC_ADDR(vkDestroyRenderPass);

		GET_PROC_ADDR(vkCreateFramebuffer);
		GET_PROC_ADDR(vkDestroyFramebuffer);

		GET_PROC_ADDR(vkBeginCommandBuffer);
		GET_PROC_ADDR(vkCmdPipelineBarrier);
		GET_PROC_ADDR(vkCmdClearColorImage);
		GET_PROC_ADDR(vkCmdBeginRenderPass);
		GET_PROC_ADDR(vkCmdBindPipeline);
		GET_PROC_ADDR(vkCmdBindVertexBuffers);
		GET_PROC_ADDR(vkCmdBindIndexBuffer);
		GET_PROC_ADDR(vkCmdBindDescriptorSets);
		GET_PROC_ADDR(vkCmdDraw);
		GET_PROC_ADDR(vkCmdEndRenderPass);
		GET_PROC_ADDR(vkCmdSetViewport);
		GET_PROC_ADDR(vkCmdPushConstants);
		GET_PROC_ADDR(vkCmdSetScissor);
		GET_PROC_ADDR(vkCmdDrawIndexed);
		GET_PROC_ADDR(vkCmdCopyBufferToImage);
		GET_PROC_ADDR(vkEndCommandBuffer);
		GET_PROC_ADDR(vkResetCommandBuffer);
		GET_PROC_ADDR(vkCmdCopyImageToBuffer);

		GET_PROC_ADDR(vkCreateCommandPool);
		GET_PROC_ADDR(vkResetCommandPool);
		GET_PROC_ADDR(vkAllocateCommandBuffers);
		GET_PROC_ADDR(vkFreeCommandBuffers);
		GET_PROC_ADDR(vkDestroyCommandPool);

		GET_PROC_ADDR(vkCreateSemaphore);
		GET_PROC_ADDR(vkDestroySemaphore);

		GET_PROC_ADDR(vkCreateFence);
		GET_PROC_ADDR(vkWaitForFences);
		GET_PROC_ADDR(vkResetFences);
		GET_PROC_ADDR(vkDestroyFence);

		GET_PROC_ADDR(vkCreateShaderModule);
		GET_PROC_ADDR(vkDestroyShaderModule);

		GET_PROC_ADDR(vkCreatePipelineLayout);
		GET_PROC_ADDR(vkDestroyPipelineLayout);

		GET_PROC_ADDR(vkCreateGraphicsPipelines);
		GET_PROC_ADDR(vkDestroyPipeline);

		GET_PROC_ADDR(vkCreateBuffer);
		GET_PROC_ADDR(vkGetBufferMemoryRequirements);
		GET_PROC_ADDR(vkGetImageMemoryRequirements);
		GET_PROC_ADDR(vkDestroyBuffer);

		GET_PROC_ADDR(vkAllocateMemory);
		GET_PROC_ADDR(vkBindBufferMemory);
		GET_PROC_ADDR(vkMapMemory);
		GET_PROC_ADDR(vkFlushMappedMemoryRanges);
		GET_PROC_ADDR(vkUnmapMemory);
		GET_PROC_ADDR(vkFreeMemory);
		GET_PROC_ADDR(vkBindImageMemory);

		GET_PROC_ADDR(vkCreateDescriptorPool);
		GET_PROC_ADDR(vkDestroyDescriptorPool);

		GET_PROC_ADDR(vkCreateDescriptorSetLayout);
		GET_PROC_ADDR(vkUpdateDescriptorSets);
		GET_PROC_ADDR(vkAllocateDescriptorSets);
		GET_PROC_ADDR(vkDestroyDescriptorSetLayout);

		#undef GET_PROC_ADDR
	}



	INLINE VkApplicationInfo AppI
	(
		uint32_t    apiVersion         = VK_API_VERSION_1_2,
		const char* pApplicationName   = nullptr,
		uint32_t    applicationVersion = 0,
		const char* pEngineName        = nullptr,
		uint32_t    engineVersion      = 0,
		const void* pNext              = nullptr
	)
	{
		VkApplicationInfo info =
		{
			VK_STRUCTURE_TYPE_APPLICATION_INFO,
			pNext,
			pApplicationName,
			applicationVersion,
			pEngineName,
			engineVersion,
			apiVersion,
		};

		return info;
	}



	// #ifdef DEBUG
	// 	#define DEBUG_REPORT_ARGS \
	// 		\
	// 		VkDebugReportFlagsEXT      flags,\
	// 		VkDebugReportObjectTypeEXT objectType,\
	// 		uint64_t                   object,\
	// 		size_t                     location,\
	// 		int32_t                    messageCode,\
	// 		const char*                pLayerPrefix,\
	// 		const char*                pMessage,\
	// 		void*                      pUserData

	// 	VkBool32 reportError(DEBUG_REPORT_ARGS);
	// 	VkBool32 reportInfo(DEBUG_REPORT_ARGS);
	// 	VkBool32 reportWarn(DEBUG_REPORT_ARGS);
	// 	VkBool32 reportPerf(DEBUG_REPORT_ARGS);
	// 	VkBool32 reportDebug(DEBUG_REPORT_ARGS);

	// 	#undef DEBUG_REPORT_ARGS
	// #endif



	struct Instance
	{
		VkInstance        handle                = VK_NULL_HANDLE;
		uint32_t          physical_device_count = 0;
		VkPhysicalDevice* physical_devices      = nullptr;

		std::vector<VkSurfaceKHR> surfaces;

		// #ifdef DEBUG
		// 	RS_VULKAN_MACRO_DECLARE_DEBUG_REPORT_CALLBACKS
		// #endif



		VkDebugReportCallbackEXT DebugReportCallbackEXT
		(
			PFN_vkDebugReportCallbackEXT pfnCallback = nullptr,
			VkDebugReportFlagsEXT        flags       = 0,
			void*                        pUserData   = nullptr,
			const void*                  pNext       = nullptr,
			const VkAllocationCallbacks* pAllocator  = nullptr
		)
		{
			VkDebugReportCallbackCreateInfoEXT info =
			{
				VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT,
				pNext,
				flags,
				pfnCallback,
				pUserData,
			};

			VkDebugReportCallbackEXT report_callback = VK_NULL_HANDLE;

			vkCreateDebugReportCallbackEXT(handle, &info, pAllocator, &report_callback);

			return report_callback;
		}

		void retrieveAvailableDevices (void)
		{
			vkEnumeratePhysicalDevices(handle, &physical_device_count, nullptr);

			physical_devices = new VkPhysicalDevice[physical_device_count];

			vkEnumeratePhysicalDevices(handle, &physical_device_count, physical_devices);
		}

		void create
		(
			const                        VkApplicationInfo* pApplicationInfo = nullptr,
			uint32_t                     enabledLayerCount                   = 0,
			const char* const*           ppEnabledLayerNames                 = nullptr,
			uint32_t                     enabledExtensionCount               = 0,
			const char* const*           ppEnabledExtensionNames             = nullptr,
			VkInstanceCreateFlags        flags                               = 0,
			const void*                  pNext                               = nullptr,
			const VkAllocationCallbacks* pAllocator                          = nullptr,
			const bool&                  loadApiFunctions                    = true
		)
		{
			VkInstanceCreateInfo info =
			{
				VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
				pNext,
				flags,
				pApplicationInfo,
				enabledLayerCount,
				ppEnabledLayerNames,
				enabledExtensionCount,
				ppEnabledExtensionNames,
			};

			if (loadApiFunctions)
			{
				loadSharedLibrary();

				loadGlobalFunctions();
			}

			vkCreateInstance(&info, pAllocator, &handle);

			// if (loadApiFunctions)
			// {
				loadInstanceFunctions(handle);
			// }

			// #ifdef DEBUG
			// 	RS_VULKAN_MACRO_CREATE_DEBUG_REPORT_CALLBACKS(handle)
			// #endif

			retrieveAvailableDevices();
		}

		VkSurfaceKHR SurfaceKHR
		(
			#if defined(__linux__)
				Display*                     dpy        = nullptr,
				Window                       window     = 0,
				VkXlibSurfaceCreateFlagsKHR  flags      = 0,
			#elif defined(_WIN64)
				HINSTANCE                    hinstance  = 0,
				HWND                         hwnd       = 0,
				VkWin32SurfaceCreateFlagsKHR flags      = 0,
			#endif

			const void*                    pNext      = nullptr,
			const VkAllocationCallbacks*   pAllocator = nullptr
		)
		{
			#if defined(__linux__)
				VkXlibSurfaceCreateInfoKHR info =
				{
					VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
					pNext,
					flags,
					dpy,
					window,
				};
			#elif defined(_WIN64)
				VkWin32SurfaceCreateInfoKHR info =
				{
					VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
					pNext,
					flags,
					hinstance,
					hwnd,
				};
			#endif

			VkSurfaceKHR surface = VK_NULL_HANDLE;

			#if defined(__linux__)
				vkCreateXlibSurfaceKHR(handle, &info, pAllocator, &surface);
			#elif defined(_WIN64)
				vkCreateWin32SurfaceKHR(handle, &info, pAllocator, &surface);
			#endif

			surfaces.push_back(surface);

			return surface;
		}

		void destroy (void)
		{
			delete[] physical_devices;

			for (uint64_t i = 0; i < surfaces.size(); ++i)
			{
				vkDestroySurfaceKHR(handle, surfaces[i], nullptr);
			}

			surfaces.resize(0);

			// #ifdef DEBUG
			// 	RS_VULKAN_MACRO_DESTROY_DEBUG_REPORT_CALLBACKS(handle);
			// #endif

			vkDestroyInstance(handle, nullptr);

			freeSharedLibrary();
		}
	};



	INLINE VkDeviceQueueCreateInfo DevQueueCI
	(
		uint32_t                 queueFamilyIndex = 0,
		uint32_t                 queueCount       = 0,
		const float*             pQueuePriorities = nullptr,
		VkDeviceQueueCreateFlags flags            = 0,
		const void*              pNext            = nullptr
	)
	{
		VkDeviceQueueCreateInfo info =
		{
			VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			pNext,
			flags,
			queueFamilyIndex,
			queueCount,
			pQueuePriorities,
		};

		return info;
	}



	INLINE VkPipelineInputAssemblyStateCreateInfo PplInputAsm
	(
		VkPrimitiveTopology                     topology               = VK_PRIMITIVE_TOPOLOGY_POINT_LIST,
		VkBool32                                primitiveRestartEnable = VK_FALSE,
		VkPipelineInputAssemblyStateCreateFlags flags                  = 0,
		const void*                             pNext                  = nullptr
	)
	{
		VkPipelineInputAssemblyStateCreateInfo info =
		{
			VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
			pNext,
			flags,
			topology,
			primitiveRestartEnable,
		};

		return info;
	}



	INLINE VkPipelineTessellationStateCreateInfo PplTess
	(
		uint32_t                               patchControlPoints = 0,
		VkPipelineTessellationStateCreateFlags flags              = 0,
		const void*                            pNext              = nullptr
	)
	{
		VkPipelineTessellationStateCreateInfo info =
		{
			VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,
			pNext,
			flags,
			patchControlPoints,
		};

		return info;
	}



	INLINE VkPipelineViewportStateCreateInfo PplView
	(
		uint32_t                           viewportCount = 0,
		const VkViewport*                  pViewports    = nullptr,
		uint32_t                           scissorCount  = 0,
		const VkRect2D*                    pScissors     = nullptr,
		VkPipelineViewportStateCreateFlags flags         = 0,
		const void*                        pNext         = nullptr
	)
	{
		VkPipelineViewportStateCreateInfo info =
		{
			VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
			pNext,
			flags,
			viewportCount,
			pViewports,
			scissorCount,
			pScissors,
		};

		return info;
	}



	INLINE VkPipelineMultisampleStateCreateInfo PplSample
	(
		VkSampleCountFlagBits                 rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT,
		VkBool32                              sampleShadingEnable   = VK_FALSE,
		float                                 minSampleShading      = 0.0f,
		const VkSampleMask*                   pSampleMask           = nullptr,
		VkBool32                              alphaToCoverageEnable = VK_FALSE,
		VkBool32                              alphaToOneEnable      = VK_FALSE,
		VkPipelineMultisampleStateCreateFlags flags                 = 0,
		const void*                           pNext                 = nullptr
	)
	{
		VkPipelineMultisampleStateCreateInfo info =
		{
			VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
			pNext,
			flags,
			rasterizationSamples,
			sampleShadingEnable,
			minSampleShading,
			pSampleMask,
			alphaToCoverageEnable,
			alphaToOneEnable,
		};

		return info;
	}



	INLINE VkPipelineRasterizationStateCreateInfo PplRast
	(
		VkBool32                                depthClampEnable        = VK_FALSE,
		VkBool32                                rasterizerDiscardEnable = VK_FALSE,
		VkPolygonMode                           polygonMode             = VK_POLYGON_MODE_FILL,
		VkCullModeFlags                         cullMode                = VK_CULL_MODE_NONE,
		VkFrontFace                             frontFace               = VK_FRONT_FACE_COUNTER_CLOCKWISE,
		VkBool32                                depthBiasEnable         = VK_FALSE,
		float                                   depthBiasConstantFactor = 0.0f,
		float                                   depthBiasClamp          = 0.0f,
		float                                   depthBiasSlopeFactor    = 0.0f,
		float                                   lineWidth               = 1.0f,
		VkPipelineRasterizationStateCreateFlags flags                   = 0,
		const void*                             pNext                   = nullptr
	)
	{
		VkPipelineRasterizationStateCreateInfo info =
		{
			VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
			pNext,
			flags,
			depthClampEnable,
			rasterizerDiscardEnable,
			polygonMode,
			cullMode,
			frontFace,
			depthBiasEnable,
			depthBiasConstantFactor,
			depthBiasClamp,
			depthBiasSlopeFactor,
			lineWidth,
		};

		return info;
	}



	INLINE VkPipelineDepthStencilStateCreateInfo PplDepthStenc
	(
		VkBool32                               depthTestEnable       = VK_FALSE,
		VkBool32                               depthWriteEnable      = VK_FALSE,
		VkCompareOp                            depthCompareOp        = VK_COMPARE_OP_NEVER,
		VkBool32                               depthBoundsTestEnable = VK_FALSE,
		VkBool32                               stencilTestEnable     = VK_FALSE,
		VkStencilOpState                       front                 = {},
		VkStencilOpState                       back                  = {},
		float                                  minDepthBounds        = 0.0f,
		float                                  maxDepthBounds        = 0.0f,
		VkPipelineDepthStencilStateCreateFlags flags                 = 0,
		const void*                            pNext                 = nullptr
	)
	{
		VkPipelineDepthStencilStateCreateInfo info =
		{
			VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
			pNext,
			flags,
			depthTestEnable,
			depthWriteEnable,
			depthCompareOp,
			depthBoundsTestEnable,
			stencilTestEnable,
			front,
			back,
			minDepthBounds,
			maxDepthBounds,
		};

		return info;
	}



	INLINE VkPipelineColorBlendStateCreateInfo PplBlend
	(
		VkBool32                                   logicOpEnable    = VK_FALSE,
		VkLogicOp                                  logicOp          = VK_LOGIC_OP_CLEAR,
		uint32_t                                   attachmentCount  = 0,
		const VkPipelineColorBlendAttachmentState* pAttachments     = nullptr,
		float                                      blendConstants_0 = 0.0f,
		float                                      blendConstants_1 = 0.0f,
		float                                      blendConstants_2 = 0.0f,
		float                                      blendConstants_3 = 0.0f,
		VkPipelineColorBlendStateCreateFlags       flags            = 0,
		const void*                                pNext            = nullptr
	)
	{
		VkPipelineColorBlendStateCreateInfo info =
		{
			VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
			pNext,
			flags,
			logicOpEnable,
			logicOp,
			attachmentCount,
			pAttachments,
			blendConstants_0,
			blendConstants_1,
			blendConstants_2,
			blendConstants_3,
		};

		return info;
	}



	INLINE VkPipelineDynamicStateCreateInfo PplDyn
	(
		uint32_t                          dynamicStateCount = 0,
		const VkDynamicState*             pDynamicStates    = nullptr,
		VkPipelineDynamicStateCreateFlags flags             = 0,
		const void*                       pNext             = nullptr
	)
	{
		VkPipelineDynamicStateCreateInfo info =
		{
			VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
			pNext,
			flags,
			dynamicStateCount,
			pDynamicStates,
		};

		return info;
	}



	INLINE VkPipelineShaderStageCreateInfo PplShader
	(
		VkShaderStageFlagBits            stage               = VK_SHADER_STAGE_VERTEX_BIT,
		VkShaderModule                   module              = VK_NULL_HANDLE,
		const char*                      pName               = "main",
		const VkSpecializationInfo*      pSpecializationInfo = nullptr,
		VkPipelineShaderStageCreateFlags flags               = 0,
		const void*                      pNext               = nullptr
	)
	{
		VkPipelineShaderStageCreateInfo info =
		{
			VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			pNext,
			flags,
			stage,
			module,
			pName,
			pSpecializationInfo,
		};

		return info;
	}



	INLINE VkPipelineVertexInputStateCreateInfo PplVertex
	(
		uint32_t                                 vertexBindingDescriptionCount   = 0,
		const VkVertexInputBindingDescription*   pVertexBindingDescriptions      = nullptr,
		uint32_t                                 vertexAttributeDescriptionCount = 0,
		const VkVertexInputAttributeDescription* pVertexAttributeDescriptions    = nullptr,
		VkPipelineVertexInputStateCreateFlags    flags                           = 0,
		const void*                              pNext                           = nullptr
	)
	{
		VkPipelineVertexInputStateCreateInfo info =
		{
			VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
			pNext,
			flags,
			vertexBindingDescriptionCount,
			pVertexBindingDescriptions,
			vertexAttributeDescriptionCount,
			pVertexAttributeDescriptions,
		};

		return info;
	}



	INLINE VkWriteDescriptorSet WriteDescrSet
	(
		VkDescriptorSet               dstSet           = VK_NULL_HANDLE,
		uint32_t                      dstBinding       = 0,
		uint32_t                      dstArrayElement  = 0,
		uint32_t                      descriptorCount  = 0,
		VkDescriptorType              descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		const VkDescriptorImageInfo*  pImageInfo       = nullptr,
		const VkDescriptorBufferInfo* pBufferInfo      = nullptr,
		const VkBufferView*           pTexelBufferView = nullptr,
		const void*                   pNext            = nullptr
	)
	{
		VkWriteDescriptorSet info =
		{
			VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			pNext,
			dstSet,
			dstBinding,
			dstArrayElement,
			descriptorCount,
			descriptorType,
			pImageInfo,
			pBufferInfo,
			pTexelBufferView,
		};

		return info;
	}



	INLINE VkCommandBufferBeginInfo CmdBufferBeginI
	(
		const VkCommandBufferInheritanceInfo* pInheritanceInfo = nullptr,
		VkCommandBufferUsageFlags             flags            = 0,
		const void*                           pNext            = nullptr
	)
	{
		VkCommandBufferBeginInfo info =
		{
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			pNext,
			flags,
			pInheritanceInfo,
		};

		return info;
	}



	INLINE VkRenderPassBeginInfo RenderPassBeginI
	(
		VkRenderPass        renderPass      = VK_NULL_HANDLE,
		VkFramebuffer       framebuffer     = VK_NULL_HANDLE,
		VkRect2D            renderArea      = {},
		uint32_t            clearValueCount = 0,
		const VkClearValue* pClearValues    = nullptr,
		const void*         pNext           = nullptr
	)
	{
		VkRenderPassBeginInfo info =
		{
			VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
			pNext,
			renderPass,
			framebuffer,
			renderArea,
			clearValueCount,
			pClearValues,
		};

		return info;
	}



	INLINE VkSubmitInfo SubmitI
	(
		uint32_t                    waitSemaphoreCount   = 0,
		const VkSemaphore*          pWaitSemaphores      = nullptr,
		const VkPipelineStageFlags* pWaitDstStageMask    = nullptr,
		uint32_t                    commandBufferCount   = 0,
		const VkCommandBuffer*      pCommandBuffers      = nullptr,
		uint32_t                    signalSemaphoreCount = 0,
		const VkSemaphore*          pSignalSemaphores    = nullptr,
		const void*                 pNext                = nullptr
	)
	{
		VkSubmitInfo info =
		{
			VK_STRUCTURE_TYPE_SUBMIT_INFO,
			pNext,
			waitSemaphoreCount,
			pWaitSemaphores,
			pWaitDstStageMask,
			commandBufferCount,
			pCommandBuffers,
			signalSemaphoreCount,
			pSignalSemaphores,
		};

		return info;
	}



	INLINE VkPresentInfoKHR PresentI
	(
		uint32_t              waitSemaphoreCount = 0,
		const VkSemaphore*    pWaitSemaphores    = nullptr,
		uint32_t              swapchainCount     = 0,
		const VkSwapchainKHR* pSwapchains        = nullptr,
		const uint32_t*       pImageIndices      = nullptr,
		VkResult*             pResults           = nullptr,
		const void*           pNext              = nullptr
	)
	{
		VkPresentInfoKHR info =
		{
			VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
			pNext,
			waitSemaphoreCount,
			pWaitSemaphores,
			swapchainCount,
			pSwapchains,
			pImageIndices,
			pResults,
		};

		return info;
	}



	struct Device
	{
		VkDevice                         handle                      = VK_NULL_HANDLE;
		uint32_t                         queue_family_prop_count     = 0;
		VkQueueFamilyProperties*         queue_family_props          = nullptr;
		uint32_t                         surface_format_count        = 0;
		VkSurfaceFormatKHR*              surface_formats             = nullptr;
		VkPhysicalDeviceMemoryProperties mem_props                   = {};
		VkSurfaceCapabilitiesKHR         surface_capabilities        = {};
		uint32_t                         graphics_queue_family_index = -1;
		uint64_t                         graphics_queue_count        = 0;
		uint32_t                         present_queue_family_index  = -1;
		uint64_t                         present_queue_count         = 0;

		std::vector<VkRenderPass> render_passes;
		std::vector<VkSwapchainKHR> swapchains;
		std::vector<VkImageView> image_views;
		std::vector<VkImage> images;
		std::vector<VkDeviceMemory> memories;
		std::vector<VkFramebuffer> framebuffers;
		std::vector<VkFence> fences;
		std::vector<VkSemaphore> semaphores;
		std::vector<VkBuffer> buffers;
		std::vector<VkDescriptorSetLayout> descr_set_layouts;
		std::vector<VkPipelineLayout> ppl_layouts;
		std::vector<VkDescriptorPool> descr_pools;
		std::vector<VkShaderModule> shader_modules;
		std::vector<VkPipeline> pipelines;
		std::vector<VkCommandPool> cmd_pools;



		void getProps (VkPhysicalDevice physical_device, VkSurfaceKHR surface)
		{
			VkPhysicalDeviceProperties device_props {};
			vkGetPhysicalDeviceProperties(physical_device, &device_props);

			std::cout << "device_props.deviceName: " << device_props.deviceName << std::endl;

			vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_prop_count, nullptr);
			queue_family_props = new VkQueueFamilyProperties[queue_family_prop_count];
			vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_prop_count, queue_family_props);

			if (surface != VK_NULL_HANDLE)
			{
				vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &surface_format_count, nullptr);
				surface_formats = new VkSurfaceFormatKHR[surface_format_count];
				vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &surface_format_count, surface_formats);

				vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &surface_capabilities);
			}

			vkGetPhysicalDeviceMemoryProperties(physical_device, &mem_props);

			for (uint64_t i = 0; i < mem_props.memoryTypeCount; ++i) {

			  VkMemoryType type = mem_props.memoryTypes[i];

			  std::cout << "heap index: " << type.heapIndex << std::endl;

			  if (type.propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT || 0) {

			    std::cout << "  device local" << std::endl;
			  }

			  if (type.propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT || 0) {

			    std::cout << "  host visible" << std::endl;
			  }

			  if (type.propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT || 0) {

			    std::cout << "  host coherent" << std::endl;
			  }

			  if (type.propertyFlags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT || 0) {

			    std::cout << "  host cached" << std::endl;
			  }

			  if ((type.propertyFlags & VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT || 0)) {

			    std::cout << "  lazily allocated" << std::endl;
			  }

			  if (type.propertyFlags & VK_MEMORY_PROPERTY_PROTECTED_BIT || 0) {

			    std::cout << "  protected" << std::endl;
			  }

			  if (type.propertyFlags & VK_MEMORY_PROPERTY_DEVICE_COHERENT_BIT_AMD || 0) {

			    std::cout << "  device coherent AMD" << std::endl;
			  }

			  if (type.propertyFlags & VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD || 0) {

			    std::cout << "  device uncached AMD" << std::endl;
			  }

			  std::cout << std::endl << std::endl;
			}

			std::cout << std::endl << std::endl;

			for (uint64_t i = 0; i < mem_props.memoryHeapCount; ++i) {

			  VkMemoryHeap heap = mem_props.memoryHeaps[i];

			  std::cout << "heap size: " << heap.size << std::endl;

			  if (heap.flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT || 0) {

			    std::cout << "  device local" << std::endl;
			  }

			  if (heap.flags & VK_MEMORY_HEAP_MULTI_INSTANCE_BIT || 0) {

			    std::cout << "  multi instance" << std::endl;
			  }

			  if (heap.flags & VK_MEMORY_HEAP_MULTI_INSTANCE_BIT_KHR || 0) {

			    std::cout << "  multi instance KHR" << std::endl;
			  }

			  std::cout << std::endl << std::endl;
			}

			// vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &surface_capabilities);

			// std::cout << surface_capabilities.minImageCount << std::endl;
			// std::cout << surface_capabilities.maxImageCount << std::endl;
			// std::cout << surface_capabilities.currentExtent.width << " " << surface_capabilities.currentExtent.height << std::endl;
			// std::cout << surface_capabilities.minImageExtent.width << " " << surface_capabilities.minImageExtent.height << std::endl;
			// std::cout << surface_capabilities.maxImageExtent.width << " " << surface_capabilities.maxImageExtent.height << std::endl;


			// std::cout << "queue family properties count: " << queue_family_prop_count << std::endl;
			// std::cout << std::endl << std::endl;

			for (uint64_t i = 0; i < queue_family_prop_count; ++i)
			{
				VkBool32 surface_support = VK_FALSE;

				if (surface != VK_NULL_HANDLE)
				{
					vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, surface, &surface_support);
				}

				if (queue_family_props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT || 0)
				{
					graphics_queue_family_index = i;
					graphics_queue_count = queue_family_props[i].queueCount;
				}

				if (surface_support)
				{
					present_queue_family_index = i;
					present_queue_count = queue_family_props[i].queueCount;
				}

				// std::cout << "queue family index: " << i << std::endl;
				// std::cout << std::endl;
				// std::cout << "graphics: " << (queue_family_props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT || 0) << std::endl;
				// std::cout << "compute: " << (queue_family_props[i].queueFlags & VK_QUEUE_COMPUTE_BIT || 0) << std::endl;
				// std::cout << "transfer: " << (queue_family_props[i].queueFlags & VK_QUEUE_TRANSFER_BIT || 0) << std::endl;
				// std::cout << "sparse binding: " << (queue_family_props[i].queueFlags & VK_QUEUE_SPARSE_BINDING_BIT || 0) << std::endl;
				// std::cout << "protected: " << (queue_family_props[i].queueFlags & VK_QUEUE_PROTECTED_BIT || 0) << std::endl;
				// std::cout << "all: " << (queue_family_props[i].queueFlags & VK_QUEUE_FLAG_BITS_MAX_ENUM || 0) << std::endl;
				// std::cout << std::endl;
				// std::cout << "queue count: " << queue_family_props[i].queueCount << std::endl;
				// std::cout << "timestamp valid bits: " << queue_family_props[i].timestampValidBits << std::endl;
				// // std::cout << queue_family_props[i].minImageTransferGranularity << std::endl;
				// std::cout << "presentation support: " << surface_support << std::endl;
				// std::cout << std::endl << std::endl;
			}

			// throw "";
		}

		void create
		(
			VkPhysicalDevice                physical_device,
			uint32_t                        queueCreateInfoCount    = 0,
			const VkDeviceQueueCreateInfo*  pQueueCreateInfos       = nullptr,
			uint32_t                        enabledLayerCount       = 0,
			const char* const*              ppEnabledLayerNames     = nullptr,
			uint32_t                        enabledExtensionCount   = 0,
			const char* const*              ppEnabledExtensionNames = nullptr,
			const VkPhysicalDeviceFeatures* pEnabledFeatures        = nullptr,
			VkDeviceCreateFlags             flags                   = 0,
			const void*                     pNext                   = nullptr,
			const VkAllocationCallbacks*    pAllocator              = nullptr
		)
		{
			VkDeviceCreateInfo info =
			{
				VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
				pNext,
				flags,
				queueCreateInfoCount,
				pQueueCreateInfos,
				enabledLayerCount,
				ppEnabledLayerNames,
				enabledExtensionCount,
				ppEnabledExtensionNames,
				pEnabledFeatures,
			};

			vkCreateDevice(physical_device, &info, pAllocator, &handle);

			loadDeviceFunctions(handle);
		}

		VkQueue Queue (uint32_t queueFamilyIndex, uint32_t queueIndex)
		{
			VkQueue queue = VK_NULL_HANDLE;

			vkGetDeviceQueue(handle, queueFamilyIndex, queueIndex, &queue);

			return queue;
		}

		VkRenderPass RenderPass
		(
			uint32_t                       attachmentCount = 0,
			const VkAttachmentDescription* pAttachments    = nullptr,
			uint32_t                       subpassCount    = 0,
			const VkSubpassDescription*    pSubpasses      = nullptr,
			uint32_t                       dependencyCount = 0,
			const VkSubpassDependency*     pDependencies   = nullptr,
			VkRenderPassCreateFlags        flags           = 0,
			const void*                    pNext           = nullptr,
			const VkAllocationCallbacks*   pAllocator      = nullptr
		)
		{
			VkRenderPassCreateInfo info =
			{
				VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
				pNext,
				flags,
				attachmentCount,
				pAttachments,
				subpassCount,
				pSubpasses,
				dependencyCount,
				pDependencies,
			};

			VkRenderPass render_pass = VK_NULL_HANDLE;

			vkCreateRenderPass(handle, &info, pAllocator, &render_pass);

			render_passes.push_back(render_pass);

			return render_pass;
		}

		VkSwapchainKHR SwapchainKHR
		(
			VkSurfaceKHR                  surface               = VK_NULL_HANDLE,
			uint32_t                      minImageCount         = 0,
			VkFormat                      imageFormat           = VK_FORMAT_B8G8R8A8_UNORM,
			VkColorSpaceKHR               imageColorSpace       = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
			uint32_t                      imageExtent_width     = 0,
			uint32_t                      imageExtent_height    = 0,
			uint32_t                      imageArrayLayers      = 0,
			VkImageUsageFlags             imageUsage            = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
			VkSharingMode                 imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE,
			uint32_t                      queueFamilyIndexCount = 0,
			const uint32_t*               pQueueFamilyIndices   = nullptr,
			VkSurfaceTransformFlagBitsKHR preTransform          = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
			VkCompositeAlphaFlagBitsKHR   compositeAlpha        = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
			VkPresentModeKHR              presentMode           = VK_PRESENT_MODE_FIFO_KHR,
			VkBool32                      clipped               = VK_FALSE,
			VkSwapchainKHR                oldSwapchain          = VK_NULL_HANDLE,
			VkSwapchainCreateFlagsKHR     flags                 = 0,
			const void*                   pNext                 = nullptr,
			const VkAllocationCallbacks*  pAllocator            = nullptr
		)
		{
			VkSwapchainCreateInfoKHR info =
			{
				VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
				pNext,
				flags,
				surface,
				minImageCount,
				imageFormat,
				imageColorSpace,

				{
					imageExtent_width,
					imageExtent_height,
				},

				imageArrayLayers,
				imageUsage,
				imageSharingMode,
				queueFamilyIndexCount,
				pQueueFamilyIndices,
				preTransform,
				compositeAlpha,
				presentMode,
				clipped,
				oldSwapchain,
			};

			VkSwapchainKHR swapchain = VK_NULL_HANDLE;

			vkCreateSwapchainKHR(handle, &info, pAllocator, &swapchain);

			swapchains.push_back(swapchain);

			return swapchain;
		}

		std::vector<VkImage> getSwapchainImages (VkSwapchainKHR swapchain)
		{
			uint32_t count = 0;

			vkGetSwapchainImagesKHR(handle, swapchain, &count, nullptr);
			std::vector<VkImage> images(count);
			vkGetSwapchainImagesKHR(handle, swapchain, &count, images.data());

			return images;
		}

		VkImage Image
		(
			VkImageType                  imageType             = VK_IMAGE_TYPE_2D,
			VkFormat                     format                = VK_FORMAT_B8G8R8A8_UNORM,
			uint32_t                     extent_width          = 0,
			uint32_t                     extent_height         = 0,
			uint32_t                     extent_depth          = 0,
			uint32_t                     mipLevels             = 0,
			uint32_t                     arrayLayers           = 0,
			VkSampleCountFlagBits        samples               = VK_SAMPLE_COUNT_1_BIT,
			VkImageTiling                tiling                = VK_IMAGE_TILING_OPTIMAL,
			VkImageUsageFlags            usage                 = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
			VkSharingMode                sharingMode           = VK_SHARING_MODE_EXCLUSIVE,
			uint32_t                     queueFamilyIndexCount = 0,
			const uint32_t*              pQueueFamilyIndices   = nullptr,
			VkImageLayout                initialLayout         = VK_IMAGE_LAYOUT_UNDEFINED,
			VkImageCreateFlags           flags                 = 0,
			const void*                  pNext                 = nullptr,
			const VkAllocationCallbacks* pAllocator            = nullptr
		)
		{
			VkImageCreateInfo info =
			{
				VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
				pNext,
				flags,
				imageType,
				format,

				{
					extent_width,
					extent_height,
					extent_depth,
				},

				mipLevels,
				arrayLayers,
				samples,
				tiling,
				usage,
				sharingMode,
				queueFamilyIndexCount,
				pQueueFamilyIndices,
				initialLayout,
			};

			VkImage image = VK_NULL_HANDLE;

			vkCreateImage(handle, &info, pAllocator, &image);

			images.push_back(image);

			return image;
		}

		VkImageView ImageView
		(
			VkImage                      image                           = VK_NULL_HANDLE,
			VkImageViewType              viewType                        = VK_IMAGE_VIEW_TYPE_2D,
			VkFormat                     format                          = VK_FORMAT_B8G8R8A8_UNORM,
			VkComponentSwizzle           compenents_r                    = VK_COMPONENT_SWIZZLE_IDENTITY,
			VkComponentSwizzle           compenents_g                    = VK_COMPONENT_SWIZZLE_IDENTITY,
			VkComponentSwizzle           compenents_b                    = VK_COMPONENT_SWIZZLE_IDENTITY,
			VkComponentSwizzle           compenents_a                    = VK_COMPONENT_SWIZZLE_IDENTITY,
			VkImageAspectFlags           subresourceRange_aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
			uint32_t                     subresourceRange_baseMipLevel   = 0,
			uint32_t                     subresourceRange_levelCount     = 0,
			uint32_t                     subresourceRange_baseArrayLayer = 0,
			uint32_t                     subresourceRange_layerCount     = 0,
			VkImageViewCreateFlags       flags                           = 0,
			const void*                  pNext                           = nullptr,
			const VkAllocationCallbacks* pAllocator                      = nullptr
		)
		{
			VkImageViewCreateInfo info =
			{
				VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
				pNext,
				flags,
				image,
				viewType,
				format,

				{
					compenents_r,
					compenents_g,
					compenents_b,
					compenents_a,
				},

				{
					subresourceRange_aspectMask,
					subresourceRange_baseMipLevel,
					subresourceRange_levelCount,
					subresourceRange_baseArrayLayer,
					subresourceRange_layerCount,
				}
			};

			VkImageView image_view = VK_NULL_HANDLE;

			vkCreateImageView(handle, &info, pAllocator, &image_view);

			image_views.push_back(image_view);

			return image_view;
		}

		VkMemoryRequirements MemReqs (VkImage image)
		{
			VkMemoryRequirements reqs;

			vkGetImageMemoryRequirements(handle, image, &reqs);

			return reqs;
		}

		VkMemoryRequirements MemReqs (VkBuffer buffer)
		{
			VkMemoryRequirements reqs;

			vkGetBufferMemoryRequirements(handle, buffer, &reqs);

			return reqs;
		}

		uint32_t getMemTypeIndex (VkMemoryRequirements* mem_reqs, VkMemoryPropertyFlags flags)
		{
			uint32_t mem_type_count = mem_props.memoryTypeCount;

			for (uint32_t i = 0; i < mem_type_count; ++i)
			{
				uint32_t match = (mem_reqs->memoryTypeBits & (1 << i)) && (mem_props.memoryTypes[i].propertyFlags & flags);

				if (match)
				{
					return i;
				}
			}

			return -1;
		}

		VkDeviceMemory Mem
		(
			VkDeviceSize                 allocationSize  = 0,
			uint32_t                     memoryTypeIndex = 0,
			const void*                  pNext           = nullptr,
			const VkAllocationCallbacks* pAllocator      = nullptr
		)
		{
			VkMemoryAllocateInfo info =
			{
				VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
				pNext,
				allocationSize,
				memoryTypeIndex,
			};

			VkDeviceMemory mem = VK_NULL_HANDLE;

			vkAllocateMemory(handle, &info, pAllocator, &mem);

			memories.push_back(mem);

			return mem;
		}

		void bindMem (VkImage image, VkDeviceMemory mem, VkDeviceSize offset = 0)
		{
			vkBindImageMemory(handle, image, mem, offset);
		}

		void bindMem (VkBuffer buffer, VkDeviceMemory mem, VkDeviceSize offset = 0)
		{
			vkBindBufferMemory(handle, buffer, mem, offset);
		}

		void* mapMem (VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags)
		{
			void* mem_addr = nullptr;

			vkMapMemory(handle, memory, offset, size, flags, &mem_addr);

			return mem_addr;
		}

		VkFramebuffer Framebuffer
		(
			VkRenderPass                 renderPass      = VK_NULL_HANDLE,
			uint32_t                     attachmentCount = 0,
			const VkImageView*           pAttachments    = nullptr,
			uint32_t                     width           = 0,
			uint32_t                     height          = 0,
			uint32_t                     layers          = 0,
			VkFramebufferCreateFlags     flags           = 0,
			const void*                  pNext           = nullptr,
			const VkAllocationCallbacks* pAllocator      = nullptr
		)
		{
			VkFramebufferCreateInfo info =
			{
				VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
				pNext,
				flags,
				renderPass,
				attachmentCount,
				pAttachments,
				width,
				height,
				layers,
			};

			VkFramebuffer framebuffer = VK_NULL_HANDLE;

			vkCreateFramebuffer(handle, &info, pAllocator, &framebuffer);

			framebuffers.push_back(framebuffer);

			return framebuffer;
		}

		VkFence Fence
		(
			VkFenceCreateFlags           flags      = 0,
			const void*                  pNext      = nullptr,
			const VkAllocationCallbacks* pAllocator = nullptr
		)
		{
			VkFenceCreateInfo info =
			{
				VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
				pNext,
				flags,
			};

			VkFence fence = VK_NULL_HANDLE;

			vkCreateFence(handle, &info, pAllocator, &fence);

			fences.push_back(fence);

			return fence;
		}

		VkSemaphore Semaphore
		(
			VkSemaphoreCreateFlags       flags      = 0,
			const void*                  pNext      = nullptr,
			const VkAllocationCallbacks* pAllocator = nullptr
		)
		{
			VkSemaphoreCreateInfo info =
			{
				VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
				pNext,
				flags,
			};

			VkSemaphore semaphore = VK_NULL_HANDLE;

			vkCreateSemaphore(handle, &info, pAllocator, &semaphore);

			semaphores.push_back(semaphore);

			return semaphore;
		}

		VkBuffer Buffer
		(
			VkDeviceSize                 size                  = 0,
			VkBufferUsageFlags           usage                 = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VkSharingMode                sharingMode           = VK_SHARING_MODE_EXCLUSIVE,
			uint32_t                     queueFamilyIndexCount = 0,
			const uint32_t*              pQueueFamilyIndices   = 0,
			VkBufferCreateFlags          flags                 = 0,
			const void*                  pNext                 = nullptr,
			const VkAllocationCallbacks* pAllocator            = nullptr
		)
		{
			VkBufferCreateInfo info =
			{
				VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
				pNext,
				flags,
				size,
				usage,
				sharingMode,
				queueFamilyIndexCount,
				pQueueFamilyIndices,
			};

			VkBuffer buffer = VK_NULL_HANDLE;

			vkCreateBuffer(handle, &info, pAllocator, &buffer);

			buffers.push_back(buffer);

			return buffer;
		}

		VkDescriptorSetLayout DescrSetLayout
		(
			uint32_t                            bindingCount = 0,
			const VkDescriptorSetLayoutBinding* pBindings    = nullptr,
			VkDescriptorSetLayoutCreateFlags    flags        = 0,
			const void*                         pNext        = nullptr,
			const VkAllocationCallbacks*        pAllocator   = nullptr
		)
		{
			VkDescriptorSetLayoutCreateInfo info =
			{
				VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
				pNext,
				flags,
				bindingCount,
				pBindings,
			};

			VkDescriptorSetLayout layout = VK_NULL_HANDLE;

			vkCreateDescriptorSetLayout(handle, &info, pAllocator, &layout);

			descr_set_layouts.push_back(layout);

			return layout;
		}

		VkDescriptorPool DescrPool
		(
			uint32_t                     maxSets       = 0,
			uint32_t                     poolSizeCount = 0,
			const VkDescriptorPoolSize*  pPoolSizes    = nullptr,
			VkDescriptorPoolCreateFlags  flags         = 0,
			const void*                  pNext         = nullptr,
			const VkAllocationCallbacks* pAllocator    = nullptr
		)
		{
			VkDescriptorPoolCreateInfo info =
			{
				VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
				pNext,
				flags,
				maxSets,
				poolSizeCount,
				pPoolSizes,
			};

			VkDescriptorPool pool = VK_NULL_HANDLE;

			vkCreateDescriptorPool(handle, &info, pAllocator, &pool);
			// cout << vkCreateDescriptorPool(handle, &info, pAllocator, &pool) << endl;

			descr_pools.push_back(pool);

			return pool;
		}

		std::vector<VkDescriptorSet> DescrSet
		(
			VkDescriptorPool             descriptorPool     = VK_NULL_HANDLE,
			uint32_t                     descriptorSetCount = 0,
			const VkDescriptorSetLayout* pSetLayouts        = nullptr,
			const void*                  pNext              = nullptr
		)
		{
			VkDescriptorSetAllocateInfo info =
			{
				VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
				pNext,
				descriptorPool,
				descriptorSetCount,
				pSetLayouts,
			};

			std::vector<VkDescriptorSet> sets(descriptorSetCount);

			VkResult result = vkAllocateDescriptorSets(handle, &info, sets.data());

			switch (result)
			{
				case VK_ERROR_OUT_OF_HOST_MEMORY: cout << "vkAllocateDescriptorSets: VK_ERROR_OUT_OF_HOST_MEMORY" << endl; break;
				case VK_ERROR_OUT_OF_DEVICE_MEMORY: cout << "vkAllocateDescriptorSets: VK_ERROR_OUT_OF_DEVICE_MEMORY" << endl; break;
				case VK_ERROR_FRAGMENTED_POOL: cout << "vkAllocateDescriptorSets: VK_ERROR_FRAGMENTED_POOL" << endl; break;
				case VK_ERROR_OUT_OF_POOL_MEMORY: cout << "vkAllocateDescriptorSets: VK_ERROR_OUT_OF_POOL_MEMORY" << endl; break;
				default:;
			}

			return sets;
		}

		void updateDescrSets
		(
			uint32_t descriptorWriteCount,
			const VkWriteDescriptorSet* pDescriptorWrites,
			uint32_t descriptorCopyCount,
			const VkCopyDescriptorSet* pDescriptorCopies
		)
		{
			vkUpdateDescriptorSets(handle, descriptorWriteCount, pDescriptorWrites, descriptorCopyCount, pDescriptorCopies);
		}

		VkShaderModule Shader
		(
			size_t                       codeSize   = 0,
			const uint32_t*              pCode      = nullptr,
			VkShaderModuleCreateFlags    flags      = 0,
			const void*                  pNext      = nullptr,
			const VkAllocationCallbacks* pAllocator = nullptr
		)
		{
			VkShaderModuleCreateInfo info =
			{
				VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
				pNext,
				flags,
				codeSize,
				pCode,
			};

			VkShaderModule module = VK_NULL_HANDLE;

			vkCreateShaderModule(handle, &info, pAllocator, &module);

			shader_modules.push_back(module);

			return module;
		}

		VkPipelineLayout PplLayout
		(
			uint32_t                     setLayoutCount         = 0,
			const VkDescriptorSetLayout* pSetdescr_set_layouts  = nullptr,
			uint32_t                     pushConstantRangeCount = 0,
			const VkPushConstantRange*   pPushConstantRanges    = nullptr,
			VkPipelineLayoutCreateFlags  flags                  = 0,
			const void*                  pNext                  = nullptr,
			const VkAllocationCallbacks* pAllocator             = nullptr
		)
		{
			VkPipelineLayoutCreateInfo info =
			{
				VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
				pNext,
				flags,
				setLayoutCount,
				pSetdescr_set_layouts,
				pushConstantRangeCount,
				pPushConstantRanges,
			};

			VkPipelineLayout layout = VK_NULL_HANDLE;

			vkCreatePipelineLayout(handle, &info, pAllocator, &layout);

			ppl_layouts.push_back(layout);

			return layout;
		}

		VkPipeline PplG
		(
			uint32_t                                      stageCount          = 0,
			const VkPipelineShaderStageCreateInfo*        pStages             = nullptr,
			const VkPipelineVertexInputStateCreateInfo*   pVertexInputState   = nullptr,
			const VkPipelineInputAssemblyStateCreateInfo* pInputAssemblyState = nullptr,
			const VkPipelineTessellationStateCreateInfo*  pTessellationState  = nullptr,
			const VkPipelineViewportStateCreateInfo*      pViewportState      = nullptr,
			const VkPipelineRasterizationStateCreateInfo* pRasterizationState = nullptr,
			const VkPipelineMultisampleStateCreateInfo*   pMultisampleState   = nullptr,
			const VkPipelineDepthStencilStateCreateInfo*  pDepthStencilState  = nullptr,
			const VkPipelineColorBlendStateCreateInfo*    pColorBlendState    = nullptr,
			const VkPipelineDynamicStateCreateInfo*       pDynamicState       = nullptr,
			VkPipelineLayout                              layout              = VK_NULL_HANDLE,
			VkRenderPass                                  renderPass          = VK_NULL_HANDLE,
			uint32_t                                      subpass             = 0,
			VkPipeline                                    basePipelineHandle  = VK_NULL_HANDLE,
			int32_t                                       basePipelineIndex   = 0,
			VkPipelineCreateFlags                         flags               = 0,
			const void*                                   pNext               = nullptr,
			VkPipelineCache                               pipelineCache       = VK_NULL_HANDLE,
			const VkAllocationCallbacks*                  pAllocator          = nullptr
		)
		{
			VkGraphicsPipelineCreateInfo info =
			{
				VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
				pNext,
				flags,
				stageCount,
				pStages,
				pVertexInputState,
				pInputAssemblyState,
				pTessellationState,
				pViewportState,
				pRasterizationState,
				pMultisampleState,
				pDepthStencilState,
				pColorBlendState,
				pDynamicState,
				layout,
				renderPass,
				subpass,
				basePipelineHandle,
				basePipelineIndex,
			};

			VkPipeline pipeline = VK_NULL_HANDLE;

			vkCreateGraphicsPipelines(handle, pipelineCache, 1, &info, pAllocator, &pipeline);

			pipelines.push_back(pipeline);

			return pipeline;
		}

		VkCommandPool CmdPool
		(
			uint32_t                     queueFamilyIndex = 0,
			VkCommandPoolCreateFlags     flags            = 0,
			const void*                  pNext            = nullptr,
			const VkAllocationCallbacks* pAllocator       = nullptr
		)
		{
			VkCommandPoolCreateInfo info =
			{
				VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
				pNext,
				flags,
				queueFamilyIndex,
			};

			VkCommandPool pool = VK_NULL_HANDLE;

			vkCreateCommandPool(handle, &info, pAllocator, &pool);

			cmd_pools.push_back(pool);

			return pool;
		}

		std::vector<VkCommandBuffer> CmdBuffer
		(
			VkCommandPool        commandPool        = VK_NULL_HANDLE,
			VkCommandBufferLevel level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			uint32_t             commandBufferCount = 0,
			const void*          pNext              = nullptr
		)
		{
			VkCommandBufferAllocateInfo info =
			{
				VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
				pNext,
				commandPool,
				level,
				commandBufferCount,
			};

			std::vector<VkCommandBuffer> buffers(commandBufferCount);

			vkAllocateCommandBuffers(handle, &info, buffers.data());

			return buffers;
		}

		void destroy (void)
		{
			delete[] surface_formats;
			delete[] queue_family_props;

			for (uint64_t i = 0; i < render_passes.size(); ++i)
			{
				vkDestroyRenderPass(handle, render_passes[i], nullptr);
			}

			render_passes.resize(0);

			for (uint64_t i = 0; i < swapchains.size(); ++i)
			{
				vkDestroySwapchainKHR(handle, swapchains[i], nullptr);
			}

			swapchains.resize(0);

			for (uint64_t i = 0; i < image_views.size(); ++i)
			{
				vkDestroyImageView(handle, image_views[i], nullptr);
			}

			image_views.resize(0);

			for (uint64_t i = 0; i < images.size(); ++i)
			{
				vkDestroyImage(handle, images[i], nullptr);
			}

			images.resize(0);

			for (uint64_t i = 0; i < memories.size(); ++i)
			{
				vkFreeMemory(handle, memories[i], nullptr);
			}

			memories.resize(0);

			for (uint64_t i = 0; i < framebuffers.size(); ++i)
			{
				vkDestroyFramebuffer(handle, framebuffers[i], nullptr);
			}

			framebuffers.resize(0);

			for (uint64_t i = 0; i < fences.size(); ++i)
			{
				vkDestroyFence(handle, fences[i], nullptr);
			}

			fences.resize(0);

			for (uint64_t i = 0; i < semaphores.size(); ++i)
			{
				vkDestroySemaphore(handle, semaphores[i], nullptr);
			}

			semaphores.resize(0);

			for (uint64_t i = 0; i < buffers.size(); ++i)
			{
				vkDestroyBuffer(handle, buffers[i], nullptr);
			}

			buffers.resize(0);

			for (uint64_t i = 0; i < descr_set_layouts.size(); ++i)
			{
				vkDestroyDescriptorSetLayout(handle, descr_set_layouts[i], nullptr);
			}

			descr_set_layouts.resize(0);

			for (uint64_t i = 0; i < ppl_layouts.size(); ++i)
			{
				vkDestroyPipelineLayout(handle, ppl_layouts[i], nullptr);
			}

			ppl_layouts.resize(0);

			for (uint64_t i = 0; i < descr_pools.size(); ++i)
			{
				vkDestroyDescriptorPool(handle, descr_pools[i], nullptr);
			}

			descr_pools.resize(0);

			for (uint64_t i = 0; i < shader_modules.size(); ++i)
			{
				vkDestroyShaderModule(handle, shader_modules[i], nullptr);
			}

			shader_modules.resize(0);

			for (uint64_t i = 0; i < pipelines.size(); ++i)
			{
				vkDestroyPipeline(handle, pipelines[i], nullptr);
			}

			pipelines.resize(0);

			for (uint64_t i = 0; i < cmd_pools.size(); ++i)
			{
				vkDestroyCommandPool(handle, cmd_pools[i], nullptr);
			}

			cmd_pools.resize(0);

			vkDestroyDevice(handle, nullptr);
		}
	};
}



// #undef RS_VULKAN_MACRO_DESTROY_DEBUG_REPORT_CALLBACKS
// #undef RS_VULKAN_MACRO_CREATE_DEBUG_REPORT_CALLBACKS
// #undef RS_VULKAN_MACRO_DECLARE_DEBUG_REPORT_CALLBACKS

#undef SHARED_LIBRARY_FREE
#undef SHARED_LIBRARY_LOAD
#undef SHARED_LIBRARY_MODULE_INIT_VALUE
#undef SHARED_LIBRARY_MODULE_TYPE



using namespace RDTY::VULKAN::HELPERS;



namespace RDTY
{
	namespace VULKAN
	{
		struct RendererBase : public RDTY::RENDERERS::Renderer
		{
			static bool loaded;

			static std::map<std::string, size_t> test (void)
			{
				VkApplicationInfo app_i { AppI() };

				Instance inst {};

				inst.create(&app_i, 0, nullptr, 0, nullptr);

				RendererBase::loaded = true;

				std::map<std::string, size_t> result {};

				for (size_t i {}; i < inst.physical_device_count; ++i)
				{
					VkPhysicalDeviceProperties physical_device_properties {};

					vkGetPhysicalDeviceProperties(inst.physical_devices[i], &physical_device_properties);
					// cout << inst.physical_device_count << endl;
					// cout << pProperties.apiVersion << endl;
					// cout << pProperties.driverVersion << endl;
					// cout << pProperties.vendorID << endl;
					// cout << pProperties.deviceID << endl;
					// cout << pProperties.deviceType << endl;

					// result.insert({ std::string(physical_device_properties.deviceName), (size_t) inst.physical_devices[i] });
					result.insert({ std::string(physical_device_properties.deviceName), i });
				}

				delete[] inst.physical_devices;

				vkDestroyInstance(inst.handle, nullptr);

				return result;
			}



			// WRAPPERS::Renderer* wrapper {};

			GLFWwindow* window {};



			RendererBase (WRAPPERS::Renderer*);



			Instance inst {};
			// VkSurfaceKHR surf { VK_NULL_HANDLE };
			Device device {};
			VkQueue graphics_queue { VK_NULL_HANDLE };
			// VkQueue present_queue { VK_NULL_HANDLE };

			VkDescriptorPool descriptor_pool { VK_NULL_HANDLE };

			// uint64_t swapchain_image_count {};

			// std::vector<VkFence> submission_completed_fences {};
			// VkSwapchainKHR swapchain { VK_NULL_HANDLE };
			VkRenderPass render_pass { VK_NULL_HANDLE };
			std::vector<VkFramebuffer> framebuffers {};
			// std::vector<VkSemaphore> image_available_semaphores {};
			// std::vector<VkSemaphore> submission_completed_semaphores {};
			// std::vector<uint32_t> image_indices {};
			std::vector<VkSubmitInfo> submit_i {};
			// std::vector<VkPresentInfoKHR> present_i {};
			std::vector<VkRenderPassBeginInfo> render_pass_bi {};
			std::vector<VkCommandBuffer> cmd_buffers {};
			VkClearValue clear_value [2] { {}, {} };
			// VkClearValue clear_value [] { {}, {} };
			// std::vector<VkDescriptorSet> descr_set {};
			// VkPipelineLayout ppl_layout { VK_NULL_HANDLE };
			// VkPipeline ppl { VK_NULL_HANDLE };
			// // VkPipelineLayout ppl_layout2 { VK_NULL_HANDLE };
			// VkPipeline ppl2 { VK_NULL_HANDLE };
			// VkBuffer vertex_buffer { VK_NULL_HANDLE };

			// TODO: remove
			uint32_t curr_image {};



			// virtual void endLoop (void) = 0;
			// virtual void destroy (void) = 0;
		};



		struct Renderer : public RendererBase
		{
			// Renderer (WRAPPERS::Renderer*, const VkPhysicalDevice& = VK_NULL_HANDLE);
			Renderer (WRAPPERS::Renderer*, const size_t& physical_device_index = 0);


			VkSurfaceKHR surf { VK_NULL_HANDLE };
			VkQueue present_queue { VK_NULL_HANDLE };

			uint64_t swapchain_image_count {};

			std::vector<VkFence> submission_completed_fences {};
			VkSwapchainKHR swapchain { VK_NULL_HANDLE };

			std::vector<VkSemaphore> image_available_semaphores {};
			std::vector<VkSemaphore> submission_completed_semaphores {};
			std::vector<uint32_t> image_indices {};

			std::vector<VkPresentInfoKHR> present_i {};

			// uint32_t curr_image {};



			virtual void endLoop (void) override;
			virtual void destroy (void) override;
		};



		struct RendererOffscreen : public RendererBase
		{
			// RendererOffscreen (WRAPPERS::Renderer*, const VkPhysicalDevice& = VK_NULL_HANDLE);
			RendererOffscreen (WRAPPERS::Renderer*, const size_t& physical_device_index = 0);



			std::vector<VkImage> render_images {};
			VkBuffer pixel_buffer { VK_NULL_HANDLE };
			// void* pixel_data {};



			virtual void endLoop (void) override;
			virtual void destroy (void) override;
		};



		struct Uniform
		{
			// uniform_update_t functions
			// static void uniformMatrix4fv (Uniform*);



			RendererBase* renderer {};
			WRAPPERS::Uniform* wrapper {};

			// GLint location {};

			// GLint locaiton {};

			// using uniform_update_t = void (*) (Uniform*);

			// uniform_update_t update {};



			// Isn't Renderer* parameter needed?
			Uniform (RendererBase*, WRAPPERS::Uniform*);
		};



		struct UniformBlock
		{
			RendererBase* renderer {};
			WRAPPERS::UniformBlock* wrapper {};

			VkBuffer buffer {};

			size_t buffer_length {};

			void* mapped_memory_addr {};

			std::vector<Uniform*> uniforms {};

			VkDescriptorBufferInfo descr_bi { buffer, 0, VK_WHOLE_SIZE };

			// VkWriteDescriptorSet entry {};

			VkDescriptorSetLayoutBinding layout
			{
				.binding = 0,
				.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				.descriptorCount = 1,
				.stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
				.pImmutableSamplers = nullptr,
			};



			UniformBlock (RendererBase*, WRAPPERS::UniformBlock*);



			void use (void);
		};



		struct Material;

		struct DescriptorSet
		{
			RendererBase* renderer {};
			WRAPPERS::DescriptorSet* wrapper {};



			std::vector<UniformBlock*> bindings {};



			VkDescriptorSetLayout layout {};

			VkDescriptorSet handle {};



			DescriptorSet (RendererBase*, WRAPPERS::DescriptorSet*);



			void goOn (void);
			void use (Material*);
		};



		namespace MATERIAL
		{
			enum class ShaderUsage : size_t
			{
				SPIRV,
				GLSL_VULKAN,
			};
		}

		struct Material
		{
			static Material* used_instance;

			// VK_PRIMITIVE_TOPOLOGY_POINT_LIST = 0,
			// VK_PRIMITIVE_TOPOLOGY_LINE_LIST = 1,
			// VK_PRIMITIVE_TOPOLOGY_LINE_STRIP = 2,
			// VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST = 3,
			// VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP = 4,
			// VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN = 5,
			// VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY = 6,
			// VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY = 7,
			// VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY = 8,
			// VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY = 9,
			// VK_PRIMITIVE_TOPOLOGY_PATCH_LIST = 10,
			static const VkPrimitiveTopology TOPOLOGY [3];

			// VK_FRONT_FACE_COUNTER_CLOCKWISE = 0,
			// VK_FRONT_FACE_CLOCKWISE = 1,
			static const VkFrontFace FRONT_FACE [2];



			RendererBase* renderer {};
			WRAPPERS::Material* wrapper {};

			VkPrimitiveTopology topology {};
			VkFrontFace front_face {};

			VkPipelineLayout ppl_layout {};
			VkPipeline ppl {};

			// GLuint program {};

			// std::vector<Uniform*> uniforms {};
			// std::vector<UniformBlock*> uniform_blocks {};



			Material (RendererBase*, WRAPPERS::Material*, const MATERIAL::ShaderUsage = MATERIAL::ShaderUsage::GLSL_VULKAN);



			void use (void);
		};



		struct Object
		{
			RendererBase* renderer {};
			WRAPPERS::Object* wrapper {};



			Object (RendererBase*, WRAPPERS::Object*);



			void draw (void) const;
		};



		struct Scene
		{
			RendererBase* renderer {};
			WRAPPERS::Scene* wrapper {};



			Scene (RendererBase*, WRAPPERS::Scene*);
		};



		template <class T, class WrapperT>
		T* getInstance (RendererBase* renderer, WrapperT* wrapper)
		{
			T* instance {};

			if (wrapper->vulkan_impl)
			{
				instance = static_cast<T*>(wrapper->vulkan_impl);
			}
			else
			{
				instance = new T { renderer, wrapper };

				wrapper->vulkan_impl = instance;
			}

			return instance;
		}
	}
}



#define RDTY_VULKAN_GET_INSTANCE(type, renderer, wrapper) RDTY::VULKAN::getInstance<RDTY::VULKAN:: type, RDTY::WRAPPERS:: type>(renderer, wrapper)



#endif
