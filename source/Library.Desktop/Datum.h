#pragma once
#include <cstdint>
#include <functional>
#include <string>

#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include "HashMap.h"

namespace LibraryDesktop
{
	class RTTI;
	class Scope;

	class Datum
	{
	public:
		/** @brief					Enumeration of each type a Datum can assume (except for Max, which is mainly used for iteration purposes.)
		*
		*/
		enum class DatumType
		{
			Unknown,
			Integer,
			Float,
			Vector,
			Matrix,
			Table,
			String,
			Pointer,
			MAX
		};

		/** @brief					Constructor for the Datum class.
		*   @return                 Instance of the Datum class.
		*/
		explicit Datum();

		/** @brief					Copy constructor for the Datum class.
		*   @param otherDatum		Constant reference to another Datum.
		*   @return                 Instance of the Datum class from the deep copy of another.
		*/
		Datum(const Datum& otherDatum);

		/** @brief					Move constructor for the Datum class.
		*   @param otherDatum		Right-hand reference to another Datum.
		*   @return                 Instance of the Datum class from the transferred ownership of another.
		*/
		Datum(Datum&& otherDatum);

		/** @brief					Assignment operator for the Datum class.
		*   @param otherDatum		Constant reference to another Datum.
		*   @return                 Instance of the Datum class from the deep copy of another.
		*/
		Datum& operator=(const Datum& otherDatum);

		/** @brief					Move assignment operator for the Datum class.
		*   @param otherDatum		Right-hand reference to another Datum.
		*   @return                 Instance of the Datum class from the transferred ownership of another.
		*/
		Datum& operator=(Datum&& otherDatum);

		/** @brief					Destructor for the Datum class.
		*
		*/
		~Datum();

		/** @brief					States whether or not the Datum has external data (data it didn't create itself) or not.
		*   @return                 True if the Datum has external data, and false otherwise.
		*/
		bool IsExternal() const;

		/** @brief					Getter for the type of the Datum.
		*   @return                 A DatumType enum entry corresponding to the Datum's type.
		*/
		DatumType Type() const;

		/** @brief					Setter for the type of the Datum.
		*   @param type				type the Datum will assume.
		*   @exception				An std::exception is thrown if an attempt to change the type after it has already been set is made.
		*/
		void SetType(const DatumType& type);

		/** @brief					Getter for the size of the Datum.
		*   @return                 The number of elements of the Datum.
		*/
		std::uint32_t Size() const;

		/** @brief					Setter for the size of the Datum.
		*   @param size				New size of the Datum. (Note that if this value is greater than the current capacity, then both the size and capacity will grow. Otherwise, only the size will change.)
		*   @exception				An std::exception is thrown if the type for the Datum hasn't been set yet, or the data is external. (External data cannot be tampered with.)
		*/
		void SetSize(const std::uint32_t& size);

		/** @brief					Getter for the capacity of the Datum.
		*   @return                 The maximum number of elements the Datum can contain.
		*/
		std::uint32_t Capacity() const;

		/** @brief					Store external data into the Datum. As a result, any internal data is cleared, the size and capacity are modified to reflect the number of external elements, and no tampering of the data can occur.
		*   @param container        Array of elements of a specific type.
		*   @param size				Number of elements in container.
		*   @exception				An std::exception is thrown if the type of container is different than that of the Datum.
		*/
		void SetStorage(int* const container, const std::uint32_t& size);
		void SetStorage(float* const container, const std::uint32_t& size);
		void SetStorage(glm::vec4* const container, const std::uint32_t& size);
		void SetStorage(glm::mat4* const container, const std::uint32_t& size);
		void SetStorage(std::string* const container, const std::uint32_t& size);
		void SetStorage(RTTI** const container, const std::uint32_t& size);

		/** @brief					Scalar assignment operator. This will modify the first entry of the inner data, or automatically allocate space for a single entry if nothing has been allocated yet.
		*   @param data				Data of a specific type to store.
		*   @return					Reference to the modified Datum.
		*   @exception				An std::exception is thrown if the type of the data is different than that of the Datum, or the data is external.
		*/
		Datum& operator=(const int& data);
		Datum& operator=(const float& data);
		Datum& operator=(const glm::vec4& data);
		Datum& operator=(const glm::mat4& data);
		Datum& operator=(Scope& data);
		Datum& operator=(const std::string& data);
		Datum& operator=(RTTI* const& data);

		/** @brief					Datum equality operator. (To be considered equal, both Datums must be of the same type, have the same size and capacity, and have the same data.)
		*   @param otherDatum		Data of a specific type to store.
		*   @return					True if this and another Datum are equal, and false otherwise.
		*/
		bool operator==(const Datum& otherDatum) const;

		/** @brief					Datum inequality operator.
		*   @param otherDatum		Data of a specific type to store.
		*   @return					False if this and another Datum are equal, and true otherwise.
		*/
		bool operator!=(const Datum& otherDatum) const;

		/** @brief					Scalar equality operator. (To be equal, the passed-in value must be the same type as the Datum, and the first element of the Datum's data must be that value.)
		*   @param data				Value of a specific type to compare.
		*   @return					True if the scalars are equal, and false otherwise.
		*/
		bool operator==(const int& data) const;
		bool operator==(const float& data) const;
		bool operator==(const glm::vec4& data) const;
		bool operator==(const glm::mat4& data) const;
		bool operator==(Scope& data) const;
		bool operator==(const std::string& data) const;
		bool operator==(RTTI* const& data) const;

		/** @brief					Scalar inequality operator.
		*   @param data				Value of a specific type to compare.
		*   @return					False if the scalars are equal, and true otherwise.
		*/
		bool operator!=(const int& data) const;
		bool operator!=(const float& data) const;
		bool operator!=(const glm::vec4& data) const;
		bool operator!=(const glm::mat4& data) const;
		bool operator!=(Scope& data) const;
		bool operator!=(const std::string& data) const;
		bool operator!=(RTTI* const& data) const;

		/** @brief					Gets a data entry of a specific type at the desired index.
		*   @param index			Location of the data entry to grab.
		*   @return					The data at the desired index.
		*   @exception				An std::exception is thrown if the type of the data to grab is different than that of the Datum, or the index of out of bounds.
		*/
		template<typename T> T& Get(const std::uint32_t& index = 0) const;
		template<> int& Get<int>(const std::uint32_t& index) const;
		template<> float& Get<float>(const std::uint32_t& index) const;
		template<> glm::vec4& Get<glm::vec4>(const std::uint32_t& index) const;
		template<> glm::mat4& Get<glm::mat4>(const std::uint32_t& index) const;
		template<> Scope*& Get<Scope*>(const std::uint32_t& index) const;
		template<> std::string& Get<std::string>(const std::uint32_t& index) const;
		template<> RTTI*& Get<RTTI*>(const std::uint32_t& index) const;

		/** @brief					Subscript operators for Scopes (used for syntactic sugar.)
		*   @param index			Location of the data entry to grab.
		*   @return					The data at the desired index.
		*   @exception				An std::exception is thrown if the type of the data to grab is not of type Table, or the index of out of bounds.
		*/
		Scope& operator[](std::uint32_t index);
		const Scope& operator[](std::uint32_t index) const;

		/** @brief					Sets the data entry at the back of the data array. If this addition will exceed size or capacity, the array will auto-grow.
		*   @param value			Value to set the entry to.
		*   @return					An index to the newly-added value.
		*   @exception				An std::exception is thrown if the type of the value is different than that of the Datum, or the data is external.
		*/
		std::uint32_t PushBack(const int& value);
		std::uint32_t PushBack(const float& value);
		std::uint32_t PushBack(const glm::vec4& value);
		std::uint32_t PushBack(const glm::mat4& value);
		std::uint32_t PushBack(Scope& value);
		std::uint32_t PushBack(const std::string& value);
		std::uint32_t PushBack(RTTI* const& value);

		/** @brief					Sets a data entry of a specific type at the desired index.
		*   @param value			Value to set the entry to.
		*   @param index			Location of the data entry to modify. (Note that ifthis value is greater or equal to the size, the Datum's size [and possibly capacity] will grow.)
		*   @exception				An std::exception is thrown if the type of the value is different than that of the Datum, or the data is external.
		*/
		void Set(const int& value, const std::uint32_t& index = 0);
		void Set(const float& value, const std::uint32_t& index = 0);
		void Set(const glm::vec4& value, const std::uint32_t& index = 0);
		void Set(const glm::mat4& value, const std::uint32_t& index = 0);
		void Set(Scope& value, const std::uint32_t& index = 0);
		void Set(const std::string& value, const std::uint32_t& index = 0);
		void Set(RTTI* const& value, const std::uint32_t& index = 0);

		/** @brief					Removes a specified value from the data array.
		*   @param value			Value to remove from the data array.
		*   @exception				An std::exception is thrown if the type of the value is different than that of the Datum, or the data is external.
		*/
		void Remove(const int& value);
		void Remove(const float& value);
		void Remove(const glm::vec4& value);
		void Remove(const glm::mat4& value);
		void Remove(Scope& value);
		void Remove(const std::string& value);
		void Remove(RTTI* const& value);

		/** @brief					Gets an std::string representation of the data at the desired index.
		*   @param index			Location of the data entry to get a string of.
		*   @return					A string representation of the data at the desired index.
		*   @exception				An std::exception is thrown if the index is out of bounds.
		*/
		std::string ToString(const std::uint32_t& index = 0);

		/** @brief					Sets a data entry at the desired index based on a string representation of the new value.
		*   @param value			String representation of the value to use for modification. (Please note the valid formats of these variable types:
		*							-floats need not include an f at the end of the number. For instance, \"1.01\" is considered valid, while \"1.01f\" is not.
		*							-glm::vec4s need the x, y, z, and w components all in one line with spaces in between (i.e. \"x y z w\")
		*							-glm::mat4s need all four x, y z, and w components all in one line with spaces in between (i.e. \"x0 y0 z0 w0 x1 y1 z1 w1 x2 y2 z2 w2 x3 y3 z3 w3\")
		*							-RTTI pointers CANNOT be set from strings.
		*   @param type				Type of the value being used.
		*   @param index			Data of a specific type to store.
		*   @exception				An std::exception is thrown if the type of the data is different than that of the Datum, the data is external, the format of the string is incorrect, or an attempt to set a Pointer or Table type from string is made.
		*/
		void SetFromString(const std::string& value, DatumType type, const std::uint32_t& index = 0);

		/** @brief					Sets the Datum back to its default settings: empty, internal, and with an Unknown type. (Note that if the Datum has external data, no freeing of memory will occur.)
		*
		*/
		void Clear();
	private:
		/** @brief					Union of data containers for each type a Datum can assume.
		*
		*/
		typedef union DatumValues
		{
			/** @brief					Container for int values.
			*
			*/
			int* mIntArray;

			/** @brief					Container for float values.
			*
			*/
			float* mFloatArray;

			/** @brief					Container for glm::vec4 values.
			*
			*/
			glm::vec4* mVectorArray;

			/** @brief					Container for glm::mat4 values.
			*
			*/
			glm::mat4* mMatrixArray;

			/** @brief					Container for std::string values.
			*
			*/
			std::string* mStringArray;

			/** @brief					Container for Scope* values.
			*
			*/
			Scope** mTableArray;

			/** @brief					Container for RTTI* values.
			*
			*/
			RTTI** mPointerArray;

			/** @brief					Constructor for DatumValues union.
			*   @return					Instance of the DataValues union.
			*/
			DatumValues() : mIntArray(nullptr)
			{
			}

			void Clear()
			{
				mIntArray = nullptr;
			}
		} DatumValues;

		/** @brief					Helper template functions for SetSize. (See description of SetSize for more info.)
		*
		*/
		template<typename T> static void Resize(Datum& datum, const std::uint32_t& size);
		template<> static void Resize<int>(Datum& datum, const std::uint32_t& size);
		template<> static void Resize<float>(Datum& datum, const std::uint32_t& size);
		template<> static void Resize<glm::vec4>(Datum& datum, const std::uint32_t& size);
		template<> static void Resize<glm::mat4>(Datum& datum, const std::uint32_t& size);
		template<> static void Resize<Scope*>(Datum& datum, const std::uint32_t& size);
		template<> static void Resize<std::string>(Datum& datum, const std::uint32_t& size);
		template<> static void Resize<RTTI*>(Datum& datum, const std::uint32_t& size);

		/** @brief					Data container.
		*
		*/
		DatumValues mData;

		/** @brief					Type of the Datum.
		*
		*/
		DatumType mType;

		/** @brief					Dictates whether or not the Datum has external data.
		*
		*/
		bool mIsExternal;

		/** @brief					Maximum number of elements the Datum can contain.
		*
		*/
		std::uint32_t mCapacity;

		/** @brief					Number of elements the Datum has.
		*
		*/
		std::uint32_t mSize;

		typedef std::function<void(Datum&, const std::string&, const std::uint32_t&)> SetFromStringFunc;
		typedef std::function<std::string(Datum&, const std::uint32_t&)> ToStringFunc;
		typedef void(*SetSizeFunc)(Datum&, const std::uint32_t&);
		typedef std::function<void(Datum&, const Datum&)> DeepCopyFunc;
		typedef std::function<bool(const Datum&, const Datum&)> EqualityFunc;

		/** @brief					Function table for the different variations of ToString.
		*
		*/
		static const ToStringFunc toStringFuncTable[static_cast<std::uint32_t>(DatumType::MAX)];

		/** @brief					Function table for the different variations of SetFromString.
		*
		*/
		static const SetFromStringFunc setFromStringFuncTable[static_cast<std::uint32_t>(DatumType::MAX)];

		/** @brief					Function table for the different variations of SetSize.
		*
		*/
		static const SetSizeFunc setSizeFuncTable[static_cast<std::uint32_t>(DatumType::MAX)];

		/** @brief					Function table for the different variations of deep copies.
		*
		*/
		static const DeepCopyFunc deepCopyFuncTable[static_cast<std::uint32_t>(DatumType::MAX)];

		/** @brief					Function table for the different variations of operator==.
		*
		*/
		static const EqualityFunc equalityFuncTable[static_cast<std::uint32_t>(DatumType::MAX)];
	};
}

