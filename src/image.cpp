#include "image.h"

/* constructor ****************************************************************/

Image::Image()
: m_height(-1), m_width(-1), m_channel(-1)
{}

Image::Image(int height, int width, int channel)
: m_height(height), m_width(width), m_channel(channel)
{
  init_data(m_height, m_width, m_channel);
}

Image::Image(int height, int width, int channel, const DataType& data)
: m_height(height), m_width(width), m_channel(channel), m_data(data)
{
  assert( m_data.rows() == m_height*m_width );
}

Image::Image(const Image& other)
: m_height(other.m_height), m_width(other.m_width), m_channel(other.m_channel), m_data(other.m_data)
{}

/* destructors ****************************************************************/

Image::~Image()
{}

/* operations *****************************************************************/
void
Image::normalize()
{
  double max = m_data.maxCoeff();
  m_data /= max;
}

void
Image::convolve(const Image& kernel, Image& out) const
{  
  out = Image(m_height, m_width);

  for(int i=0; i<m_width; ++i)
    for(int j=0; j<m_height; ++j)
      out.data().row(i*m_height+j) = convolution_kernel(i, j, kernel);
}

/* helper functions **********************************************************/
Image::PixelType
Image::convolution_kernel(int x, int y, const Image& kernel) const
{
  int w_ker = kernel.width() ; int w_ker_2 = w_ker/2;
  int h_ker = kernel.height(); int h_ker_2 = h_ker/2;

  PixelType sum = PixelType::Zero(m_channel);
  for(int i=0; i<w_ker; ++i){
    for(int j=0; j<h_ker; ++j){
      int px_i = x - w_ker_2 + i;
      int px_j = y - h_ker_2 + j;

      //use mirror border condition
      if(px_i < 0)
        px_i = (-px_i) - 1;
      if(px_j < 0)
        px_j = (-px_j) - 1;

      if(px_i >= m_width)
        px_i = (m_width-1) - (px_i-m_width);
      if(px_j >= m_height)
        px_j = (m_height-1) - (px_j-m_height);

      //get pixel value and add
      sum += data().row(px_i*m_height + px_j) * kernel.data().row(i*h_ker + j);
    }
  }

  return sum;
}
