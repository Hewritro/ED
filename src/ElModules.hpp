#pragma once

#include "Modules/ModuleBase.hpp"
#include "Modules/ModuleGame.hpp"
#include "Modules/ModulePlayer.hpp"
#include "Modules/ModuleServer.hpp"
#include "Modules/ModuleSettings.hpp"
#include "Modules/ModuleCamera.hpp"
#include "Modules/ModuleInput.hpp"
#include "Modules/ModuleTime.hpp"
#include "Modules/ModuleGraphics.hpp"

namespace Modules
{
	class ElModules : public Utils::Singleton<ElModules>, public ModuleBase
	{
	public:
		ElModules();

	private:
	};
}