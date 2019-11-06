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

  if(!datasetComparison.compareDataset<std::string>("CrystalData/ProgramName"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<std::string>("CrystalData/Source"))
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

  if(!datasetComparison.compareDataset<float>("EMData/EBSD/EBSDPatterns", 0.002))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<float>("EMData/EBSD/EulerAngles", 0.001))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<int32_t>("EMData/EBSD/numangles"))
  {
    result = false;
  }

  if(result)
  {
    return EXIT_SUCCESS;
  }

  return EXIT_FAILURE;
}
