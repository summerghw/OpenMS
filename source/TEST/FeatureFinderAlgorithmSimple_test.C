// --------------------------------------------------------------------------
//                   OpenMS -- Open-Source Mass Spectrometry               
// --------------------------------------------------------------------------
// Copyright The OpenMS Team -- Eberhard Karls University Tuebingen,
// ETH Zurich, and Freie Universitaet Berlin 2002-2013.
// 
// This software is released under a three-clause BSD license:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of any author or any participating institution 
//    may be used to endorse or promote products derived from this software 
//    without specific prior written permission.
// For a full list of authors, refer to the file AUTHORS. 
// --------------------------------------------------------------------------
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL ANY OF THE AUTHORS OR THE CONTRIBUTING 
// INSTITUTIONS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
// --------------------------------------------------------------------------
// $Maintainer: Clemens Groepl $
// $Authors: $
// --------------------------------------------------------------------------

#include <OpenMS/CONCEPT/ClassTest.h>

///////////////////////////

#include <OpenMS/TRANSFORMATIONS/FEATUREFINDER/FeatureFinderAlgorithmSimple.h>

///////////////////////////

START_TEST(FeatureFinderAlgorithmSimple, "$Id$")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace OpenMS;
using namespace OpenMS::Math;
using namespace std;

typedef FeatureFinderAlgorithmSimple<Peak1D,Feature> FFASS;

FFASS* ptr;
START_SECTION((FeatureFinderAlgorithmSimple()))
	ptr = new FFASS;
	TEST_NOT_EQUAL(ptr,0)
END_SECTION

START_SECTION((virtual ~FeatureFinderAlgorithmSimple()))
	delete ptr;
END_SECTION

//TODO: this should work (Clemens)
//START_SECTION([EXTRA] FeatureFinderAlgorithmSimple() - with RichPeak1D)
//	FeatureFinderAlgorithmSimple<RichPeak1D,Feature> ffa;
//END_SECTION

START_SECTION((virtual void run()))
	// dummy subtest
	TEST_EQUAL(1,1)
END_SECTION

START_SECTION((virtual Param getDefaultParameters() const))
{
  // dummy subtest
	TEST_EQUAL(1,1)
}
END_SECTION

START_SECTION((static FeatureFinderAlgorithm<PeakType,FeatureType>* create()))
	FeatureFinderAlgorithm<Peak1D,Feature>* ptr2 = FFASS::create();
	TEST_NOT_EQUAL(ptr2,0)
	delete ptr2;
END_SECTION

START_SECTION((static const String getProductName()))
	TEST_EQUAL(FFASS::getProductName(),"simple")
END_SECTION

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_TEST
