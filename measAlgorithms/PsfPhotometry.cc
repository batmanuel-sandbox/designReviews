// -*- lsst-c++ -*-
#include "Photometry.h"

/**
 * Implement PSF photometry.  The astronomical details are left to the reader
 */
class PsfPhotometry : public Photometry
{
public:
    typedef boost::shared_ptr<PsfPhotometry> Ptr;
    typedef boost::shared_ptr<PsfPhotometry const> ConstPtr;

    /// Ctor
    PsfPhotometry(double flux, float fluxErr=-1) {
        init();                         // This allocates space for fields added by defineSchema
        set<FLUX>(flux);                // ... if you don't, these set calls will fail an assertion
        set<FLUX_ERR>(fluxErr);         // the type of the value must match the schema
    }

    /// Add desired fields to the schema
    virtual void defineSchema(Schema::Ptr schema ///< our schema; == _mySchema
                     ) {
        Photometry::defineSchema(schema);
    }
};

PHOTOMETRY_BOILERPLATE("psf", Psf)

/**
 * Process the image; calculate values
 */
Photometry::Ptr PsfMeasurePhotometry::doMeasure(Image const& im, Peak const&) {
    // Here is the real work, hiding in a comment
    return boost::make_shared<PsfPhotometry>(3*im);
}
