#include "MusicLibraryAdministrator.h"
#include "FileOperations.h"
#include <iostream>
#include <string>
#include <QtCore>
#include <vector>
#include <Windows.h>
#include <thread>

using namespace std;

fstream* MusicLibraryAdministrator::logfile = new fstream();
string MusicLibraryAdministrator::logfileName = "MusicLibraryAdministrator.log";
string MusicLibraryAdministrator::settingsFileName = "MusicLibraryAdministrator.settings.json";
MusicLibraryAdministrator::Settings* MusicLibraryAdministrator::settings;
int MusicLibraryAdministrator::movedFilesNumber = 0;
chrono::time_point<chrono::high_resolution_clock> MusicLibraryAdministrator::start;

MusicLibraryAdministrator::MusicLibraryAdministrator()
{
	MusicLibraryAdministrator::logfile->open(MusicLibraryAdministrator::logfileName, fstream::out);
	MusicLibraryAdministrator::Write("Starting MusicLibraryAdministrator...");
	start = chrono::high_resolution_clock::now();

	MusicLibraryAdministrator::settings = FileOperations::ReadSettings(settingsFileName);
}

MusicLibraryAdministrator::~MusicLibraryAdministrator()
{
	auto end = chrono::high_resolution_clock::now();

	std::chrono::duration<double> diff = end - MusicLibraryAdministrator::start;
	MusicLibraryAdministrator::Write("It took " + to_string(diff.count()) + "s to move " + to_string(MusicLibraryAdministrator::movedFilesNumber) + " files.");
	MusicLibraryAdministrator::Write("Closing MusicLibraryAdministrator.");
	logfile->close();
}

void MusicLibraryAdministrator::Execute()
{
	QDir directory(settings->watchfolder.c_str());
	QStringList namefilters;
	namefilters << "*.mp3" << "*.m4a";
	directory.setNameFilters(namefilters);
	directory.setFilter(QDir::Files);
	
	QStringList filesInDirectory = directory.entryList();
	while (!filesInDirectory.empty())
	{
		MusicLibraryAdministrator::Write("--------------------------------------------------------------------------------------------------");
		QString filename = filesInDirectory[0];
		QString filepath = MusicLibraryAdministrator::JoinPath(settings->watchfolder.c_str(), filename);
		MusicFile* file = FileOperations::ExtractMetadata(filepath);
		FileOperations::Move(file);
		MusicLibraryAdministrator::movedFilesNumber++;
		directory.refresh();
		filesInDirectory = directory.entryList();
		MusicLibraryAdministrator::Write("--------------------------------------------------------------------------------------------------");
	}
}

void MusicLibraryAdministrator::Write(string text)
{
	cout << text << endl;
	*logfile << text << endl;
}

QString MusicLibraryAdministrator::JoinPath(QString path1, QString path2)
{
	while (path1.startsWith("\\"))
	{
		path1.remove(0, 1);
	}
	while (path1.endsWith("\\"))
	{
		path1.remove(path1.length() - 1, 1);
	}
	while (path2.startsWith("\\"))
	{
		path2.remove(0, 1);
	}
	while (path2.endsWith("\\"))
	{
		path2.remove(path1.length() - 1, 1);
	}

	return path1 + "\\" + path2;
}


