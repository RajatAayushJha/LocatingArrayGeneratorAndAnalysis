library('inline')
library('Rcpp')

#' @param la_path Path to locating array TSV.

buildLA <- function(la_path){
  if(missing(la_path)){ 
    stop("Please supply a locating array file.")
  }

  args <- commandArgs(trailingOnly = TRUE)
  if (length(args) < 1 || args[1] == "--help") {
    # Call the print_usage function
    print_usage()
    # Optionally, halt further execution
    quit("no")
  }

  ##load parser module 
  parser_module <- Module("Parser_module")
  Parser <- Parser_module$Parser
  (parser_ptr <- new(Parser, len(args),args))
  status <- parser_ptr$process_input()

  if (status==-1){
    return(1)
  }

  array_module <- Module("Array_module")
  Array <- array_module$Array
  (array_ptr <- new(Array,parser_ptr))
  score = array_ptr$getScore()

  if (score==0){
    cat("Nothing to do\n\n")
    return(1)
  }

  for (row in parser_ptr$getArray){
    array_ptr$add_row(row)
  }

  array_ptr$print_stats(TRUE)
  if (array_ptr$getScore()==0){
    return(1)
  }

  prev_score <- 0
  no_change_counter <- 0

  while(array_ptr$getScore()>0){
    prev_score <- array_ptr$getScore()
    array_ptr$add_row()
    if (array_ptr$getOut_of_Memory) break
    if (array_ptr$getScore == prev_score) no_change_counter <- no_change_counter+1
    else no_change_counter <- 0
    if (no_change_counter > 10) break
    array_ptr$print_stats();        
    }
  return (printResults_wrapper(parser_ptr, array_ptr, (no_change_counter == 0 || array_ptr$getOut_of_Memory)))
  
}


print_usage <- function() {
  cat("usage: ./generate [flags] (<t> | <d> <t> | <d> <t> <δ>) <input file> [output file]\n")
  cat("flags (single hyphens can be combined):\n")
  cat("\t-d          : debug mode (prints extra state information while running)\n")
  cat("\t-h          : halfway mode (prints less output than normal)\n")
  cat("\t-s          : silent mode (prints no output, cancels other output flags)\n")
  cat("\t-v          : verbose mode (prints more output than normal)\n")
  cat("\t--partial   : use partially complete array; a filepath must follow this flag\n")
  cat("\t--help      : print help message (what you are seeing here)\n")
  cat("arguments (assume order matters):\n")
  cat("\tt           : strength of interactions, needed for all types of arrays\n")
  cat("\td           : size of sets of interactions, needed for locating and detecting arrays\n")
  cat("\tδ           : separation of interactions from other sets, needed for detecting arrays\n")
  cat("\tinput file  : file containing array parameter info\n")
  cat("\toutput file : file in which to print finished array (if not specified, stdout is used)\n")
  cat("for more details, please refer to the README, or visit https://github.com/gatoflaco/Array-Generator\n")
  return(0)
}

