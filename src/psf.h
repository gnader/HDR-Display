#ifndef PSF_H
#define PSF_H

/* base psf class **********************************************************/

/**
 *`@brief a generic base class than represents a psf.
 *        takes two types as template:
 *          - TImage is the desired generated image type
 *          - TPram is a type capable of handeling the psf parameters
 *
 *        in order to define a new PSF model, it is required to extend this
 *        class, implement the generate() function and define its conrresponding
 *        parameter class
 */
template <class TImage, class TParam>
class BasePSF
{
public:
  typedef TImage     ImageType;
  typedef TParam ParameterType;

public:
  BasePSF() {}
  BasePSF(const ParameterType& params)
  : m_params(params)
  {}

  virtual ~BasePSF() {}

  void set_model_parameters(const ParameterType& params)
  {
    m_params = params;
  }

public:
  virtual void generate(ImageType& psf) const = 0;

protected:
  ParameterType m_params;
};

/* Gaussian psf class ********************************************************/

/**
 *`@brief represents a 2d isotropic gaussian psf.
 *        takes two types as template:
 *          - TImage is the desired generated image type
 *          - TPram is a type capable of handeling the psf parameters
 *
 *        TParam should contain the following attributes:
 *          - h : the psf height in pixels
 *          - w : the psf width in pixels
 *          - c : the number of channels
 *          - sigma : the guassian parameters in pixels
 */
template <class TImage, class TParam>
class GaussianPSF : public BasePSF<TImage, TParam>
{
public:
  typedef BasePSF<TImage, TParam> SuperClass;

  typedef typename SuperClass::ImageType         ImageType;
  typedef typename SuperClass::ParameterType ParameterType;

public:
  GaussianPSF() : SuperClass() {}
  GaussianPSF(const ParameterType& params)
  : SuperClass(params)
  {}

  virtual ~GaussianPSF() {}

public:
  virtual void generate(ImageType& psf) const
  {
    int h = this->m_params.h;
    int w = this->m_params.w;
    int c = this->m_params.c;

    psf = ImageType(h, w);

    //compute center
    double xc = double(w)/2. - 1.;
    double yc = double(h)/2. - 1.;

    //constants
    double sigma2_sq = 2.*this->m_params.sigma*this->m_params.sigma;

    //compute psf
    for(int i=0; i<w; ++i){
      for(int j=0; j<h; ++j){
        double x = (double(i) - xc);
        double y = (double(j) - yc);

        double value = exp(-((x*x + y*y)/sigma2_sq));

        psf.set_pixel(i, j, value);
      }
    }

    //normalize psf
    psf.data() /= psf.data().sum();
  }
};

#endif //PSF
