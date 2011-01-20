// -*- mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// --------------------------------------------------------------------------
//                   OpenMS Mass Spectrometry Framework
// --------------------------------------------------------------------------
//  Copyright (C) 2003-2011 -- Oliver Kohlbacher, Knut Reinert
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
// $Maintainer: Clemens Groepl $
// $Authors: Hendrik Weisser $
// --------------------------------------------------------------------------

#ifndef OPENMS_ANALYSIS_MAPMATCHING_TRANSFORMATIONDESCRIPTION_H
#define OPENMS_ANALYSIS_MAPMATCHING_TRANSFORMATIONDESCRIPTION_H

#include <OpenMS/DATASTRUCTURES/Param.h>
#include <algorithm>

namespace OpenMS
{
	/**
	@brief Generic description of a coordinate transformation.
		
	This description stores the transformation name and parameters.
		
	The transformation can be applied to a double coordinate by using the @ref apply() method.
	@n Available transformations and parameters are:
	- none : \f$ f(x) = x \f$
	- linear : \f$ f(x) = \textit{intercept} + \textit{slope} * x \f$
	- interpolated_linear : Linear interpolation between pairs, extrapolation using first and last pair. At least two pairs must be given.
	- b_spline : Smoothing cubic B-spline.
		
	@ingroup MapAlignment
	*/
	class OPENMS_DLLAPI TransformationDescription
	{
		friend class MapAlignmentAlgorithm;

	 public:
			
		/// Coordinate pair vector type
		typedef std::vector< std::pair<DoubleReal,DoubleReal> > PairVector;
			
		/// Constructor
		TransformationDescription();
		/// Destructor
		~TransformationDescription();
				
		/// Copy constructor 
		TransformationDescription(const TransformationDescription& rhs);
		/// Assignment operator
		TransformationDescription& operator = (const TransformationDescription& rhs);
				
		/// Resets everything
		void clear();
			
		///Returns the name
    const String& getName() const;
		/// Sets the name
    void setName(const String& name);
			
		/// Non-mutable access to the parameters
		const Param& getParameters() const;
			
		/// Sets the name
		void setParameters(const Param& param);
			
		/// Returns the pairs
		const PairVector& getPairs() const;
		/// Returns the pairs
    PairVector& getPairs();
			
		/// Sets the pairs
		void setPairs(const PairVector& pairs);

    /**
    @brief Convenience method for const access to parameters

    @exception Exception::ElementNotFound is thrown if the parameter does not exist.
    */
    const DataValue& getParam(const String& name) const;

    /// Convenience method to set double parameters
    void setParam(const String& name, DoubleReal value);
			
		/// Convenience method to set Int parameters
		void setParam(const String& name, Int value);
				
    /// Convenience method to set String parameters
    void setParam(const String& name, const String& value);

		/**
		@brief Apply the transformation to @p value.
					 
		@exception Exception::IllegalArgument is thrown if the transformation cannot be initialized according to the given name and parameters.
		*/
		void apply(DoubleReal& value) const;
				
		/// Compute an (approximate) inverse of the transformation
		void getInverse(TransformationDescription& result);

    /// Get maximal RT difference for current model versus an alternative model
    /// , currently used for BSplines only when their fit is bad.
    DoubleReal getMaxRTErrorEstimate() const;
	 protected:
			
		/**@brief Base class for all transformations

		Derived classes are:
		<ul>

		<li>None_ : No transformation (i.e. identity) </li>

		<li>Linear_ : Linear transformation that actually applies an affine
		transformation ;-) </li>

		<li>InterpolatedLinear_ : Piecewise linear transformation.  In between the
		pairs, the interpolation uses the neighboring pairs.  Outside the range
		spanned by the pairs, we extrapolate using a line through the first and
		the last pair. (Each time this is applied, a binary search is performed.
		We could precompute slopes for each segment, but it is not clear if this
		will pay off.)  </li>

		<li>BSpline_ : Smoothing B-Spline transformation.  In between the
    pairs, the transformation function is given by a cubic B-spline
    approximating the pairs.  The number of breakpoints is given as a parameter.
    Below and above the range spanned by the pairs, we extrapolate using a line through
    the last point having the same slope as the spline at the last point, respectively.</li>

		</ul>

		(Note: The derived classes are defined in TransformationDescription.C .)
		*/
		struct OPENMS_DLLAPI Trafo_
		{
      Trafo_(const TransformationDescription&): max_rt_diff_(0)
      {
      }
			virtual void operator()(DoubleReal& value) const = 0;
			virtual void getInverse(TransformationDescription& result) = 0;
      DoubleReal getMaxRTErrorEstimate() const {return max_rt_diff_;}
     protected:
   		/// Max Error
      mutable DoubleReal max_rt_diff_;
		 private:			
			Trafo_(const Trafo_&) {}
		};
			
		/**
		@brief Initialize the transformation according to the name and parameters.
		
		This is declared a const method for usability, but in fact it changes
		"hidden" state.

		@exception Exception::IllegalArgument is thrown if the transformation
		cannot be initialized according to the name and parameters.
		*/
		void init_() const;
		
		/// Transformation name
		String name_;
		/// Transformation parameters
		Param param_;
		/// Pairs of corresponding values
		PairVector pairs_;
		/// Pointer to actual transformation functor
		Trafo_ * trafo_;

		/// See Trafo_ for documentation.
		struct None_;
			
		/// See Trafo_ for documentation
		struct Linear_;
		
		/// See Trafo_ for documentation
		struct InterpolatedLinear_;

		/// See Trafo_ for documentation
		struct BSpline_;
		
	};

	OPENMS_DLLAPI std::ostream& operator<<(std::ostream& os, TransformationDescription const & td);
	
} // end of namespace OpenMS

#endif  // OPENMS_ANALYSIS_MAPMATCHING_TRANSFORMATIONDESCRIPTION_H

