// -*- lsst-c++ -*-
#include "Photometry.h"
#include "MeasurementData.h"

namespace {                             // N.b. none of this implementation need be globally visible
/************************************************************************************************************/
/**
 * Implement model fit photometry.  We include the sersic index in the schema, but don't provide
 * an accessor function
 */
class ModelPhotometry : public PhotometryImpl<ModelPhotometry>
{
    /// We need a new, unused, index to save the Sersic index in.  [0, Photometry::NVALUE) are already taken
    enum { SERSIC_N=Photometry::NVALUE, NVALUE };
public:
    typedef boost::shared_ptr<ModelPhotometry> Ptr;
    typedef boost::shared_ptr<ModelPhotometry const> ConstPtr;

    /// Create a ModelPhotometry to record our measurements
    ModelPhotometry(double flux, float fluxErr=-1) {
        init();                         // This allocates space for fields added by defineSchema
        set<FLUX>(flux);                // ... if you don't, these set calls will fail an assertion
        set<FLUX_ERR>(fluxErr);         // the type of the value must match the schema
        set<SERSIC_N>(4);
    }

    /// Add desired fields to the schema
    static void defineSchema(Schema::Ptr schema ///< our schema; == ModelPhotometry::_mySchema
                      ) {
        Photometry::defineSchema(schema);
        schema->add(SchemaEntry("sersic_n", SERSIC_N, Schema::INT));
    }

    /// Virtual function called by operator<< to dynamically dispatch the type to a stream
    std::ostream &output(std::ostream &os ///< the output stream
                        ) const {
        os << "n_s: " << get("sersic_n") << "  ";
        return Photometry::output(os);
    }
};

PHOTOMETRY_BOILERPLATE("model", Model)

/************************************************************************************************************/
/**
 * Process the image; calculate values
 */
Photometry::Ptr ModelMeasurePhotometry::doMeasure(Image const& im, Peak const&) {
    // Burn CPU time here
    return boost::make_shared<ModelPhotometry>(2*im, 0.2);
}
}

