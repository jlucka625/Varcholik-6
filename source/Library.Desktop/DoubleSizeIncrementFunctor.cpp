#include "pch.h"

using namespace LibraryDesktop;

std::uint32_t DoubleSizeIncrementFunctor::operator()(std::uint32_t size, std::uint32_t capacity) const
{
	return ((size * 2) + (capacity * 0));
}
