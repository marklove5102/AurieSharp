#pragma once
#include <YYToolkit/YYTK_Shared.hpp>
namespace YYTKInterop
{
	using namespace System::Runtime::InteropServices;
	namespace Gen = System::Collections::Generic;

	ref class GameVariable;

	// YYObjectBase
	public ref class GameObject
	{
	internal:
		YYTK::YYObjectBase* m_Object;
		GameObject(YYTK::YYObjectBase* Object)
		{
			m_Object = Object;
		}
	public:
		bool IsInstance();

		void AddMember(
			System::String^ Name,
			GameVariable^ Value
		);

		bool RemoveMember(
			System::String^ Name
		);

		property System::String^ Name
		{
			System::String ^ get();
		}

		property Gen::IReadOnlyDictionary<System::String^, GameVariable^>^ Members
		{
			Gen::IReadOnlyDictionary<System::String^, GameVariable^> ^ get();
		}

		property GameVariable^ default[System::String^]
		{
			GameVariable^ get(System::String ^ name);
			void set(System::String ^ name, GameVariable ^ value);
		}
	};
}