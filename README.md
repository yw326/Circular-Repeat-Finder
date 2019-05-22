# MicroDNA_Detection

The goal of this project is to provide an efficient algorithm to locate potential microDNA reintegration pairs within DNA sequences. Each potential microDNA is mathematically defined as a circle repeat, with form "...s1s2...s2s1..." (direct circle repeat), or "...s1s2...s1's2'..." (reversed circle repeat), where s' denotes the reversed complement of s. The program takes a sequence as input, and output two text files that contains the circle repeats sequences found by the algorithm and as well as their locations in the sequence. The detailed description of the algorithm can be found in the pdf file (to be uploaded).

The suffix tree construction code is based on https://github.com/shysaur/shysaur-suffixtrees.

## Getting Started

Enter the project directory, and generate the executable from code by using 
```
make
```
You should be getting an executable called mccreight.


## Command Line Options

The executable has 2 required argument and 2 optional arguments:
* First argument (required):  sequence file name. The file should contain the DNA sequence in which you search the circle repeats; note that any characters that are not A, T, C, G are filtered out in the preprocessing.
* Second argument (required): either "direct" or "reversed". If "direct", then the program will search direct circle repeats (of form "...s1s2...s2s1..."); if "reversed", the program will search reversed circle repeats (of form "...s1s2...s1^(-1)s2^(-1)...").
* Optional argument: "-r minimum_maximal_repeat_length". Default value of minimum maximal repeat length is 40.
* Optional argument: "-m percentage_mismatch_allowed". Default value of percentage mismatch is 0.1

Broadly speaking, minimum maximal repeat length and minimum extension length can be thought of as the minimum length of s1 and s2. The mismatch ratio of a circle repeat pair is defined as  (edit_dist(first s1, second s1) + edit_dist(first s2, second s2))/(len(s1s2)). See https://en.wikipedia.org/wiki/Levenshtein_distance for more info on edit distance.

## Example
```
./mccreight test.txt direct -r 40 -m 0.1
```

The above command searches for direct circle repeats in the sequence from file test.txt, with minimum maximal repeat length 40, and allowed mismatch ration 10%.

## Output Files
There will be 2 output files: one sequence files and one index files. Recall that the circle repeat is either of form "...s1s2...s2s1..." (direct circle repeat) or "...s1s2...s1^(-1)s2^(-1)..." (reversed circle repeat). The file labeled with "1st type" contains the circle repeat found with s1 being maximal; the file labeled "2nd type" contains the circle repeat found with s2 being maximal.

The files whose names end with "seq" contains the string of s1s2, s1, s2 for each circle repeat.

The files whose names end with "circle repeat" contains the index info for each circle repeat, represented by 4-tuple (p1,p2,l1,l2).
* p1: the start index of s1s2
* p2: the start index of s2s1 (or s1^(-1)s2^(-1) if reverse)
* l1: the length of s1s2 (i.e. |s1|+|s2|)
* l2: the length of the maximal repeat (if s1 is maximal, then l2 = |s1|, else l2 = |s2|)
