#include "H5StructureComparison.h"

#include <iostream>

namespace
{
// List the groups that will be used for this test.  The test will check that all groups exist and have the same structure in both files.
const std::list<std::string> k_H5GroupPaths = {"CrystalData", "EMData/MCOpenCL", "EMheader/MCOpenCL"};
} // namespace

/**
 * @brief main This test will check the output of the EMMCOpenCL program against an exemplary
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char* argv[])
{
  if(argc < 3)
  {
    std::cout << "Line " << __LINE__ << ": There should be 3 arguments, but there are only " << argc
              << ".  Make sure that you are passing in the exemplary and test HDF5 files as arguments to this test.\n";
    return EXIT_FAILURE;
  }

  std::string testFilePath = argv[1];
  std::string exemplaryFilePath = argv[2];

  H5StructureComparison structureComparison(exemplaryFilePath, testFilePath);

  // Use recursive algorithm to compare all groups between both files
  if(!structureComparison.compareStructure(k_H5GroupPaths))
  {
    std::string output = structureComparison.getOutput();
    std::cout << output;

    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
