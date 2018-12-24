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

For example,

```
./mccreight NC_021868.txt direct -r 10 -e 5 -m 0.1
```

And repeat

```
until finished
```

End with an example of getting some data out of the system or using it for a little demo

## Running the tests

Explain how to run the automated tests for this system

### Break down into end to end tests

Explain what these tests test and why

```
Give an example
```


### And coding style tests

Explain what these tests test and why

```
Give an example
```

## Deployment

Add additional notes about how to deploy this on a live system

## Built With

* [Dropwizard](http://www.dropwizard.io/1.0.2/docs/) - The web framework used
* [Maven](https://maven.apache.org/) - Dependency Management
* [ROME](https://rometools.github.io/rome/) - Used to generate RSS Feeds

## Contributing

Please read [CONTRIBUTING.md](https://gist.github.com/PurpleBooth/b24679402957c63ec426) for details on our code of conduct, and the process for submitting pull requests to us.

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/your/project/tags). 

## Authors

* **Billie Thompson** - *Initial work* - [PurpleBooth](https://github.com/PurpleBooth)

See also the list of [contributors](https://github.com/your/project/contributors) who participated in this project.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Hat tip to anyone whose code was used
* Inspiration
* etc
