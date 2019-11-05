#include "H5DatasetComparison.h"

#include <iomanip>

namespace
{
const int k_TableTitleIndent = 65;
const int k_TableLineLength = 100;
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
H5DatasetComparison::H5DatasetComparison(std::string exemplaryFilePath, std::string testFilePath)
: m_ExemplaryFilePath(std::move(exemplaryFilePath))
, m_OutputFilePath(std::move(testFilePath))
{
}

// -----------------------------------------------------------------------------
std::string H5DatasetComparison::getObjectPath(hid_t id, const std::string& objName)
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
std::string H5DatasetComparison::getOutput() const
{
  std::stringstream ss;
  ss << std::setw(k_TableTitleIndent) << "DATASETS ERROR TABLE\n";
  ss << line(k_TableLineLength);
  ss << "HDF5 Path" << k_TableDelimiter << "Comparison Type" << k_TableDelimiter << "Exemplary Type" << k_TableDelimiter << "Test Type" << k_TableDelimiter << "Matching Dims" << k_TableDelimiter
     << "Matching Data\n";
  ss << line(k_TableLineLength);

  return ss.str() + m_Output;
}
