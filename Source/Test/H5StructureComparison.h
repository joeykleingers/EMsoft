#include <list>
#include <string>

#include "H5Ipublic.h"

class H5StructureComparison
{
public:
  /**
   * @brief H5StructureComparison
   * @param exemplaryFilePath
   * @param testFilePath
   */
  H5StructureComparison(std::string exemplaryFilePath, std::string testFilePath);

  /**
   * @brief compareStructure
   * @param h5GroupPaths
   * @return
   */
  bool compareStructure(const std::list<std::string>& h5GroupPaths);

  /**
   * @brief getGroupOutput
   * @return
   */
  std::string getOutput() const;

private:
  std::string m_ExemplaryFilePath;
  std::string m_OutputFilePath;
  std::string m_Output;

  /**
   * @brief compareStructure
   * @param h5GroupPath
   * @return
   */
  bool compareStructure(const std::string& h5GroupPath);

  /**
   * @brief compareGroup
   * @param testParentId
   * @param exemplaryParentId
   * @param h5GroupName
   * @return
   */
  bool compareGroup(hid_t testParentId, hid_t exemplaryParentId, const std::string& h5GroupName);

  /**
   * @brief compareDatasets
   * @param testParentId
   * @param exemplaryParentId
   * @param h5GroupName
   * @return
   */
  bool compareDatasets(hid_t testParentId, hid_t exemplaryParentId, const std::string& h5GroupName);

  /**
   * @brief splitH5Path
   * @param h5Path
   * @return
   */
  std::list<std::string> splitH5Path(const std::string& h5Path);

  /**
   * @brief getObjectPath
   * @param id
   * @param objName
   * @return
   */
  std::string getObjectPath(hid_t id, const std::string& objName);

  /**
   * @brief listContainsElement
   * @param list
   * @param var
   * @return
   */
  bool listContainsElement(const std::list<std::string>& list, const std::string& var);
};
