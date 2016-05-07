#pragma once

#include "HashMap.h"

namespace LibraryDesktop
{
	template <class AbstractProductT> class Factory
	{
		//Factory Interface
	public:
		/** @brief					Retrieves the string representation of the concrete product's type.
		*	@return					The string representation of the concrete product's type.
		*/
		virtual const std::string& ClassName(void) const = 0;

	private:
		/** @brief					Non-static Create method. All factories know how to create a single type of product, so it will make a heap-allocated instance of whatever kind of
		*							product it knows how to make (provided the class exists.)
		*	@return					A heap-allocated product of a specific type.
		*	@exception				If the class the product is associated with does not exist, then the program will crash.
		*/
		virtual AbstractProductT* Create(void) const = 0;

		//Manager Interface
	public:
		typedef const Factory<AbstractProductT>* const FactoryPointer;
		typedef HashMap<std::string, FactoryPointer> FactoryTable;

		/** @brief					Searches for a factory in the manager's table of factories and returns the address of it if it exists, or nullptr otherwise.
		*	@param strClassName		String representation of the concrete product's type.
		*	@return					The address of the specified factory, or nullptr if it doesn't exist.
		*/
		static FactoryPointer Find(const std::string& strFactoryName)
		{
			FactoryTable::Iterator it = sFactoryTable.Find(strFactoryName);

			if (it == end())
			{
				return nullptr;
			}

			return (*it).second;
		}

		/** @brief					Static Create method. (See description of non-static Create above for more details.) Once the product has been created, this method will add it to
		*							the list of products this factory has created.
		*	@param strClassName		Name of the product's type.
		*	@return					A heap-allocated product of a specific type.
		*	@exception				If the class the product is associated with does not exist, then the program will crash.
		*/
		static AbstractProductT* Create(const std::string& strClassName)
		{
			FactoryPointer factory = Find(strClassName);

			if (factory == nullptr)
			{
				throw std::exception("Error: the class associated with this factory does not exist");
			}

			return factory->Create();
		}

		/** @brief					Retrieves an iterator to the beginning of the manager's table of factories.
		*	@return					An iterator to the beginning of the manager's table of factories.
		*/
		static typename FactoryTable::Iterator begin() { return sFactoryTable.begin(); }

		/** @brief					Retrieves an iterator to one past the end of the manager's table of factories.
		*	@return					An iterator to one past the end of the manager's table of factories.
		*/
		static typename FactoryTable::Iterator end() { return sFactoryTable.end(); }

		/** @brief					Retrieves the size of, or number of factories within, the manager's table of factories.
		*	@return					The number of factories within, the manager's table of factories.
		*/
		static std::uint32_t Size() { return sFactoryTable.Size(); }

		/** @brief					Dictates whether or not the manager's table of factories has any entries in it.
		*	@return					True if the table has at least one entry, and false otherwise.
		*/
		static bool IsEmpty() { return sFactoryTable.IsEmpty(); }

	protected:
		/** @brief					Adds a factory to the manager's table of factories. If a factory of the specified type already exists, nothing will happen.
		*   @param pFactory			Factory to add.
		*/
		static void Add(Factory<AbstractProductT>& pFactory) { sFactoryTable.Insert(std::pair<std::string, FactoryPointer>(pFactory.ClassName(), &pFactory)); }

		/** @brief					Removes a factory from the manager's table of factories, and destroys all of its products with it.
		*   @param pFactory			Factory to remove.
		*/
		static void Remove(Factory<AbstractProductT>& pFactory) { sFactoryTable.Remove(pFactory.ClassName()); }

	private:
		/** @brief					Table of factories.
		*
		*/
		static FactoryTable sFactoryTable;
	};

#define ConcreteFactory( AbstractProductT, ConcreteProductT )										\
    class ConcreteProductT ## Factory : public Factory<AbstractProductT>							\
    {																								\
        public:																						\
             ConcreteProductT ## Factory() : mClassName(# ConcreteProductT) { Add   ( *this ) ; }	\
            ~ConcreteProductT ## Factory()  { Remove( *this ) ; }									\
            virtual const std::string& ClassName( void ) const override								\
			{ return mClassName; }																	\
            virtual AbstractProductT *  Create( void ) const override								\
            {																						\
                AbstractProductT * product = new ConcreteProductT() ;								\
                assert( product != NULL ) ;															\
                return product ;																	\
            }																						\
		private:																					\
			std::string mClassName;																	\
	};																								

	template <class AbstractProductT> typename Factory<AbstractProductT>::FactoryTable Factory<AbstractProductT>::sFactoryTable;
}