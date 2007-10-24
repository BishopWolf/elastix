#ifndef __elxSimilarityTransform_H__
#define __elxSimilarityTransform_H__

#include "itkSimilarityTransform.h"
#include "itkCenteredTransformInitializer.h"
#include "elxIncludes.h"

namespace elastix
{
using namespace itk;

  /**
   * \class SimilarityTransformElastix
   * \brief A transform based on the itk SimilarityTransforms.
   *
   * This transform is a rigid body transformation, with an isotropic scaling.
   *
   * The parameters used in this class are:
   * \parameter Transform: Select this transform as follows:\n
   *    <tt>(%Transform "SimilarityTransform")</tt>
   * \parameter Scales: the scale factor between the rotations, translations,
   *    and the isotropic scaling, used in the optimizer. \n
   *    example: <tt>(Scales 200000.0)</tt> \n
   *    example: <tt>(Scales 100000.0 60000.0 ... 80000.0)</tt> \n
   *    If only one argument is given, that factor is used both for the rotations
   *    and the isotropic scaling.
   *    If more than one argument is given, then the number of arguments should be
   *    equal to the number of parameters: for each parameter its scale factor.
   *    If this parameter option is not used, by default the rotations are scaled
   *    by a factor of 100000.0.
   * \parameter CenterOfRotation: an index around which the image is rotated. \n
   *    example: <tt>(CenterOfRotation 128 128 90)</tt> \n
   *    By default the CenterOfRotation is set to the geometric center of the image.
   * \parameter AutomaticTransformInitialization: whether or not the initial translation
   *    between images should be estimated as the distance between their centers.\n
   *    example: <tt>(AutomaticTransformInitialization "true")</tt> \n
   *    By default "false" is assumed. So, no initial translation.
   *
   * The transform parameters necessary for transformix, additionally defined by this class, are:
   * \transformparameter CenterOfRotation: stores the center of rotation as an index. \n
   *    example: <tt>(CenterOfRotation 128 128 90)</tt>\n
   *    <b>depecrated!</b> From elastix version 3.402 this is changed to CenterOfRotationPoint!
   * \transformparameter CenterOfRotationPoint: stores the center of rotation, expressed in world coordinates. \n
   *    example: <tt>(CenterOfRotationPoint 10.555 6.666 12.345)</tt>
   *
   * \ingroup Transforms
   */

  template < class TElastix >
    class SimilarityTransformElastix:
      public CombinationTransform<
        ITK_TYPENAME elx::TransformBase< TElastix >::CoordRepType,
        elx::TransformBase< TElastix >::FixedImageDimension >,
      public elx::TransformBase< TElastix >
  {
  public:
    
    /** Standard ITK-stuff. */
    typedef SimilarityTransformElastix                          Self;
    typedef CombinationTransform<
      typename elx::TransformBase< TElastix >::CoordRepType,
      elx::TransformBase< TElastix >::FixedImageDimension >     Superclass1;
    typedef elx::TransformBase< TElastix >                      Superclass2;
    typedef SmartPointer<Self>                                  Pointer;
    typedef SmartPointer<const Self>                            ConstPointer;

    /** The ITK-class that provides most of the functionality, and
     * that is set as the "CurrentTransform" in the CombinationTransform */
    typedef SimilarityTransform<
      typename elx::TransformBase< TElastix >::CoordRepType,
      elx::TransformBase< TElastix >::FixedImageDimension >     SimilarityTransformType;
    
    /** Method for creation through the object factory. */
    itkNewMacro( Self );
    
    /** Run-time type information (and related methods). */
    itkTypeMacro( SimilarityTransformElastix, CombinationTransform );

    /** Name of this class.
     * Use this name in the parameter file to select this specific transform. \n
     * example: <tt>(Transform "SimilarityTransform")</tt>\n
     */
    elxClassNameMacro( "SimilarityTransform" );
    
    /** Dimension of the fixed image. */
    itkStaticConstMacro( SpaceDimension, unsigned int, Superclass2::FixedImageDimension );
    
    /** Typedefs inherited from the superclass. */

    /** These are both in Similarity2D and Similarity3D. */
    typedef typename Superclass1::ScalarType                  ScalarType;
    typedef typename Superclass1::ParametersType              ParametersType;
    typedef typename Superclass1::JacobianType                JacobianType;
    
    typedef typename Superclass1::InputPointType              InputPointType;
    typedef typename Superclass1::OutputPointType             OutputPointType;
    typedef typename Superclass1::InputVectorType             InputVectorType;
    typedef typename Superclass1::OutputVectorType            OutputVectorType;
    typedef typename Superclass1::InputCovariantVectorType    InputCovariantVectorType;
    typedef typename Superclass1::OutputCovariantVectorType   OutputCovariantVectorType;
    typedef typename Superclass1::InputVnlVectorType          InputVnlVectorType;
    typedef typename Superclass1::OutputVnlVectorType         OutputVnlVectorType;
    
    /** NOTE: use this one only in 3D (otherwise it's just an int). */
    typedef typename SimilarityTransformType::AngleType       AngleType;
    typedef typename SimilarityTransformType::Pointer         SimilarityTransformPointer;
    typedef typename SimilarityTransformType::OffsetType      OffsetType;
    
    /** Typedef's inherited from TransformBase. */
    typedef typename Superclass2::ElastixType               ElastixType;
    typedef typename Superclass2::ElastixPointer            ElastixPointer;
    typedef typename Superclass2::ConfigurationType         ConfigurationType;
    typedef typename Superclass2::ConfigurationPointer      ConfigurationPointer;
    typedef typename Superclass2::RegistrationType          RegistrationType;
    typedef typename Superclass2::RegistrationPointer       RegistrationPointer;
    typedef typename Superclass2::CoordRepType              CoordRepType;
    typedef typename Superclass2::FixedImageType            FixedImageType;
    typedef typename Superclass2::MovingImageType           MovingImageType;
    typedef typename Superclass2::ITKBaseType               ITKBaseType;
    typedef typename Superclass2::CombinationTransformType  CombinationTransformType;
    
    /** Other typedef's. */
    typedef typename RegistrationType::ITKBaseType          ITKRegistrationType;
    typedef typename ITKRegistrationType::OptimizerType     OptimizerType;
    typedef typename OptimizerType::ScalesType              ScalesType;

    typedef typename FixedImageType::IndexType              IndexType;
    typedef typename IndexType::IndexValueType              IndexValueType;
    typedef typename FixedImageType::SizeType               SizeType;
    typedef typename FixedImageType::PointType              PointType;
    typedef typename FixedImageType::SpacingType            SpacingType;
    typedef typename FixedImageType::RegionType             RegionType;

    typedef CenteredTransformInitializer<
      SimilarityTransformType, FixedImageType, MovingImageType> TransformInitializerType;
    typedef typename TransformInitializerType::Pointer      TransformInitializerPointer;
    
    /** Execute stuff before the actual registration:
     * \li Call InitializeTransform
     * \li Set the scales.
     */
    virtual void BeforeRegistration( void );

    /** Initialize Transform.
     * \li Set all parameters to zero. 
     * \li Set center of rotation:
     *  automatically initialized to the geometric center of the image, or
     *   assigned a user entered voxel index, given by the parameter 
     *   (CenterOfRotation <index-x> <index-y> ...); 
     *   If an initial transform is present and HowToCombineTransforms is
     *   set to "Compose", the initial transform is taken into account 
     *   while setting the center of rotation.
     * \li Set initial translation:
     *  the initial translation between fixed and moving image is guessed,
     *  if the user has set (AutomaticTransformInitialization "true"). 
     * 
     * It is not yet possible to enter an initial rotation angle.
     */
    virtual void InitializeTransform( void );

    /** Function to read transform-parameters from a file. 
     * 
     * It reads the center of rotation and calls the superclass' implementation.
     */
    virtual void ReadFromFile( void );

    /** Function to write transform-parameters to a file. 
     *
     * It writes the center of rotation to file and calls the superclass' implementation.
     */
    virtual void WriteToFile( const ParametersType & param );

  protected:

    /** The constructor. */
    SimilarityTransformElastix();
    /** The destructor. */
    virtual ~SimilarityTransformElastix() {};

    /** Try to read the CenterOfRotation from the transform parameter file
     * This is an index value, and, thus, converted to world coordinates.
     * Transform parameter files generated by elastix version < 3.402
     * saved the center of rotation in this way.
     */
    virtual bool ReadCenterOfRotationIndex( InputPointType & rotationPoint );

    /** Try to read the CenterOfRotationPoint from the transform parameter file 
     * The CenterOfRotationPoint is already in world coordinates.
     * Transform parameter files generated by elastix version > 3.402
     * save the center of rotation in this way.
     */
    virtual bool ReadCenterOfRotationPoint( InputPointType & rotationPoint );
    
  private:

    /** The private constructor. */
    SimilarityTransformElastix( const Self& );  // purposely not implemented
    /** The private copy constructor. */
    void operator=( const Self& );        // purposely not implemented

    SimilarityTransformPointer        m_SimilarityTransform;
    
  }; // end class SimilarityTransformElastix
  
  
} // end namespace elastix

#ifndef ITK_MANUAL_INSTANTIATION
#include "elxSimilarityTransform.hxx"
#endif

#endif // end #ifndef __elxSimilarityTransform_H__
