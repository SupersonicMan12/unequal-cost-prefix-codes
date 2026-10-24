# Solution to 43rd BWINF Round 2 Problem 1: Jewelry Messages

This repository contains the results from my work on `43rd BWINF Round 2 Problem 1: Jewelry Messages`, including the final paper, the code, and the inputs and outputs.

The work was completed and submitted to the competition in April 2025, and was then revised in October 2026, where a one-line bug in the reduction step was fixed (from `reduce(m)` to `reduce(inSign[0])`) and many smaller revisions regarding formatting, typesetting etc. were made.

## Commands

The following commands run from `code/`

To build both programs, run

```bash
g++ -O2 -std=c++17 FinalVersion.cpp -o FinalVersion
g++ -O2 -std=c++17 FinalVersionInt.cpp -o FinalVersionInt
```

To run them as written, run

```bash
cd inputs
../FinalVersion       # reads schmuck0.txt, writes schmuck0.out
../FinalVersionInt    # reads schmuck7.txt, writes schmuck7.out
```

To reproduce every output without editing the sources. This loop can swap the file name in a temporary copy:

```bash
for f in 0 00 01 1 2 3 4 5 6 8; do
    sed "s/schmuck0\./schmuck$f./g" FinalVersion.cpp > /tmp/fv_$f.cpp
    g++ -O2 -std=c++17 /tmp/fv_$f.cpp -o /tmp/fv_$f
    (cd inputs && /tmp/fv_$f)
done
(cd inputs && ../FinalVersionInt)   # schmuck7
```

## Files contained

`main.pdf` is the final paper
`code/FinalVersion.cpp` is the program used for most codes, except `schmuck7`
`code/FinalVersionInt.cpp` is the program used for `schmuck7`, where `unsigned short` was insufficiently large to capture the size of the numbers
`code/inputs` are the original inputs
`code/outputs` are the produced outputs after running the `cpp` files on each of the inputs

## AI Disclosure

During the October 2026 Revision, an AI assistant (Claude Opus 5.5 on Devin, Cognition AI) helped substantially to find the reduction bug, regenerate outputs, and fix typesetting; the algorithm, analysis and original code are my own.