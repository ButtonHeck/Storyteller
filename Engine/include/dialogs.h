#pragma once

#include <string>

struct GLFWwindow;

namespace Storyteller
{
	namespace Dialogs
	{
		std::string OpenFile(const char* filter, GLFWwindow* window);
		std::string SaveFile(const char* filter, GLFWwindow* window);
		bool Message(const char* text, const char* caption, GLFWwindow* window);
	};
}