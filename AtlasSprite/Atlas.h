#pragma once
class Atlas
{
private:
	LPDIRECT3DTEXTURE9 m_tex;


private:


public:
	void Draw(LPDIRECT3DDEVICE9 device);
	void MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	Atlas();
	~Atlas();
};

