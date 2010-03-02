#if !defined(TABLE_H)
#define TABLE_H 1

template<typename T>
void writeTable(Measurement<T> const& v)
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

#endif
