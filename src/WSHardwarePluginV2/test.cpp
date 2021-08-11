#include "stdafx.h"
#include "test.h"

#ifndef _WINDLL

#include <comutil.h>
#include "WSHardwarePluginV2.h"

namespace Hardware
{
	namespace Test
	{
#if defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif
		namespace DiskTest
		{
			std::map<int, std::vector<std::string>> PaserInitJson(const std::string& JsonValue)
			{
				Json::Reader reader;
				Json::Value root;
				std::map<int, std::vector<std::string>> DriverMap;
				if (reader.parse(JsonValue, root))
				{
					if (root.isNull())
						return DriverMap;
					for (const auto& temp : root)
					{
						std::vector<std::string> StrTemp;

						try
						{
							for (const auto& Str : temp["Elements"])
							{
								StrTemp.push_back(Str.asString());
							}
						}
						catch (...)
						{
						}

						try
						{
							for (const auto& Str : temp["UpdateElements"])
							{
								StrTemp.push_back(Str.asString());
							}
						}
						catch (...)
						{
						}
						DriverMap.emplace(temp["id"].asInt(), std::move(StrTemp));
					}
				}
				return DriverMap;
			}

			Json::Value BuildGetJSON(int CurrentId, const std::vector<std::string>& DiskDisplayName)
			{
				Json::Value temp;
				temp["id"] = CurrentId;
				Json::Value Ele;
				for (const auto& name : DiskDisplayName)
				{
					Ele.append(name);
				}
				temp["Elements"] = Ele;
				return temp;
			}

			Json::Value BuildUpdateJSON(int CurrentId)
			{
				Json::Value temp;
				temp["id"] = CurrentId;
				Json::Value Ele;
				constexpr auto name = "Temperature";
				Ele.append(name);
				temp["Elements"] = Ele;
				return temp;
			}

			void GetDiskTest()
			{
				PcmHardwareInitialize(nullptr, nullptr);

				BSTR data{};
				PcmHardwareAction(PCM_HARDWARE_ACTION_DISK_INIT, nullptr, &data);
				auto DriverMap = PaserInitJson(_com_util::ConvertBSTRToString(data));
				SysFreeString(data);

				for (const auto& Driver : DriverMap)
				{
					PcmHardwareAction(PCM_HARDWARE_ACTION_DISK_GET, Json::FastWriter().write(BuildGetJSON(Driver.first, Driver.second)).c_str(), &data);
					auto Str = _com_util::ConvertBSTRToString(data);
					SysFreeString(data);

					Json::Reader arrayReader;
					Json::Value arrayValue;
					if (arrayReader.parse(Str, arrayValue))
					{
						for (const auto& node : arrayValue)
						{
							Json::Value::Members arrayMember = node.getMemberNames();
							for (auto iter = arrayMember.begin(); iter != arrayMember.end(); ++iter) //遍历json成员
							{
								std::string member_name = *iter;
								std::string value_str = node[member_name].asString();
								std::cout << member_name << ":" << value_str << std::endl;
							}
						}
					}

					for (int i = 0; i < 10; ++i)
					{
						BSTR data{};
						PcmHardwareAction(PCM_HARDWARE_ACTION_DISK_UPDATE, Json::FastWriter().write(BuildUpdateJSON(Driver.first)).c_str(), &data);
						auto Str = _com_util::ConvertBSTRToString(data);
						SysFreeString(data);

						Json::Reader arrayReader;
						Json::Value arrayValue;
						if (arrayReader.parse(Str, arrayValue))
						{
							for (const auto& node : arrayValue)
							{
								Json::Value::Members arrayMember = node.getMemberNames();
								for (auto iter = arrayMember.begin(); iter != arrayMember.end(); ++iter) //遍历json成员
								{
									std::string member_name = *iter;
									std::string value_str = node[member_name].asString();
									std::cout << member_name << ":" << value_str << std::endl;
								}
							}
						}
						Sleep(1000);
					}
				}
			}
		}

		namespace
		{
			void PrintElementJsonObject(const Json::Value& Ele)
			{
				for (const auto& node : Ele)
				{
					Json::Value::Members arrayMember = node.getMemberNames();
					for (auto iter = arrayMember.begin(); iter != arrayMember.end(); ++iter) //遍历json成员
					{
						std::string member_name = *iter;
						if (node[member_name].isString())
						{
							std::string value_str = node[member_name].asString();
							std::cout << member_name << ":" << value_str << std::endl;
						}
						else if (node[member_name].isUInt())
						{
							auto value_str = node[member_name].asUInt();
							std::cout << member_name << ":" << value_str << std::endl;
						}
						else if (node[member_name].isArray())
						{
							for (const auto& SubNode : node[member_name])
							{
								std::cout << member_name << ":" << std::endl;
								auto SubarrayMember = SubNode.getMemberNames();
								for (auto subiter = SubarrayMember.begin(); subiter != SubarrayMember.end(); ++subiter) //遍历json成员
								{
									std::string Submember_name = *subiter;
									if (SubNode[Submember_name].isString())
									{
										std::string value_str = SubNode[Submember_name].asString();
										std::cout << "\t" << Submember_name << ":" << value_str << std::endl;
									}
									else if (SubNode[Submember_name].isUInt())
									{
										auto value_str = SubNode[Submember_name].asUInt();
										std::cout << "\t" << Submember_name << ":" << value_str << std::endl;
									}
								}
							}
						}
						else
						{
							_CrtDbgBreak();
						}
					}
				}
			}

			void PrintElementJsonInfo(const std::string& InitJson)
			{
				try
				{
					Json::Reader reader;
					Json::Value root;
					if (reader.parse(InitJson, root))
					{
						for (const auto& node : root)
						{
							PrintElementJsonObject(node);
						}
					}
				}
				catch (Json::Exception& e)
				{
					return;
				}
			}

			void PrintElementJsonInfoNoArray(const std::string& InitJson)
			{
				try
				{
					Json::Reader reader;
					Json::Value root;
					if (reader.parse(InitJson, root))
					{
						PrintElementJsonObject(root);
					}
				}
				catch (Json::Exception& e)
				{
					return;
				}
			}
		}

		namespace CPUTest
		{
			std::string BuildUpdateJsonFromInitializeJson(const std::string& InitializeJson)
			{
				try
				{
					Json::Reader reader;
					Json::Value root;
					Json::Value UpdateJson;
					if (reader.parse(InitializeJson, root))
					{
						Json::Value temp;
						temp["SocketId"] = root["SocketId"];
						for (auto& CoreEle : root["CoreIds"])
						{
							bool First = true;
							temp["CoreId"] = CoreEle["CoreId"];
							for (const auto& ThreadEle : CoreEle["ThreadIds"])
							{
								if (First)
								{
									temp["ThreadId"] = ThreadEle["ThreadId"];
									First = false;
								}
							}
							UpdateJson.append(temp);
						}
						return Json::FastWriter().write(UpdateJson);
					}
				}
				catch (Json::Exception&)
				{
				}
				return{};
			}

			std::string BuildElementJsonFromInitializeJson(const std::string& InitializeJson)
			{
				try
				{
					Json::Reader reader;
					Json::Value root;
					if (reader.parse(InitializeJson, root))
					{
						Json::Value UpdateJson;
						UpdateJson["SocketId"] = root["SocketId"];
						return Json::FastWriter().write(UpdateJson);
					}
				}
				catch (Json::Exception&)
				{
				}
				return{};
			}

			void PrintUpdateJsonInfo(const std::string& UpdateJson)
			{
				try
				{
					Json::Reader reader;
					Json::Value root;
					if (reader.parse(UpdateJson, root))
					{
						auto SocRoot = root["Socket"];
						auto ThreadRoot = root["Thread"];

						std::cout << "Socket" << std::endl;
						for (auto& soc : SocRoot)
						{
							for (auto& ele : soc["Element"])
							{
								std::cout << ele["Name"].asString() << " " << ele["Value"].asString() << std::endl;
							}
						}

						std::cout << "Thread" << std::endl;
						for (auto& Thread : ThreadRoot)
						{
							for (auto& ele : Thread["Element"])
							{
								std::cout << "Thread " << Thread["ThreadId"].asInt() << " " << ele["Name"].asString() << " " << ele["Value"].asString() << std::endl;
							}
						}
					}
				}
				catch (Json::Exception& e)
				{
					return;
				}
			}

			void PrintIniaializeJson(const std::string& InitializeJson)
			{
				try
				{
					Json::Reader reader;
					Json::Value root;
					Json::Value UpdateJson;
					if (reader.parse(InitializeJson, root))
					{
						std::cout << "SocketId:" << root["SocketId"] << std::endl;
						for (auto& CoreEle : root["CoreIds"])
						{
							std::cout << "\tCocketId:" << CoreEle["CoreId"] << std::endl;
							for (const auto& ThreadEle : CoreEle["ThreadIds"])
							{
								std::cout << "\t\tThreadId:" << ThreadEle["ThreadId"] << std::endl;
							}
						}
					}
				}
				catch (Json::Exception&)
				{
				}
			}

			void GetCPUTest()
			{
				PcmHardwareInitialize(nullptr, nullptr);
				BSTR data{};

				PcmHardwareAction(PCM_HARDWARE_ACTION_CPU_INIT, nullptr, &data);
				auto Str = _com_util::ConvertBSTRToString(data);
				SysFreeString(data);
				auto InitializeJson = BuildUpdateJsonFromInitializeJson(Str);
				auto GetElementJson = BuildElementJsonFromInitializeJson(Str);
				PrintIniaializeJson(Str);
				delete[] Str;
				PcmHardwareAction(PCM_HARDWARE_ACTION_CPU_GET, GetElementJson.c_str(), &data);
				Str = _com_util::ConvertBSTRToString(data);
				SysFreeString(data);
				PrintElementJsonInfo(Str);
				delete[] Str;

				Sleep(1000);
				for (int i = 0; i < 10; ++i)
				{
					PcmHardwareAction(PCM_HARDWARE_ACTION_CPU_UPDATE, InitializeJson.c_str(), &data);
					Str = _com_util::ConvertBSTRToString(data);
					SysFreeString(data);
					PrintUpdateJsonInfo(Str);
					delete[] Str;
					Sleep(1000);
				}
			}
		}

		namespace MemoryTest
		{
			std::string BuildJsonFromInitializeJson(const std::string& InitializeJson)
			{
				try
				{
					Json::Reader reader;
					Json::Value root;

					if (reader.parse(InitializeJson, root))
					{
						for (const auto& MemoryEle : root["MemoryIds"])
						{
							if (MemoryEle.isUInt())
							{
								Json::Value UpdateJson;
								UpdateJson["MemoryId"] = MemoryEle;
								return Json::FastWriter().write(UpdateJson);
							}
						}
					}
				}
				catch (Json::Exception&)
				{
				}
				return{};
			}

			void GetMemoryTest()
			{
				PcmHardwareInitialize(nullptr, nullptr);
				BSTR data{};
				PcmHardwareAction(PCM_HARDWARE_ACTION_MEMORY_INIT, nullptr, &data);
				auto Str = _com_util::ConvertBSTRToString(data);
				SysFreeString(data);
				auto InitializeJson = BuildJsonFromInitializeJson(Str);
				if (Str)
					delete[] Str;
				PcmHardwareAction(PCM_HARDWARE_ACTION_MEMORY_GET, InitializeJson.c_str(), &data);
				Str = _com_util::ConvertBSTRToString(data);
				SysFreeString(data);
				PrintElementJsonInfoNoArray(Str);
				if (Str)
					delete[] Str;
			}
		}

		namespace BroadTest
		{
			std::string BuildJsonFromInitializeJson(const std::string& InitializeJson)
			{
				try
				{
					Json::Reader reader;
					Json::Value root;
					Json::Value retValue;

					if (reader.parse(InitializeJson, root))
					{
						for (const auto& MemoryEle : root["BroadId"])
						{
							if (MemoryEle.isUInt())
							{
								retValue["BroadId"] = MemoryEle;
							}
						}
					}
					return Json::FastWriter().write(retValue);
				}
				catch (Json::Exception&)
				{
				}
				return{};
			}

			void GetBroadTest()
			{
				PcmHardwareInitialize(nullptr, nullptr);
				BSTR data{};
				PcmHardwareAction(PCM_HARDWARE_ACTION_BROAD_INIT, nullptr, &data);
				auto Str = _com_util::ConvertBSTRToString(data);
				SysFreeString(data);
				auto InitializeJson = BuildJsonFromInitializeJson(Str);
				if (Str)
					delete[] Str;
				PcmHardwareAction(PCM_HARDWARE_ACTION_BROAD_GET, InitializeJson.c_str(), &data);
				Str = _com_util::ConvertBSTRToString(data);
				try
				{
					Json::Reader reader;
					Json::Value root;
					if (reader.parse(Str, root))
					{
						if (root["Broad"].isArray())
							PrintElementJsonObject(root["Broad"]);
					}
				}
				catch (Json::Exception& e)
				{
					return;
				}
				SysFreeString(data);
				if (Str)
					delete[] Str;
			}
		}

		namespace BiosTest
		{
			std::string BuildJsonFromInitializeJson(const std::string& InitializeJson)
			{
				try
				{
					Json::Reader reader;
					Json::Value root;
					Json::Value retValue;

					if (reader.parse(InitializeJson, root))
					{
						for (const auto& MemoryEle : root["BiosId"])
						{
							if (MemoryEle.isUInt())
							{
								retValue["BiosId"] = MemoryEle;
							}
						}
					}
					return Json::FastWriter().write(retValue);
				}
				catch (Json::Exception&)
				{
				}
				return{};
			}

			void GetBiosTest()
			{
				PcmHardwareInitialize(nullptr, nullptr);
				BSTR data{};
				PcmHardwareAction(PCM_HARDWARE_ACTION_BIOS_INIT, nullptr, &data);
				auto Str = _com_util::ConvertBSTRToString(data);
				SysFreeString(data);
				auto InitializeJson = BuildJsonFromInitializeJson(Str);
				if (Str)
					delete[] Str;
				PcmHardwareAction(PCM_HARDWARE_ACTION_BIOS_GET, InitializeJson.c_str(), &data);
				Str = _com_util::ConvertBSTRToString(data);
				try
				{
					Json::Reader reader;
					Json::Value root;
					if (reader.parse(Str, root))
					{
						if (root["Bios"].isArray())
							PrintElementJsonObject(root["Bios"]);
					}
				}
				catch (Json::Exception& e)
				{
					return;
				}
				SysFreeString(data);
				if (Str)
					delete[] Str;
			}
		}

		namespace AudioTest
		{
			std::string BuildJsonFromInitializeJson(const std::string& InitializeJson)
			{
				try
				{
					Json::Reader reader;
					Json::Value root;

					if (reader.parse(InitializeJson, root))
					{
						for (const auto& MemoryEle : root["AudioIds"])
						{
							if (MemoryEle.isUInt())
							{
								Json::Value UpdateJson;
								UpdateJson["AudioId"] = MemoryEle;
								return Json::FastWriter().write(UpdateJson);
							}
						}
					}
				}
				catch (Json::Exception&)
				{
				}
				return{};
			}

			std::string BuildMicJsonFromInitializeJson(const std::string& InitializeJson)
			{
				try
				{
					Json::Reader reader;
					Json::Value root;

					if (reader.parse(InitializeJson, root))
					{
						for (const auto& MemoryEle : root["MicIds"])
						{
							if (MemoryEle.isUInt())
							{
								Json::Value UpdateJson;
								UpdateJson["MicId"] = MemoryEle;
								return Json::FastWriter().write(UpdateJson);
							}
						}
					}
				}
				catch (Json::Exception&)
				{
				}
				return{};
			}

			void GetAudioTest()
			{
				PcmHardwareInitialize(nullptr, nullptr);
				BSTR data{};
				PcmHardwareAction(PCM_HARDWARE_ACTION_AUDIO_INIT, nullptr, &data);
				auto Str = _com_util::ConvertBSTRToString(data);
				SysFreeString(data);
				auto AudioJson = BuildJsonFromInitializeJson(Str);
				auto MicJson = BuildMicJsonFromInitializeJson(Str);
				if (Str)
					delete[] Str;
				PcmHardwareAction(PCM_HARDWARE_ACTION_AUDIO_GET, AudioJson.c_str(), &data);
				Str = _com_util::ConvertBSTRToString(data);
				PrintElementJsonInfoNoArray(Str);
				SysFreeString(data);
				if (Str)
					delete[] Str;

				PcmHardwareAction(PCM_HARDWARE_ACTION_AUDIO_GET, MicJson.c_str(), &data);
				Str = _com_util::ConvertBSTRToString(data);
				PrintElementJsonInfoNoArray(Str);
				SysFreeString(data);
				if (Str)
					delete[] Str;
			}
		}

		namespace NICTest
		{
			std::string BuildJsonFromInitializeJson(const std::string& InitializeJson)
			{
				try
				{
					Json::Reader reader;
					Json::Value root;

					if (reader.parse(InitializeJson, root))
					{
						for (const auto& MemoryEle : root["NICIds"])
						{
							if (MemoryEle.isUInt())
							{
								Json::Value UpdateJson;
								UpdateJson["NICId"] = MemoryEle;
								return Json::FastWriter().write(UpdateJson);
							}
						}
					}
				}
				catch (Json::Exception&)
				{
				}
				return{};
			}

			void GetNICTest()
			{
				PcmHardwareInitialize(nullptr, nullptr);
				BSTR data{};
				PcmHardwareAction(PCM_HARDWARE_ACTION_NIC_INIT, nullptr, &data);
				auto Str = _com_util::ConvertBSTRToString(data);
				SysFreeString(data);
				auto InitializeJson = BuildJsonFromInitializeJson(Str);
				if (Str)
					delete[] Str;
				PcmHardwareAction(PCM_HARDWARE_ACTION_NIC_GET, InitializeJson.c_str(), &data);
				Str = _com_util::ConvertBSTRToString(data);
				PrintElementJsonInfoNoArray(Str);
				SysFreeString(data);
				if (Str)
					delete[] Str;
			}
		}

		namespace BatteryTest
		{
			std::string BuildJsonFromInitializeJson(const std::string& InitializeJson)
			{
				try
				{
					Json::Reader reader;
					Json::Value root;

					if (reader.parse(InitializeJson, root))
					{
						for (const auto& MemoryEle : root["BatteryIds"])
						{
							if (MemoryEle.isUInt())
							{
								Json::Value UpdateJson;
								UpdateJson["BatteryId"] = MemoryEle;
								return Json::FastWriter().write(UpdateJson);
							}
						}
					}
				}
				catch (Json::Exception&)
				{
				}
				return{};
			}

			void GetBatteryTest()
			{
				PcmHardwareInitialize(nullptr, nullptr);
				BSTR data{};
				PcmHardwareAction(PCM_HARDWARE_ACTION_BATTERY_INIT, nullptr, &data);
				auto Str = _com_util::ConvertBSTRToString(data);
				SysFreeString(data);
				auto InitializeJson = BuildJsonFromInitializeJson(Str);
				if (Str)
					delete[] Str;
				PcmHardwareAction(PCM_HARDWARE_ACTION_BATTERY_UPDATE, InitializeJson.c_str(), &data);
				Str = _com_util::ConvertBSTRToString(data);
				PrintElementJsonInfoNoArray(Str);
				SysFreeString(data);
				if (Str)
					delete[] Str;
			}
		}

		namespace WinBioTest
		{
			std::string BuildJsonFromInitializeJson(const std::string& InitializeJson)
			{
				try
				{
					Json::Reader reader;
					Json::Value root;

					if (reader.parse(InitializeJson, root))
					{
						for (const auto& MemoryEle : root["WinBioIds"])
						{
							if (MemoryEle.isUInt())
							{
								Json::Value UpdateJson;
								UpdateJson["WinBioId"] = MemoryEle;
								return Json::FastWriter().write(UpdateJson);
							}
						}
					}
				}
				catch (Json::Exception&)
				{
				}
				return{};
			}

			void GetWinBioTest()
			{
				PcmHardwareInitialize(nullptr, nullptr);
				BSTR data{};
				PcmHardwareAction(PCM_HARDWARE_ACTION_WINBIO_INIT, nullptr, &data);
				auto Str = _com_util::ConvertBSTRToString(data);
				SysFreeString(data);
				auto InitializeJson = BuildJsonFromInitializeJson(Str);
				if (Str)
					delete[] Str;
				PcmHardwareAction(PCM_HARDWARE_ACTION_WINBIO_GET, InitializeJson.c_str(), &data);
				Str = _com_util::ConvertBSTRToString(data);
				PrintElementJsonInfoNoArray(Str);
				SysFreeString(data);
				if (Str)
					delete[] Str;
			}
		}

		namespace MonitorTest
		{
			std::string BuildJsonFromInitializeJson(const std::string& InitializeJson)
			{
				try
				{
					Json::Reader reader;
					Json::Value root;

					if (reader.parse(InitializeJson, root))
					{
						for (const auto& MemoryEle : root["MonitorIds"])
						{
							if (MemoryEle.isString())
							{
								Json::Value UpdateJson;
								UpdateJson["MonitorId"] = MemoryEle;
								return Json::FastWriter().write(UpdateJson);
							}
						}
					}
				}
				catch (Json::Exception&)
				{
				}
				return{};
			}

			void GetMonitorTest()
			{
				PcmHardwareInitialize(nullptr, nullptr);
				BSTR data{};
				PcmHardwareAction(PCM_HARDWARE_ACTION_MONITOR_INIT, nullptr, &data);
				auto Str = _com_util::ConvertBSTRToString(data);
				SysFreeString(data);
				auto InitializeJson = BuildJsonFromInitializeJson(Str);
				if (Str)
					delete[] Str;
				PcmHardwareAction(PCM_HARDWARE_ACTION_MONITOR_GET, InitializeJson.c_str(), &data);
				Str = _com_util::ConvertBSTRToString(data);
				PrintElementJsonInfoNoArray(Str);
				SysFreeString(data);
				if (Str)
					delete[] Str;
			}
		}

		namespace GPUTest
		{
			std::string BuildJsonFromInitializeJson(const std::string& InitializeJson)
			{
				try
				{
					Json::Reader reader;
					Json::Value root;

					if (reader.parse(InitializeJson, root))
					{
						for (const auto& MemoryEle : root["GPUIds"])
						{
							if (MemoryEle.isIntegral())
							{
								Json::Value UpdateJson;
								UpdateJson["GPUId"] = MemoryEle;
								return Json::FastWriter().write(UpdateJson);
							}
						}
					}
				}
				catch (Json::Exception&)
				{
				}
				return{};
			}

			void GetGPUTest()
			{
				PcmHardwareInitialize(nullptr, nullptr);
				BSTR data{};
				PcmHardwareAction(PCM_HARDWARE_ACTION_GPU_INIT, nullptr, &data);
				auto Str = _com_util::ConvertBSTRToString(data);
				SysFreeString(data);
				if (Str)
				{
					auto InitializeJson = BuildJsonFromInitializeJson(Str);
					if (Str)
						delete[] Str;
					int i = 0;
					while (i++ < 5)
					{
						PcmHardwareAction(PCM_HARDWARE_ACTION_GPU_UPDATE, InitializeJson.c_str(), &data);
						Str = _com_util::ConvertBSTRToString(data);
						PrintElementJsonInfoNoArray(Str);
						SysFreeString(data);
						if (Str)
							delete[] Str;
					}

					PcmHardwareAction(PCM_HARDWARE_ACTION_GPU_GET, InitializeJson.c_str(), &data);
					Str = _com_util::ConvertBSTRToString(data);
					PrintElementJsonInfoNoArray(Str);
					SysFreeString(data);
					if (Str)
						delete[] Str;
				}
			}
		}

#if defined(_MSC_VER)
#pragma warning(default : 4996)
#endif
	}
}

int main()
{
	if SUCCEEDED(CoInitialize(NULL))
	{
		// 	Hardware::Test::DiskTest::GetDiskTest();
		// 	Hardware::Test::CPUTest::GetCPUTest();
		//Hardware::Test::MemoryTest::GetMemoryTest();
		// 	Hardware::Test::BroadTest::GetBroadTest();
		// 	Hardware::Test::AudioTest::GetAudioTest();
		// 	Hardware::Test::NICTest::GetNICTest();
		// 	Hardware::Test::BatteryTest::GetBatteryTest();
		// 	Hardware::Test::WinBioTest::GetWinBioTest();
		// 	Hardware::Test::MonitorTest::GetMonitorTest();
		// 	Hardware::Test::BiosTest::GetBiosTest();
		Hardware::Test::GPUTest::GetGPUTest();
		CoUninitialize();
	}
}

#endif // !WINDLL