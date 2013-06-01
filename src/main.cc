// (C) Copyright 2013 Florian Plaza Oñate
// Use, modification and distribution is subject to the MIT License (MIT)
// (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)

#include "XsqParallelConverter.hh"
#include <boost/program_options.hpp>
namespace po = boost::program_options;
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#include <boost/optional.hpp>
#include <boost/thread.hpp>



/**
* Function which checks and retrieves command line parameters.
**/
auto get_parameters(int argc, char *argv[], std::string& input_file, std::string& output_dir, unsigned& nb_processes, boost::optional<std::vector<std::string>>& prefixes_wanted) -> void
{
	// Create options decription
	po::options_description opts_desc(
		"xsq-convert-parallel converts XSQ files by launching several xsq-convert processes in parallel.\nAvailable options");

	opts_desc.add_options()
		("help,h", "print this help message")
		("input-file,i", po::value<std::string>(), "xsq file to convert")
		("output-dir,o", po::value<std::string>()->default_value("."), "directory in which cfasta and qval files are saved")
		("extract-only", po::value<std::vector<std::string> >()->multitoken(), "extract only libraries whose name is prefixed by one of the args. (case sensitive)")
		("processes,p", po::value<unsigned>(), "number of xsq-convert processes to launch in parallel") 
		;

	// Retrieve and parse command line parameters
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, opts_desc), vm);
	po::notify(vm);

	// Print help
	if (vm.size() == 1 || vm.count("help"))
	{
		std::cout << opts_desc << std::endl;
		std::exit(0);
	}

	// Check input file
	if (!vm.count("input-file"))
	{
		std::cerr << "error: No XSQ input file. Goodbye." << std::endl;
		std::exit(-1);
	}
	else
	{
		input_file = vm["input-file"].as<std::string>();

		if (!fs::exists(input_file))
		{
			std::cerr << "error: file " << input_file <<  " not found." << std::endl;
			std::exit(-1);
		}

		if (!fs::is_regular_file(input_file))
		{
			std::cerr << "error: " << input_file <<  " is not a regular file." << std::endl;
			std::exit(-1);
		}

		if (fs::extension(input_file) != ".xsq")
		{
			std::cerr << "error: " << input_file << " should have the extension \".xsq\"." << std::endl;
			std::exit(-1);
		}
	}

	// Check output dir
	output_dir = vm["output-dir"].as<std::string>();
	if (!fs::exists(output_dir))
	{
		std::cerr << "error: directory " << output_dir <<  " not found." << std::endl;
		std::exit(-1);
	}
	if (!fs::is_directory(output_dir))
	{
		std::cerr << "error: " << output_dir <<  " is not a directory." << std::endl;
		std::exit(-1);
	}

	if (!vm.count("processes"))
	{
		std::cerr << "error: -p option should be set. (no default value)" << std::endl;
		std::exit(-1);
	}
	else
	{
		nb_processes = vm["processes"].as<unsigned>();
		unsigned nb_cores =  boost::thread::hardware_concurrency();
		if (nb_processes == 0)
		{
			std::cerr << "error: number of processes should be stricly positive." << std::endl;
			std::exit(-1);
		}
		else if (nb_processes > nb_cores)
		{
			std::cerr << "error: number of processes should be inferior to the number of cores available on this machine (" <<
				nb_cores << ")." << std::endl;
			std::exit(-1);
		}
	}

	if (vm.count("extract-only"))
		prefixes_wanted = vm["extract-only"].as<std::vector<std::string> >();

}

auto main(int argc , char *argv[]) -> int
{
	std::string input_file;
	std::string output_dir;
	unsigned nb_processes;
	boost::optional<std::vector<std::string>> prefixes_wanted;

	try 
	{	
		get_parameters(argc, argv, input_file, output_dir, nb_processes, prefixes_wanted);
		XsqParallelConverter().convert(input_file, output_dir, nb_processes, prefixes_wanted);
		std::exit(0);
	}
	catch (const H5::Exception& h5e)
	{
		std::cerr << h5e.getDetailMsg() << std::endl;
		std::exit(-1);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		std::exit(-1);
	}

	return 0;
}