#include "MusicFile.h"
#include <string>

using namespace std;

MusicFile::MusicFile()
{
	quellpfad = "";
	zielpfad = "";
	titel = "";
	interpret = "";
	albuminterpret = "";
	album = "";
	titelnummer = "";
	genre = "";
	komponisten = "";
	istinkompilation = false;
}

MusicFile::~MusicFile()
{
}

void MusicFile::Quellpfad(QString dateipfad)
{
	this->quellpfad = dateipfad;
}

QString MusicFile::Quellpfad()
{
	return this->quellpfad;
}

void MusicFile::Zielpfad(QString dateipfad)
{
	this->zielpfad = dateipfad;
}

QString MusicFile::Zielpfad()
{
	return this->zielpfad;
}

void MusicFile::Dateiname(QString dateiname)
{
	this->dateiname = dateiname;
}

QString MusicFile::Dateiname()
{
	return this->dateiname;
}

void MusicFile::Titel(QString titel)
{
	this->titel = titel;
}

QString MusicFile::Titel()
{
	return this->titel;
}

void MusicFile::Interpret(QString interpret)
{
	this->interpret = interpret;
}

QString MusicFile::Interpret()
{
	return this->interpret;
}

void MusicFile::Albuminterpret(QString albuminterpret) 
{
	albuminterpret.replace("/", "_");
	albuminterpret.replace("\\", "_");
	albuminterpret.replace("*", "_");
	albuminterpret.replace('"', "_");
	albuminterpret.replace("?", "_");
	albuminterpret.replace(":", "_");
	this->albuminterpret = albuminterpret;
}

QString MusicFile::Albuminterpret()
{
	return this->albuminterpret;
}

void MusicFile::Album(QString album)
{
	album.replace("/", "_");
	album.replace("\\", "_");
	album.replace("*", "_");
	album.replace('"', "_");
	album.replace("?", "_");
	album.replace(":", "_");
	this->album = album;
}

QString MusicFile::Album()
{
	return this->album;
}

void MusicFile::Titelnummer(QString titelnummer)
{
	this->titelnummer = titelnummer;
}

QString MusicFile::Titelnummer()
{
	return this->titelnummer;
}

void MusicFile::Genre(QString genre)
{
	this->genre = genre;
}

QString MusicFile::Genre()
{
	return this->genre;
}

void MusicFile::Komponisten(QString komponisten)
{
	this->komponisten = komponisten;
}

QString MusicFile::Komponisten()
{
	return this->komponisten;
}

void MusicFile::IstInKompilation(QString istinkompilation)
{
	if (QString::compare(istinkompilation, "1") == 0)
		this->istinkompilation = true;
}

bool MusicFile::IstInKompilation()
{
	return this->istinkompilation;
}

