// -*- lsst-c++ -*-
#include "Photometry.h"

/// Output to stream os
std::ostream &Photometry::output(std::ostream &os ///< The stream to output to
                                ) const {
    os << getFlux();
    if (getFluxErr() >= 0) {
        os << "+-" << getFluxErr();
    }
    return os;
}
