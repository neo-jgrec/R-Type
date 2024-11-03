#ifndef SHELL_HPP
#define SHELL_HPP

#include <thread>
#include <fstream>

namespace core::ge {
    class Shell {
    private:
        std::string _logFilename;
        std::ofstream _file;
        std::jthread _thread;
        ecs::Registry _registry;
        std::atomic<bool> _running = true;

        std::string help() const
        {
            std::string helpMessage = "Available commands:\n";
            for (const auto &[command, description] : commands) {
                helpMessage += command + " - " + description.first + "\n";
            }
            helpMessage += "exit - exit the shell";
            return helpMessage;
        }

        std::string log() const
        {
            std::ifstream log(_logFilename);
            if (!log.is_open()) {
                return "Failed to open log file: " + _logFilename;
            }
            std::string logContent;
            std::string line;
            while (std::getline(log, line)) {
                logContent += line + "\n";
            }
            if (logContent.empty()) {
                return "Log file is empty";
            }
            logContent.pop_back();
            return logContent;
        }

        std::string save() const
        {
            const auto backupFilename = _logFilename + "_" + std::to_string(std::time(nullptr)) + ".bak";
            std::ifstream src(_logFilename, std::ios::binary);
            std::ofstream dst(backupFilename, std::ios::binary);
            dst << src.rdbuf();
            return "Log file saved to " + backupFilename;
        }

        std::map<std::string, std::pair<std::string, std::function<std::string(std::string)>>> commands = {
            {"help", {"display help message", [this](const std::string&) -> std::string {
                return help();
            }}},
            {"log", {"display the log file", [this](const std::string&) -> std::string {
                return log();
            }}},
            {"save", {"save the log file", [this](const std::string&) -> std::string {
                return save();
            }}}
        };

        static std::string executable_name()
        {
            #ifdef _WIN32

            return "game";

            #else

            std::string sp;
            std::ifstream("/proc/self/comm") >> sp;
            return sp;

            #endif
        }


    public:
        Shell(const ecs::Registry &registry)
            : _registry(registry)
        {
            _logFilename = std::filesystem::temp_directory_path().string() + executable_name() + ".log";
            _file.open(_logFilename, std::ios::out | std::ios::trunc);
            if (!_file.is_open()) {
                throw std::runtime_error("Failed to open log file: " + _logFilename);
            }

            _thread = std::jthread([this] {
                std::string command;
                while (_running) {
                    std::cout << "> ";
                    std::getline(std::cin, command);

                    std::erase(command, '\n');
                    if (command.empty())
                        continue;
                    if (command == "exit")
                        break;

                    std::string args;
                    if (const auto space = command.find(' ');
                        space != std::string::npos) {
                        args = command.substr(space + 1);
                        command = command.substr(0, space);
                    }

                    if (const auto it = commands.find(command); it != commands.end()) {
                        std::cout << it->second.second(args) << std::endl;
                    } else {
                        std::cout << "Unknown command: " << command << std::endl;
                    }
                }
            });
        }

        void close()
        {
            static bool isClosed = false;
            if (isClosed)
                return;

            _running = false;
            _thread.request_stop();
            _thread.join();
            _file.close();
        }

        std::ofstream &out()
        {
            return _file;
        }

        void addCommand(const std::string &command, const std::string &description, const std::function<std::string(std::string)> &callback)
        {
            commands[command] = {description, callback};
        }
    };
}

#endif //SHELL_HPP
