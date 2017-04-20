#pragma once

#include "ModuleBase.hpp"
  
namespace Blam
{
	namespace Input
	{
		struct BindingsTable;
	}
}

namespace Modules
{
	class ModuleInput : public Utils::Singleton<ModuleInput>, public ModuleBase
	{
	public:
		Command* VarInputRawInput;
		Command* VarControllerSensitivityX;
		Command* VarControllerSensitivityY;
  
		Command* CommandBindControllerAction;
		Command* CommandBindKeyboardAction;
		Command* VarInputControllerPort;

		ModuleInput();

		static Blam::Input::BindingsTable *GetBindings();
		static void UpdateBindings();
		std::string ExportBindings() const;
	};
}