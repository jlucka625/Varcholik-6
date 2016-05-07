#include "pch.h"

using namespace LibraryDesktop;

std::uint32_t DefaultIncrementFunctor::operator()(std::uint32_t size, std::uint32_t capacity) const
{
	return ((capacity + 1) + (size * 0));
}
