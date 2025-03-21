/**
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 **/

/**
 * @file exampleCurvature.cpp
 * @ingroup Examples
 * @author Tristan Roussillon (\c tristan.roussillon@liris.cnrs.fr )
 * Laboratoire d'InfoRmatique en Image et Systèmes d'information - LIRIS (CNRS, UMR 5205), CNRS, France
 *
 * @date 2012/06/26
 *
 * An example file named exampleCurvature.
 *
 * This file is part of the DGtal library.
 */

///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "DGtal/base/Common.h"

#include "DGtal/kernel/SpaceND.h"
#include "DGtal/kernel/domains/HyperRectDomain.h"
#include "DGtal/topology/KhalimskySpaceND.h"
#include "DGtal/topology/SurfelAdjacency.h"
#include "DGtal/topology/SurfelNeighborhood.h"

#include "DGtal/shapes/Shapes.h"
#include "DGtal/shapes/ShapeFactory.h"
#include "DGtal/shapes/GaussDigitizer.h"
#include "DGtal/geometry/curves/GridCurve.h"

#include "DGtal/geometry/curves/estimation/MostCenteredMaximalSegmentEstimator.h"
#include "DGtal/geometry/curves/GeometricalDCA.h"

///////////////////////////////////////////////////////////////////////////////

using namespace std;
using namespace DGtal;

// #include <boost/program_options/options_description.hpp>
// #include <boost/program_options/parsers.hpp>
// #include <boost/program_options/variables_map.hpp>

//namespace po = boost::program_options;

///////////////////////////////////////////////////////////////////////////////
template <typename Shape, typename RealPoint>
bool 
estimatorOnShapeDigitization( const string& name,
			      Shape & aShape, 
			      const RealPoint& low, const RealPoint& up, 
			      double h )
{
  using namespace Z2i;

  trace.beginBlock ( ( "Curvature estimation on digitization of "
		       + name ). c_str() );
  
  // Creates a digitizer on the window (low, up).
  GaussDigitizer<Space,Shape> dig;  
  dig.attach( aShape ); // attaches the shape.
  dig.init( low, up, h ); 
  
  // The domain size is given by the digitizer 
  // according to the window and the step.
  Domain domain = dig.getDomain();

  // Create cellular space
  KSpace K;
  bool ok = K.init( dig.getLowerBound(), dig.getUpperBound(), true );
  if ( ! ok )
    {
      std::cerr << "[estimatorOnShapeDigitization]"
		<< " error in creating KSpace." << std::endl;
    }
  else
    try {
      // Extracts shape boundary
      SurfelAdjacency<KSpace::dimension> SAdj( true );
      SCell bel = Surfaces<KSpace>::findABel( K, dig, 10000 );
      // Getting the consecutive surfels of the 2D boundary
      std::vector<Point> points;
      Surfaces<KSpace>::track2DBoundaryPoints( points, K, SAdj, dig, bel );
      // Create GridCurve
      GridCurve<KSpace> gridcurve( K );
      gridcurve.initFromVector( points );
      // Create range of incident points
      typedef GridCurve<KSpace>::IncidentPointsRange Range;
      typedef Range::ConstIterator ClassicIterator;
      typedef Range::ConstCirculator CircularIterator;
      Range r = gridcurve.getIncidentPointsRange();//building range
      // Estimation
      std::vector<double> estimations; 
      if (gridcurve.isOpen())
        { 
	  typedef GeometricalDCA<ClassicIterator> SegmentComputer;
	  typedef CurvatureFromDCAEstimator<SegmentComputer> SCEstimator;
	  typedef MostCenteredMaximalSegmentEstimator<SegmentComputer,SCEstimator> CurvatureEstimator;
	  SegmentComputer sc;
	  SCEstimator sce; 
	  CurvatureEstimator estimator(sc, sce);
	  std::cout << "# open grid curve" << endl;
          estimator.init( h, r.begin(), r.end() );
          estimator.eval( r.begin(), r.end(), std::back_inserter(estimations) ); 
        }
      else
        { 
	  typedef GeometricalDCA<CircularIterator> SegmentComputer;
	  typedef CurvatureFromDCAEstimator<SegmentComputer> SCEstimator;
	  typedef MostCenteredMaximalSegmentEstimator<SegmentComputer,SCEstimator> CurvatureEstimator;
	  SegmentComputer sc;
	  SCEstimator sce;
	  CurvatureEstimator estimator(sc, sce);
	  std::cout << "# closed grid curve" << endl;
          estimator.init( h, r.c(), r.c() );
          estimator.eval( r.c(), r.c(), std::back_inserter(estimations) ); 
        }
      // Print (standard output)
      std::cout << "# idx kappa" << endl;
      unsigned int i = 0;
      for ( ClassicIterator it = r.begin(), ite = r.end();
	    it != ite; ++it, ++i )
	{
	  std::cout << i << " " << estimations.at(i) << std::endl;
	}
    }    
    catch ( InputException e )
      {
	std::cerr << "[estimatorOnShapeDigitization]"
		  << " error in finding a bel." << std::endl;
	ok = false;
      }
  trace.emphase() << ( ok ? "Passed." : "Error." ) << endl;
  trace.endBlock();
  return ok;
}

///////////////////////////////////////////////////////////////////////////////
int main( int argc, char** argv )
{
  trace.beginBlock ( "Example exampleCurvature" );

  // parse command line 
  // po::options_description general_opt("Allowed options are");
  // general_opt.add_options()
  //   ("help,h", "display this message")
  //   ("shape,s",  po::value<string>()->default_value("flower"), "Shape to digitize: flower, ellipse, ball" )
  //   ("gridStep,g",  po::value<double>()->default_value(0.01), "Grid step" ); 
  
  // po::variables_map vm;
  // po::store(po::parse_command_line(argc, argv, general_opt), vm);  
  // po::notify(vm);    
  // trace.info()<< "curvature estimation" << std::endl
  // 	      << "Basic usage: "<<std::endl
  // 	      << argv[0] << " [other options] -i <vol file> -t <threshold> " << std::endl
  // 	      << general_opt << "\n";
  
  
// grid step
  double h = 0.01;//vm["gridStep"].as<double>(); 
  // shape
  string shapeName ="flower";// vm["shape"].as<string>(); 

  
  // parse shape
  bool res = true; 
  typedef Z2i::Space Space; 
  typedef Space::RealPoint RealPoint; 
  if (shapeName == "flower")
    {
      Flower2D<Space> flower( 0.5, 0.5, 5.0, 3.0, 5, 0.3 );
      res = estimatorOnShapeDigitization("flower", flower, 
					 RealPoint::diagonal(-10), 
					 RealPoint::diagonal(10), 
					 h); 
    }
  else if (shapeName == "ellipse")
    {
      Ellipse2D<Space> ellipse( 0.5, 0.5, 5.0, 3.0, 0.3 );
      res = estimatorOnShapeDigitization("ellipse", ellipse, 
					 RealPoint::diagonal(-10), 
					 RealPoint::diagonal(10), 
					 h); 
    }
  else if (shapeName == "ball")
    {
      Ball2D<Space> ball( 0.5, 0.5, 5.0 );
      res = estimatorOnShapeDigitization("ball", ball, 
					 RealPoint::diagonal(-10), 
					 RealPoint::diagonal(10), 
					 h); 
    }
  else 
    {
      trace.error() << "Unknown shape. Use option -h" << std::endl;
      res = false; 
    }

  trace.endBlock();

  return res;
}
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
