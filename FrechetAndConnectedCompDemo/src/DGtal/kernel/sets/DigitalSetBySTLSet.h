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

#pragma once

/**
 * @file DigitalSetBySTLSet.h
 * @author Jacques-Olivier Lachaud (\c jacques-olivier.lachaud@univ-savoie.fr )
 * Laboratory of Mathematics (CNRS, UMR 5807), University of Savoie, France
 *
 * @author Sebastien Fourey (\c Sebastien.Fourey@greyc.ensicaen.fr )
 * Groupe de Recherche en Informatique, Image, Automatique et
 * Instrumentation de Caen - GREYC (CNRS, UMR 6072), ENSICAEN, France
 *
 * @date 2010/07/01
 *
 * Header file for module DigitalSetBySTLSet.cpp
 *
 * This file is part of the DGtal library.
 */

#if defined(DigitalSetBySTLSet_RECURSES)
#error Recursive header files inclusion detected in DigitalSetBySTLSet.h
#else // defined(DigitalSetBySTLSet_RECURSES)
/** Prevents recursive inclusion of headers. */
#define DigitalSetBySTLSet_RECURSES

#if !defined DigitalSetBySTLSet_h
/** Prevents repeated inclusion of headers. */
#define DigitalSetBySTLSet_h

//////////////////////////////////////////////////////////////////////////////
// Inclusions
#include <iostream>
#include <set>
#include <string>
#include "DGtal/base/Common.h"
#include "DGtal/kernel/domains/CDomain.h"
//////////////////////////////////////////////////////////////////////////////

//#include "DGtal/io/Display3D.h"


namespace DGtal
{

  /////////////////////////////////////////////////////////////////////////////
  // template class DigitalSetBySTLSet
  /**
    Description of template class 'DigitalSetBySTLSet' <p>

    \brief Aim: A container class for storing sets of digital points
    within some given domain.

    This is the most versatile implementation for a set of point, and
    is essentially a wrapper to std::set<Point>. It added the notion
    of domain.

    Model of CDigitalSet.
   */
  template <typename TDomain>
  class DigitalSetBySTLSet
  {
  public:
 
    ///Concept checks
    BOOST_CONCEPT_ASSERT(( CDomain< TDomain > ));
    
    typedef TDomain Domain;
    typedef DigitalSetBySTLSet<Domain> Self;
    typedef typename Domain::Space Space;
    typedef typename Domain::Point Point;
    typedef typename Domain::Size Size;
    typedef typename std::set<Point>::iterator Iterator;
    typedef typename std::set<Point>::const_iterator ConstIterator;

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor.
     */
    ~DigitalSetBySTLSet();

    /**
     * Constructor.
     * Creates the empty set in the domain [d].
     *
     * @param d any domain.
     */
    DigitalSetBySTLSet( const Domain & d );

    /**
     * Copy constructor.
     * @param other the object to clone.
     */
    DigitalSetBySTLSet ( const DigitalSetBySTLSet & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     */
    DigitalSetBySTLSet & operator= ( const DigitalSetBySTLSet & other );

    /**
     * @return the embedding domain.
     */
    const Domain & domain() const;

    // ----------------------- Standard Set services --------------------------
  public:

    /**
     * @return the number of elements in the set.
     */
    Size size() const;

    /**
     * @return 'true' iff the set is empty (no element).
     */
    bool empty() const;
     
    /**
     * Adds point [p] to this set.
     *
     * @param p any digital point.
     * @pre p should belong to the associated domain.
     */
    void insert( const Point & p );

    /**
     * Adds the collection of points specified by the two iterators to
     * this set.
     *
     * @param first the start point in the collection of Point.
     * @param last the last point in the collection of Point.
     * @pre all points should belong to the associated domain.
     */
    template <typename PointInputIterator>
    void insert( PointInputIterator first, PointInputIterator last );

    /**
     * Adds point [p] to this set if the point is not already in the
     * set.
     *
     * @param p any digital point.
     *
     * @pre p should belong to the associated domain.
     * @pre p should not belong to this.
     */
    void insertNew( const Point & p );

    /**
     * Adds the collection of points specified by the two iterators to
     * this set.
     *
     * @param first the start point in the collection of Point.
     * @param last the last point in the collection of Point.
     *
     * @pre all points should belong to the associated domain.
     * @pre each point should not belong to this.
     */
    template <typename PointInputIterator>
    void insertNew( PointInputIterator first, PointInputIterator last );

    /**
     * Removes point [p] from the set.
     * 
     * @param p the point to remove.
     * @return the number of removed elements (0 or 1).
     */
    Size erase( const Point & p );

    /**
     * Removes the point pointed by [it] from the set.
     * 
     * @param it an iterator on this set.
     * Note: generally faster than giving just the point.
     */
    void erase( Iterator it );

    /**
     * Removes the collection of points specified by the two iterators from
     * this set.
     *
     * @param first the start point in this set.
     * @param last the last point in this set.
     */
    void erase( Iterator first, Iterator last );

    /**
     * Clears the set.
     * @post this set is empty.
     */
    void clear();

    /**
     * @param p any digital point.
     */
    ConstIterator find( const Point & p ) const;

    /**
     * @param p any digital point.
     * @return an iterator pointing on [p] if found, otherwise end().
     */
    Iterator find( const Point & p );

    /**
     * @return a const iterator on the first element in this set.
     */
    ConstIterator begin() const;

    /**
     * @return a const iterator on the element after the last in this set.
     */
    ConstIterator end() const;

    /**
     * @return an iterator on the first element in this set.
     */
    Iterator begin();

    /**
     * @return a iterator on the element after the last in this set.
     */
    Iterator end();

    /**
     * set union to left.
     * @param aSet any other set.
     */
    DigitalSetBySTLSet<Domain> & operator+=
    ( const DigitalSetBySTLSet<Domain> & aSet );

    // ----------------------- Model of CPointPredicate -----------------------------
  public:

    /**
       @param p any point.
       @return 'true' if and only if \a p belongs to this set.
    */
    bool operator()( const Point & p ) const;

    // ----------------------- Other Set services -----------------------------
  public:
    
    /**
     * Computes the complement in the domain of this set
     * @param ito an output iterator
     * @tparam TOutputIterator a model of output iterator
     */
   template< typename TOutputIterator >
    void computeComplement(TOutputIterator& ito) const; 

    /**
     * Builds the complement in the domain of the set [other_set] in
     * this.
     *
     * @param other_set defines the set whose complement is assigned to 'this'.
     */
    void assignFromComplement( const DigitalSetBySTLSet<Domain> & other_set ); 
    
    /**
     * Computes the bounding box of this set.
     *
     * @param lower the first point of the bounding box (lowest in all
     * directions).
     * @param upper the last point of the bounding box (highest in all
     * directions).
     */
    void computeBoundingBox( Point & lower, Point & upper ) const;


    // ----------------------- Interface --------------------------------------
  public:

    /**
     * Writes/Displays the object on an output stream.
     * @param out the output stream where the object is written.
     */
    void selfDisplay ( std::ostream & out ) const;

    /**
     * Checks the validity/consistency of the object.
     * @return 'true' if the object is valid, 'false' otherwise.
     */
    bool isValid() const;

    // ------------------------- Protected Datas ------------------------------
  protected:

    /**
     * The associated domain;
     */
    const Domain & myDomain;

    /**
     * The container storing the points of the set.
     */
    std::set<Point> mySet;


  public:
    


    // --------------- CDrawableWithBoard2D realization ---------------------
  public:

    /**
     * Default drawing style object.
     * @return the dyn. alloc. default style for this object. 
     */
    //DrawableWithBoard2D* defaultStyle( std::string mode = "" ) const;

    /**
     * @return the style name used for drawing this object.
     */
    std::string className() const;


    // ------------------------- Hidden services ------------------------------
  protected:

    /**
     * Default Constructor.
     * Forbidden since a Domain is necessary for defining a set.
     */
    DigitalSetBySTLSet();

  private:


    // ------------------------- Internals ------------------------------------
  private:


  }; // end of class DigitalSetBySTLSet


  /**
   * Overloads 'operator<<' for displaying objects of class 'DigitalSetBySTLSet'.
   * @param out the output stream where the object is written.
   * @param object the object of class 'DigitalSetBySTLSet' to write.
   * @return the output stream after the writing.
   */
  template <typename Domain>
  std::ostream&
  operator<< ( std::ostream & out, const DigitalSetBySTLSet<Domain> & object );

} // namespace DGtal


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions.
#include "DGtal/kernel/sets/DigitalSetBySTLSet.ih"

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined DigitalSetBySTLSet_h

#undef DigitalSetBySTLSet_RECURSES
#endif // else defined(DigitalSetBySTLSet_RECURSES)
