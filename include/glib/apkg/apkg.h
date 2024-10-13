#pragma once

#include "../DLLDefs.h"

#define APKG_FORMAT_VERSION 1

#define APKG_CHECK_RESULT_RET(table) if (!table.result) return;
#define APKG_CHECK_RESULT_RETfd(fileData) if (!fileData.buf) return;
#define APKG_CHECK_RESULT_RETi(table, i) if (!table.result) return i;
#define APKG_CHECK_RESULT_RETfdi(fileData, i) if (!fileData.buf) return i;

#include <map>
#include <vector>
#include <string>

/**
* Structure:
*
*	Offset | Size | Datatype | Value | Description
*
*	Header:
*
*	0	   | 4	  | int8	 | APKG  | apkg file declaration
*   4      | 1    | uint8    | 1     | apkg format version
*	5      | 4    | uint32   | ?     | amount of packed files
*
*	File:
*
*	5 + ...| 2	  |	uint16	 | ?	 | file name length
* 	2	   | ?	  |	int8	 | ?	 | file name
*	2 + ?  | 8    | uint64   | ?     | file size
*	2+?+8  | ?    | int8     | ?     | file data
*
*/

namespace glib
{
	namespace apkg
	{
		struct FileData
		{
			void* buf;
			size_t bufLen;
			std::string name;
		};

		struct FileTable
		{
			int result;
			std::map<std::string, FileData> files;
		};

		/*!
		 \brief Pack a directory and it's sub-directories
		 \param path : Path to the directory
		 \param outputFile : The output file
		 \param recur : When true the directory is packed recursively

		 \return 1 success
		 \return -1 failed to create output file
		 \return -2 failed to access a file
		*/
		GLIB_API int PackDir(const std::string& path, const std::string& outputFile, bool recur = false);

		/*!
		 \brief Pack a list of files
		 \param files : The list of files
		 \param outputFile : The output file

		 \return 1 success
		 \return -1 failed to create output file
		 \return -2 failed to access a file
		*/
		GLIB_API int PackFiles(const std::vector<std::string>& files, const std::string& outputFile);

		/*!
		 \brief Unpack a .apkg file
		 \param path : Path to the file

		 \return 1 success
		 \return -1 failed to open file
		 \return -2 incompatible format version
		 \return -3 corrupt or broken file
		 \return -4 invalid format
		*/
		GLIB_API FileTable Unpack(const std::string& path);

		/*!
		 \brief Get only the data of the specified file name of a .apkg file. This function is pretty useful if you need to only extract certain files from a big apkg package.
		 \param path : Path to the file
		*/
		GLIB_API FileData UnpackOnce(const std::string& path, const std::string& fileName);

		/*!
		 \brief Frees the buffers of all files in a FileTable
		 \param table : The table to free
		*/
		GLIB_API void FreeTable(const FileTable& table);
	}
}