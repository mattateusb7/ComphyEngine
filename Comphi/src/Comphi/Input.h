#pragma once
#include "Comphi/Core.h"

namespace Comphi {

	class COMPHI_API Input
	{
	public:
		~Input() { delete s_instance; };
		inline static bool IsKeyPressed(int keycode) { return s_instance->IsKeyPressedImpl(keycode); };
		inline static bool IsMouseButtonPressed(int button) { return s_instance->IsMouseButtonPressedImpl(button); };
		inline static std::pair<int, int> GetMousePos() { return s_instance->GetMousePosImpl(); };
		inline static int GetMouseX() { return s_instance->GetMouseXImpl(); };
		inline static int GetMouseY() { return s_instance->GetMouseYImpl(); };
	protected:
		virtual bool IsKeyPressedImpl(int KeyCode) = 0;
		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<int, int> GetMousePosImpl() = 0;
		virtual int GetMouseXImpl() = 0;
		virtual int GetMouseYImpl() = 0;
	private:
		static Input* s_instance;
	};

}