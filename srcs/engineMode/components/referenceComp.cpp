
#include "referenceComp.h"

Reference::Reference()
{
	CreateInputField("Name", n_VarType::TEXT, inputName);
	CreateInputField("Key", n_VarType::TEXT, inputKey);
	CreateInputField("Add Referene", n_VarType::BOOL, &added);
}

void Reference::Init(void *data, size_t size)
{
	if (size < sizeof(char) * 64)
		return ;
	size_t iterator = 0;
	char *cast = (char*)data;
	char name[32] = {0};
	char key[32] = {0};
	while (iterator < size)
	{
		memcpy(name, cast + iterator, sizeof(char) * 32);
		iterator += sizeof(char) * 32;
		memcpy(key, cast + iterator, sizeof(char) * 32);
		iterator += sizeof(char) * 32;
		std::string n = name;
		std::string k = key;
		references[n] = std::stoull(k);
	}
}

void Reference::EngineUpdate()
{
	if (added == false)
		return ;
	std::string name = inputName;
	std::string key = inputKey;
	if (key.length() <= 0)
		return ;
	if (name.length() <= 0)
		return ;
	AddToSave(inputName, sizeof(char) * 32);
	AddToSave(inputKey, sizeof(char) * 32);
	references[name] = std::stoull(key);
	bzero(inputName, sizeof(char) * 32);
	bzero(inputKey, sizeof(char) * 32);
}
