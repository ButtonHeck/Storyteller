#pragma once

#include <boost/program_options.hpp>

namespace Storyteller
{
    class ProgramOptions
    {
    public:
        ProgramOptions();

        void ProcessCommandLine(char* lpCmdLine);
        void ProcessCommandLine(int argc, char** argv);

        const std::string& GetConfigPath() const;

    private:
        boost::program_options::command_line_parser CreateCmdParser(char* lpCmdLine) const;
        boost::program_options::command_line_parser CreateCmdParser(int argc, char** argv) const;
        void ProcessCommandLineArgs(boost::program_options::command_line_parser& cmdParser);

    private:
        std::string _configPath;
    };
    //--------------------------------------------------------------------------
}