# Cicular Repeat Finder

## Purpose

MicroDNA is the most abundent type of extrachromosomal circular DNA (eccDNA). They are small in size and have been found in various eukaryotes ranging from yeasts to humans. MicroDNAs are thought to originate from genomic regions with active chromatin marks. To better understand the function of microDNAs, it is of interest to investigate their interaction with the chromosomal DNA, given the frequent presence of microDNAs in the nuclei of human and mouse cell lines. In particular, do microDNAs reintegrate back into the genome?

The goal of this algorithm is to provide an efficient algorithm to locate potential microDNA reintegration patterns within DNA sequences. A reintegration pattern is mathematically defined as a circle repeat, with form ...s<sub>1</sub>s<sub>2</sub>...s<sub>2</sub>'s<sub>1</sub>'... (direct circlar repeat), or ...s1s2...s̅'<sub>2</sub>s̅'<sub>1</sub>... (inverted circle repeat), where s' denotes the approximate copy of s, and s̅ the reverse complement of s. The program takes a sequence as input, and output the circlar repeated pairs found by the algorithm in text files. Optional parameters for s<sub>1</sub>, s<sub>2</sub>, and mismatch ratio, and neighorhood size can also be specified. 

For large input sequences, memory requirement is often prohibitive too run the entire sequence. In such cases, we provide a helper program that partitions the long sequences into smaller sequences, and divide the original task into a set of smaller subtasks. This also provides a way to speed up the task by running the subtasks on multiple machines. 

The suffix array construction code is based on https://github.com/felipelouza/sacak-lcp.

Here is an illustration for circular repeats pattern:
![](reintegration.png)




## Getting Started

Enter the project directory, and generate the executable from code by using 
```
make
```
You should be able to get the following 2 executables: partition and CRPFinder.

## Preprocessing using Tandem Repeat Finder
It is important in the proprocessing step to remove (mask) the tandem repeat segments of the input DNA sequence using [Tandem Repeat Finder](https://github.com/Benson-Genomics-Lab/TRF). This is because tandem repeats trivially satifies the definition of circle repeats by definition. 

## Command Line Options for partition
The <code> partition </code> executable is used as a preprocessing step to for the <code>--partition</code> option for <code>CRPFinder</code>. It is often used when the input sequence is long, to speed up the searching through parallelization or to deal with memory issue.

The partition executable takes 3 arguments:
* 1st argument: sequence file name
* 2nd argument: number of partitions
* 3rd argument: name of the directory where the splitted sequence files are stored



## Command Line Options for mdf

The executable has 1 required argument, 1 argument from either -t or -s, and 2 optional arguments:
* First argument (required): either "direct" or "reversed". 
<!---
If "direct", then the program will search direct circle repeats (of form "...s1s2...s2s1..."); if "reversed", the program will search reversed circle repeats (of form "...s1s2...s1^(-1)s2^(-1)...").
-->

* -s sequence file name. 
<!---
The file should contain the DNA sequence in which you search the circle repeats; note that any characters that are not A, T, C, G are filtered out in the preprocessing.
-->
* -t partition_file_dir task_numbers
* Optional argument: "-r minimum_first_level_maximal_repeat_length". Default value is 40.
* Optional argument: "-c minimum_second_level_maximal_repeat_length". Default value is 20.
* Optional argument: "-e extension checking length". Default value is 800.
* Optional argument: "-m percentage_mismatch_allowed". Default value of percentage mismatch is 0.1

## Example
```
./mdf direct -s you_seq_file.txt -r 40 -m 0.1
```

The above command searches for direct circle repeats in the sequence from file you_seq_file.txt, with minimum maximal repeat length 40, and allowed mismatch ration 10%.

## Output File
There will be an index file as output in the result folder. Each circle repeat is represented by a 8-tuple.
* 1st: the start index of s1s2
* 2nd: the start index of s2s1 (or s1's2' if reverse)
* 3rd: length of first s1
* 4th: length of first s2
* 5th: length of second s1
* 6th: length of second s2
* 7th: total mismatch ratio
* 8th: mismatch ratio for s1
* 9th: mismatch ratio for s2
* 10th: length of s1s2 (which is the same as s2s1 or s1's2', even if the two s1's and two s2's are not exact match)


