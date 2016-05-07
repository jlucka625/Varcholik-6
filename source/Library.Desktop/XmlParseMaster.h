#pragma once

#include <cstdint>

#include "expat.h"
#include "RTTI.h"
#include "Vector.h"

namespace LibraryDesktop
{
	class IXmlParseHelper;
	class Scope;

	class XmlParseMaster final
	{
	public:
		class SharedData : public RTTI
		{
			RTTI_DECLARATIONS(SharedData, RTTI)

			friend class XmlParseMaster;
		public:
			/** @brief					Default constructor for the SharedData base class. (However, since this is an abstract base class, it can only be called on the
			*							initialization lines of derived class constructors.)
			*   @return                 Instance of the SharedData base class.
			*/
			SharedData();

			SharedData(const SharedData& otherSharedData) = delete;

			SharedData& operator=(const SharedData& otherSharedData) = delete;

			/** @brief					Default destructor for the SharedData base class.
			*
			*/
			virtual ~SharedData() = default;

			/** @brief					Creates a new instance of this parse master, which is ready for a fresh new file to parse. (Note that this returns a heap-allocated value, so it must be deleted at some point before the program's termination.)
			*   @return                 Heap-allocated instance of the XmlParseMaster class.
			*/
			virtual SharedData* Clone() const = 0;

			/** @brief					Gets the address of the parse master associated with this SharedData instance.
			*   @return                 Address of the respective parse master.
			*/
			XmlParseMaster* GetXmlParseMaster();

			/** @brief					Increments the current depth counter. (Done when a callback to a StartElementHandler function is made.)
			*
			*/
			void IncrementDepth();

			/** @brief					Decrements the current depth counter. (Done when a callback to an EndElementHandler function is made.)
			*
			*/
			void DecrementDepth();

			/** @brief					Gets the current depth of the shared data.
			*   @return                 The current value of the depth counter.
			*/
			std::uint32_t Depth() const;
		protected:
			/** @brief					Parse master associated with this SharedData instance.
			*
			*/
			XmlParseMaster* mParseMaster;

			/** @brief					Depth counter.
			*
			*/
			std::uint32_t mDepth;
		private:
			/** @brief					Gets the address of the parse master associated with this SharedData instance.
			*	@param parseMaster		Reference to an XmlParseMaster instance.
			*/
			void SetXmlParseMaster(XmlParseMaster& parseMaster);
		};

		/** @brief					Constructor for the XmlParseMaster class.
		*	@param sharedData		Reference to a SharedData instance.
		*   @return                 Instance of the XmlParseMaster class.
		*/
		explicit XmlParseMaster(SharedData& sharedData);

		XmlParseMaster(const XmlParseMaster& otherSharedData) = delete;

		XmlParseMaster& operator=(const XmlParseMaster& otherSharedData) = delete;

		/** @brief					Creates a new instance of this parse master, which is ready for a fresh new file to parse. (Note that this returns a heap-allocated value, so it must be deleted at some point before the program's termination.)
		*   @return                 Heap-allocated instance of the XmlParseMaster class.
		*/
		XmlParseMaster* Clone() const;

		/** @brief					Destructor for the XmlParseMaster class.
		*
		*/
		~XmlParseMaster();

		/** @brief					Adds a helper to the internal list of helpers.
		*	@param sharedData		Parsing helper to add.
		*   @exception              An std::exception is thrown if this XmlParseMaster instance is a clone.
		*/
		void AddHelper(IXmlParseHelper& helper);

		/** @brief					Removes a helper from the internal list of helpers.
		*	@param helper			Parsing helper to remove.
		*   @exception              An std::exception is thrown if this XmlParseMaster instance is a clone.
		*/
		void RemoveHelper(IXmlParseHelper& helper);

		/** @brief					Parses a section of XML data. Before calling this, you must make a call to Reset in order to flush old data out and prepare the parse master for a fresh file.
		*	@param chunkData		Section of the file to parse.
		*	@param chunkLength		Length of the file chunk.
		*	@param isFinalChunk		Flag that dictates whether or not this is the last file chunk to parse.
		*/
		void Parse(const char* fileData, int fileLength, bool isFirstChunk = true, bool isFinalChunk = false);

		/** @brief					Removes a helper from the internal list of helpers.
		*	@param filename			Name of the file to parse.
		*/
		void ParseFromFile(const std::string& filename);

		/** @brief					Gets the number of helpers within this parse master's list of helpers.
		*   @return                 The size of the parse master's list of helpers.
		*/
		std::uint32_t GetHelperCount() const;

		/** @brief					Gets the filename of the file currently being used for parsing.
		*   @return                 The filename of the file currently being used for parsing.
		*/
		std::string GetFileName() const;

		/** @brief					Gets the address of this parse master's shared data.
		*   @return                 The address of this parse master's shared data.
		*/
		SharedData* GetSharedData() const;

		/** @brief					Sets the shared data this XmlParseMaster instance will use.
		*	@param sharedData		Reference to a SharedData instance.
		*   @exception              An std::exception is thrown if this XmlParseMaster instance is a clone.
		*/
		void SetSharedData(SharedData& sharedData);
	private:
		/** @brief					Static callback Expat uses when it comes across a starting XML tag.
		*	@param userData			Shared data the parse master provides to its helpers.
		*	@param name				Name of the starting XML tag.
		*	@param attributes		List of strings representing attributes within the start tag. (This is null if there are no attributes.)
		*/
		static void StartElementHandler(void* userData, const XML_Char* name, const XML_Char** attributes);

		/** @brief					Static callback Expat uses when it comes across an ending XML tag.
		*	@param userData			Shared data the parse master provides to its helpers.
		*	@param name				Name of the ending XML tag.
		*/
		static void EndElementHandler(void* userData, const XML_Char* name);

		/** @brief					Static callback Expat uses when it comes across data within XML tags.
		*	@param userData			Shared data the parse master provides to its helpers.
		*	@param name				Name of the data.
		*	@param length			Length of the data.
		*/
		static void CharDataHandler(void* userData, const XML_Char* name, int length);

		/** @brief					Resets the Expat member and shared data for more parsing.
		*
		*/
		void Reset();

		/** @brief					Helper used to reset the callbacks and user data Expat uses. (Whenever the Expat member is instantiated or reset, this information is nulled out, so it must be reset to work properly.)
		*
		*/
		void SetXMLInfo();
		
		/** @brief					List of parsing helpers this parse master utilizes
		*
		*/
		Vector<IXmlParseHelper*> mHelpers;

		/** @brief					Flag that dictates whether or not this parse master is a clone of another.
		*
		*/
		bool mIsClone;

		/** @brief					Name of the file this parse master is parsing.
		*
		*/
		std::string mFilename;

		/** @brief					Address to data shared by the parse master and its list of helpers.
		*
		*/
		SharedData* mSharedData;

		/** @brief					Used to bridge the relationship between the Expat library and the XmlParseMaster class.
		*
		*/
		XML_Parser mParser;
	};
}
