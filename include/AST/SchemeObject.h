#ifndef SCHEME_OBJECT_H
#define SCHEME_OBJECT_H

#include <memory>

namespace Scheme {
    class SchemeObject {
    public:
        enum ObjectTy {
            FIXNUM_TY = 0,
            BOOLEAN_TY,
            CHARACTER_TY,
            STRING_TY,
            EMPTYLIST_TY,
            PAIR_TY,
            SYMBOL_TY,
            QUOTE_TY,
            DEFINE_TY,
            REDEFINE_TY,
            IF_TY,
            PROC_TY,
        };

        SchemeObject(ObjectTy type);

        virtual ~SchemeObject();

        ObjectTy getType() const;

        bool isFixnum() const;

        bool isBoolean() const;

        bool isCharacter() const;

        bool isString() const;

        bool isEmptyList() const;

        bool isPair() const;

        bool isSymbol() const;

        bool isQuote() const;

        bool isDefinition() const;

        bool isRedefinition() const;

        bool isIf() const;

        bool isProcedure() const;

    private:
        ObjectTy type_; // type of this object
    };


    typedef std::shared_ptr<SchemeObject> SchemeObjectPtr;
} // namespace Scheme

#endif
