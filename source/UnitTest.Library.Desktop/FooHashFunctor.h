#pragma once

#include <cstdint>
#include <memory>

#include "Foo.h"

namespace UnitTestLibraryDesktop
{
	template <typename T> class FooHashFunctor
	{
	public:
		std::uint32_t operator()(const Foo& key)
		{
			//std::uintptr_t pointerAddress = reinterpret_cast<std::uintptr_t>(key.PointerData());
			return (key.Data() + (*key.PointerData()));
		}
	};
}
