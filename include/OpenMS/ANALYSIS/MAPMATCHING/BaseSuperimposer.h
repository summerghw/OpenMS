// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// --------------------------------------------------------------------------
//                   OpenMS Mass Spectrometry Framework
// --------------------------------------------------------------------------
//  Copyright (C) 2003-2008 -- Oliver Kohlbacher, Knut Reinert
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// --------------------------------------------------------------------------
// $Maintainer: Eva Lange$
// --------------------------------------------------------------------------


#ifndef OPENMS_ANALYSIS_MAPMATCHING_BASESUPERIMPOSER_H
#define OPENMS_ANALYSIS_MAPMATCHING_BASESUPERIMPOSER_H

#include <OpenMS/ANALYSIS/MAPMATCHING/LinearMapping.h>
#include <OpenMS/KERNEL/FeatureMap.h>

#include <utility>
#include <fstream>

namespace OpenMS
{

  /**
    @brief The base class of all superimposer algorithms.

     This class defines the basic interface for all superimposer
     algorithms. It works on two element maps (FeatureMap is the default map type) 
     computes a transformation, that maps the elements of one map (scene map) 
     as near as possible to the elements in the other map (model map).
     A element can be a DPeak, a DFeature or ConsensusFeature 
     (wheras DFeature is the default element type).
     
     Policy for copy constructor and assignment: element_map_ is
     maintained as pointer and taken shallow copy. 
  */
  template <typename MapT = FeatureMap<> >
  class BaseSuperimposer 
  	: public FactoryProduct
  {
  public:
    
    /** Symbolic names for indices for the two dimensions of LC-MS
    */
    enum LCMS
    {
      RT = 0,
      MZ = 1
    };

    /** Symbolic names for indices of element maps etc.
    This should make things more understandable and maintainable. 
    */
    enum Maps
    {
      MODEL = 0,
      SCENE = 1
    };

    /// Container for input elements
    typedef MapT PointMapType;

    /// Type of elements considered here
    typedef typename PointMapType::value_type PointType;

    /// Quality type
    typedef DoubleReal QualityType;

    /// Position type
    typedef DPosition < 2 > PositionType;

    //// Intensity type
    typedef DoubleReal IntensityType;

    /// Type of estimated transformation
    typedef LinearMapping TransformationType;

    /// Constructor
    BaseSuperimposer()
        : FactoryProduct("BaseSuperimposer"),
          final_transformation_()
    {
      element_map_[MODEL] = 0;
      element_map_[SCENE] = 0;
    }

    /// Destructor
    virtual ~BaseSuperimposer()
  	{
  	}

    /// Set element map
    void setElementMap(UInt const index, const PointMapType& element_map)
    {
      element_map_[index] = &element_map;
    }

    /// Get element map
    const PointMapType& getElementMap(UInt index)
    {
      return *element_map_[index];
    }

    /// Get element maps (non-mutable)
    const PointMapType& getElementMap(UInt index) const
    {
      return *element_map_[index];
    }

    /// Set transformation
    void setTransformation(UInt dim, const TransformationType& trafo)
    {
      final_transformation_[dim] = trafo;
    }

    /// Get transformation
    const TransformationType& getTransformation(UInt dim) const
    {
      return final_transformation_[dim];
    }

    /// Estimates the transformation for each grid cell
    virtual void run() = 0;


    /// Register all derived classes here
    static void registerChildren();

  protected:
    /// Two maps of elements to be matched
    PointMapType const * element_map_[2];

    /// Final transformation
    TransformationType final_transformation_[2];
  }
  ; // BaseSuperimposer

} // namespace OpenMS

#endif  // OPENMS_ANALYSIS_MAPMATCHING_BASESUPERIMPOSER_H
