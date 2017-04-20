#pragma once

namespace Patches
{
	namespace Logging
	{
		void ApplyAll();
		void EnableNetworkLog(bool enable);
		void EnableSslLog(bool enable);
		void EnableUiLog(bool enable);
		void EnableGame1Log(bool enable);
		void EnableGame2Log(bool enable);
		void EnablePacketsLog(bool enable);
	}
}