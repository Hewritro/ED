#pragma once
#include "Tags.hpp"

namespace Blam
{
	namespace Tags
	{
		using Blam::Tags::TagBlock;
		using Blam::Tags::TagData;
		using Blam::Tags::StringID;

		struct MultilingualUnicodeStringList : Tag<'unic'>
		{
			struct LocalizedString;

			TagBlock<LocalizedString> Strings;
			TagData<int8_t> Data;

			struct LocalizedString
			{
				StringID StringID;
				char StringIDStr[32]; //This should really be stored as a string.
				int32_t Offsets[12];
			};
			TAG_STRUCT_SIZE_ASSERT(LocalizedString, 0x54);
		};
	}
}
