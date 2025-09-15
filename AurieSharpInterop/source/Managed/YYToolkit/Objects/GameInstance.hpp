#pragma once
#include <YYToolkit/YYTK_Shared.hpp>
#include "GameObject.hpp"

namespace YYTKInterop
{
	using namespace System::Runtime::InteropServices;

	ref class GameVariable;

	// CInstance
	public ref class GameInstance
	{
	internal:
		YYTK::CInstance* m_Object;

		void ThrowIfInvalidNativeInstance();
		YYTK::CInstance* GetNativeInstance();

		GameInstance(YYTK::CInstance* Instance);
	public:
		static GameInstance^ FromInstanceID(int InstanceID);

		static GameInstance^ FromObject(GameObject^ Object);

		void AddMember(
			System::String^ Name,
			GameVariable^ Value
		);

		bool RemoveMember(
			System::String^ Name
		);

		property System::String^ Name
		{
			System::String^ get();
		}

		property double X
		{
			double get();
			void set(double NewValue);
		}

		property double Y
		{
			double get();
			void set(double NewValue);
		}

		property int ID
		{
			int get();
		}

		property bool Visible
		{
			bool get();
			void set(bool NewValue);
		}

		property Gen::IReadOnlyDictionary<System::String^, GameVariable^>^ Members
		{
			Gen::IReadOnlyDictionary<System::String^, GameVariable^>^ get();
		}

		property GameVariable^ default[System::String^]
		{
			GameVariable^ get(System::String ^ name);
			void set(System::String ^ name, GameVariable ^ value);
		}
	};
}