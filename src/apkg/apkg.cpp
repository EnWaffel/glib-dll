#include "glib/apkg/apkg.h"
#include <fstream>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;
using namespace glib::apkg;

template<typename T>
static void Write(std::ofstream& o, T v)
{
	o.write(reinterpret_cast<const char*>(&v), sizeof(v));
}

static void WriteU8(std::ofstream& o, uint8_t v)
{
	Write(o, v);
}

static void WriteU64(std::ofstream& o, uint64_t v)
{
	Write(o, v);
}

static void WriteU32(std::ofstream& o, uint32_t v)
{
	Write(o, v);
}

static void WriteU16(std::ofstream& o, uint16_t v)
{
	Write(o, v);
}

static void Write8(std::ofstream& o, int8_t v)
{
	Write(o, v);
}

static std::vector<std::string> GetFiles(const std::string& path, bool doRecur)
{
	std::vector<std::string> files;

	try
	{
		for (const auto& v : fs::directory_iterator(path))
		{
			if (v.is_directory() && doRecur)
			{
				std::vector<std::string> f = GetFiles(v.path().string(), true);
				files.insert(std::end(files), std::begin(f), std::end(f));
			}
			else
			{
				files.push_back(fs::relative(v.path()).string());
			}
		}
	}
	catch (std::exception& e)
	{
		std::cout << "glib (apkg) Error: Failed to pack: \"" << path << "\" (" << e.what() << ")" << std::endl;
	}

	return files;
}

/**
* -1 = invalid file size
* -2 = invalid format
* -3 = incompatible format version
*/
static int ParseHeader(std::ifstream& in)
{
	std::streampos fSize = 0;
	fSize = in.tellg();
	in.seekg(0, std::ios::end);
	fSize = in.tellg() - fSize;
	in.seekg(0, std::ios::beg);

	if (fSize < 9)
	{
		return -1;
	}

	char buf[4];
	in.read(buf, sizeof(buf));

	if (buf[0] != 'A' || buf[1] != 'P' || buf[2] != 'K' || buf[3] != 'G')
	{
		return -2;
	}

	uint8_t v;
	in.read(reinterpret_cast<char*>(&v), sizeof(v));
	if (v != APKG_FORMAT_VERSION)
	{
		return -3;
	}

	uint32_t filesNum = 0;
	in.read(reinterpret_cast<char*>(&filesNum), sizeof(filesNum));

	return filesNum;
}

int glib::apkg::PackDir(const std::string& path, const std::string& outputFile, bool recur)
{
	return PackFiles(GetFiles(path, recur), outputFile);
}

int glib::apkg::PackFiles(const std::vector<std::string>& files, const std::string& outputFile)
{
	std::ofstream o(outputFile, std::ios::binary);
	if (!o.is_open())
	{
		return -1;
	}

	// Header

	Write8(o, 'A');
	Write8(o, 'P');
	Write8(o, 'K');
	Write8(o, 'G');

	WriteU8(o, APKG_FORMAT_VERSION);
	WriteU32(o, files.size());

	// Files

	for (const std::string& s : files)
	{
		std::string fileName = s;
		WriteU16(o, fileName.size());
		for (int8_t c : fileName) Write8(o, c);

		std::ifstream in(s, std::ios::binary);
		if (!in.is_open())
		{
			return -2;
		}

		std::streampos fSize = 0;
		fSize = in.tellg();
		in.seekg(0, std::ios::end);
		fSize = in.tellg() - fSize;
		in.seekg(0, std::ios::beg);

		WriteU64(o, fSize);

		int8_t* buf = new int8_t[fSize];

		in.read(reinterpret_cast<char*>(buf), fSize);
		o.write(reinterpret_cast<char*>(buf), fSize);

		delete[] buf;

		in.close();
	}

	o.close();

	return 1;
}

FileTable glib::apkg::Unpack(const std::string& path)
{
	std::ifstream in(path, std::ios::binary);

	uint32_t filesNum = ParseHeader(in);
	if (!filesNum)
	{
		in.close();
		if (filesNum == -1)
		{
			return { -3 };
		}
		else if (filesNum == -2)
		{
			return { -4 };
		}
		if (filesNum == -3)
		{
			return { -2 };
		}
	}

	FileTable table{};
	table.result = 1;

	for (uint32_t i = 0; i < filesNum; i++)
	{
		FileData data{};

		uint16_t nameLen = 0;
		in.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));
		int8_t* nameBuf = new int8_t[nameLen];
		in.read(reinterpret_cast<char*>(nameBuf), nameLen);

		std::string name;
		for (uint16_t j = 0; j < nameLen; j++) name.push_back(nameBuf[j]);
		data.name = name;

		uint64_t fileSize = 0;
		in.read(reinterpret_cast<char*>(&fileSize), sizeof(fileSize));

		int8_t* dataBuf = new int8_t[fileSize];
		in.read(reinterpret_cast<char*>(dataBuf), fileSize);

		data.buf = dataBuf;
		data.bufLen = fileSize;

		table.files.insert({ data.name, data });
	}

	in.close();
	return table;
}

FileData glib::apkg::UnpackOnce(const std::string& path, const std::string& fileName)
{
	std::ifstream in(path, std::ios::binary);

	uint32_t filesNum = ParseHeader(in);
	if (!filesNum)
	{
		in.close();
		return { nullptr };
	}

	FileData data{};
	data.name = fileName;

	for (uint32_t i = 0; i < filesNum; i++)
	{
		uint16_t nameLen = 0;
		in.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));
		int8_t* nameBuf = new int8_t[nameLen];
		in.read(reinterpret_cast<char*>(nameBuf), nameLen);

		std::string name;
		for (uint16_t j = 0; j < nameLen; j++) name.push_back(nameBuf[j]);

		uint64_t fileSize = 0;
		in.read(reinterpret_cast<char*>(&fileSize), sizeof(fileSize));

		if (strcmp(fileName.c_str(), name.c_str()) != 0)
		{
			in.ignore(fileSize);
			continue;
		}

		int8_t* dataBuf = new int8_t[fileSize];
		in.read(reinterpret_cast<char*>(dataBuf), fileSize);

		data.buf = dataBuf;
		data.bufLen = fileSize;
	}

	in.close();
	return data;
}

void glib::apkg::FreeTable(const FileTable& table)
{
	for (const auto& v : table.files)
	{
		delete[] v.second.buf;
	}
}
