#pragma once

#include "../DLLDefs.h"
#include "Sound.h"
#include "AudioDataSource.h"

#include <string>

namespace glib
{
	class SoundManagerImpl;

	class SoundManager
	{
	private:
		SoundManagerImpl* impl;
	public:
		SoundManager();
		~SoundManager();

		GLIB_API Sound* CreateSound(AudioDataSource* source);
		GLIB_API Sound* CreateSound(const std::string& sourceName);
		GLIB_API Sound* CreatePersistantSound(AudioDataSource* source);
		GLIB_API Sound* CreatePersistantSound(const std::string& sourceName);

		GLIB_API AudioDataSource* CreateSourceFromFile(const std::string& name, const std::string& path);
		GLIB_API AudioDataSource* CreateSourceFromPackage(const std::string& name, const std::string& packagePath, const std::string& path);

		GLIB_API void ChangeOutputDevice(const std::string& device); // !!! Invalidates all previously active or created sounds and loaded data! !!!

		GLIB_API void SetGeneralVolume(float volume);

		void Update();
	};
}