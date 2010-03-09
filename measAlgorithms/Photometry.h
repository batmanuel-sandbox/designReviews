#if !defined(PHOTOMETRY_H)
#define PHOTOMETRY_H 1

#include "Measurement.h"
#include "Image.h"

/**
 * A version of Measurement designed to support Photometry
 */
class Photometry : public Measurement<Photometry> {
protected:
    /// The quantities that the base-class Photometry knows how to measure
    /// These values will be used as an index into Measurement::_data
    ///
    /// NVALUE is used by subclasses to add more quantities that they care about
    enum { FLUX=0, FLUX_ERR, NVALUE };
public:
    typedef boost::shared_ptr<Photometry> Ptr;
    typedef boost::shared_ptr<Photometry const> ConstPtr;

    /// Add desired members to the schema
    virtual void defineSchema(Schema::Ptr schema) {
        schema->add(SchemaEntry("flux", FLUX, Schema::DOUBLE));
        schema->add(SchemaEntry("fluxErr", FLUX_ERR, Schema::FLOAT, 1));
    }

    /// Return the flux
    virtual double getFlux() const {
        return Measurement<Photometry>::get<FLUX, double>();
    }
    /// Return the flux (if an array)
    virtual double getFlux(int i) const {
        return Measurement<Photometry>::get<FLUX, double>(i);
    }
    /// Return the error in the flux
    virtual float getFluxErr() const {
        return Measurement<Photometry>::get<FLUX_ERR, float>();
    }
    /// Return the error in the flux (if an array)
    virtual float getFluxErr(int i) const {
        return Measurement<Photometry>::get<FLUX_ERR, float>(i);
    }

    virtual std::ostream &output(std::ostream &os) const;
};

/**
 * A class that actually does the work of measuring things
 *
 * More precisely, a base class whose subclasses do the real work.  The main purpose of MeasurePhotometry is
 * to provide the FactoryPtr typedef, and also to hide MeasureQuantity<Photometry::Ptr, Image>. Note that
 * MeasureQuantity knows nothing about Image classes
 */
class MeasurePhotometry : public MeasureQuantity<Photometry::Ptr, Image> {
public:
    typedef boost::shared_ptr<MeasurePhotometry> Ptr;
    typedef MeasureQuantity<Photometry::Ptr, Image> *FactoryPtr;

    MeasurePhotometry() {}
};

/************************************************************************************************************/

/*
 * Define a macro to provide all the per-algorithm boilerplate
 */
#define PHOTOMETRY_BOILERPLATE(NAME, ALG) \
    MEASUREMENT_BOILERPLATE(Photometry, NAME, ALG, (Image const& im, Peak const& peak))

#endif
