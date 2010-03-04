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
Photometry::Ptr ApertureMeasurePhotometry::doMeasure(Image const& im, float, float) {
    /// Measure your fluxes here
    std::vector<float> rad(AperturePhotometry::NRADIUS);
    std::vector<double> flux(AperturePhotometry::NRADIUS);
    std::vector<float> fluxErr(AperturePhotometry::NRADIUS);

    rad[0] =       6.66; flux[0] = im;     fluxErr[0] = -1;
    rad[1] = rad[0] + 1; flux[1] = 1.1*im; fluxErr[1] = -1;
    rad[2] = rad[1] + 1; flux[2] = 1.2*im; fluxErr[2] = -1;

    return boost::make_shared<AperturePhotometry>(rad, flux, fluxErr);
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
