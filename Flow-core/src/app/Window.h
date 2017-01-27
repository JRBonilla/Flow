#pragma once

#include <iostream>
#include <Windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../math/Math.h"
#include "../audio/SoundManager.h"
#include "../input/InputManager.h"
#include "../graphics/fonts/FontManager.h"

namespace fl {
	
	struct WindowProperties {
		uint width, height;
		bool fullscreen;
		bool vsync;
	};

	class Window {
	private:
		WindowProperties m_Properties;
		GLFWwindow*		 m_Window;
		int				 m_WindowX, m_WindowY;
		int				 m_ViewPortWidth, m_ViewPortHeight;
		const char*		 m_Title;
	public:
		Window(const char* title, const WindowProperties& properties);
		Window(const char* title, uint width, uint height, bool fullscreen = false, bool vsync = true);
		~Window();

		void Update();
		void ClearBuffer() const;
		void SwapBuffers() const;
		bool Closed() const;

		inline int GetWidth()  const { return m_Properties.width; }
		inline int GetHeight() const { return m_Properties.height; }

		inline GLFWwindow* GetGLFWWindow() const { return m_Window; }

		inline void SetClearColor(const math::vec4& color) const { glClearColor(color.x, color.y, color.z, color.w); }
	private:
		bool Init();
		void CenterWindowOnScreen();

		static void PrintOpenGLVersion();
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void CursorPositionCallback(GLFWwindow* window, double x, double y);
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	};

}
