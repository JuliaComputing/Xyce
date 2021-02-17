//-------------------------------------------------------------------------
//   Copyright 2002-2020 National Technology & Engineering Solutions of
//   Sandia, LLC (NTESS).  Under the terms of Contract DE-NA0003525 with
//   NTESS, the U.S. Government retains certain rights in this software.
//
//   This file is part of the Xyce(TM) Parallel Electrical Simulator.
//
//   Xyce(TM) is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
//   Xyce(TM) is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with Xyce(TM).
//   If not, see <http://www.gnu.org/licenses/>.
//-------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
// Purpose        : Block Vector access
//
// Special Notes  :
//
// Creator        : Robert Hoekstra, SNL, Computational Sciences
//
// Creation Date  : 3/12/04
//
//
//
//
//-----------------------------------------------------------------------------

#ifndef Xyce_N_LAS_EpetraBlockVector_h
#define Xyce_N_LAS_EpetraBlockVector_h

#include <vector>

#include <N_LAS_BlockVector.h>
#include <N_LAS_Vector.h>
#include <N_PDS_fwd.h>

#include <Teuchos_RCP.hpp>

namespace Xyce {
namespace Linear {

//-----------------------------------------------------------------------------
// Class         : EpetraBlockVector
// Purpose       : Provides an Epetra implementation for block vectors
// Special Notes :
// Creator       : Robert Hoekstra, SNL, Computational Sciences
// Creation Date : 3/12/04
//-----------------------------------------------------------------------------
class EpetraBlockVector : public BlockVector
{
 public:
  EpetraBlockVector( int numBlocks,
               const Teuchos::RCP<const Parallel::ParMap> & globalMap,
               const Teuchos::RCP<const Parallel::ParMap> & subBlockMap,
               int augmentRows = 0 );

  // Constructor that uses the block size to divide up the number of elements on
  // each processor into vectors whose values are all "owned" by one processor.
  // NOTE:  This constructor is handy for frequency-domain representations of time-domain vectors.
  EpetraBlockVector( int blockSize,
               const Teuchos::RCP<const Parallel::ParMap> & globalMap,
               int augmentRows = 0 );

  // View constructor
  //NOTE:  This constructor assumes that the Vector is divided up into blockSize subvectors,
  //       whose values are solely owned by one of the processors.
  EpetraBlockVector( const Vector * right, int blockSize );

  // Destructor
  virtual ~EpetraBlockVector() {};

  // Assignment operator
  EpetraBlockVector & operator=(const EpetraBlockVector & right);

  // Block accessors
  Vector & block( int Loc ) const
  { return *blocks_[Loc]; }

  int blockSize() const
  { return globalBlockSize_; }

  int blockCount() const
  { return numBlocks_; }

  int startBlock() const
  { return startBlock_; }

  int endBlock() const
  { return endBlock_; }

  // Get the ParMap objects for each BLOCK in this block vector.
  const Parallel::ParMap * blockPmap() const { return newBlockMap_.get(); }

  // Print out the underlying data in this object.
  void print(std::ostream &os) const;

 private:

  EpetraBlockVector( const EpetraBlockVector & rhs );

  bool blocksViewGlobalVec_;
  int globalBlockSize_;
  int localBlockSize_;
  int overlapBlockSize_;
  int numBlocks_;
  int augmentCount_;

  // In frequency domain, whole blocks may be owned by one processor.
  // NOTE:  This assumes they are contiguous.  By default these routines
  //        will return 0 and numBlocks_ (which is sane for the time domain specs).
  int startBlock_, endBlock_;

  Teuchos::RCP<const Parallel::ParMap> newBlockMap_;

  std::vector<Teuchos::RCP<Vector> > blocks_;

};

} // namespace Linear
} // namespace Xyce

#endif

