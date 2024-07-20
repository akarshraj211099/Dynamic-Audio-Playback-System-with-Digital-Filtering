/**
 * \file CFileSound.cpp
 *
 * \brief implementation class CFileSound
 * \date 17.11.2023
 *
 * \see File
 */
#include <iostream>
#include <stdio.h>
#include <string.h>
using namespace std;

#include <SKSLib.h>
#include "CFileSound.h"

CFileSound::CFileSound(string path, const FILEMODES mode) :
		CFileBase(path, mode) {
	memset(&m_sfinfo, 0, sizeof(m_sfinfo));
	m_pSFile = NULL;
	//cout << "CSoundFile constructor" << endl;
}

CFileSound::~CFileSound() {
	close();
	//cout << "CSoundFile destructor" << endl;
}

/**
 * overloaded method
 */
void CFileSound::print(void) {
	CFileBase::print();
	cout << "CSoundFile[" << hex << m_pSFile << dec << "]" << endl;
	cout << "Soundfile: channels(" << m_sfinfo.channels << ") frames("
			<< m_sfinfo.frames;
	cout << ") fs(" << m_sfinfo.samplerate / 1000. << "kHz)" << endl;
	cout << "Duration: " << m_sfinfo.frames / m_sfinfo.samplerate << "s"
			<< endl;
}

void CFileSound::open() {
	int mode;
	if (isFileR() && (isFileWA() || isFileW()))
		mode = SFM_RDWR;
	else if (isFileW() || isFileWA())
		mode = SFM_WRITE;
	else if (isFileR())
		mode = SFM_READ;
	else
		throw CException(CException::SRC_File, FILE_E_UNKNOWNOPENMODE,
				getErrorTxt(FILE_E_UNKNOWNOPENMODE));

	m_pSFile = sf_open(m_path.c_str(), mode, &m_sfinfo);
	if (!m_pSFile)
		throw CException(CException::SRC_File, sf_error(m_pSFile),
				m_path + ": " + sf_strerror(m_pSFile));
}

void CFileSound::close() {
	if (m_pSFile != NULL) {
		sf_close(m_pSFile);
		m_pSFile = NULL;
	}
}

int CFileSound::read(float *buf, int bufsize) {
	if (m_pSFile == NULL)
		throw CException(CException::SRC_File, FILE_E_FILENOTOPEN,
				getErrorTxt(FILE_E_FILENOTOPEN));
	if (buf == NULL)
		throw CException(CException::SRC_File, FILE_E_NOBUFFER,
				getErrorTxt(FILE_E_NOBUFFER));
	if (isFileW() || isFileWA())
		throw CException(CException::SRC_File, FILE_E_CANTREAD,
				getErrorTxt(FILE_E_CANTREAD));

	int szread = sf_read_float(m_pSFile, buf, bufsize);
	// returns 0 if no data left to read
	return szread;
}

void CFileSound::write(float *buf, int bufsize) {
	if (m_pSFile == NULL)
		throw CException(CException::SRC_File, FILE_E_FILENOTOPEN,
				getErrorTxt(FILE_E_FILENOTOPEN));
	if (buf == NULL)
		throw CException(CException::SRC_File, FILE_E_NOBUFFER,
				getErrorTxt(FILE_E_NOBUFFER));
	if (isFileR())
		throw CException(CException::SRC_File, FILE_E_CANTWRITE,
				getErrorTxt(FILE_E_CANTWRITE));

	int szwrite = sf_write_float(m_pSFile, buf, bufsize);
	if (szwrite != bufsize) {
		close();
		throw CException(CException::SRC_File, FILE_E_WRITE,
				getErrorTxt(FILE_E_WRITE));
	}
}

void CFileSound::rewind() {
	if (m_pSFile == NULL)
		throw CException(CException::SRC_File, FILE_E_FILENOTOPEN,
				getErrorTxt(FILE_E_FILENOTOPEN));

	sf_seek(m_pSFile, 0, SEEK_SET);
}

int CFileSound::getNumFrames() {
	return m_sfinfo.frames;
}

int CFileSound::getSampleRate() {
	return m_sfinfo.samplerate;
}

int CFileSound::getFormat() {
	return m_sfinfo.format;
}

int CFileSound::getNumChannels() {
	return m_sfinfo.channels;
}

void CFileSound::setSampleRate(int fs) {
	if (fs <= 0)
		throw CException(CException::SRC_File, -1,
				"Invalid sample rate (must be > 0)!");
	m_sfinfo.samplerate = fs;
}

void CFileSound::setNumChannels(int numChan) {
	if (numChan <= 0)
		throw CException(CException::SRC_File, -1,
				"Invalid number of channels (must be > 0)!");
	m_sfinfo.channels = numChan;
}

void CFileSound::setFormat(int format) {
	m_sfinfo.format = format;
}
