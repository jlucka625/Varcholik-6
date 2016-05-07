#pragma once

#include "XmlParseMaster.h"
#include "Scope.h"

namespace LibraryDesktop
{
	class TableSharedData : public XmlParseMaster::SharedData
	{
		RTTI_DECLARATIONS(TableSharedData, SharedData)

		friend class XmlParseMaster;
		friend class TableXmlParseHelper;
		friend class WorldXmlParseHelper;
		friend class SectorXmlParseHelper;
		friend class EntityXmlParseHelper;
		friend class ActionXmlParseHelper;
		friend class ReactionXmlParseHelper;
	public:
		/** @brief					Default constructor for the TableSharedData derived class.
		*   @return                 Instance of the TableSharedData derived class.
		*/
		TableSharedData();

		TableSharedData(const TableSharedData& otherSharedData) = delete;

		TableSharedData& operator=(const TableSharedData& otherSharedData) = delete;

		/** @brief					Destructor for the TableSharedData base class.
		*
		*/
		virtual ~TableSharedData();

		/** @brief					Creates a new instance of this shared data, which is ready for a fresh new file to parse. (Note that this returns a heap-allocated value,
		*							so it must be deleted at some point before the program's termination. Also, if the XML file to be parsed contains World elements, you must call
		*							GetCurrentScope and delete its return value to )
		*   @return                 Heap-allocated instance of the TableSharedData class.
		*/
		virtual SharedData* Clone() const override;

		/** @brief					Creates a new instance of this shared data, which is ready for a fresh new file to parse. (Note that this returns a heap-allocated value,
		*							so it must be deleted at some point before the program's termination.)
		*   @return                 Heap-allocated instance of the TableSharedData class.
		*/
		Scope& GetCurrentScope() const;

		/** @brief					Clears the Scope member for parsing new files (to be called explicitly by the user.)
		*							
		*/
		void Clear();
	private:
		/** @brief					Default destructor for the TableSharedData base class.
		*
		*/
		Scope* mScope;
	};
}

