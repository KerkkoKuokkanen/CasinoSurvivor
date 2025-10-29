
#ifndef OBJ_BAR_H
# define OBJ_BAR_H

# include "customComponent.h"
# include "componentRegistry.h"
# include "objSelector.h"
# include "objEditor.h"
# include "image.h"
# include "mainBar.h"

class ObjBar : public CustomComponent
{
	private:
		ObjectSelector *objSelect = NULL;
		ObjectEditor *objEditor = NULL;
		MainBar *mainBar = NULL;

		void InitObjSelector();
	public:
		ObjBar();
		~ObjBar() override;
		void SaveRoomChange(uint64_t key, uint16_t room) {if (objSelect != NULL) {objSelect->SaveRoomChange(key, room);}};
		void InitSecondaryHierarchy(void *hier);
		bool HoveredOverWindow();
		void EngineUpdate() override;
};

REGISTER_COMPONENT(ObjBar);

#endif
