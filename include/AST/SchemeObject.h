#ifndef SCHEME_OBJECT_H
#define SCHEME_OBJECT_H

#include <memory>

namespace Scheme {
    class SchemeObject; // forward decl for SchemeObject
    typedef std::shared_ptr<SchemeObject> SchemeObjectPtr;

    class SchemeObject {
    public:
        // some useful symbols
        static SchemeObjectPtr lambda_symbol;
        static SchemeObjectPtr ok_symbol;
        static SchemeObjectPtr begin_symbol;
        static SchemeObjectPtr else_symbol;

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
            COMP_PROC_TY,
            ENV_TY,
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

        bool isPrimitiveProcedure() const;

        bool isCompoundProcedure() const;

        bool isEnvironment() const;

    private:
        ObjectTy type_; // type of this object
    };
} // namespace Scheme

#endif
