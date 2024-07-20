/**
 * \file CFileFilter.h
 *
 * interface class CFileFilter
 * \date 17.11.2023
 *
 * \see File
 */
#ifndef FILEFILTER_H_
#define FILEFILTER_H_

#include "sndfile.h"
#include "CFileBase.h"
#include <string>
using namespace std;

/**
 * class to read files with the ASDD filter file format
 * derived from CFileBase
 * implements additional methods for parsing filter files
 * overloads the basic class' print method
 */
class CFileFilter: public CFileBase {
private:
	FILE *m_pFile;
	string m_filterType; // type of the filter (e.g. lowpass, shelving, delay, ...)
	string m_filterInfo;	// free textual filter description
	int m_order;			// filter order
	float *m_b;				// filter numerator coefficients
	int m_blen;				// number of filter numerator coefficients
	float *m_a;				// filter denominator coefficients
	int m_alen;				// number of filter denominator coefficients
	int m_fs;				// sampling frequency

public:
	CFileFilter(string path, CFileBase::FILEMODES mode = CFileBase::FILE_MODEUNKNOWN);
	~CFileFilter();
	/**
	 * opens a file
	 */
	void open();
	/**
	 * closes a file
	 */
	void close();
	/**
	 * reads the content of the file for a given sampling frequency
	 */
	int read(int fs);
	/**
	 * prints content of the filter file on console
	 */
	void print(void);
	/**
	 * retrieve information from the filter file
	 */
	//static void getFs(const char *path, int *fs, int &numFs);// get the sample rates for which the file contains coefficients
	int getOrder();						// filter order
	string getFilterType();	// type of the filter (e.g. lowpass, shelving, delay, ...)
	string getFilterInfo();				// free textual filter description
	float* getBCoeffs();				// filter numerator coefficients
	int getNumBCoeffs();			// number of filter numerator coefficients
	float* getACoeffs();				// filter denominator coefficients
	int getNumACoeffs();			// number of filter denominator coefficients
};
// < ---------- SAMPLE-END ----------->

#endif /* FILEFILTER_H_ */
