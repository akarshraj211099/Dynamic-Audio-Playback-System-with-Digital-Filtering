/**
 * \file CFilterBase.h
 * \brief interface CFilterBase
 *
 * \date 11.09.2019
 * \author A. Wirth <antje.wirth@h-da.de
 */
#ifndef CFILTERBASE_H_
#define CFILTERBASE_H_

/**
 * \brief filter base class
 *
 * provides common attributes for derived filter classes and defines basic
 * interface
 */
class CFilterBase {
protected:
	/**
	 * \brief intermediate states from last sample or circular buffer (optimized delay filters)
	 */
	float *m_z;
	/**
	 * \brief filter order
	 */
	int m_order;
	/**
	 * \brief number of channels of the signals to be filtered
	 */
	int m_channels;

public:
	/**
	 * \brief Constructor
	 *
	 * - initializes attributes
	 * - initializes buffer for intermediate states
	 * - throws exception if order or channels are zero
	 *
	 * \param order order of the filter
	 * \param channels no of channels of input signal
	 */
	CFilterBase(int order, int channels);
	/**
	 * Destructor
	 *
	 * deletes buffer for intermediate states
	 */
	virtual ~CFilterBase();
	/**
	 * \brief filters signal from input buffer x into output buffer y
	 *
	 * this virtual method defines the filter methods interface for the
	 * implementation in derived classes
	 *
	 * \param inBuf pointer on block buffer of original signal
	 * \param outBuf pointer on block buffer of filtered signal
	 * \param framesPerBuffer no of frames in the block buffers (original & filtered)
	 */
	virtual bool filter(float *inBuf, float *outBuf, int framesPerBuffer)=0;
	/**
	 * \brief resets filter
	 *
	 * Clears all intermediate values. May be used before filtering a new signal
	 * by the same filter object.
	 */
	void reset();

	/**
	 * \brief retrieves the order of the filter
	 */
	int getOrder();
};
#endif /* CFILTERBASE_H_ */

