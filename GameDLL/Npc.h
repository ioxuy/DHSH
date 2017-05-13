#ifndef __DHSH_GAMEDLL_GAME_OBJECT_NPC_NPC_H__
#define __DHSH_GAMEDLL_GAME_OBJECT_NPC_NPC_H__

#include "Player.h"

class CNpc : public CPlayer
{
public:
	CNpc();
	CNpc(_In_ DWORD dwNodeBase);
	~CNpc() = default;



private:

};



#endif // !__DHSH_GAMEDLL_GAME_OBJECT_NPC_NPC_H__
