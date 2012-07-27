/*======================================================================

  This file is part of the elastix software.

  Copyright (c) University Medical Center Utrecht. All rights reserved.
  See src/CopyrightElastix.txt or http://elastix.isi.uu.nl/legal.php for
  details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE. See the above copyright notices for more information.

======================================================================*/

// OpenCL implementation of itk::LinearInterpolateImageFunction

//------------------------------------------------------------------------------
float bspline_evaluate_at_continuous_index_1d(const float index,
                                              __global const INPIXELTYPE* in,
                                              __constant GPUImageBase1D *in_image,
                                              __constant GPUImageFunction1D* image_function,
                                              __global const INTERPOLATOR_PRECISION_TYPE* coefficients,
                                              __constant GPUImageBase1D *coefficients_image)
{
  return 0.0;
}

//------------------------------------------------------------------------------
float bspline_evaluate_at_continuous_index_2d(const float2 index,
                                              __global const INPIXELTYPE* in,
                                              __constant GPUImageBase2D *in_image,
                                              __constant GPUImageFunction3D* image_function,
                                              __global const INTERPOLATOR_PRECISION_TYPE* coefficients,
                                              __constant GPUImageBase2D *coefficients_image)
{
  return 0.0;
}

//------------------------------------------------------------------------------
float bspline_evaluate_at_continuous_index_3d(const float3 index,
                                              __global const INPIXELTYPE* in,
                                              __constant GPUImageBase3D *in_image,
                                              __constant GPUImageFunction3D* image_function,
                                              __global const INTERPOLATOR_PRECISION_TYPE* coefficients,
                                              __constant GPUImageBase3D *coefficients_image)
{
  return 0.0;
}

#ifdef DIM_1
//------------------------------------------------------------------------------
float evaluate_at_continuous_index_1d(const float index,
                                      __global const INPIXELTYPE* in,
                                      __constant GPUImageBase1D *image,
                                      __constant GPUImageFunction1D* image_function)
{
  long basei = (long)(floor(index));
  if(basei < image_function->StartIndex)
  {
    basei = image_function->StartIndex;
  }

  const float distance = index - (float)(basei);
  const float val0 = get_pixel_1d(basei, in, image);

  if(distance <= 0.)
  {
    return val0;
  }
  basei = basei + 1;
  if(basei > image_function->EndIndex)
  {
    return val0;
  }
  const float val1 = get_pixel_1d(basei, in, image);

  return (val0 + ( val1 - val0 ) * distance);
}
#endif // DIM_1

#ifdef DIM_2
//------------------------------------------------------------------------------
float evaluate_at_continuous_index_2d(const float2 index,
                                      __global const INPIXELTYPE* in,
                                      __constant GPUImageBase2D *image,
                                      __constant GPUImageFunction2D* image_function)
{
  long2 basei = (long2)((long)(floor(index.x)), (long)(floor(index.y)));
  if(basei.x < image_function->StartIndex.x)
  {
    basei.x = image_function->StartIndex.x;
  }
  const float distance0 = index.x - (float)(basei.x);

  if(basei.y < image_function->StartIndex.y)
  {
    basei.y = image_function->StartIndex.y;
  }
  const float distance1 = index.y - (float)(basei.y);
  const float val00 = get_pixel_2d(basei, in, image);

  if(distance0 <= 0. && distance1 <= 0.)
  {
    return val00;
  }
  else if(distance1 <= 0.) // if they have the same "y"
  {
    basei.x = basei.x + 1; // then interpolate across "x"
    if(basei.x > image_function->EndIndex.x)
    {
      return val00;
    }
    const float val10 = get_pixel_2d(basei, in, image);
    return (val00 + ( val10 - val00 ) * distance0);
  }
  else if(distance0 <= 0.) // if they have the same "x"
  {
    basei.y = basei.y + 1; // then interpolate across "y"
    if(basei.y > image_function->EndIndex.y)
    {
      return val00;
    }
    const float val01 = get_pixel_2d(basei, in, image);
    return (val00 + ( val01 - val00 ) * distance1);
  }
  // fall-through case:
  // interpolate across "xy"
  basei.x = basei.x + 1;
  if(basei.x > image_function->EndIndex.x) // interpolate across "y"
  {
    basei.x = basei.x - 1;
    basei.y = basei.y + 1;
    if(basei.y > image_function->EndIndex.y)
    {
      return val00;
    }
    const float val01 = get_pixel_2d(basei, in, image);
    return (val00 + ( val01 - val00 ) * distance1);
  }
  const float val10 = get_pixel_2d(basei, in, image);
  const float valx0 = val00 + ( val10 - val00 ) * distance0;

  basei.y = basei.y + 1;
  if(basei.y > image_function->EndIndex.y) // interpolate across "x"
  {
    return valx0;
  }
  const float val11 = get_pixel_2d(basei, in, image);
  basei.x = basei.x - 1;
  const float val01 = get_pixel_2d(basei, in, image);

  const float valx1 = val01 + ( val11 - val01 ) * distance0;

  return (valx0 + ( valx1 - valx0 ) * distance1);
}
#endif // DIM_2

#ifdef DIM_3
//------------------------------------------------------------------------------
float evaluate_at_continuous_index_3d(const float3 index,
                                      __global const INPIXELTYPE* in,
                                      __constant GPUImageBase3D *image,
                                      __constant GPUImageFunction3D* image_function)
{
  long3 basei = (long3)((long)(floor(index.x)), (long)(floor(index.y)), (long)(floor(index.z)));
  if(basei.x < image_function->StartIndex.x)
  {
    basei.x = image_function->StartIndex.x;
  }
  const float distance0 = index.x - (float)(basei.x);

  if(basei.y < image_function->StartIndex.y)
  {
    basei.y = image_function->StartIndex.y;
  }
  const float distance1 = index.y - (float)(basei.y);

  if(basei.z < image_function->StartIndex.z)
  {
    basei.z = image_function->StartIndex.z;
  }
  const float distance2 = index.z - (float)(basei.z);

  if(distance0 <= 0. && distance1 <= 0. && distance2 <= 0.)
  {
    return ( ( get_pixel_3d(basei, in, image) ) );
  }

  const float val000 = get_pixel_3d(basei, in, image);

  if(distance2 <= 0.)
  {
    if(distance1 <= 0.) // interpolate across "x"
    {
      basei.x = basei.x + 1;
      if(basei.x > image_function->EndIndex.x)
      {
        return val000;
      }
      const float val100 = get_pixel_3d(basei, in, image);

      return (val000 + ( val100 - val000 ) * distance0);
    }
    else if(distance0 <= 0.) // interpolate across "y"
    {
      basei.y = basei.y + 1;
      if(basei.y > image_function->EndIndex.y)
      {
        return val000;
      }
      const float val010 = get_pixel_3d(basei, in, image);

      return (val000 + ( val010 - val000 ) * distance1);
    }
    else  // interpolate across "xy"
    {
      basei.x = basei.x + 1;
      if(basei.x > image_function->EndIndex.x) // interpolate across "y"
      {
        basei.x = basei.x - 1;
        basei.y = basei.y + 1;
        if(basei.y > image_function->EndIndex.y)
        {
          return val000;
        }
        const float val010 = get_pixel_3d(basei, in, image);

        return (val000 + ( val010 - val000 ) * distance1);
      }
      const float val100 = get_pixel_3d(basei, in, image);
      const float valx00 = val000 + ( val100 - val000 ) * distance0;
      basei.y = basei.y + 1;
      if(basei.y > image_function->EndIndex.y) // interpolate across "x"
      {
        return valx00;
      }
      const float val110 = get_pixel_3d(basei, in, image);
      basei.x = basei.x - 1;
      const float val010 = get_pixel_3d(basei, in, image);
      const float valx10 = val010 + ( val110 - val010 ) * distance0;

      return (valx00 + ( valx10 - valx00 ) * distance1);
    }
  }
  else
  {
    if(distance1 <= 0.)
    {
      if(distance0 <= 0.) // interpolate across "z"
      {
        basei.z = basei.z + 1;
        if(basei.z > image_function->EndIndex.z)
        {
          return val000;
        }
        const float val001 = get_pixel_3d(basei, in, image);

        return (val000 + ( val001 - val000 ) * distance2);
      }
      else // interpolate across "xz"
      {
        basei.x = basei.x + 1;
        if(basei.x > image_function->EndIndex.x) // interpolate across "z"
        {
          basei.x = basei.x - 1;
          basei.z = basei.z + 1;
          if(basei.z > image_function->EndIndex.z)
          {
            return val000;
          }
          const float val001 = get_pixel_3d(basei, in, image);

          return (val000 + ( val001 - val000 ) * distance2);
        }
        const float val100 = get_pixel_3d(basei, in, image);
        const float valx00 = val000 + ( val100 - val000 ) * distance0;
        basei.z = basei.z + 1;
        if(basei.z > image_function->EndIndex.z) // interpolate across "x"
        {
          return valx00;
        }
        const float val101 = get_pixel_3d(basei, in, image);
        basei.x = basei.x - 1;
        const float val001 = get_pixel_3d(basei, in, image);
        const float valx01 = val001 + ( val101 - val001 ) * distance0;

        return (valx00 + ( valx01 - valx00 ) * distance2);
      }
    }
    else if(distance0 <= 0.) // interpolate across "yz"
    {
      basei.y = basei.y + 1;
      if(basei.y > image_function->EndIndex.y) // interpolate across "z"
      {
        basei.y = basei.y - 1;
        basei.z = basei.z + 1;
        if(basei.z > image_function->EndIndex.z)
        {
          return ( ( val000 ) );
        }
        const float val001 = get_pixel_3d(basei, in, image);

        return (val000 + ( val001 - val000 ) * distance2);
      }
      const float val010 = get_pixel_3d(basei, in, image);
      const float val0x0 = val000 + ( val010 - val000 ) * distance1;

      basei.z = basei.z + 1;
      if(basei.z > image_function->EndIndex.z) // interpolate across "y"
      {
        return val0x0;
      }
      const float val011 = get_pixel_3d(basei, in, image);
      basei.y = basei.y - 1;
      const float val001 = get_pixel_3d(basei, in, image);
      const float val0x1 = val001 + ( val011 - val001 ) * distance1;

      return (val0x0 + ( val0x1 - val0x0 ) * distance2);
    }
    else // interpolate across "xyz"
    {
      basei.x = basei.x + 1;
      if(basei.x > image_function->EndIndex.x) // interpolate across "yz"
      {
        basei.x = basei.x - 1;
        basei.y = basei.y + 1;
        if(basei.y > image_function->EndIndex.y)  // interpolate across "z"
        {
          basei.y = basei.y - 1;
          basei.z = basei.z + 1;
          if(basei.z > image_function->EndIndex.z)
          {
            return val000;
          }
          const float val001 = get_pixel_3d(basei, in, image);

          return (val000 + ( val001 - val000 ) * distance2);
        }
        const float val010 = get_pixel_3d(basei, in, image);
        const float val0x0 = val000 + ( val010 - val000 ) * distance1;
        basei.z = basei.z + 1;
        if(basei.z > image_function->EndIndex.z) // interpolate across "y"
        {
          return ( ( val0x0 ) );
        }
        const float val011 = get_pixel_3d(basei, in, image);
        basei.y = basei.y - 1;
        const float val001 = get_pixel_3d(basei, in, image);
        const float val0x1 = val001 + ( val011 - val001 ) * distance1;

        return (val0x0 + ( val0x1 - val0x0 ) * distance2);
      }
      const float val100 = get_pixel_3d(basei, in, image);
      const float valx00 = val000 + ( val100 - val000 ) * distance0;
      basei.y = basei.y + 1;
      if(basei.y > image_function->EndIndex.y) // interpolate across "xz"
      {
        basei.y = basei.y - 1;
        basei.z = basei.z + 1;
        if(basei.z > image_function->EndIndex.z) // interpolate across "x"
        {
          return ( ( valx00 ) );
        }
        const float val101 = get_pixel_3d(basei, in, image);
        basei.x = basei.x - 1;
        const float val001 = get_pixel_3d(basei, in, image);
        const float valx01 = val001 + ( val101 - val001 ) * distance0;

        return (valx00 + ( valx01 - valx00 ) * distance2);
      }
      const float val110 = get_pixel_3d(basei, in, image);
      basei.x = basei.x - 1;
      const float val010 = get_pixel_3d(basei, in, image);
      const float valx10 = val010 + ( val110 - val010 ) * distance0;
      const float valxx0 = valx00 + ( valx10 - valx00 ) * distance1;
      basei.z = basei.z + 1;
      if(basei.z > image_function->EndIndex.z) // interpolate across "xy"
      {
        return valxx0;
      }
      const float val011 = get_pixel_3d(basei, in, image);
      basei.x = basei.x + 1;
      const float val111 = get_pixel_3d(basei, in, image);
      basei.y = basei.y - 1;
      const float val101 = get_pixel_3d(basei, in, image);
      basei.x = basei.x - 1;
      const float val001 = get_pixel_3d(basei, in, image);
      const float valx01 = val001 + ( val101 - val001 ) * distance0;
      const float valx11 = val011 + ( val111 - val011 ) * distance0;
      const float valxx1 = valx01 + ( valx11 - valx01 ) * distance1;

      return (valxx0 + ( valxx1 - valxx0 ) * distance2);
    }
  }
}
#endif // DIM_3