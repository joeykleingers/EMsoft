#include "H5FileComparison.h"

namespace
{
const std::list<std::string> k_DataComparisonBlacklist = {"CrystalData/CreationDate", "CrystalData/CreationTime"};
}

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

  H5FileComparison fileComparison(exemplaryFilePath, testFilePath, k_DataComparisonBlacklist);

  // Use recursive algorithm to compare all groups between both files
  if(!fileComparison.compareHDF5Files())
  {
    std::string groupOutput = fileComparison.getGroupOutput();
    if(!groupOutput.empty())
    {
      std::cout << groupOutput;
    }

    std::string datasetOutput = fileComparison.getDatasetOutput();
    if(!datasetOutput.empty())
    {
      std::cout << "\n\n" << datasetOutput;
    }

    std::string attributeOutput = fileComparison.getAttributeOutput();
    if(!attributeOutput.empty())
    {
      std::cout << "\n\n" << attributeOutput;
    }
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
