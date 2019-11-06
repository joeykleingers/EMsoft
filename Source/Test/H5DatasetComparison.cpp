#include "H5DatasetComparison.h"

#include <iomanip>

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
std::string H5DatasetComparison::generateObjectPath(hid_t id, const std::string& objName)
{
  std::string objPath = H5Utilities::getObjectPath(id);
  if(objPath.at(objPath.size() - 1) != '/')
  {
    objPath.append("/");
  }
  objPath.append(objName);
  return objPath;
}
