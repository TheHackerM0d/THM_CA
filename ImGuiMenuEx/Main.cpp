#include "dx/dx9.h" 
#include "stdafx.h"
#include "Functions.h"

WNDPROC p_window_proc = nullptr;
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT __stdcall wnd_proc(const HWND hwnd, const UINT u_msg, const WPARAM w_param, const LPARAM l_param)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, u_msg, w_param, l_param) && GetAsyncKeyState(VK_INSERT) & 1)
	{
		return 1l;
	}
	return CallWindowProc(p_window_proc, hwnd, u_msg, w_param, l_param);
}
VOID FOCUS_INIT(LPDIRECT3DDEVICE9 pDevice)
{
	if (MenusFont == 0) D3DXCreateFontA(pDevice, 14, 0, 400, 1, 0, 1, 0, 4, 0 | (0 << 1), "Arial", &MenusFont);
}
//==========================================================================================================================//
//                                                  Menu Render                                                             //
//==========================================================================================================================//
#define PROCESS_CSHELL		"cshell.dll"
VOID MenuLoopers(LPDIRECT3DDEVICE9 pDevice)
{
	FOCUS_INIT(pDevice);
	//if (GetModuleHandleA(PROCESS_CSHELL))
	//{
	//	
	//}

}
void __stdcall main_render(IDirect3DDevice9* pDevice)
{
	MenuLoopers(pDevice);
	if (Modmenu::do_ini)
	{
		const auto window = GetForegroundWindow();
		DWORD pid;
		if (!window || !GetWindowThreadProcessId(window, &pid) || pid != GetCurrentProcessId())return;
		Modmenu::do_ini = false;
		p_window_proc = reinterpret_cast<WNDPROC>(SetWindowLongA(window, GWLP_WNDPROC, LONG_PTR(wnd_proc)));
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui_ImplWin32_Init(window);
		ImGui_ImplDX9_Init(pDevice);
		//===========================================================//
		//		           MENU EFFECT                               //
		//===========================================================//
		ImGui::SetNextWindowPos(ImVec2{ 100 , 150 }, ImGuiCond_Once); ImGui::SetNextWindowSize(ImVec2{ 510, 410 }, ImGuiCond_Once);
		static int Active_Tab = 0;
		ImGui::StyleColorsBlueDark();
	}
	ImGui_ImplDX9_NewFrame(); ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	{
		static auto is_down = false; static auto is_clicked = false; if (GetAsyncKeyState(VK_INSERT)) { is_clicked = false; is_down = true; }
		else if (!GetAsyncKeyState(VK_INSERT) && is_down) { is_clicked = true; is_down = false; }
		else { is_clicked = false; is_down = false; }if (is_clicked) { Modmenu::Activation = !Modmenu::Activation; }
		if (Modmenu::Activation)
		{
			ImGui::SetNextWindowSize({ 730, 460 });
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
			ImGui::Begin("MenuName", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
			{
				if (ImGui::CollapsingHeader("Player"))
				{
				}
			}
			ImGui::End();
		}
		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	}
}
void do_thread() { dx9::set_frame_render(reinterpret_cast<void*>( main_render )); }
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH) { CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(do_thread), nullptr, 0, nullptr); }
	return TRUE;
}
