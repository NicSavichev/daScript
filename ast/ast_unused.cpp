#include "precomp.h"

#include "ast.h"

namespace yzg {
    
    /*
     TODO:
        cond ? a : b
        a ?? b
        ?.
     */
    
    // here we propagate r2cr flag
    //  a.@b    ->  $a.@b
    //  a@[b]   ->  $a@[b]
    //  a.b.@c  ->  $a.$b.@c    
    class TrackFieldAndAtFlags : public Visitor {
        void propagate ( Expression * expr ) {
            if ( expr->rtti_isVar() ) {
                auto var = (ExprVar *) expr;
                var->r2cr = true;
            } else if ( expr->rtti_isField() ) {
                auto field = (ExprField *) expr;
                field->r2cr = true;
                propagate(field->value.get());
            } else if ( expr->rtti_isAt() ) {
                auto at = (ExprAt *) expr;
                at->r2cr = true;
                propagate(at->subexpr.get());
            }
        }
    protected:
        // field
        virtual void preVisit ( ExprField * expr ) override {
            Visitor::preVisit(expr);
            if ( expr->r2v ) propagate(expr->value.get());
        }
        // at
        virtual void preVisit ( ExprAt * expr ) override {
            Visitor::preVisit(expr);
            if ( expr->r2v ) propagate(expr->subexpr.get());
        }
    };
    
    class TrackVariableFlags : public Visitor {
    protected:
    // global let
        virtual void preVisitGlobalLet ( const VariablePtr & var ) override {
            Visitor::preVisitGlobalLet(var);
            var->access_init = false;
            var->access_get = false;
            var->access_ref = false;
        }
        virtual void preVisitGlobalLetInit ( const VariablePtr & var, Expression * init ) override {
            Visitor::preVisitGlobalLetInit(var, init);
            var->access_init = true;
        }
    // let
        virtual void preVisitLet ( ExprLet * let, const VariablePtr & var, bool last ) override {
            Visitor::preVisitLet(let, var, last);
            var->access_init = false;
            var->access_get = false;
            var->access_ref = false;
        }
        virtual void preVisitLetInit ( ExprLet * let, const VariablePtr & var, Expression * init ) override {
            Visitor::preVisitLetInit(let, var, init);
            var->access_init = true;
        }
    // function arguments
        virtual void preVisitArgument ( Function * fn, const VariablePtr & var, bool lastArg ) override {
            Visitor::preVisitArgument(fn, var, lastArg);
            var->access_init = false;
            var->access_get = false;
            var->access_ref = false;
        }
        virtual void preVisitArgumentInit ( Function * fn, const VariablePtr & var, Expression * init ) override {
            Visitor::preVisitArgumentInit(fn, var, init);
            var->access_init = true;
        }
    // var
        virtual void preVisit ( ExprVar * expr ) override {
            Visitor::preVisit(expr);
            if ( expr->r2v || expr->r2cr ) {
                expr->variable->access_get = true;
            } else {
                expr->variable->access_ref = true;
            }
        }
    };
    
    class RemoveUnusedLocalVariables : public Visitor {
    public:
        bool didAnything () const { return anyFolding; }
    protected:
        bool        anyFolding = false;
    protected:
        virtual VariablePtr visitLet ( ExprLet * let, const VariablePtr & var, bool last ) override {
            if ( !var->access_get && !var->access_ref && !var->access_init ) {
                anyFolding = true;
                return nullptr;
            }
            return Visitor::visitLet(let,var,last);
        }
    };
    
    // program
    
    bool Program::optimizationUnused() {
        TrackFieldAndAtFlags faf;
        visit(faf);
        TrackVariableFlags vaf;
        visit(vaf);
        RemoveUnusedLocalVariables context;
        visit(context);
        return context.didAnything();
    }
}

