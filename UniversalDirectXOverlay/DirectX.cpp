#include "DirectX.h"
#include <iostream>
#include <vector>
#include <Windows.h>
#include "proc.h"
#include "mem.h"
#include "Main.h"
#include "Drawings.h"

IDirect3D9Ex* p_Object = 0;
IDirect3DDevice9Ex* p_Device = 0;
D3DPRESENT_PARAMETERS p_Params;
RECT rc;
ID3DXFont* pFont;

float center = 0;
float width = 0;
float head = 0;
float extra = 0;

struct Vec3
{
	float x, y, z;
};

struct Vec4
{
	float x, y, z, w;
};

struct Vec2
{
	float x, y;
};

//Base of player
Vec2 vScreen;

//Head of player
Vec2 vHead;

DWORD dwExit = 0;
HANDLE hProcess = 0;
uintptr_t moduleBase = 0;
const char* gameProcName = "ac_client.exe";
DWORD procId = GetProcId(gameProcName);

float Matrix[16];

int DirectXInit(HWND hWnd)
{
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &p_Object)))
		exit(1);

	ZeroMemory(&p_Params, sizeof(p_Params));
	p_Params.Windowed = TRUE;
	p_Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	p_Params.hDeviceWindow = hWnd;
	p_Params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	p_Params.BackBufferFormat = D3DFMT_A8R8G8B8;
	p_Params.BackBufferWidth = Width;
	p_Params.BackBufferHeight = Height;
	p_Params.EnableAutoDepthStencil = TRUE;
	p_Params.AutoDepthStencilFormat = D3DFMT_D16;

	if (FAILED(p_Object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &p_Params, 0, &p_Device)))
		exit(1);
	D3DXCreateFont(p_Device, 30, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &pFont);

	return 0;
}

bool WorldToScreen(Vec3 pos, Vec2 &screen, float matrix[16], int windowWidth, int windowHeight)
{
	//Matrix-vector Product, multiplying world(eye) coordinates by projection matrix = clipCoords
	Vec4 clipCoords;
	clipCoords.x = pos.x*matrix[0] + pos.y*matrix[1] + pos.z*matrix[2] + matrix[3];
	clipCoords.y = pos.x*matrix[4] + pos.y*matrix[5] + pos.z*matrix[6] + matrix[7];
	clipCoords.z = pos.x*matrix[8] + pos.y*matrix[9] + pos.z*matrix[10] + matrix[11];
	clipCoords.w = pos.x*matrix[12] + pos.y*matrix[13] + pos.z*matrix[14] + matrix[15];

	if (clipCoords.w < 0.1f)
		return false;

	//perspective division, dividing by clip.W = Normalized Device Coordinates
	Vec3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
	return true;
}

int Render()
{
	//Hacking stuff

	if (procId) {

		// Set offsets
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

		moduleBase = GetModuleBaseAddress(procId, gameProcName);

		// Example

		/*localPlayerPtr = moduleBase + 0x10F4F4;

		healthAddr = FindDMAAddy(hProcess, localPlayerPtr, { 0xF8 });

		moveForward = localPlayerPtr + 0x80;

		entityListPtr = moduleBase + 0x10F4F8;

		numberOfPlayers = moduleBase + 0x110D98;*/


	}
	else {
		std::cout << "Process not found, press enter to exit\n";
		getchar();
		return 0;
	}

	// Cheats binded to keys, like

	//if (GetAsyncKeyState(VK_NUMPAD1) & 1) {
	//	bAmmo = !bAmmo;

	//	if (bAmmo) {
	//		//ff 06 = inc [esi]
	//		mem::PatchEx((BYTE*)(moduleBase + 0x637E9), (BYTE*)"\xFF\x06", 2, hProcess);
	//		std::cout << "Inf. ammo activated!\n";
	//	}
	//	else {
	//		//ff 0E = dec [esi]
	//		mem::PatchEx((BYTE*)(moduleBase + 0x637E9), (BYTE*)"\xFF\x0E", 2, hProcess);
	//		std::cout << "Inf. ammo deactivated!\n";
	//	}
	//}


	Sleep(1);
	if (tWnd == GetForegroundWindow())
	{
		char * value = tWindowName;

		HWND newhwnd = FindWindow(NULL, value);
		if (newhwnd != NULL) {
			GetWindowRect(newhwnd, &rc);
		}
		else {
			ExitProcess(0);
		}

		p_Device->Clear(0, 0, D3DCLEAR_TARGET, 0, 1.0f, 0);
		p_Device->BeginScene();


		//Drawing Stuff

		p_Device->EndScene();
		p_Device->PresentEx(0, 0, 0, 0, 0);
		return 0;
	}
}