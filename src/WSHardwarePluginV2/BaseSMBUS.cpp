#include "stdafx.h"
#include "BaseSMBUS.h"

bool Hardware::Memory::SMBUSControllerBase::IsDDR3(const uint8_t& DDRTypes)
{
	// ��ȷ��
	return (DDRTypes == 11) || (DDRTypes == 15);
}

bool Hardware::Memory::SMBUSControllerBase::IsDDR4(const uint8_t& DDRTypes)
{
	// ��ȷ��
	return(DDRTypes == 12) || (DDRTypes == 14) || (DDRTypes == 16);
}