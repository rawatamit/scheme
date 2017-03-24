#ifndef SCHEME_OBJECT_H
#define SCHEME_OBJECT_H

#include <memory>

namespace Scheme {
    class SchemeObject; // forward decl for SchemeObject
    typedef std::shared_ptr<SchemeObject> SchemeObjectPtr;

    class SchemeObject {
    public:
        // some useful symbols
        static SchemeObjectPtr quote_symbol;
        static SchemeObjectPtr if_symbol;
        static SchemeObjectPtr lambda_symbol;
        static SchemeObjectPtr ok_symbol;
        static SchemeObjectPtr begin_symbol;
        static SchemeObjectPtr else_symbol;
        static SchemeObjectPtr eof_symbol;

        enum ObjectTy {
            FIXNUM_TY = 0,
            BOOLEAN_TY,
            CHARACTER_TY,
            STRING_TY,
            EMPTYLIST_TY,
            PAIR_TY,
            SYMBOL_TY,
            PROC_TY,
            COMP_PROC_TY,
            ENV_TY,
            INPUT_PORT_TY,
            OUTPUT_PORT_TY,
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

        bool isProcedure() const;

        bool isPrimitiveProcedure() const;

        bool isCompoundProcedure() const;

        bool isEnvironment() const;

        bool isInputPort() const;

        bool isOutputPort() const;

    private:
        ObjectTy type_; // type of this object
    };
} // namespace Scheme

#endif
