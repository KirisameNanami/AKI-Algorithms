AKI Algorithms
==============

Thank you for your interest in my code.

Compile
-------------
Use command:
    make
to compile the project.

Key Dependency Path Generation
-------------
The generation of key dependency path is included in the ``PathGen.cpp`` file.
You have to change those places to fit your encryption algorithm:
1. LEN in #define part: the block size
2. KEY\_LEN in #define part: the length of the master key
3. START\_BIT (optional) in #define part: the first bit you want to generate KDP (for example SIMON-32 should consider from the 16-th bit)
4. ROUND in #define part: the rounds you want to generate KDPs
5. filename of the round function dependency matrix
Then use
    ./PathGen
to generate the Key Dependency Paths. Make sure the folder data/Round\_i has been created.

AKI - Minimum Cut Algorithm
---------------
This algorithm is included in the ``main.cpp`` file.
You have to change those places to fit your encryption algorithm:
1. LEN in #define part: the block size
2. KEY\_LEN in #define part: the length of the master key
3. ROUND in #define part: the rounds of KDPs you want to analyze
4. filename of the key dependency matrix
You can use
    ./main
to analyze the KDPs in data/Round\_i folders, or use
    ./main file/to/your/subkeyset.txt
to analyze a given subkey set.

Matrix Generation Algorithm
---------------------
This algorithm is included in the ``cost.cpp`` file.
You have to change those places to fit your encryption algorithm:
1. LEN in #define part: the block size
2. KEY\_LEN in #define part: the length of the master key
Use
    ./cost file/to/your/subkeyset.txt
to generate a key dependency matrix without Key Information leakage based on your subkey sets.
The answer will be in ``AnsMatrix.txt`` as an output file.
