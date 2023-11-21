#include "pch.h"
#include <string>
#include <sstream>

struct Vector3 {
	float x, y, z;
};

struct Rotation {
	float x, y, z;
};

bool mana = false, ammo = false, items = false, speed = false, damage = false, ak = false;

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
float spread; // Our custom spread angle
void __declspec(naked) akSpread()
{
	__asm {
		fld spread
		jmp[akSpreadJmpBackAddr] // Jump back to original code
	}
}

DWORD akCooldownJmpBackAddr;
float cooldown; // Our custom ak cooldown
void __declspec(naked) akCooldown()
{
	__asm {
		fld cooldown
		jmp[akCooldownJmpBackAddr] // Jump back to original code
	}
}

DWORD fCooldownJmpBackAddr;
float fcooldown; // Our custom item cooldown
void __declspec(naked) itemCooldown()
{
	__asm {
		movss xmm0, fcooldown
		jmp[fCooldownJmpBackAddr] // Jump back to original code
	}
}

void our_player_chat(char* msg, char* player);
DWORD chatJmpBackAddr;
void __declspec(naked) player_chat() {
	__asm {
		push ecx
		mov eax, [esp + 0x8]
		push eax
		call our_player_chat
		pop eax
		pop ecx
		push ebp
		mov ebp, esp
		and esp, 0x0fffffff8
		push 0xffffffff
		jmp[chatJmpBackAddr]
	}
}


//values from cheat engine(player position coordinates)
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

//void addCoins(int amount) {
//	*(int*)findAddr(moduleBase + 0x97D7C, { 0x1C, 0x6C, 0x4C, 0x0, 0X18 }) += amount;
//	// adding offsets to the base game dll address to find the address of the coins variable and adding the amount to it
//}

//bool make_execute_readwrite(BYTE* base, unsigned int size) {
//	DWORD oldProtect;
//	return VirtualProtect(base, size, PAGE_EXECUTE_READWRITE, &oldProtect) != 0;
//}

void tp(const char* name) {

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
	
	typedef void* (__thiscall* _getActor)(void* actorObj);
	_getActor getActor;
	
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
	float x = *(float*)findAddr(procBase + 0x018FFDE4, { 0x4, 0x4, 0x1D4, 0x408, 0x24C, 0x180, 0x90 })+200.0f;
	float y = *(float*)findAddr(procBase + 0x018FFDE4, { 0x4, 0x4, 0x1D4, 0x408, 0x24C, 0x180, 0x94 });
	float z = *(float*)findAddr(procBase + 0x018FFDE4, { 0x4, 0x4, 0x1D4, 0x408, 0x24C, 0x180, 0x98 })+500.0f;

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

	/*if (!tokens.empty() && tokens[0] == "fire") {
		teleport(procBase, -43640.0f, -56040.0f, 310.0f);
	}*/
	if (!tokens.empty() && tokens[0] == "spawn") {
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
	else if (!tokens.empty() && tokens[0] == "tp") {
		if (tokens.size() == 2) {
			const char* name = tokens[1].c_str();			
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
		else {
			std::cout << "no value provided after 'tp'." << std::endl;
		}
	}

	//else if (!tokens.empty() && tokens[0] == "coins") {
	//	if (tokens.size() > 1) {
	//		// extracting the value after 'coins'
	//		int amount;
	//		std::istringstream(tokens[1]) >> amount;

	//		// call function to add coins with the extracted amount
	//		addCoins(amount);
	//	}
	//	else {
	//		std::cout << "no value provided after 'coins'." << std::endl;
	//	}
	//}

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
			std::cout << "Incorrect number of coordinates provided after 'teleport'. Use format: teleport [x] [y] [z]." << std::endl;
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
			std::cout << "Incorrect number of coordinates provided after 'rtp'. Use format: rtp [x] [y] [z]." << std::endl;
		}
	}

	else if (!tokens.empty() && tokens[0] == "item") {
		if (tokens.size() == 2) {
			const char* name = tokens[1].c_str();			
			item(name);
		}
		else {
			std::cout << "no value provided after 'item'." << std::endl;
		}
	}
	else if (!tokens.empty() && tokens.size() == 2 && tokens[0] == "infinite" && tokens[1]=="mana") {
		mana = !mana;

		if (mana)
		{
			std::cout << "[+] Enabling infinite mana\n";
			//nop 6 bytes which update mana
			nop((BYTE*)(moduleBase + 0x525C7), 6);
		}
		else
		{
			std::cout << "[-] Disabling infinite mana\n";
			//restore original bytes
			patch((BYTE*)(moduleBase + 0x525C7), (BYTE*)"\x89\x86\xbc\x00\x00\x00", 6);
		}
	}

	else if (!tokens.empty() && tokens.size() == 2 && tokens[0] == "infinite" && tokens[1] == "ammo") {
		ammo = !ammo;

		if (ammo)
		{
			std::cout << "[+] Enabling infinite ammo\n";
			//nop 3 bytes which update mana
			nop((BYTE*)(moduleBase + 0x52396), 3);
		}
		else
		{
			std::cout << "[-] Disabling infinite ammo\n";
			// Look at IDA Hex View to find original hex values for patching
			patch((BYTE*)(moduleBase + 0x52396), (BYTE*)"\x89\x48\x1c", 3);
		}
	}

	else if (!tokens.empty() && tokens.size() == 2 && tokens[0] == "infinite" && ((tokens[1] == "items") ||(tokens[1]=="coins")) ){
		items = !items;

		if (items)
		{
			std::cout << "[+] Enabling infinite coins/items\n";
			nop((BYTE*)(moduleBase + 0x52217), 3);
		}
		else
		{
			std::cout << "[-] Disabling infinite coins/items\n";
			patch((BYTE*)(moduleBase + 0x52217), (BYTE*)"\x89\x48\x18", 3);
		}
	}

	else if (!tokens.empty() && tokens.size() == 1 && tokens[0] == "speed") {
		speed = !speed;
		//GetWalkingSpeed function address in GameLogic.dll
		DWORD speedHookAddress = moduleBase + 0x4FF90;
		int speedHookLength = 6;
		if (speed)
		{
			//address in real function to jump back to after our code
			speedJmpBackAddr = speedHookAddress + speedHookLength;
			std::cout << "[-] Hooking into walking speed method\n";
			walkSpeed = 2000.0f;//custom speed
			//our code, in functions.cpp
			if (hook((void*)speedHookAddress, speedHook, speedHookLength))
			{
				std::cout << "[+] Enabled speed hack\n";
			}
		}
		else
		{
			std::cout << "[-] Disabling speed hack\n";
			//Original bytes
			patch((BYTE*)(speedHookAddress), (BYTE*)"\xD9\x81\x20\x01\x00\x00", speedHookLength);
		}
	}

	else if (!tokens.empty() && tokens.size() == 2 && tokens[0] == "zero" && tokens[1]=="damage") {
		damage = !damage;
		if (damage)
		{
			std::cout << "[+] Enabling zero damage\n";
			// \x84 -> jnz patching with \x85 -> jz to skip the part which updates health after damage is taken
			patch((BYTE*)(moduleBase + 0x51176), (BYTE*)"\x0F\x84\x9C\x00\x00\x00", 6);
		}
		else
		{
			std::cout << "[-] Disabling zero damage\n";
			// restoring
			patch((BYTE*)(moduleBase + 0x51176), (BYTE*)"\x0F\x85\x9C\x00\x00\x00", 6);
		}
	}

	else if (!tokens.empty() && tokens.size() == 1 && tokens[0] == "coords") {
		float x = *(float*)findAddr(procBase + 0x018FFDE4, { 0x4, 0x4, 0x1D4, 0x408, 0x24C, 0x180, 0x90 });
		float y = *(float*)findAddr(procBase + 0x018FFDE4, { 0x4, 0x4, 0x1D4, 0x408, 0x24C, 0x180, 0x94 });
		float z = *(float*)findAddr(procBase + 0x018FFDE4, { 0x4, 0x4, 0x1D4, 0x408, 0x24C, 0x180, 0x98 });

		std::cout << "x: " << x << "\n";
		std::cout << "y: " << y << "\n";
		std::cout << "z: " << z << "\n";

	}

	else if (!tokens.empty() && tokens.size() == 2 && tokens[0] == "op" && tokens[1] == "ak") {
		ak = !ak;
		DWORD akSpreadHookAddress = moduleBase + 0x13A00;
		DWORD akCooldownHookAddress = moduleBase + 0x139E0;
		DWORD fCooldownHookAddress = moduleBase + 0x5263A;
		int fCooldownHookLength = 5;
		int akCooldownHookLength = 6;
		int akSpreadHookLength = 6;
		if (ak)
		{
			std::cout << "[+] Enabling increased ak damage\n";
			//patch((BYTE*)(moduleBase + 0x13930), (BYTE*)"\xB8\xFF\x00\x00\x00", 5);
			patch((BYTE*)(moduleBase + 0x139F0), (BYTE*)"\xB8\xFF\xFF\x00\x00", 5);


			//address in real function to jump back to after our code
			akSpreadJmpBackAddr = akSpreadHookAddress + akSpreadHookLength;
			std::cout << "[-] Hooking into ak spread method\n";
			spread = 0.0f;
			//our code, in functions.cpp
			if (hook((void*)akSpreadHookAddress, akSpread, akSpreadHookLength))
			{
				std::cout << "[+] Enabled ak spread hack\n";
			}


			//address in real function to jump back to after our code
			akCooldownJmpBackAddr = akCooldownHookAddress + akCooldownHookLength;
			std::cout << "[-] Hooking into ak cooldown method\n";
			cooldown = 0.0f;
			//our code, in functions.cpp
			if (hook((void*)akCooldownHookAddress, akCooldown, akCooldownHookLength))
			{
				std::cout << "[+] Enabled ak cooldown hack\n";
			}

			//address in real function to jump back to after our code
			fCooldownJmpBackAddr = fCooldownHookAddress + fCooldownHookLength;
			std::cout << "[-] Hooking into item cooldown method\n";
			fcooldown = 0.0f;
			//our code, in functions.cpp
			if (hook((void*)fCooldownHookAddress, itemCooldown, fCooldownHookLength))
			{
				std::cout << "[+] Enabled item cooldown hack\n";
			}

		}
		else
		{
			//disable all
			std::cout << "[-] Disabling hacks\n";
			patch((BYTE*)(moduleBase + 0x139F0), (BYTE*)"\xB8\x0B\x00\x00\x00", 5);
			patch((BYTE*)(moduleBase + 0x13A00), (BYTE*)"\xD9\x05\x20\x8B\x07\x10", akSpreadHookLength);
			patch((BYTE*)(moduleBase + 0x139E0), (BYTE*)"\xD9\x05\xF0\x8A\x07\x10", akCooldownHookLength);
			patch((BYTE*)(moduleBase + 0x5263A), (BYTE*)"\xF3\x0F\x10\x45\x0C", fCooldownHookLength);
		}
	}
	else {
		std::cout << "Invalid Hack" << std::endl;
	}

}

void our_player_chat(char* msg, char* player) {
	std::string str(msg);
	processInput(str);
}
