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

#include "ChangeHDF5FileCommand.h"

#include "Common/HDF5DatasetSelectionWidget.h"
#include "Common/HDF5FileTreeModel.h"
#include "Common/HDF5FileTreeModelItem.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ChangeHDF5FileCommand::ChangeHDF5FileCommand(QString &oldFilePath, QString &newFilePath, HDF5DatasetSelectionWidget* widget, QUndoCommand* parent)
: QUndoCommand(parent)
, m_Widget(widget)
, m_OldFilePath(std::move(oldFilePath))
, m_NewFilePath(std::move(newFilePath))
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ChangeHDF5FileCommand::~ChangeHDF5FileCommand() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ChangeHDF5FileCommand::redo()
{
  m_OldDatasetPaths = m_Widget->getSelectedHDF5Paths();

  m_Widget->setInputFilePath(m_NewFilePath);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ChangeHDF5FileCommand::undo()
{
  m_Widget->setInputFilePath(m_OldFilePath);

  HDF5FileTreeModel* model = m_Widget->getModel();

  for (const QString & selectedHDF5Path : m_OldDatasetPaths)
  {
    QModelIndex index = model->hdf5PathToIndex(selectedHDF5Path);
    HDF5FileTreeModelItem* item = static_cast<HDF5FileTreeModelItem*>(index.internalPointer());
    item->setCheckState(Qt::Checked);
    model->m_SelectedHDF5Paths.push_back(selectedHDF5Path);
  }
}
