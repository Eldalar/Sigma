#pragma once
#ifndef OS_H
#define OS_H

#ifndef __APPLE__
#include <GL/glew.h>
#ifndef __unix
#include <GL/wglew.h>
#endif
#endif
#include <GLFW/glfw3.h>
#include <string>

#include "Sigma.h"
#include "systems/KeyboardInputSystem.h"
#include "systems/MouseInputSystem.h"

namespace Sigma {
	class OS {
	public:
		OS() : mouseLock(false) { }
		~OS() { }

		/**
		 * \brief Initialize the rendering window and makes the window's context the current one.
		 *
		 * \param[in] const unsigned int width, height The window's client width and height
		 * \param[in] const std::string title The title to show in the title bar and task manager.
		 * \return bool If creation was successful or not.
		 */
		DLL_EXPORT bool InitializeWindow(const int width, const int height, const std::string title, const unsigned int glMajor = 3, const unsigned int glMinor = 2);

		DLL_EXPORT void Terminate();

		/**
		 * \brief Checks if the window is closing.
		 *
		 * \return bool True if the window is closing.
		 */
		DLL_EXPORT bool Closing();

		/**
		 * \brief Swap the front and back buffers. Call after rendering.
		 *
		 * \return void
		 */
		DLL_EXPORT void SwapBuffers();

		/**
		 * \brief Processes events in the OS message event loop.
		 *
		 * \return void
		 */
		DLL_EXPORT void OSMessageLoop();

		DLL_EXPORT int GetWindowWidth();

		DLL_EXPORT int GetWindowHeight();

		/**
		 * \brief Returns the time since this last call to GetDeltaTime().
		 *
		 * \return double The number of seconds (or fractions of a second) that has passed.
		 */
		DLL_EXPORT double GetDeltaTime();

		/**
		 * \brief Callback for when the window is resized.
		 *
		 * \param[in] GLFWwindow * window
		 * \param[in] int width, height The new client width and height of the window.
		 * \return void
		 */
		static void windowResized(GLFWwindow* window, int width, int height);

		/**
		 * \brief Callback for keyboard events.
		 *
		 * \param[in] GLFWwindow * window
		 * \param[in] int key ASCII key number.
		 * \param[in] int scancode The converted key value
		 * \param[in] int action The event type.
		 * \param[in] int mods Modifier keys.
		 * \return void
		 */
		static void keyboardEvent(GLFWwindow* window, int key, int scancode, int action, int mods);

		/**
		 * \brief Callback for unicode character event.
		 *
		 * This is different from just a normal keyboard event as it has been translated and modified by
		 * the OS and is just like typing into a text editor.
		 * \param[in] GLFWwindow * window
		 * \param[in] unsigned int uchar The unicode character key code.
		 * \return void
		 */
		static void characterEvent(GLFWwindow* window, unsigned int uchar);

		/**
		 * \brief Callback for mouse move events.
		 *
		 * \param[in] GLFWwindow * window
		 * \param[in] double x, y The new x and y coordinate of the mouse in screen coordinates.
		 * \return void
		 */
		static void mouseMoveEvent(GLFWwindow* window, double x, double y);

		/**
		 * \brief Callback for mouse button events.
		 *
		 * \param[in/out] GLFWwindow * window
		 * \param[in] int button The button that the action occurred on.
		 * \param[in] int action The action that occurred.
		 * \param[in] int mods Modifier keys.
		 * \return void
		 */
		static void mouseButtonEvent(GLFWwindow* window, int button, int action, int mods);

		/**
		 * \brief Callback for window focus change events.
		 *
		 * \param[in/out] GLFWwindow * window
		 * \param[in/out] int focused GL_TRUE if focused, GL_FALSE if unfocused.
		 * \return void
		 */
		static void windowFocusChange(GLFWwindow* window, int focused);

		DLL_EXPORT void RegisterKeyboardEventHandler(event::IKeyboardEventHandler* handler);
		DLL_EXPORT void RegisterMouseEventHandler(event::IMouseEventHandler* handler);

		DLL_EXPORT bool HasKeyboardFocusLock();

		DLL_EXPORT void ToggleMouseLock();
		
		DLL_EXPORT void SetMousePosition(double x, double y);

		DLL_EXPORT bool CheckKeyState(event::KEY_STATE state, const int key);

	private:
		/**
		 * \brief Updates the internal size variables from the windowResized callback.
		 *
		 * \param[in] const int width, height The new client width and height of the window
		 * \return void
		 */
		void UpdateWindowSize(const int width, const int height );

		/**
		 * \brief Dispatches keyboard events from the callback.
		 *
		 * \param[in] const int key ASCII key number.
		 * \param[in] const int scancode The converted key value
		 * \param[in] const int action The event type.
		 * \param[in] const int mods Modifier keys.
		 * \return void
		 */
		void DispatchKeyboardEvent(const int key, const int scancode, const int action, const int mods);

		/**
		 * \brief Dispatches a character event.
		 *
		 * \param[in] const unsigned int uchar The unicode character key code.
		 * \return void
		 */
		void DispatchCharacterEvent(const unsigned int uchar);

		/**
		 * \brief Dispatches mouse movement events.
		 *
		 * It determines the changes in mouse position and stores the new position for later.
		 * \param[in] const double x, y The new x and y coordinate of the mouse in screen coordinates.
		 * \return void
		 */
		void DispatchMouseMoveEvent(const double x, const double y);

		/**
		 * \brief Dispatches mouse button events from the callback.
		 *
		 * \param[in] const int button The button that the action occurred on.
		 * \param[in] const int action The action that occurred.
		 * \param[in] const int mods Modifier keys.
		 * \return void
		 */
		void DispatchMouseButtonEvent(const int button, const int action, const int mods);

		GLFWwindow* window;
		int width, height; // Current window's client width and height.
		double oldMouseX, oldMouseY;
		double lastTime; // The time at the last call to GetDeltaTime().
		bool mouseLock; // If mouse lock is enabled causing the cursor to snap to mid-window each movement event.
		event::KeyboardInputSystem KeyboardEventSystem;
		event::MouseInputSystem MouseEventSystem;
	};
}
#endif //OS_H