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

#include "ChangeHDF5DatasetStateCommand.h"

#include "Common/HDF5FileTreeModel.h"
#include "Common/HDF5FileTreeModelItem.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ChangeHDF5DatasetStateCommand::ChangeHDF5DatasetStateCommand(const QModelIndex &index, Qt::CheckState newState, HDF5FileTreeModel* model, QUndoCommand* parent)
: QUndoCommand(parent)
, m_Model(model)
, m_ModelIndex(index)
, m_OldState(qvariant_cast<Qt::CheckState>(index.data(Qt::CheckStateRole)))
, m_NewState(newState)
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ChangeHDF5DatasetStateCommand::~ChangeHDF5DatasetStateCommand() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ChangeHDF5DatasetStateCommand::redo()
{
  HDF5FileTreeModelItem* item = static_cast<HDF5FileTreeModelItem*>(m_ModelIndex.internalPointer());
  item->setCheckState(m_NewState);

  QString hdf5Path = item->generateHDFPath();
  if (m_NewState == Qt::Checked)
  {
    m_Model->m_SelectedHDF5Paths.push_back(hdf5Path);
  }
  else
  {
    m_Model->m_SelectedHDF5Paths.removeAll(hdf5Path);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ChangeHDF5DatasetStateCommand::undo()
{
  HDF5FileTreeModelItem* item = static_cast<HDF5FileTreeModelItem*>(m_ModelIndex.internalPointer());
  item->setCheckState(m_OldState);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QPersistentModelIndex ChangeHDF5DatasetStateCommand::getModelIndex() const
{
  return m_ModelIndex;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ChangeHDF5DatasetStateCommand::setModelIndex(const QPersistentModelIndex &value)
{
  m_ModelIndex = value;
}
