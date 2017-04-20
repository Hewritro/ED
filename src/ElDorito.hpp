#pragma once

#include <chrono>
#include <map>
#include <vector>
#include <stdint.h>
#include <memory>

#include "Utils/Utils.hpp"
#include "ElModules.hpp"
#include "Pointer.hpp"
#include "Patch.hpp"

class ElDorito : public Utils::Singleton < ElDorito >
{
public:
	bool GameHasMenuShown = false;

	static void SetMainThreadID(size_t ThreadID)
	{
		MainThreadID = ThreadID;
	}
	static size_t GetMainThreadID()
	{
		return MainThreadID;
	}

	static Pointer GetMainTls(size_t Offset = 0);

	ElDorito();
	~ElDorito() = default;

	std::string GetDirectory();

	void Initialize();
	void Tick(const std::chrono::duration<double>& DeltaTile);
	void OnMainMenuShown();
	bool IsHostPlayer();
	std::string GetInstanceName() const { return instanceName; }

private:
	static size_t MainThreadID; // Thread
	bool executeCommandQueue = false;
	static bool(__cdecl * Video_InitD3D)(bool, bool);
	std::string instanceName = "";

	void setWatermarkText(const std::string& Message);
	void killProcessByName(const char *filename, int ourProcessID);
	static bool __cdecl hooked_Video_InitD3D(bool windowless, bool nullRefDevice);
};