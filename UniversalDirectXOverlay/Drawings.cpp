#include "Drawings.h"


void DrawString(char* String, int x, int y, int a, int r, int g, int b, ID3DXFont* font)
{
	RECT FontPos;
	FontPos.left = x;
	FontPos.top = y;
	font->DrawTextA(0, String, strlen(String), &FontPos, DT_NOCLIP, D3DCOLOR_ARGB(a, r, g, b));
}

void DrawFilledRect(int x, int y, int w, int h, int a, int r, int g, int b)
{
	D3DCOLOR color = D3DCOLOR_ARGB(a, r, g, b);
	D3DRECT rect = { x, y, x + w, y + h };
	p_Device->Clear(1, &rect, D3DCLEAR_TARGET | D3DCLEAR_TARGET, color, 0, 0);
}


void DrawBorderBox(int x, int y, int w, int h, int thickness, int a, int r, int g, int b)
{

	DrawFilledRect(x, y, w, thickness, a, r, g, b);

	DrawFilledRect(x, y, thickness, h, a, r, g, b);

	DrawFilledRect((x + w), y, thickness, h, a, r, g, b);

	DrawFilledRect(x, y + h, w + thickness, thickness, a, r, g, b);
}