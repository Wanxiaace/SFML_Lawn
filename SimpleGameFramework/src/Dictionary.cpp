#include "../include/Dictionary.h"
#include <fstream>

sgf::Dictionary::Dictionary()
{
}

sgf::Dictionary::~Dictionary()
{
}

void sgf::Dictionary::LoadFromFile(const char* path)
{
	std::ifstream json;
	int jsonSize;
	json.open(path, std::ios::binary);
	json.seekg(0,std::ios::end);
	jsonSize = json.tellg();
	json.seekg(0, std::ios::beg);

	char* jsonData = new char[jsonSize+1];
	memset(jsonData,0, jsonSize + 1);
	json.read(jsonData, jsonSize);

	
	json.close();

	Parse(jsonData);

	delete[] jsonData;
}

sgf::String sgf::Dictionary::Dump() const
{
	return mJson.dump();
}

void sgf::Dictionary::Parse(const sgf::String& src)
{
	mJson = nlohmann::json::parse(src);
}

sgf::String sgf::Dictionary::GetString(const sgf::String& key)
{
	return mJson.value<sgf::String>(key, "<Missing \"" + key + "\" >");
}


sgf::String sgf::Dictionary::operator[](const sgf::String& key)
{
	return mJson[key];
}
