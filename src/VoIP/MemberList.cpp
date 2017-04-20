#include "MemberList.hpp"
#include "../Patches/Ui.hpp"
#include "../Modules/ModuleVoIP.hpp"
#include "../Modules/ModulePlayer.hpp"

void MemberList::ShowPlayerTalkEvent(std::string name)
{
	auto &modulePlayer = Modules::ModulePlayer::Instance();
	if (name != modulePlayer.VarPlayerName->ValueString)
	{
		memberList.push_back(name);
		Patches::Ui::SetSpeakingPlayer(memberList[0]);
		Patches::Ui::ToggleSpeakingPlayer(true);
	}
	else
		Patches::Ui::SetVoiceChatIcon(Patches::Ui::VoiceChatIcon::Speaking);

}

void MemberList::HidePlayerTalkEvent(std::string name)
{
	auto &modulePlayer = Modules::ModulePlayer::Instance();
	if (name == modulePlayer.VarPlayerName->ValueString)
	{
		auto &modulesVoIP = Modules::ModuleVoIP::Instance();
		if (modulesVoIP.VarVoIPPushToTalk->ValueInt == 1)
			Patches::Ui::SetVoiceChatIcon(Patches::Ui::VoiceChatIcon::PushToTalk);
		else
			Patches::Ui::SetVoiceChatIcon(Patches::Ui::VoiceChatIcon::Available);
	}
	else
	{
		for (size_t i = 0; i < memberList.size(); i++)
		{
			if (memberList.at(i) == name)
			{
				auto &modulePlayer = Modules::ModulePlayer::Instance();

				memberList.erase(memberList.begin() + i);

				if (memberList.size() < 1)
					Patches::Ui::ToggleSpeakingPlayer(false);
				else
					Patches::Ui::SetSpeakingPlayer(memberList[0]);
				return;
			}
		}
	}
}