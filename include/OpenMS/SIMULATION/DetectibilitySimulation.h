// -*- mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// --------------------------------------------------------------------------
//                   OpenMS Mass Spectrometry Framework
// --------------------------------------------------------------------------
//  Copyright (C) 2003-2009 -- Oliver Kohlbacher, Knut Reinert
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
// $Maintainer: Stephan Aiche$
// $Authors: Stephan Aiche Chris Bielow$
// --------------------------------------------------------------------------

#ifndef OPENMS_SIMULATION_DETECTIBILITYSIMULATION_H
#define OPENMS_SIMULATION_DETECTIBILITYSIMULATION_H

#include <OpenMS/DATASTRUCTURES/DefaultParamHandler.h>
#include <OpenMS/SIMULATION/SimTypes.h>

namespace OpenMS
{
  /**
   @brief 
   @ingroup Simulation
  */
  class OPENMS_DLLAPI DetectibilitySimulation
    : public DefaultParamHandler
  {

  public:
    /** @name Constructors and Destructors
      */
    //@{
    /// Constructor taking a random generator
    DetectibilitySimulation();

    /// Copy constructor
    DetectibilitySimulation(const DetectibilitySimulation& source);

    /// Destructor
    virtual ~DetectibilitySimulation();
    //@}

    DetectibilitySimulation& operator = (const DetectibilitySimulation& source);
    
    void filterDetectibility(FeatureMapSim &);
    
  private:
    /// set defaults
    void setDefaultParams_();
    
    /// Synchronize members with param class
		void updateMembers_();    
    
    /// Minimum allowed detectability likelihood of a peptide
		DoubleReal min_detect_;    
		
		/// Name of the svm model file
		OpenMS::String dtModelFile_;

  protected:
    /// filter using a svm model
    void svm_filter(FeatureMapSim &);
    
    /// no filter just let them pass
    void no_filter(FeatureMapSim &);
    
  };

}

#endif
