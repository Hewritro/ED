#pragma once

#include <string>

namespace Patches
{
	namespace Ui
	{
		void ApplyAll();
		void ApplyMapNameFixes();
		void ApplyUIResolution();

		void Tick();

		extern bool DialogShow; // todo: put this somewhere better
		extern unsigned int DialogStringId;
		extern int DialogArg1; // todo: figure out a better name for this
		extern int DialogFlags;
		extern unsigned int DialogParentStringId;
		extern void* UIData;

		enum VoiceChatIcon
		{
			None,
			Speaking,
			Available,
			Unavailable,
			PushToTalk
		};

		void ToggleSpeakingPlayer(bool newSomeoneSpeaking);
		void SetSpeakingPlayer(std::string speakingPlayer);

		void SetVoiceChatIcon(VoiceChatIcon newIcon);
		void UpdateVoiceChatHUD();
		void ApplyAfterTagsLoaded();
	}
}