
#ifndef SAVING_INTERFACE_H
# define SAVING_INTERFACE_H

# include "sysSaver.h"
# include "snapshot.h"

void SaveSnapShot(SnapShot snapShot, std::string file);
uint32_t SetAskedData(std::string file);
bool CollectAskedState(uint32_t key, void **collector);
void SaveThread();
bool GetSaving();

#endif
