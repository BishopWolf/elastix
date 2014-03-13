/*======================================================================

  This file is part of the elastix software.

  Copyright (c) University Medical Center Utrecht. All rights reserved.
  See src/CopyrightElastix.txt or http://elastix.isi.uu.nl/legal.php for
  details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE. See the above copyright notices for more information.

======================================================================*/

#ifndef __elxRecursiveBSplineInterpolator_h
#define __elxRecursiveBSplineInterpolator_h

#include "itkRecursiveBSplineInterpolateImageFunctionWrapper.h"
#include "elxIncludes.h"

namespace elastix
{

  /**
   * \class RecursiveBSplineInterpolator
   * \brief An interpolator based on the itkRecursiveBSplineInterpolateImageFunction.
   *
   * This interpolator recursively interpolates images with an underlying B-spline
   * polynomial, such that it is faster than the normal BSplineInterpolator.
   *
   * The parameters used in this class are:
   * \parameter Interpolator: Select this interpolator as follows:\n
   *    <tt>(Interpolator "RecursiveBSplineInterpolator")</tt>
   * \parameter BSplineInterpolationOrder: the order of the B-spline polynomial. \n
   *    example: <tt>(BSplineInterpolationOrder 3 3 3)</tt> \n
   *    The default order is 3. The parameter can be specified for each resolution.\n
   *    If only given for one resolution, that value is used for the other resolutions as well. \n
   *    Currently only first order B-spline interpolation is supported.
   *
   * \ingroup Interpolators
   */

  template < class TElastix >
    class RecursiveBSplineInterpolator :
    public
      itk::RecursiveBSplineInterpolateImageFunctionWrapper<
        typename InterpolatorBase<TElastix>::InputImageType,
        typename InterpolatorBase<TElastix>::CoordRepType,
        double > , //CoefficientType
    public
      InterpolatorBase<TElastix>
  {
  public:

    /** Standard ITK-stuff. */
    typedef RecursiveBSplineInterpolator  Self;
    typedef itk::RecursiveBSplineInterpolateImageFunctionWrapper<
      typename InterpolatorBase<TElastix>::InputImageType,
      typename InterpolatorBase<TElastix>::CoordRepType,
      double >                                  Superclass1;
    typedef InterpolatorBase<TElastix>          Superclass2;
    typedef itk::SmartPointer<Self>             Pointer;
    typedef itk::SmartPointer<const Self>       ConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro( RecursiveBSplineInterpolator, RecursiveBSplineInterpolateImageFunctionWrapper );

    /** Name of this class.
     * Use this name in the parameter file to select this specific interpolator. \n
     * example: <tt>(Interpolator "RecursiveBSplineInterpolator")</tt>\n
     */
    elxClassNameMacro( "RecursiveBSplineInterpolator" );

    /** Get the ImageDimension. */
    itkStaticConstMacro( ImageDimension, unsigned int, Superclass1::ImageDimension );

    /** Typedefs inherited from the superclass. */
    typedef typename Superclass1::OutputType                OutputType;
    typedef typename Superclass1::InputImageType            InputImageType;
    typedef typename Superclass1::IndexType                 IndexType;
    typedef typename Superclass1::ContinuousIndexType       ContinuousIndexType;
    typedef typename Superclass1::PointType                 PointType;
    typedef typename Superclass1::Iterator                  Iterator;
    typedef typename Superclass1::CoefficientDataType       CoefficientDataType;
    typedef typename Superclass1::CoefficientImageType      CoefficientImageType;
    typedef typename Superclass1::CoefficientFilter         CoefficientFilter;
    typedef typename Superclass1::CoefficientFilterPointer  CoefficientFilterPointer;
    typedef typename Superclass1::CovariantVectorType       CovariantVectorType;

    /** Typedefs inherited from Elastix. */
    typedef typename Superclass2::ElastixType               ElastixType;
    typedef typename Superclass2::ElastixPointer            ElastixPointer;
    typedef typename Superclass2::ConfigurationType         ConfigurationType;
    typedef typename Superclass2::ConfigurationPointer      ConfigurationPointer;
    typedef typename Superclass2::RegistrationType          RegistrationType;
    typedef typename Superclass2::RegistrationPointer       RegistrationPointer;
    typedef typename Superclass2::ITKBaseType               ITKBaseType;

    /** Execute stuff before each new pyramid resolution:
     * \li Set the spline order.
     */
    virtual void BeforeEachResolution(void);

  protected:

    /** The constructor. */
    RecursiveBSplineInterpolator() {}
    /** The destructor. */
    virtual ~RecursiveBSplineInterpolator() {}

  private:

    /** The private constructor. */
    RecursiveBSplineInterpolator( const Self& ); // purposely not implemented
    /** The private copy constructor. */
    void operator=( const Self& );      // purposely not implemented

  }; // end class RecursiveBSplineInterpolator


} // end namespace elastix

#ifndef ITK_MANUAL_INSTANTIATION
#include "elxRecursiveBSplineInterpolator.hxx"
#endif

#endif // end #ifndef __elxRecursiveBSplineInterpolator_h
