#ifndef __RDTY_RENDERERS_VULKAN__
#define __RDTY_RENDERERS_VULKAN__



#if defined(__GNUC__)
	#define INLINE __attribute__((always_inline)) inline
#elif defined(_MSC_VER)
	#define INLINE __forceinline
#endif



#include <cstddef> // size_t
#include <vector>
#include <map>

#include <iostream>
using std::cout;
using std::endl;

#include "glslang/Include/glslang_c_interface.h"

#include "renderers/src/glfw/glfw-3.3.5/include/GLFW/glfw3.h"

#if defined(__linux__)
	#define GLFW_EXPOSE_NATIVE_X11
#elif defined(_WIN64)
	#define GLFW_EXPOSE_NATIVE_WIN32
#endif

#include "renderers/src/glfw/glfw-3.3.5/include/GLFW/glfw3native.h"

#include "renderers/src/vulkan/helpers.h"
#include "renderers/src/base/renderer.h"

#include "wrappers/src/renderer/renderer.h"
#include "wrappers/src/uniform/uniform.h"
#include "wrappers/src/descriptor-binding/descriptor-binding.h"
#include "wrappers/src/uniform-block/uniform-block.h"
#include "wrappers/src/storage-block/storage-block.h"
#include "wrappers/src/descriptor-set/descriptor-set.h"
#include "wrappers/src/material/material.h"
#include "wrappers/src/object/object.h"
#include "wrappers/src/scene/scene.h"




std::vector<uint32_t> compileGlslToSpirv (const char*, glslang_stage_t);



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

				// delete[] inst.physical_devices;

				// vkDestroyInstance(inst.handle, nullptr);
				inst.destroy();

				return result;
			}



			// WRAPPERS::Renderer* wrapper {};

			GLFWwindow* window {};



			RendererBase (WRAPPERS::Renderer*);



			Instance inst {};
			Device device {};
			VkQueue graphics_queue { VK_NULL_HANDLE };

			VkDescriptorPool descriptor_pool { VK_NULL_HANDLE };

			VkRenderPass render_pass { VK_NULL_HANDLE };

			// Per swapchain image objects
			std::vector<VkFramebuffer> framebuffers {};
			std::vector<VkSubmitInfo> submit_i {};
			std::vector<VkRenderPassBeginInfo> render_pass_bi {};
			std::vector<VkCommandBuffer> cmd_buffers {};



			// TODO: remove, rename
			VkClearValue clear_value [2] { {}, {} };
			uint32_t curr_image {};
			//



			virtual void destroy (void) override final;

			// virtual void beginRenderPass (const VkRenderPassBeginInfo&&) = 0;
		};



		struct Renderer : public RendererBase
		{
			// Renderer (WRAPPERS::Renderer*, const VkPhysicalDevice& = VK_NULL_HANDLE);
			Renderer (WRAPPERS::Renderer*, const size_t& physical_device_index = 0);


			VkSurfaceKHR surf { VK_NULL_HANDLE };
			VkQueue present_queue { VK_NULL_HANDLE };

			VkSwapchainKHR swapchain { VK_NULL_HANDLE };
			uint64_t swapchain_image_count {};

			std::vector<VkFence> submission_completed_fences {};
			std::vector<VkSemaphore> image_available_semaphores {};
			std::vector<VkSemaphore> submission_completed_semaphores {};
			std::vector<uint32_t> image_indices {};
			std::vector<VkPresentInfoKHR> present_i {};

			// uint32_t curr_image {};



			virtual void beginLoop (void) override;
			virtual void endLoop (void) override;
			// virtual void beginRenderPass (const VkRenderPassBeginInfo&&) override;
		};



		struct RendererOffscreen : public RendererBase
		{
			// RendererOffscreen (WRAPPERS::Renderer*, const VkPhysicalDevice& = VK_NULL_HANDLE);
			RendererOffscreen (WRAPPERS::Renderer*, const size_t& physical_device_index = 0);



			std::vector<VkImage> render_images {};
			VkBuffer pixel_buffer { VK_NULL_HANDLE };
			// void* pixel_data {};



			virtual void beginLoop (void) override;
			virtual void endLoop (void) override;
			// virtual void beginRenderPass (const VkRenderPassBeginInfo&&) override;
		};



		struct Uniform
		{
			RendererBase* renderer {};
			WRAPPERS::Uniform* wrapper {};



			Uniform (RendererBase*, WRAPPERS::Uniform*);
		};



		struct UniformBlock
		{
			static const VkShaderStageFlagBits VISIBILITY [3];



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
				.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_COMPUTE_BIT,
				.pImmutableSamplers = nullptr,
			};



			UniformBlock (RendererBase*, WRAPPERS::UniformBlock*);



			void use (void);
		};



		struct StorageBlock
		{
			static const VkShaderStageFlags VISIBILITY [3];



			RendererBase* renderer {};
			WRAPPERS::StorageBlock* wrapper {};

			VkBuffer buffer {};

			void* mapped_memory_addr {};

			VkDescriptorBufferInfo descr_bi { buffer, 0, VK_WHOLE_SIZE };

			VkDescriptorSetLayoutBinding layout
			{
				.binding = 0,
				.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
				.descriptorCount = 1,
				.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_COMPUTE_BIT,
				.pImmutableSamplers = nullptr,
			};



			StorageBlock (RendererBase*, WRAPPERS::StorageBlock*);



			void use (void);
		};



		struct Material;

		struct DescriptorSet
		{
			RendererBase* renderer {};
			WRAPPERS::DescriptorSet* wrapper {};



			// std::vector<UniformBlock*> bindings {};



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
				GLSL,
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

			static const VkBool32 BLEND_ENABLED [2];
			static const VkBlendOp BLEND_OP [5];
			static const VkBlendFactor BLEND_FACTOR [2];



			RendererBase* renderer {};
			WRAPPERS::Material* wrapper {};

			VkPrimitiveTopology topology {};
			VkFrontFace front_face {};

			VkBool32 blend_enabled {};

			VkBlendOp blend_color_op {};
			VkBlendFactor blend_color_factor_src {};
			VkBlendFactor blend_color_factor_dst {};

			VkBlendOp blend_alpha_op {};
			VkBlendFactor blend_alpha_factor_src {};
			VkBlendFactor blend_alpha_factor_dst {};

			VkPipelineLayout ppl_layout {};
			VkPipeline ppl {};

			// GLuint program {};

			// std::vector<Uniform*> uniforms {};
			// std::vector<UniformBlock*> uniform_blocks {};



			Material (RendererBase*, WRAPPERS::Material*, const MATERIAL::ShaderUsage = MATERIAL::ShaderUsage::GLSL);



			void use (void);
		};



		struct Object
		{
			RendererBase* renderer {};
			WRAPPERS::Object* wrapper {};



			VkBuffer position_buffer {};



			Object (RendererBase*, WRAPPERS::Object*);



			void draw (void) const;
			void draw2 (void) const;
			void createBuffers (void);
		};



		struct Scene
		{
			RendererBase* renderer {};
			WRAPPERS::Scene* wrapper {};



			Scene (RendererBase*, WRAPPERS::Scene*);
		};




		struct Image
		{
			VkImage image { VK_NULL_HANDLE };
			VkImageView image_view { VK_NULL_HANDLE };
		};



		template <class T, class WrapperT>
		T* getInstance (RendererBase* renderer, WrapperT* wrapper)
		{
			T* instance {};

			if (wrapper->impl_vulkan)
			{
				instance = static_cast<T*>(wrapper->impl_vulkan);
			}
			else
			{
				instance = new T { renderer, wrapper };

				wrapper->impl_vulkan = instance;

				renderer->wrappers.push_back(wrapper);
			}

			return instance;
		}

		template <class T, class WrapperT>
		T* getInstance (RDTY::RENDERERS::Renderer* renderer, WrapperT* wrapper)
		{
			T* instance {};

			if (wrapper->impl_vulkan)
			{
				instance = static_cast<T*>(wrapper->impl_vulkan);
			}
			else
			{
				RendererBase* _renderer = dynamic_cast<RendererBase*>(renderer);

				instance = new T { _renderer, wrapper };

				wrapper->impl_vulkan = instance;

				_renderer->wrappers.push_back(wrapper);
			}

			return instance;
		}
	}
}



#define RDTY_VULKAN_GET_INSTANCE(type, renderer, wrapper) RDTY::VULKAN::getInstance<RDTY::VULKAN:: type, RDTY::WRAPPERS:: type>(renderer, wrapper)



#endif
