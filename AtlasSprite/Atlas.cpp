#include "stdafx.h"
#include "Atlas.h"


Atlas::Atlas()
{
}
Atlas::~Atlas()
{
}



void Atlas::Draw(LPDIRECT3DDEVICE9 device)
{
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
		}
		DragFinish(dropFile);
	}

	}
}