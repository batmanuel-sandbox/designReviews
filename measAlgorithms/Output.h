#if !defined(OUTPUT_H)
#define OUTPUT_H 1

#include <fstream>
#include "Measurement.h"
#include "Source.h"

template<typename T>
void showFromSchema(Measurement<T> const& v)
{
    for (typename Measurement<T>::const_iterator vptr = v.begin(); vptr != v.end(); ++vptr) {
        Schema const& sch = *(*vptr)->getSchema();

        for (Schema::const_iterator sptr = sch.begin(); sptr != sch.end(); ++sptr) {
            Schema const& se = *(*sptr);

            std::cout << sch.getComponent() << "." << se.getName() << " \t:    " << se.getType() << "\t|   ";
            if (se.isArray()) {
                for (int i = 0; i != se.getDimen(); ++i) {
                    std::cout << (se.getType() == Schema::INT ?
                                  (*vptr)->getAsLong(i, **sptr) : (*vptr)->get(i, **sptr));
                    if (i + 1 != se.getDimen()) {
                        std::cout << " ";
                    }
                }
            } else {
                std::cout << (se.getType() == Schema::INT ? (*vptr)->getAsLong(**sptr) : (*vptr)->get(**sptr));
            }
            if (se.getUnits() != "") {
                std::cout << "\t" << se.getUnits();
            }
            std::cout << std::endl;
        }
    }
}

void showFromSchema(Source const& s)
{
    showFromSchema(s.getAstrometry());
    showFromSchema(s.getPhotometry());
}

/************************************************************************************************************/

namespace {
    template<typename M>
    struct writeHeader {
        writeHeader(std::string const& comp, Schema const& se, M const&) : _comp(comp), _se(se) {}
        std::string operator()(int i=-1) {
            std::ostringstream ss;
            ss << _comp << "." << _se.getName();
            if (i >= 0) {
                ss << i;
            }
            return ss.str();
        }

        std::string const& _comp;
        Schema const& _se;
    };

    template<typename M>
    struct writeUnits {
        writeUnits(std::string const&, Schema const& se, M const&) : _se(se) {}
        std::string operator()(int= -1) {
            return _se.getUnits();
        }
        Schema const& _se;
    };

    template<typename M>
    struct writeData {
        writeData(std::string const&, Schema const& se, M const& m) : _se(se), _m(m) {}
        std::string operator()(int i=-1) {
            std::ostringstream ss;
            if (i >= 0) {
                ss << (_se.getType() == Schema::INT ? _m.getAsLong(i, _se) : _m.get(i, _se));
            } else {
                ss << (_se.getType() == Schema::INT ? _m.getAsLong(_se) : _m.get(_se));
            }

            return ss.str();
        }

        Schema const& _se;
        M const& _m;
    };
}

template<typename outputMethod, typename M>
void writeCsv(
              std::ostream &fd,
              M const& v
             )
{
    for (typename M::const_iterator vptr = v.begin(); vptr != v.end(); ++vptr) {
        Schema const& sch = *(*vptr)->getSchema();

        if (vptr != v.begin()) {
            fd << ", ";
        }
        for (Schema::const_iterator sptr = sch.begin(); sptr != sch.end(); ++sptr) {
            Schema const& se = *(*sptr);
            outputMethod output(sch.getComponent(), se, **vptr);

            if (se.isArray()) {
                for (int i = 0; i != se.getDimen(); ++i) {
                    fd << output(i);
                    if (i != se.getDimen() - 1) {
                        fd << ", ";
                    }
                }
            } else {
                fd << output();
            }

            if (sptr != sch.end() - 1) {
                fd << ", ";
            }
        }
    }
}

void writeCsv(std::vector<Source::Ptr> values,
              std::string const& filename=""
             )
{
    if (values.begin() == values.end()) {
        return;
    }

    std::ofstream fs;
    std::ostream &fd = (filename == "") ? std::cout : fs;
    if (filename != "") {
        fs.open(filename.c_str());
    }

    {
        Source::ConstPtr first = *(values.begin());
        writeCsv<writeHeader<Measurement<Astrometry> > >(fd, first->getAstrometry());
        fd << ", ";
        writeCsv<writeHeader<Measurement<Photometry> > >(fd, first->getPhotometry());
        fd << std::endl;

        writeCsv<writeUnits<Measurement<Astrometry> > >(fd, first->getAstrometry());
        fd << ", ";
        writeCsv<writeUnits<Measurement<Photometry> > >(fd, first->getPhotometry());
        fd << std::endl;
    }
    
    for (std::vector<Source::Ptr>::iterator ptr = values.begin(); ptr != values.end(); ++ptr) {

        writeCsv<writeData<Measurement<Astrometry> > >(fd, (*ptr)->getAstrometry());
        fd << ", ";
        writeCsv<writeData<Measurement<Photometry> > >(fd, (*ptr)->getPhotometry());
        fd << std::endl;
    }
}

#endif
