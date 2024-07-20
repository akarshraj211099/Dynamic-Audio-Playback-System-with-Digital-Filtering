/**
 * \file CIOWarrior.cpp
 *
 * \brief implementation of the IOWarrior40 control class
 * \author Antje Wirth <antje.wirth@h-da.de>
 * \author Holger Frank <holger.frank@h-da.de>
 */
using namespace std;
#include <iostream>
#include <string>
#include <typeinfo>
#include <SKSLib.h>
#include "CIOWarriorExt.h"

CIOWarriorExt::CIOWarriorExt(void) :
		CIOWarrior() {
}

CIOWarriorExt::~CIOWarriorExt() {

}

void CIOWarriorExt::detect(void) {
	// check the state
	if (m_state != S_READY) {
		m_lastError = E_DEVICENOTREADY;
		throw(CException(this, typeid(this).name(), __FUNCTION__, m_lastError,
				getLastErrorStr()));
	}

	ULONG ret = IowKitReadNonBlocking(m_handle, IOW_PIPE_IO_PINS,
			(char*) &m_reportIn, sizeof(IOWKIT40_IO_REPORT));

	// set PORT0.1 LOW
	m_reportOut.Bytes[0] &= ~0x2;
	writeReportOut();

	/*
	 * the status of the ports is read typ. every 8ms (see Datasheet pg. 1)
	 * => wait for 1.5 interval time (i.e. 12ms) for reliable detection of changing state of
	 * Port0.1 (output) on Port0.0 (input)
	 */
	Sleep(12);

	// check if PORT0.0 is LOW
	ret = IowKitReadNonBlocking(m_handle, IOW_PIPE_IO_PINS, (char*) &m_reportIn,
			sizeof(IOWKIT40_IO_REPORT));
	if (sizeof(IOWKIT40_IO_REPORT) == ret) {
		/*
		 * buttons are low active => invert value of port 0 and mask SW1
		 *
		 * extension is available if value is 0
		 */
		if (!((~m_reportIn.Bytes[0]) & BTN_1)) {
			m_lastError = E_NOEXTENSION;
			throw(CException(this, typeid(this).name(), __FUNCTION__,
					m_lastError, "no extension found"));
		}
	} else {
		throw(CException(this, typeid(this).name(), __FUNCTION__, m_lastError,
				getLastErrorStr()));
	}
	// set PORT0.1 HIGH
	m_reportOut.Bytes[0] |= 0x2;
	writeReportOut();
	Sleep(12);
}

void CIOWarriorExt::open(void) {
	CIOWarrior::open();
	// throws exception if no extension detected
	detect();
}

void CIOWarriorExt::printDeviceInfo(void) {
	CIOWarrior::printDeviceInfo();
	cout << "EIT extension: available" << endl;
}

uint8_t CIOWarriorExt::readButtons(uint8_t mask) {
	if (m_state != S_READY) {
		m_lastError = E_DEVICENOTREADY;
		throw(CException(this, typeid(this).name(), __FUNCTION__, m_lastError,
				getLastErrorStr()));
	}

	if (sizeof(IOWKIT40_IO_REPORT)
			== IowKitReadNonBlocking(m_handle, IOW_PIPE_IO_PINS,
					(char*) &m_reportIn, sizeof(IOWKIT40_IO_REPORT))) {
		/*
		 * SW 1 and the button on the base board are connected both to
		 * Port0.0 AND 0.1 => a shift right for one digit moves all
		 */
		return (((~m_reportIn.Bytes[0]) >> 1) & mask);
	}
	/*
	 * IowKitReadNonBlocking returns the size of the chunk (5 for IoW40) only
	 * if a pin state has changed since the last query. Otherwise it returns 0
	 *
	 * => no exception at this point
	 */

	// no button state changed => returning 0x0 (all keys released)
	return 0x0;
}

void CIOWarriorExt::writeLEDs(uint16_t value) {
	// check the state
	if (m_state != S_READY) {
		m_lastError = E_DEVICENOTREADY;
		throw(CException(this, typeid(this).name(), __FUNCTION__, m_lastError,
				getLastErrorStr()));
	}
	/*
	 * set all LEDs on
	 * set all LEDs off whose bit in inverted value is set (LEDs are low active)
	 */
	m_reportOut.Value &= 0xff0000ff;
	m_reportOut.Value |= ((~(uint32_t)value) << 8);
	writeReportOut();
}

