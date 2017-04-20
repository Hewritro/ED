#pragma once
#include <stdint.h>
#include <d3dx9core.h>

class DirectXHook {
private:
	static uint32_t* horizontalRes;
	static uint32_t* verticalRes;
	static int normalSizeCurrentFontHeight;
	static int largeSizeCurrentFontHeight;
	static int normalSizeFontHeight;
	static int largeSizeFontHeight;

	static LPDIRECT3DDEVICE9 pDevice;
	static LPD3DXFONT normalSizeFont;
	static LPD3DXFONT largeSizeFont;
	static HRESULT(__stdcall * origEndScenePtr)(LPDIRECT3DDEVICE9);
	static HRESULT __stdcall hookedEndScene(LPDIRECT3DDEVICE9 device);
	static HRESULT(__stdcall * origDrawIndexedPrimitivePtr)(LPDIRECT3DDEVICE9, D3DPRIMITIVETYPE, INT, UINT, UINT, UINT, UINT);
	static HRESULT __stdcall hookedDrawIndexedPrimitive(LPDIRECT3DDEVICE9 device, D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount);

	static uint32_t* getDirectXVTableMethod1();
	static uint32_t* getDirectXVTableMethod2();
	static uint32_t* getDirectXVTableMethod3();
	static void drawText(int x, int y, DWORD color, const char* text, LPD3DXFONT pFont);
	static void drawRect(int x, int y, int width, int height, DWORD Color);
	static void drawHorizontalLine(int x, int y, int width, D3DCOLOR Color);
	static void drawVerticalLine(int x, int y, int height, D3DCOLOR Color);
	static void drawBox(int x, int y, int width, int height, D3DCOLOR BorderColor, D3DCOLOR FillColor);
	static void drawChatInterface();
	static int getTextWidth(const char *szText, LPD3DXFONT pFont);
	static int getSpaceCharacterWidth(LPD3DXFONT pFont);
	static int centerTextHorizontally(const char* text, int x, int width, LPD3DXFONT pFont);
	static void initFontsIfRequired();
	static void drawHelpMessage();

public:
	static CONST D3DCOLOR COLOR_RED = D3DCOLOR_ARGB(255, 255, 000, 000);
	static CONST D3DCOLOR COLOR_GREEN = D3DCOLOR_ARGB(255, 127, 255, 000);
	static CONST D3DCOLOR COLOR_ORANGE = D3DCOLOR_ARGB(255, 255, 140, 000);
	static CONST D3DCOLOR COLOR_BLUE = D3DCOLOR_ARGB(255, 000, 000, 255);
	static CONST D3DCOLOR COLOR_YELLOW = D3DCOLOR_ARGB(255, 255, 255, 51);
	static CONST D3DCOLOR COLOR_BLACK = D3DCOLOR_ARGB(255, 000, 000, 000);
	static CONST D3DCOLOR COLOR_GREY = D3DCOLOR_ARGB(255, 112, 112, 112);
	static CONST D3DCOLOR COLOR_GOLD = D3DCOLOR_ARGB(255, 255, 215, 000);
	static CONST D3DCOLOR COLOR_PINK = D3DCOLOR_ARGB(255, 255, 192, 203);
	static CONST D3DCOLOR COLOR_PURPLE = D3DCOLOR_ARGB(255, 128, 000, 128);
	static CONST D3DCOLOR COLOR_CYAN = D3DCOLOR_ARGB(255, 000, 255, 255);
	static CONST D3DCOLOR COLOR_MAGNETA = D3DCOLOR_ARGB(255, 255, 000, 255);
	static CONST D3DCOLOR COLOR_WHITE = D3DCOLOR_ARGB(255, 255, 255, 249);
	static int helpMessageStartTime;

	static void hookDirectX();
};