#include <cmath>
#include <numeric>
#include <sstream>
#include <string>

#include "H5Support/H5ScopedSentinel.h"
#include "H5Support/H5Utilities.h"

//#include "UnitTestSupport.hpp"

class H5DatasetComparison
{
public:
  H5DatasetComparison(std::string exemplaryFilePath, std::string testFilePath);

  /**
   * @brief Compares a dataset between both the test file and exemplary file for equality
   * @param h5DatasetPath The path to the dataset
   * @return
   */
  template <typename T>
  bool compareDataset(const std::string& h5DatasetPath, double tolerance = 0.0)
  {
    // Open both files
    hid_t testParentId = H5Utilities::openFile(m_OutputFilePath, true);
    if(testParentId < 0)
    {
      std::cout << "Line " << __LINE__ << ": output file at path '" << m_OutputFilePath << "' could not be opened for reading." << std::endl;
      return false;
    }

    H5ScopedFileSentinel testSentinel(&testParentId, false);

    hid_t exemplaryParentId = H5Utilities::openFile(m_ExemplaryFilePath, true);
    if(exemplaryParentId < 0)
    {
      std::cout << "Line " << __LINE__ << ": Exemplary file at path '" << m_ExemplaryFilePath << "' could not be opened for reading." << std::endl;
      return false;
    }

    H5ScopedFileSentinel exemplarySentinel(&exemplaryParentId, false);

    if(!H5Lite::datasetExists(testParentId, h5DatasetPath))
    {
      std::cout << "Line " << __LINE__ << ": Test dataset does not exist at object path '" << h5DatasetPath << "'." << std::endl;
      return false;
    }

    if(!H5Lite::datasetExists(exemplaryParentId, h5DatasetPath))
    {
      std::cout << "Line " << __LINE__ << ": Exemplary dataset does not exist at object path '" << h5DatasetPath << "'." << std::endl;
      return false;
    }

    return compareDataset<T>(testParentId, exemplaryParentId, h5DatasetPath, tolerance);
  }

private:
  std::string m_ExemplaryFilePath;
  std::string m_OutputFilePath;

  /**
   * @brief Compares a dataset between both the test file and exemplary file for equality
   * @param testParentId The test file's id
   * @param exemplaryParentId The exemplary file's id
   * @param h5DatasetPath The path to the dataset
   * @return
   */
  template <typename T>
  bool compareDataset(hid_t testParentId, hid_t exemplaryParentId, const std::string& h5DatasetPath, double tolerance)
  {
    std::vector<T> testVector;
    herr_t err = H5Lite::readVectorDataset(testParentId, h5DatasetPath, testVector);
    switch(err)
    {
    case -1:
      std::cout << "Line " << __LINE__ << ": Unrecognized data type to read and compare test dataset at object path '" << h5DatasetPath << "'." << std::endl;
      return false;
    case -2:
      std::cout << "Line " << __LINE__ << ": Could not open test dataset at object path '" << h5DatasetPath << "'." << std::endl;
      return false;
    case -3:
      std::cout << "Line " << __LINE__ << ": Storage type is different from the test dataset type." << std::endl;
      return false;
    }

    std::vector<T> exemplaryVector;
    err = H5Lite::readVectorDataset(exemplaryParentId, h5DatasetPath, exemplaryVector);
    if(err < 0)
    {
      std::cout << "Line " << __LINE__ << ": Could not read exemplary dataset at object path '" << h5DatasetPath << "'." << std::endl;
      return false;
    }

    size_t exemplaryVecSize = exemplaryVector.size();
    size_t testVecSize = testVector.size();

    return compareElements<T>(testVector.data(), testVecSize, exemplaryVector.data(), exemplaryVecSize, h5DatasetPath, tolerance);
  }

  template <>
  bool compareDataset<std::string>(hid_t testParentId, hid_t exemplaryParentId, const std::string& h5DatasetPath, double tolerance)
  {
    std::string testString;
    herr_t err = H5Lite::readStringDataset(testParentId, h5DatasetPath, testString);
    if(err < 0)
    {
      std::cout << "Line " << __LINE__ << ": Could not read dataset at object path '" << h5DatasetPath << "' in output file at path '" << m_OutputFilePath << "'." << std::endl;
      return false;
    }

    std::string exemplaryString;
    err = H5Lite::readStringDataset(exemplaryParentId, h5DatasetPath, exemplaryString);
    if(err < 0)
    {
      std::cout << "Line " << __LINE__ << ": Could not read dataset at object path '" << h5DatasetPath << "' in exemplary file at path '" << m_ExemplaryFilePath << "'." << std::endl;
      return false;
    }

    size_t exemplarySize = exemplaryString.size();
    size_t testSize = testString.size();

    return compareElements<char>(testString.c_str(), testSize, exemplaryString.c_str(), exemplarySize, h5DatasetPath, tolerance);
  }

  /**
   * @brief Compares two elements for equality
   * @param testElement
   * @param exemplaryElement
   * @return
   */
  template <typename T>
  bool compareElements(const T* testElements, size_t testCount, const T* exemplaryElements, size_t exemplaryCount, const std::string& h5DatasetPath, double tolerance)
  {
    if(testCount != exemplaryCount)
    {
      std::cout << "Line " << __LINE__ << ": The test file dataset at object path '" << h5DatasetPath << "' has a size of " << testCount
                << ", which is not the same size as the same dataset in the exemplary file which has a size of " << exemplaryCount << "." << std::endl;
      return false;
    }

    for(size_t i = 0; i < exemplaryCount; i++)
    {
      T exemplaryElement = exemplaryElements[i];
      T testElement = testElements[i];
      //      uint32_t* testIntPtr = reinterpret_cast<uint32_t*>(&testElement);
      //      uint32_t* exemplaryIntPtr = reinterpret_cast<uint32_t*>(&exemplaryElement);
      if(!compareElements<T>(testElement, exemplaryElement, tolerance))
      {
        std::cout << "Line " << __LINE__ << ": The test dataset at object path '" << h5DatasetPath << "' has a different value at element " << i
                  << " than the same exemplary dataset's element value.  Expected '" << exemplaryElement << "', but instead got '" << testElement << "'.\n";
        //        printf("%0.8f    %0.8f\n", testElement, exemplaryElement);
        //        printf("0x%08X    0x%08X\n", testIntPtr[0], exemplaryIntPtr[0]);
        return false;
      }
    }

    return true;
  }

  /**
   * @brief Compares two elements for equality
   * @param testElement
   * @param exemplaryElement
   * @return
   */
  template <typename T>
  bool compareElements(T& testElement, T& exemplaryElement, double tolerance)
  {
    return testElement == exemplaryElement;
  }

  /**
   * @brief Compares two float elements for equality
   * @param testElement
   * @param exemplaryElement
   * @return
   */
  template <>
  bool compareElements<float>(float& testElement, float& exemplaryElement, double tolerance)
  {
    double testDouble = static_cast<double>(testElement);
    double exemplaryDouble = static_cast<double>(exemplaryElement);
    return withinTolerance(testDouble, exemplaryDouble, tolerance);
  }

  /**
   * @brief Compares two double elements for equality
   * @param testElement
   * @param exemplaryElement
   * @return
   */
  template <>
  bool compareElements<double>(double& testElement, double& exemplaryElement, double tolerance)
  {
    return withinTolerance(testElement, exemplaryElement, tolerance);
  }

  bool withinTolerance(double& testElement, double& exemplaryElement, double tolerance)
  {
    double diff = abs(exemplaryElement - testElement);
    return (diff < tolerance);
  }

  /**
   * @brief Creates and returns the concatenated object path from the given id and the given objName
   * @param id
   * @param objName
   * @return
   */
  std::string generateObjectPath(hid_t id, const std::string& objName);
};
