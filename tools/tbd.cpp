#include <editline/readline.h>
#include <libtbd/process.hpp>
#include <libtbd/error.hpp>
#include <sys/ptrace.h>
#include <string_view>
#include <sys/wait.h>
#include <algorithm>
#include <iostream>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>



namespace {
	std::vector<std::string> split(std::string_view str, char delimeter) {
		std::vector<std::string> out{};
		std::stringstream ss {std::string{str}};
		std::string item;

		while (std::getline(ss, item, delimeter)) {
			out.push_back(item);
		}

		return out;
	}
	
	bool is_prefix(std::string_view str, std::string_view of) {
		if (str.size() > of.size()) return false;
		return std::equal(str.begin(), str.end(), of.begin());
	}

	void print_stop_reason(const tbd::process& process, tbd::stop_reason reason) {
		std::cout << "Process " << process.pid() << ' ';

		switch (reason.reason) {
			case tbd::process_state::exited:
				std::cout << "exited with status "
						<< static_cast<int>(reason.info);
				break;
			case tbd::process_state::terminated:
				std::cout << "terminated with signal "
						<< sigabbrev_np(reason.info);
				break;
			case tbd::process_state::stopped:
				std::cout << "stopped with signal " << sigabbrev_np(reason.info);
				break;
		}
		std::cout << std::endl;
	}

	void handle_command(std::unique_ptr<tbd::process>& process, std::string_view line) {
		auto args = split(line, ' ');
		auto commands = args[0];
		
		if (is_prefix(commands, "continue")) {
			process->resume();
			auto reason = process->wait_on_signal();
			print_stop_reason(*process, reason);
			process->wait_on_signal();
		} else {
			std::cerr << "Unknown command\n";
		}

	}

	void main_loop(std::unique_ptr<tbd::process>& process) {
		char *line = nullptr;
		while ((line = readline("tbd > ")) != nullptr) {
			std:: string line_str;

			if (line == std::string_view("")) {
				free(line);
				if (history_length > 0) {
					line_str = history_list()[history_length - 1]->line;
				}
			}
			else {
				line_str = line;
				add_history(line);
				free(line);
			}
			if (!line_str.empty()) {
				try {
					handle_command(process, line_str);
				}
				catch (const tbd::error& err) {
					std::cout << err.what() << '\n';
				}
			}
		}
	}


	std::unique_ptr<tbd::process> attach(int argc, const char** argv) {
		// Passing PID
		if (argc == 3 && argv[1] == std::string_view("-p")) {
			pid_t pid = std::atoi(argv[2]);
			return tbd::process::attach(pid);
		}	// Passing a program name 
		else {
			const char* program_path = argv[1];
			return tbd::process::launch(program_path);

		}
	}
}

int main(int argc, const char** argv) {
	if (argc == 1) {
		std::cerr << "No arguments given\n";
		return -1;
	}

	try {
		auto process = attach(argc, argv);
		main_loop(process);
	}

	catch (const tbd::error& err) {
		std::cout << err.what() << '\n';
	}

}
