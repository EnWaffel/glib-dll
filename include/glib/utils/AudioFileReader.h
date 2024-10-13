#pragma once

#include "../DLLDefs.h"

#include <string>

namespace glib
{
	struct AudioData
	{
		const void* buf;
		unsigned int sampleRate;
		unsigned int depth;
		unsigned int channels;
		unsigned long long size;
	};

	class AudioFileReader
	{
	public:
		GLIB_API static AudioData ReadFile(const std::string& path);
		GLIB_API static AudioData ReadPackage(const std::string& packagePath, const std::string& path);
	};
}