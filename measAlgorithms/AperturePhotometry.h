// -*- lsst-c++ -*-
#if !defined(APERTURE_PHOTOMETRY_H)
#define APERTURE_PHOTOMETRY_H 1
#include "Photometry.h"
#include "MeasurementData.h"

/**
 * Implement aperture photometry.  We include the radius in the schema, and also provide
 * an accessor function
 */
#if defined(__ICC)
#pragma warning (push)
#pragma warning (disable: 1125)         // "Photometry::XXX" is hidden by "AperturePhotometry::XXX"
#endif

class AperturePhotometry : public PhotometryImpl<AperturePhotometry>
{
public:
    enum { NRADIUS = 3 };               // dimension of RADIUS array
private:
    /// We need a new, unused, index to save the radius in.  [0, Photometry::NVALUE) are already taken
    enum { FLUX=Photometry::FLUX,
           FLUX_ERR = FLUX     + NRADIUS,
           RADIUS   = FLUX_ERR + NRADIUS,
           NVALUE   = RADIUS   + NRADIUS };
public:
    typedef boost::shared_ptr<AperturePhotometry> Ptr;
    typedef boost::shared_ptr<AperturePhotometry const> ConstPtr;

    /// Create an AperturePhotometry to record our measurements
    AperturePhotometry(std::vector<float> const& radius,
                       std::vector<double> const& flux,
                       std::vector<float> const& fluxErr) {
        init();                         // This allocates space for everything in the schema

        assert(radius.size() == NRADIUS);
        for (int i = 0; i != NRADIUS; ++i) {
            set<RADIUS>(i, radius[i]);
            set<FLUX>(i, flux[i]);
            set<FLUX_ERR>(i, fluxErr[i]);
        }
    }

    /// Add desired fields to the schema
    static void defineSchema(Schema::Ptr schema ///< our schema; == AperturePhotometry::_mySchema
                      ) {
        schema->add(SchemaEntry("flux",    FLUX,     Schema::DOUBLE, NRADIUS));
        schema->add(SchemaEntry("fluxErr", FLUX_ERR, Schema::FLOAT,  NRADIUS));
        schema->add(SchemaEntry("radius",  RADIUS,   Schema::FLOAT,  NRADIUS, "arcsec"));
    }

    /// Return the number of radii
    int getNRadius() const {
        return NRADIUS;
    }

    /// Return the flux
    double getFlux(int i) const {
        return get<FLUX, double>(i);
    }
    /// Return the error in the flux
    float getFluxErr(int i) const {
        return get<FLUX_ERR, float>(i);
    }
    /// Return the radius
    float getRadius(int i) const {
        return get<RADIUS, float>(i);
    }
    
    virtual std::ostream &output(std::ostream &os) const;
};
#if defined(__ICC)
#pragma warning (pop)
#endif

#endif
