#include "H5StructureComparison.h"

#include <iomanip>
#include <sstream>

#include "H5Support/H5ScopedSentinel.h"
#include "H5Support/H5Utilities.h"

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

// -----------------------------------------------------------------------------
H5StructureComparison::H5StructureComparison(std::string exemplaryFilePath, std::string testFilePath)
: m_ExemplaryFilePath(std::move(exemplaryFilePath))
, m_OutputFilePath(std::move(testFilePath))
{
}

// -----------------------------------------------------------------------------
bool H5StructureComparison::listContainsElement(const std::list<std::string>& list, const std::string& var)
{
  return (std::find(list.begin(), list.end(), var) != list.end());
}

// -----------------------------------------------------------------------------
std::string H5StructureComparison::getObjectPath(hid_t id, const std::string& objName)
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
bool H5StructureComparison::compareStructure(const std::list<std::string>& h5GroupPaths)
{
  bool result = true;

  for(const std::string& h5GroupPath : h5GroupPaths)
  {
    if(!compareStructure(h5GroupPath))
    {
      result = false;
    }
  }

  return result;
}

// -----------------------------------------------------------------------------
bool H5StructureComparison::compareStructure(const std::string& h5GroupPath)
{
  bool result = true;

  // Open both files
  hid_t testParentId = H5Utilities::openFile(m_OutputFilePath, true);
  if(testParentId < 0)
  {
    std::cout << "Line " << __LINE__ << ": Test file at path '" << m_OutputFilePath << "' could not be opened for reading." << std::endl;
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

  std::list<std::string> h5GroupNames = splitH5Path(h5GroupPath);
  for(const std::string& h5GroupName : h5GroupNames)
  {
    if(!compareGroup(testParentId, exemplaryParentId, h5GroupName))
    {
      result = false;
      break;
    }

    testParentId = H5Utilities::openHDF5Object(testParentId, h5GroupName);
    if(testParentId < 0)
    {
      std::stringstream ss;
      std::string objPath = getObjectPath(testParentId, h5GroupName);
      ss << "Line " << __LINE__ << ": Could not open group at object path '" << objPath << "' in test file at path '" << m_OutputFilePath << "'." << std::endl;
      m_Output.append(ss.str());
      return false;
    }

    testSentinel.addGroupId(&testParentId);

    exemplaryParentId = H5Utilities::openHDF5Object(exemplaryParentId, h5GroupName);
    if(exemplaryParentId < 0)
    {
      std::stringstream ss;
      std::string objPath = getObjectPath(exemplaryParentId, h5GroupName);
      ss << "Line " << __LINE__ << ": Could not open group at object path '" << objPath << "' in exemplary file at path '" << m_ExemplaryFilePath << "'." << std::endl;
      m_Output.append(ss.str());
      return false;
    }

    exemplarySentinel.addGroupId(&exemplaryParentId);
  }

  return result;
}

// -----------------------------------------------------------------------------
bool H5StructureComparison::compareGroup(hid_t testParentId, hid_t exemplaryParentId, const std::string& h5GroupName)
{
  bool result = true;
  std::string objPath = H5Utilities::getObjectPath(testParentId);

  std::list<std::string> testGroupNames;
  herr_t err = H5Utilities::getGroupObjects(testParentId, H5Utilities::H5Support_GROUP, testGroupNames);
  if(err < 0)
  {
    std::stringstream ss;
    ss << "Line " << __LINE__ << ": Could not get object names inside group at object path '" << objPath << "' in test file at path '" << m_OutputFilePath << "'." << std::endl;
    m_Output.append(ss.str());
    result = false;
  }

  std::list<std::string> exemplaryGroupNames;
  err = H5Utilities::getGroupObjects(exemplaryParentId, H5Utilities::H5Support_GROUP, exemplaryGroupNames);
  if(err < 0)
  {
    std::stringstream ss;
    ss << "Line " << __LINE__ << ": Could not get object names inside group at object path '" << objPath << "' in exemplary file at path '" << m_ExemplaryFilePath << "'." << std::endl;
    m_Output.append(ss.str());
    result = false;
  }

  std::stringstream ss;
  objPath = getObjectPath(testParentId, h5GroupName);
  ss << objPath << k_TableDelimiter;

  bool existsInE = listContainsElement(exemplaryGroupNames, h5GroupName);
  bool existsInT = listContainsElement(testGroupNames, h5GroupName);
  if(existsInE && existsInT)
  {
    if(!compareDatasets(testParentId, exemplaryParentId, h5GroupName))
    {
      result = false;
    }
  }
  else if(!existsInE && existsInT)
  {
    ss << k_NoChar << k_TableDelimiter << k_YesChar << std::endl;
    m_Output.append(ss.str());
    result = false;
  }
  else if(existsInE && !existsInT)
  {
    ss << k_YesChar << k_TableDelimiter << k_NoChar << std::endl;
    m_Output.append(ss.str());
    result = false;
  }
  else
  {
    ss << k_NoChar << k_TableDelimiter << k_NoChar << std::endl;
    m_Output.append(ss.str());
    result = false;
  }

  return result;
}

// -----------------------------------------------------------------------------
bool H5StructureComparison::compareDatasets(hid_t testParentId, hid_t exemplaryParentId, const std::string& h5GroupName)
{
  bool result = true;

  testParentId = H5Utilities::openHDF5Object(testParentId, h5GroupName);
  if(testParentId < 0)
  {
    std::stringstream ss;
    std::string objPath = getObjectPath(testParentId, h5GroupName);
    ss << "Line " << __LINE__ << ": Could not open group at object path '" << objPath << "' in test file at path '" << m_OutputFilePath << "'." << std::endl;
    m_Output.append(ss.str());
    return false;
  }

  H5ScopedGroupSentinel testSentinel(&testParentId, false);

  exemplaryParentId = H5Utilities::openHDF5Object(exemplaryParentId, h5GroupName);
  if(exemplaryParentId < 0)
  {
    std::stringstream ss;
    std::string objPath = getObjectPath(exemplaryParentId, h5GroupName);
    ss << "Line " << __LINE__ << ": Could not open group at object path '" << objPath << "' in exemplary file at path '" << m_ExemplaryFilePath << "'." << std::endl;
    m_Output.append(ss.str());
    return false;
  }

  H5ScopedGroupSentinel exemplarySentinel(&exemplaryParentId, false);

  std::string objPath = H5Utilities::getObjectPath(testParentId);

  std::list<std::string> testDSetNames;
  herr_t err = H5Utilities::getGroupObjects(testParentId, H5Utilities::H5Support_DATASET, testDSetNames);
  if(err < 0)
  {
    std::stringstream ss;
    ss << "Line " << __LINE__ << ": Could not get object names inside group at object path '" << objPath << "' in test file at path '" << m_OutputFilePath << "'." << std::endl;
    m_Output.append(ss.str());
    result = false;
  }

  std::list<std::string> exemplaryDSetNames;
  err = H5Utilities::getGroupObjects(exemplaryParentId, H5Utilities::H5Support_DATASET, exemplaryDSetNames);
  if(err < 0)
  {
    std::stringstream ss;
    ss << "Line " << __LINE__ << ": Could not get object names inside group at object path '" << objPath << "' in exemplary file at path '" << m_ExemplaryFilePath << "'." << std::endl;
    m_Output.append(ss.str());
    result = false;
  }

  for(const std::string& exemplaryDatasetName : exemplaryDSetNames)
  {
    std::stringstream ss;
    objPath = getObjectPath(exemplaryParentId, exemplaryDatasetName);
    ss << objPath << k_TableDelimiter;

    if(!listContainsElement(testDSetNames, exemplaryDatasetName))
    {
      ss << k_YesChar << k_TableDelimiter << k_NoChar << std::endl;
      m_Output.append(ss.str());
      result = false;
    }
  }

  for(const std::string& testDatasetName : testDSetNames)
  {
    std::stringstream ss;
    objPath = getObjectPath(testParentId, testDatasetName);
    ss << objPath << k_TableDelimiter;

    if(!listContainsElement(exemplaryDSetNames, testDatasetName))
    {
      ss << k_NoChar << k_TableDelimiter << k_YesChar << std::endl;
      m_Output.append(ss.str());
      result = false;
    }
  }

  return result;
}

// -----------------------------------------------------------------------------
std::list<std::string> H5StructureComparison::splitH5Path(const std::string& h5Path)
{
  std::list<std::string> h5Names;
  //  size_t slashPos = h5Path.find('/');
  //  while(slashPos != )

  std::istringstream f(h5Path);
  std::string s;
  while(getline(f, s, '/'))
  {
    h5Names.push_back(s);
  }

  return h5Names;
}

// -----------------------------------------------------------------------------
std::string H5StructureComparison::getOutput() const
{
  std::stringstream ss;
  ss << std::setw(k_TableTitleIndent) << "STRUCTURE ERROR TABLE\n";
  ss << line(k_TableLineLength);
  ss << "HDF5 Path" << k_TableDelimiter << "Exists In Exemplary" << k_TableDelimiter << "Exists In Test\n";
  ss << line(k_TableLineLength);
  return ss.str() + m_Output;
}
