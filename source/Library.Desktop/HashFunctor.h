#pragma once

#include <cstdint>
#include <cstring>

namespace LibraryDesktop
{
	template<typename T> class DefaultHash
	{
	public:
		std::uint32_t operator()(const T& key) { return 0; }
	};

	template<> class DefaultHash<int>
	{
	public:
		std::uint32_t operator()(const int& key) { return key; }
	};

	template<> class DefaultHash<int*>
	{
	public:
		std::uint32_t operator()(const int* key) { return (*key); }
	};

	template<> class DefaultHash<char*>
	{
	public:
		std::uint32_t operator()(const char* key)
		{
			std::uint32_t hashCode = 0;

			for (size_t i = 0; i < strlen(key); i++)
			{
				hashCode += key[i];
			}

			return hashCode;
		}
	};

	template<> class DefaultHash<std::string>
	{
	public:
		std::uint32_t operator()(const std::string& key)
		{
			return DefaultHash<char*>().operator()(key.c_str());
		}
	};
}

