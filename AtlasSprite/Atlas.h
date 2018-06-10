#pragma once
class Atlas
{
private:
	LPDIRECT3DDEVICE9 m_device;

	LPDIRECT3DTEXTURE9 m_tex;
	D3DXIMAGE_INFO m_texInfo;


private:
	void ChangeTexture(const std::wstring& path);

public:
	void Render();
	void MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	Atlas(LPDIRECT3DDEVICE9 device);
	~Atlas();
};

