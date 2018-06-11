#include "stdafx.h"
#include "MainLoop.h"
#define CREATE_CONSOLE

IME_Manager g_imeManager;

#include "Atlas.h"
Atlas* g_atlas = nullptr;

bool MainLoop::Initialize()
{
#ifdef CREATE_CONSOLE
	if (AllocConsole())
	{
		freopen("CONIN$", "rb", stdin);
		freopen("CONOUT$", "wb", stdout);
		freopen("CONOUT$", "wb", stderr);
	}
#endif

				

	DragAcceptFiles(g_processManager->GetWndInfo()->hWnd, true);



	cout.imbue(std::locale("kor"));
	wcout.imbue(std::locale("kor"));
	DEVICE->SetRenderState(D3DRS_LIGHTING, false);
	DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	DEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	g_atlas = new Atlas(DEVICE);

	return true;
}

void MainLoop::Update()
{
	g_inputDevice.BeginFrame(g_processManager->GetWndInfo()->hWnd);

	SingletonInstance(Camera)->Update();
	g_atlas->Update();

	g_inputDevice.EndFrame();
}

bool MainLoop::Render()
{
	SingletonInstance(Camera)->ApplyTransform();

	LPD3DXSPRITE sp;
	D3DXCreateSprite(DEVICE, &sp);
	sp->Begin(D3DXSPRITE_ALPHABLEND);
	
	LPD3DXFONT font;
	D3DXCreateFontW(DEVICE, 10, 0, 0, 0, 0, 1, 0, 0, 0, L"", &font);
	RECT rc;
	SetRect(&rc, 100, 100, 100, 100);
	font->DrawTextW(sp, g_imeManager.GetString().data(), -1, &rc, DT_NOCLIP, D3DXCOLOR(0, 0, 0, 1));
	font->Release();

	sp->End();
	sp->Release();


	g_atlas->Render();
	return true;
}

bool MainLoop::Release()
{



#ifdef CREATE_CONSOLE
	FreeConsole();
#endif
	return true;
}


LRESULT MainLoop::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	g_inputDevice.MsgProc(msg, wParam, lParam);
	g_imeManager.MsgProc(hWnd, msg, wParam, lParam);
	g_atlas->MsgProc(msg, wParam, lParam);

	switch (msg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			g_imeManager.Clear();
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProcW(hWnd, msg, wParam, lParam);
	}

	return 0;
}
