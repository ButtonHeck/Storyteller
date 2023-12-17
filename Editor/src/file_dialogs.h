#pragma once

#include <string>

struct GLFWwindow;

namespace Storyteller
{
	class FileDialogs
	{
	public:
		// These return empty strings if cancelled
		static std::string OpenFile(const char* filter, GLFWwindow* window);
		static std::string SaveFile(const char* filter, GLFWwindow* window);
	};
}