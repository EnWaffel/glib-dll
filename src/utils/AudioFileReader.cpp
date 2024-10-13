#include "glib/utils/AudioFileReader.h"
#include "glib/apkg/apkg.h"

#include <filesystem>
#include <AudioFile.h>
#include <stb_vorbis.c>

namespace fs = std::filesystem;
using namespace glib;

static const std::string GetFileExt(const std::string& path)
{
    fs::path filePath(path);
    std::string fileExt = filePath.extension().string();

    if (!fileExt.empty()) {
        if (fileExt[0] == '.') {
            fileExt = fileExt.substr(1);
        }
        return fileExt;
    }
    else {
        return "";
    }
}

static const std::string ToLowercase(const std::string& str)
{
    std::string result = str;
    for (char& c : result) {
        c = std::tolower(c);
    }
    return result;
}

AudioData glib::AudioFileReader::ReadFile(const std::string& path)
{
	AudioData data{};
    const std::string ext = ToLowercase(GetFileExt(path));

    if (ext == "wav" || ext == "aiff" || ext == "aif")
    {
        AudioFile<short> f;
        if (!f.load(path))
        {
            return { nullptr, 0, 3000, 0 };
        }

        data.sampleRate = f.getSampleRate();
        data.channels = f.getNumChannels();
        data.depth = f.getBitDepth();

        unsigned long long numSamples = f.getNumSamplesPerChannel();
        short* buf = new short[numSamples * data.channels];
        data.size = numSamples * 2;

        data.buf = buf;
    }
    else if (ext == "ogg")
    {
        FILE* f;
        fopen_s(&f, path.c_str(), "rb");
        if (!f)
        {
            return { nullptr, 0, 3000, 0 };
        }

        int err = VORBIS__no_error;
        stb_vorbis* v = stb_vorbis_open_file(f, true, &err, NULL);
        if (err != VORBIS__no_error)
        {
            return { nullptr, 0, 3000, 0 };
        }

        stb_vorbis_info i = stb_vorbis_get_info(v);
        data.channels = i.channels;
        data.sampleRate = i.sample_rate;
        data.depth = 16;

        data.size = stb_vorbis_stream_length_in_samples(v) * data.channels * 2;
        short* buf = new short[data.size];
        data.buf = buf;

        stb_vorbis_get_samples_short_interleaved(v, data.channels, buf, data.size);

        stb_vorbis_close(v);
        fclose(f);
    }
    else
    {
        return { nullptr, 0, 1000, 0 };
    }

	return data;
}

AudioData glib::AudioFileReader::ReadPackage(const std::string& packagePath, const std::string& path)
{
    AudioData data{};
    const std::string ext = ToLowercase(GetFileExt(path));

    apkg::FileData fd = apkg::UnpackOnce(packagePath, path);
    if (fd.bufLen <= 0)
    {
        return { nullptr, 0, 3000, 0 };
    }

    if (ext == "wav" || ext == "aiff" || ext == "aif")
    {
        std::vector<uint8_t> d;
        for (int i = 0; i < fd.bufLen; i++) d.push_back(((uint8_t*)fd.buf)[i]);
        AudioFile<short> f;
        if (!f.loadFromMemory(d))
        {
            return { nullptr, 0, 3000, 0 };
        }

        data.sampleRate = f.getSampleRate();
        data.channels = f.getNumChannels();
        data.depth = f.getBitDepth();

        unsigned long long numSamples = f.getNumSamplesPerChannel();
        short* buf = new short[numSamples * data.channels];
        data.size = numSamples * 2;

        data.buf = buf;
    }
    else if (ext == "ogg")
    {
        int err = VORBIS__no_error;
        stb_vorbis* v = stb_vorbis_open_memory((uint8_t*)fd.buf, fd.bufLen, &err, NULL);
        if (err != VORBIS__no_error)
        {
            return { nullptr, 0, 3000, 0 };
        }

        stb_vorbis_info i = stb_vorbis_get_info(v);
        data.channels = i.channels;
        data.sampleRate = i.sample_rate;
        data.depth = 16;

        data.size = stb_vorbis_stream_length_in_samples(v) * data.channels * 2;
        short* buf = new short[data.size];
        data.buf = buf;

        stb_vorbis_get_samples_short_interleaved(v, data.channels, buf, data.size);

        stb_vorbis_close(v);
    }
    else
    {
        return { nullptr, 0, 1000, 0 };
    }

    return data;
}
