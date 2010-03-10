#if !defined(ASTROMETRY_H)
#define ASTROMETRY_H 1

#include "Measurement.h"
#include "Image.h"
#include "Peak.h"

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
 * Here's the object that remembers and can execute our choice of astrometric algorithms
 */
typedef MeasureQuantity<Astrometry::Ptr, Image, Peak>  MeasureAstrometry;

#endif
