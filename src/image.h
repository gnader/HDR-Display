#ifndef IMAGE_H
#define IMAGE_H

#include <assert.h>
#include <vector>

#include <Eigen/Core>

/**
 * @brief Minimal Image class
 */
class Image
{
public:
  /* internal types ***********************************************************/
  typedef Eigen::Array<double, Eigen::Dynamic, Eigen::Dynamic>  DataType;
  typedef Eigen::Array<double, Eigen::Dynamic, 1           > ChannelType;
  typedef Eigen::Array<double, 1             , Eigen::Dynamic> PixelType;

public:
  /* contructors **************************************************************/
  Image();
  Image(int height, int width, int channel=3);
  Image(int height, int width, int channel, const DataType& data);
  Image(const Image& other);

  /* destructors **************************************************************/
  virtual ~Image();

  /* access image properties **************************************************/
  inline int height() const
  {
    return m_height;
  }

  inline int width() const
  {
    return m_width;
  }

  inline int channel() const
  {
    return m_channel;
  }

  inline bool is_valid() const
  {
    return m_data.rows() > 0;
  }

  /* access image data ********************************************************/
  inline DataType& data()
  {
    return m_data;
  }

  inline const DataType& data() const
  {
    return m_data;
  }

  inline double& data(int i, int j, int channel)
  {
    return m_data(i*m_height+j, channel);
  }

  inline const double& data(int i, int j, int channel) const
  {
    return m_data(i*m_height+j, channel);
  }

  /**
   * @brief set all channels of pixel [i,j] to value
   * @param i is the ith row
   * @param j is the jth row
   * @param value is the pixel value
   */
  inline void set_pixel(int i, int j, double value)
  {
    m_data.row(i*m_height+j) = PixelType::Ones(m_channel)*value;
  }

  /* operations ***************************************************************/
  /**
   * @brief normalizes the values between 0 and 1
   */
  void normalize();

  /**
   * @brief performes a convolution operation with kernel.
   *        Mirror boundary conditions are implemented..
   * @param kernel is the convolution kernel
   * @param out is the resuting image
   */
  void convolve(const Image& kernel, Image& out) const;

protected:
  /* initialisation ***********************************************************/
  /**
   * @brief initialises m_data to the correct dimentions and sets it to 0
   */
  inline void init_data(int height, int width, int channel=3)
  {
    m_data.resize(height*width, channel);
    m_data.setZero();
  }

  /* helper functions *********************************************************/
  /**
   * @brief a helper function for the convolution operation. its goal is to make
   *        the code easier to read.
   */
  PixelType convolution_kernel(int x, int y, const Image& kernel) const;

private:
  /* image size ***************************************************************/
  int m_height;
  int m_width;
  int m_channel;

  /* image data ***************************************************************/
  DataType m_data;
};

#endif //IMAGE_H
