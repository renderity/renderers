#ifndef __RDTY_RENDERERS_OPENGL__
#define __RDTY_RENDERERS_OPENGL__



// size_t
#include <cstddef>
#include <string>

#include "renderers/src/glad/include/glad/glad.h"
#include "renderers/src/glfw/glfw-3.3.5/include/GLFW/glfw3.h"
#include "renderers/src/base/renderer.h"

#include "wrappers/src/renderer/renderer.h"
#include "wrappers/src/uniform/uniform.h"
#include "wrappers/src/uniform-block/uniform-block.h"
#include "wrappers/src/material/material.h"
#include "wrappers/src/object/object.h"
#include "wrappers/src/scene/scene.h"



namespace RDTY
{
	namespace OPENGL
	{
		struct RendererBase : public RDTY::RENDERERS::Renderer
		{
			static bool loaded;

			static std::string test (void)
			{
				glfwInit();

				GLFWwindow* window = glfwCreateWindow(1, 1, "", nullptr, nullptr);

				glfwHideWindow(window);
				glfwMakeContextCurrent(window);

				// gladLoadGL();
				gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

				RendererBase::loaded = true;

				std::string opengl_version((const char*) glGetString(GL_VERSION));

				glfwDestroyWindow(window);
				glfwTerminate();

				return opengl_version;
			}



			// WRAPPERS::Renderer* wrapper {};

			GLFWwindow* window {};



			RendererBase (WRAPPERS::Renderer*);



			virtual void destroy (void) override final;
		};



		struct Renderer : public RendererBase
		{
			Renderer (WRAPPERS::Renderer*);



			virtual void beginLoop (void) override;
			virtual void endLoop (void) override;
		};



		struct RendererOffscreen : public RendererBase
		{
			// void* pixel_data {};

			GLuint framebuffer {};
			GLuint framebuffer_renderbuffer_color {};
			GLuint framebuffer_renderbuffer_depth {};
			GLuint pixel_pack_buffer {};

			size_t pbo_index {};
			size_t next_pbo_index { 1 };



			RendererOffscreen (WRAPPERS::Renderer*);



			virtual void beginLoop (void) override;
			virtual void endLoop (void) override;
		};



		struct Uniform
		{
			// uniform_update_t functions
			static void uniformMatrix4fv (Uniform*);



			RendererBase* renderer {};
			WRAPPERS::Uniform* wrapper {};

			GLint location {};

			// GLint locaiton {};

			using uniform_update_t = void (*) (Uniform*);

			uniform_update_t update {};



			// Isn't Renderer* parameter needed?
			Uniform (RendererBase*, WRAPPERS::Uniform*);
		};



		struct UniformBlock
		{
			RendererBase* renderer {};
			WRAPPERS::UniformBlock* wrapper {};

			GLuint buffer {};

			size_t buffer_length {};

			std::vector<Uniform*> uniforms {};



			UniformBlock (RendererBase*, WRAPPERS::UniformBlock*);



			void use (void);
		};



		struct Material
		{
			static Material* used_instance;

			static const GLenum TOPOLOGY [3];
			static const GLenum FRONT_FACE [2];



			RendererBase* renderer {};
			WRAPPERS::Material* wrapper {};

			GLenum topology {};
			GLenum front_face {};

			GLuint program {};

			std::vector<Uniform*> uniforms {};
			std::vector<UniformBlock*> uniform_blocks {};



			Material (RendererBase*, WRAPPERS::Material*);



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

			if (wrapper->impl_opengl)
			{
				instance = static_cast<T*>(wrapper->impl_opengl);
			}
			else
			{
				instance = new T { renderer, wrapper };

				wrapper->impl_opengl = instance;

				renderer->wrappers.push_back(wrapper);
			}

			return instance;
		}
	}
}



#define RDTY_OPENGL_GET_INSTANCE(type, renderer, wrapper) RDTY::OPENGL::getInstance<RDTY::OPENGL:: type, RDTY::WRAPPERS:: type>(renderer, wrapper)



#endif
