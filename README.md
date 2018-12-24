# MicroDNA_Detection

The goal of this project is to provide an efficient algorithm to locate potential microDNAs within DNA sequences. Each potential microDNA is mathematically defined as a circle repeat, with form "...s1s2...s2s1..." (direct circle repeat), and "...s1s2...s1^(-1)s2^(-1)..." (reversed circle repeat). The program takes a sequence as input, and output four text files that contains the circle repeats sequences found by the algorithm and as well as their indices. The detailed description of the algorithm can be found in the pdf file (to be uploaded).

## Getting Started

Enter the project directory, and generate the executable from code by using 
```
make
```
You should be getting an executable called mccreight.


## Command Line Options

The executable has 2 required argument and 3 optional arguments:
* First argument (required):  sequence file name. The file should contain the DNA sequence in which you search the circle repeats; note that any characters that are not A, T, C, G are filtered out in the preprocessing.
* Second argument (required): either "direct" or "reversed". If "direct", then the program will search direct circle repeats (of form "...s1s2...s2s1..."); if "reversed", the program will search reversed circle repeats (of form "...s1s2...s1^(-1)s2^(-1)...").
* Optional argument: "-r minimum_maximal_repeat_length". Default value of minimum maximal repeat length is 90.
* Optional argument: "-e minimum_extension_length". Default value of minimum extension length length is 20.
* Optional argument: "-m percentage_mismatch_allowed". Default value of percentage mismatch is 0.1

Broadly speaking, minimum maximal repeat length and minimum extension length can be thought of as the minimum length of s1 and s2. Also note that the mismatch ratio is for the extension section of the circle repeat, not the entire circle repeat. We encourage you to read the pdf report (to be uploaded) to have a better understanding of these parameters.

## Example
```
./mccreight NC_021868.txt direct -r 10 -e 5 -m 0.1
```

The above command searches for direct circle repeats in the sequence from file NC_021868.txt, with minimum maximal repeat length 10, minimum extension length 5, and allowed mismatch ration 10%.

