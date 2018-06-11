#pragma once


class Atlas
{
private:
	LPDIRECT3DDEVICE9 m_device;

	LPDIRECT3DTEXTURE9 m_tex;
	D3DXIMAGE_INFO m_texInfo;

	D3DXVECTOR3 m_raycastPlane[4];

private:
	void ChangeTexture(const std::wstring& path);
	bool GetRaycastUV(D3DXVECTOR2& uv);

public:
	void Update();
	void Render();
	void MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	Atlas(LPDIRECT3DDEVICE9 device);
	~Atlas();
};

