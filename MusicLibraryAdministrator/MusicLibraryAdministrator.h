#pragma once
#include <string>
#include <fstream>
#include "MusicFile.h"

using namespace std;



class MusicLibraryAdministrator
{
public:
	struct Settings
	{
		string watchfolder;
		string librarypath;
	};
	static Settings* settings;

	MusicLibraryAdministrator();
	~MusicLibraryAdministrator();
	
	void Execute();
	static void Write(string);
	static QString MusicLibraryAdministrator::JoinPath(QString, QString);

private:
	static fstream* logfile;
	static string logfileName;
	static string settingsFileName;
	static int movedFilesNumber;
	static chrono::time_point<chrono::high_resolution_clock> start;

	void DetermineTargetPath(MusicFile*);
};
