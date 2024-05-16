// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

#ifdef RCPP_USE_GLOBAL_ROSTREAM
Rcpp::Rostream<true>&  Rcpp::Rcout = Rcpp::Rcpp_cout_get();
Rcpp::Rostream<false>& Rcpp::Rcerr = Rcpp::Rcpp_cerr_get();
#endif

// array_array2
SEXP array_array2(SEXP in);
RcppExport SEXP _LABuilder_array_array2(SEXP inSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< SEXP >::type in(inSEXP);
    rcpp_result_gen = Rcpp::wrap(array_array2(in));
    return rcpp_result_gen;
END_RCPP
}
// printResults_wrapper
RcppExport SEXP printResults_wrapper(SEXP p_, SEXP array_, SEXP success_);
RcppExport SEXP _LABuilder_printResults_wrapper(SEXP p_SEXP, SEXP array_SEXP, SEXP success_SEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< SEXP >::type p_(p_SEXP);
    Rcpp::traits::input_parameter< SEXP >::type array_(array_SEXP);
    Rcpp::traits::input_parameter< SEXP >::type success_(success_SEXP);
    rcpp_result_gen = Rcpp::wrap(printResults_wrapper(p_, array_, success_));
    return rcpp_result_gen;
END_RCPP
}
// rcpp_hello_world
List rcpp_hello_world();
RcppExport SEXP _LABuilder_rcpp_hello_world() {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    rcpp_result_gen = Rcpp::wrap(rcpp_hello_world());
    return rcpp_result_gen;
END_RCPP
}

RcppExport SEXP _rcpp_module_boot_Parser_module();
RcppExport SEXP _rcpp_module_boot_Array_module();

static const R_CallMethodDef CallEntries[] = {
    {"_LABuilder_array_array2", (DL_FUNC) &_LABuilder_array_array2, 1},
    {"_LABuilder_printResults_wrapper", (DL_FUNC) &_LABuilder_printResults_wrapper, 3},
    {"_LABuilder_rcpp_hello_world", (DL_FUNC) &_LABuilder_rcpp_hello_world, 0},
    {"_rcpp_module_boot_Parser_module", (DL_FUNC) &_rcpp_module_boot_Parser_module, 0},
    {"_rcpp_module_boot_Array_module", (DL_FUNC) &_rcpp_module_boot_Array_module, 0},
    {NULL, NULL, 0}
};

RcppExport void R_init_LABuilder(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
