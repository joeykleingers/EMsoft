#include "H5DatasetComparison.h"

/**
 * @brief main This test will check the output of the EMEBSDDI program against an exemplary
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
  if(!datasetComparison.compareDataset<uint8_t>("Scan 1/EBSD/Data/AvDotProductMap"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<float>("Scan 1/EBSD/Data/CI"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<uint8_t>("Scan 1/EBSD/Data/CIMap"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<float>("Scan 1/EBSD/Data/DictionaryEulerAngles"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<float>("Scan 1/EBSD/Data/EulerAngles"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<int32_t>("Scan 1/EBSD/Data/FZcnt"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<float>("Scan 1/EBSD/Data/Fit"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<float>("Scan 1/EBSD/Data/IQ"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<uint8_t>("Scan 1/EBSD/Data/IQMap"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<float>("Scan 1/EBSD/Data/ISM"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<uint8_t>("Scan 1/EBSD/Data/ISMap"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<float>("Scan 1/EBSD/Data/IndexingSuccessRate"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<float>("Scan 1/EBSD/Data/KAM"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<int32_t>("Scan 1/EBSD/Data/Ncubochoric"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<int32_t>("Scan 1/EBSD/Data/NumExptPatterns"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<float>("Scan 1/EBSD/Data/OSM"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<int8_t>("Scan 1/EBSD/Data/Phase"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<float>("Scan 1/EBSD/Data/Phi"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<float>("Scan 1/EBSD/Data/Phi1"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<float>("Scan 1/EBSD/Data/Phi2"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<int32_t>("Scan 1/EBSD/Data/PointGroupNumber"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<float>("Scan 1/EBSD/Data/RefinedDotProducts"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<float>("Scan 1/EBSD/Data/RefinedEulerAngles"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<int32_t>("Scan 1/EBSD/Data/SEM Signal"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<float>("Scan 1/EBSD/Data/TopDotProductList"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<int32_t>("Scan 1/EBSD/Data/TopMatchIndices"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<int8_t>("Scan 1/EBSD/Data/Valid"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<float>("Scan 1/EBSD/Data/X Position"))
  {
    result = false;
  }

  if(!datasetComparison.compareDataset<float>("Scan 1/EBSD/Data/Y Position"))
  {
    result = false;
  }

  if(result)
  {
    return EXIT_SUCCESS;
  }

  return EXIT_FAILURE;
}
