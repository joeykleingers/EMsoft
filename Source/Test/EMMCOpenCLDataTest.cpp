#include "H5DatasetComparison.h"

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

  H5DatasetComparison datasetComparison(exemplaryFilePath, testFilePath);
  bool result = true;

  // Dataset Comparisons
  if(!datasetComparison.compareDataset<float>("CrystalData/AtomData"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<int32_t>("CrystalData/Atomtypes"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<int32_t>("CrystalData/CrystalSystem"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<double>("CrystalData/LatticeParameters"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<int32_t>("CrystalData/Natomtypes"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<char>("CrystalData/ProgramName"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<char>("CrystalData/Source"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<int32_t>("CrystalData/SpaceGroupNumber"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<int32_t>("CrystalData/SpaceGroupSetting"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<float>("EMData/MCOpenCL/accumSP"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<int32_t>("EMData/MCOpenCL/accum_e"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<int32_t>("EMData/MCOpenCL/accum_z"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<int32_t>("EMData/MCOpenCL/multiplier"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<int32_t>("EMData/MCOpenCL/numEbins"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<int32_t>("EMData/MCOpenCL/numzbins"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<int32_t>("EMData/MCOpenCL/totnum_el"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<char>("EMheader/MCOpenCL/ProgramName"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<char>("EMheader/MCOpenCL/Version"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<char>("NMLfiles/MCOpenCLNML"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<double>("NMLparameters/MCCLNameList/Ebinsize"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<double>("NMLparameters/MCCLNameList/Ehistmin"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<double>("NMLparameters/MCCLNameList/EkeV"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<char>("NMLparameters/MCCLNameList/MCmode"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<char>("NMLparameters/MCCLNameList/dataname"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<double>("NMLparameters/MCCLNameList/depthmax"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<double>("NMLparameters/MCCLNameList/depthstep"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<int32_t>("NMLparameters/MCCLNameList/globalworkgrpsz"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<char>("NMLparameters/MCCLNameList/mode"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<int32_t>("NMLparameters/MCCLNameList/multiplier"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<int32_t>("NMLparameters/MCCLNameList/num_el"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<int32_t>("NMLparameters/MCCLNameList/numsx"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<double>("NMLparameters/MCCLNameList/omega"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<double>("NMLparameters/MCCLNameList/sig"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<int32_t>("NMLparameters/MCCLNameList/stdout"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<int32_t>("NMLparameters/MCCLNameList/totnum_el"))
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
