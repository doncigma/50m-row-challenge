# 50 Million Row Challenge

Inspired by the 1 Billion Row Challenge (https://github.com/gunnarmorling/1brc/tree/main), this is the 50 Million Row Challenge.

The idea is simple, and is as follows:

You're given a text file with 50 million lines of data. Each line has the name
of a city, a ';' and then a number in the format NN.N with an optional leading '-'.
Each of these lines represents a temperature measurement for a given city. The
task that lay before you is to write a program that will read this file and
output the average, minimum, and maximum temperature for each city.

The path to the file is provided as the first command line argument to your
program. Your program must open the file, read it's contents, compute the
minimum, average, and maximum temperatures for each city, and then output the
results. An example invocation of your program will look like:

```
./temperatures measurements.txt
```

Upon completion, the program should output the name of the city, the minimum, average,
and maximum temperatures, all separated by a ',' and no spaces, followed by a newline.
For example, your program's output might look like:

```
Naha,10,24,40
Manama,-4,10,25
```

## Constraints/Guidelines

1) Your solution must be implemented in C and optionally RISC-V assembly.
2) It must run on the Single Board RISC-V Computer you've been given.
3) You may only use the standard C library, no external libraries or data structures
    except for what is provided in the C standard library.
4) There will be no more than 1000 unique cities in the data set.
5) City name is between 1 and 100 characters.
6) All temperatures will be between -99.9 and 99.9.
7) Output format must have 1 digit after the decimal point (even if 0)
8) The program will be named 'process_measurements' and compiled by running
    `make` in the directory with the source code.
9) Any floating point rounding should round towards positive infinity.

## Suggestions/Hints

### Faster Testing

You will likely want to start testing using a cut down version of the data set.
To create a smaller version of the data set, you can read a subset of rows
by running `cat measurements.txt | tail -n 1000 > measurements_small.txt` to
create a new file with only 1000 lines of input.

### Data Structures

Consider what kind of data structure you need to store intermediate results and
how you will access it. You will need some way to associate city names to data
about minimum, maximum, and partial averages. You only need to handle adding
new entries to this data structure, and looking up or modifying existing
entries. Helpfully, you do not need to handle deleting entries.

### Problem Decomposition

It will be most helpful for you to break the problem down into smaller logical
sub pieces, and solve each of those sub problems independently. Consider reading
the file, parsing each line, and updating the data structure as separate tasks,
each of which can be implemented independently of each other.

### Unit Tests

It may be helpful to write out smaller unit tests for specific sub pieces of
functionality. Consider using a separate .c file with a different main() function
to run test cases for specific functionality.

## Grading

Submissions will be graded based on computing the correct minimum, average, and
maximum temperatures for each city. The grading script will be run on the
Single Board RISC-V Computer for accurate reproduction of results. A different
input than the one provided with the project will be used for grading.

## Extra Credit

Up to 25% extra credit will be given for submissions that are optimized for
performance. The fastest submission will receive the full 25% extra credit.
The second fastest submission will receive 15% extra credit, and the third
fastest submission will receive 10% extra credit.

Performance will be measured by the average of three repeated runs of the
program against the same input file. Time will be measured using the `time`
tool in Linux: `time process_measurements path/to/measurements.txt`

## Extra Extra Credit

The dataset here is only 50 million rows, if your program is optimized
sufficiently that it can process 250 million rows (all other constraints
remain the same), you will receive an additional 10% extra credit.
