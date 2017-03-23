cmake_minimum_required(VERSION 2.8.12)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(AST
            SchemeObject.cc
            Fixnum.cc Boolean.cc
            Character.cc String.cc
            Pair.cc Symbol.cc
            If.cc Quote.cc Definition.cc Redefinition.cc
            Procedure.cc CompoundProcedure.cc CompoundProcedure.cc
            InputPort.cc OutputPort.cc)
