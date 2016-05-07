#pragma once

#include <cstdint>
#include <memory>
#include <Windows.h>

#include "Foo.h"

namespace UnitTestLibraryDesktop
{
	template <typename T> class CustomHashFunctor
	{
	public:
		/** @brief					Hash functor used for any ADTs not specified below (or primitive types, which can be found in DefaultHashFunctor.h of the Library.Desktop project.)
		*   @param key				key to get the hashcode for.
		*	@return					Hashcode of the key.
		*/
		inline std::uint32_t operator()(const T& key)
		{
			UNREFERENCED_PARAMETER(key);
			return (index + 1);
		}
	private:
		/** @brief					Value used to generate non-constant hashcodes.
		*
		*/
		std::uint32_t index = 0;
	};

	template<> class CustomHashFunctor<Foo>
	{
	public:
		/** @brief					Hash functor for Foo.
		*   @param key				key to get the hashcode for.
		*	@return					Hashcode of the key.
		*/
		inline std::uint32_t operator()(const Foo& key)
		{
			return (key.Data() + (*key.PointerData()));
		}
	};
}
