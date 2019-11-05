#include <numeric>
#include <sstream>
#include <string>

#include "H5Support/H5ScopedSentinel.h"
#include "H5Support/H5Utilities.h"

namespace
{
const std::string k_TableDelimiter = "  |  ";
const std::string k_YesChar = "Y";
const std::string k_NoChar = "N";
const std::string k_NAChar = "-";
} // namespace

class H5DatasetComparison
{
public:
  /**
   * @brief H5DatasetComparison
   * @param exemplaryFilePath
   * @param testFilePath
   */
  H5DatasetComparison(std::string exemplaryFilePath, std::string testFilePath);

  /**
   * @brief compareDataset
   * @param h5DatasetPath
   * @return
   */
  template <typename T>
  bool compareDataset(const std::string& h5DatasetPath)
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
      std::stringstream ss;
      std::string objPath = getObjectPath(testParentId, h5DatasetPath);
      ss << "Line " << __LINE__ << ": Dataset does not exist at object path '" << objPath << "' in output file at path '" << m_OutputFilePath << "'." << std::endl;
      m_Output.append(ss.str());
      return false;
    }

    if(!H5Lite::datasetExists(exemplaryParentId, h5DatasetPath))
    {
      std::stringstream ss;
      std::string objPath = getObjectPath(exemplaryParentId, h5DatasetPath);
      ss << "Line " << __LINE__ << ": Dataset does not exist at object path '" << objPath << "' in exemplary file at path '" << m_ExemplaryFilePath << "'." << std::endl;
      m_Output.append(ss.str());
      return false;
    }

    return compareDataset<T>(testParentId, exemplaryParentId, h5DatasetPath);
  }

  /**
   * @brief getGroupOutput
   * @return
   */
  std::string getOutput() const;

private:
  std::string m_ExemplaryFilePath;
  std::string m_OutputFilePath;
  std::string m_Output;

  template <typename T>
  bool compareDataset(hid_t testParentId, hid_t exemplaryParentId, const std::string& h5DatasetPath)
  {
    std::stringstream ss;

    hid_t testDSetId = H5Utilities::openHDF5Object(testParentId, h5DatasetPath);
    if(testDSetId < 0)
    {
      std::string objPath = getObjectPath(testParentId, h5DatasetPath);
      ss << "Line " << __LINE__ << ": Could not open dataset at object path '" << objPath << "' in output file at path '" << m_OutputFilePath << "'." << std::endl;
      m_Output.append(ss.str());
      return false;
    }

    H5ScopedObjectSentinel testSentinel(&testDSetId, false);

    hid_t exemplaryDSetId = H5Utilities::openHDF5Object(exemplaryParentId, h5DatasetPath);
    if(exemplaryDSetId < 0)
    {
      std::string objPath = getObjectPath(exemplaryParentId, h5DatasetPath);
      ss << "Line " << __LINE__ << ": Could not open dataset at object path '" << objPath << "' in exemplary file at path '" << m_ExemplaryFilePath << "'." << std::endl;
      m_Output.append(ss.str());
      return false;
    }

    H5ScopedObjectSentinel exemplarySentinel(&exemplaryDSetId, false);

    std::string objPath = H5Utilities::getObjectPath(exemplaryDSetId);

    ss << objPath;

    hid_t testTypeId = H5Dget_type(testDSetId);
    std::string testTypeStr = H5Lite::StringForHDFType(testTypeId);
    hid_t exemplaryTypeId = H5Dget_type(exemplaryDSetId);
    std::string exemplaryTypeStr = H5Lite::StringForHDFType(exemplaryTypeId);

    T dummy = 0x00;
    hid_t officialTypeId = H5Lite::HDFTypeForPrimitive(dummy);
    std::string officialTypeStr = H5Lite::StringForHDFType(officialTypeId);

    ss << k_TableDelimiter << officialTypeStr << k_TableDelimiter << exemplaryTypeStr << k_TableDelimiter << testTypeStr;

    if(exemplaryTypeStr == officialTypeStr && testTypeStr == officialTypeStr)
    {
      // Both types match, so now test the data itself
      std::vector<hsize_t> testDims;
      H5T_class_t testClassType;
      size_t testSizeType;
      herr_t err = H5Lite::getDatasetInfo(testParentId, h5DatasetPath, testDims, testClassType, testSizeType);
      if(err < 0)
      {
        ss << "Line " << __LINE__ << ": Could not get dataset info at object path '" << objPath << "' in output file at path '" << m_OutputFilePath << "'." << std::endl;
        m_Output.append(ss.str());
        return false;
      }

      std::vector<hsize_t> exemplaryDims;
      H5T_class_t exemplaryClassType;
      size_t exemplarySizeType;
      err = H5Lite::getDatasetInfo(exemplaryParentId, h5DatasetPath, exemplaryDims, exemplaryClassType, exemplarySizeType);
      if(err < 0)
      {
        ss << "Line " << __LINE__ << ": Could not get dataset info at object path '" << objPath << "' in exemplary file at path '" << m_ExemplaryFilePath << "'." << std::endl;
        m_Output.append(ss.str());
        return false;
      }

      if(testDims != exemplaryDims)
      {
        ss << "Line " << __LINE__ << ": The dimensions of the test output dataset at object path '" << objPath << "' do not match the dimensions of the exemplary dataset at the same object path."
           << std::endl;
        ss << k_TableDelimiter << k_NoChar << k_TableDelimiter << k_NAChar << std::endl;
        m_Output.append(ss.str());
        return false;
      }

      ss << k_TableDelimiter << k_YesChar;

      size_t totalTestElements = std::accumulate(testDims.begin(), testDims.end(), static_cast<size_t>(1), std::multiplies<>());
      std::vector<T> testBuffer(totalTestElements);
      err = H5Dread(testDSetId, testTypeId, H5S_ALL, H5S_ALL, H5P_DEFAULT, testBuffer.data());
      if(err < 0)
      {
        std::stringstream ss;
        std::string objPath = H5Utilities::getObjectPath(testDSetId);
        ss << "Line " << __LINE__ << ": Could not read dataset at object path '" << objPath << "' in output file at path '" << m_OutputFilePath << "'." << std::endl;
        m_Output.append(ss.str());
        return false;
      }

      size_t totalExemplaryElements = std::accumulate(exemplaryDims.begin(), exemplaryDims.end(), 1, std::multiplies<>());
      std::vector<T> exemplaryBuffer(totalExemplaryElements);
      hid_t exemplaryTypeId = H5Dget_type(exemplaryDSetId);
      err = H5Dread(exemplaryDSetId, exemplaryTypeId, H5S_ALL, H5S_ALL, H5P_DEFAULT, exemplaryBuffer.data());
      if(err < 0)
      {
        std::stringstream ss;
        ss << "Line " << __LINE__ << ": Could not read dataset at object path '" << objPath << "' in exemplary file at path '" << m_ExemplaryFilePath << "'." << std::endl;
        m_Output.append(ss.str());
        return false;
      }

      for(size_t i = 0; i < totalTestElements; i++)
      {
        T testElement = testBuffer[i];
        T exemplaryElement = exemplaryBuffer[i];

        if(testElement != exemplaryElement)
        {
          std::stringstream ss;
          ss << k_TableDelimiter << k_NoChar;
          ss << "Line " << __LINE__ << ": Dataset at object path '" << objPath << "' has a different value at element " << i
             << " in the test file than the element located at the same location in the same dataset in the exemplary file.  Expected '" << exemplaryElement << "', but instead got '" << testElement
             << "'.\n";
          m_Output.append(ss.str());
          return false;
        }
      }
    }
    else
    {
      ss << k_TableDelimiter << k_NAChar << k_TableDelimiter << k_NAChar << std::endl;
      m_Output.append(ss.str());
      return false;
    }

    return true;
  }

  /**
   * @brief getObjectPath
   * @param id
   * @param objName
   * @return
   */
  std::string getObjectPath(hid_t id, const std::string& objName);
};
