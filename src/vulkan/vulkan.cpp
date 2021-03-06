#include <cstring>
#include <numeric>

#include "renderers/src/vulkan/vulkan.h"



#if defined(__linux__)
	#define SHARED_LIBRARY_MODULE_TYPE void*
#elif defined(_WIN64)
	#define SHARED_LIBRARY_MODULE_TYPE HMODULE
#endif



#include <iostream>
using std::cout;
using std::endl;



template<class RT, class PT1, class PT2>
RT getBitMask (std::vector<PT1> wrapper_set, PT2* renderer_set)
{
	RT acc {};

	for (PT1 elm : wrapper_set)
	{
		cout << "getBitMask" << (size_t) elm << endl;
		acc |= renderer_set[(size_t) elm];
	}

	return acc;
}



// vulkan api function declarations
#define DECL_PROC(name) PFN_##name name {}

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



std::vector<uint32_t> compileGlslToSpirv (const char* code_glsl, glslang_stage_t stage)
{
	const char* shaderCodeVertex = code_glsl;

	glslang_resource_t resource
	{
		.max_draw_buffers = 1,
	};

	const glslang_input_t input
	{
		.language = GLSLANG_SOURCE_GLSL,
		.stage = stage,
		.client = GLSLANG_CLIENT_VULKAN,
		.client_version = GLSLANG_TARGET_VULKAN_1_2,
		.target_language = GLSLANG_TARGET_SPV,
		.target_language_version = GLSLANG_TARGET_SPV_1_3,
		.code = shaderCodeVertex,
		.default_version = 100,
		.default_profile = GLSLANG_NO_PROFILE,
		.force_default_version_and_profile = false,
		.forward_compatible = false,
		.messages = GLSLANG_MSG_DEFAULT_BIT,
		.resource = &resource,
	};

	glslang_initialize_process();

	glslang_shader_t* shader = glslang_shader_create(&input);

	if (!glslang_shader_preprocess(shader, &input))
	{
		// use glslang_shader_get_info_log() and glslang_shader_get_info_debug_log()

		cout << glslang_shader_get_info_log(shader) << endl;
		cout << glslang_shader_get_info_debug_log(shader) << endl;
	}

	if (!glslang_shader_parse(shader, &input))
	{
		// use glslang_shader_get_info_log() and glslang_shader_get_info_debug_log()

		cout << glslang_shader_get_info_log(shader) << endl;
		cout << glslang_shader_get_info_debug_log(shader) << endl;
	}

	glslang_program_t* program = glslang_program_create();

	glslang_program_add_shader(program, shader);

	if (!glslang_program_link(program, GLSLANG_MSG_SPV_RULES_BIT | GLSLANG_MSG_VULKAN_RULES_BIT))
	{
		// use glslang_program_get_info_log() and glslang_program_get_info_debug_log();

		cout << glslang_program_get_info_log(program) << endl;
		cout << glslang_program_get_info_debug_log(program) << endl;
	}

	glslang_program_SPIRV_generate(program, input.stage);

	if (glslang_program_SPIRV_get_messages(program))
	{
		printf("%s", glslang_program_SPIRV_get_messages(program));
	}

	glslang_shader_delete(shader);

	std::vector<uint32_t> result(glslang_program_SPIRV_get_size(program));

	memcpy(result.data(), glslang_program_SPIRV_get_ptr(program), glslang_program_SPIRV_get_size(program) * sizeof(uint32_t));

	return result;
}



namespace RDTY::VULKAN::HELPERS
{
	SHARED_LIBRARY_MODULE_TYPE vulkan_loader {};
}



#undef SHARED_LIBRARY_MODULE_TYPE



namespace RDTY
{
	namespace VULKAN
	{
		bool RendererBase::loaded { false };

		RendererBase::RendererBase (WRAPPERS::Renderer* _wrapper)
		{
			wrapper = _wrapper;
		}

		void RendererBase::destroy (void)
		{
			vkDeviceWaitIdle(device.handle);

			device.destroy();

			inst.destroy();

			if (window)
			{
				glfwDestroyWindow(window);
				glfwTerminate();
			}

			for (WRAPPERS::Base* wrapper : wrappers)
			{
				wrapper->impl_vulkan = nullptr;
			}
		}



		// Renderer::Renderer (WRAPPERS::Renderer* _wrapper, const VkPhysicalDevice& physical_device) : RendererBase(_wrapper)
		Renderer::Renderer (WRAPPERS::Renderer* _wrapper, const size_t& physical_device_index) : RendererBase(_wrapper)
		{
			glfwInit();

			glfwDefaultWindowHints();
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

			window = glfwCreateWindow(wrapper->width, wrapper->height, "", nullptr, nullptr);

			glfwMakeContextCurrent(window);



			curr_image = 0;

			const char* inst_exts []
			{
				"VK_KHR_surface",

				#if defined(__linux__)
					"VK_KHR_xlib_surface",
				#elif defined(_WIN64)
					"VK_KHR_win32_surface",
				#endif
			};

			VkApplicationInfo app_i { AppI() };

			// if (RendererBase::loaded)
			// {
			// 	inst.create(&app_i, 0, nullptr, 2, inst_exts, false);
			// }
			// else
			// {
			// 	inst.create(&app_i, 0, nullptr, 2, inst_exts);

			// 	RendererBase::loaded = true;
			// }
			inst.create(&app_i, 0, nullptr, 2, inst_exts);

			// VkPhysicalDevice _physical_device { physical_device == VK_NULL_HANDLE ? inst.physical_devices[0] : physical_device };
			VkPhysicalDevice _physical_device { inst.physical_devices[physical_device_index] };
			cout << physical_device_index << endl;
			// VkPhysicalDeviceProperties pProperties = {};
			// vkGetPhysicalDeviceProperties(physical_device, &pProperties);
			// cout << inst.physical_device_count << endl;
			// cout << pProperties.apiVersion << endl;
			// cout << pProperties.driverVersion << endl;
			// cout << pProperties.vendorID << endl;
			// cout << pProperties.deviceID << endl;
			// cout << pProperties.deviceType << endl;
			// cout

			surf =
			#if defined(__linux__)
				inst.SurfaceKHR(glfwGetX11Display(), glfwGetX11Window(window));
			#elif defined(_WIN64)
				inst.SurfaceKHR(GetModuleHandle(nullptr), glfwGetWin32Window(window));
			#endif

			const char* dev_exts [] { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

			device.getProps(_physical_device, surf);

			static const float queue_priorities { 1.0f };

			std::vector<VkDeviceQueueCreateInfo> queue_ci { DevQueueCI(device.graphics_queue_family_index, 1, &queue_priorities) };

			if (device.graphics_queue_family_index != device.present_queue_family_index)
			{
				queue_ci.push_back(DevQueueCI(device.present_queue_family_index, 1, &queue_priorities));
			}

			// cout << "G " << device.graphics_queue_family_index << endl;
			// cout << "P " <<  device.present_queue_family_index << endl;

			device.create(_physical_device, device.graphics_queue_family_index != device.present_queue_family_index ? 2 : 1, queue_ci.data(), 0, nullptr, 1, dev_exts);

			graphics_queue = device.Queue(device.graphics_queue_family_index, 0);
			present_queue = device.Queue(device.present_queue_family_index, 0);



			// render pass

			// objects accessed by render pass
			VkAttachmentDescription render_pass_attach []
			{
				// color
				{
					0,
					VK_FORMAT_B8G8R8A8_UNORM,
					VK_SAMPLE_COUNT_1_BIT,
					// VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_DONT_CARE,
					VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE,
					VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE,
					VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
				},

				// depth
				{
					0,
					VK_FORMAT_D32_SFLOAT,
					VK_SAMPLE_COUNT_1_BIT,
					VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_DONT_CARE,
					VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE,
					VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
				},

				// // color_resolve
				// {
				//   0,
				//   VK_FORMAT_B8G8R8A8_UNORM,
				//   VK_SAMPLE_COUNT_1_BIT,
				//   VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_STORE,
				//   VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE,
				//   VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
				// },
			};

			// object accessed by subpass
			VkAttachmentReference color_attach_ref { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
			VkAttachmentReference depth_attach_ref { 1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };
			// VkAttachmentReference color_attach_resolve_ref = { 2, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };

			VkSubpassDescription subpass_desc
			{
				0, VK_PIPELINE_BIND_POINT_GRAPHICS,
				0, nullptr,
				// 1, &color_attach_ref, &color_attach_resolve_ref, &depth_attach_ref
				1, &color_attach_ref, nullptr, &depth_attach_ref
			};

			VkSubpassDependency subpass_dep
			{
				VK_SUBPASS_EXTERNAL, 0,
				VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
				0, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
			};

			render_pass =
				device.RenderPass
				(
					// 3, render_pass_attach,
					2, render_pass_attach,
					1, &subpass_desc,
					1, &subpass_dep
				);



			const uint32_t qfi [] { device.graphics_queue_family_index, device.present_queue_family_index };

			swapchain = device.SwapchainKHR
			(
				surf,
				8,
				VK_FORMAT_B8G8R8A8_UNORM,
				VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
				800, 600,
				1,
				VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
				VK_SHARING_MODE_EXCLUSIVE,
				0, nullptr,
				// VK_SHARING_MODE_CONCURRENT,
				// 2, qfi,
				VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
				VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
				VK_PRESENT_MODE_IMMEDIATE_KHR,
				// VK_PRESENT_MODE_FIFO_KHR,
				// VK_PRESENT_MODE_FIFO_RELAXED_KHR,
				VK_TRUE
			);

			std::vector<VkImage> swapchain_images { device.getSwapchainImages(swapchain) };

			swapchain_image_count = swapchain_images.size();

			std::vector<VkImageView> swapchain_image_views(swapchain_image_count);
			std::vector<VkImage> render_images(swapchain_image_count);
			VkMemoryRequirements render_image_mem_reqs {};
			uint64_t render_image_dev_local_mem_index {};
			std::vector<VkDeviceMemory> render_image_mems(swapchain_image_count);
			std::vector<VkImageView> render_image_views(swapchain_image_count);
			framebuffers.resize(swapchain_image_count);
			submission_completed_fences.resize(swapchain_image_count);
			submission_completed_semaphores.resize(swapchain_image_count);
			image_available_semaphores.resize(swapchain_image_count);

			std::vector<VkImage> depth_images(swapchain_image_count);
			std::vector<VkImageView> depth_image_views(swapchain_image_count);
			VkMemoryRequirements depth_image_mem_reqs {};
			uint64_t depth_image_dev_local_mem_index {};
			std::vector<VkDeviceMemory> depth_image_mems(swapchain_image_count);

			for (uint64_t i = 0; i < swapchain_image_count; ++i)
			{
				// swapchain_image_views[i] = device.ImageView
				// (
				// 	swapchain_images[i],
				// 	VK_IMAGE_VIEW_TYPE_2D,
				// 	VK_FORMAT_B8G8R8A8_UNORM,
				// 	VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY,
				// 	VK_IMAGE_ASPECT_COLOR_BIT,
				// 	0, 1,
				// 	0, 1
				// );

				swapchain_image_views[i] =
					device.ImageView
					({
						.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
						// .pNext = nullptr,
						// .flags = 0,
						.image = swapchain_images[i],
						.viewType = VK_IMAGE_VIEW_TYPE_2D,
						.format = VK_FORMAT_B8G8R8A8_UNORM,

						.components =
						{
							.r = VK_COMPONENT_SWIZZLE_IDENTITY,
							.g = VK_COMPONENT_SWIZZLE_IDENTITY,
							.b = VK_COMPONENT_SWIZZLE_IDENTITY,
							.a = VK_COMPONENT_SWIZZLE_IDENTITY,
						},

						.subresourceRange =
						{
							.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
							.baseMipLevel = 0,
							.levelCount = 1,
							.baseArrayLayer = 0,
							.layerCount = 1,
						},
					});

				render_images[i] = device.Image
				(
					VK_IMAGE_TYPE_2D,
					VK_FORMAT_B8G8R8A8_UNORM,
					800, 600, 1,
					1,
					1,
					VK_SAMPLE_COUNT_1_BIT,
					VK_IMAGE_TILING_OPTIMAL,
					VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
					VK_SHARING_MODE_EXCLUSIVE,
					0, nullptr,
					VK_IMAGE_LAYOUT_UNDEFINED
				);

				if (!i)
				{
					render_image_mem_reqs = device.MemReqs(render_images[i]);

					render_image_dev_local_mem_index = device.getMemTypeIndex(&render_image_mem_reqs, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
				}

				render_image_mems[i] = device.Mem(render_image_mem_reqs.size, render_image_dev_local_mem_index);

				device.bindMem(render_images[i], render_image_mems[i]);

				render_image_views[i] = device.ImageView
				(
					render_images[i],
					VK_IMAGE_VIEW_TYPE_2D,
					VK_FORMAT_B8G8R8A8_UNORM,
					VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY,
					VK_IMAGE_ASPECT_COLOR_BIT,
					0, 1,
					0, 1
				);



				depth_images[i] = device.Image
				(
					VK_IMAGE_TYPE_2D,
					VK_FORMAT_D32_SFLOAT,
					800, 600, 1,
					1,
					1,
					VK_SAMPLE_COUNT_1_BIT,
					VK_IMAGE_TILING_OPTIMAL,
					VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
					VK_SHARING_MODE_EXCLUSIVE,
					0, nullptr,
					VK_IMAGE_LAYOUT_UNDEFINED
				);

				if (!i)
				{
					depth_image_mem_reqs = device.MemReqs(depth_images[i]);

					depth_image_dev_local_mem_index = device.getMemTypeIndex(&depth_image_mem_reqs, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
				}

				depth_image_mems[i] = device.Mem(depth_image_mem_reqs.size, depth_image_dev_local_mem_index);

				device.bindMem(depth_images[i], depth_image_mems[i]);

				depth_image_views[i] = device.ImageView
				(
					depth_images[i],
					VK_IMAGE_VIEW_TYPE_2D,
					VK_FORMAT_D32_SFLOAT,
					VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY,
					VK_IMAGE_ASPECT_DEPTH_BIT,
					0, 1,
					0, 1
				);



				// VkImageView framebuffer_attach[] = { render_image_views[i], depth_image_views[i], swapchain_image_views[i] };
				VkImageView framebuffer_attach [] { swapchain_image_views[i], depth_image_views[i] };

				framebuffers[i] = device.Framebuffer
				(
					render_pass,
					// 3, framebuffer_attach,
					2, framebuffer_attach,
					800, 600,
					1
				);

				submission_completed_fences[i] = device.Fence(VK_FENCE_CREATE_SIGNALED_BIT);

				VkSemaphoreTypeCreateInfo semaphore_type_ci
				{
					VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO,
					nullptr,
					VK_SEMAPHORE_TYPE_BINARY,
					0
				};

				submission_completed_semaphores[i] = device.Semaphore(0, &semaphore_type_ci);
				image_available_semaphores[i] = device.Semaphore(0, &semaphore_type_ci);
			}



			// command buffers

			VkCommandPool cmd_pool { device.CmdPool(device.graphics_queue_family_index, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT) };

			cmd_buffers = device.CmdBuffer(cmd_pool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, swapchain_image_count);



			// VkCommandBufferBeginInfo command_buffer_bi = CmdBufferBeginI(nullptr, VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);

			clear_value[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
			clear_value[1].depthStencil = { 1.0f, 0 };

			submit_i.resize(swapchain_image_count);
			present_i.resize(swapchain_image_count);
			image_indices.resize(swapchain_image_count);
			render_pass_bi.resize(swapchain_image_count);

			static const VkPipelineStageFlags wait_stages { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

			for (uint64_t i = 0; i < swapchain_image_count; ++i)
			{
				submit_i[i] =
					SubmitI
					(
						1, &image_available_semaphores[i], &wait_stages,
						1, &cmd_buffers[i],
						1, &submission_completed_semaphores[i]
					);

				present_i[i] =
					PresentI
					(
						1, &submission_completed_semaphores[i],
						1, &swapchain,
						nullptr
					);

				render_pass_bi[i] = RenderPassBeginI(render_pass, framebuffers[i], { 0, 0, 800, 600 }, 2, clear_value);
			}
		}

		void Renderer::beginLoop (void)
		{
			vkAcquireNextImageKHR(device.handle, swapchain, 0xFFFFFFFF, image_available_semaphores[curr_image], VK_NULL_HANDLE, &image_indices[curr_image]);

			// VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
			static const VkCommandBufferBeginInfo command_buffer_bi { CmdBufferBeginI(nullptr, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT) };

			vkWaitForFences(device.handle, 1, &submission_completed_fences[curr_image], VK_TRUE, 0xFFFFFFFF);

			vkBeginCommandBuffer(cmd_buffers[curr_image], &command_buffer_bi);
		}

		// void Renderer::beginRenderPass (const VkFramebuffer& framebuffer = VK_NULL_HANDLE)
		// {
		// 	const VkRenderPassBeginInfo render_pass_begin_info
		// 	{
		// 		RenderPassBeginI(render_pass, framebuffers[i], { 0, 0, 800, 600 }, 2, clear_value)
		// 	};

		// 	vkCmdBeginRenderPass(cmd_buffers[_renderer->curr_image], &_renderer->render_pass_bi[_renderer->curr_image], VK_SUBPASS_CONTENTS_INLINE);
		// }

		void Renderer::endLoop (void)
		{
			vkCmdEndRenderPass(cmd_buffers[curr_image]);
			vkEndCommandBuffer(cmd_buffers[curr_image]);

			vkResetFences(device.handle, 1, &submission_completed_fences[curr_image]);

			vkQueueSubmit(graphics_queue, 1, &submit_i[curr_image], submission_completed_fences[curr_image]);

			present_i[curr_image].pImageIndices = &image_indices[curr_image];

			vkQueuePresentKHR(present_queue, &present_i[curr_image]);



			const static uint64_t max_swapchain_image_index { swapchain_image_count - 1 };

			if (++curr_image > max_swapchain_image_index)
			{
				curr_image = 0;
			}
		}

		// void Renderer::beginRenderPass (const VkRenderPassBeginInfo&& render_pass_begin_info)
		// {
		// 	vkCmdBeginRenderPass(cmd_buffers[curr_image], &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);
		// }



		// RendererOffscreen::RendererOffscreen (WRAPPERS::Renderer* _wrapper, const VkPhysicalDevice& physical_device) : RendererBase(_wrapper)
		RendererOffscreen::RendererOffscreen (WRAPPERS::Renderer* _wrapper, const size_t& physical_device_index) : RendererBase(_wrapper)
		{
			VkApplicationInfo app_i { AppI() };

			// if (RendererBase::loaded)
			// {
			// 	inst.create(&app_i, 0, nullptr, 0, nullptr, false);
			// }
			// else
			// {
			// 	inst.create(&app_i, 0, nullptr, 0, nullptr);

			// 	RendererBase::loaded = true;
			// }
			inst.create(&app_i, 0, nullptr, 0, nullptr);

			// VkPhysicalDevice _physical_device { physical_device == VK_NULL_HANDLE ? inst.physical_devices[0] : physical_device };
			VkPhysicalDevice _physical_device { inst.physical_devices[physical_device_index] };

			device.getProps(_physical_device, VK_NULL_HANDLE);

			static const float queue_priorities { 1.0f };

			std::vector<VkDeviceQueueCreateInfo> queue_ci { DevQueueCI(device.graphics_queue_family_index, 1, &queue_priorities) };

			device.create(_physical_device, 1, queue_ci.data(), 0, nullptr, 0, nullptr);

			graphics_queue = device.Queue(device.graphics_queue_family_index, 0);



			VkAttachmentDescription render_pass_attach []
			{
				// color
				{
					0,
					VK_FORMAT_B8G8R8A8_UNORM,
					VK_SAMPLE_COUNT_1_BIT,
					VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE,
					VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE,
					VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL
				},

				// depth
				{
					0,
					VK_FORMAT_D32_SFLOAT,
					VK_SAMPLE_COUNT_1_BIT,
					VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_DONT_CARE,
					VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE,
					VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
				},

				// // color_resolve
				// {
				//   0,
				//   VK_FORMAT_B8G8R8A8_UNORM,
				//   VK_SAMPLE_COUNT_1_BIT,
				//   VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_STORE,
				//   VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE,
				//   VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
				// },
			};

			// object accessed by subpass
			VkAttachmentReference color_attach_ref { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
			VkAttachmentReference depth_attach_ref { 1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };
			// VkAttachmentReference color_attach_resolve_ref = { 2, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };

			VkSubpassDescription subpass_desc
			{
				0, VK_PIPELINE_BIND_POINT_GRAPHICS,
				0, nullptr,
				// 1, &color_attach_ref, &color_attach_resolve_ref, &depth_attach_ref
				1, &color_attach_ref, nullptr, &depth_attach_ref
			};

			VkSubpassDependency subpass_dep
			{
				VK_SUBPASS_EXTERNAL, 0,
				VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
				0, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
			};

			render_pass =
				device.RenderPass
				(
					// 3, render_pass_attach,
					2, render_pass_attach,
					1, &subpass_desc,
					1, &subpass_dep
				);



			uint64_t image_count { 1 };

			std::vector<VkImageView> swapchain_image_views(image_count);
			render_images.resize(image_count);
			VkMemoryRequirements render_image_mem_reqs {};
			uint64_t render_image_dev_local_mem_index {};
			std::vector<VkDeviceMemory> render_image_mems(image_count);
			std::vector<VkImageView> render_image_views(image_count);
			framebuffers.resize(image_count);

			std::vector<VkImage> depth_images(image_count);
			std::vector<VkImageView> depth_image_views(image_count);
			VkMemoryRequirements depth_image_mem_reqs {};
			uint64_t depth_image_dev_local_mem_index {};
			std::vector<VkDeviceMemory> depth_image_mems(image_count);

			for (uint64_t i = 0; i < image_count; ++i)
			{
				render_images[i] = device.Image
				(
					VK_IMAGE_TYPE_2D,
					VK_FORMAT_B8G8R8A8_UNORM,
					800, 600, 1,
					1,
					1,
					VK_SAMPLE_COUNT_1_BIT,
					VK_IMAGE_TILING_OPTIMAL,
					VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
					VK_SHARING_MODE_EXCLUSIVE,
					0, nullptr,
					VK_IMAGE_LAYOUT_UNDEFINED
				);

				render_image_mem_reqs = device.MemReqs(render_images[i]);

				render_image_dev_local_mem_index = device.getMemTypeIndex(&render_image_mem_reqs, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

				render_image_mems[i] = device.Mem(render_image_mem_reqs.size, render_image_dev_local_mem_index);

				device.bindMem(render_images[i], render_image_mems[i]);

				{
					pixel_buffer = device.Buffer(render_image_mem_reqs.size, VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_SHARING_MODE_EXCLUSIVE);

					VkMemoryRequirements vertex_buffer_mem_reqs { device.MemReqs(pixel_buffer) };

					uint64_t vertex_buffer_mem_index { device.getMemTypeIndex(&vertex_buffer_mem_reqs, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) };

					VkDeviceMemory vertex_buffer_mem { device.Mem(vertex_buffer_mem_reqs.size, vertex_buffer_mem_index) };

					device.bindMem(pixel_buffer, vertex_buffer_mem);

					pixel_data = device.mapMem(vertex_buffer_mem, 0, render_image_mem_reqs.size, 0);
				}

				render_image_views[i] = device.ImageView
				(
					render_images[i],
					VK_IMAGE_VIEW_TYPE_2D,
					VK_FORMAT_B8G8R8A8_UNORM,
					VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY,
					VK_IMAGE_ASPECT_COLOR_BIT,
					0, 1,
					0, 1
				);



				depth_images[i] = device.Image
				(
					VK_IMAGE_TYPE_2D,
					VK_FORMAT_D32_SFLOAT,
					800, 600, 1,
					1,
					1,
					VK_SAMPLE_COUNT_1_BIT,
					VK_IMAGE_TILING_OPTIMAL,
					VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
					VK_SHARING_MODE_EXCLUSIVE,
					0, nullptr,
					VK_IMAGE_LAYOUT_UNDEFINED
				);

				if (!i)
				{
					depth_image_mem_reqs = device.MemReqs(depth_images[i]);

					depth_image_dev_local_mem_index = device.getMemTypeIndex(&depth_image_mem_reqs, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
				}

				depth_image_mems[i] = device.Mem(depth_image_mem_reqs.size, depth_image_dev_local_mem_index);

				device.bindMem(depth_images[i], depth_image_mems[i]);

				depth_image_views[i] = device.ImageView
				(
					depth_images[i],
					VK_IMAGE_VIEW_TYPE_2D,
					VK_FORMAT_D32_SFLOAT,
					VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY,
					VK_IMAGE_ASPECT_DEPTH_BIT,
					0, 1,
					0, 1
				);



				VkImageView framebuffer_attach [] { render_image_views[i], depth_image_views[i] };

				framebuffers[i] = device.Framebuffer
				(
					render_pass,
					2, framebuffer_attach,
					800, 600,
					1
				);
			}



			VkCommandPool cmd_pool { device.CmdPool(device.graphics_queue_family_index, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT) };

			cmd_buffers = device.CmdBuffer(cmd_pool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, image_count);



			clear_value[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
			clear_value[1].depthStencil = { 1.0f, 0 };

			submit_i.resize(image_count);
			render_pass_bi.resize(image_count);

			static const VkPipelineStageFlags wait_stages { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

			for (uint64_t i = 0; i < image_count; ++i)
			{
				submit_i[i] =
					SubmitI
					(
						0, nullptr, nullptr,
						1, &cmd_buffers[i],
						0, nullptr
					);

				render_pass_bi[i] = RenderPassBeginI(render_pass, framebuffers[i], { 0, 0, 800, 600 }, 2, clear_value);
			}
		}

		void RendererOffscreen::beginLoop (void)
		{
			static const VkCommandBufferBeginInfo command_buffer_bi { CmdBufferBeginI(nullptr, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT) };

			vkBeginCommandBuffer(cmd_buffers[0], &command_buffer_bi);
		}

		void RendererOffscreen::endLoop (void)
		{
			vkCmdEndRenderPass(cmd_buffers[0]);

			VkImageSubresourceLayers image_subresource_layers
			{
				VK_IMAGE_ASPECT_COLOR_BIT,
				0, 0, 1
			};

			VkBufferImageCopy buffer_image_copy
			{
				0, 800, 600,
				image_subresource_layers,
				{ 0, 0, 0 },
				{ 800, 600, 1 },
			};

			vkCmdCopyImageToBuffer
			(
				cmd_buffers[0],
				render_images[0],
				VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				pixel_buffer,
				1, &buffer_image_copy
			);

			vkEndCommandBuffer(cmd_buffers[0]);

			vkQueueSubmit(graphics_queue, 1, &submit_i[0], nullptr);

			vkQueueWaitIdle(graphics_queue);
		}



		Uniform::Uniform (RendererBase* _renderer, WRAPPERS::Uniform* _wrapper)
		{
			renderer = _renderer;
			wrapper = _wrapper;
		}



		const VkShaderStageFlagBits UniformBlock::VISIBILITY [3]
		{
			VK_SHADER_STAGE_VERTEX_BIT,
			VK_SHADER_STAGE_FRAGMENT_BIT,
			VK_SHADER_STAGE_COMPUTE_BIT,
		};

		UniformBlock::UniformBlock (RendererBase* _renderer, WRAPPERS::UniformBlock* _wrapper)
		{
			renderer = _renderer;
			wrapper = _wrapper;



			// entry =
			// 	WriteDescrSet
			// 	(
			// 		VK_NULL_HANDLE, wrapper->binding, 0,
			// 		1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			// 		nullptr,
			// 		&descr_bi,
			// 		nullptr
			// 	);

			layout.binding = wrapper->binding;

			layout.stageFlags = getBitMask<VkShaderStageFlags>(wrapper->visibility, UniformBlock::VISIBILITY);



			for (WRAPPERS::Uniform* uniform_wrapper : wrapper->uniforms)
			{
				Uniform* uniform { getInstance<Uniform, WRAPPERS::Uniform>(renderer, uniform_wrapper) };

				uniforms.push_back(uniform);

				buffer_length += uniform->wrapper->size;
			}



			buffer = renderer->device.Buffer(buffer_length, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE);

			VkMemoryRequirements buffer_mem_reqs { renderer->device.MemReqs(buffer) };

			uint64_t buffer_mem_index { renderer->device.getMemTypeIndex(&buffer_mem_reqs, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) };

			VkDeviceMemory buffer_mem { renderer->device.Mem(buffer_mem_reqs.size, buffer_mem_index) };

			renderer->device.bindMem(buffer, buffer_mem);

			mapped_memory_addr = renderer->device.mapMem(buffer_mem, 0, buffer_length, 0);



			descr_bi.buffer = buffer;



			// for (WRAPPERS::Uniform* uniform_wrapper : wrapper->uniforms)
			// {
			// 	Uniform* uniform { new Uniform { renderer, uniform_wrapper } };

			// 	uniforms.push_back(uniform);
			// }



			use();
		}

		void UniformBlock::use (void)
		{
			for (Uniform* uniform : uniforms)
			{
				/**
				 * This is suitable only for small amounts fo data for unmapped buffers
				 * and not for initial buffer updates.
				 */
				// vkCmdUpdateBuffer(renderer->cmd_buffers[renderer->curr_image], buffer, 0, uniform->wrapper->size, uniform->wrapper->object_addr);

				memcpy(mapped_memory_addr + uniform->wrapper->block_index, uniform->wrapper->object_addr, uniform->wrapper->size);
			}
		}



		StorageBlock::StorageBlock (RendererBase* _renderer, WRAPPERS::StorageBlock* _wrapper)
		{
			renderer = _renderer;
			wrapper = _wrapper;



			layout.binding = wrapper->binding;

			layout.stageFlags = getBitMask<VkShaderStageFlags>(wrapper->visibility, UniformBlock::VISIBILITY);



			buffer = renderer->device.Buffer(wrapper->size, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE);

			VkMemoryRequirements buffer_mem_reqs { renderer->device.MemReqs(buffer) };

			uint64_t buffer_mem_index { renderer->device.getMemTypeIndex(&buffer_mem_reqs, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) };

			VkDeviceMemory buffer_mem { renderer->device.Mem(buffer_mem_reqs.size, buffer_mem_index) };

			renderer->device.bindMem(buffer, buffer_mem);

			mapped_memory_addr = renderer->device.mapMem(buffer_mem, 0, wrapper->size, 0);

			memcpy(mapped_memory_addr, wrapper->data, wrapper->size);



			descr_bi.buffer = buffer;
		}



		DescriptorSet::DescriptorSet (RendererBase* _renderer, WRAPPERS::DescriptorSet* _wrapper)
		{
			renderer = _renderer;
			wrapper = _wrapper;



			std::vector<VkDescriptorSetLayoutBinding> descr_set_layout_bindings {};
			std::vector<VkWriteDescriptorSet> write_descr_sets {};

			// for (WRAPPERS::UniformBlock* binding_wrapper : wrapper->bindings)
			// {
			// 	UniformBlock* binding { getInstance<UniformBlock, WRAPPERS::UniformBlock>(renderer, binding_wrapper) };

			// 	bindings.push_back(binding);

			// 	descr_set_layout_bindings.push_back(binding->layout);
			// }

			for (void* binding_wrapper : wrapper->bindings)
			{
				WRAPPERS::DESCRIPTOR_BINDING::Type* descriptor_binding_type =
					reinterpret_cast<WRAPPERS::DESCRIPTOR_BINDING::Type*>(binding_wrapper);

				if (*descriptor_binding_type == WRAPPERS::DESCRIPTOR_BINDING::Type::UNIFORM_BLOCK)
				{
					UniformBlock* binding { getInstance<UniformBlock, WRAPPERS::UniformBlock>(renderer, reinterpret_cast<WRAPPERS::UniformBlock*>(binding_wrapper)) };

					// bindings.push_back(binding);

					descr_set_layout_bindings.push_back(binding->layout);



					VkWriteDescriptorSet write_descr_set =
						WriteDescrSet
						(
							handle, binding->wrapper->binding, 0,
							1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
							nullptr,
							&binding->descr_bi,
							nullptr
						);

					write_descr_sets.push_back(write_descr_set);
				}
				else if (*descriptor_binding_type == WRAPPERS::DESCRIPTOR_BINDING::Type::STORAGE_BLOCK)
				{
					StorageBlock* binding { getInstance<StorageBlock, WRAPPERS::StorageBlock>(renderer, reinterpret_cast<WRAPPERS::StorageBlock*>(binding_wrapper)) };

					// bindings.push_back(binding);

					descr_set_layout_bindings.push_back(binding->layout);



					VkWriteDescriptorSet write_descr_set =
						WriteDescrSet
						(
							handle, binding->wrapper->binding, 0,
							1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
							nullptr,
							&binding->descr_bi,
							nullptr
						);

					write_descr_sets.push_back(write_descr_set);
				}

				// UniformBlock* binding { getInstance<UniformBlock, WRAPPERS::UniformBlock>(renderer, binding_wrapper) };

				// bindings.push_back(binding);

				// descr_set_layout_bindings.push_back(binding->layout);
			}

			// cout << "Bindings: " << descr_set_layout_bindings.size() << endl;
			layout = renderer->device.DescrSetLayout(descr_set_layout_bindings.size(), descr_set_layout_bindings.data());

			handle = renderer->device.DescrSet(renderer->descriptor_pool, 1, &layout).data()[0];

			// cout << "handle: " << handle << endl;

			// for (UniformBlock* binding : bindings)
			// {
			// 	// VkWriteDescriptorSet write_descr_set =
			// 	// 	WriteDescrSet
			// 	// 	(
			// 	// 		handle, binding->wrapper->binding, 0,
			// 	// 		1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			// 	// 		nullptr,
			// 	// 		&binding->descr_bi,
			// 	// 		nullptr
			// 	// 	);

			// 	// write_descr_sets.push_back(write_descr_set);
			// }



			renderer->device.updateDescrSets(write_descr_sets.size(), write_descr_sets.data(), 0, nullptr);
		}

		// void DescriptorSet::goOn (void)
		// {
		// 	// handle = renderer->device.DescrSet(renderer->descriptor_pool, 1, &layout).data()[0];

		// 	std::vector<VkWriteDescriptorSet> write_descr_sets {};

		// 	for (UniformBlock* binding : bindings)
		// 	{
		// 		VkWriteDescriptorSet write_descr_set =
		// 			WriteDescrSet
		// 			(
		// 				handle, binding->wrapper->binding, 0,
		// 				1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		// 				nullptr,
		// 				&binding->descr_bi,
		// 				nullptr
		// 			);

		// 		write_descr_sets.push_back(write_descr_set);
		// 	}



		// 	renderer->device.updateDescrSets(write_descr_sets.size(), write_descr_sets.data(), 0, nullptr);
		// }

		void DescriptorSet::use (Material* material)
		{
			vkCmdBindDescriptorSets(renderer->cmd_buffers[renderer->curr_image], VK_PIPELINE_BIND_POINT_GRAPHICS, material->ppl_layout, 0, 1, &handle, 0, nullptr);
		}



		Material* Material::used_instance {};

		const VkPrimitiveTopology Material::TOPOLOGY [3]
		{
			VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
			VK_PRIMITIVE_TOPOLOGY_POINT_LIST,
			VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
		};

		const VkFrontFace Material::FRONT_FACE [2]
		{
			VK_FRONT_FACE_COUNTER_CLOCKWISE,
			VK_FRONT_FACE_CLOCKWISE,
		};

		const VkBool32 Material::BLEND_ENABLED [2]
		{
			VK_FALSE,
			VK_TRUE,
		};

		const VkBlendOp Material::BLEND_OP [5]
		{
			VK_BLEND_OP_ADD,
			VK_BLEND_OP_SUBTRACT,
			VK_BLEND_OP_REVERSE_SUBTRACT,
			VK_BLEND_OP_MIN,
			VK_BLEND_OP_MAX,

			// ...and many others available via extensions.
		};

		const VkBlendFactor Material::BLEND_FACTOR [2]
		{
			VK_BLEND_FACTOR_ZERO,
			VK_BLEND_FACTOR_ONE,
		};

		Material::Material (RendererBase* _renderer, WRAPPERS::Material* _wrapper, const MATERIAL::ShaderUsage shader_usage)
		{
			{
				renderer = _renderer;
				wrapper = _wrapper;
			}



			topology = Material::TOPOLOGY[static_cast<size_t>(wrapper->topology)];

			VkPipelineInputAssemblyStateCreateInfo default_ppl_input_asm { PplInputAsm(topology, VK_FALSE) };



			VkPipelineTessellationStateCreateInfo default_ppl_tess { PplTess(0, 0) };

			// flip vulkan viewport
			VkViewport viewport { 0.0f, 0.0f, static_cast<float>(renderer->wrapper->width), static_cast<float>(renderer->wrapper->height), 0.0f, 1.0f };

			VkRect2D scissor { { 0, 0 }, { renderer->wrapper->width, renderer->wrapper->height } };

			VkPipelineViewportStateCreateInfo default_ppl_view { PplView(1, &viewport, 1, &scissor) };

			VkPipelineMultisampleStateCreateInfo default_ppl_sample { PplSample(VK_SAMPLE_COUNT_1_BIT, VK_FALSE, 0.0f, nullptr, VK_FALSE, VK_FALSE) };



			front_face = Material::FRONT_FACE[static_cast<size_t>(wrapper->front_face)];

			VkPipelineRasterizationStateCreateInfo default_ppl_rast
			{ PplRast(VK_FALSE, VK_FALSE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, front_face, VK_FALSE, 0.0f, 0.0f, 0.0f, 1.0f) };



			VkStencilOpState default_ppl_stenc { VK_STENCIL_OP_KEEP, VK_STENCIL_OP_KEEP, VK_STENCIL_OP_KEEP, VK_COMPARE_OP_ALWAYS, 0, 0, 0 };

			VkStencilOpState front
			{
				.failOp = VK_STENCIL_OP_KEEP,
				.passOp = VK_STENCIL_OP_KEEP,
				.compareOp = VK_COMPARE_OP_ALWAYS,
			};

			VkStencilOpState back
			{
				.failOp = VK_STENCIL_OP_KEEP,
				.passOp = VK_STENCIL_OP_KEEP,
				.compareOp = VK_COMPARE_OP_ALWAYS,
			};

			VkPipelineDepthStencilStateCreateInfo default_ppl_depth_stenc { PplDepthStenc(VK_TRUE, VK_TRUE, VK_COMPARE_OP_LESS_OR_EQUAL, VK_FALSE, VK_FALSE, default_ppl_stenc, default_ppl_stenc, 0.0f, 1.0f) };

			// VkPipelineDepthStencilStateCreateInfo default_ppl_depth_stenc { PplDepthStenc(VK_FALSE, VK_FALSE, VK_COMPARE_OP_LESS, VK_FALSE, VK_FALSE, { 0 }, { 0 }, 0.0f, 1.0f) };



			blend_enabled = Material::BLEND_ENABLED[static_cast<size_t>(wrapper->blend_enabled)];

			blend_color_op = Material::BLEND_OP[static_cast<size_t>(wrapper->blend_color_op)];
			blend_color_factor_src = Material::BLEND_FACTOR[static_cast<size_t>(wrapper->blend_color_factor_src)];
			blend_color_factor_dst = Material::BLEND_FACTOR[static_cast<size_t>(wrapper->blend_color_factor_dst)];

			blend_alpha_op = Material::BLEND_OP[static_cast<size_t>(wrapper->blend_alpha_op)];
			blend_alpha_factor_src = Material::BLEND_FACTOR[static_cast<size_t>(wrapper->blend_alpha_factor_src)];
			blend_alpha_factor_dst = Material::BLEND_FACTOR[static_cast<size_t>(wrapper->blend_alpha_factor_dst)];

			VkPipelineColorBlendAttachmentState blend_attach
			{
				blend_enabled,
				blend_color_factor_src, blend_color_factor_dst, blend_color_op,
				blend_alpha_factor_src, blend_alpha_factor_dst, blend_alpha_op,
				VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
			};

			VkPipelineColorBlendStateCreateInfo default_ppl_blend
			{
				PplBlend
				(
					VK_FALSE,
					VK_LOGIC_OP_CLEAR,
					1, &blend_attach,
					0.0f, 0.0f, 0.0f, 0.0f
				)
			};



			VkPipelineDynamicStateCreateInfo default_ppl_dyn { PplDyn(0, nullptr) };



			VkPipelineShaderStageCreateInfo ppl_stages [2] {};

			switch (shader_usage)
			{
				case MATERIAL::ShaderUsage::SPIRV:
				{
					ppl_stages[0] = PplShader(VK_SHADER_STAGE_VERTEX_BIT, renderer->device.Shader(wrapper->code_vertex_spirv.size() * sizeof(uint32_t), wrapper->code_vertex_spirv.data()));
					ppl_stages[1] = PplShader(VK_SHADER_STAGE_FRAGMENT_BIT, renderer->device.Shader(wrapper->code_fragment_spirv.size() * sizeof(uint32_t), wrapper->code_fragment_spirv.data()));

					break;
				}

				case MATERIAL::ShaderUsage::GLSL:
				{
					std::vector<uint32_t> code_vertex_spirv = compileGlslToSpirv(wrapper->code_vertex_glsl.c_str(), GLSLANG_STAGE_VERTEX);
					std::vector<uint32_t> code_fragment_spirv = compileGlslToSpirv(wrapper->code_fragment_glsl.c_str(), GLSLANG_STAGE_FRAGMENT);

					ppl_stages[0] = PplShader(VK_SHADER_STAGE_VERTEX_BIT, renderer->device.Shader(code_vertex_spirv.size() * sizeof(uint32_t), code_vertex_spirv.data()));
					ppl_stages[1] = PplShader(VK_SHADER_STAGE_FRAGMENT_BIT, renderer->device.Shader(code_fragment_spirv.size() * sizeof(uint32_t), code_fragment_spirv.data()));

					break;
				}

				default: break;
			}



			VkVertexInputBindingDescription vertex_binding { 0, 12, VK_VERTEX_INPUT_RATE_VERTEX };
			VkVertexInputAttributeDescription vertex_attr { 0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0 };

			VkPipelineVertexInputStateCreateInfo ppl_vertex { PplVertex(1, &vertex_binding, 1, &vertex_attr) };



			std::vector<VkDescriptorSetLayout> descr_set_layouts {};

			for (WRAPPERS::DescriptorSet* descriptor_set_wrapper : wrapper->descriptor_sets)
			{
				DescriptorSet* descriptor_set { getInstance<DescriptorSet, WRAPPERS::DescriptorSet>(renderer, descriptor_set_wrapper) };

				descr_set_layouts.push_back(descriptor_set->layout);
			}

			ppl_layout = renderer->device.PplLayout(descr_set_layouts.size(), descr_set_layouts.data());



			ppl =
				renderer->device.PplG
				(
					2, ppl_stages,
					&ppl_vertex,
					&default_ppl_input_asm,
					&default_ppl_tess,
					&default_ppl_view,
					&default_ppl_rast,
					&default_ppl_sample,
					&default_ppl_depth_stenc,
					&default_ppl_blend,
					&default_ppl_dyn,
					ppl_layout,
					renderer->render_pass, 0
				);
		}

		void Material::use (void)
		{
			// Material::used_instance = this;

			vkCmdBindPipeline(renderer->cmd_buffers[renderer->curr_image], VK_PIPELINE_BIND_POINT_GRAPHICS, ppl);
		}



		Object::Object (RendererBase* _renderer, WRAPPERS::Object* _wrapper)
		{
			renderer = _renderer;
			wrapper = _wrapper;
		}

		void Object::draw (void) const
		{
			vkCmdDraw(renderer->cmd_buffers[renderer->curr_image], wrapper->scene_vertex_data_length, 1, wrapper->scene_vertex_data_offset, 0);
		}

		void Object::draw2 (void) const
		{
			vkCmdDraw(renderer->cmd_buffers[renderer->curr_image], wrapper->position_data.size() / 3, 1, 0, 0);
		}

		void Object::createBuffers (void)
		{
			position_buffer = renderer->device.Buffer(wrapper->position_data.size() * sizeof(float), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE);

			VkMemoryRequirements position_buffer_mem_reqs { renderer->device.MemReqs(position_buffer) };

			uint64_t position_buffer_mem_index { renderer->device.getMemTypeIndex(&position_buffer_mem_reqs, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) };

			VkDeviceMemory position_buffer_mem { renderer->device.Mem(position_buffer_mem_reqs.size, position_buffer_mem_index) };

			renderer->device.bindMem(position_buffer, position_buffer_mem);

			void* position_buffer_mem_addr { renderer->device.mapMem(position_buffer_mem, 0, wrapper->position_data.size() * sizeof(float), 0) };

			memcpy(position_buffer_mem_addr, wrapper->position_data.data(), wrapper->position_data.size() * sizeof(float));
		}



		Scene::Scene (RendererBase* _renderer, WRAPPERS::Scene* _wrapper)
		{
			renderer = _renderer;
			wrapper = _wrapper;
		}
	}
}
