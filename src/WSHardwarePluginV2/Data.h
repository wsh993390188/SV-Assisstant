/*!
* @file Data.h
* @brief ͨ�����ݽӿ�
*
* @author ��˶(wangshuo20@lenovo.com)
* @version 1.0
* @date 2020��6��3��
*/
#pragma once

namespace Hardware
{
	/// @brief ͨ������
	namespace Data
	{
		/// @brief ����ֵ����
		enum class ErrorType : int
		{
			SUCCESS = 0,		///< �����ɹ�
			UNKNOWN,			///< δ֪�Ĵ���
			UNKNOWNCOMMAND,		///< δ֪������
			UNKNOWNJSON,		///<��ʶ���JSON����
			UNKNOWNCPU,			///<δ֪��CPU
			OVERBOUND,			///<����Խ��
			NOTINITIALIZE,		///<δ���г�ʼ����ִ�б�����
			NOTIMPLEMENTED,		///<δʵ�ֵĹ���
			NOTSUPPORT,			///<��֧�ֵĹ���
			XMLERROR,			///<XML�����ļ�����
			DATAEMPTY,			///<����Ϊ��
			COMCREATEERROR,		///<����Com����ʱ����
		};
	}
}
