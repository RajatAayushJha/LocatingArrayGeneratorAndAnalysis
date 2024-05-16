# R Package Documentation :  [ LocatingArrayGeneratorAndAnalysis ]

# Overview

“LocatingArrayGeneratorAndAnalysis” is a R package designed for constructing and analyzing locating arrays. It provides a comprehensive suite of functionalities for constructing locating arrays from a description of test factors, building models, and analyzing locating arrays using a valid locating array file, factor data file, and responses directory.

# Dependencies

[LocatingArrayGeneratorAndAnalysis] package depends on the following R packages :- 

LATools
Rcpp
LABuilder

Make sure to install these dependencies before using [LocatingArrayGeneratorAndAnalysis].

Installation

RStudio Version 2021.09.0 Build 351

This package is intended to be used in RStudio. To install, first install devtools.
```R
install.packages("devtools")
```
Next, load devtools and use it to install this package.
```R
library(devtools)
install_github("/rajataayushjha/LocatingArrayGeneratorAndAnalysis")
```

# Usage

Here is a basic example demonstrating how to use the [LocatingArrayGeneratorAndAnalysis] package:



To construct Locating Array:
```R
generateLA(d,t,𝛿,input_file.tsv,output_file.tsv) 
```
If values of d,t,𝛿 are not provided, it will assume d=1,t=2,𝛿=1. input_file.tsv should be given everytime while executing.
To analyze the Locating Array:
```R
buildModels((locatingArray_path, factorData_path, response_path, response_column , logit ))
```

# C++ Backend

The backend of [LocatingArrayGeneratorAndAnalysis] is implemented in C++ for efficiency and performance. Here's an overview of the logic of the C++ implementation:

# Construction

There are two ways of constructing the Locating Arrays. 

Method-1: (Work done by Isaac Jung - link to C++ code is attached)
Here, the program begins by getting input from the command line. After doing basic syntax parsing and semantic error checking, it passes the information from input to an Array object constructor, which organizes the data in the array into groups of vectors and sets. When this is done, the main program simply calls Array methods on the instantiated object to perform each check requested. If any of the less strict checks fail, by definition the more strict ones will too, so the program doesn't bother computing them and jumps to a conclusion (unless verbose mode is enabled, in which case it will compute everything requested regardless). 
The Array constructor creates Factor objects representing the columns in the array. It checks how many levels each Factor has, and for each factor, it instantiates Single objects for every value that factor can take on, in an array of pointers. 

A Single object consists of the (factor, value) pair along with the rows in which it occurs. At instantiation, a Single object has an empty set of rows in which it occurs. After all of the Single object pointer arrays are created for every Factor, the Array constructor begins actually processing the Parser object's array. 

For every (row, column) pair, it uses a map to find the associated Single object and update its set of rows in which it occurs to include the newly found row. At the end, the Array has access to every (factor, value) that could be present based on the provided array and the levels of each column, as well as the sets of rows in which those (factor, value) Singles occur. The Array constructor then uses a somewhat complicated recursive method to construct all possible t-way Interactions among those Singles, simultaneously filling out the rows in which those Interactions occur (defined as the intersection of the sets of rows in which all of the Singles involved occur). 

Whereas the Singles were stuck in pointer arrays associated with each Factor (because an Interaction such as {(0, 0), (0, 1)} should not be possible; interactions must be between different factors), all of the Interaction objects have their pointers added to a single vector; if size-d sets of Interactions are later needed, no distinct categories of Interactions are needed to build them. After all possible Interaction pointers have been added to the vector, the Array object has enough information to compute strength t coverage. Next, the Array constructor checks if location or detection were requested (via the Parser object's fields). 

If so, it also must build all possible size-d sets of the t-way interactions. Note that the name of this class is simply T. Building all possible Ts requires a method very similar to building all possible Interactions. It is slightly simpler due to the linear nature of the vector containing all Interaction pointers. As size-d sets are created, they are inserted into a set; conceptually, the order does not matter. At the end of this step, the Array object has enough information to compute location and detection. If neither was requested, this entire phase is skipped, saving computation time. 

For checking t-coverage, the is_covering() method is called by the main program. The method iterates through all Interactions and simply ensures that the magnitude of the set of rows associated with each Interaction is at least t. If it is not, the issue is logged for the user to see which Interaction does not appear enough times (the program could detect a coverage issue earlier if a Single itself did not have an adequate-size set of rows, but the design decision was made to not check for that). As soon as a single issue is discovered, the Array by definition lacks t-coverage. Therefore, when certain flags are set to avoid extra work, this method will immediately return false in this situation. Otherwise, it will simply set a boolean and continue reporting any other issues it finds, waiting to return false till the end. If all Interactions satisfy t-coverage, the method returns true. 

For checking (d, t)-location, the is_locating() method is called by the main program. The method considers all pairs of Ts and ensures that no two Ts share the same set of rows in which they occur. If any two distinct Ts have the same set of rows, it is reported as an issue to the user. Just like with the coverage check, a single issue means the entire method should return false, and setting certain flags can make the method return false upon encountering any issue at all. Otherwise, the method reports all issues discovered. If all pairs of distinct Ts satisfy (d, t)-location, the method returns true.

 For checking (d, t, δ)-detection, the is_detecting() method is called by the main program. The method iterates over all Interactions, and for each Interaction, it iterates over all Ts. Then, for a given (Interaction, T) pair, the method ensures that either (a) the Interaction is a member of T, or (b) the set difference between the Interaction's rows and the T's rows is at least δ in magnitude. If the separation between a given Interaction and T is less than δ, the user is informed of the issue, and as with the previous methods, a boolean is set to false. Also like the previous methods, it is possible to immediately return false upon discovering a single issue, when certain flags are set. If the method finishes iterating and finds no (d, t, δ)-detection issues, the method returns true. During a check for detection, the Array also updates a true_delta field, defined simply as the minimum separation between all (Interaction, T) pairs. This is useful for letting the user know that there may be an even higher separation than they intended.

Method-2: ( Work done by Stephen Seidel - link to C++ code is attached)

 Here, the exactFix() method, which is a part of a class named CSMatrix that constructs locating arrays. It initializes a work array of type CSCol pointers with a size equal to the number of columns in the matrix. It copies column pointers from the data structure of the matrix into this work array. It sorts the work array using the smartSort() function, which sorts the array based on a criteria that ensures that columns are sorted in a way that maintains the order of rows as much as possible while considering the already sorted rows. This sorting operation is crucial for arranging the columns effectively to generate locating arrays. It calculates a score for the current arrangement of columns using the getArrayScore() function. This score represents the effectiveness of the locating array. The higher the score, the better the locating array. If there are no constraints on the locating array, it enters a loop to improve the locating array by adding rows until the score is reduced to zero. It prints the score and the number of rows after each iteration.
If there are constraints on the locating array, it prints a message indicating that it's unable to perform the fixing operation due to the presence of constraints. It deallocates memory for the work array to prevent memory leaks.


# Analysis

The procedure for analysis is as follows :- 

Priority queue begins with 1 model: the model with no terms but an intercept. At every iteration, the models are pulled out of the queue, one by one, and for each, the top (n) terms are taken, one at a time, based on the distance to residuals. For every term, it is added to the current model and r-squared is calculated. The new model is then placed in the next priority queue. This process stops when all models have as many terms as maxTerms. Each model taken out of the queue produces newModels_n new models that are added to the next queue. The queues are then priority queues (by model R^2) with a maximum number of models. The same model (with the same terms) can be generated in multiple ways, and in this case, duplicates will not be added and the function below prints "Duplicate Model!!!".


# Definition of Fundamental Keywords

Coverage: A covering array A of strength t is one in which every interaction T of strength t occurs at least once. It could be noted that for a (d, t, δ)-detecting array, every interaction of strength t must actually be covered at least δ times. The coverage property is a baseline requirement for a test suite which can identify the presence of interaction faults.
Detection: A (d, t)-detecting array A of strength t can identify a set of d faulty t-way interactions when the set of candidate faults in A is at most d. The way that this is ensured is by checking that for every possible pair of t-way interactions T with size-d sets of t-way interactions 𝒯, if ρ(A, T) (the set of rows in A in which T occurs) is a subset of ρ(A, 𝒯) (the set of rows in which 𝒯 occurs), T muat be a member of 𝒯. 
Interaction: An interaction is the failure of a system to produce the same effect at different levels of another factor. A t-way interaction is a grouping of t factors that have been assigned specific levels, and is often associated with a test in an array. Such an interaction is also said to have strength t. For example, if a test over the four factors {a, b, c, d} assigns {a1b0c2d0}, then one of the 1-way interactions in this test is a1, another is b0, and so on. Some of the 2-way interactions in the test include a1b0, a1c2, b0d0, and c2d0. In reality, interactions among varying numbers of factors with different levels may be responsible for a given failing test. 
Location: A (d, t)-locating array A of strength t is able to locate any size d set of t-way interactions in A. Somewhat formally, this means that if every possible set 𝒯 is formed from from d pairs of t-way interactions in A, no two pairs of 𝒯 will share the same set of rows in which they occur. Note that the set of rows in which a 𝒯 occurs, ρ(A, 𝒯), is simply the union of all the sets of rows in which the t-way interactions it contains occur. The location property is a step up from the covering property, and can reveal the location of t-way interaction faults. To quantify the degree to which level-wise t-way interactions can be distinguished in an array, the separation between sets of runs for different sets of level-wise t-way interactions is introduced. A (d, t, 𝛿)-locating array guarantees that any two sets of d level-wise t-way interactions are separated by at least 𝛿 runs. By definition, a locating array has a separation of at least one. A locating array with larger 𝛿 is more robust to, for example, outliers or missing data; however, there is a tradeoff between large 𝛿 and small array size.
Factor: A parameter in a complex engineered system. Every factor has a range of values it can be assigned; these are called the factor's levels, and are restricted to integers. If a factor is represented by the letter a, and it can take on 6 levels, 0-5, then the notation a2 would represent the factor a with the value 2 assigned to it. Because factors directly correspond to columns in an n x m array A, an element in A represents an assignment to some factor, often denoted in the form (factor, value).
Strength: The strength is the number of (factor, level)s involved in the interaction. For an interaction to have strength ‘t’ is synonymous with calling it a t-way interaction.

# Datasets that can be used with the Tool

Wireless Conferencing Experiments with BEOF
Screening Experiments with REACT
Chemical reactor experiment : Box, Hunter, and Hunter (2005) describe a chemical reactor experiment with five binary factors A–E. The experiment is run as a 25 full-factorial, and the original analysis indicates that B, D, E, B x D, and D x E are active.
Rubber experiment : 
A (1,2,1)-locating array including runs 1-11,13,15-17,22-25,and 28 from the data set presented in the Appendix of (Sundberg, 2008). It contains the 14-run half-fraction supersaturated design described in (Lin, 1993). 
A  (1,2,1)-locating  array  including  runs  5,7,and  10-27  from  the  data  set presented in the Appendix of (Sundberg, 2008). It contains the 14-run half-fraction supersaturated design described in (Wang et al., 1995).

Wireless network test-bed experiment : Seidel, Mehari, et al. (2018)


# Reference

Yasmeen Akhtar, Fan Zhang, Charles J. Colbourn, John Stufken & Violet R.
Syrotiuk (07 Jul 2023): Scalable level-wise screening experiments using locating arrays, Journal of Quality Technology, DOI: 10.1080/00224065.2023.2220973
Colbourn, C. J., and D. W. McClary. 2008. Locating and detecting arrays for interaction faults. Journal of Combinatorial Optimization 15 (1):17–48. doi: 10.1007/s10878-007-9082-4.
Box, G. E. P., J. S. Hunter, and W. G. Hunter. 2005. Statistics for experimenters. 2nd ed. Hoboken, NJ: John Wiley & Sons, Inc.
Sundberg, R. 2008. A classical dataset from Williams, and its role in the study of supersaturated designs. Journal of Chemometrics 22 (7):436–40. doi: 10.1002/cem.1167.
Lin, D. K. J. 1993. A new class of supersaturated designs. Technometrics 35 (1):28–31. doi: 10.1080/00401706.1993. 10484990.
Wang, P. C., N. Kettaneh-Wold, and D. K. J. Lin. 1995. Comments on Lin (1993). Technometrics 37 (3):358–9. doi: 10.2307/1269944.
Seidel, S. A., M. T. Mehari, C. J. Colbourn, E. De Poorter, I. Moerman, and V. R. Syrotiuk. 2018. Analysis of largescale experimental data from wireless networks. In IEEE INFOCOM 2018 - IEEE Conference on Computer Communications Workshops (INFOCOM WKSHPS), 535– 540, April.


# Acknowledgements

Dr. Violet Syrotiuk - https://www.public.asu.edu/~syrotiuk/index.html

Dr. Charles Colbourn - https://www.public.asu.edu/~ccolbou/ 

Stephen Seidel - https://github.com/syrotiuk/sseidel-la-tools 

Isaac Jung - https://github.com/gatoflaco/Array-Checker 

Leah Darwin - https://github.com/leahdarwin/LATools 




