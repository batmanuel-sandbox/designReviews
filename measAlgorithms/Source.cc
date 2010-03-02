// -*- lsst-c++ -*-
#include "Source.h"

/// Output to stream os
std::ostream &operator<<(
        std::ostream &os,               ///< The stream to output to
        Source const& s                 ///< The Source to print
                        ) {
    return os << "{" << s.getAstrometry() << "\t" << s.getPhotometry() << "}";
}
