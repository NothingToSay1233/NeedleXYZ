#include "protect/tcp_client.hpp"
#include <thread>
#include <chrono>
#include <filesystem>
#include "DLL_MAIN.h"
#include "Hooks.h"
#include "Features.h"
#include "netvar_manager.h"
#include "EnginePrediction.h"
#include "render.h"
#include "protect/md5.hpp"
#include "protect/cryptoc.hpp"
#include "server_api.hpp"
#include "protect/config.h"


CNetVarManager netvars;

void InitializeNetvars() {
	netvars.Initialize();
}

#define wstr(x) nnx::encoding::utf8to16(x)

std::wstring project_dir;
c_config config;
std::int32_t cfg_last_update_time_in_minutes;
std::string loader_hwid;
void load_cfg()
{
	if (config.offset > 0)
	{
		config.pop_bool(); /* save_data */
		cfg_last_update_time_in_minutes = config.pop_int32_t(); /* cfg_last_update_time_in_minutes */
		printf("%i\n", cfg_last_update_time_in_minutes);
		printf("%i\n\n", static_cast<int32_t>(GetTickCount() / 1000 / 60));

		csgo->password = config.pop_string();
		csgo->username = config.pop_string();
		loader_hwid = config.pop_string(); /* hwid */
	}
}


static UINT8 p_cli_dynamic_key_raw[DEF_KEY_SIZE]{};


bool is_dir(const TCHAR* dir) {
	DWORD flag = GetFileAttributes(dir);
	if (flag == 0xFFFFFFFFUL) {
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
			return false;
	}
	if (!(flag & FILE_ATTRIBUTE_DIRECTORY))
		return false;
	return true;
}



DWORD WINAPI CheatMain(LPVOID lpThreadParameter)
{
#ifdef _DEBUG
	AllocConsole();                                
	freopen_s((FILE**)stdin, "CONIN$", "r", stdin);
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
#endif
	srand(time(0));

	c_config::SH_APPDATA.resize(MAX_PATH);
	if (!SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_APPDATA, NULL, 0, &c_config::SH_APPDATA[0])))
		Exit();
	c_config::SH_APPDATA.resize(wcslen(&c_config::SH_APPDATA[0]));
	//add_log(str("-----\tnew log started\t-----\n"));
	project_dir = c_config::SH_APPDATA + nnx::encoding::utf8to16(str("\\Needle"));
	config = c_config(project_dir, nnx::encoding::utf8to16(str("config.cfg")));
	csgo->log_location = project_dir + strw(L"\\log.txt");

	static TCHAR path[MAX_PATH];
	std::string cfg_folder, js_folder;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path)))
	{
		cfg_folder = std::string(path) + str("\\Needle\\configs\\");
		js_folder = std::string(path) + str("\\Needle\\scripts\\");
	}
	if (!is_dir(cfg_folder.c_str()))
		CreateDirectory(cfg_folder.c_str(), NULL);
	if (!is_dir(js_folder.c_str()))
		CreateDirectory(js_folder.c_str(), NULL);

	printf(str("0\n"));

	printf(str("entry point\n"));
	while (!(csgo->Init.Window = FindWindowA(hs::Valve001.s().c_str(), NULL)))
		this_thread::sleep_for(200ms);
	while (!GetModuleHandleA(hs::client_dll.s().c_str()))
		this_thread::sleep_for(200ms);
	while (!GetModuleHandleA(hs::engine_dll.s().c_str()))
		this_thread::sleep_for(200ms);
	while (!GetModuleHandleA(hs::serverbrowser_dll.s().c_str()))
		this_thread::sleep_for(200ms);


	xs64_reset_seed();
	load_cfg();
	I::Setup();
	g_Chams->Init();
	InitializeNetvars();
	H::Hook();

	while (!csgo->DoUnload)
		this_thread::sleep_for(1s);
	I::Setup();
	g_Chams->Init();
	InitializeNetvars();
	H::Hook();

	while (!csgo->DoUnload)
		this_thread::sleep_for(1s);

	if (csgo->DoUnload)
		H::UnHook();
	else
	printf(str("Needle has been injected!\n"));

	interfaces.engine->ClientCmd_Unrestricted(hs::clear.s().c_str(), 0);
	interfaces.engine->ClientCmd_Unrestricted(hs::unload_message.s().c_str(), 0);

	SetWindowLongPtr(csgo->Init.Window, GWL_WNDPROC, (LONG_PTR)csgo->Init.OldWindow);
	FreeLibraryAndExitThread(csgo->Init.Dll, 0);

	return 0;
}

BOOL APIENTRY DllMain(HMODULE hDll, DWORD dwReason, LPVOID lpThreadParameter) {
	
	if (dwReason == DLL_PROCESS_ATTACH) {
		CreateThread(nullptr, 0, static_cast<LPTHREAD_START_ROUTINE>(CheatMain), lpThreadParameter, 0, nullptr);
		csgo->Init.Dll = hDll;
	}
	return TRUE;
}