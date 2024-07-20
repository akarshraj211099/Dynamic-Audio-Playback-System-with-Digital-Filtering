/**
 * \file CFilterBase.cpp
 * \brief implementation CFilterBase
 *
 * \date 11.09.2019
 * \author A. Wirth <antje.wirth@h-da.de
 */
#include <math.h>
#include <SKSLib.h>
#include "CFilterBase.h"

CFilterBase::CFilterBase(int order, int channels) {
	m_order = abs(order);
	m_channels = abs(channels);
	if ((m_order != 0) && (m_channels != 0)) {
		// intermediate buffer: for the intermediate filter states from last sample
		m_z = new float[m_channels * (m_order + 1)];
		for (int i = 0; i < m_channels * (m_order + 1); i++) {
			m_z[i] = 0.;
		}
	} else
		throw CException(CException::SRC_Filter, -1,
				"Filter order and channels must not be zero!");
}

CFilterBase::~CFilterBase() {
	if (m_z != NULL)
		delete[] m_z;
}

void CFilterBase::reset() {
	for (int i = 0; i < m_channels * (m_order + 1); i++) {
		m_z[i] = 0.;
	}
}

int CFilterBase::getOrder() {
	return m_order;
}
