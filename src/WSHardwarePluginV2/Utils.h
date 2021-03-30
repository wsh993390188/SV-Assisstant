#pragma once
#include <iomanip>
#include <sstream>
#include "StringHelper.hpp"

namespace Hardware
{
	/// @brief ��ȡӲ����Ϣ�ĸ�������
	namespace Utils
	{
#define GET_VARIABLE_NAME(Variable) (#Variable)
#define MAKE_VARIABLE_TO_NAME(Variable, Name) (Variable##Name)
		/// @brief �����߳���Ե�԰�����
		class ThreadGroupTempAffinity final
		{
			/// @brief ��һ���߳����е�CPU��Ե������
			GROUP_AFFINITY PreviousGroupAffinity;

			ThreadGroupTempAffinity() = delete;
			ThreadGroupTempAffinity(const ThreadGroupTempAffinity&) = delete;
			ThreadGroupTempAffinity& operator = (const ThreadGroupTempAffinity&) = delete;

		public:
			/// @brief �����߳���Ե��
			/// @param[in] core_id �߳���Ҫ���е���Ե��ID
			/// @param[in] checkStatus �Ƿ�������״̬��������׳��쳣��Ĭ�ϲ����
			ThreadGroupTempAffinity(uint32_t core_id, bool checkStatus = false);
			/// @brief �ָ��߳���Ե��
			~ThreadGroupTempAffinity();
		};

		/// @brief CPUID����Ϣ
		union CPUID_INFO
		{
			/// @brief �����ʽ
			int array[4];
			/// @brief �Ĵ�����ʽ
			struct { unsigned int eax, ebx, ecx, edx; } reg;
		};

		/// @brief 32λ���ݺ�64λ���ݻ�ת�ṹ,�����Ż�WinRing0����
		union CvtDs
		{
			/// @brief 64λ����
			UINT64 ui64;
			struct
			{
				/// @brief 64λ���ݵĵ�32λ����
				DWORD Eax;
				/// @brief 64λ���ݵĸ�32λ����
				DWORD Edx;
			} ui32;
		};

		/// @brief ��ȡCPUID��Ϣ
		/// @param[in] leaf CPUIDҳ��
		/// @param[out] info COUID��ȡ����Ϣ
		void GetCpuid(const int leaf, CPUID_INFO& info);

		/// @brief ��ȡCPUID��Ϣ
		/// @param[in] leaf CPUIDҳ��
		/// @param[in] subleaf CPUID��ҳ��
		/// @param[out] info COUID��ȡ����Ϣ
		void GetCpuid(const unsigned leaf, const unsigned subleaf, CPUID_INFO& info);

		/// @brief ������ȡ��Bits���룬32λ����
		/// @param[in] beg ���뿪ʼ
		/// @param[in] end �������
		/// @return ����
		uint32_t build_bit_ui(uint32_t beg, uint32_t end);

		/// @brief ��ȡָ��λ�õ�Bits��Ϣ�� 32λ����
		/// @param[in] myin ԭʼ����
		/// @param[in] beg ��ʼλ��
		/// @param[in] end ����λ��
		/// @return �õ�������
		uint32_t extract_bits_ui(uint32_t myin, uint32_t beg, uint32_t end);

		/// @brief ������ȡ��Bits���룬64λ����
		/// @param[in] beg ���뿪ʼ
		/// @param[in] end �������
		/// @return ����
		uint64_t build_bit(uint32_t beg, uint32_t end);

		/// @brief ��ȡָ��λ�õ�Bits��Ϣ�� 64λ����
		/// @param[in] myin ԭʼ����
		/// @param[in] beg ��ʼλ��
		/// @param[in] end ����λ��
		/// @return �õ�������
		uint64_t extract_bits(uint64_t myin, uint32_t beg, uint32_t end);

		/// @brief ǿö������ת�ɵײ�洢��ʽ
		/// @tparam T ö������
		/// @param[in] enumerator ö�ٶ���
		/// @return �ײ�洢����
		template<typename T>
		constexpr decltype(auto) ToUType(T enumerator)
		{
			return static_cast<std::underlying_type_t<T>>(enumerator);
		}

		/// @brief �ر�Handle�ĺ���
		struct HANDLEDeleter
		{
			/// @brief �ر�����
			/// @param[in] handle �رվ��
			void operator()(HANDLE handle) const
			{
				if (handle != INVALID_HANDLE_VALUE && handle)
					::CloseHandle(handle);
			}
		};

		/// @brief ���������ָ��
		using HANDLE_unique_ptr = std::unique_ptr<void, HANDLEDeleter>;
		/// @brief �����������ָ��
		/// @param[in] handle Ҫ�����ľ��
		/// @return ���������ָ��
		HANDLE_unique_ptr make_HANDLE_unique(HANDLE handle);

		/// @brief Win32API��������ʱ��������
		/// @param[in] lpszFunction ���ú�����
		void ErrorCode(LPCSTR lpszFunction);

		/// @brief Win32API��������ʱ��������
		/// @param[in] lpszFunction ���ú�����
		/// @param[in] error ����ֵ
		void ErrorCode(LPCSTR lpszFunction, const DWORD error);

		/// @brief �����ȿ��Ƶ�ToString
		/// @tparam T ����������
		/// @param[in] a_value ��Ҫת���ĸ�����
		/// @param[in] n ����λ��
		/// @return String
		template<typename T,
			typename std::enable_if_t < std::is_floating_point<T>::value, int > = 0 >
			std::string to_string_with_precision(const T a_value, const int n = 2)
		{
			std::ostringstream out;
			out.setf(std::ios::fixed);
			out << std::setprecision(n) << a_value;
			auto str = out.str();
			// Ensure that there is a decimal point somewhere (there should be)
			if (str.find('.') != std::string::npos)
			{
				// Remove trailing zeroes
				str = str.substr(0, str.find_last_not_of('0') + 1);
				// If the decimal point is now the last character, remove that as well
				if (str.find('.') == str.size() - 1)
				{
					str = str.substr(0, str.size() - 1);
				}
			}
			return str;
		}

		/// @brief ����תʮ�������ַ���
		/// @tparam T ��������
		/// @param value ��ֵ
		/// @param n ���뾫��
		/// @return ʮ�������ַ���
		template<typename T, typename std::enable_if_t<std::is_integral_v<T>, int> = 0>
		std::string to_string_hex(const T value, const int n = 2)
		{
			std::ostringstream out;
			out << std::hex << std::uppercase << std::setw(n) << std::setfill('0') << value + 0;
			return out.str();
		}

		/// @brief �ڴ��С����ת����λ���ַ���
		/// @tparam Numberic ��������
		/// @param num ��ֵ
		/// @return ����λ�ַ���
		template<typename Numberic, class = std::enable_if_t<std::is_integral_v<Numberic>>>
		std::string MemoryToStringWithUnit(Numberic num)
		{
			if (num < 1024)
			{
				return std::to_string(num) + "B";
			}
			else if (num / 1024 < 1024) { //С��1MB����ת����KB
				return to_string_with_precision(num / 1024.0) + "KB";
			}
			else if (num / (1024 * 1024) < 1024) { //С��1GB����ת����MB
				return to_string_with_precision(num / (1024.0 * 1024.0)) + "MB";
			}
			else { //����ת����GB
				return to_string_with_precision(num / (1024 * 1024 * 1024.0)) + "GB";
			}
		}

		/// @brief ���������ת����λ���ַ���
		/// @tparam Numberic ��������
		/// @param num ��ֵ
		/// @return �ַ���
		template<typename Numberic, class = std::enable_if_t<std::is_integral_v<Numberic>>>
		std::string NetSpeedToStringWithUnit(Numberic num)
		{
			if (num < 1000)
			{
				return std::to_string(num) + " bps";
			}
			else if (num / 1000 < 1000) {
				return to_string_with_precision(num / 1000.0) + " Kbps";
			}
			else if (num / (1000 * 1000) < 1000) { //С��1GB����ת����MB
				return to_string_with_precision(num / (1000.0 * 1000.0)) + " Mbps";
			}
			else { //����ת����GB
				return to_string_with_precision(num / (1000.0 * 1000.0 * 1000.0)) + " Gbps";
			}
		}

		/// @brief ��ȡģ��ĺ���ָ��
		/// @tparam Func ����ָ������
		/// @param hModule ģ����
		/// @param FuncName ������
		/// @return ����ָ��
		template<typename Func>
		decltype(auto) GetADLFuncAddress(HMODULE hModule, const std::string& FuncName)
		{
			if (hModule)
			{
				auto FuncImpl = reinterpret_cast<Func>(GetProcAddress(hModule, FuncName.c_str()));
				if (FuncImpl)
				{
					return FuncImpl;
				}
				else
				{
					Utils::ErrorCode(("Load" + FuncName + "Error ").c_str());
				}
			}
			return reinterpret_cast<Func>(nullptr);
		}

		/// @brief GUIDת�ַ���
		/// @param guid guid
		/// @return ANSI�ַ���
		std::string GUIDToString(const GUID& guid);
	}
}
