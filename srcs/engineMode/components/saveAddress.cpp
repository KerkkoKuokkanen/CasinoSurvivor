
#include "saveAddress.h"

void SaveAddress::Init(void *data, size_t size)
{
	bzero(fileBuff, SAVE_BUFF_SIZE);
	std::string used((char*)data, size);
	SetFile(used);
	CreateInputField("Save Address", n_VarType::TEXT, fileBuff);
	CreateInputField("Add", n_VarType::BOOL, &add);
}

void SaveAddress::SetFile(std::string file)
{
	SaveAddress::file = file;
	void *data = (void*)SaveAddress::file.data();
	size_t size = SaveAddress::file.size();
	ClearSaveData();
	AddToSave(data, size);
}

void SaveAddress::EngineUpdate()
{
	if (add)
	{
		add = false;
		std::string neew = fileBuff;
		SetFile(neew);
		bzero(fileBuff, SAVE_BUFF_SIZE);
	}
}
