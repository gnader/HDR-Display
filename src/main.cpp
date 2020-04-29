#include <algorithm>
#include <iostream>

#include "input_parser.h"

#include "image.h"
#include "image_io.h"

#include "psf.h"
#include "display_response.h"
#include "hdr_display.h"

std::vector<std::string> valild_formats{ "png",
                                         "jpg",
                                         "jpeg" } ; //add more valid formats here

void output_usage()
{
  std::cout << "./hdr <option> <values>" << std::endl;
  std::cout << "  -in  [filename]               : input image" << std::endl;
  std::cout << "  -out [format]                 : format of output image (optional)" << std::endl;
  std::cout << "  -res [width] [height]         : output resolution      (optional)" << std::endl;
  std::cout << "  -psf [sigma]                  : gaussian psf parameter (optional)" << std::endl;
  std::cout << "  -dlp [Lpeak] [Lblack] [gamma] : dlp response model     (optional)" << std::endl;
  std::cout << "  -lcd [Lpeak] [Lblack] [gamma] : lcd response model     (optional)" << std::endl;
}

void check_format_validity(std::string& format)
{
  if(std::find(valild_formats.begin(), valild_formats.end(), format) == valild_formats.end()){
    std::cerr << format << " is not supported, using png instead" << std::endl;
    format = "png";
  }
}

/* PSF Model *************************************************/
struct PSFParams
{
 int h;
 int w;
 int c;
 double sigma;

 PSFParams(double pixels=8.)
 : h(int(pixels)), w(int(pixels)), c(3), sigma(pixels)
 {}

 void set_sigma(double pixels)
 {
   sigma = pixels;
   h = int(pixels);
   w = int(pixels);
 }
};

typedef GaussianPSF<Image, PSFParams> PSF;
/*************************************************************/

/* Display Response Model ************************************/
struct DRParams
{
 double Lpeak;
 double Lblack;
 double gamma;

 DRParams(int _Lpeak=1, int _Lbalck=0, double _gamma=2.2)
 : Lpeak(_Lpeak), Lblack(_Lbalck), gamma(_gamma)
 {}
};

typedef GainOffsetGamma<Image, DRParams> DisplayResponse;
/*************************************************************/

/* HDR Display Algorithm *************************************/
struct HDRDisplayParams
{
 PSF* psf;
 DisplayResponse* dlp_response;
 DisplayResponse* lcd_response;

 HDRDisplayParams(PSF* _psf=NULL, DisplayResponse* _dlp=NULL, DisplayResponse* _lcd=NULL)
 : psf(_psf), dlp_response(_dlp), lcd_response(_lcd)
 {}
};

typedef ProjectorBasedDisplay<Image, HDRDisplayParams> HDRDisplay;
/**************************************************************/

int main(int argc, char** argv)
{
  bool success = true;

  InputParser parser(argc, argv);

  if(parser.cmdOptionExists("-h") || parser.cmdOptionExists("-help")){
    output_usage();
    return 0;
  }

  InputParser::TokenList tokens;

  //input image
  std::string filename;

  if(parser.getCmdOption("-in", tokens) > 0)
    filename = tokens[0];
  else{
    std::cerr << "error parsing input image : -in option not found" << std::endl;
    output_usage();
    return 1;
  }

  //output format
  std::string format = "png";
  if(parser.getCmdOption("-out", tokens) > 0)
    format = tokens[0];
  check_format_validity(format);

  //default parameters
  int w = 1024, h = 768;
  PSFParams p_psf(8.);
  DRParams  p_dlp(5000., 5., 2.2);
  DRParams  p_lcd(1., 0.005, 2.2);

  //load parameter values if available
  if(parser.getCmdOption("-res", tokens) == 2){
    w = std::atof(tokens[0].c_str());
    h = std::atof(tokens[1].c_str());
  }

  if(parser.getCmdOption("-psf", tokens) > 0)
    p_psf.set_sigma(std::atof(tokens[0].c_str()));

  if(parser.getCmdOption("-dlp", tokens) == 3){
    p_dlp.Lpeak  = std::atof(tokens[0].c_str());
    p_dlp.Lblack = std::atof(tokens[1].c_str());
    p_dlp.gamma  = std::atof(tokens[2].c_str());
  }

  if(parser.getCmdOption("-lcd", tokens) == 3){
    p_lcd.Lpeak  = std::atof(tokens[0].c_str());
    p_lcd.Lblack = std::atof(tokens[1].c_str());
    p_lcd.gamma  = std::atof(tokens[2].c_str());
  }
  
    //load image
  Image i_hdr;
  success = read_image(i_hdr, filename, h, w);
  if(!success){
    std::cerr << "unable to load image" << std::endl;
    return -1;
  }

  std::cout << "image loaded." << std::endl;

  write_image(i_hdr, "test.png");
  
 //make sure that the psf has the same number of channels as the input image
 p_psf.c = i_hdr.channel();

 //create models
 PSF psf(p_psf);
 DisplayResponse r_dlp(p_dlp);
 DisplayResponse r_lcd(p_lcd);

 HDRDisplayParams p_hdr(&psf, &r_dlp, &r_lcd);
 HDRDisplay hdr(p_hdr);

 //run algorithm
 Image i_dlp, i_lcd;
 std::cout << "processing hdr ... " << std::flush;
 hdr.process(i_hdr, i_dlp, i_lcd);
 std::cout << "done." << std::endl;

 //i_dlp and i_lcd are between 0 and 1
 //perfrom a linear map between 0 and 255 and save
 i_dlp.data() *= 255.;
 i_lcd.data() *= 255.;

 //compute output filenames
 std::string image_name = filename.substr(filename.find_last_of("/")+1,
                                          filename.find_last_of(".")-filename.find_last_of("/")-1);

 std::string dlp_out(image_name), lcd_out(image_name);
 dlp_out.append("_dlp.").append(format);
 lcd_out.append("_lcd.").append(format);

 //save images
 success = write_image(i_dlp, dlp_out);
 if(!success)
   std::cerr << "unable to save dlp image" << std::endl;
 std::cout << "dlp image saved." << std::endl;

 success = write_image(i_lcd, lcd_out);
 if(!success)
   std::cerr << "unable to save lcd image" << std::endl;
 std::cout << "lcd image saved." << std::endl;

  return 0;
}
