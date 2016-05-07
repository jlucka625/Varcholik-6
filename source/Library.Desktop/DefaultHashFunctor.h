#pragma once

#include <cstdint>
#include <cstring>
#include <Windows.h>

namespace LibraryDesktop
{
	template<typename T> class DefaultHashFunctor
	{
	public:
		/** @brief					Hash functor used for any types not specialized below (or Foo, which can be located in CustomHashFunctor.h of the UnitTest.Library.Desktop project.)
		*   @param key				key to get the hashcode for.
		*	@return					Hashcode of the key.
		*/
		//std::uint32_t operator()(const T& key){ return reinterpret_cast<uintptr_t>(&key); }
		std::uint32_t operator()(const T& key)
		{
			char *bytes = reinterpret_cast<char*>(reinterpret_cast<uintptr_t>(&key));
			return DefaultHashFunctor<char*>().operator()(bytes);
		}
	};

	template<> class DefaultHashFunctor<int>
	{
	public:
		/** @brief					Hash functor for int.
		*   @param key				key to get the hashcode for.
		*	@return					Hashcode of the key.
		*/
		std::uint32_t operator()(const int& key){ return (key * 31); }
	};

	template<> class DefaultHashFunctor<int*>
	{
	public:
		/** @brief					Hash functor for int*
		*   @param key				key to get the hashcode for.
		*	@return					Hashcode of the key.
		*/
		std::uint32_t operator()(const int* key){ return ((*key) * 31); }
	};

	template<> class DefaultHashFunctor<char*>
	{
	public:
		/** @brief					Hash functor for char*
		*   @param key				key to get the hashcode for.
		*	@return					Hashcode of the key.
		*/
		std::uint32_t operator()(const char* key)
		{
			std::uint32_t hashCode = 0;

			for (size_t i = 0; i < strlen(key); i++)
			{
				hashCode += (key[i] * 31);
			}

			return hashCode;
		}
	};

	template<> class DefaultHashFunctor<std::string>
	{
	public:
		/** @brief					Hash functor for std::string
		*   @param key				key to get the hashcode for.
		*	@return					Hashcode of the key.
		*/
		std::uint32_t operator()(const std::string& key){ return DefaultHashFunctor<char*>().operator()(key.c_str()); }
	};
}

