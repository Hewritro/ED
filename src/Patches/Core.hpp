#pragma once

namespace Patches
{
	namespace Core
	{
		void ApplyAll();

		// A function that should be called when a map finishes loading.
		// mapPath is the path to the map file without the extension, e.g. "maps\mainmenu".
		typedef std::function<void(const char *mapPath)> MapLoadedCallback;

		// Registers a function to be called when a map finishes loading.
		void OnMapLoaded(MapLoadedCallback callback);
	}
}