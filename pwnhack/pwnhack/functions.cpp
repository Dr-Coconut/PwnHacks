#include "pch.h"
#include "libGameLogic.h"
#include <string>
#include <sstream>

DWORD procId = GetProcId(L"PwnAdventure3-Win32-Shipping.exe");
uintptr_t procBase = (uintptr_t)GetModuleHandle(L"PwnAdventure3-Win32-Shipping.exe");
uintptr_t moduleBase = GetModuleBaseAddress(procId, L"GameLogic.dll");

DWORD speedJmpBackAddr;
float walkSpeed; // Our custom walk speed
void __declspec(naked) speedHook()
{
	__asm {
		fld walkSpeed
		jmp[speedJmpBackAddr] // Jump back to original code
	}
}

DWORD akSpreadJmpBackAddr;
float spread;
void __declspec(naked) akSpread()
{
	__asm {
		fld spread
		jmp[akSpreadJmpBackAddr] // Jump back to original code
	}
}

DWORD akCooldownJmpBackAddr;
float cooldown;
void __declspec(naked) akCooldown()
{
	__asm {
		fld cooldown
		jmp[akCooldownJmpBackAddr] // Jump back to original code
	}
}

DWORD fCooldownJmpBackAddr;
float fcooldown;
void __declspec(naked) itemCooldown()
{
	__asm {
		movss xmm0, fcooldown
		jmp[fCooldownJmpBackAddr] // Jump back to original code
	}
}

void on_player_chat(char* msg, char* player);
DWORD chatJmpBackAddr;
void __declspec(naked) player_chat_injected() {
	__asm {
		push ecx
		mov eax, [esp + 0x8]
		push eax
		call on_player_chat
		pop eax
		pop ecx
		push ebp
		mov ebp, esp
		and esp, 0x0fffffff8
		push 0xffffffff
		jmp[chatJmpBackAddr]
	}
}


//values from cheat engine
void teleport(uintptr_t procBase, float x, float y, float z)
{
	*(float*)findAddr(procBase + 0x018FFDE4, { 0x4, 0x4, 0x1D4, 0x408, 0x24C, 0x180, 0x90 }) = x;
	*(float*)findAddr(procBase + 0x018FFDE4, { 0x4, 0x4, 0x1D4, 0x408, 0x24C, 0x180, 0x94 }) = y;
	*(float*)findAddr(procBase + 0x018FFDE4, { 0x4, 0x4, 0x1D4, 0x408, 0x24C, 0x180, 0x98 }) = z;
}

void rteleport(uintptr_t procBase, float x, float y, float z)
{
	*(float*)findAddr(procBase + 0x018FFDE4, { 0x4, 0x4, 0x1D4, 0x408, 0x24C, 0x180, 0x90 }) += x;
	*(float*)findAddr(procBase + 0x018FFDE4, { 0x4, 0x4, 0x1D4, 0x408, 0x24C, 0x180, 0x94 }) += y;
	*(float*)findAddr(procBase + 0x018FFDE4, { 0x4, 0x4, 0x1D4, 0x408, 0x24C, 0x180, 0x98 }) += z;
}

void addCoins(int amount) {
	*(int*)findAddr(moduleBase + 0x97D7C, { 0x1C, 0x6C, 0x4C, 0x0, 0X18 }) += amount;
	// adding offsets to the base game dll address to find the address of the coins variable and adding the amount to it
}

bool make_execute_readwrite(BYTE* base, unsigned int size) {
	DWORD oldProtect;
	return VirtualProtect(base, size, PAGE_EXECUTE_READWRITE, &oldProtect) != 0;
}

void tp(const char* name) {
	/*if (!make_execute_readwrite((BYTE*)procBase, 117000)) {
		std::cout << "Fail" << std::endl;
	}
	if (!make_execute_readwrite((BYTE*)moduleBase, 60000)) {
		std::cout << "Fail" << std::endl;
	}*/
	//	uintptr_t fn_tp = moduleBase + 0x54e50;
	//	//char* player = (char*)myPlayer;	
	//	Player* player = *(Player**)findAddr(moduleBase + 0x97D7C, { 0x1c,0x6c,0x0 });
	//	char* gameworld = *(char**)(moduleBase + 0x97d7c);
	//	char* testp = *(char**)myPlayer;
	//	std::cout << "testp" << std::hex << testp << std::endl;
	//	std::cout << "gameworld" << std::hex << gameworld << std::endl;
	//	std::cout << "player" << std::hex << player << std::endl;
	//	std::cout << "myPlayer" << std::hex << myPlayer << std::endl;
	//
	//	__asm {
	//		/*mov eax, name
	//		push eax
	//		mov ecx, player
	//		mov eax, fn_tp
	//		call eax*/
	//		mov ecx, player
	//		mov edx, name
	//		mov eax, fn_tp
	//		call eax
	//	}
	//}
	typedef void(__thiscall* _Tp)(void* player, const char* location);
	_Tp Tp;

	void* player = (void*)findAddr(moduleBase + 0x97D7C, { 0x1c,0x6c,0x0 });
	Tp = (_Tp)(moduleBase + 0x54e50);
	Tp(player, name);
}

void item(const char* name) {

	typedef void* (__thiscall* _GetItemByName)(void* gamePtr, const char* name);
	_GetItemByName GetItemByName;

	typedef bool(__thiscall* _AddItem)(void* playervftblptr, void* IItemPtr, unsigned int count, bool allowPartial);
	_AddItem AddItem;
	void* player = (void*)findAddr(moduleBase + 0x97D7C, { 0x1c,0x6c,0x0 });
	void* game = (void*)(moduleBase + 0x9780);
	GetItemByName = (_GetItemByName)(moduleBase + 0x1DE20);
	AddItem = (_AddItem)(moduleBase + 0x51BA0);
	void* item = GetItemByName(game, name);
	AddItem(player, item, 1, 0);
	//GreatBallsOfFire
	//HandCannon

}
void spawnActor(int name) {
	//const char* name
	typedef void* (__thiscall* _getActor)(void* actorObj);
	_getActor getActor;
	
	//if (name == "bear") {
	//	void* act = (void*)(moduleBase + 0x73fdc);
	//	getActor = (_getActor)(moduleBase + 0x22400);
	//	} 
	void* act = (void*)(moduleBase + 0x73fdc);
	getActor = (_getActor)(moduleBase + 0x22400);
	if (name == 1) {
		void* act = (void*)(moduleBase + 0x72fa4);
		getActor = (_getActor)(moduleBase + 0x222b0);
	}
	else if (name == 2) {
		void* act = (void*)(moduleBase + 0x71d78);
		getActor = (_getActor)(moduleBase + 0x22160);
	}
	else if (name == 3) {
		void* act = (void*)(moduleBase + 0x74158);
		getActor = (_getActor)(moduleBase + 0x225c0);
	}
	else if (name == 4) {
		void* act = (void*)(moduleBase + 0x7423c);
		getActor = (_getActor)(moduleBase + 0x221d0);
	}
	else if (name == 5) {
		void* act = (void*)(moduleBase + 0x73b80);
		getActor = (_getActor)(moduleBase + 0x22240);
	}
	else if (name == 6) {
		void* act = (void*)(moduleBase + 0x72654);
		getActor = (_getActor)(moduleBase + 0x22320);
	}
	else if (name == 7) {
		void* act = (void*)(moduleBase + 0x1a130);
		getActor = (_getActor)(moduleBase + 0x22390);
	}
	else if (name == 8) {
		void* act = (void*)(moduleBase + 0x73e80);
		getActor = (_getActor)(moduleBase + 0x22470);
	}
	else if (name == 9) {
		void* act = (void*)(moduleBase + 0x7262c);
		getActor = (_getActor)(moduleBase + 0x224e0);
	}
	else if (name == 10) {
		void* act = (void*)(moduleBase + 0x73b90);
		getActor = (_getActor)(moduleBase + 0x22550);
	}


	void* actor = getActor(act);

	typedef void(__thiscall* _spawnActor)(void* world, unsigned int id, void* actor, const Vector3* pos, const Rotation* rot);
	_spawnActor spawnActor;

	void* world = (void*)(moduleBase + 0x97D7C);

	spawnActor = (_spawnActor)(moduleBase + 0x630c0);
	float x = *(float*)findAddr(procBase + 0x018FFDE4, { 0x4, 0x4, 0x1D4, 0x408, 0x24C, 0x180, 0x90 });
	float y = *(float*)findAddr(procBase + 0x018FFDE4, { 0x4, 0x4, 0x1D4, 0x408, 0x24C, 0x180, 0x94 });
	float z = *(float*)findAddr(procBase + 0x018FFDE4, { 0x4, 0x4, 0x1D4, 0x408, 0x24C, 0x180, 0x98 }) + 500.0f;

	Vector3 pos = { x, y, z };
	Rotation rot = { 0.0f, 0.0f, 0.0f };
	spawnActor(world, 1, actor, &pos, &rot);

}
void processInput(const std::string& input) { //splitting the input into tokens/words
	std::istringstream iss(input);
	std::vector<std::string> tokens;
	std::string token;

	while (iss >> token) {
		tokens.push_back(token);
	}

	if (!tokens.empty() && tokens[0] == "fire") {
		teleport(procBase, -43644.0f, -56041.0f, 309.0f);
		//hkghkj
	}
	else if (!tokens.empty() && tokens[0] == "spawn") {
		if (tokens[1] == "bear") {
			spawnActor(0);
		}
		else if (tokens[1] == "giantrat") {
			spawnActor(1);
		}
		else if (tokens[1] == "justintolerable"){
			spawnActor(2);
		}
		else if (tokens[1] == "magmarok") {
			spawnActor(3);
		}
		else if (tokens[1] == "farmer") {
			spawnActor(4);
		}
		else if (tokens[1] == "michaelangelo") {
			spawnActor(5);
		}
		else if (tokens[1] == "cavespider") {
			spawnActor(6);
		}
		else if (tokens[1] == "icespiderqueen") {
			spawnActor(7);
		}
		else if (tokens[1] == "angrybear") {
			spawnActor(8);
		}
		else if (tokens[1] == "madcow") {
			spawnActor(9);
		}
		else if (tokens[1] == "cowking") {
			spawnActor(10);
		}
		else {
			std::cout << "Invalid actor name. Ensure it is in 1 word in lower case e.g. 'cowking'." << std::endl;
		}

	}
	else if (!tokens.empty() && tokens[0] == "tp" && tokens.size() == 2) {
		const char* name = tokens[1].c_str();
		std::cout << name;
		tp(name);
	   /*"Town"		
		"PirateBay"
		"GoldFarm"
		"BallmerPeak"
	    "UnbearableWoods"
		"Sewer"
		"LostCave"
		"MoltenCave"*/

	}

	else if (!tokens.empty() && tokens[0] == "coins") {
		if (tokens.size() > 1) {
			// extracting the value after 'coins'
			int amount;
			std::istringstream(tokens[1]) >> amount;

			// call function to add coins with the extracted amount
			addCoins(amount);
		}
		else {
			std::cout << "no value provided after 'coins'." << std::endl;
		}
	}

	else if (!tokens.empty() && tokens[0] == "teleport") {
		if (tokens.size() == 4) {
			// extracting x, y, and z coordinates
			float x, y, z;
			std::istringstream(tokens[1]) >> x;
			std::istringstream(tokens[2]) >> y;
			std::istringstream(tokens[3]) >> z;

			// call the teleport function with the extracted coordinates
			teleport(procBase, x, y, z);
		}
		else {
			std::cout << "Incorrect number of coordinates provided after 'teleport'. Use format: teleport x y z." << std::endl;
		}
	}

	else if (!tokens.empty() && tokens[0] == "rtp") {
		if (tokens.size() == 4) {
			// extracting x, y, and z coordinates
			float x, y, z;
			std::istringstream(tokens[1]) >> x;
			std::istringstream(tokens[2]) >> y;
			std::istringstream(tokens[3]) >> z;

			// call the teleport function with the extracted coordinates
			rteleport(procBase, x, y, z);
		}
		else {
			std::cout << "Incorrect number of coordinates provided after 'teleport'. Use format: teleport x y z." << std::endl;
		}
	}

	else if (!tokens.empty() && tokens[0] == "item") {
		if (tokens.size() == 2) {
			const char* name = tokens[1].c_str();
			std::cout << name;
			item(name);
		}	
	}
	else {
		std::cout << "Input does not start with 'teleport'." << std::endl;
	}

}

void on_player_chat(char* msg, char* player) {
	std::string str(msg);
	processInput(str);
}
