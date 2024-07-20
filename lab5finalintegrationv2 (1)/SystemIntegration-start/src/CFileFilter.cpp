/**
 * \file CFileFilter.cpp
 *
 * \brief implementation class CFileFilter
 * \date 17.11.2023
 *
 * \see File
 */
#include <iostream>
#include <stdio.h>
#include <string.h>
using namespace std;

#include <SKSLib.h>
#include "CFileFilter.h"

// < ---------- SAMPLE-START ----------->

CFileFilter::CFileFilter(string path, FILEMODES mode) :
		CFileBase(path, mode) {
	m_pFile = NULL;	// file pointer
	m_fs = 0;		// sample rate of the filter to load
	m_order = 0;	// filter order
	m_b = NULL;		// filter numerator coefficients
	m_blen = 0;		// number of filter numerator coefficients
	m_a = NULL;		// filter denominator coefficients
	m_alen = 0;		// number of filter denominator coefficients
}

CFileFilter::~CFileFilter() {
	close();
}

void CFileFilter::open() {
	if (!isFileR())
		throw CException(CException::SRC_File, FILE_E_SPECIAL,
				getErrorTxt(FILE_E_SPECIAL)
						+ "Writing is not implemented for this class!");
	m_pFile = fopen(m_path.c_str(), "r");
	if (m_pFile == NULL)
		throw CException(CException::SRC_File, FILE_E_NOFILE,
				getErrorTxt(FILE_E_NOFILE));
}

void CFileFilter::close() {
	if (m_pFile != NULL)
		fclose(m_pFile);
	if (m_b)
		delete[] m_b;
	if (m_a)
		delete[] m_a;
}

int CFileFilter::read(int fs) {
	if (m_pFile == NULL)
		throw CException(CException::SRC_File, FILE_E_FILENOTOPEN,
				getErrorTxt(FILE_E_FILENOTOPEN));

	if (fs == 0)
		throw CException(CException::SRC_File, FILE_E_SPECIAL,
				getErrorTxt(FILE_E_SPECIAL)
						+ "Sampling rate must be specified for a filter!");
	m_fs = abs(fs);

	fseek(m_pFile, 0, SEEK_SET);		// start at the beginning of the file

	// get the header
	int bufsize = 360;
	char buf[bufsize];

	// get the header
	fgets(buf, bufsize, m_pFile);

	// parse the header
	string s = buf;
	int pos = 0, end = 0;
	end = s.find(";", pos);
	m_filterType = s.substr(pos, end - pos);
	pos = end + 1;
	end = s.find(";", pos);
	string sorder = s.substr(pos, end - pos);
	m_order = stoi(sorder);
	m_filterInfo = s.substr(end + 1);

	// read data
	int i, fsr;
	while ( NULL != fgets(buf, bufsize, m_pFile)) {
		fsr = stoi(buf);			// find fs
		if (m_fs != fsr) {
			char *pgot = buf;
			while (pgot)			// skip b coefficients
			{
				pgot = fgets(buf, bufsize, m_pFile);
				if (NULL != strrchr(buf, '\n'))
					break; // end of line has been read
			}
			while (pgot)			// skip a coefficients
			{
				pgot = fgets(buf, bufsize, m_pFile);
				if (NULL != strrchr(buf, '\n'))
					break;
			}
		} else {
			m_b = new float[m_order + 1];
			m_a = new float[m_order + 1];
			char sep;
			for (i = 0; i < m_order + 1; i++) {
				if (EOF == fscanf(m_pFile, "%f%c", &m_b[i], &sep))
					break;
				if (sep == '\n')
					break;
			}
			m_blen = i;
			if (sep != '\n')
				fscanf(m_pFile, "%c", &sep);
			for (i = 0; i < m_order + 1; i++) {
				if (EOF == fscanf(m_pFile, "%f%c", &m_a[i], &sep))
					break;
				if (sep == '\n')
					break;
			}
			m_alen = i;
			if (sep != '\n')
				fscanf(m_pFile, "%c", &sep);
			return ftell(m_pFile);			// returns the number of bytes read
		}
	}
	return 0;						// 0 if no appropriate sampling rate found
}

void CFileFilter::print(void) {
	CFileBase::print();
	cout << m_filterType << "fiter [order=" << m_order << ", fs=" << m_fs
			<< "] " << m_filterInfo << endl;
	cout << "coefficients b={";
	for (int i = 0; i < m_blen; i++)
		cout << m_b[i] << "\t";
	cout << "}" << endl << "coefficients a={";
	for (int i = 0; i < m_alen; i++)
		cout << m_a[i] << "\t";
	cout << "}" << endl;
}

int CFileFilter::getOrder() {
	return m_order;
}

string CFileFilter::getFilterType() {
	return m_filterType;
}

string CFileFilter::getFilterInfo() {
	return m_filterInfo;
}

float* CFileFilter::getBCoeffs() {
	return m_b;
}

int CFileFilter::getNumBCoeffs() {
	return m_blen;
}

float* CFileFilter::getACoeffs() {
	return m_a;
}

int CFileFilter::getNumACoeffs() {
	return m_alen;
}
// < ---------- SAMPLE-END ----------->
