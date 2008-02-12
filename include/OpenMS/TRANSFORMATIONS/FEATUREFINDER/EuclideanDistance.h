// -*- C++: make; tab-width: 2; -*-
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
// $Maintainer: Ole Schulz-Trieglaff $
// --------------------------------------------------------------------------

#ifndef OPENMS_TRANSFORMATIONS_FEATUREFINDER_EUCLIDEANDISTANCE_H
#define OPENMS_TRANSFORMATIONS_FEATUREFINDER_EUCLIDEANDISTANCE_H

#include <OpenMS/TRANSFORMATIONS/FEATUREFINDER/BaseQuality.h>

#include <math.h>

namespace OpenMS
{
	/**
		@brief Measures the quality of a modelfit to some realworld data. 
	  
 		Quality is measured as the euclidean distance between model and data.
 		
 		@ingroup FeatureFinder
 	*/
  class EuclideanDistance 
    : public BaseQuality
  {

  public:
    /// Default constructor
    EuclideanDistance();

    /// destructor 
    virtual ~EuclideanDistance();

    /// evaluates the quality of the fit of @p model to @p set
    double evaluate(const IndexSet& set, const BaseModel<2>& model);
    
    /// evaluates the quality of the fit of @p model to @p set along dimension @p dim
    double evaluate(const IndexSet& set, const BaseModel<1>& model, UInt dim);

		/// creates instance of this class (function is called by factory).
    static BaseQuality* create() 
    { 
    	return new EuclideanDistance(); 
    }

		/// Name of this quality
    static const String getProductName() 
    { 
    	return "EuclideanDistance"; 
    }
		
		/// Returns significance
		double getPvalue() 
		{ 
			return pval_; 
		}
  };
}
#endif // OPENMS_TRANSFORMATIONS_FEATUREFINDER_EUCLIDEANDISTANCE_H
