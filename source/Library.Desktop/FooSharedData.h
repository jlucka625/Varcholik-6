#pragma once

#include "XmlParseMaster.h"

namespace LibraryDesktop
{
	class FooSharedData : public XmlParseMaster::SharedData
	{
		RTTI_DECLARATIONS(FooSharedData, SharedData)
	public:
		/** @brief					Default constructor for the FooSharedData derived class.
		*   @return                 Instance of the FooSharedData derived class.
		*/
		FooSharedData();

		FooSharedData(const FooSharedData& otherSharedData) = delete;

		FooSharedData& operator=(const FooSharedData& otherSharedData) = delete;

		/** @brief					Default destructor for the FooSharedData base class.
		*
		*/
		virtual ~FooSharedData() = default;

		/** @brief					Creates a new instance of this shared data, which is ready for a fresh new file to parse. (Note that this returns a heap-allocated value, 
		*							so it must be deleted at some point before the program's termination.)
		*   @return                 Heap-allocated instance of the FooSharedData class.
		*/
		virtual SharedData* Clone() const override;
	};
}

