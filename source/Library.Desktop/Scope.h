#pragma once
#include <cstdint>

#include "RTTI.h"

#include "Vector.h"
#include "HashMap.h"
#include "Datum.h"

namespace LibraryDesktop
{
	class Scope : public RTTI
	{
		RTTI_DECLARATIONS(Scope, RTTI)
	public:

		/** @brief					Constructor for the Scope class.
		*   @param size				Default size of the Scope.
		*   @return                 Instance of the Scope class.
		*/
		Scope(const std::uint32_t& size = 6U);

		/** @brief					Copy constructor for the Scope class.
		*   @param otherScope		Constant reference to another Scope.
		*   @return                 Instance of the Scope class from the deep copy of another.
		*/
		Scope(const Scope& otherScope);

		/** @brief					Move constructor for the Scope class.
		*   @param otherScope		Right-hand reference to another Scope.
		*   @return                 Instance of the Scope class from the transferred ownership of another.
		*/
		Scope(Scope&& otherScope);

		/** @brief					Assignment operator for the Scope class.
		*   @param otherScope		Constant reference to another Scope.
		*   @return                 Instance of the Scope class from the deep copy of another.
		*/
		Scope& operator=(const Scope& otherScope);

		/** @brief					Move assignment operator for the Scope class.
		*   @param otherScope		Right-hand reference to another Scope.
		*   @return                 Instance of the Scope class from the transferred ownership of another.
		*/
		Scope& operator=(Scope&& otherScope);

		/** @brief					Destructor for the Scope class.
		*
		*/
		virtual ~Scope();

		/** @brief					Equality operator for the Scope class. (To be equal, the symbol tables and symbol table entry pointer vectors must be the same.)
		*   @param otherScope		Constant reference to another Scope.
		*   @return                 True if the Scopes are the same, and false otherwise.
		*/
		bool operator==(const Scope& otherScope) const;

		/** @brief					Inequality operator for the Scope class.
		*   @param otherScope		Constant reference to another Scope.
		*   @return                 False if the Scopes are the same, and true otherwise.
		*/
		bool operator!=(const Scope& otherScope) const;

		/** @brief					Getter for the size of the Scope.
		*   @return                 The current number of std::string-Datum pairs.
		*/
		std::uint32_t Size() const;

		/** @brief					Getter for the capacity of the Scope.
		*   @return                 The maximum number of std::string-Datum pairs allowed before re-allocation.
		*/
		std::uint32_t Capacity() const;

		/** @brief					Attempts to retrieve the address of the Datum with the specified name in this Scope. If no such Datum is found, a nullptr is returned instead.
		*   @param name				The name of the Datum to find.
		*   @return					The address of the corresponding Datum, or nullptr if no such Datum exists in this Scope.
		*/
		Datum* Find(const std::string& name) const;

		/** @brief					Traverses a hierarchy of Scopes (starting from the bottom and going up) in an attempts to retrieve the address of the Datum with the specified name. (Note that if Datums with the specified name are present in each nested Scope, the address of the closest one is returned.)
		*   @param name				The name of the Datum to find.
		*   @return					The address of the closest Datum with the specified name, or nullptr if no such Datum exists anywhere in the hierarchy.
		*/
		Datum* Search(const std::string& name, Scope** scope = nullptr);

		/** @brief					Adds a Datum with a specified name to the scope, or returns the existing one.
		*   @param name				The name of the Datum to append.
		*   @return					A new or existing Datum in accordance with the specified name.
		*   @exception				An std::exception occurs if the specified name is an empty string.
		*/
		Datum& Append(const std::string& name);

		/** @brief					Helper function for the single-parameter version of Append (see above for description of objective.)
		*   @param name				The name of the Datum to append.
		*	@param wasAppended		Output parameter that dictates whether or not the Datum was successfully appended to the scope.
		*/
		Datum& Append(const std::string& name, bool& wasAppended);

		/** @brief					Public accessor for the parent of this Scope.
		*   @return					Address of this Scope's parent.
		*/
		Scope* GetParent();

		/** @brief					Subscript operator with a string index. (Much like the subscript operator for HashMap, it creates a new Datum to append to the Scope if no pair with the specified index exists, or returns the Datum of the already-existing pair.)
		*   @param name				Key of an std::string-Datum pair.
		*   @return					A reference to a new or existing Datum.
		*   @exception				An std::exception occurs if the specified name is an empty string, or an existing Datum with the specified name is not of type Table.
		*/
		Datum& operator[](const std::string& name);

		Datum& operator[](const std::string& name) const;

		/** @brief					Subscript operator with a number index.
		*   @param index			Value representing the order in which the Datum was stored in.
		*   @return					A reference to the Datum at the specified index.
		*   @exception
		*/
		Datum& operator[](const std::uint32_t& index);

		/** @brief					Adds a new child Scope to a parent Scope. (Note that if a Datum of type Table with the specified name hasn't already been created, one is created before the appending occurs. Otherwise, the existing one is used.)
		*   @param name				Name of the Scope to create.
		*   @return					A reference to the newly-appended Scope.
		*   @exception				An std::exception occurs if the specified name is an empty string, or an existing Datum with the specified name is not of type Table.
		*/
		Scope& AppendScope(const std::string& name);

		/** @brief					Appends a child Scope to a parent Scope with the specified name. (Note that the child in question will always be orphaned (a call to Orphan will be made) before the adoption takes place.)
		*   @param child			Reference of the Scope to adopt.
		*   @param name				Name of the parent Scope that does the adopting.
		*   @exception				An std::exception occurs if the specified name is an empty string, or an existing Datum with the specified name is not of type Table.
		*/
		void Adopt(Scope& child, const std::string& name);

		/** @brief					Retrieves the name of the specified Scope. If the Scope does not exist in this Scope, an empty string is returned instead.
		*   @param name				The address of the Scope to search for.
		*   @return					The name of the specified Scope, or an empty string if the Scope wasn't found.
		*/
		std::string FindName(const Scope& scope) const;

		/** @brief					Disowns the specified child Scope of this parent.
		*   @param child			Child Scope to disown.
		*/
		//void Orphan(Scope& child);
		void Orphan();

		void Reparent();

		/** @brief					De-allocates any memory created by the Scope.
		*
		*/
		virtual void Clear();
	protected:
		typedef HashMap<std::string, Datum, DefaultHashFunctor<std::string>> SymbolTable;

		/** @brief					Private constructor used for AppendScope.
		*   @param name				Name of the Datum of type Table that this Scope resides in.
		*   @param size				Default size of the Scope.
		*   @return                 Instance of the Scope class.
		*/
		Scope(const std::string& name, const std::uint32_t& size = 6U);

		/** @brief					Keeps track of all Datum values (and their given names) within this Scope.
		*
		*/
		SymbolTable mSymbolTable;

		/** @brief					Keeps track of pointers to symbol table entries (used to make lookups much faster.)
		*
		*/
		Vector<SymbolTable::PairType*> mOrderVector;

		/** @brief					Address of the parent of this Scope (nullptr means the child Scope has no parent.)
		*
		*/
		Scope* mParent;

		/** @brief					Name of the Datum containing this Scope.
		*
		*/
		std::string mParentName;
	};
}
