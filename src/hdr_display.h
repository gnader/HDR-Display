#ifndef HDR_DISPLAY_H
#define HDR_DISPLAY_H

/* base hdr display class ***************************************************************/

/**
 *`@brief a generic base class that implements HDR displays algorithms.
 *        takes two types as template:
 *          - TImage is the desired generated image type
 *          - TPram is a type capable of handeling the algorithm parameters
 *
 *        in order to define a new algorithm, it is required to extend this
 *        class, implement the process() function and define its conrresponding
 *        parameter class
 */
template <class TImage, class TParams>
class BaseHDRDisplay
{
public:
  typedef TImage      ImageType;
  typedef TParams ParameterType;

public:
  BaseHDRDisplay() {}
  BaseHDRDisplay(const ParameterType& params)
  : m_params(params)
  {}

  virtual ~BaseHDRDisplay() {}

  void set_model_parameters(const ParameterType& params)
  {
    m_params = params;
  }

public:
  virtual void process(const ImageType& hdr_in, ImageType& ldr_out1, ImageType& ldr_out2) const = 0;

protected:
  ParameterType m_params;
};

/* projector-based display algorithm ***************************************************/

/**
 *`@brief The projector-base display algorithm as described in :
 *          H. Seetzen, W. Heidrich, W. Stuerzlinger, G. Ward, L. Whitehead, M. Trentacoste, A. Ghosh,
 *          and A. Vorozcovs, “High dynamic range display systems,” ACM Trans. Graph., vol. 23, no. 3, p.
 *          760, Aug. 2004. (section 4, figure 5)
 *
 *        takes two types as template:
 *          - TImage is the desired generated image type
 *          - TPram is a type capable of handeling the algorithm parameters
 *
 *        TParam should contain the following attributes:
 *          - psf : the projector's psf model
 *          - dlp_response : the response model of the dlp projector
 *          - lcd_response : the response model of the lcd screen
 */

template <class TImage, class TParams>
class ProjectorBasedDisplay : public BaseHDRDisplay<TImage, TParams>
{
public:
  typedef BaseHDRDisplay<TImage, TParams> SuperClass;

  typedef typename SuperClass::ImageType         ImageType;
  typedef typename SuperClass::ParameterType ParameterType;

public:
  ProjectorBasedDisplay() : SuperClass() {}
  ProjectorBasedDisplay(const ParameterType& params)
  : SuperClass(params)
  {}

  virtual ~ProjectorBasedDisplay() {}

public:
  virtual void process(const ImageType &hdr_in, ImageType &ldr_out1, ImageType &ldr_out2) const
  {
    int h = hdr_in.height();
    int w = hdr_in.width();

    //compute sqrt(I)
    ImageType sqroot(h, w);
    sqroot.data() = hdr_in.data().sqrt();

    //compute convolution(psf, sqrt(I))
    ImageType kernel, temp;
    this->m_params.psf->generate(kernel);
    sqroot.convolve(kernel, temp);

    //compute I/convolution(psf, sqrt(I));
    temp.data() = hdr_in.data()/temp.data();

    //compute the dlp image using the projector's response
    ldr_out1 = ImageType(h, w);
    this->m_params.dlp_response->luma(sqroot, ldr_out1);

    //compute the lcd image using the screen's response
    ldr_out2 = ImageType(h, w);
    this->m_params.lcd_response->luma(temp, ldr_out2);
  }
};

#endif //HDR_DISPLAY_H
