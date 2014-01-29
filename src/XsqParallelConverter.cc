// (C) Copyright 2013-2014 INRA
// Use, modification and distribution is subject to the MIT License (MIT)
// (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)

#include "XsqParallelConverter.hh"

void XsqParallelConverter::convert(const std::string& input_file, const std::string& output_dir, unsigned nb_processes, const boost::optional<std::vector<std::string>>& prefixes_wanted)
{
	Xsq::XsqFile f(input_file);

	// Get libraries to extract
	const auto& libraries = prefixes_wanted ? 
		f.get_libraries_by_prefix(*prefixes_wanted): f.get_libraries();

	if (libraries.size() == 0)
	{
		std::cerr << "error: no libraries to extract. Goodbye." << std::endl;
		std::exit(-1);
	}

	// Ajust the number of processes to launch if necessary
	if (nb_processes > libraries.size())
	{
		nb_processes = libraries.size();
		std::cerr << "warning: number of processes is superior to the number of libraries.\n" <<
			"warning: number of processes changed to " << nb_processes << "." << std::endl;
	}

	// Build command lines used to launch each xsq-convert process
	const auto& command_lines = 
		build_command_lines(libraries, input_file, output_dir, nb_processes);

	launch_sub_processes(command_lines);
	wait_sub_processes();
}


XsqParallelConverter::CommandLines XsqParallelConverter::build_command_lines(const std::vector<std::string>& libraries, const std::string& input_file, const std::string& output_dir, unsigned nb_processes)
{
	// Parameters shared by each process
	CommandLines command_lines(nb_processes, {"xsq-convert", "-i", input_file.c_str(), "-o", output_dir.c_str(), "--extract-only"});
		
	// Assign libraries to each xsq-convert process
	unsigned nb_libs_per_process = libraries.size() / nb_processes;
	auto lib = libraries.begin();
		
	for(auto& command_line: command_lines)
	{
		for (unsigned i = 0; i < nb_libs_per_process; i++)
		{
			command_line.push_back(lib->c_str());
			lib++;
		}
	}
		
	auto command_line = command_lines.begin();
	while (lib != libraries.end())
	{
		command_line->push_back(lib->c_str());
		lib++;
		command_line++;
	}
		

	// End of each command line
	for(auto& command_line: command_lines)
		command_line.push_back(nullptr);

	return command_lines;
}

void XsqParallelConverter::launch_sub_processes(const CommandLines& command_lines)
{
	for(const auto& command_line: command_lines)
	{
		switch(fork())
		{
		case -1:
			std::cerr << "error: fork failed." << std::endl;
			exit(-1);
		case 0:
			execvp("xsq-convert", const_cast<char* const *>(command_line.data()));
		default:
			break;
		}
	}
}

void XsqParallelConverter::wait_sub_processes()
{
	while(wait(nullptr) != -1)
		;
}
