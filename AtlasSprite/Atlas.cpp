#include "stdafx.h"
#include "Atlas.h"


Atlas::Atlas(LPDIRECT3DDEVICE9 device)
	: m_device(device)

	, m_tex(nullptr)
{
	ZeroMemory(&m_raycastPlane, sizeof(D3DXVECTOR3) * 4);
}
Atlas::~Atlas()
{
}



void Atlas::Update()
{
	if (m_tex)
	{
		D3DXVECTOR2 uv;
		if (GetRaycastUV(uv))
		{
			cout << uv.x << " " << uv.y << endl;
		}
	}
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

	DEVICE->SetRenderState(D3DRS_ZENABLE, false);
	DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, false);
	
	m_device->SetTexture(0, m_tex);
	SingletonInstance(PlaneRenderer)->Render(m_device);
	
	m_device->SetTexture(0, nullptr);
	SingletonInstance(FrameRenderer)->Render(m_device, D3DXCOLOR(1, 0, 1, 1));

	DEVICE->SetRenderState(D3DRS_ZENABLE, true);
	DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, true);
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
	//기존 텍스쳐 제거
	if (m_tex)
		SAFE_RELEASE(m_tex);
	
	//텍스쳐 변경
	D3DXCreateTextureFromFileExW(m_device, path.data(), D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, NULL, NULL, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, NULL, &m_texInfo, nullptr, &m_tex);


	if (m_tex)
	{
		//텍스쳐 크기에 맞는 레이캐스트 플랜 생성
		float x = m_texInfo.Width	* 0.5f;
		float y = m_texInfo.Height	* 0.5f;
		m_raycastPlane[0] = D3DXVECTOR3(-x, +y, 0);
		m_raycastPlane[1] = D3DXVECTOR3(+x, +y, 0);
		m_raycastPlane[2] = D3DXVECTOR3(-x, -y, 0);
		m_raycastPlane[3] = D3DXVECTOR3(+x, -y, 0);

		//텍스쳐 크기에 화면을 맞춰줌
		D3DVIEWPORT9 viewport;
		DEVICE->GetViewport(&viewport);

		SingletonInstance(Camera)->SetViewScale(
			m_texInfo.Width / (float)viewport.Width > m_texInfo.Height / (float)viewport.Height ?
			m_texInfo.Width : m_texInfo.Height
		);
		SingletonInstance(Camera)->SetFocus(D3DXVECTOR2(0, 0));
	}
}

bool Atlas::GetRaycastUV(D3DXVECTOR2& uv)
{
	D3DXVECTOR3 rayPos, rayDir;
	//Get RayData
	{
		POINT mousePos = g_inputDevice.MousePos();
		D3DXMATRIX vm, pm, vInvM;
		D3DVIEWPORT9 viewPort;

		DEVICE->GetTransform(D3DTS_VIEW, &vm);
		DEVICE->GetTransform(D3DTS_PROJECTION, &pm);
		D3DXMatrixInverse(&vInvM, 0, &vm);
		DEVICE->GetViewport(&viewPort);

		rayDir.x = ((mousePos.x * 2.f) / viewPort.Width - 1) / pm._11;
		rayDir.y = ((mousePos.y * -2.f) / viewPort.Height + 1) / pm._22;
		rayDir.z = 1;
		D3DXVec3TransformNormal(&rayDir, &rayDir, &vInvM);
		D3DXVec3Normalize(&rayDir, &rayDir);

		rayPos.x = vInvM._41;
		rayPos.y = vInvM._42;
		rayPos.z = vInvM._43;
	}

	float u = NULL, v = NULL, dist = NULL;
	if (D3DXIntersectTri(&m_raycastPlane[0], &m_raycastPlane[1], &m_raycastPlane[2], &rayPos, &rayDir, &u, &v, &dist))
	{
		uv.x = u;
		uv.y = v;
		return true;
	}
	if (D3DXIntersectTri(&m_raycastPlane[3], &m_raycastPlane[2], &m_raycastPlane[1], &rayPos, &rayDir, &u, &v, &dist))
	{
		uv.x = 1 - u;
		uv.y = 1 - v;
		return true;
	}
	return false;
}
