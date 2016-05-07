#include "pch.h"

using namespace LibraryDesktop;

std::uint32_t ZeroIncrementFunctor::operator()(std::uint32_t size, std::uint32_t capacity) const
{
	return (size * capacity * 0);
}
