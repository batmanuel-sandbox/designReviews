#if !defined(PHOTOMETRY_H)
#define PHOTOMETRY_H 1

#include "Measurement.h"
#include "MeasurementData.h"
#include "Image.h"

/**
 * A version of Measurement designed to support Photometry
 */
class Photometry : public Measurement<Photometry>
{
protected:
    /// The quantities that the base-class Photometry knows how to measure
    /// These values will be used as an index into MeasurementData::_data
    ///
    /// NVALUE is used by subclasses to add more quantities that they care about
    enum { FLUX=0, FLUX_ERR, NVALUE };
public:
    typedef boost::shared_ptr<Photometry> Ptr;
    typedef boost::shared_ptr<Photometry const> ConstPtr;

    /// Add desired members to the schema
    static void defineSchema(Schema::Ptr schema) {
        schema->add(SchemaEntry("flux", FLUX, Schema::DOUBLE));
        schema->add(SchemaEntry("fluxErr", FLUX_ERR, Schema::FLOAT, 1));
    }

    /// Return the flux
    virtual double getFlux() const = 0;
    /// Return the error in the flux
    virtual float getFluxErr() const = 0;

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
/**
 * A class to provide the implement the Photometry interface, and provide a class containing the
 * data (using the CRTP --- XxxPhotometry ultimately derives from MeasurementData<XxxPhotometry, Photometry>)
 */
template<typename T>
class PhotometryImpl : public MeasurementData<T, Photometry> {
public:
    /// Return the flux
    virtual double getFlux() const {
        return MeasurementData<T, Photometry>::template get<Photometry::FLUX, double>();
    }
    /// Return the flux
    virtual double getFlux(int i) const {
        return MeasurementData<T, Photometry>::template get<Photometry::FLUX, double>(i);
    }
    /// Return the error in the flux
    virtual float getFluxErr() const {
        return MeasurementData<T, Photometry>::template get<Photometry::FLUX_ERR, float>();
    }
    /// Return the error in the flux
    virtual float getFluxErr(int i) const {
        return MeasurementData<T, Photometry>::template get<Photometry::FLUX_ERR, float>(i);
    }
};

/*
 * Define a macro to provide all the per-algorithm boilerplate
 */
#define PHOTOMETRY_BOILERPLATE(NAME, ALG) \
    MEASUREMENT_BOILERPLATE(Photometry, NAME, ALG, (Image const& im, Peak const& peak))

#endif
