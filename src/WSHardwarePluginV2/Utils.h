#pragma once
#include <iomanip>
#include <sstream>
#include "StringHelper.hpp"

namespace Hardware
{
	/// @brief 获取硬件信息的辅助工具
	namespace Utils
	{
#define GET_VARIABLE_NAME(Variable) (#Variable)
#define MAKE_VARIABLE_TO_NAME(Variable, Name) (Variable##Name)
		/// @brief 设置线程亲缘性帮助类
		class ThreadGroupTempAffinity final
		{
			/// @brief 上一次线程运行的CPU亲缘性数据
			GROUP_AFFINITY PreviousGroupAffinity;

			ThreadGroupTempAffinity() = delete;
			ThreadGroupTempAffinity(const ThreadGroupTempAffinity&) = delete;
			ThreadGroupTempAffinity& operator = (const ThreadGroupTempAffinity&) = delete;

		public:
			/// @brief 设置线程亲缘性
			/// @param[in] core_id 线程需要运行的亲缘性ID
			/// @param[in] checkStatus 是否检查设置状态，出错会抛出异常，默认不检查
			ThreadGroupTempAffinity(uint32_t core_id, bool checkStatus = false);
			/// @brief 恢复线程亲缘性
			~ThreadGroupTempAffinity();
		};

		/// @brief CPUID的信息
		union CPUID_INFO
		{
			/// @brief 数组格式
			int array[4];
			/// @brief 寄存器格式
			struct { unsigned int eax, ebx, ecx, edx; } reg;
		};

		/// @brief 32位数据和64位数据互转结构,用于优化WinRing0传参
		union CvtDs
		{
			/// @brief 64位数据
			UINT64 ui64;
			struct
			{
				/// @brief 64位数据的低32位数据
				DWORD Eax;
				/// @brief 64位数据的高32位数据
				DWORD Edx;
			} ui32;
		};

		/// @brief 获取CPUID信息
		/// @param[in] leaf CPUID页面
		/// @param[out] info COUID获取得信息
		void GetCpuid(const int leaf, CPUID_INFO& info);

		/// @brief 获取CPUID信息
		/// @param[in] leaf CPUID页面
		/// @param[in] subleaf CPUID子页面
		/// @param[out] info COUID获取得信息
		void GetCpuid(const unsigned leaf, const unsigned subleaf, CPUID_INFO& info);

		/// @brief 构建获取的Bits掩码，32位函数
		/// @param[in] beg 掩码开始
		/// @param[in] end 掩码结束
		/// @return 掩码
		uint32_t build_bit_ui(uint32_t beg, uint32_t end);

		/// @brief 获取指定位置的Bits信息， 32位函数
		/// @param[in] myin 原始数据
		/// @param[in] beg 开始位置
		/// @param[in] end 结束位置
		/// @return 得到的数据
		uint32_t extract_bits_ui(uint32_t myin, uint32_t beg, uint32_t end);

		/// @brief 构建获取的Bits掩码，64位函数
		/// @param[in] beg 掩码开始
		/// @param[in] end 掩码结束
		/// @return 掩码
		uint64_t build_bit(uint32_t beg, uint32_t end);

		/// @brief 获取指定位置的Bits信息， 64位函数
		/// @param[in] myin 原始数据
		/// @param[in] beg 开始位置
		/// @param[in] end 结束位置
		/// @return 得到的数据
		uint64_t extract_bits(uint64_t myin, uint32_t beg, uint32_t end);

		/// @brief 强枚举类型转成底层存储格式
		/// @tparam T 枚举类型
		/// @param[in] enumerator 枚举对象
		/// @return 底层存储类型
		template<typename T>
		constexpr decltype(auto) ToUType(T enumerator)
		{
			return static_cast<std::underlying_type_t<T>>(enumerator);
		}

		/// @brief 关闭Handle的函数
		struct HANDLEDeleter
		{
			/// @brief 关闭主体
			/// @param[in] handle 关闭句柄
			void operator()(HANDLE handle) const
			{
				if (handle != INVALID_HANDLE_VALUE && handle)
					::CloseHandle(handle);
			}
		};

		/// @brief 句柄的智能指针
		using HANDLE_unique_ptr = std::unique_ptr<void, HANDLEDeleter>;
		/// @brief 创建句柄智能指针
		/// @param[in] handle 要创建的句柄
		/// @return 句柄的智能指针
		HANDLE_unique_ptr make_HANDLE_unique(HANDLE handle);

		/// @brief Win32API发生错误时，错误码
		/// @param[in] lpszFunction 调用函数名
		void ErrorCode(LPCSTR lpszFunction);

		/// @brief Win32API发生错误时，错误码
		/// @param[in] lpszFunction 调用函数名
		/// @param[in] error 错误值
		void ErrorCode(LPCSTR lpszFunction, const DWORD error);

		/// @brief 带精度控制的ToString
		/// @tparam T 浮点数类型
		/// @param[in] a_value 需要转换的浮点数
		/// @param[in] n 精度位数
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

		/// @brief 数字转十六进制字符串
		/// @tparam T 数字类型
		/// @param value 数值
		/// @param n 对齐精度
		/// @return 十六进制字符串
		template<typename T, typename std::enable_if_t<std::is_integral_v<T>, int> = 0>
		std::string to_string_hex(const T value, const int n = 2)
		{
			std::ostringstream out;
			out << std::hex << std::uppercase << std::setw(n) << std::setfill('0') << value + 0;
			return out.str();
		}

		/// @brief 内存大小数字转带单位的字符串
		/// @tparam Numberic 数字类型
		/// @param num 数值
		/// @return 带单位字符串
		template<typename Numberic, class = std::enable_if_t<std::is_integral_v<Numberic>>>
		std::string MemoryToStringWithUnit(Numberic num)
		{
			if (num < 1024)
			{
				return std::to_string(num) + "B";
			}
			else if (num / 1024 < 1024) { //小于1MB，则转化成KB
				return to_string_with_precision(num / 1024.0) + "KB";
			}
			else if (num / (1024 * 1024) < 1024) { //小于1GB，则转化成MB
				return to_string_with_precision(num / (1024.0 * 1024.0)) + "MB";
			}
			else { //其他转化成GB
				return to_string_with_precision(num / (1024 * 1024 * 1024.0)) + "GB";
			}
		}

		/// @brief 网络包数量转带单位的字符串
		/// @tparam Numberic 数字类型
		/// @param num 数值
		/// @return 字符串
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
			else if (num / (1000 * 1000) < 1000) { //小于1GB，则转化成MB
				return to_string_with_precision(num / (1000.0 * 1000.0)) + " Mbps";
			}
			else { //其他转化成GB
				return to_string_with_precision(num / (1000.0 * 1000.0 * 1000.0)) + " Gbps";
			}
		}

		/// @brief 获取模块的函数指针
		/// @tparam Func 函数指针类型
		/// @param hModule 模块句柄
		/// @param FuncName 函数名
		/// @return 函数指针
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

		/// @brief GUID转字符串
		/// @param guid guid
		/// @return ANSI字符串
		std::string GUIDToString(const GUID& guid);
	}
}
