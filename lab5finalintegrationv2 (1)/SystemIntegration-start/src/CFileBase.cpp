/**
 * \file CFileBase.cpp
 *
 * \brief implementation class CFileBase
 * \date 17.11.2023
 *
 * \see File
 */
#include <iostream>
#include <stdio.h>
#include <string.h>
using namespace std;

#include <SKSLib.h>
#include "CFileBase.h"

/**
 * Constructor
 */
CFileBase::CFileBase(string path, unsigned int mode) {
	m_path = path;
	if ((mode != FILE_READ) && (mode != FILE_WRITE)
			&& (mode != FILE_WRITEAPPEND))
		mode = FILE_MODEUNKNOWN;
	if (mode == FILE_MODEUNKNOWN)
		m_mode = FILE_READ | FILE_WRITEAPPEND; // allow all operations
	else
		m_mode = mode;
}

CFileBase::~CFileBase() {
	//cout << "CFileBase destructor" << endl;
}

void CFileBase::print(void) {
	cout << "CFileBase[" << getModeTxt() << "]: " << m_path << endl;
}

/**
 * utility methods to get the open mode
 */
bool CFileBase::isFileW() {
	if ((m_mode & FILE_WRITE) == 0)
		return false;
	return true;
}

bool CFileBase::isFileR() {
	if ((m_mode & FILE_READ) == 0)
		return false;
	return true;
}

bool CFileBase::isFileWA() {
	if ((m_mode & FILE_WRITEAPPEND) == 0)
		return false;
	return true;
}

string CFileBase::getErrorTxt(FILEERRORS err) {
	switch (err) {
	case FILE_E_UNKNOWNOPENMODE:
		return string("unknown file open mode");
	case FILE_E_NOFILE:
		return string("file not found");
	case FILE_E_FILENOTOPEN:
		return string("file not open");
	case FILE_E_NOBUFFER:
		return string("no data buffer available");
	case FILE_E_CANTREAD:
		return string("file has been opened in write only mode");
	case FILE_E_READ:
		return string("error during read");
	case FILE_E_CANTWRITE:
		return string("file has been opened in read only mode");
	case FILE_E_WRITE:
		return string("error during write");
	case FILE_E_SPECIAL:
		return string("file error: ");
	default:
		return string("unknown error");
	}
}

// here, we may have a combination of different modes
string CFileBase::getModeTxt() {
	string sret;
	if (m_mode & FILE_READ)
		sret += "read mode";
	if (m_mode & FILE_WRITE) {
		if (sret.empty())
			sret = "write mode";
		else
			sret += " & write mode";
	}
	if (m_mode & FILE_WRITEAPPEND) {
		if (sret.empty())
			sret = "write/append mode";
		else
			sret += " & write/append mode";
	}
	if (sret.empty())
		sret = "unknown mode";
	return sret;
}
