#include "Input.h"

#include <GLFW/glfw3.h>

Input* Input::m_instance = nullptr;

Input::Input()
	: m_oldMouseX{ 0 }, m_oldMouseY{ 0 }, m_firstMouseMove{ false },
	m_lastKeys{ new int[GLFW_KEY_LAST + 1] }, m_currentKeys{ new int[GLFW_KEY_LAST + 1] }
{
	auto window = glfwGetCurrentContext();

	for (int i = GLFW_KEY_SPACE; i <= GLFW_KEY_LAST; ++i)
		m_lastKeys[i] = m_currentKeys[i] = glfwGetKey(window, i);

	for (int i = 0; i < 8; ++i)
		m_lastButtons[i] = m_currentButtons[i] = glfwGetMouseButton(window, i);

	// set up callbacks
	auto keyPressCallback = [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			for (auto& f : Input::Get()->m_keyCallbacks)
				f(window, key, scancode, action, mods);
		};

	auto characterInputCallback = [](GLFWwindow* window, unsigned int character)
		{
			Get()->m_pressedCharacters.push_back(character);

			for (auto& f : Get()->m_charCallbacks)
				f(window, character);
		};

	auto mouseMoveCallback = [](GLFWwindow* window, double x, double y)
		{
			int w = 0, h = 0;
			glfwGetWindowSize(window, &w, &h);

			Get()->OnMouseMove(static_cast<int>(x), h - static_cast<int>(y));

			for (auto& f : Input::Get()->m_mouseMoveCallbacks)
				f(window, x, h - y);
		};

	auto mouseInputCallback = [](GLFWwindow* window, int button, int action, int mods)
		{
			for (auto& f : Get()->m_mouseButtonCallbacks)
				f(window, button, action, mods);
		};

	auto mouseScrollCallback = [](GLFWwindow* window, double xOffset, double yOffset)
		{
			Get()->m_mouseScroll += yOffset;

			for (auto& f : Get()->m_mouseScrollCallbacks)
				f(window, xOffset, yOffset);
		};

	auto mouseEnterCallback = [](GLFWwindow* window, int entered)
		{
			// Set flag to prevent large mouse delta on entering screen
			Get()->m_firstMouseMove = true;
		};

	glfwSetKeyCallback(window, keyPressCallback);
	glfwSetCharCallback(window, characterInputCallback);
	glfwSetMouseButtonCallback(window, mouseInputCallback);
	glfwSetCursorPosCallback(window, mouseMoveCallback);
	glfwSetScrollCallback(window, mouseScrollCallback);
	glfwSetCursorEnterCallback(window, mouseEnterCallback);

	m_mouseX = 0;
	m_mouseY = 0;
	m_mouseScroll = 0;
}

Input::~Input()
{
	delete[] m_lastKeys;
	delete[] m_currentKeys;
}

void Input::OnMouseMove(int newXPos, int newYPos)
{
	m_mouseX = newXPos;
	m_mouseY = newYPos;

	if (m_firstMouseMove)
	{
		// On first move after Startup/entering window reset old mouse position
		m_oldMouseX = newXPos;
		m_oldMouseY = newYPos;
		m_firstMouseMove = false;
	}
}

void Input::Create()
{
	m_instance = new Input;
}

void Input::Destroy()
{
	delete m_instance;
}

void Input::ClearStatus()
{
	m_pressedCharacters.clear();

	auto window = glfwGetCurrentContext();

	m_pressedKeys.clear();

	// Update keys
	for (int i = GLFW_KEY_SPACE; i <= GLFW_KEY_LAST; ++i)
	{
		m_lastKeys[i] = m_currentKeys[i];

		if ((m_currentKeys[i] = glfwGetKey(window, i)) == GLFW_PRESS)
		{
			m_pressedKeys.push_back(m_currentKeys[i]);
		}
	}

	// Update mouse
	for (int i = 0; i < 8; ++i)
	{
		m_lastButtons[i] = m_currentButtons[i];
		m_currentButtons[i] = glfwGetMouseButton(window, i);
	}

	// Update old mouse position
	m_oldMouseX = m_mouseX;
	m_oldMouseY = m_mouseY;
}

Input* Input::Get()
{
	return m_instance;
}

bool Input::IsKeyDown(int inputKeyID) const
{
	return m_currentKeys[inputKeyID] == GLFW_PRESS;
}

bool Input::IsKeyUp(int inputKeyID) const
{
	return m_currentKeys[inputKeyID] == GLFW_RELEASE;
}

bool Input::WasKeyPressed(int inputKeyID) const
{
	return m_currentKeys[inputKeyID] == GLFW_PRESS && m_lastKeys[inputKeyID] == GLFW_RELEASE;
}

bool Input::WasKeyReleased(int inputKeyID) const
{
	return m_currentKeys[inputKeyID] == GLFW_RELEASE && m_lastKeys[inputKeyID] == GLFW_PRESS;
}

const vector<int>& Input::GetPressedKeys() const
{
	return m_pressedKeys;
}

const vector<unsigned int>& Input::GetPressedCharacters() const
{
	return m_pressedCharacters;
}

bool Input::IsMouseButtonDown(int inputMouseID) const
{
	return m_currentButtons[inputMouseID] == GLFW_PRESS;
}

bool Input::IsMouseButtonUp(int inputMouseID) const
{
	return m_currentButtons[inputMouseID] == GLFW_RELEASE;
}

bool Input::WasMouseButtonPressed(int inputMouseID) const
{
	return m_currentButtons[inputMouseID] == GLFW_PRESS &&
		m_lastButtons[inputMouseID] == GLFW_RELEASE;
}

bool Input::WasMouseButtonReleased(int inputMouseID) const
{
	return m_currentButtons[inputMouseID] == GLFW_RELEASE &&
		m_lastButtons[inputMouseID] == GLFW_PRESS;
}

int Input::GetMouseX() const
{
	return m_mouseX;
}

int Input::GetMouseY() const
{
	return m_mouseY;
}

double Input::GetMouseScroll() const
{
	return m_mouseScroll;
}

void Input::GetMouseXY(int* x, int* y) const
{
	if (x != nullptr)
	{
		*x = m_mouseX;
	}

	if (y != nullptr)
	{
		*y = m_mouseY;
	}
}

vec2i Input::GetMousePos()
{
	return
	{
		m_mouseX,
		m_mouseY
	};
}

int Input::GetMouseDeltaX() const
{
	return m_mouseX - m_oldMouseX;
}

int Input::GetMouseDeltaY() const
{
	return m_mouseY - m_oldMouseY;
}

void Input::GetMouseDelta(int* x, int* y) const
{
	if (x != nullptr)
	{
		*x = m_mouseX - m_oldMouseX;
	}

	if (y != nullptr)
	{
		*y = m_mouseY - m_oldMouseY;
	}
}
