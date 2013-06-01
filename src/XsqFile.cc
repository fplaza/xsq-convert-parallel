#include "XsqFile.hh"
using namespace Xsq;

/**
* Method which returns a list of all the libraries in this XsqFile
* except those which are unclassified or unassigned.
*/
auto XsqFile::get_libraries() const -> std::vector<std::string> 
{
	std::vector<std::string> libraries;

	for (unsigned library_id = 0; library_id < m_file.getNumObjs(); library_id++)
	{
		const auto& library_name = m_file.getObjnameByIdx(library_id);

		if (std::find_if(s_reserved_names.begin(), s_reserved_names.end(), StringPrefixComparator(library_name)) == s_reserved_names.end())
			libraries.push_back(library_name);
	}


	return libraries;
}

/**
* Method which returns a list of all the libraries in this XsqFile
* whose name is prefixed by one of the elements in the prefixes_wanted list.
*/
auto XsqFile::get_libraries_by_prefix(const std::vector<std::string>& prefixes_wanted) const -> std::vector<std::string>
{
	std::vector<std::string> libraries;

	for (unsigned library_id = 0; library_id < m_file.getNumObjs(); library_id++)
	{
		const auto& library_name = m_file.getObjnameByIdx(library_id);

		if (std::find_if(prefixes_wanted.begin(), prefixes_wanted.end(), StringPrefixComparator(library_name)) != prefixes_wanted.end())
		{
			libraries.push_back(library_name);
		}
	}

	return libraries;
}

const std::vector<std::string> XsqFile::s_reserved_names = 
	{"RunMetadata", "TagDetails", "Indexing", "Unassigned", "Unclassified"};