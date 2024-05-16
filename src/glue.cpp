#include<RcppCommon.h>
#include <Rcpp.h>
#include "array.h"
#include "factor.h"
#include "parser.h"

using namespace std;
using namespace Rcpp;


//make the exposed class visible 
RCPP_EXPOSED_CLASS(Parser);
RCPP_EXPOSED_CLASS(Array);


Parser* parse(int32_t argc, const std::vector<std::string>& argv){
  Parser* p = new Parser(argc, argv);
  return p;
}

Array* array_array(Parser* p){
  Array*  ar= new Array(p);
  return ar;
}


RCPP_MODULE(Parser_module){
  class_<Parser>("Parser")
  .constructor<int32_t, const std::vector<std::string>>()  // Constructor takes int32_t and char** arguments
  .factory<int32_t, const std::vector<std::string>&>(parse)  
  .method("process_input", &Parser::process_input)
  .method("get_d", &Parser::get_d)
  .method("get_t", &Parser::get_t)
  .method("get_delta", &Parser::get_delta)
  .method("getArray",&Parser::getArray);
}

RCPP_MODULE(Array_module){
  class_<Array>("Array")
  .constructor<Parser*>()  
  .factory(array_array)  
  .method("getScore", &Array::getScore)
  .method("print_stats",&Array::print_stats)
  // Expose the add_row method taking uint16_t* as "add_row_uint16"
  .method("add_row_uint16", static_cast<void (Array::*)(uint16_t*)>(&Array::add_row))
  // Expose the add_row method with no arguments as "add_row_no_args"
  .method("add_row_no_args", static_cast<void (Array::*)()>(&Array::add_row))
  .method("getOut_of_Memory",&Array::getOut_of_Memory);
}

