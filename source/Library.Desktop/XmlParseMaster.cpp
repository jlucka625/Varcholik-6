#include "pch.h"

#include <fstream>
#include <sstream>

using namespace LibraryDesktop;

RTTI_DEFINITIONS(XmlParseMaster::SharedData)

//SharedData definitions
XmlParseMaster::SharedData::SharedData() : mDepth(0), mParseMaster(nullptr)
{
}

XmlParseMaster* XmlParseMaster::SharedData::GetXmlParseMaster()
{
	return mParseMaster;
}

void XmlParseMaster::SharedData::SetXmlParseMaster(XmlParseMaster& parseMaster)
{
	mParseMaster = &parseMaster;
}

void XmlParseMaster::SharedData::IncrementDepth()
{
	++mDepth;
}

void XmlParseMaster::SharedData::DecrementDepth()
{
	if (mDepth > 0)
	{
		--mDepth;
	}
}

std::uint32_t XmlParseMaster::SharedData::Depth() const
{
	return mDepth;
}

//XmlParseMaster definitions
XmlParseMaster::XmlParseMaster(SharedData& sharedData) : mIsClone(false), mFilename(), mHelpers(), mSharedData(nullptr), mParser(nullptr)
{
	mParser = XML_ParserCreate("UTF-8");
	SetSharedData(sharedData);
	SetXMLInfo();
}

XmlParseMaster* XmlParseMaster::Clone() const
{
	XmlParseMaster* parseMasterClone = new XmlParseMaster(*mSharedData->Clone());

	for (const auto& helper : mHelpers)
	{
		parseMasterClone->AddHelper(*helper->Clone());
	}

	parseMasterClone->mIsClone = true;
	return parseMasterClone;
}

XmlParseMaster::~XmlParseMaster()
{
	XML_ParserFree(mParser);

	if (mIsClone)
	{
		delete mSharedData;

		for (std::uint32_t i = 0; i < mHelpers.Size(); i++)
		{
			delete mHelpers[i];
		}
	}
}

void XmlParseMaster::AddHelper(IXmlParseHelper& helper)
{
	if (mIsClone)
	{
		throw std::exception("Error: cannot modify the helpers list of a cloned parse master");
	}

	if (mHelpers.Find(&helper) != mHelpers.end())
	{
		throw std::exception("Error: this helper has already been added");
	}

	mHelpers.PushBack(&helper);
}

void XmlParseMaster::RemoveHelper(IXmlParseHelper& helper)
{
	if (mIsClone)
	{
		throw std::exception("Error: cannot modify the helpers list of a cloned parse master");
	}

	mHelpers.Remove(&helper);
}

void XmlParseMaster::Parse(const char* fileData, int fileLength, bool isFirstChunk, bool isLastChunk)
{
	if (isFirstChunk)
	{
		Reset();

		for (const auto& helper : mHelpers)
		{
			helper->Initialize();
		}
	}

	if (XML_Parse(mParser, fileData, fileLength, isLastChunk) != XML_STATUS_OK)
	{
		std::stringstream errorMessage;
		errorMessage << "Error parsing the XML file (" << mFilename << "): " << XML_ErrorString(XML_GetErrorCode(mParser));
		throw std::exception(errorMessage.str().c_str());
	}
}

void XmlParseMaster::ParseFromFile(const std::string& filename)
{
	mFilename = filename;

	std::ifstream inputFile(mFilename);

	if (!inputFile.is_open())
	{
		throw std::exception("Error: could not open the file");
	}

	std::string contents((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
	Parse(contents.c_str(), static_cast<int>(contents.length()), true, true);
	inputFile.close();
}

std::uint32_t XmlParseMaster::GetHelperCount() const
{
	return mHelpers.Size();
}

void XmlParseMaster::Reset()
{
	XML_ParserReset(mParser, "UTF-8");
	SetXMLInfo();
}

void XmlParseMaster::SetXMLInfo()
{
	XML_SetStartElementHandler(mParser, &XmlParseMaster::StartElementHandler);
	XML_SetEndElementHandler(mParser, &XmlParseMaster::EndElementHandler);
	XML_SetCharacterDataHandler(mParser, &XmlParseMaster::CharDataHandler);
	XML_SetUserData(mParser, mSharedData);
}

std::string XmlParseMaster::GetFileName() const
{
	return mFilename;
}

XmlParseMaster::SharedData* XmlParseMaster::GetSharedData() const
{
	return mSharedData;
}

void XmlParseMaster::SetSharedData(SharedData& sharedData)
{
	if (mIsClone)
	{
		throw std::exception("Error: cannot modify the helpers list of a cloned parse master");
	}

	mSharedData = &sharedData;

	if (mSharedData->GetXmlParseMaster() == nullptr)
	{
		mSharedData->SetXmlParseMaster(*this);
	}
}

void XmlParseMaster::StartElementHandler(void* userData, const XML_Char* name, const XML_Char** attributes)
{
	SharedData* sharedData = reinterpret_cast<SharedData*>(userData);

	if (sharedData)
	{
		XmlParseMaster* parseMaster = sharedData->GetXmlParseMaster();
		sharedData->IncrementDepth();

		HashMap<std::string, std::string> attributesMap;
		int numAttributes = XML_GetSpecifiedAttributeCount(parseMaster->mParser);

		for (int i = 0; i < numAttributes; i += 2)
		{
			attributesMap[std::string(attributes[i])] = std::string(attributes[i + 1]);
		}

		for (const auto& helper : parseMaster->mHelpers)
		{
			if (helper->StartElementHandler(*sharedData, name, attributesMap))
			{
				break;
			}
		}
	}
}

void XmlParseMaster::EndElementHandler(void* userData, const XML_Char* name)
{
	SharedData* sharedData = reinterpret_cast<SharedData*>(userData);

	if (sharedData)
	{
		sharedData->DecrementDepth();
		std::string elementName(name);

		for (const auto& helper : sharedData->GetXmlParseMaster()->mHelpers)
		{
			if (helper->EndElementHandler(*sharedData, elementName))
			{
				break;
			}
		}
	}
}

void XmlParseMaster::CharDataHandler(void* userData, const XML_Char* name, int length)
{
	SharedData* sharedData = reinterpret_cast<SharedData*>(userData);

	if (sharedData)
	{
		std::string data(name);
		data = data.substr(0, length);

		for (const auto& helper : sharedData->GetXmlParseMaster()->mHelpers)
		{
			if (helper->CharDataHandler(*sharedData, data))
			{
				break;
			}
		}
	}
}