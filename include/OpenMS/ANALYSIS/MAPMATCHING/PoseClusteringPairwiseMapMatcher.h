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
// $Maintainer: Eva Lange $
// --------------------------------------------------------------------------


#ifndef OPENMS_ANALYSIS_MAPMATCHING_POSECLUSTERINGPAIRWISEMAPMATCHER_H
#define OPENMS_ANALYSIS_MAPMATCHING_POSECLUSTERINGPAIRWISEMAPMATCHER_H

#include <OpenMS/ANALYSIS/MAPMATCHING/BasePairwiseMapMatcher.h>
#include <OpenMS/ANALYSIS/MAPMATCHING/PoseClusteringShiftSuperimposer.h>
#include <OpenMS/ANALYSIS/MAPMATCHING/PoseClusteringAffineSuperimposer.h>
#include <OpenMS/ANALYSIS/MAPMATCHING/BaseSuperimposer_impl.h>
#include <OpenMS/ANALYSIS/MAPMATCHING/BasePairFinder.h>
#include <OpenMS/ANALYSIS/MAPMATCHING/SimplePairFinder.h>
#include <OpenMS/ANALYSIS/MAPMATCHING/BasePairFinder_impl.h>
#include <OpenMS/CONCEPT/Factory.h>
#include <OpenMS/DATASTRUCTURES/StringList.h>

namespace OpenMS
{
  /**
		@brief This class represents a point matching algorithm.
		
		It works on two point maps and computes a vector of corresponding points
		in both maps (given by a point pairs vector). 
		A point can be a DPeak, a DFeature or ConsensusFeature 
		(wheras DFeature is the default element type).
		
		Therefore it first estimates the transformation, which maps one point map
		onto the other. This is done by a superimposer class 
		(e.g. PoseClusteringShiftSuperimposer or the PoseClusteringAffineSuperimposer).
		Afterwards a pairfinder class (e.g. SimplePairFinder or DelaunayPairFinder) 
		dewarps the data and searches for corresponding points in both maps.
		    
		@note If a piecewise transformation is assumed, the user can define a grid by setting 
		the number of buckets in the RT as well as the MZ dimension.   
		Call initGridTransformation() before run()!   
		 
		@ref PoseClusteringPairwiseMapMatcher_Parameters are explained on a separate page.
		
		@ingroup MatchingAlgorithm
  */
  template < typename MapT = FeatureMap< > >
  class PoseClusteringPairwiseMapMatcher 
  	: public BasePairwiseMapMatcher<MapT>
  {
  public:
    /** Symbolic names for indices of element maps etc.
          This should make things more understandable and maintainable.
           */
    enum Maps
    {
      MODEL = 0,
      SCENE = 1
    };

    /// Base
    typedef BasePairwiseMapMatcher<MapT> Base;

    typedef typename Base::PointMapType PointMapType;
    typedef typename Base::ElementType ElementType;
    typedef typename Base::PositionType PositionType;
    typedef typename Base::CoordinateType CoordinateType;

    using Base::param_;
    using Base::defaults_;
    using Base::subsections_;
    using Base::element_map_;
    using Base::grid_;
    using Base::all_element_pairs_;


    /// Constructor
    PoseClusteringPairwiseMapMatcher()
        : Base(),
        	superimposer_(0),
        	pair_finder_(0)
    {
    	//set the name for DefaultParamHandler error messages
    	Base::setName(getProductName());
    	
      defaults_.setValue("pairfinder:type", "DelaunayPairFinder","The pair finder used ");
			defaults_.setValidStrings("pairfinder:type",Factory<BasePairFinder<PointMapType> >::registeredProducts());
			defaults_.setValue("superimposer:type", "poseclustering_affine","The superimposer used ");
			StringList superimposer_list = Factory<BaseSuperimposer<PointMapType> >::registeredProducts();
			superimposer_list.push_back("none");
			defaults_.setValidStrings("superimposer:type",superimposer_list);
			subsections_.push_back("pairfinder");
			subsections_.push_back("superimposer");
			
			Base::defaultsToParam_();
    }

    /// Destructor
    virtual ~PoseClusteringPairwiseMapMatcher()
  	{
  	}

    /// Returns an instance of this class
    static BasePairwiseMapMatcher<MapT>* create()
    {
      return new PoseClusteringPairwiseMapMatcher();
    }

    /// Returns the name of this module
    static const String getProductName()
    {
      return "poseclustering_pairwise";
    }

    /// Estimates the transformation for each grid cell and searches for element pairs.
    virtual void run()
    {
      // clear pairs
      all_element_pairs_.clear();

      // assign each element of the scene map to the grid cells and build a pointer map for each grid cell
      PointMapType scene_pointer_map(element_map_[SCENE]->begin(), element_map_[SCENE]->end());
      PointMapType scene_grid_map;
    	/// Initializes a peak pointer map for each grid cell of the scene (second) map
      for (UInt i = 0; i < scene_pointer_map.size(); ++i)
      {
        scene_grid_map.push_back(scene_pointer_map[i]);
      }

      // initialize a pointer map with the elements of the first (model or reference) map
      PointMapType model_pointer_map(element_map_[MODEL]->begin(), element_map_[MODEL]->end());

      // compute the matching of each scene's grid cell elements and all the elements of the model map
      computeMatching_(model_pointer_map,scene_grid_map);
    }

  protected:
   virtual void updateMembers_()
    {
      Base::updateMembers_();
			
			//create pairfinder if it does not exist or if it changed
			if (pair_finder_==0 || pair_finder_->getName()!=param_.getValue("pairfinder:type"))
			{
				delete pair_finder_;
      	pair_finder_ = Factory<BasePairFinder<PointMapType> >::create(param_.getValue("pairfinder:type"));
      }
      //update pairfinder parameters if necessary
      Param param_copy = param_.copy("pairfinder:",true);
      param_copy.remove("type");
      if (!(pair_finder_->getParameters()==param_copy))
      {
      	pair_finder_->setParameters(param_copy);
     	}
     	 
      //update superimposer
      String type = param_.getValue("superimposer:type");
      if (superimposer_==0)
      {
      	if (type != "none")
      	{
      		delete superimposer_;
        	superimposer_ = Factory<BaseSuperimposer<PointMapType> >::create(type);
      	}
      }
      else
      {
      	if (type == "none")
      	{
      		delete superimposer_;
      	}
      	else if (superimposer_->getName()!=type)
      	{
      		delete superimposer_;
        	superimposer_ = Factory<BaseSuperimposer<PointMapType> >::create(type);
      	}
      }
      //update superimposer parameters if necessary
      if (superimposer_!=0)
      {
      	Param param_copy = param_.copy("superimposer:",true);	
      	param_copy.remove("type");
	      if (!(superimposer_->getParameters()==param_copy))
	      {
	      	superimposer_->setParameters(param_copy);
	     	}
	  	}
    }
    
    /// This class computes the shift for the best mapping of one element map to another
    BaseSuperimposer<PointMapType>* superimposer_;
    /// Given the shift, the pair_finder_ searches for corresponding elements in the two maps
    BasePairFinder< PointMapType >* pair_finder_;

    /// Computes the matching between each grid cell in the scene map and the model map
    void computeMatching_(const PointMapType& model_map, PointMapType& scene_grid_map)
    {

      pair_finder_->setElementMap(MODEL, model_map);

      if (superimposer_ !=0 )
      {
        // same model map for all superpositions
        superimposer_->setElementMap(MODEL, model_map);
      }

      if ( superimposer_ != 0 )
      {
        superimposer_->setElementMap(SCENE, scene_grid_map);
        superimposer_->run();
        grid_ = superimposer_->getTransformation(0);
        pair_finder_->setTransformation(0, superimposer_->getTransformation(0));
      }

      pair_finder_->setElementPairs(all_element_pairs_);
      pair_finder_->setElementMap(SCENE, scene_grid_map);
      pair_finder_->findElementPairs();
    }
    
  }
  ; // PoseClusteringPairwiseMapMatcher
} // namespace OpenMS

#endif  // OPENMS_ANALYSIS_MAPMATCHING_POSECLUSTERINGPAIRWISEMAPMATCHER_H
