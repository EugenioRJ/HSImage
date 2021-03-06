/*! \mainpage Hyperspectral Image Interface Library Documentation
 *
 * \section intro_sec Introduction
 *This library is designed to allow open-source experimentation with ENVI-BIL hyperspectral images. The goal of this software is to promote the study of hyperspectral images in the academic computer vision research world. This software provides both a C++ and Python interface for ease of use. An associated project has resulted in a publicly available library of images available at https://osf.io/pd49t/. These images are all compatible and intended to be used with this software interface library.
 *
 * \section install_sec Installation
 * Currently the software ONLY supports Linux (tested on Ubuntu 14.04) and MacOS. There is no platform specific software included in HSImage, but the software has not been tested on Windows, and so cannot claim to support it. If a user wishes to use HSImage on Windows, it should be possible, but there may be issues in the compliation with linking to the OpenCV libraries.
 *
 * The dependencies for HSImage are:
 * - Python 2 >=2.7 or Python 3 >=3.2
 * - Boost
 * - Numpy
 * - OpenCV 3.x - C++ and Python Bindings
 *      (OpenCV 2.x is NOT supported)
 *
 * Tnstall it and use it to create a virtual environment, type the following into terminal:he practice of using Python virtual environments is recommended. To set up a virtual environment, type the following into terminal:
 * \code{.sh}
 * pip install virtualenv
 * virtualenv hsi_env
 * source hsi_env/bin/activate
 * pip install HSI
 * \endcode
 *
 * Virtualenvwrapper is a convenience package built around virtualenv and is optional for this software, but is useful for managing multiple virtual environments. THIS DOES NOT NEED TO BE INSTALLED FOR HSI TO FUNCTION.
 *
 * To install it and use it to create a virtual environment, type the following into terminal:
 * \code{.sh}
 * pip install virtualenv virtualenvwrapper
 * source /usr/local/bin/virtualenvwrapper.sh
 * mkvirtualenv hsi_env
 * workon hsi_env
 * \endcode
 *
 *  To install Boost and Numpy type into the terminal
 * \code{.sh}
 * sudo apt-get install libboost-dev
 * pip install numpy
 * \endcode
 *
 * The OpenCV installation is non-trivial, as the HSI package requires both the Python and C++ OpenCV libraries. If you already have OpenCV 3.x with the appropriate Python version installed, there should be no additional steps needed to install HSImage.
 * If you do not have OpenCV installed, a bash script on the repository will can peform the installation for you on Ubuntu. The steps in the script should be appropriate for MacOS, but it has not been tested.
 * \code{.sh}
 * wget https://raw.githubusercontent.com/DTChuck/HSImage/master/install_opencv.sh
 * bash install_opencv.sh
 * \endcode
 *
 * After OpenCV is installed, the Python module will need to be symlinked into the virtual environment you have created.
 *
 * Python 2:
 * \code{.sh}
 * ln -s /usr/local/lib/python2.x/site_packages/cv2.so cv2.so
 * \endcode
 *
 * Python 3:
 * \code{.sh}
 * ln -s /usr/local/lib/python3.x/site_packages/cv2.cpython-3xm.so cv2.so
 * \endcode
 * where x is the minor version of Python installed on your system.
 *
 * Potentially, you will need to provide the directory of the C++ OpenCV libraries to your virtualenv. This is done by modifying the LD_LIBRARY_PATH variable
 * \code{.sh}
 * export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib/folder_holding_opencv_libs/
 * \endcode
 *
 * Installation of HSI is handled through the PIP package manager. Simply type into your terminal:
 * \code{.sh}
 * pip install HSI
 * \endcode
 *
 * This will build the c++/python combined library file and install it to the site-packages folder in your current Python evironment. To use the C++ libary, simply include `hsimage.h, classifiedhsimage.h, and labelfile.h in your project and link to the generated library file at compile.
 *
 * \section test_sec Testing
 * To test the software, the Github repository will need to be cloned to your computer. This will give you access to the setup.py file with the testing scripts. Simply run
 * \code{.sh}
 * python setup.py pytest
 * \endcode
 * This will perform the set of tests and output a result to the terminal the software is ran from.
 */

#ifndef HSIMAGE_H
#define HSIMAGE_H

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <tuple>
#include <algorithm>
#include <memory>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include "pybind11_opencv_numpy/pybind11/pybind11.h"
#include "pybind11_opencv_numpy/pybind11/stl.h"
#include "pybind11_opencv_numpy/ndarray_converter.h"

/*! \defgroup cplus_module C++ Interface 
 * This library allows C++ interactivity with ENVI-BIL hyperspectral images
 * \{
 */

/*!
 * \brief The HSImage class is the base class for interacting with ENVI type hyperspectral images.
 *
 * The HSImage class handles all the logistical operations required to support ENVI hyperspectral images in a C++ environment.
 * The class is designed to handle ENVI type hyperspectral images as well as spectrometer files in the format of "wavelength, intensity\n" as a .txt file.
 * The spectrometer files required to properly enable this behavior are a visible light file (400-700 nm) and a NIR file (700-1000 nm).
 */

class HSImage
{

public:
    HSImage(); /*!< Base Constructor*/
    HSImage(std::string header_location, std::string image_location);/*!< Constuctor with file locations */
    HSImage(std::string header_location, std::string image_location, std::vector<std::string> &spec_location); /*!< Constructor with spectrometer files*/
    HSImage(const HSImage& other); /*!< Reference Constructor */
    HSImage& operator=( const HSImage& other); /*!< Copy Constructor */

    /*!
     * \brief  Load hyperspectral image into HSImage
     * \param header_location location of .hdr file
     * \param image_location location of .raw file
     */
    void load(std::string header_location, std::string image_location);
    /*!
     * \brief  Load hyperspectral image into HSImage
     * \param header_location location of .hdr file
     * \param image_location location of .raw file
     * \param spec_locations location of the spectrometer files.
     */
    void load(std::string header_location, std::string image_location, std::vector<std::string> spec_locations);
    /*!
     * \brief  Load only spectrometer data for use with hyperspectral image
     * \param filenames locations of the spectrometer filenames.
     */
    void loadSpectrometerData(std::vector<std::string> filenames);

    /*!
     * \brief  Checks if spectrometer file locations are stored as part of the ENVI .hdr file
     * \param header_location location of .hdr file
     * \return Boolean value showing the presence of spectrometer location data in the .hdr file
     */
    static bool hasSpecFiles(std::string header_location);
    /*!
     * \brief  Adds spectrometer file data to the corresponding .hdr file.
     * \param filenames location of spectrometer files
     * This function modifies the .hdr file of the hyperspectral image to store information about the directory locations relative to the .hdr file where the spectromteter files corresponding to the hyperspectral image are stored.
     */
    void addSpecDataToHeader(std::vector<std::string> filenames);

    /*!
     * \brief  returns vector of pixel data
     * \param row row of target pixel
     * \param col column of target pixel
     * \return std::vector<u_int16_t> of intensity values for each wavelength at pixel (row,col)
     */
    std::vector<u_int16_t> getPixelSpectra(int row, int col);
    /*!
     * \brief Depracated. Do Not Use
     * \param row
     * \param col
     * \return
     */
    std::vector<u_int16_t> getNormalizedPixelSpectra(int row, int col);
    /*!
     * \brief  Returns pixel "transfer function". Requires spectrometer data.
     * \param row row of target pixel
     * \param col column of target pixel
     * \return std::vector<double> of transfer function values for each wavelength at pixel (row,col)
     * The transfer function of a hyperspectral pixel requires spectrometer data and returns Y in SY = W, where S is the input from the spectrometer and W is the measured wavelength from the hyperspectral camera. This gives the function that will transform the input light into the output light observed reflecting off of the target pixel.
     */
    std::vector<double> getPixelTransferFunction(int row, int col);

    /*!
     * \brief  Return a range of wavelength images as OpenCV cv::Mat objects.
     * \param lower_wavelength Bottom wavlength of the desired range
     * \param upper_wavelength Top wavlength of the desired range
     * \return Vector of cv::Mat objects containing the desired range of wavelength images.
     */
    std::vector<cv::Mat> getRange(const float lower_wavelength, const float upper_wavelength);
    /*!
     * \brief Return a disparate set of wavelength images as OpenCV cv::Mat objects.
     * \param wavelength_set Vector of desired wavlengths
     * \return Vector of cv::Mat objects containing desired wavelength images.
     */
    std::vector<cv::Mat> getSet(const std::vector<float> wavelength_set);
    /*!
     * \brief Get single deisred wavelength image
     * \param wavelength Desired wavelength
     * \return cv::Mat object containing the desired wavelength image.
     */
    cv::Mat operator[] (const float wavelength);

    /*!
     * \brief Get all data formatted as a row major pixel array (row*col,bands)
     * \return std::vector<u_int16_t> Vector of pixel values
     */
    std::vector<u_int16_t> getRawPixelData();

    /*!
     * \brief Return vector of imaged wavlengths.
     * \return std::vector<float> containing each wavelength that the sensor detected.
     */
    std::vector<float> getWavelengths();
    
    /*!
     * \brief Return vector of ambient wavelength intensites.
     * \return std::vector<float> containing the intensity of the ambient light at each wavelength imaged.
     */
    std::vector<float> getAmbientIntensities(); 

    /*!
     * \brief Return tuple containing the shape of the HSImage data array (row,col,bands)
     * \return std::tuple<int,int,int> containing shape of HSImage data array
     */
    std::tuple<int,int,int> getShape();


    //image metadata
    std::string acquisition_date; /*!< Date of image aquisition. Acquired from .hdr file. */
    int tint; /*!< Exposure value of hyperspectral image. Acquired from .hdr file. */
    int samples;/*!< Number of pixels per row in image. Acquired from .hdr file. */
    int lines;/*!< Number of rows in image. Acquired from .hdr file. */
    int bands;/*!< Number of wavelengths in image. Acquired from .hdr file. */
    int fps;/*!< Number of wavelengths aquired per second. Acquired from .hdr file. */
    int binning[2]; /*!< Array containing the data reduction employed by the camera. Acquired from .hdr file. */
    std::vector<float> wavelengths;/*!< Vector of acquired wavelength values. Acquired from .hdr file. */
    std::vector<float> fwhm;/*!< Vector of individual wavelength band width. Acquired from .hdr file. */
    std::vector<float> ambient_intensities; /*!< Vector of ambient intensity data. Acquired from .hdr file. */
    bool has_spec_data;/*!< Value showing presence of spectrometer file locations. Acquired from .hdr file. */

    //image file location
    std::string img_file;/*!< Location of .raw hyperspectral image file */
    std::string hdr_file;/*!< Location of .hdr hyperspectral image file */
    std::string nir_spec_file;/*!< Location of NIR spectrometer file */
    std::string vis_spec_file;/*!< Location of VIS spectrometer file */

private:
    /*!
     * \brief  Load only .hdr file of hyperspectral image.
     * \param header_location location of .hdr file
     * Not intended for external use. Use load() instead.
     */
    void loadHeader(std::string header_location);
    /*!
     * \brief  Load only .raw file of hyperspectral image
     * \param image_location
     * Not intended for external use. Use load() instead.
     */
    void loadRawImage(std::string image_location);

    std::unordered_map<float,uchar*> image_map;
    //std::unique_ptr<u_int16_t[]> image_data;
    //std::unique_ptr<u_int16_t[]> pixel_data;

    std::vector<u_int16_t> image_data;
    std::vector<u_int16_t> pixel_data;

    std::string createRelativeSpecFilepath(std::string abs_spec_filepath);
    std::string createAbsoluteSpecFilepath(std::string rel_spec_filepath);

    template <typename T>
    static T closest(std::vector<T> const& vec, T value) {
     auto const it = std::lower_bound(vec.begin(), vec.end(), value);
     if (it == vec.end()) { return -1; }
     return *it;
    }

    static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            if(!item.empty())
            {
                elems.push_back(item);
            }
        }
        return elems;
    }

    static std::vector<std::string> split(const std::string &s, char delim) {
        std::vector<std::string> elems;
        split(s, delim, elems);
        return elems;
    }

};

/*! \} */

#endif // HSIMAGE_H
