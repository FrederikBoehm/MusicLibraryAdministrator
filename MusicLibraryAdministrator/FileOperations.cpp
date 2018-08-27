#include "FileOperations.h"
#include <QtCore>
#include <iostream>
#include "MusicLibraryAdministrator.h"
#include <Windows.h>
#include <strsafe.h>

extern "C"
{
#include <libavformat\avformat.h>
#include <libavutil\avutil.h>
}

using namespace std;

MusicFile* FileOperations::ExtractMetadata(QString filepath)
{
	MusicLibraryAdministrator::Write("Extracting Metadata from file " + filepath.toStdString());
	AVFormatContext *fmt_ctx = NULL;
	AVDictionaryEntry *tag = NULL;
	int ret;

	if ((ret = avformat_open_input(&fmt_ctx, filepath.toStdString().c_str(), NULL, NULL)))
		throw new exception(to_string(ret).c_str());

	MusicFile* musicfile = new MusicFile;
	QFile file(filepath);
	QFileInfo fileInfo(file.fileName());
	QString filename = fileInfo.fileName();
	QString filedirectory = fileInfo.absolutePath();
	musicfile->Dateiname(filename);
	musicfile->Quellpfad(filedirectory.replace("/", "\\"));

	while ((tag = av_dict_get(fmt_ctx->metadata, "", tag, AV_DICT_IGNORE_SUFFIX)))
	{
		QString tagkey(tag->key);
		if (QString::compare(tagkey, "album") == 0)
		{
			musicfile->Album(tag->value);
			MusicLibraryAdministrator::Write(tagkey.toStdString() + ": " + tag->value);
		}
		else if (QString::compare(tagkey, "album_artist") == 0)
		{
			musicfile->Albuminterpret(tag->value);
			MusicLibraryAdministrator::Write(tagkey.toStdString() + ": " + tag->value);
		}
		else if (QString::compare(tagkey, "artist") == 0)
		{
			musicfile->Interpret(tag->value);
			MusicLibraryAdministrator::Write(tagkey.toStdString() + ": " + tag->value);
		}
		else if (QString::compare(tagkey, "composer") == 0)
		{
			musicfile->Komponisten(tag->value);
			MusicLibraryAdministrator::Write(tagkey.toStdString() + ": " + tag->value);
		}
		else if (QString::compare(tagkey, "compilation") == 0)
		{
			musicfile->IstInKompilation(tag->value);
			MusicLibraryAdministrator::Write(tagkey.toStdString() + ": " + tag->value);
		}
		else if (QString::compare(tagkey, "genre") == 0)
		{
			musicfile->Genre(tag->value);
			MusicLibraryAdministrator::Write(tagkey.toStdString() + ": " + tag->value);
		}
		else if (QString::compare(tagkey, "title") == 0)
		{
			musicfile->Titel(tag->value);
			MusicLibraryAdministrator::Write(tagkey.toStdString() + ": " + tag->value);
		}
		else if (QString::compare(tagkey, "track") == 0)
		{
			musicfile->Titelnummer(tag->value);
			MusicLibraryAdministrator::Write(tagkey.toStdString() + ": " + tag->value);
		}
	}
		
	avformat_close_input(&fmt_ctx);

	return musicfile;
}

MusicLibraryAdministrator::Settings* FileOperations::ReadSettings(string settingspath)
{
	MusicLibraryAdministrator::Write("Reading settings from " + settingspath);
	QString fileContent;
	QFile file;
	file.setFileName(settingspath.c_str());
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	fileContent = file.readAll();
	file.close();
	QJsonDocument d = QJsonDocument::fromJson(fileContent.toUtf8());
	QJsonObject jsettings = d.object();

	QJsonValue jwatchfolder = jsettings.value(QString("Watchfolder"));
	QJsonValue jlibrarypath = jsettings.value(QString("Librarypath"));
	
	MusicLibraryAdministrator::Settings* settings = new MusicLibraryAdministrator::Settings;
	settings->watchfolder = jwatchfolder.toString().toStdString();
	settings->librarypath = jlibrarypath.toString().toStdString();

	MusicLibraryAdministrator::Write("Watchfolder: " + settings->watchfolder);
	MusicLibraryAdministrator::Write("Librarypath: " + settings->librarypath);

	return settings;
}

//MusicLibraryAdministrator::Settings* FileOperations::ReadSettings(string settingspath)
//{
//	MusicLibraryAdministrator::Write("Reading settings from " + settingspath);
//	QString fileContent;
//	FILE* file = fopen(settingspath.c_str(), "r, ccs=UTF-(");
//	QTextStream stream(file, QIODevice::ReadOnly);
//	stream.setCodec("UTF-8");
//	fileContent = stream.readAll();
//	QJsonDocument d = QJsonDocument::fromJson(fileContent.toUtf8());
//	QJsonObject jsettings = d.object();
//
//	QJsonValue jwatchfolder = jsettings.value(QString("Watchfolder"));
//	QJsonValue jlibrarypath = jsettings.value(QString("Librarypath"));
//	
//	MusicLibraryAdministrator::Settings* settings = new MusicLibraryAdministrator::Settings;
//	settings->watchfolder = jwatchfolder.toString().toStdString();
//	settings->librarypath = jlibrarypath.toString().toStdString();
//
//	MusicLibraryAdministrator::Write("Watchfolder: " + settings->watchfolder);
//	MusicLibraryAdministrator::Write("Librarypath: " + settings->librarypath);
//
//	return settings;
//}

void FileOperations::Move(MusicFile* file)
{
	FileOperations::CreateDirectoryHelper(file);
	QString qsourcepath = file->Quellpfad() + "\\" + file->Dateiname();
	QString qtargetpath = file->Zielpfad() + "\\" + file->Dateiname();
	wstring sourcepath = qsourcepath.toStdWString();
	wstring targetpath = qtargetpath.toStdWString();

	int success = MoveFileW(sourcepath.c_str(), targetpath.c_str());
	if (success != 0)
	{
		MusicLibraryAdministrator::Write(file->Dateiname().toStdString() + " has successfully been moved to " + file->Zielpfad().toStdString());
	}
	else
	{
		MusicLibraryAdministrator::Write("Unable to copy " + file->Dateiname().toStdString() + " to " + file->Zielpfad().toStdString());
	}
}

void FileOperations::CreateDirectoryHelper(MusicFile* file)
{
	QString path = MusicLibraryAdministrator::settings->librarypath.c_str();
	if (file->IstInKompilation())
	{
		path = MusicLibraryAdministrator::JoinPath(path, "Compilations");
	}
	else if (QString::compare(file->Albuminterpret(), "") == 0)
	{
		path = MusicLibraryAdministrator::JoinPath(path, "Unknown Artist");
	}
	else
	{
		path = MusicLibraryAdministrator::JoinPath(path, file->Albuminterpret());
	}
	MusicLibraryAdministrator::Write("Creating directory " + path.toStdString());
	int result = CreateDirectoryW(path.toStdWString().c_str(), NULL);
	if (result == 0)
		MusicLibraryAdministrator::Write("Successfully created directory " + path.toStdString());

	if (QString::compare(file->Album(), "") == 0)
	{
		path = MusicLibraryAdministrator::JoinPath(path, "Unknown Album");
	}
	else
	{
		path = MusicLibraryAdministrator::JoinPath(path, file->Album());
	}
	MusicLibraryAdministrator::Write("Creating directory " + path.toStdString());
	result = CreateDirectoryW(path.toStdWString().c_str(), NULL);
	if (result == 0)
		MusicLibraryAdministrator::Write("Successfully created directory " + path.toStdString());

	file->Zielpfad(path);
}

void FileOperations::ErrorExit(LPTSTR lpszFunction)
{
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message and exit the process

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	ExitProcess(dw);
}






