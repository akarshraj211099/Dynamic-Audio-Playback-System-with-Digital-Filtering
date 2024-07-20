 /**
 * \file CAmpMeter.cpp
 * \brief implementation of CAmpMeter - <b><u>DISFUNCTIONAL VERSION !!!!!!</u></b>
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
#include <iostream>
#include <math.h>
#include <bitset>
using namespace std;

#include <SKSLib.h>
#include "CPlayerIOCtrls.h"

//#include "CIOWarriorExt.h"
#include "CAmpMeter.h"

CAmpMeter::CAmpMeter() {
	m_scmode = SCALING_MODE_LIN;		// logarithmic or linear bar?
	m_inValMax = 0;						// maximum input value
	for (int i = 0; i < 16; i++)
		m_thresholds[i] = 0;	// thresholds for the bar with 16 segments

	m_IoDev = NULL;						// address of the IOWarrior extension board control object (visualizer)
}

//void CAmpMeter::init(CIOWarriorExt *pIoDev, SCALING_MODE scmode, float inValMin,float inValMax, int8_t outLogValMin)
//void CAmpMeter::init(CPlayerIOCtrls *pIoDev, SCALING_MODE scmode, float inValMin,float inValMax, int8_t outLogValMin)
/*

void CAmpMeter::init(CPlayerCVDevice *pIoDev, SCALING_MODE scmode, float inValMin,float inValMax, int8_t outLogValMin)
{

	 * calculate the highest absolute value of the value range of the input signal

	m_inValMax = fmax(fabs(inValMax), fabs(inValMin));

	 * set the scaling mode

	m_scmode = scmode;

	 * a value of NULL for pIoDev is accepted here and won't throw an exception
	 * in order to get writeConsole() to work without an IoW attached.
	 *
	 * all other params are necessary to calculate the thresholds.

	m_IoDev = pIoDev;


	 * todo Calculate the threshold values according to the formulas from preparation task 2 and
	 * store them in m_thresholds.
	 *
	if(m_scmode == SCALING_MODE_LIN){
		for (int i=0;i<16;i++)
		{
			m_thresholds[i]= (m_inValMax/16.0)*i;
			//cout<<m_thresholds[i]<<endl;
		}
	}
	else{
		for (int i=0;i<16;i++)
		{
			float log_thresh = (outLogValMin/15)*(15-i);
			cout<<log_thresh<<endl;
			m_thresholds[i] = std::pow(10, log_thresh / 10);
			// cout<<m_thresholds[i]<<endl;
		}
	}cout<<endl;
}

*/



void CAmpMeter::init(CPlayerCVDevice *pIoDev, SCALING_MODE scmode, float inValMin,float inValMax, int8_t outLogValMin) {

	m_inValMax = fmax(fabs(inValMax), fabs(inValMin));

	m_scmode = scmode;

	m_IoDev = pIoDev;



	float delta = 0;
	if(m_scmode == SCALING_MODE_LIN)
	{

		delta = m_inValMax/16.0;
		for(int i=0;i<16;i++)
		{
			m_thresholds[i] = (delta * (i+1));
			//cout<<m_thresholds[i]<<endl;

		}
	}
	else
	{

		if(outLogValMin>0)
			outLogValMin=-outLogValMin;

		delta = outLogValMin/15;
		float log_thresh = 0;
		for(int i=0;i<16;i++)
		{
			log_thresh = (delta * (15-i));
			m_thresholds[i] = pow(10, log_thresh/10);
			//cout<<m_thresholds[i]<<endl;

		}
	}

}



void CAmpMeter::write(float *databuf, unsigned long databufsize) {
	if (NULL == databuf)
		throw CException(CException::SRC_AmpMeter, AMP_E_NOBUFFER,
				"Invalid data buffer.");
	return write(_getValueFromBuffer(databuf, databufsize));
}

void CAmpMeter::write(float data) {
	if (NULL == m_IoDev)
		throw CException(CException::SRC_AmpMeter, AMP_E_NOVISUALIZER,
				"Can't do binary pattern output.");

	m_IoDev->writeLEDs(_getBarPattern(data));
}

void CAmpMeter::writeConsole(float data) {
	uint16_t pattern = _getBarPattern(data);
	uint16_t revPattern = 0;

	/*
	 * the LEDs of the extension board are assigned from LSB leftmost (D1) to
	 * MSB rightmost (D16) and so _getBarPattern() is working
	 * => pattern for console output has to be reversed
	 */
	for (int i = 0; i < 16; i++) {
		revPattern = revPattern << 1 | (pattern & 1);
		pattern >>= 1;
	}

	cout << (bitset<16> ) revPattern << endl;
}
/*
uint16_t CAmpMeter::_getBarPattern(float data) {
uint16_t pattern = 0;
if(m_scmode == SCALING_MODE_LIN){
	data = abs(data);
}

else
{
	data = abs(data/m_inValMax);
}

for(int i=0;i<16;i++){
	if (data>=m_thresholds[i]){
		pattern|=(1<<i);
		//cout << "Bit set at position " << i << ", Pattern: " << bitset<16>(pattern) << endl;
	}
}
return pattern;


//cout << "Final pattern: " << bitset<16>(pattern) << endl;

* todo: Calculate the appropriate bar pattern (pattern) for the absolute value of data.
* The value of data is a linear value in any case.
*
*
*
* The calculated bar pattern is used to visualize the absolute value of data on the
* IOWarrior extension board.
*
* Example for a bar pattern with 8 elements:
* pat is 0b11111111 , if the absolute value
* of data is greater than the threshold of the 8th interval.
*
* Logarithmic scaling mode:
* Before calculating the bar pattern, the absolute value of data shall be
* divided by the maximum input value to adjust the highest value to 0 dB
* (peak normalization).

}*/

uint16_t CAmpMeter::_getBarPattern(float data) {
	uint16_t pattern = 0;
	if(m_scmode == SCALING_MODE_LIN)
	{
		data = abs(data);
	}
	else
	{
		data = abs(data/m_inValMax); //normalizing
	}

	for(int i = 0;i<16;i++)
	{
		if(data >= m_thresholds[i])
			pattern |= 1<<i ;
		//cout<<bitset<16>(pattern)<<endl;
	}
	return pattern;
}
float CAmpMeter::_getValueFromBuffer(float *databuf,
		unsigned long databufsize) {
	float maxVal = 0., tmpVal;
	for (unsigned long idx = 0; idx < databufsize; idx++) {
		tmpVal = fabs(databuf[idx]);
		if (maxVal < tmpVal) {
			maxVal = tmpVal;
		}
	}
	return maxVal;

}


