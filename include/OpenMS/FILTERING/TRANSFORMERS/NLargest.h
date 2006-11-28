// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// --------------------------------------------------------------------------
//                   OpenMS Mass Spectrometry Framework
// --------------------------------------------------------------------------
//  Copyright (C) 2003-2006 -- Oliver Kohlbacher, Knut Reinert
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
// $Maintainer: Andreas Bertsch $
// --------------------------------------------------------------------------
//
#ifndef OPENMS_FILTERING_TRANSFORMERS_NLARGEST_H
#define OPENMS_FILTERING_TRANSFORMERS_NLARGEST_H

#include <OpenMS/FILTERING/TRANSFORMERS/PreprocessingFunctor.h>

namespace OpenMS
{

	/**	
  	@brief NLargest removes all but the n largest peaks<br>
  
  	@param n number of peaks to keep

		@ingroup SpectraPreprocessing
  */
  class NLargest
    : public PreprocessingFunctor
  {
  public:

		// @name Constructors and Destructors
		// @{
    /// default constructor
    NLargest();

		/// detailed constructor
		NLargest(Size n);

    /// copy constructor 
    NLargest(const NLargest& source);

    /// destructor
    virtual ~NLargest();
		// @}

		// @name Operators
		// @{
    /// assignment operator
    NLargest& operator=(const NLargest& source);
		// @}

		// @name Accessors
		// @{
		// static create function for factory
    static PreprocessingFunctor* create() 
		{ 
			return new NLargest();
		}	
		
		/// static name method to register with factory
		static const String getName()
		{
			return "NLargest";
		}

		///
		template <typename SpectrumType> void filterSpectrum(SpectrumType& spectrum)
		{
			// get parameter how many peaks are wanted
			unsigned int n = (unsigned int)param_.getValue("n");
			if (spectrum.size() <= n) return;
			
			// sort by reverse intensity
			spectrum.getContainer().sortByIntensity(true);

			// keep the n largest peaks if more than n are present
			spectrum.resize(n);
		}

		void filterPeakSpectrum(PeakSpectrum& spectrum);

		void filterPeakMap(PeakMap& exp);
		// @}
	
  };
	
}
#endif //OPENMS_FILTERING_TRANSFORMERS_NLARGEST_H
