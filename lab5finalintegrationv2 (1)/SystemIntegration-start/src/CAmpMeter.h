/**
 * \file CAmpMeter.h
 * \brief interface of CAmpMeter - <b><u>DISFUNCTIONAL VERSION !!!!!!</u></b>
 *
 * \date 29.11.2022
 * \author A. Wirth <antje.wirth@h-da.de
 * \author H. Frank <holger.frank@h-da.de
 */

/************************************************************************
 *
 *	 					ATTENTION !!!!!!!!!
 *
 * This file contains only rudimentary code in a nonfunctional version
 * to make the start project compilable and executable in its initial version.
 *
 * It should be replaced by an complete implementation.
 *
 ************************************************************************/

#ifndef CAMPMETER_H_
#define CAMPMETER_H_

//class CIOWarriorExt;
//class CPlayerIOCtrls;
class CPlayerCVDevice;
class CAmpMeter {
public:
	enum SCALING_MODE {
		SCALING_MODE_LIN, SCALING_MODE_LOG
	};
	enum AMP_ERROR {
		AMP_E_NOBUFFER, AMP_E_NOVISUALIZER
	};

private:
	/**
	 * logarithmic or linear scaling
	 */
	SCALING_MODE m_scmode;
	/**
	 * maximum input value for this instance of CAmpMeter
	 *
	 * This value is the highest absolute value of the value range of the input signal.
	 * For a WAV file containing float samples in the range of -1.0 ... 1.0 this is 1.0 for example.
	 *
	 * In the linear scaling mode, this value is used for calculating the thresholds of
	 * the intervals and in the logarithmic scaling mode, it is used  as the reference value
	 * for the peak normalization. see also _getBarPattern().
	 */
	float m_inValMax;
	/**
	 * array of thresholds
	 *
	 * The threshold values are compared with the linear input values (absolute values)
	 * to calculate the bit pattern for the displayed bar.
	 *
	 * the content is calculated in init()
	 */
	float m_thresholds[16];
	/**
	 * pointer to an instance of the IOWarrior extension board class that shows the bar patterns on
	 * a line of 16 LEDs
	 */
//  CIOWarriorExt *m_IoDev;
//  CPlayerIOCtrls *m_IoDev;
    CPlayerCVDevice *m_IoDev;

public:
	/**
	 * Constructor
	 * initializes the attributes with initial values (see UML class diagram)
	 */
	CAmpMeter();

//	void init(CIOWarriorExt *pIoDev, SCALING_MODE scmode, float inValMin,float inValMax, int8_t outLogValMin = 0);

//	void init(CPlayerIOCtrls *pIoDev, SCALING_MODE scmode, float inValMin, float inValMax, int8_t outLogValMin = 0);
	void init(CPlayerCVDevice *pIoDev, SCALING_MODE scmode, float inValMin, float inValMax, int8_t outLogValMin = 0);


	/**
	 * \brief visualizes the amplitude of a data buffer on the connected IODevice as a bar pattern
	 *
	 * \param databuf [in] The address of the first data buffer element.
	 * \param databufsize [in] The number of elements of the data buffer.
	 *
	 * the displayed value is an amplitude value representative for the data buffer
	 *
	 * the bar is displayed from left to right
	 */
	void write(float *databuf, unsigned long databufsize);

	/**
	 * \brief Visualizes the amplitude of one single data value on the connected IODevice as a bar pattern
	 * \param data [in] The data value.
	 *
	 * the bar is displayed from left to right
	 */
	void write(float data);

	/**
	 * \brief visualizes the amplitude of a single sample on the console as a binary pattern
	 * \param data [in] the sample value
	 *
	 * this is only for testing the AmpMeter without an IoW connected
	 *
	 * the bar is displayed from left to right, 1 represents an active element,
	 * 0 represents an inactive element
	 */
	void writeConsole(float data);
private:
	/**
	 * \brief Returns an appropriate bar pattern for the data value scaled according to
	 * the current settings of the amplitude meter
	 *
	 * \param data [in] linear data value.
	 */
	uint16_t _getBarPattern(float data);

	/**
	 * \brief Returns an amplitude value representative for the data buffer.
	 * This value is taken for the visualization of the buffer.
	 *
	 * \param databuf [in] The address of the first data buffer element.
	 * \param databufsize [in] The number of elements of the data buffer.
	 */
	float _getValueFromBuffer(float *databuf, unsigned long databufsize);
};
#endif /* CAMPMETER_H_ */


/*#ifndef CAMPMETER_H_
#define CAMPMETER_H_

#include "CIOWarriorExt.h"

class CAmpMeter {
public:
	enum SCALING_MODE {
		SCALING_MODE_LIN,
		SCALING_MODE_LOG
	};
	enum AMP_ERROR {
		AMP_E_NOBUFFER,
		AMP_E_NOVISUALIZER
	};

private:
	SCALING_MODE m_scmode;
	float m_inValMax;
	float m_thresholds[16];
	CIOWarriorExt *m_IoDev;

public:
	CAmpMeter();
	void init(CIOWarriorExt *pIoDev, SCALING_MODE scmode, float inValMin,
			float inValMax, int8_t outLogValMin = 0);
	void write(float *databuf, unsigned long databufsize);
	void write(float data);
private:
	uint16_t _getBarPattern(float data);
	float _getValueFromBuffer(float *databuf, unsigned long databufsize);
};
#endif  CAMPMETER_H_ */

