﻿/***********************************************************************
!!!!!! DO NOT MODIFY !!!!!!

GacGen.exe MainWindow.xml

This file is generated by Workflow compiler
https://github.com/vczh-libraries
***********************************************************************/

#include "HardwareReflection.h"

#if defined( _MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#elif defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wparentheses-equality"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#endif

/***********************************************************************
Reflection
***********************************************************************/

namespace vl
{
	namespace reflection
	{
		namespace description
		{
#ifndef VCZH_DEBUG_NO_REFLECTION
			IMPL_CPP_TYPE_INFO(Hardware::MainWindow)
			IMPL_CPP_TYPE_INFO(Hardware::MainWindowConstructor)
			IMPL_CPP_TYPE_INFO(Hardware::UI::CPUCoreTreeInternalProperty)
			IMPL_CPP_TYPE_INFO(Hardware::UI::HardwareTreeInternalProperty)
			IMPL_CPP_TYPE_INFO(Hardware::UI::IntWrapper)
			IMPL_CPP_TYPE_INFO(Hardware::UI::TreeViewItemData)

#ifdef VCZH_DESCRIPTABLEOBJECT_WITH_METADATA
#define _ ,
			BEGIN_CLASS_MEMBER(::Hardware::MainWindow)
				CLASS_MEMBER_BASE(::vl::presentation::controls::GuiWindow)
				CLASS_MEMBER_BASE(::Hardware::MainWindowConstructor)
				CLASS_MEMBER_CONSTRUCTOR(::Hardware::MainWindow*(), NO_PARAMETER)
				CLASS_MEMBER_METHOD(__vwsn_instance_ctor_, NO_PARAMETER)
				CLASS_MEMBER_METHOD(GetCustomIcon, NO_PARAMETER)
				CLASS_MEMBER_METHOD(GetListNodesToBind, NO_PARAMETER)
				CLASS_MEMBER_METHOD(GetnodesToBind, NO_PARAMETER)
				CLASS_MEMBER_METHOD(SetCustomIcon, { L"__vwsn_value_" })
				CLASS_MEMBER_METHOD(SetListNodesToBind, { L"__vwsn_value_" })
				CLASS_MEMBER_METHOD(SetnodesToBind, { L"__vwsn_value_" })
				CLASS_MEMBER_METHOD(TreeView_Selection_Changed, { L"sender" _ L"arguments" })
				CLASS_MEMBER_EVENT(CustomIconChanged)
				CLASS_MEMBER_EVENT(ListNodesToBindChanged)
				CLASS_MEMBER_EVENT(nodesToBindChanged)
				CLASS_MEMBER_FIELD(__vwsn_prop_CustomIcon)
				CLASS_MEMBER_FIELD(__vwsn_prop_ListNodesToBind)
				CLASS_MEMBER_FIELD(__vwsn_prop_nodesToBind)
				CLASS_MEMBER_PROPERTY_EVENT(CustomIcon, GetCustomIcon, SetCustomIcon, CustomIconChanged)
				CLASS_MEMBER_PROPERTY_EVENT(ListNodesToBind, GetListNodesToBind, SetListNodesToBind, ListNodesToBindChanged)
				CLASS_MEMBER_PROPERTY_EVENT(nodesToBind, GetnodesToBind, SetnodesToBind, nodesToBindChanged)
			END_CLASS_MEMBER(::Hardware::MainWindow)

			BEGIN_CLASS_MEMBER(::Hardware::MainWindowConstructor)
				CLASS_MEMBER_BASE(::vl::reflection::DescriptableObject)
				CLASS_MEMBER_CONSTRUCTOR(::vl::Ptr<::Hardware::MainWindowConstructor>(), NO_PARAMETER)
				CLASS_MEMBER_METHOD(__vwsn_Hardware_MainWindow_Initialize, { L"__vwsn_this_" })
				CLASS_MEMBER_FIELD(__vwsn_precompile_0)
				CLASS_MEMBER_FIELD(__vwsn_precompile_1)
				CLASS_MEMBER_FIELD(__vwsn_precompile_2)
				CLASS_MEMBER_FIELD(__vwsn_precompile_3)
				CLASS_MEMBER_FIELD(__vwsn_precompile_4)
				CLASS_MEMBER_FIELD(__vwsn_precompile_5)
				CLASS_MEMBER_FIELD(__vwsn_precompile_6)
				CLASS_MEMBER_FIELD(__vwsn_precompile_7)
				CLASS_MEMBER_FIELD(__vwsn_precompile_8)
				CLASS_MEMBER_FIELD(bindableListView)
				CLASS_MEMBER_FIELD(bindableTreeView)
				CLASS_MEMBER_FIELD(self)
			END_CLASS_MEMBER(::Hardware::MainWindowConstructor)

			BEGIN_CLASS_MEMBER(::Hardware::UI::CPUCoreTreeInternalProperty)
				CLASS_MEMBER_BASE(::vl::reflection::DescriptableObject)
				CLASS_MEMBER_CONSTRUCTOR(::vl::Ptr<::Hardware::UI::CPUCoreTreeInternalProperty>(::vl::vint, ::vl::vint, ::vl::Ptr<::vl::reflection::description::IValueObservableList>), { L"coreid" _ L"socketid" _ L"threadIds" })
				CLASS_MEMBER_METHOD(GetCoreId, NO_PARAMETER)
				CLASS_MEMBER_METHOD(GetName, NO_PARAMETER)
				CLASS_MEMBER_METHOD(GetSocketId, NO_PARAMETER)
				CLASS_MEMBER_METHOD(GetThreadIds, NO_PARAMETER)
				CLASS_MEMBER_METHOD(SetCoreId, { L"__vwsn_value_" })
				CLASS_MEMBER_METHOD(SetName, { L"__vwsn_value_" })
				CLASS_MEMBER_METHOD(SetSocketId, { L"__vwsn_value_" })
				CLASS_MEMBER_METHOD(SetThreadIds, { L"__vwsn_value_" })
				CLASS_MEMBER_EVENT(CoreIdChanged)
				CLASS_MEMBER_EVENT(SocketIdChanged)
				CLASS_MEMBER_EVENT(ThreadIdsChanged)
				CLASS_MEMBER_FIELD(__vwsn_prop_CoreId)
				CLASS_MEMBER_FIELD(__vwsn_prop_Name)
				CLASS_MEMBER_FIELD(__vwsn_prop_SocketId)
				CLASS_MEMBER_FIELD(__vwsn_prop_ThreadIds)
				CLASS_MEMBER_PROPERTY_EVENT(CoreId, GetCoreId, SetCoreId, CoreIdChanged)
				CLASS_MEMBER_PROPERTY(Name, GetName, SetName)
				CLASS_MEMBER_PROPERTY_EVENT(SocketId, GetSocketId, SetSocketId, SocketIdChanged)
				CLASS_MEMBER_PROPERTY_EVENT(ThreadIds, GetThreadIds, SetThreadIds, ThreadIdsChanged)
			END_CLASS_MEMBER(::Hardware::UI::CPUCoreTreeInternalProperty)

			BEGIN_CLASS_MEMBER(::Hardware::UI::HardwareTreeInternalProperty)
				CLASS_MEMBER_BASE(::vl::reflection::DescriptableObject)
				CLASS_MEMBER_CONSTRUCTOR(::vl::Ptr<::Hardware::UI::HardwareTreeInternalProperty>(::vl::vint), { L"id" })
				CLASS_MEMBER_CONSTRUCTOR(::vl::Ptr<::Hardware::UI::HardwareTreeInternalProperty>(::vl::vint, const ::vl::WString&), { L"id" _ L"name" })
				CLASS_MEMBER_METHOD(GetId, NO_PARAMETER)
				CLASS_MEMBER_METHOD(GetName, NO_PARAMETER)
				CLASS_MEMBER_METHOD(SetId, { L"__vwsn_value_" })
				CLASS_MEMBER_METHOD(SetName, { L"__vwsn_value_" })
				CLASS_MEMBER_EVENT(IdChanged)
				CLASS_MEMBER_FIELD(__vwsn_prop_Id)
				CLASS_MEMBER_FIELD(__vwsn_prop_Name)
				CLASS_MEMBER_PROPERTY_EVENT(Id, GetId, SetId, IdChanged)
				CLASS_MEMBER_PROPERTY(Name, GetName, SetName)
			END_CLASS_MEMBER(::Hardware::UI::HardwareTreeInternalProperty)

			BEGIN_CLASS_MEMBER(::Hardware::UI::IntWrapper)
				CLASS_MEMBER_BASE(::vl::reflection::DescriptableObject)
				CLASS_MEMBER_CONSTRUCTOR(::vl::Ptr<::Hardware::UI::IntWrapper>(::vl::vint), { L"value" })
				CLASS_MEMBER_METHOD(GetValue, NO_PARAMETER)
				CLASS_MEMBER_METHOD(SetValue, { L"__vwsn_value_" })
				CLASS_MEMBER_EVENT(ValueChanged)
				CLASS_MEMBER_FIELD(__vwsn_prop_Value)
				CLASS_MEMBER_PROPERTY_EVENT(Value, GetValue, SetValue, ValueChanged)
			END_CLASS_MEMBER(::Hardware::UI::IntWrapper)

			BEGIN_CLASS_MEMBER(::Hardware::UI::TreeViewItemData)
				CLASS_MEMBER_BASE(::vl::reflection::DescriptableObject)
				CLASS_MEMBER_CONSTRUCTOR(::vl::Ptr<::Hardware::UI::TreeViewItemData>(), NO_PARAMETER)
				CLASS_MEMBER_CONSTRUCTOR(::vl::Ptr<::Hardware::UI::TreeViewItemData>(const ::vl::WString&, const ::vl::WString&), { L"name" _ L"desc" })
				CLASS_MEMBER_METHOD(GetDesc, NO_PARAMETER)
				CLASS_MEMBER_METHOD(GetName, NO_PARAMETER)
				CLASS_MEMBER_METHOD(SetDesc, { L"__vwsn_value_" })
				CLASS_MEMBER_METHOD(SetName, { L"__vwsn_value_" })
				CLASS_MEMBER_FIELD(__vwsn_prop_Desc)
				CLASS_MEMBER_FIELD(__vwsn_prop_Name)
				CLASS_MEMBER_PROPERTY(Desc, GetDesc, SetDesc)
				CLASS_MEMBER_PROPERTY(Name, GetName, SetName)
			END_CLASS_MEMBER(::Hardware::UI::TreeViewItemData)

#undef _
			class HardwareTypeLoader : public Object, public ITypeLoader
			{
			public:
				void Load(ITypeManager* manager)
				{
					ADD_TYPE_INFO(::Hardware::MainWindow)
					ADD_TYPE_INFO(::Hardware::MainWindowConstructor)
					ADD_TYPE_INFO(::Hardware::UI::CPUCoreTreeInternalProperty)
					ADD_TYPE_INFO(::Hardware::UI::HardwareTreeInternalProperty)
					ADD_TYPE_INFO(::Hardware::UI::IntWrapper)
					ADD_TYPE_INFO(::Hardware::UI::TreeViewItemData)
				}

				void Unload(ITypeManager* manager)
				{
				}
			};
#endif
#endif

			bool LoadHardwareTypes()
			{
#ifdef VCZH_DESCRIPTABLEOBJECT_WITH_METADATA
				if (auto manager = GetGlobalTypeManager())
				{
					return manager->AddTypeLoader(Ptr(new HardwareTypeLoader));
				}
#endif
				return false;
			}
		}
	}
}

#if defined( _MSC_VER)
#pragma warning(pop)
#elif defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
