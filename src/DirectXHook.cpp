#include "DirectXHook.hpp"
#include "Console/GameConsole.hpp"
#include <detours.h>
#include "Menu.hpp"

uint32_t* DirectXHook::horizontalRes = 0;
uint32_t* DirectXHook::verticalRes = 0;
int DirectXHook::normalSizeCurrentFontHeight = 0;
int DirectXHook::largeSizeCurrentFontHeight = 0;
int DirectXHook::largeSizeFontHeight = 0;
int DirectXHook::normalSizeFontHeight = 0;

LPDIRECT3DDEVICE9 DirectXHook::pDevice = 0;
D3DVIEWPORT9 viewport;
LPD3DXFONT DirectXHook::normalSizeFont = 0;
LPD3DXFONT DirectXHook::largeSizeFont = 0;
HRESULT(__stdcall * DirectXHook::origEndScenePtr)(LPDIRECT3DDEVICE9) = 0;
HRESULT(__stdcall * DirectXHook::origDrawIndexedPrimitivePtr)(LPDIRECT3DDEVICE9, D3DPRIMITIVETYPE, INT, UINT, UINT, UINT, UINT) = 0;

int DirectXHook::helpMessageStartTime = 0;

HRESULT __stdcall DirectXHook::hookedEndScene(LPDIRECT3DDEVICE9 device)
{
	DirectXHook::pDevice = device;

	//Fixes the viewport if the game is in fullscreen with an incorrect aspect ratio.
	DirectXHook::pDevice->GetViewport(&viewport);
	viewport.X = 0;
	DirectXHook::pDevice->SetViewport(&viewport);

	initFontsIfRequired();

	DirectXHook::drawHelpMessage();

	return (*DirectXHook::origEndScenePtr)(device);
}
HRESULT __stdcall DirectXHook::hookedDrawIndexedPrimitive(LPDIRECT3DDEVICE9 device, D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount)
{
	return (*DirectXHook::origDrawIndexedPrimitivePtr)(device, Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

int DirectXHook::getTextWidth(const char *szText, LPD3DXFONT pFont)
{
	RECT rcRect = { 0, 0, 0, 0 };
	if (pFont)
	{
		pFont->DrawText(NULL, szText, strlen(szText), &rcRect, DT_CALCRECT, D3DCOLOR_XRGB(0, 0, 0));
	}
	int width = rcRect.right - rcRect.left;
	std::string text(szText);
	std::reverse(text.begin(), text.end());

	text = text.substr(0, text.find_first_not_of(' ') != std::string::npos ? text.find_first_not_of(' ') : 0);
	for(char c : text)
	{
		width += getSpaceCharacterWidth(pFont);
	}
	return width;
}

int DirectXHook::getSpaceCharacterWidth(LPD3DXFONT pFont)
{
	return getTextWidth("i i", pFont) - ((getTextWidth("i", pFont)) * 2);
}

void DirectXHook::drawHelpMessage()
{
	if (!helpMessageStartTime)
	{
		//helpMessageStartTime = GetTickCount();
		return;
	}
	else if (GetTickCount() - helpMessageStartTime < 15000)
	{
		int x = (int)(0.01 * *horizontalRes);
		int y = (int)(0.01 * *verticalRes);
		int verticalSpacingBetweenEachLine = (int)(1.0 * largeSizeFontHeight);

		drawText(x, y, COLOR_RED, "Important Hotkeys", largeSizeFont);
		y += verticalSpacingBetweenEachLine;
		drawText(x, y, COLOR_WHITE, "F9 = Read this help text for an extra 15 seconds", largeSizeFont);
		y += verticalSpacingBetweenEachLine;
		drawText(x, y, COLOR_WHITE, "F10 = Disable Chat Interface", largeSizeFont);
		y += verticalSpacingBetweenEachLine;
		drawText(x, y, COLOR_WHITE, ">>>>>>>>>>> F11 = Server Browser <<<<<<<<<<< Click this to join a game/server.", largeSizeFont);
		y += verticalSpacingBetweenEachLine;
		drawText(x, y, COLOR_WHITE, "T = Chat", largeSizeFont);
		y += verticalSpacingBetweenEachLine;
		y += verticalSpacingBetweenEachLine;
		drawText(x, y, COLOR_RED, "Server Hosting Instructions", largeSizeFont);
		y += verticalSpacingBetweenEachLine;
		drawText(x, y, COLOR_WHITE, "If you want to host, use the Halo 3 menu by using your arrow keys, and the Enter, Escape, and X buttons on your keyboard.", largeSizeFont);
		y += verticalSpacingBetweenEachLine;
		drawText(x, y, COLOR_WHITE, "You need to port forward: 11775 and 11774.", largeSizeFont);
		y += verticalSpacingBetweenEachLine;
		y += verticalSpacingBetweenEachLine;
		drawText(x, y, COLOR_RED, "Common Bugs", largeSizeFont);
		y += verticalSpacingBetweenEachLine;
		drawText(x, y, COLOR_WHITE, "1. If your key bindings are messed up, reset your key bindings to default in settings.", largeSizeFont);
		y += verticalSpacingBetweenEachLine;
		drawText(x, y, COLOR_WHITE, "2. If the game is too bright, reset your NVidia control panel settings.", largeSizeFont);
		y += verticalSpacingBetweenEachLine;
		drawText(x, y, COLOR_WHITE, "3. If server browser doesn't work, go to http://vicelio.github.io/menu/, open the console (F1 key or ` key) and type: connect <ip address>", largeSizeFont);
		y += verticalSpacingBetweenEachLine;
	}
}

void DirectXHook::drawChatInterface()
{
	auto& console = GameConsole::Instance();
	if ((console.getMsSinceLastConsoleOpen() > 10000 && !console.showChat && !console.showConsole) || (GetAsyncKeyState(VK_TAB) & 0x8000 && !console.showChat && !console.showConsole))
	{
		return;
	}

	int x = (int)(0.05 * *horizontalRes);
	int y = (int)(0.65 * *verticalRes);
	int inputTextBoxWidth = (int)(0.5 * *horizontalRes);
	int inputTextBoxHeight = normalSizeFontHeight + (int)(0.769 * normalSizeFontHeight);
	int horizontalSpacing = (int)(0.012 * inputTextBoxWidth);
	int verticalSpacingBetweenEachLine = (int)(0.154 * normalSizeFontHeight);
	int verticalSpacingBetweenLinesAndInputBox = (int)(1.8 * normalSizeFontHeight);
	int verticalSpacingBetweenTopOfInputBoxAndFont = (inputTextBoxHeight - normalSizeFontHeight) / 2;
	size_t maxCharsPerLine = 105;

	if (console.showChat)
	{
		int tempX = x;

		std::string helpText = "Press ` or F1 to open console.";
		drawText(tempX + horizontalSpacing, y + verticalSpacingBetweenTopOfInputBoxAndFont, COLOR_WHITE, helpText.c_str(), normalSizeFont);
	}

	y -= verticalSpacingBetweenLinesAndInputBox;

	if (console.showChat || console.showConsole)
	{
		// Display current input
		drawBox(x, y, inputTextBoxWidth, inputTextBoxHeight, COLOR_WHITE, COLOR_BLACK);
		drawText(x + horizontalSpacing, y + verticalSpacingBetweenTopOfInputBoxAndFont, COLOR_WHITE, console.currentInput.currentInput.c_str(), normalSizeFont);

		// START: Line showing where the user currently is in the input field.
		if (console.getMsSinceLastConsoleBlink() > 300)
		{
			console.consoleBlinking = !console.consoleBlinking;
			console.lastTimeConsoleBlink = GetTickCount();
		}
		if (!console.consoleBlinking)
		{
			std::string currentInput = console.currentInput.currentInput;
			char currentChar;
			int width = 0;
			if (currentInput.length() > 0) {
				currentChar = currentInput[console.currentInput.currentPointerIndex];
				width = getTextWidth(currentInput.substr(0, console.currentInput.currentPointerIndex).c_str(), normalSizeFont) - 3;
			}
			else
			{
				width = -3;
			}
			drawText(x + horizontalSpacing + width, y + verticalSpacingBetweenTopOfInputBoxAndFont, COLOR_WHITE, "|", normalSizeFont);
		}
		// END: Line showing where the user currently is in the input field.
	}

	y -= verticalSpacingBetweenLinesAndInputBox;

	// Draw text from chat or console
	for (int i = console.selectedQueue->startIndexForScrolling; i < console.selectedQueue->numOfLinesToShow + console.selectedQueue->startIndexForScrolling; i++)
	{
		std::string line = console.selectedQueue->queue.at(i);

		if (line.size() > maxCharsPerLine)
		{
			std::vector<std::string> linesWrapped = std::vector < std::string > {};

			for (size_t i = 0; i < line.size(); i += maxCharsPerLine)
			{
				linesWrapped.push_back(line.substr(i, maxCharsPerLine));
			}

			for (int i = linesWrapped.size() - 1; i >= 0; i--)
			{
				drawText(x, y, COLOR_WHITE, linesWrapped.at(i).c_str(), normalSizeFont);
				y -= normalSizeFontHeight + verticalSpacingBetweenEachLine;
			}
		}
		else
		{
			drawText(x, y, COLOR_WHITE, line.c_str(), normalSizeFont);
			y -= normalSizeFontHeight + verticalSpacingBetweenEachLine;
		}
	}
}

void DirectXHook::hookDirectX()
{
	horizontalRes = (uint32_t*)0x2301D08;
	verticalRes = (uint32_t*)0x2301D0C;

	uint32_t* directXVTable = **((uint32_t***)0x50DADDC);	// d3d9 interface ptr
	origEndScenePtr = (HRESULT(__stdcall *) (LPDIRECT3DDEVICE9)) directXVTable[42];
	origDrawIndexedPrimitivePtr = (HRESULT(__stdcall *) (LPDIRECT3DDEVICE9, D3DPRIMITIVETYPE, INT, UINT, UINT, UINT, UINT)) directXVTable[82];

	DetourRestoreAfterWith();
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach((PVOID*)&origEndScenePtr, &DirectXHook::hookedEndScene); // redirect origEndScenePtr to newEndScene

	if (DetourTransactionCommit() != NO_ERROR)
	{
		OutputDebugString("DirectX EndScene hook failed.");
	}

	DetourRestoreAfterWith();
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach((PVOID*)&origDrawIndexedPrimitivePtr, &DirectXHook::hookedDrawIndexedPrimitive); // redirect DrawIndexedPrimitive to newDrawIndexedPrimitive

	if (DetourTransactionCommit() != NO_ERROR)
	{
		OutputDebugString("DirectX DrawIndexedPrimitive hook failed.");
	}
}

void DirectXHook::drawText(int x, int y, DWORD color, const char* text, LPD3DXFONT pFont)
{
	RECT shadow1;
	RECT shadow2;
	RECT shadow3;
	RECT shadow4;
	SetRect(&shadow1, x + 1, y + 1, x + 1, y + 1);
	SetRect(&shadow2, x - 1, y + 1, x - 1, y + 1);
	SetRect(&shadow3, x + 1, y - 1, x + 1, y - 1);
	SetRect(&shadow4, x - 1, y - 1, x - 1, y - 1);
	pFont->DrawText(NULL, text, -1, &shadow1, DT_LEFT | DT_NOCLIP, D3DCOLOR_XRGB(0, 0, 0));
	pFont->DrawText(NULL, text, -1, &shadow2, DT_LEFT | DT_NOCLIP, D3DCOLOR_XRGB(0, 0, 0));
	pFont->DrawText(NULL, text, -1, &shadow3, DT_LEFT | DT_NOCLIP, D3DCOLOR_XRGB(0, 0, 0));
	pFont->DrawText(NULL, text, -1, &shadow4, DT_LEFT | DT_NOCLIP, D3DCOLOR_XRGB(0, 0, 0));

	RECT rect;
	SetRect(&rect, x, y, x, y);
	pFont->DrawText(NULL, text, -1, &rect, DT_LEFT | DT_NOCLIP, color);
}

void DirectXHook::drawRect(int x, int y, int width, int height, DWORD Color)
{
	D3DRECT rec = { x, y, x + width, y + height };
	pDevice->Clear(1, &rec, D3DCLEAR_TARGET, Color, 0, 0);
}

void DirectXHook::drawHorizontalLine(int x, int y, int width, D3DCOLOR Color)
{
	drawRect(x, y, width, 1, Color);
}

void DirectXHook::drawVerticalLine(int x, int y, int height, D3DCOLOR Color)
{
	drawRect(x, y, 1, height, Color);
}

void DirectXHook::drawBox(int x, int y, int width, int height, D3DCOLOR BorderColor, D3DCOLOR FillColor)
{
	drawRect(x, y, width, height, FillColor);
	drawHorizontalLine(x, y, width, BorderColor);
	drawVerticalLine(x, y, height, BorderColor);
	drawVerticalLine(x + width, y, height, BorderColor);
	drawHorizontalLine(x, y + height, width, BorderColor);
}

int DirectXHook::centerTextHorizontally(const char* text, int x, int width, LPD3DXFONT pFont)
{
	return x + (width - getTextWidth(text, pFont)) / 2;
}

void DirectXHook::initFontsIfRequired()
{
	normalSizeFontHeight = (int)(0.017 * *verticalRes);
	largeSizeFontHeight = (int)(0.034 * *verticalRes); 

	if (!normalSizeFont || normalSizeFontHeight != normalSizeCurrentFontHeight) {
		if (normalSizeFont)
		{
			normalSizeFont->Release();
		}

		D3DXCreateFont(pDevice, normalSizeFontHeight, 0, FW_NORMAL, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Verdana", &normalSizeFont);
		normalSizeCurrentFontHeight = normalSizeFontHeight;
	}

	if (!largeSizeFont || largeSizeFontHeight != largeSizeCurrentFontHeight) {
		if (largeSizeFont)
		{
			largeSizeFont->Release();
		}

		D3DXCreateFont(pDevice, largeSizeFontHeight, 0, FW_NORMAL, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Tahoma", &largeSizeFont);
		largeSizeCurrentFontHeight = largeSizeFontHeight;
	}
}
