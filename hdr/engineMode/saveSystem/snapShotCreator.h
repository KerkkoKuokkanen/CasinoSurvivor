
#ifndef SNAPSHOT_CREATOR
# define SNAPSHOT_CREATOR

# include <vector>

# define SNAPSHOT_AMOUNT 333

typedef struct s_SnapSaves
{
	uint8_t has;
	std::vector<std::tuple<SnapShot, uint16_t>> snap;
	uint16_t callingRoom;
}				t_SnapSaves;

void UpdateSnapCreator();
void SetCreatingSnap(bool set);
bool GetCreatingSnap();
bool GiveSnapData(std::unordered_map<uint64_t, SaveObj> *data, uint16_t callingRoom);
void SetSaveData(bool set);
bool GetSaveData();
t_SnapSaves CurrentState();
void ControlZ(bool *cz);

#endif
