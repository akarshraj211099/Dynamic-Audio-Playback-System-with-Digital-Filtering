/**
 * \file CFilter.h
 * \brief interface CFilter
 *
 * \date 11.09.2019
 * \author A. Wirth <antje.wirth@h-da.de
 */
#ifndef CFILTER_H_
#define CFILTER_H_
#include "CFilterBase.h"

/**
 * \brief General filter class to calculate digital filter output by direct form II transposed
 *
 * implements filter method, handles errors by exception
 */
class CFilter: public CFilterBase {
private:
	/**
	 * \brief filter coefficients of numerator
	 */
	float *m_b;
	/**
	 * \brief filter coefficients of denominator
	 */
	float *m_a;
	/*
	 * filter file path
	 */
	string m_filePath;

public:
	/**
	 * \brief Constructor
	 *
	 * - dynamic creation of arrays for filter coefficients
	 * - initialization of arrays
	 * - throws exception if order or channels are zero
	 *
	 * \param filePath path of the associated filter file
	 * \param ca pointer to array of denominator filter coefficients
	 * \param cb pointer to array of numerator filter coefficients
	 * \param order filter order
	 * \param channels number of channels of original signal
	 */
	CFilter(const string filePath, float *ca, float *cb, int order, int channels = 2);
	/**
	 * \brief deletes filter coefficient arrays
	 */
	virtual ~CFilter();
	/**
	 * \brief Filters a signal.
	 *
	 * \param inBuf pointer on block buffer of original signal
	 * \param outBuf pointer on block buffer of filtered signal
	 * \param framesPerBuffer no of frames in the block buffers (original & filtered)
	 * \return flag for successful execution (true) or error condition (false)
	 *
	 * The input/output buffer are float arrays, i.e. each element
	 * represents only a sample. For signals with multiple channels (e.g. stereo,
	 * surround sound etc.) the signal duration resp. buffer size must be
	 * framesPerBuffer*channels
	 */
	bool filter(float *inBuf, float *outBuf, int framesPerBuffer); // straight forward difference equation

	/**
	 * \return path of filter file
	 */
	string getFilePath();
};
#endif /* CFILTER_H_ */

