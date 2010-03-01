// -*- lsst-c++ -*-
/************************************************************************************************************/
/**
 * Implement aperture photometry.  We have an extra data member which we want to appear in the schema;
 * if we didn't it could just be a regular (private?) data member 
 *
 * We only need to provide a separate header file so that we can perform the dynamic cast in main
 * to call getRadius();  if we were willing to say photom->get("radius") instead this wouldn't be necessary
 */
#include "AperturePhotometry.h"

PHOTOMETRY_BOILERPLATE("aper", Aperture)

/************************************************************************************************************/
/**
 * Process the image; calculate values
 */
Photometry::Ptr ApertureMeasurePhotometry::doMeasure(Image const& im) {
    /// Measure your flux here
    return boost::make_shared<AperturePhotometry>(6.66, im, 0.1);
}

/************************************************************************************************************/
/// Virtual function called by operator<< to dynamically dispatch the type to a stream
std::ostream &AperturePhotometry::output(std::ostream &os ///< the output stream
                                        ) const {
    for (int i = 0; i != getNRadius(); ++i) {
        os << "[R: " << getRadius(i) << "  " << getFlux(i) << "+=" << getFluxErr(i) << "]";
        if (i + 1 != getNRadius()) {
            os << " ";
        }
    }

    return os;
}
