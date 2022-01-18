#ifndef __RDTY_RENDERERS_BASE__
#define __RDTY_RENDERERS_BASE__



// size_t
#include <cstddef>
#include <vector>

#include "wrappers/src/base/base.h"
#include "wrappers/src/renderer/renderer.h"



namespace RDTY
{
	namespace RENDERERS
	{
		// TODO: remove?
		enum class RendererType : size_t
		{
			VULKAN,
			OPENGL,
		};



		struct Renderer
		{
			WRAPPERS::Renderer* wrapper {};

			void* pixel_data {};

			RendererType type {};

			std::vector<WRAPPERS::Base*> wrappers {};

			virtual void endLoop (void) = 0;
			virtual void destroy (void) = 0;
		};
	}
}



#endif
