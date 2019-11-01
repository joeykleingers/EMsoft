#include "H5FileComparison.h"

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

namespace
{
const int k_TableTitleIndent = 65;
const int k_TableLineLength = 100;

const std::string k_TableDelimiter = "  |  ";
const std::string k_YesChar = "Y";
const std::string k_NoChar = "N";

} // namespace

// -----------------------------------------------------------------------------
std::string line(int length)
{
  std::string line;
  for(int i = 1; i < length; i++)
  {
    line.append("-");
  }

  line.append("\n");
  return line;
}

class IStats
{
public:
  std::string generateStatsLine()
  {
    std::stringstream ss;
    std::list<std::string> statsList = getStatsList();

    for(const std::string& stat : statsList)
    {
      ss << stat << k_TableDelimiter;
    }
    std::string objLine = ss.str();
    objLine[objLine.size() - 1] = '\n';
    return objLine;
  }

  std::string objPath;
  bool existsE = false;
  bool existsT = false;

protected:
  IStats() = default;

  virtual std::list<std::string> getStatsList() = 0;

  std::string getExistsEStr()
  {
    return (existsE ? k_YesChar : k_NoChar);
  }

  std::string getExistsTStr()
  {
    return (existsT ? k_YesChar : k_NoChar);
  }
};

class GroupStats : public IStats
{
public:
  static std::string getTableHeaders()
  {
    std::stringstream ss;
    ss << std::setw(k_TableTitleIndent) << "GROUPS ERROR TABLE (E = Exemplary, T = Test)\n";
    ss << line(k_TableLineLength);
    ss << "HDF5 Path" << k_TableDelimiter << "Exists (E)" << k_TableDelimiter << "Exists (T)\n";
    ss << line(k_TableLineLength);
    return ss.str();
  }

protected:
  std::list<std::string> getStatsList() override
  {
    std::string existsEStr = getExistsEStr();
    std::string existsTStr = getExistsTStr();
    std::list<std::string> statsList = {objPath, existsEStr, existsTStr};
    return statsList;
  }
};

class DatasetStats : public IStats
{
public:
  static std::string getTableHeaders()
  {
    std::stringstream ss;
    ss << std::setw(k_TableTitleIndent) << "DATASETS ERROR TABLE (E = Exemplary, T = Test)\n";
    ss << line(k_TableLineLength);
    ss << "HDF5 Path" << k_TableDelimiter << "Exists (E)" << k_TableDelimiter << "Exists (T)" << k_TableDelimiter << "Type (E)" << k_TableDelimiter << "Type (T)\n";
    ss << line(k_TableLineLength);
    return ss.str();
  }

protected:
  std::list<std::string> getStatsList() override
  {
    std::string existsEStr = getExistsEStr();
    std::string existsTStr = getExistsTStr();
    std::list<std::string> statsList = {objPath, existsEStr, existsTStr, typeE, typeT};
    return statsList;
  }

public:
  std::string typeE = "Unknown";
  std::string typeT = "Unknown";
};

class AttributeStats : public DatasetStats
{
public:
  static std::string getTableHeaders()
  {
    std::stringstream ss;
    ss << std::setw(k_TableTitleIndent) << "ATTRIBUTES ERROR TABLE (E = Exemplary, T = Test)\n";
    ss << line(k_TableLineLength);
    ss << "HDF5 Path" << k_TableDelimiter << "Attribute" << k_TableDelimiter << "Exists (E)" << k_TableDelimiter << "Exists (T)" << k_TableDelimiter << "Type (E)" << k_TableDelimiter << "Type (T)\n";
    ss << line(k_TableLineLength);
    return ss.str();
  }

protected:
  std::list<std::string> getStatsList() override
  {
    std::string existsEStr = getExistsEStr();
    std::string existsTStr = getExistsTStr();
    std::list<std::string> statsList = {objPath, attributeName, existsEStr, existsTStr, typeE, typeT};
    return statsList;
  }

public:
  std::string attributeName;
};

// -----------------------------------------------------------------------------
H5FileComparison::H5FileComparison(std::string exemplaryFilePath, std::string testFilePath, std::list<std::string> blacklist)
: m_ExemplaryFilePath(exemplaryFilePath)
, m_TestFilePath(testFilePath)
, m_DataComparisonBlacklist(blacklist)
{
}

// -----------------------------------------------------------------------------
bool H5FileComparison::listContainsElement(const std::list<std::string>& list, const std::string& var)
{
  return (std::find(list.begin(), list.end(), var) != list.end());
}

// -----------------------------------------------------------------------------
std::pair<hid_t, bool> H5FileComparison::getDatasetType(hid_t id, const std::string& datasetName)
{
  hid_t dSetId = H5Utilities::openHDF5Object(id, datasetName);
  if(dSetId < 0)
  {
    return std::make_pair(-1, false);
  }

  H5ScopedObjectSentinel createdSentinel(&dSetId, false);

  hid_t typeId = H5Dget_type(dSetId);
  std::string str = H5Lite::StringForHDFType(typeId);
  return std::make_pair(typeId, true);
}

// -----------------------------------------------------------------------------
std::pair<H5T_class_t, bool> H5FileComparison::getDatasetClassType(hid_t id, const std::string& datasetName)
{
  std::vector<hsize_t> dummy;
  H5T_class_t classType;
  size_t dummy2;
  herr_t err = H5Lite::getDatasetInfo(id, datasetName, dummy, classType, dummy2);
  if(err < 0)
  {
    return std::make_pair(H5T_NO_CLASS, false);
  }

  return std::make_pair(classType, true);
}

// -----------------------------------------------------------------------------
std::pair<std::vector<hsize_t>, bool> H5FileComparison::getDatasetDimensions(hid_t id, const std::string& datasetName)
{
  std::vector<hsize_t> dims;
  H5T_class_t dummy;
  size_t dummy2;
  herr_t err = H5Lite::getDatasetInfo(id, datasetName, dims, dummy, dummy2);
  if(err < 0)
  {
    return std::make_pair(std::vector<hsize_t>(), false);
  }

  return std::make_pair(dims, true);
}

// -----------------------------------------------------------------------------
std::pair<size_t, bool> H5FileComparison::getDatasetSizeType(hid_t id, const std::string& datasetName)
{
  std::vector<hsize_t> dummy;
  H5T_class_t dummy2;
  size_t sizeType;
  herr_t err = H5Lite::getDatasetInfo(id, datasetName, dummy, dummy2, sizeType);
  if(err < 0)
  {
    return std::make_pair(0, false);
  }

  return std::make_pair(sizeType, true);
}

// -----------------------------------------------------------------------------
std::pair<H5T_class_t, bool> H5FileComparison::getAttributeClassType(hid_t id, const std::string& datasetName, const std::string& attributeName)
{
  std::vector<hsize_t> dummy;
  H5T_class_t classType;
  size_t dummy2;
  hid_t dummy3;
  herr_t err = H5Lite::getAttributeInfo(id, datasetName, attributeName, dummy, classType, dummy2, dummy3);
  if(err < 0)
  {
    return std::make_pair(H5T_NO_CLASS, false);
  }

  return std::make_pair(classType, true);
}

// -----------------------------------------------------------------------------
std::pair<std::vector<hsize_t>, bool> H5FileComparison::getAttributeDimensions(hid_t id, const std::string& datasetName, const std::string& attributeName)
{
  std::vector<hsize_t> dims;
  H5T_class_t dummy;
  size_t dummy2;
  hid_t dummy3;
  herr_t err = H5Lite::getAttributeInfo(id, datasetName, attributeName, dims, dummy, dummy2, dummy3);
  if(err < 0)
  {
    return std::make_pair(std::vector<hsize_t>(), false);
  }

  return std::make_pair(dims, true);
}

// -----------------------------------------------------------------------------
std::pair<size_t, bool> H5FileComparison::getAttributeSizeType(hid_t id, const std::string& datasetName, const std::string& attributeName)
{
  std::vector<hsize_t> dummy;
  H5T_class_t dummy2;
  size_t sizeType;
  hid_t dummy3;
  herr_t err = H5Lite::getAttributeInfo(id, datasetName, attributeName, dummy, dummy2, sizeType, dummy3);
  if(err < 0)
  {
    return std::make_pair(0, false);
  }

  return std::make_pair(sizeType, true);
}

// -----------------------------------------------------------------------------
std::string H5FileComparison::getObjectPath(hid_t id, const std::string& objName)
{
  std::string objPath = H5Utilities::getObjectPath(id);
  if(objPath.at(objPath.size() - 1) != '/')
  {
    objPath.append("/");
  }
  objPath.append(objName);
  return objPath;
}

// -----------------------------------------------------------------------------
bool H5FileComparison::compareAttribute(hid_t createdId, hid_t exemplaryId, const std::string& datasetName, const std::string& attributeName, std::list<std::string>& createdAttributeNames)
{
  bool result = true;
  AttributeStats stats;
  stats.objPath = getObjectPath(exemplaryId, datasetName);
  stats.attributeName = "-";
  stats.attributeName = attributeName;
  stats.existsE = true;

  std::pair<H5T_class_t, bool> exemplaryTypeResult = getAttributeClassType(exemplaryId, datasetName, attributeName);
  stats.typeE = exemplaryTypeResult.first;

  if(!listContainsElement(createdAttributeNames, datasetName))
  {
    // Exemplary attribute is NOT included in the test file
    stats.existsT = false;
    stats.typeT = H5T_NO_CLASS;

    result = false;
    m_AttributeOutput.append(stats.generateStatsLine());
  }
  else
  {
    // Exemplary attribute is included in the test file
    stats.existsT = true;
    std::pair<H5T_class_t, bool> createdTypeResult = getAttributeClassType(createdId, datasetName, attributeName);
    stats.typeT = createdTypeResult.first;

    createdAttributeNames.remove(datasetName);
  }

  if(stats.typeE != stats.typeT)
  {
    // The exemplary and test attribute types do not match
    result = false;
    m_AttributeOutput.append(stats.generateStatsLine());
  }

  return result;
}

// -----------------------------------------------------------------------------
bool H5FileComparison::compareAttributes(hid_t createdId, hid_t exemplaryId, const std::string& datasetName)
{
  bool result = true;
  std::stringstream ss;

  hid_t exemplaryDSetId = H5Utilities::openHDF5Object(exemplaryId, datasetName);
  if(exemplaryDSetId < 0)
  {
    std::string objPath = getObjectPath(exemplaryDSetId, datasetName);
    ss << "Line " << __LINE__ << ": Could not open dataset at object path '" << objPath << "' in exemplary file at path '" << m_ExemplaryFilePath << "'.";
    m_AttributeOutput.append(ss.str());
    return false;
  }

  H5ScopedObjectSentinel sentinel(&exemplaryDSetId, true);

  hid_t createdDSetId = H5Utilities::openHDF5Object(createdId, datasetName);
  if(exemplaryDSetId < 0)
  {
    std::string objPath = getObjectPath(exemplaryDSetId, datasetName);
    ss << "Line " << __LINE__ << ": Could not open dataset at object path '" << objPath << "' in exemplary file at path '" << m_ExemplaryFilePath << "'.";
    m_AttributeOutput.append(ss.str());
    return false;
  }
  sentinel.addGroupId(&createdDSetId);

  std::list<std::string> exemplaryAttributes;
  herr_t err = H5Utilities::getAllAttributeNames(exemplaryDSetId, exemplaryAttributes);
  if(err < 0)
  {
    std::string objPath = H5Utilities::getObjectPath(exemplaryDSetId, false);
    ss << "Line " << __LINE__ << ": Could not get attribute names at object path '" << objPath << "' in exemplary file at path '" << m_ExemplaryFilePath << "'.";
    m_AttributeOutput.append(ss.str());
    return false;
  }

  std::list<std::string> createdAttributes;
  err = H5Utilities::getAllAttributeNames(createdDSetId, createdAttributes);
  if(err < 0)
  {
    std::string objPath = H5Utilities::getObjectPath(exemplaryDSetId, false);
    ss << "Line " << __LINE__ << ": Could not get attribute names at object path '" << objPath << "' in test file at path '" << m_TestFilePath << "'.";
    m_AttributeOutput.append(ss.str());
    return false;
  }

  for(const std::string& exemplaryAttributeName : exemplaryAttributes)
  {
    if(!compareAttribute(createdId, exemplaryId, datasetName, exemplaryAttributeName, createdAttributes))
    {
      result = false;
    }
  }

  // If there are remaining attribute names in the test file, then they are not in the exemplary file
  for(const std::string& createdAttributeName : createdAttributes)
  {
    DatasetStats stats;
    stats.objPath = getObjectPath(createdId, datasetName);
    stats.existsE = false;
    stats.existsT = true;
    std::pair<H5T_class_t, bool> createdTypeResult = getAttributeClassType(createdId, datasetName, createdAttributeName);
    if(!createdTypeResult.second)
    {
      std::stringstream ss;
      std::string objPath = getObjectPath(createdId, createdAttributeName);
      ss << "Line " << __LINE__ << ": Could not get class type for attribute on dataset at path '" << objPath << "' in test file at path '" << m_TestFilePath << "'.";
      m_AttributeOutput.append(ss.str());
      result = false;
    }
    stats.typeT = createdTypeResult.first;

    result = false;
    m_AttributeOutput.append(stats.generateStatsLine());
  }

  return result;
}

// -----------------------------------------------------------------------------
bool H5FileComparison::compareDatasetData(hid_t createdId, hid_t exemplaryId, const std::string& datasetName)
{
  bool result = true;

  // Get total test dataset bytes, open the dataset, and read the data
  std::pair<std::vector<hsize_t>, bool> createdDimsResult = getDatasetDimensions(createdId, datasetName);
  if(!createdDimsResult.second)
  {
    std::stringstream ss;
    std::string objPath = getObjectPath(createdId, datasetName);
    ss << "Line " << __LINE__ << ": Could not get dimensions for dataset inside group at object path '" << objPath << "' in test file at path '" << m_TestFilePath << "'.";
    m_DatasetOutput.append(ss.str());
    return false;
  }

  std::pair<size_t, bool> createdSTResult = getDatasetSizeType(createdId, datasetName);
  if(!createdSTResult.second)
  {
    std::stringstream ss;
    std::string objPath = getObjectPath(createdId, datasetName);
    ss << "Line " << __LINE__ << ": Could not get the size type for dataset inside group at object path '" << objPath << "' in test file at path '" << m_TestFilePath << "'.";
    m_DatasetOutput.append(ss.str());
    return false;
  }

  size_t totalTestElements = std::accumulate(createdDimsResult.first.begin(), createdDimsResult.first.end(), static_cast<size_t>(1), std::multiplies<>());
  size_t totalTestBits = createdSTResult.first * totalTestElements;

  hid_t createdDSetId = H5Utilities::openHDF5Object(createdId, datasetName);
  if(createdDSetId < 0)
  {
    std::stringstream ss;
    std::string objPath = getObjectPath(createdId, datasetName);
    ss << "Line " << __LINE__ << ": Could not open group at object path '" << objPath << "' in test file at path '" << m_TestFilePath << "'.";
    m_DatasetOutput.append(ss.str());
    return false;
  }

  H5ScopedObjectSentinel createdSentinel(&createdDSetId, false);

  std::vector<uint8_t> createdBuffer(totalTestBits);
  hid_t createdTypeId = H5Dget_type(createdDSetId);
  herr_t err = H5Dread(createdDSetId, createdTypeId, H5S_ALL, H5S_ALL, H5P_DEFAULT, createdBuffer.data());
  if(err < 0)
  {
    std::stringstream ss;
    std::string objPath = getObjectPath(createdId, datasetName);
    ss << "Line " << __LINE__ << ": Could not read dataset at object path '" << objPath << "' in test file at path '" << m_TestFilePath << "'.";
    m_DatasetOutput.append(ss.str());
    return false;
  }

  // Get total exemplary dataset bytes, open the dataset, and read the data
  std::pair<std::vector<hsize_t>, bool> exemplaryDimsResult = getDatasetDimensions(exemplaryId, datasetName);
  if(!exemplaryDimsResult.second)
  {
    std::stringstream ss;
    std::string objPath = getObjectPath(createdId, datasetName);
    ss << "Line " << __LINE__ << ": Could not get dimensions for dataset inside group at object path '" << objPath << "' in test file at path '" << m_ExemplaryFilePath << "'.";
    m_DatasetOutput.append(ss.str());
    return false;
  }

  std::pair<size_t, bool> exemplarySTResult = getDatasetSizeType(exemplaryId, datasetName);
  if(!exemplarySTResult.second)
  {
    std::stringstream ss;
    std::string objPath = getObjectPath(exemplaryId, datasetName);
    ss << "Line " << __LINE__ << ": Could not get the size type for dataset inside group at object path '" << objPath << "' in test file at path '" << m_ExemplaryFilePath << "'.";
    m_DatasetOutput.append(ss.str());
    return false;
  }

  size_t totalExemplaryElements = std::accumulate(exemplaryDimsResult.first.begin(), exemplaryDimsResult.first.end(), 1, std::multiplies<>());
  size_t totalExemplaryBits = exemplarySTResult.first * totalExemplaryElements;

  hid_t exemplaryDSetId = H5Utilities::openHDF5Object(exemplaryId, datasetName);
  if(exemplaryDSetId < 0)
  {
    std::stringstream ss;
    std::string objPath = getObjectPath(exemplaryId, datasetName);
    ss << "Line " << __LINE__ << ": Could not open group at object path '" << objPath << "' in exemplary file at path '" << m_ExemplaryFilePath << "'.";
    m_DatasetOutput.append(ss.str());
    return false;
  }

  H5ScopedObjectSentinel exemplarySentinel(&exemplaryDSetId, false);

  std::vector<uint8_t> exemplaryBuffer(totalExemplaryBits);
  hid_t exemplaryTypeId = H5Dget_type(exemplaryDSetId);
  err = H5Dread(exemplaryDSetId, exemplaryTypeId, H5S_ALL, H5S_ALL, H5P_DEFAULT, exemplaryBuffer.data());
  if(err < 0)
  {
    std::stringstream ss;
    std::string objPath = getObjectPath(exemplaryId, datasetName);
    ss << "Line " << __LINE__ << ": Could not read dataset at object path '" << objPath << "' in exemplary file at path '" << m_ExemplaryFilePath << "'.";
    m_DatasetOutput.append(ss.str());
    return false;
  }

  // Compare the data
  if(totalTestElements != totalExemplaryElements)
  {
    std::stringstream ss;
    std::string objPath = getObjectPath(exemplaryId, datasetName);
    ss << "Line " << __LINE__ << ": Dataset at object path '" << objPath << "' has an element count of " << totalTestElements << " in test file at path '" << m_TestFilePath
       << "'. However, the same dataset at the same location in the exemplary file at path '" << m_ExemplaryFilePath << "' has an element count of " << totalExemplaryElements
       << ". These element counts must be the same in both files.\n";
    m_DatasetOutput.append(ss.str());
    return false;
  }

  for(size_t i = 0; i < totalTestBits; i++)
  {
    uint8_t testBit = createdBuffer[i];
    uint8_t exemplaryBit = exemplaryBuffer[i];

    if(testBit != exemplaryBit)
    {
      size_t elementNum = i / createdSTResult.first;

      std::stringstream ss;
      std::string objPath = getObjectPath(exemplaryId, datasetName);
      ss << "Line " << __LINE__ << ": Dataset at object path '" << objPath << "' has a different value at element " << elementNum
         << " in the test file than the element located at the same location in the same dataset in the exemplary file.  Expected '', but instead got ''.\n";
      m_DatasetOutput.append(ss.str());
      return false;
    }
  }

  return result;
}

// -----------------------------------------------------------------------------
bool H5FileComparison::compareDataset(hid_t createdId, hid_t exemplaryId, const std::string& datasetName, std::list<std::string>& createdDatasetNames)
{
  bool result = true;
  DatasetStats stats;
  stats.objPath = getObjectPath(exemplaryId, datasetName);
  stats.existsE = true;

  std::pair<hid_t, bool> exemplaryTypeResult = getDatasetClassType(exemplaryId, datasetName);
  if(!exemplaryTypeResult.second)
  {
    std::stringstream ss;
    std::string objPath = getObjectPath(createdId, datasetName);
    ss << "Line " << __LINE__ << ": Could not get HDF class type for dataset inside group at object path '" << objPath << "' in test file at path '" << m_ExemplaryFilePath << "'.";
    m_DatasetOutput.append(ss.str());
    return false;
  }
  if(exemplaryTypeResult.first != H5T_STRING)
  {
    exemplaryTypeResult = getDatasetType(exemplaryId, datasetName);
    if(!exemplaryTypeResult.second)
    {
      std::stringstream ss;
      std::string objPath = getObjectPath(createdId, datasetName);
      ss << "Line " << __LINE__ << ": Could not get HDF type for dataset inside group at object path '" << objPath << "' in test file at path '" << m_ExemplaryFilePath << "'.";
      m_DatasetOutput.append(ss.str());
      return false;
    }
  }
  stats.typeE = H5Lite::StringForHDFType(exemplaryTypeResult.first);

  if(!listContainsElement(createdDatasetNames, datasetName))
  {
    // Exemplary dataset is NOT included in the test file
    stats.existsT = false;
    stats.typeT = H5T_NO_CLASS;

    result = false;
    m_DatasetOutput.append(stats.generateStatsLine());
  }
  else
  {
    // Exemplary dataset is included in the test file
    stats.existsT = true;
    std::pair<hid_t, bool> createdTypeResult = getDatasetClassType(createdId, datasetName);
    if(!createdTypeResult.second)
    {
      std::stringstream ss;
      std::string objPath = getObjectPath(createdId, datasetName);
      ss << "Line " << __LINE__ << ": Could not get HDF class type for dataset inside group at object path '" << objPath << "' in test file at path '" << m_TestFilePath << "'.";
      m_DatasetOutput.append(ss.str());
      return false;
    }
    if(createdTypeResult.first != H5T_STRING)
    {
      createdTypeResult = getDatasetType(createdId, datasetName);
      if(!createdTypeResult.second)
      {
        std::stringstream ss;
        std::string objPath = getObjectPath(createdId, datasetName);
        ss << "Line " << __LINE__ << ": Could not get HDF type for dataset inside group at object path '" << objPath << "' in test file at path '" << m_TestFilePath << "'.";
        m_DatasetOutput.append(ss.str());
        return false;
      }
    }
    stats.typeT = H5Lite::StringForHDFType(createdTypeResult.first);

    if(stats.typeE != stats.typeT)
    {
      // The exemplary and test dataset types do not match
      result = false;
      m_DatasetOutput.append(stats.generateStatsLine());
    }
    else
    {
      // The dataset types match, so now compare the data (if the dataset isn't on the blacklist)
      std::string createdObjPath = getObjectPath(createdId, datasetName);
      if(!listContainsElement(m_DataComparisonBlacklist, createdObjPath) && !compareDatasetData(createdId, exemplaryId, datasetName))
      {
        result = false;
      }
    }

    if(!compareAttributes(createdId, exemplaryId, datasetName))
    {
      result = false;
    }

    createdDatasetNames.remove(datasetName);
  }

  return result;
}

// -----------------------------------------------------------------------------
bool H5FileComparison::compareDatasets(hid_t createdId, std::list<std::string> createdDatasetNames, hid_t exemplaryId, const std::list<std::string>& exemplaryDatasetNames)
{
  bool result = true;
  for(const std::string& exemplaryDatasetName : exemplaryDatasetNames)
  {
    if(!compareDataset(createdId, exemplaryId, exemplaryDatasetName, createdDatasetNames))
    {
      result = false;
    }
  }

  // If there are remaining dataset names in the test file, then they are not in the exemplary file
  for(const std::string& createdDatasetName : createdDatasetNames)
  {
    DatasetStats stats;
    stats.objPath = getObjectPath(createdId, createdDatasetName);
    stats.existsE = false;
    stats.existsT = true;
    std::pair<H5T_class_t, bool> createdTypeResult = getDatasetClassType(createdId, createdDatasetName);
    if(!createdTypeResult.second)
    {
      std::stringstream ss;
      std::string objPath = getObjectPath(createdId, createdDatasetName);
      ss << "Line " << __LINE__ << ": Could not get class type for dataset inside group at object path '" << objPath << "' in test file at path '" << m_TestFilePath << "'.";
      m_DatasetOutput.append(ss.str());
      result = false;
    }
    stats.typeT = createdTypeResult.first;

    result = false;
    m_DatasetOutput.append(stats.generateStatsLine());
  }

  return result;
}

// -----------------------------------------------------------------------------
bool H5FileComparison::compareGroups(hid_t createdId, std::list<std::string> createdGroupNames, hid_t exemplaryId, const std::list<std::string>& exemplaryGroupNames)
{
  bool result = true;
  for(const std::string& exemplaryGroupName : exemplaryGroupNames)
  {
    GroupStats stats;
    stats.objPath = getObjectPath(exemplaryId, exemplaryGroupName);
    stats.existsE = true;

    if(!listContainsElement(createdGroupNames, exemplaryGroupName))
    {
      // Exemplary group is NOT included in the test file
      stats.existsT = false;

      result = false;
      m_GroupOutput.append(stats.generateStatsLine());
    }
    else
    {
      // Exemplary group is included in the test file
      stats.existsT = true;
      createdGroupNames.remove(exemplaryGroupName);
    }
  }

  // If there are remaining group names in the test file, then they are not in the exemplary file
  for(const std::string& createdGroupName : createdGroupNames)
  {
    GroupStats stats;
    stats.objPath = getObjectPath(createdId, createdGroupName);
    stats.existsE = false;
    stats.existsT = true;

    result = false;
    m_GroupOutput.append(stats.generateStatsLine());
  }

  return result;
}

// -----------------------------------------------------------------------------
bool H5FileComparison::getObjectNames(hid_t createdId, std::list<std::string>& createdNames, hid_t exemplaryId, std::list<std::string>& exemplaryNames, H5Utilities::CustomHDFDataTypes dataType,
                                      std::string& testResults)
{
  std::stringstream ss;
  bool result = true;

  herr_t err = H5Utilities::getGroupObjects(createdId, dataType, createdNames);
  if(err < 0)
  {
    std::string objPath = H5Utilities::getObjectPath(createdId);
    ss << "Line " << __LINE__ << ": Could not get object names inside group at object path '" << objPath << "' in test file at path '" << m_TestFilePath << "'.";
    testResults.append(ss.str());
    result = false;
  }

  err = H5Utilities::getGroupObjects(exemplaryId, dataType, exemplaryNames);
  if(err < 0)
  {
    std::string objPath = H5Utilities::getObjectPath(exemplaryId);
    ss << "Line " << __LINE__ << ": Could not get object names inside group at object path '" << objPath << "' in exemplary file at path '" << m_ExemplaryFilePath << "'.";
    testResults.append(ss.str());
    result = false;
  }

  return result;
}

// -----------------------------------------------------------------------------
bool H5FileComparison::compareHDF5Files(hid_t createdId, hid_t exemplaryId)
{
  // Recursively call the method for all sub-groups
  std::list<std::string> createdGroupNames;
  std::list<std::string> exemplaryGroupNames;
  if(!getObjectNames(createdId, createdGroupNames, exemplaryId, exemplaryGroupNames, H5Utilities::H5Support_GROUP, m_GroupOutput))
  {
    return false;
  }
  bool result = compareGroups(createdId, createdGroupNames, exemplaryId, exemplaryGroupNames);

  std::list<std::string> createdDatasetNames;
  std::list<std::string> exemplaryDatasetNames;
  if(!getObjectNames(createdId, createdDatasetNames, exemplaryId, exemplaryDatasetNames, H5Utilities::H5Support_DATASET, m_DatasetOutput))
  {
    return false;
  }
  result = compareDatasets(createdId, createdDatasetNames, exemplaryId, exemplaryDatasetNames);

  // Recursively call the method for all sub-groups
  for(const std::string& createdGroupName : createdGroupNames)
  {
    hid_t newCreatedId = H5Utilities::openHDF5Object(createdId, createdGroupName);
    if(newCreatedId < 0)
    {
      std::stringstream ss;
      std::string objPath = getObjectPath(createdId, createdGroupName);
      ss << "Line " << __LINE__ << ": Could not open group at object path '" << objPath << "' in test file at path '" << m_TestFilePath << "'.";
      m_GroupOutput.append(ss.str());
      result = false;
      continue;
    }

    H5ScopedGroupSentinel createdSentinel(&newCreatedId, false);

    hid_t newExemplaryId = H5Utilities::openHDF5Object(exemplaryId, createdGroupName);
    if(newExemplaryId < 0)
    {
      std::stringstream ss;
      std::string objPath = getObjectPath(exemplaryId, createdGroupName);
      ss << "Line " << __LINE__ << ": Could not open group at object path '" << objPath << "' in exemplary file at path '" << m_ExemplaryFilePath << "'.";
      m_GroupOutput.append(ss.str());
      result = false;
      continue;
    }

    H5ScopedGroupSentinel exemplarySentinel(&newExemplaryId, false);

    if(!compareHDF5Files(newCreatedId, newExemplaryId))
    {
      result = false;
    }
  }

  return result;
}

// -----------------------------------------------------------------------------
bool H5FileComparison::compareHDF5Files()
{
  // Open both files
  hid_t createdFileRoot = H5Utilities::openFile(m_TestFilePath, true);
  if(createdFileRoot < 0)
  {
    std::cout << "Line " << __LINE__ << ": Test file at path '" << m_TestFilePath << "' could not be opened for reading.";
    return EXIT_FAILURE;
  }

  H5ScopedFileSentinel createdSentinel(&createdFileRoot, false);

  hid_t exemplaryFileRoot = H5Utilities::openFile(m_ExemplaryFilePath, true);
  if(exemplaryFileRoot < 0)
  {
    std::cout << "Line " << __LINE__ << ": Exemplary file at path '" << m_ExemplaryFilePath << "' could not be opened for reading.";
    return EXIT_FAILURE;
  }

  H5ScopedFileSentinel exemplarySentinel(&exemplaryFileRoot, false);

  bool result = compareHDF5Files(createdFileRoot, exemplaryFileRoot);

  // Add the headers to all tables that have content
  if(!m_GroupOutput.empty())
  {
    m_GroupOutput = DatasetStats::getTableHeaders() + m_GroupOutput;
  }
  if(!m_DatasetOutput.empty())
  {
    m_DatasetOutput = DatasetStats::getTableHeaders() + m_DatasetOutput;
  }
  if(!m_AttributeOutput.empty())
  {
    m_AttributeOutput = AttributeStats::getTableHeaders() + m_AttributeOutput;
  }

  return result;
}

// -----------------------------------------------------------------------------
std::string H5FileComparison::getGroupOutput() const
{
  return m_GroupOutput;
}

// -----------------------------------------------------------------------------
std::string H5FileComparison::getDatasetOutput() const
{
  return m_DatasetOutput;
}

// -----------------------------------------------------------------------------
std::string H5FileComparison::getAttributeOutput() const
{
  return m_AttributeOutput;
}
