#if !defined(ASTROMETRY_H)
#define ASTROMETRY_H 1

#include "Measurement.h"
#include "Image.h"

/**
 * A version of Measurement designed to support Astrometry
 */
class Astrometry : public Measurement<Astrometry> {
protected:
    /// The quantities that the base-class Astrometry knows how to measure
    /// These values will be used as an index into Measurement::_data
    ///
    /// NVALUE is used by subclasses to add more quantities that they care about
    enum { X=0, X_ERR, Y, Y_ERR, NVALUE };
public:
    typedef boost::shared_ptr<Astrometry> Ptr;
    typedef boost::shared_ptr<Astrometry const> ConstPtr;

    /// Add desired members to the schema
    virtual void defineSchema(Schema::Ptr schema) {
        schema->add(SchemaEntry("x", X, Schema::DOUBLE, 1, "pixel"));
        schema->add(SchemaEntry("xErr", X_ERR, Schema::FLOAT, 1, "pixel"));
        schema->add(SchemaEntry("y", Y, Schema::DOUBLE, 1, "pixel"));
        schema->add(SchemaEntry("yErr", Y_ERR, Schema::FLOAT, 1, "pixel"));
    }
    
    /// Return the x-centroid
    double getX() const {
        return Measurement<Astrometry>::get<Astrometry::X, double>();
    }
    /// Return the error in the x-centroid
    float getXErr() const {
        return Measurement<Astrometry>::get<Astrometry::X_ERR, float>();
    }
    /// Return the y-centroid
    double getY() const {
        return Measurement<Astrometry>::get<Astrometry::Y, double>();
    }
    /// Return the error in the y-centroid
    float getYErr() const {
        return Measurement<Astrometry>::get<Astrometry::Y_ERR, float>();
    }

    virtual std::ostream &output(std::ostream &os) const {
        return os << "(" << getX() << "+-" << getXErr() << ", " << getY() << "+-" << getYErr() << ")";
    }
};

/**
 * A class that actually does the work of measuring things
 *
 * More precisely, a base class whose subclasses do the real work.  The main purpose of MeasureAstrometry is
 * to provide the FactoryPtr typedef, and also to hide MeasureQuantity<Astrometry::Ptr, Image>. Note that
 * MeasureQuantity knows nothing about Image classes
 */
class MeasureAstrometry : public MeasureQuantity<Astrometry::Ptr, Image>
{
public:
    typedef boost::shared_ptr<MeasureAstrometry> Ptr;
    typedef MeasureQuantity<Astrometry::Ptr, Image> *FactoryPtr;

    MeasureAstrometry() {}
};

/************************************************************************************************************/
/*
 * Define a macro to provide all the per-algorithm boilerplate
 */
#define ASTROMETRY_BOILERPLATE(NAME, ALG) \
    MEASUREMENT_BOILERPLATE(Astrometry, NAME, ALG, (Image const& im, Peak const& peak), (im, peak))

#endif
