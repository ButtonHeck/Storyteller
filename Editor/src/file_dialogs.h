#pragma once

#include <string>

struct GLFWwindow;

namespace Storyteller
{
	namespace FileDialogs
	{
		std::string OpenFile(const char* filter, GLFWwindow* window);
		std::string SaveFile(const char* filter, GLFWwindow* window);
	};
}