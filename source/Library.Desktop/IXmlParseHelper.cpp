#include "pch.h"

using namespace LibraryDesktop;

bool IXmlParseHelper::CharDataHandler(XmlParseMaster::SharedData& sharedData, const std::string& name)
{
	UNREFERENCED_PARAMETER(sharedData); 
	UNREFERENCED_PARAMETER(name); 
	
	return false;
}

const std::string& IXmlParseHelper::GetRequiredAttribute(const std::string& attribute, const HashMap<std::string, std::string>& attributes)
{
	HashMap<std::string, std::string>::Iterator found = attributes.Find(attribute);

	if (found == attributes.end())
	{
		std::string message = ("Error: required attribute \"" + attribute + "\" is missing");
		throw std::exception(message.c_str());
	}

	return found->second;
}

const std::string& IXmlParseHelper::GetOptionalAttribute(const std::string & attribute, const HashMap<std::string, std::string>& attributes, const std::string & alternative)
{
	HashMap<std::string, std::string>::Iterator found = attributes.Find(attribute);

	if (found == attributes.end())
	{
		return alternative;
	}
	else
	{
		return found->second;
	}
}
