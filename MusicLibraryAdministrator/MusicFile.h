#pragma once
#include <QtCore>

class MusicFile
{
public:
	MusicFile();
	~MusicFile();

	void Quellpfad(QString);
	QString Quellpfad();

	void Zielpfad(QString);
	QString Zielpfad();

	void Dateiname(QString);
	QString Dateiname();

	void Titel(QString);
	QString Titel();

	void Interpret(QString);
	QString Interpret();

	void Albuminterpret(QString);
	QString Albuminterpret();

	void Album(QString);
	QString Album();

	void Titelnummer(QString);
	QString Titelnummer();

	void Genre(QString);
	QString Genre();

	void Komponisten(QString);
	QString Komponisten();

	void IstInKompilation(QString);
	bool IstInKompilation();

private:
	QString quellpfad;
	QString zielpfad;
	QString dateiname;
	QString titel;
	QString interpret;
	QString albuminterpret;
	QString album;
	QString titelnummer;
	QString genre;
	QString komponisten;
	bool istinkompilation;
};
