#pragma once

#include <vector>
#include <string>
#include <functional>
#include <limits>
#include "stdio.h"

#include <cassert>

// OpenGM and Halide MUST be included before CImg, which includes
// X11 headers with conflicting definitions.
#include "opengm/graphicalmodel/graphicalmodel.hxx"
#include "opengm/graphicalmodel/space/simplediscretespace.hxx"
#include "opengm/functions/explicit_function.hxx"
#include "opengm/functions/potts.hxx"

#include "Halide.h"


#define cimg_use_png
#define cimg_use_jpeg
#define cimg_use_jpeg
#include "cimg/CImg.h"

#include "opencv2/opencv.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;

using namespace cimg_library;

#ifdef PRECOMPILE_CIMG

extern template struct cimg_library::CImg<float>;
extern template struct cimg_library::CImg<double>;
extern template struct cimg_library::CImg<int>;
extern template struct cimg_library::CImg<bool>;

#endif

inline double norm(float x, float y) {
    return sqrt(x * x + y * y);
}

template<class T>
inline T sqr(T v) {
    return v * v;
}
