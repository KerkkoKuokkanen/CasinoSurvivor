
#ifndef SAVE_ADDRESS_H
# define SAVE_ADDRESS_H

# include "customComponent.h"
# include "componentRegistry.h"

# define SAVE_BUFF_SIZE 128

class SaveAddress : public CustomComponent
{
	private:
		bool add = false;
		char fileBuff[SAVE_BUFF_SIZE];
		std::string file = "";
	public:
		void Init(void *data, size_t size) override;
		void SetFile(std::string file);
		std::string GetFile() {return(file);};
		void EngineUpdate() override;
};

REGISTER_COMPONENT(SaveAddress);

#endif
