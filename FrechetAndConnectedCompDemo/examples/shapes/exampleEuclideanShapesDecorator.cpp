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
 * @file exampleEuclideanShapesDecorator.cpp
 * @ingroup Examples
 * @author Jérémy Levallois (\c jeremy.levallois@liris.cnrs.fr )
 * Laboratoire d'InfoRmatique en Image et Systèmes d'information - LIRIS (CNRS, UMR 5205), INSA-Lyon, France
 * LAboratoire de MAthématiques - LAMA (CNRS, UMR 5127), Université de Savoie, France
 *
 * @date 2012/12/17
 *
 * An example file named exampleEuclideanShapesDecorator.
 *
 * This file is part of the DGtal library.
 */

//! [EuclideanShapesDecoratorUsageFull]

///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "DGtal/base/Common.h"

// Shape construction
#include "DGtal/shapes/GaussDigitizer.h"
#include "DGtal/topology/LightImplicitDigitalSurface.h"
#include "DGtal/topology/DigitalSurface.h"
#include "DGtal/io/boards/Board2D.h"
#include "DGtal/images/ImageHelper.h"
#include "DGtal/shapes/Shapes.h"
#include "DGtal/shapes/parametric/Ball2D.h"
#include "DGtal/shapes/EuclideanShapesDecorator.h"

// Drawing
#include "DGtal/io/boards/Board2D.h"

///////////////////////////////////////////////////////////////////////////////

using namespace std;
using namespace DGtal;

///////////////////////////////////////////////////////////////////////////////

int main( int argc, char** argv )
{
    trace.beginBlock ( "Example EuclideanShapesDecorator" );
    trace.info() << "Args:";
    for ( int i = 0; i < argc; ++i )
        trace.info() << " " << argv[ i ];
    trace.info() << endl;

    /// Construction of the shape + digitalization
    double h = 1.0;

    //! [EuclideanShapesDecoratorUsage]
    typedef Ball2D< Z2i::Space > MyEuclideanShapeA;
    typedef Ball2D< Z2i::Space > MyEuclideanShapeB;
    MyEuclideanShapeA shapeA(0.0, 0.0, 14.0000123);
    MyEuclideanShapeB shapeB(1.0, 0.0, 14.0000123);

    typedef EuclideanShapesMinus< MyEuclideanShapeA, MyEuclideanShapeB > Minus;
    Minus s_minus ( shapeA, shapeB );
    //! [EuclideanShapesDecoratorUsage]

    typedef Z2i::KSpace::Surfel Surfel;

    typedef GaussDigitizer< Z2i::Space, MyEuclideanShapeA > MyGaussDigitizerShapeA;
    typedef LightImplicitDigitalSurface< Z2i::KSpace, MyGaussDigitizerShapeA > LightImplDigSurfaceA;
    typedef DigitalSurface< LightImplDigSurfaceA > MyDigitalSurfaceA;
    MyGaussDigitizerShapeA digShapeA;
    digShapeA.attach( shapeA );
    digShapeA.init( shapeA.getLowerBound(), shapeA.getUpperBound(), h );
    Z2i::Domain domainShapeA = digShapeA.getDomain();

    DigitalSetSelector< Z2i::Domain, BIG_DS + HIGH_ITER_DS + HIGH_BEL_DS >::Type aSetA( domainShapeA );
    Shapes<Z2i::Domain>::digitalShaper( aSetA, digShapeA );



    typedef GaussDigitizer< Z2i::Space, MyEuclideanShapeA > MyGaussDigitizerShapeB;
    typedef LightImplicitDigitalSurface< Z2i::KSpace, MyGaussDigitizerShapeB > LightImplDigSurfaceB;
    typedef DigitalSurface< LightImplDigSurfaceB > MyDigitalSurfaceB;
    MyGaussDigitizerShapeB digShapeB;
    digShapeB.attach( shapeB );
    digShapeB.init( shapeB.getLowerBound(), shapeB.getUpperBound(), h );
    Z2i::Domain domainShapeB = digShapeB.getDomain();

    DigitalSetSelector< Z2i::Domain, BIG_DS + HIGH_ITER_DS + HIGH_BEL_DS >::Type aSetB( domainShapeB );
    Shapes<Z2i::Domain>::digitalShaper( aSetB, digShapeB );



    typedef GaussDigitizer< Z2i::Space, Minus > MyGaussDigitizer;
    typedef LightImplicitDigitalSurface< Z2i::KSpace, MyGaussDigitizer > LightImplicitDigSurface;
    typedef DigitalSurface< LightImplicitDigSurface > MyDigitalSurface;
    MyGaussDigitizer digShape;
    digShape.attach( s_minus );
    digShape.init( s_minus.getLowerBound(), s_minus.getUpperBound(), h );
    Z2i::Domain domainShape = digShape.getDomain();
    DigitalSetSelector< Z2i::Domain, BIG_DS + HIGH_ITER_DS + HIGH_BEL_DS >::Type aSet( domainShape );
    Shapes<Z2i::Domain>::digitalShaper( aSet, digShape );



    Board2D board;
    board << SetMode( domainShape.className(), "Paving" )
          << domainShape;

    Color dgreen( 0, 192, 0, 50 );
    Color dred( 192, 0, 0, 50 );
    Color dorange( 255, 136, 0, 220 );

    board << CustomStyle( aSetA.className(), new CustomFillColor( dgreen ) );
    board << aSetA;

    board << CustomStyle( aSetB.className(), new CustomFillColor( dred ) );
    board << aSetB;

    board << CustomStyle( aSet.className(), new CustomFillColor( dorange ) );
    board << aSet;

    board.saveSVG ( "example-EuclideanShapesDecorator.svg" );

    trace.endBlock();
    return 0;
}

//! [EuclideanShapesDecoratorUsageFull]
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
