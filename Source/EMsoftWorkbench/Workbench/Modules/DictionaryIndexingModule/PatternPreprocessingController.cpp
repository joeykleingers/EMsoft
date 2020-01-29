/* ============================================================================
 * Copyright (c) 2009-2017 BlueQuartz Software, LLC
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * Neither the name of BlueQuartz Software, the US Air Force, nor the names of its
 * contributors may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The code contained herein was partially funded by the followig contracts:
 *    United States Air Force Prime Contract FA8650-07-D-5800
 *    United States Air Force Prime Contract FA8650-10-D-5210
 *    United States Prime Contract Navy N00173-07-C-2068
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "PatternPreprocessingController.h"

#include <cstring>

#include <QtCore/QCoreApplication>
#include <QtCore/QDateTime>
#include <QtCore/QMap>
#include <QtCore/QMimeDatabase>
#include <QtCore/QSharedPointer>
#include <QtCore/QTextStream>
#include <QtCore/QThread>

#include <QtGui/QImage>

#include "EMsoftLib/EMsoftStringConstants.h"

#include "Workbench/Common/FileIOTools.h"

#include "Constants.h"

static size_t k_InstanceKey = 0;
static QMap<size_t, PatternPreprocessingController*> instances;

namespace SizeConstants = DictionaryIndexingModuleConstants::ArraySizes;

/**
 * @brief PatternPreprocessingControllerProgress
 * @param instance
 * @param loopCompleted
 * @param totalLoops
 * @param bseYield
 */
void PatternPreprocessingControllerProgress(size_t instance, int loopCompleted, int totalLoops)
{
  PatternPreprocessingController* obj = instances[instance];
  if(nullptr != obj)
  {
    obj->setUpdateProgress(loopCompleted, totalLoops);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
PatternPreprocessingController::PatternPreprocessingController(QObject* parent)
: QObject(parent)
{
  m_InstanceKey = ++k_InstanceKey;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
PatternPreprocessingController::~PatternPreprocessingController()
{
  k_InstanceKey--;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PatternPreprocessingController::setData(const InputDataType& data)
{
  m_InputData = data;
}

// -----------------------------------------------------------------------------
void PatternPreprocessingController::cancelProcess()
{
  if(m_CurrentProcess != nullptr)
  {
    m_CurrentProcess->kill();
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PatternPreprocessingController::executeWrapper()
{
  //  // WIP: This code currently calls out to a QProcess, but eventually it will use a C++ callback method instead

  ////  initializeData();

  ////  std::vector<int32_t> iParVector = data.getIParVector();
  ////  std::vector<float> fParVector = data.getFParVector();
  ////  std::vector<char> sParVector = data.getSParVector();

  ////  // Create a new Mask Array
  ////  m_OutputMaskVector.resize(data.patternHeight * data.patternWidth);
  ////  std::fill(m_OutputMaskVector.begin(), m_OutputMaskVector.end(), 0.0f);

  ////  // Create a new IQ Map Array
  ////  m_OutputIQMapVector.resize(data.patternHeight * data.patternWidth);
  ////  std::fill(m_OutputIQMapVector.begin(), m_OutputIQMapVector.end(), 0.0f);

  ////  // Create a new ADP Map Array
  ////  m_OutputADPMapVector.resize(data.patternHeight * data.patternWidth);
  ////  std::fill(m_OutputADPMapVector.begin(), m_OutputADPMapVector.end(), 0.0f);

  //  // Set the start time for this run (m_StartTime)
  //  m_StartTime = QDateTime::currentDateTime().time().toString();

  //  // the EMsoft call will return two arrays: mLPNH and mLPSH
  //  // call the EMsoft EMsoftCgetEBSDmaster routine to compute the patterns;
  //  // m_Executing enables the Cancel button to properly work by passing
  //  // on a m_Cancel flag to the EMsoft routine; the m_InstanceKey provides
  //  // a unique label to this particular instantiation of this filter, so that
  //  // multiple simultaneous instantiations of this filter become possible without
  //  // incorrect interactions between the callback routines.
  //  m_Executing = true;
  //  instances[m_InstanceKey] = this;
  ////  EMsoftCpreprocessEBSDPatterns(iParVector.data(), fParVector.data(), sParVector.data(), m_OutputMaskVector.data(), m_OutputIQMapVector.data(), m_OutputADPMapVector.data(),
  ///&PatternPreprocessingControllerProgress, m_InstanceKey, &m_Cancel);

  //  QSharedPointer<QProcess> ppMatrixProcess = QSharedPointer<QProcess>(new QProcess());
  //  connect(ppMatrixProcess.data(), &QProcess::readyReadStandardOutput, [=] { emit stdOutputMessageGenerated(QString::fromStdString(ppMatrixProcess->readAllStandardOutput().toStdString())); });
  //  connect(ppMatrixProcess.data(), &QProcess::readyReadStandardError, [=] { emit stdOutputMessageGenerated(QString::fromStdString(ppMatrixProcess->readAllStandardError().toStdString())); });
  //  connect(ppMatrixProcess.data(), QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [=](int exitCode, QProcess::ExitStatus exitStatus) {
  //  listenPreprocessedPatternsMatrixFinished(exitCode, exitStatus); }); QString ppMatrixExecutablePath = getPreprocessedPatternsMatrixExecutablePath(); if (!ppMatrixExecutablePath.isEmpty())
  //  {
  //    QString nmlFilePath = m_TempDir.path() + QDir::separator() + "EMEBSDDIpreview.nml";
  //    writePreprocessedPatternsMatrixToFile(nmlFilePath, data);
  //    QStringList parameters = {nmlFilePath};
  //    ppMatrixProcess->start(ppMatrixExecutablePath, parameters);

  //    // Wait until the QProcess is finished to exit this thread.
  //    // PatternPreprocessingController::createPreprocessedPatternsMatrix is currently on a separate thread, so the GUI will continue to operate normally
  //    ppMatrixProcess->waitForFinished(-1);
  //  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PatternPreprocessingController::execute()
{
  QString dtFormat("yyyy:MM:dd hh:mm:ss.zzz");

  QTemporaryDir tempDir;
  // Set the start time for this run (m_StartTime)
  QString str;
  QTextStream out(&str);

  m_CurrentProcess = QSharedPointer<QProcess>(new QProcess());
  connect(m_CurrentProcess.data(), &QProcess::readyReadStandardOutput, [=] { emit stdOutputMessageGenerated(QString::fromStdString(m_CurrentProcess->readAllStandardOutput().toStdString())); });
  connect(m_CurrentProcess.data(), &QProcess::readyReadStandardError, [=] { emit errorMessageGenerated(QString::fromStdString(m_CurrentProcess->readAllStandardError().toStdString())); });
  connect(m_CurrentProcess.data(), QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [=](int exitCode, QProcess::ExitStatus exitStatus) { processFinished(exitCode, exitStatus); });
  std::pair<QString, QString> result = FileIOTools::GetExecutablePath(k_ExeName);
  if(!result.first.isEmpty())
  {
    QProcessEnvironment env = m_CurrentProcess->processEnvironment();
    env.insert("EMSOFTPATHNAME", QString::fromStdString(FileIOTools::GetEMsoftPathName()));
    m_CurrentProcess->setProcessEnvironment(env);
    out << k_ExeName << ": Executable Path:" << result.first << "\n";
    out << k_ExeName << ": Start Time: " << QDateTime::currentDateTime().toString(dtFormat) << "\n";
    out << k_ExeName << ": Insert EMSOFTPATHNAME=" << QString::fromStdString(FileIOTools::GetEMsoftPathName()) << "\n";
    out << k_ExeName << ": Output from " << k_ExeName << " follows next...."
        << "\n";
    out << "===========================================================\n";

    emit stdOutputMessageGenerated(str);

    QString nmlFilePath = tempDir.path() + QDir::separator() + k_NMLName;
    generateNMLFile(nmlFilePath);
    QStringList parameters = {nmlFilePath};
    m_CurrentProcess->start(result.first, parameters);

    // Wait until the QProcess is finished to exit this thread.
    m_CurrentProcess->waitForFinished(-1);
  }
  else
  {
    emit errorMessageGenerated(result.second);
  }

  str = "";
  out << "===========================================================\n";
  out << k_ExeName << ": Finished: " << QDateTime::currentDateTime().toString(dtFormat) << "\n";
  //  out << k_ExeName << ": Output File Location: " << m_InputData.inputFilePath << "\n";
  emit stdOutputMessageGenerated(str);

  emit finished();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PatternPreprocessingController::generateNMLFile(const QString& filePath) const
{
  std::vector<std::string> nml;

  nml.emplace_back(std::string(" &EBSDDIpreviewdata"));
  nml.emplace_back(std::string("! The line above must not be changed"));
  nml.emplace_back(std::string("! The values below are the default values for this program"));
  nml.emplace_back(std::string("! number of pattern pixels along x and y"));
  nml.emplace_back(FileIOTools::CreateNMLEntry(EMsoft::Constants::numsx, m_InputData.patternWidth));
  nml.emplace_back(FileIOTools::CreateNMLEntry(EMsoft::Constants::numsy, m_InputData.patternHeight));
  nml.emplace_back(std::string("! number of patterns along x and y"));
  nml.emplace_back(FileIOTools::CreateNMLEntry(EMsoft::Constants::ipfwd, m_InputData.ipfWidth));
  nml.emplace_back(FileIOTools::CreateNMLEntry(EMsoft::Constants::ipfht, m_InputData.ipfHeight));
  nml.emplace_back(std::string("! hipass w parameter range and number of steps (starts near zero)"));
  nml.emplace_back(FileIOTools::CreateNMLEntry(EMsoft::Constants::hipasswmax, m_InputData.hipassValue));
  nml.emplace_back(FileIOTools::CreateNMLEntry(EMsoft::Constants::hipasswnsteps, m_InputData.hipassNumSteps));
  nml.emplace_back(std::string("! number of regions for adaptive histogram equalization"));
  nml.emplace_back(FileIOTools::CreateNMLEntry(EMsoft::Constants::nregionsmin, m_InputData.minNumOfRegions));
  nml.emplace_back(FileIOTools::CreateNMLEntry(EMsoft::Constants::nregionsmax, m_InputData.maxNumOfRegions));
  nml.emplace_back(FileIOTools::CreateNMLEntry(EMsoft::Constants::nregionsstepsize, m_InputData.numOfRegionsStepSize));
  nml.emplace_back(std::string("! name of tiff output file; will contain an array of pre-processed patterns"));
  nml.emplace_back(FileIOTools::CreateNMLEntry(EMsoft::Constants::tifffile, QString("%1/MatrixResult.tiff").arg(m_TempDir.path())));
  nml.emplace_back(std::string("! name of pattern output file; will contain raw experimental pattern (tiff, pgm, bmp)"));
  nml.emplace_back(FileIOTools::CreateNMLEntry(EMsoft::Constants::patternfile, QString("%1/ReferenceResult.tiff").arg(m_TempDir.path())));
  nml.emplace_back(std::string("! name of datafile where the patterns are stored; path relative to EMdatapathname"));
  nml.emplace_back(FileIOTools::CreateNMLEntry(EMsoft::Constants::exptfile, m_InputData.patternDataFile));
  nml.emplace_back(std::string("! input file type parameter: Binary, EMEBSD, TSLHDF, TSLup2, OxfordHDF, OxfordBinary, BrukerHDF"));

  switch(m_InputData.inputType)
  {
  case InputType::Binary:
    nml.emplace_back(FileIOTools::CreateNMLEntry(EMsoft::Constants::inputtype, QString("Binary")));
    break;
  case InputType::TSLup1:
    nml.emplace_back(FileIOTools::CreateNMLEntry(EMsoft::Constants::inputtype, QString("TSLup1")));
    break;
  case InputType::TSLup2:
    nml.emplace_back(FileIOTools::CreateNMLEntry(EMsoft::Constants::inputtype, QString("TSLup2")));
    break;
  case InputType::OxfordBinary:
    nml.emplace_back(FileIOTools::CreateNMLEntry(EMsoft::Constants::inputtype, QString("OxfordBinary")));
    break;
  case InputType::EMEBSD:
    nml.emplace_back(FileIOTools::CreateNMLEntry(EMsoft::Constants::inputtype, QString("EMEBSD")));
    break;
  case InputType::TSLHDF:
    nml.emplace_back(FileIOTools::CreateNMLEntry(EMsoft::Constants::inputtype, QString("TSLHDF")));
    break;
  case InputType::OxfordHDF:
    nml.emplace_back(FileIOTools::CreateNMLEntry(EMsoft::Constants::inputtype, QString("OxfordHDF")));
    break;
  case InputType::BrukerHDF:
    nml.emplace_back(FileIOTools::CreateNMLEntry(EMsoft::Constants::inputtype, QString("BrukerHDF")));
    break;
  }

  nml.emplace_back(std::string("! here we enter the HDF group names and data set names as individual strings (up to 10)"));
  nml.emplace_back(std::string("! enter the full path of a data set in individual strings for each group, in the correct order,"));
  nml.emplace_back(std::string("! and with the data set name as the last name; leave the remaining strings empty (they should all"));
  nml.emplace_back(std::string("! be empty for the Binary and TSLup2 formats)"));

  QString hdfStrings;
  for(int k = 0; k < 10; k++)
  {
    if(k < m_InputData.hdfStrings.size())
    {
      hdfStrings.append("'" + m_InputData.hdfStrings.at(k) + "'");
    }
    else
    {
      hdfStrings.append("''");
    }
    if(k < 9)
    {
      hdfStrings.append(" ");
    }
  }
  nml.emplace_back(FileIOTools::CreateNMLEntry(EMsoft::Constants::HDFstrings, hdfStrings, false, false));

  nml.emplace_back(std::string("! pattern coordinates to be used for the preview"));
  nml.emplace_back(FileIOTools::CreateNMLEntry(EMsoft::Constants::patx, m_InputData.patternCoordinateX));
  nml.emplace_back(FileIOTools::CreateNMLEntry(EMsoft::Constants::paty, m_InputData.patternCoordinateY));
  nml.emplace_back(std::string(" /"));

  QFile outputFile(filePath);
  if(outputFile.open(QFile::WriteOnly))
  {
    QTextStream out(&outputFile);

    for(const auto& entry : nml)
    {
      out << QString::fromStdString(entry) << "\n";
    }
    outputFile.close();
    // outputFile.copy("/tmp/EMMCOpenCL.nml");
  }
  else
  {
    emit errorMessageGenerated(QString("Could not create temp NML file at path %1").arg(filePath));
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PatternPreprocessingController::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
  // EMgetADP program automatically adds "_ADP" onto the end of the output image name
  QString imagePath = tr("%1/MatrixResult.tiff").arg(m_TempDir.path());
  QImage imageResult(imagePath);
  if(!imageResult.isNull())
  {
    emit preprocessedPatternsMatrixCreated(imageResult);
  }

  m_Executing = false;
  instances.remove(m_InstanceKey);

  // do we need to write this accumulator data into an EMsoft .h5 file?
  // This is so that the results can be read by other EMsoft programs outside of DREAM.3D...
  if(m_Cancel)
  {
    emit stdOutputMessageGenerated(QString("%1 was canceled.").arg(k_ExeName));
  }

  if(exitStatus == QProcess::CrashExit)
  {
    emit stdOutputMessageGenerated(QString("%1n process crashed with exit code %2").arg(k_ExeName).arg(exitCode));
  }

  if(exitStatus == QProcess::NormalExit)
  {
    emit stdOutputMessageGenerated(QString("%1 Completed").arg(k_ExeName));
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString PatternPreprocessingController::getPreprocessedPatternsMatrixExecutablePath() const
{
  QString executableName = "EMEBSDDIpreview";
  QString executablePath;

  QDir workingDirectory = QDir(QCoreApplication::applicationDirPath());

#if defined(Q_OS_WIN)
  if(workingDirectory.exists(tr("%1.exe").arg(executableName)))
  {
    executablePath = tr("%1%2%3").arg(workingDirectory.absolutePath(), QDir::separator(), tr("%1.exe").arg(executableName));
    return executablePath;
  }
#elif defined(Q_OS_MAC)
  // Look to see if we are inside an .app package or inside the 'tools' directory
  if(workingDirectory.dirName() == "MacOS")
  {
    workingDirectory.cdUp();
    if(workingDirectory.cd("bin"))
    {
      if(workingDirectory.exists(executableName))
      {
        executablePath = tr("%1%2%3").arg(workingDirectory.absolutePath(), QDir::separator(), executableName);
        return executablePath;
      }
      workingDirectory.cdUp();
    }

    workingDirectory.cdUp();
    workingDirectory.cdUp();

    if(workingDirectory.dirName() == "Bin" && workingDirectory.exists(executableName))
    {
      executablePath = tr("%1%2%3").arg(workingDirectory.absolutePath(), QDir::separator(), executableName);
      return executablePath;
    }
  }
#else
  if(workingDirectory.exists(tr("%1.exe").arg(executableName)))
  {
    executablePath = tr("%1%2%3").arg(workingDirectory.absolutePath(), QDir::separator(), tr("%1.exe").arg(executableName));
    return executablePath;
  }
#endif

  if(executablePath.isEmpty())
  {
    QString errMsg = tr("Could not find '%1' executable!").arg(executableName);
    emit errorMessageGenerated(errMsg);
  }

  return executablePath;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PatternPreprocessingController::initializeData()
{
  m_OutputMaskVector.clear();
  m_OutputIQMapVector.clear();
  m_OutputADPMapVector.clear();
  m_StartTime = "";
  m_Executing = false;
  m_Cancel = false;
}

//// -----------------------------------------------------------------------------
////
//// -----------------------------------------------------------------------------
// std::vector<int32_t> PatternPreprocessingController::PPMatrixData::getIParVector() const
//{
//  std::vector<int32_t> iParVector(SizeConstants::IParSize, 0);

//  iParVector[18] = patternWidth;
//  iParVector[19] = patternHeight;
//  iParVector[25] = ipfWidth;
//  iParVector[26] = ipfHeight;
//  iParVector[34] = static_cast<int>(inputType);
//  iParVector[44] = minNumOfRegions;
//  iParVector[45] = numOfRegionsStepSize;
//  iParVector[47] = patternCoordinateX;
//  iParVector[48] = patternCoordinateY;

//  // Used in wrapper routine, but not NML file...
//  iParVector[46] = numav;
//  iParVector[49] = numw;
//  iParVector[50] = numr;

//  return iParVector;
//}

//// -----------------------------------------------------------------------------
////
//// -----------------------------------------------------------------------------
// std::vector<float> PatternPreprocessingController::PPMatrixData::getFParVector() const
//{
//  std::vector<float> fParVector(SizeConstants::FParSize, 0.0f);

//  fParVector[23] = hipassFilter;
//  fParVector[25] = hipassValue;

//  return fParVector;
//}

//// -----------------------------------------------------------------------------
////
//// -----------------------------------------------------------------------------
// std::vector<char> PatternPreprocessingController::PPMatrixData::getSParVector() const
//{
//  // Move each string from the string array into the char array.  Each string has SParStringSize as its max size.
//  std::vector<char> sParVector(SizeConstants::SParSize * SizeConstants::SParStringSize, 0);
//  char* sPar = sParVector.data();

////  // Move Output File Path into the vector
////  const char* charArray = outputFilePath.toStdString().c_str();
////  std::memcpy(sPar + (30 * SizeConstants::SParStringSize), charArray, outputFilePath.size());

//  // Move Pattern Data File into the vector
//  const char* charArray = patternDataFile.toStdString().c_str();
//  std::memcpy(sPar + (31 * SizeConstants::SParStringSize), charArray, patternDataFile.size());

//  // Move HDF Strings into the vector
//  int count = 40;
//  for (const QString &hdfString : hdfStrings)
//  {
//    charArray = hdfString.toStdString().c_str();
//    std::memcpy(sPar + (count * SizeConstants::SParStringSize), charArray, hdfString.size());
//    count++;
//  }

//  return sParVector;
//}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PatternPreprocessingController::setUpdateProgress(int loopCompleted, int totalLoops)
{
  QString ss = QObject::tr("Average Dot Product: %1 of %2").arg(loopCompleted, totalLoops);
  emit stdOutputMessageGenerated(ss);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int PatternPreprocessingController::getNumCPUCores()
{
  return QThread::idealThreadCount();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool PatternPreprocessingController::getCancel() const
{
  return m_Cancel;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PatternPreprocessingController::setCancel(const bool& value)
{
  m_Cancel = value;
}
