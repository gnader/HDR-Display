#include "image_io.h"

#include <CImg/CImg.h>

bool read_image(Image& image,
                const std::string& filename,
                unsigned int h,
                unsigned int w)
{
  cimg_library::CImg<double> temp(filename.c_str());

  if(temp.is_empty())
    return false;

  bool rescale = false;
  if( h == 0 )
    h = temp.height();
  else
    rescale = true;

  if( w == 0)
    w = temp.width();
  else
    rescale = true;

  //rescale if necessary
  if(rescale)
    temp.resize(w, h, 1, temp.spectrum(), 3) ; // rescale using linear interpolation

  image = Image(temp.height(), temp.width(), temp.spectrum());
  for(int i=0; i<w; ++i){
    for(int j=0; j<h; ++j){
      int id = i*h + j;

      for(int c=0; c<temp.spectrum(); ++c){
        image.data(i, j, c) = temp(i, j, 0, c);
      }

    }
  }

  return true;
}

bool write_image(const Image &image,
                 const std::string& filename)
{
  if(!image.is_valid())
    return false;

  int h = image.height();
  int w = image.width();

  cimg_library::CImg<double> temp(w, h, 1, 3, 0);
  for(int i=0; i<w; ++i){
    for(int j=0; j<h; ++j){

      for(int c=0; c<image.channel(); ++c)
        temp(i, j, 0., c) = image.data(i, j, c);
    }
  }

  temp.save(filename.c_str());

  return true;
}
