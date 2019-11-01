#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

#include "H5Support/H5Lite.h"
#include "H5Support/H5ScopedSentinel.h"
#include "H5Support/H5Utilities.h"

#include "TestFileLocations.h"

class H5FileComparison
{
public:
  /**
   * @brief H5FileComparison
   * @param exemplaryFilePath
   * @param testFilePath
   * @param blacklist
   */
  H5FileComparison(std::string exemplaryFilePath, std::string testFilePath, std::list<std::string> blacklist);

  /**
   * @brief compareHDF5Files
   * @return
   */
  bool compareHDF5Files();

  /**
   * @brief getGroupOutput
   * @return
   */
  std::string getGroupOutput() const;

  /**
   * @brief getDatasetOutput
   * @return
   */
  std::string getDatasetOutput() const;

  /**
   * @brief getAttributeOutput
   * @return
   */
  std::string getAttributeOutput() const;

private:
  std::string m_ExemplaryFilePath;
  std::string m_TestFilePath;
  std::list<std::string> m_DataComparisonBlacklist;
  std::string m_GroupOutput;
  std::string m_DatasetOutput;
  std::string m_AttributeOutput;

  /**
   * @brief compareHDF5Files
   * @param createdId
   * @param exemplaryId
   * @return
   */
  bool compareHDF5Files(hid_t createdId, hid_t exemplaryId);

  /**
   * @brief compareGroups
   * @param createdId
   * @param createdGroupNames
   * @param exemplaryId
   * @param exemplaryGroupNames
   * @return
   */
  bool compareGroups(hid_t createdId, std::list<std::string> createdGroupNames, hid_t exemplaryId, const std::list<std::string>& exemplaryGroupNames);

  /**
   * @brief compareDatasets
   * @param createdId
   * @param createdDatasetNames
   * @param exemplaryId
   * @param exemplaryDatasetNames
   * @return
   */
  bool compareDatasets(hid_t createdId, std::list<std::string> createdDatasetNames, hid_t exemplaryId, const std::list<std::string>& exemplaryDatasetNames);

  /**
   * @brief compareDataset
   * @param createdId
   * @param exemplaryId
   * @param datasetName
   * @param createdDatasetNames
   * @return
   */
  bool compareDataset(hid_t createdId, hid_t exemplaryId, const std::string& datasetName, std::list<std::string>& createdDatasetNames);

  /**
   * @brief compareDatasetData
   * @param createdId
   * @param exemplaryId
   * @param datasetName
   * @return
   */
  bool compareDatasetData(hid_t createdId, hid_t exemplaryId, const std::string& datasetName);

  /**
   * @brief compareAttributes
   * @param createdId
   * @param exemplaryId
   * @param datasetName
   * @return
   */
  bool compareAttributes(hid_t createdId, hid_t exemplaryId, const std::string& datasetName);

  /**
   * @brief compareAttribute
   * @param createdId
   * @param exemplaryId
   * @param datasetName
   * @param attributeName
   * @param createdAttributeNames
   * @return
   */
  bool compareAttribute(hid_t createdId, hid_t exemplaryId, const std::string& datasetName, const std::string& attributeName, std::list<std::string>& createdAttributeNames);

  /**
   * @brief getObjectPath
   * @param id
   * @param objName
   * @return
   */
  std::string getObjectPath(hid_t id, const std::string& objName);

  /**
   * @brief getAttributeSizeType
   * @param id
   * @param datasetName
   * @param attributeName
   * @return
   */
  std::pair<size_t, bool> getAttributeSizeType(hid_t id, const std::string& datasetName, const std::string& attributeName);

  /**
   * @brief getAttributeDimensions
   * @param id
   * @param datasetName
   * @param attributeName
   * @return
   */
  std::pair<std::vector<hsize_t>, bool> getAttributeDimensions(hid_t id, const std::string& datasetName, const std::string& attributeName);

  /**
   * @brief getAttributeClassType
   * @param id
   * @param datasetName
   * @param attributeName
   * @return
   */
  std::pair<H5T_class_t, bool> getAttributeClassType(hid_t id, const std::string& datasetName, const std::string& attributeName);

  /**
   * @brief getDatasetSizeType
   * @param id
   * @param datasetName
   * @return
   */
  std::pair<size_t, bool> getDatasetSizeType(hid_t id, const std::string& datasetName);

  /**
   * @brief getDatasetDimensions
   * @param id
   * @param datasetName
   * @return
   */
  std::pair<std::vector<hsize_t>, bool> getDatasetDimensions(hid_t id, const std::string& datasetName);

  /**
   * @brief getDatasetClassType
   * @param id
   * @param datasetName
   * @return
   */
  std::pair<H5T_class_t, bool> getDatasetClassType(hid_t id, const std::string& datasetName);

  /**
   * @brief getDatasetType
   * @param id
   * @param datasetName
   * @return
   */
  std::pair<hid_t, bool> getDatasetType(hid_t id, const std::string& datasetName);

  /**
   * @brief line
   * @param length
   * @return
   */
  std::string line(int length);

  /**
   * @brief listContainsElement
   * @param list
   * @param var
   * @return
   */
  bool listContainsElement(const std::list<std::string>& list, const std::string& var);

  /**
   * @brief getObjectNames
   * @param createdId
   * @param createdNames
   * @param exemplaryId
   * @param exemplaryNames
   * @param dataType
   * @param testResults
   * @return
   */
  bool getObjectNames(hid_t createdId, std::list<std::string>& createdNames, hid_t exemplaryId, std::list<std::string>& exemplaryNames, H5Utilities::CustomHDFDataTypes dataType,
                      std::string& testResults);
};
