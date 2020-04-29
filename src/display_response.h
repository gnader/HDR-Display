#ifndef DISPLAY_RESPONSE_H
#define DISPLAY_RESPONSE_H

/* base display response class ***************************************************/

/**
 * @brief a generic base class than represents a display response model.
 *        takes two types as template:
 *          - TImage is the desired generated image type
 *          - TPram is a type capable of handeling the display response model
 *            parameters
 *
 *        in order to define a new display response model, it is required to
 *        extend this class, implement the luminance() and luma() functions which
 *        correspond to the display response function and inverse display response
 *        functions respectively and define its conrresponding parameter class
 */
template<class TImage, class TParam>
class BaseDisplayResponse
{
public:
  typedef TImage ImageType;
  typedef TParam ParameterType;

public:
  BaseDisplayResponse()
  : m_params(NULL)
  {}

  BaseDisplayResponse(const ParameterType& params)
  : m_params(params)
  {}

  virtual ~BaseDisplayResponse()
  {}

public:
  inline void set_model_parameters(const ParameterType& params)
  {
    m_params = params;
  }

public:
  virtual void luminance(const ImageType& in, ImageType& out) const = 0;
  virtual void luma     (const ImageType& in, ImageType& out) const = 0;

protected:
  ParameterType m_params;
};


/* GammaOffsetGain (GOG) display response model ***************************************************/

/**
 *`@brief implementation of the GOG display response model
 *        details of this model can be found in :
 *          R. K. Mantiuk, “Perceptual display calibration,” in Displays: Fundamentals and Applications,
 *          2nd ed., R. R. Hainich and O. Bimber, Eds. CRC Press, 2016. (section 1.3)
 *
 *        takes two types as template:
 *          - TImage is the desired generated image type
 *          - TPram is a type capable of handeling the GOG parameters
 *
 *        TParam should contain the following attributes:
 *          - Lpeak : peak luminance
 *          - Lblack : black luminance
 *          - gamma
 */

template<class TImage, class TParams>
class GainOffsetGamma : public BaseDisplayResponse<TImage, TParams>
{
public:
  typedef BaseDisplayResponse<TImage, TParams> SuperClass;

  typedef typename SuperClass::ParameterType  ParameterType;
  typedef typename SuperClass::ImageType          ImageType;

public:
  GainOffsetGamma() : SuperClass(){}
  GainOffsetGamma(ParameterType params) : SuperClass(params){}

  virtual ~GainOffsetGamma() {}

public:
  virtual void luminance(const ImageType& in, ImageType& out) const
  {
    out.data() = (this->m_params.Lpeak - this->m_params.Lblack) * in.data().pow(this->m_params.gamma) + this->m_params.Lblack;
  }

  virtual void luma(const ImageType& in, ImageType& out) const
  {
    out.data() = ((in.data() - this->m_params.Lblack) / (this->m_params.Lpeak-this->m_params.Lblack)).pow(1./this->m_params.gamma);

    //clamp values between 0 and 1
    out.data().min(0.).max(1.);
  }
};

#endif //DISPLAY_RESPONSE_H
