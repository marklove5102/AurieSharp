#include "GameObject.hpp"
#include "../Variable/GameVariable.hpp"
#include <msclr/marshal_cppstd.h>

using namespace msclr::interop;

namespace YYTKInterop
{
	bool GameObject::IsInstance()
	{
		return this->m_Object->m_ObjectKind == YYTK::OBJECT_KIND_CINSTANCE;
	}

	System::String^ GameObject::Name::get()
	{
		YYTK::RValue value = this->m_Object;

		std::string rvalue_kind_name = YYTK::GetPrivateInterface()->RV_GetKindName(&value);
		std::string rvalue_specific_kind_name = YYTK::GetPrivateInterface()->RV_GetObjectSpecificKind(&value);

		if (!_stricmp(rvalue_kind_name.c_str(), rvalue_specific_kind_name.c_str()))
			return gcnew System::String(value.GetKindName().c_str());

		return gcnew System::String(std::format("{} {}", rvalue_kind_name, rvalue_specific_kind_name).c_str());
	}

	Gen::IReadOnlyDictionary<System::String^, GameVariable^>^ GameObject::Members::get()
	{
		YYTK::RValue my_struct = this->m_Object;
		std::map<std::string, YYTK::RValue*> my_map = my_struct.ToRefMap();

		auto managed_map = gcnew Gen::Dictionary<System::String^, GameVariable^>(static_cast<int>(my_map.size()));

		for (const auto& [Key, Value] : my_map)
		{
			System::String^ key = gcnew System::String(Key.c_str());
			GameVariable^ value = GameVariable::CreateFromRValue(*Value);
			managed_map->Add(key, value);
		}

		return managed_map;
	}

	void GameObject::AddMember(
		System::String^ Name,
		GameVariable^ Value
	)
	{
		this->m_Object->Add(marshal_as<std::string>(Name).c_str(), Value->ToRValue(), 0);
	}

	bool GameObject::RemoveMember(
		System::String^ Name
	)
	{
		std::string name = marshal_as<std::string>(Name);
		if (!YYTK::RValue(this->m_Object).ContainsValue(name))
			return false;

		YYTK::RValue result;
		YYTK::GetInterface()->CallBuiltinEx(
			result,
			"variable_struct_remove",
			reinterpret_cast<YYTK::CInstance*>(this->m_Object),
			reinterpret_cast<YYTK::CInstance*>(this->m_Object),
			{ this->m_Object, name.c_str() }
		);
		
		return true;
	}

	GameVariable^ GameObject::default::get(System::String^ Name)
	{
		YYTK::RValue self = YYTK::RValue(this->m_Object);

		YYTK::RValue* native_value = self.GetRefMember(marshal_as<std::string>(Name));
		if (!native_value)
			throw gcnew System::InvalidCastException("Cannot access non-existing member of a struct variable!");

		return GameVariable::CreateFromRValue(*native_value);
	}

	void GameObject::default::set(System::String^ Name, GameVariable^ Value)
	{
		YYTK::RValue self = YYTK::RValue(this->m_Object);

		self[marshal_as<std::string>(Name)] = Value->ToRValue();
	}
}