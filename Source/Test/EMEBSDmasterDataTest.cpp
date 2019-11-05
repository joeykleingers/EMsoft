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

  if(!datasetComparison.compareDataset<char>("EMheader/EBSDmaster/ProgramName"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<char>("EMheader/EBSDmaster/Version"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<char>("NMLfiles/EBSDmasterNML"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<char>("NMLparameters/EBSDMasterNameList/BetheParametersFile"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<int32_t>("NMLparameters/EBSDMasterNameList/Esel"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<int32_t>("NMLparameters/EBSDMasterNameList/combinesites"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<char>("NMLparameters/EBSDMasterNameList/copyfromenergyfile"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<float>("NMLparameters/EBSDMasterNameList/dmin"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<char>("NMLparameters/EBSDMasterNameList/energyfile"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<char>("NMLparameters/EBSDMasterNameList/latgridtype"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<int32_t>("NMLparameters/EBSDMasterNameList/npx"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<int32_t>("NMLparameters/EBSDMasterNameList/nthreads"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<int32_t>("NMLparameters/EBSDMasterNameList/restart"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<int32_t>("NMLparameters/EBSDMasterNameList/stdout"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<int32_t>("NMLparameters/EBSDMasterNameList/uniform"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<float>("NMLparameters/BetheList/c1"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<float>("NMLparameters/BetheList/c2"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<float>("NMLparameters/BetheList/c3"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<float>("NMLparameters/BetheList/sgdbdiff"))
  {
    result = false;
  }

  // Output the results if the test did not pass
  if(!result)
  {
    std::string output = datasetComparison.getOutput();
    std::cout << output;

    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
