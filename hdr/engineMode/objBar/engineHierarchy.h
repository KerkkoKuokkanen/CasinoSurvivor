
#ifndef ENGINE_HIERARCHY_H
# define ENGINE_HIERARCHY_H

# include "customComponent.h"
# include "componentRegistry.h"

struct NodeData {
	char name[48];
	bool is_folder;
	int parent_id;
	uint64_t objKey;
};

class EngineHierarchy : public CustomComponent
{
	public:
		std::vector<NodeData> currentData = {};
		void Init(void *data, size_t size) override;
		void SaveHierarchy(std::vector<NodeData> &data);
};

REGISTER_COMPONENT(EngineHierarchy);

#endif
