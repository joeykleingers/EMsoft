#include "H5DatasetComparison.h"

/**
 * @brief main This test will check the output of the EMEBSDmaster program against an exemplary
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

  H5DatasetComparison datasetComparison(exemplaryFilePath, testFilePath);
  bool result = true;

  // Dataset Comparisons
  if(!datasetComparison.compareDataset<float>("EMData/EBSDmaster/BetheParameters"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<float>("EMData/EBSDmaster/EkeVs"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<float>("EMData/EBSDmaster/Z2percent"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<int32_t>("EMData/EBSDmaster/lastEnergy"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<float>("EMData/EBSDmaster/mLPNH"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<float>("EMData/EBSDmaster/mLPSH"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<float>("EMData/EBSDmaster/masterSPNH"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<float>("EMData/EBSDmaster/masterSPSH"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<int32_t>("EMData/EBSDmaster/numEbins"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<int32_t>("EMData/EBSDmaster/numset"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<std::string>("EMheader/EBSDmaster/ProgramName"))
  {
    result = false;
  }

  if(result)
  {
    return EXIT_SUCCESS;
  }

  return EXIT_FAILURE;
}
