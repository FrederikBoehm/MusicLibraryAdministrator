#pragma once
#include "MusicFile.h"
#include "MusicLibraryAdministrator.h"
#include <Windows.h>
#include <strsafe.h>

static class FileOperations {
public:
	static MusicFile* ExtractMetadata(QString);
	static MusicLibraryAdministrator::Settings* ReadSettings(string);

	static void Move(MusicFile*);
	static void CreateDirectoryHelper(MusicFile*);
	static void ErrorExit(LPTSTR);

private:
};