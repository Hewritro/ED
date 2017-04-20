#pragma warning( disable : 4996)

#include "VirtualKeyboard.hpp"

#include "../ElDorito.hpp"
#include "../Patch.hpp"

#include <ShlObj.h>
#include <windows.h>
#include "../resource.h"

namespace
{
	class VirtualKeyboard;
	VirtualKeyboard *CreateKeyboard(const char *file, int unk1, int unk2, int unk3, int unk4, int unk5, int maxLength, int unk7, int unk8);
	void __fastcall SetKeyboardDefaultValue(VirtualKeyboard *keyboard, void *unused, wchar_t *newDefaultValue);
	void __fastcall SetKeyboardTitle(VirtualKeyboard *keyboard, void *unused, wchar_t *newTitle);
	void __fastcall SetKeyboardDescription(VirtualKeyboard *keyboard, void *unused, wchar_t *newDescription);
	bool ShowKeyboard(VirtualKeyboard *keyboard, const char *file, int line);
}

namespace Patches
{
	namespace VirtualKeyboard
	{
		void ApplyAll()
		{
			// Implement keyboard stubs
			Hook(0xE1840, CreateKeyboard).Apply();
			Hook(0xE19A0, SetKeyboardDefaultValue).Apply();
			Hook(0xE19D0, SetKeyboardTitle).Apply();
			Hook(0xE19B0, SetKeyboardDescription).Apply();
			Hook(0x1A8F40, ShowKeyboard).Apply();
		}
	}
}

namespace
{
	// Based off of H3 Epsilon's code
	class VirtualKeyboard
	{
	public:
		VirtualKeyboard(const char *file, int line, int arg2, int arg3, int arg4, int arg5, int maxLength, int arg7, int arg8)
		{
			unk4 = 0;
			unk6 = 0;
			unk8 = 0;
			unkC = 0;
			unk10 = 0;
			unk14 = 0;
			memset(text, 0, sizeof(text));
			memset(defaultValue, 0, sizeof(defaultValue));
			memset(title, 0, sizeof(title));
			memset(description, 0, sizeof(description));
			this->maxLength = Utils::Clamp(maxLength, 0, 0x100);
		}

		virtual int Stub0() { return 0; }
		virtual int Stub4() { return 0; }
		virtual int Stub8() { return 0; }
		virtual int StubC() { return 0; }
		virtual int Stub10() { return 0; }
		virtual int Stub14() { return 0; }
		virtual int Stub18() { return 0; }
		virtual void Reset() { unk6 = 0; }
		virtual int Stub20() { return 0; }

		void SetDefaultValue(wchar_t *newDefaultValue)
		{
			wcscpy_s(defaultValue, newDefaultValue);
		}

		void SetTitle(wchar_t *newTitle)
		{
			wcscpy_s(title, newTitle);
		}

		void SetDescription(wchar_t *newDescription)
		{
			wcscpy_s(description, newDescription);
		}

		int16_t unk4;
		int16_t unk6;
		int32_t unk8;
		int32_t unkC;
		int32_t unk10;
		int32_t unk14;
		wchar_t text[0x100];
		wchar_t defaultValue[0x100];
		wchar_t title[0x40];
		wchar_t description[0x100];
		int32_t maxLength;
	};

	VirtualKeyboard *CreateKeyboard(const char *file, int line, int unk2, int unk3, int unk4, int unk5, int maxLength, int unk7, int unk8)
	{
		// Does this need to be allocated properly by using the game's malloc?
		return new VirtualKeyboard(file, line, unk2, unk3, unk4, unk5, maxLength, unk7, unk8);
	}

	// Technically thiscall
	void __fastcall SetKeyboardDefaultValue(VirtualKeyboard *keyboard, void *unused, wchar_t *newDefaultValue)
	{
		keyboard->SetDefaultValue(newDefaultValue);
	}

	// Technically thiscall
	void __fastcall SetKeyboardTitle(VirtualKeyboard *keyboard, void *unused, wchar_t *newTitle)
	{
		keyboard->SetTitle(newTitle);
	}

	// Technically thiscall
	void __fastcall SetKeyboardDescription(VirtualKeyboard *keyboard, void *unused, wchar_t *newDescription)
	{
		keyboard->SetDescription(newDescription);
	}

	char* inputLineText;
	INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (uMsg == WM_COMMAND && wParam == 1)
		{
			HWND textEditHandle = GetDlgItem(hwndDlg, 0);
			int len = SendMessage(textEditHandle, WM_GETTEXTLENGTH, 0, 0);
			inputLineText = new char[len + 1];
			SendMessage(textEditHandle, WM_GETTEXT, (WPARAM)len + 1, (LPARAM)inputLineText);
			EndDialog(hwndDlg, 0);
			return true;
		}
		return false;
	}

	static wchar_t* charToWChar(const char* text)
	{
		size_t size = strlen(text) + 1;
		wchar_t* wa = new wchar_t[size];
		mbstowcs(wa, text, size);
		return wa;
	}

	bool ShowKeyboard(VirtualKeyboard *keyboard, const char *file, int line)
	{
		DialogBoxParam(GetModuleHandle("mtndew.dll"), MAKEINTRESOURCE(IDD_DIALOG1), *((HWND*)0x199C014), &DialogProc, 0);
		wcscpy_s(keyboard->text, charToWChar(inputLineText));
		delete inputLineText;
		
		// Not 100% sure on this next line,
		// it's some sort of state value and 4 seems to mean "done"
		// because it makes the Forge code copy the text somewhere
		keyboard->unk6 = 4;
		return true;
	}
}