// -*- mode: C++; tab-width: 2; -*-
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
// $Maintainer: Andreas Bertsch $
// --------------------------------------------------------------------------
//

#ifndef OPENMS_MATH_STATISTICS_GAUSSFITTER_H
#define OPENMS_MATH_STATISTICS_GAUSSFITTER_H

#include <OpenMS/DATASTRUCTURES/String.h>
#include <OpenMS/CONCEPT/Types.h>
#include <OpenMS/DATASTRUCTURES/DPosition.h>

#include <vector>

// gsl includes
#include <gsl/gsl_rng.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_multifit_nlin.h>

namespace OpenMS
{

	/** @brief Class which implements a fitter for gaussian functions
	
			This class is able to fit a gaussian distribution to a number of data points.
			The results as well as the initial guess are specified using the struct GaussFitResult.

			The complete gaussian formula with the fitted parameters can be transformed into a
			gnuplot formula using getGnuplotFormula after fitting.

			The fitting is implemented using the gsl fitting classes
	*/
	class GaussFitter
	{
		public:

			/// struct of parameters of a gaussian distribution
			struct GaussFitResult
			{
				public:

					/// parameter A of gaussian distribution (amplitude)
					double A;

					/// parameter x0 of gaussian distribution (left/right shift)
					double x0; 

					/// parameter sigma of gaussian distribution (width)
					double sigma;
			};
		
			/** @name Constructors and desctructors
			*/
			//@{
			/// Default constructor
			GaussFitter();

			/// Copy constructor
			GaussFitter(const GaussFitter& rhs);

			/// Destructor
			virtual ~GaussFitter();

			/// assignment operator 
			GaussFitter& operator = (const GaussFitter& rhs);

			/** fits a gaussian distribution to the given data points 

					@param points the data points used for the gaussian fitting
			*/
			GaussFitResult fit(std::vector<DPosition<2> >& points);

			/// returns the initial parameters of the gaussian
			const GaussFitResult& getInitialParameters() const;

			/// sets the initial parameters used by the fit method as inital guess for the gaussian
			void setInitialParameters(const GaussFitResult& result);

			/// return the gnuplot formula of the gaussian
			const String& getGnuplotFormula() const;
			
		protected:
			
			static int gaussFitterf_(const gsl_vector* x, void* params, gsl_vector* f);

			static int gaussFitterdf_(const gsl_vector* x, void* params, gsl_matrix* J);

			static int gaussFitterfdf_(const gsl_vector* x, void* params, gsl_vector* f, gsl_matrix* J);
			
			void printState_(size_t iter, gsl_multifit_fdfsolver * s);
			
			GaussFitResult init_param_;
			
			String gnuplot_formula_;
	};
}

#endif

