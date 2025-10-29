
#include "componentRegistry.h"
#include "image.h"
#include "structure.h"
#include "sysEnv.h"

size_t SystemObj::FetchComponentDataSize()
{
	if (componentSaveFetchIndex >= components.size())
		return (0);
	int i = componentSaveFetchIndex;
	switch (components[i].classType)
	{
		case n_ComponentTypes::IMAGE_CLASS:
		{
			Image *img = (Image*)components[i].obj;
			return (img->GetSaveDataSize());
		}
		case n_ComponentTypes::STRUCTURE_CLASS:
		{
			Structure *structure = (Structure*)components[i].obj;
			return (structure->GetSaveDataSize());
		}
		default:
		{
			CustomComponent *cust = (CustomComponent*)components[i].obj;
			return (cust->GetComponentSize());
		}
	}
	return (0);
}

void *SystemObj::FetchComponentSaveData(void *buffer, size_t bufferSize, size_t &compSize)
{
	if (componentSaveFetchIndex >= components.size())
		return (NULL);
	int i = componentSaveFetchIndex;
	switch (components[i].classType)
	{
		case n_ComponentTypes::IMAGE_CLASS:
		{
			Image *img = (Image*)components[i].obj;
			void *ret = img->CollectSaveData(buffer, bufferSize, compSize);
			if (ret == NULL)
				return (NULL);
			return (buffer);
		}
		case n_ComponentTypes::STRUCTURE_CLASS:
		{
			Structure *structure = (Structure*)components[i].obj;
			void *ret = structure->CollectSaveData(buffer, bufferSize, compSize);
			if (ret == NULL)
				return (NULL);
			return (buffer);
		}
		default:
		{
			CustomComponent *cust = (CustomComponent*)components[i].obj;
			void *ret = cust->CollectSaveData(compSize);
			if (compSize == 0)
			{
				int8_t use = -128;
				void *saveData = malloc(sizeof(int8_t));
				char *cast = (char*)saveData;
				compSize = sizeof(int8_t);
				memcpy(cast, &use, sizeof(int8_t));
				return (saveData);
			}
			return (ret);
		}
	}
	return (NULL);
}

void SystemObj::LastUpdateSystemObj()
{
	for (int i = 0; i < components.size(); i++)
	{
		switch (components[i].classType)
		{
			case n_ComponentTypes::IMAGE_CLASS:
				break ;
			case n_ComponentTypes::STRUCTURE_CLASS:
				break ;
			default :
			{
				CustomComponent *cust = (CustomComponent*)components[i].obj;
				if (EngineModeOn())
					cust->EngineUpdate();
				else
					cust->LastUpdate();
				break ;
			}
		}
	}
}

void SystemObj::UpdateSystemObj()
{
	if (EngineModeOn())
		return ;
	for (int i = 0; i < components.size(); i++)
	{
		switch (components[i].classType)
		{
			case n_ComponentTypes::IMAGE_CLASS:
				break ;
			case n_ComponentTypes::STRUCTURE_CLASS:
				break ;
			default :
			{
				CustomComponent *cust = (CustomComponent*)components[i].obj;
				if (components[i].started == false)
				{
					cust->Start();
					components[i].started = true;
				}
				cust->Update();
				break ;
			}
		}
	}
}

void SystemObj::GiveComponentId(void *component, uint32_t classType, uint32_t id)
{
	if (classType == n_ComponentTypes::IMAGE_CLASS)
	{
		Image *obj = (Image*)component;
		obj->self = this;
		obj->id = id;
		return ;
	}
	else if (classType == n_ComponentTypes::STRUCTURE_CLASS)
	{
		Structure *obj = (Structure*)component;
		obj->self = this;
		obj->id = id;
		return ;
	}
	CustomComponent *obj = (CustomComponent*)component;
	obj->self = this;
	obj->ownId = id;
}

void SystemObj::DeleteComponentOwn(void *component, uint32_t classType)
{
	switch (classType)
	{
		case n_ComponentTypes::IMAGE_CLASS:
		{
			Image *img = (Image*)component;
			delete img;
			break ;
		}
		case n_ComponentTypes::STRUCTURE_CLASS:
		{
			Structure *structure = (Structure*)component;
			delete structure;
			break ;
		}
		default :
		{
			CustomComponent *cust = (CustomComponent*)component;
			cust->Destroy();
			delete cust;
			break ;
		}
	}
}

/* SystemObj is not aware if components get
deleted outside this function */
SystemObj::~SystemObj()
{
	deleting = true;
	for (int i = 0; i < components.size(); i++)
		DeleteComponentOwn(components[i].obj, components[i].classType);
	if (controller == NULL)
		return ;
	SysEnv *env = (SysEnv*)controller;
	env->RemoveObject(this);
}
