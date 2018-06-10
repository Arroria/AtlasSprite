#include "stdafx.h"
#include "Atlas.h"


Atlas::Atlas(LPDIRECT3DDEVICE9 device)
	: m_device(device)

	, m_tex(nullptr)
{
}
Atlas::~Atlas()
{
}



void Atlas::Render()
{
	if (m_tex)
	{
		D3DXMATRIX sm;
		D3DXMatrixScaling(&sm, m_texInfo.Width, m_texInfo.Height, 1);
		m_device->SetTransform(D3DTS_WORLD, &sm);
	}
	else
	{
		D3DXMATRIX iden;
		D3DXMatrixIdentity(&iden);
		m_device->SetTransform(D3DTS_WORLD, &iden);
	}

	m_device->SetTexture(0, m_tex);
	SingletonInstance(PlaneRenderer)->Render(m_device);
}

void Atlas::MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DROPFILES:
	{
		HDROP dropFile = (HDROP)wParam;

		size_t fileCount = DragQueryFileW(dropFile, 0xFFFFFFFF, nullptr, NULL);
		if (fileCount > 1)	wcout << L"여러개의 파일 호출은 구현되지 않았습니다." << endl;
		for (size_t fileIndex = 0; fileIndex < fileCount; fileIndex++)
		{
			size_t filePathSize = DragQueryFileW(dropFile, fileIndex, nullptr, NULL);
			wchar_t filePath[256] = { NULL };
			DragQueryFileW(dropFile, fileIndex, filePath, filePathSize + 1);
			if (fileIndex)
			{
				wcout << L"사용되지 않은 파일 : " << filePath << endl;
				continue;
			}

			wcout << L"TEST : " << filePath << endl;
			ChangeTexture(filePath);
		}
		DragFinish(dropFile);
	}
	break;
	}
}



void Atlas::ChangeTexture(const std::wstring & path)
{
	if (m_tex)
		SAFE_RELEASE(m_tex);

	D3DXCreateTextureFromFileExW(m_device, path.data(), D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, NULL, NULL, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, NULL, &m_texInfo, nullptr, &m_tex);
}
