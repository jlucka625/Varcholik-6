#pragma once
#include <cstdint>

namespace LibraryDesktop
{
	class DoubleSizeIncrementFunctor
	{
	public:
		/** @brief					Calculates the new capacity of the vector.
		*  @param size				current size of the vector.
		*  @param capacity			current capacity of the vector.
		*  @return					A new capacity value equal to the current size doubled.
		*/
		std::uint32_t operator()(std::uint32_t size, std::uint32_t capacity) const;
	};
}

