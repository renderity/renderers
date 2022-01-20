#ifndef __RDTY_RENDERERS_BASE__
#define __RDTY_RENDERERS_BASE__



#include <cstddef> // size_t
#include <vector>

#include "wrappers/src/base/base.h"
#include "wrappers/src/renderer/renderer.h"



namespace RDTY::RENDERERS
{
	struct Renderer
	{
		WRAPPERS::Renderer* wrapper {};

		std::vector<WRAPPERS::Base*> wrappers {};

		void* pixel_data {};



		virtual void beginLoop (void) = 0;
		virtual void endLoop (void) = 0;
		virtual void destroy (void) = 0;
	};
}



#endif
