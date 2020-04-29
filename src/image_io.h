#ifndef IMAGE_IO_H
#define IMAGE_IO_H

#include <string>

#include "image.h"

/**
 * @brief simple function that reads an image and resizes it if necessary
 *        This functions uses CImg for reading and resizing.
 * @param image is the output
 * @param filename is the path of the image
 * @param h is the new height of the image, set to 0 if no height resize is desired
 * @param w is the new width of the image, set to 0 if not width resize is desired
 * @return true if reading was successfull and false if not
 */
bool read_image(Image& image,
                const std::string& filename,
                unsigned int h=0,
                unsigned int w=0);

/**
 * @brief simple function that saves and image.
 *        This function uses CImg for writing
 * @param image is the input
 * @param filename is the path of the image
 * @return true if writing was successfull.
 */
bool write_image(const Image &image,
                 const std::string& filename);

#endif //IMAGE_IO_H
