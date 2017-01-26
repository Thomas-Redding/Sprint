# Sprint
Compiler for New Language
Updated Dec 15, 2016

## PROGRESS
1. Tokenizer                            - DONE
2. Parser                               - DONE
3. Parse Rules                          - DONE
4. Verify Parsing                       - DONE
5. Type Checking and Variable Scoping   - TODO: finish February
6. Templating                           - TODO: finish February
7. Translate to C++                     - TODO: finish February
8. Garbage Collection                   - TODO
9. Output to C++ Files                  - TODO; expected to take a couple days
10. Allow Input From Multiples Files    - TODO; expected to take a couple days
11. Standard Library                    - TODO
12. Sample Projects                     - TODO

## FEATURES
This language is generally similar to C++. Here are it's planned noteworthy features

1. Will support all C++ libraries.
2. Garbage Collector (Mark-and-sweep, concurrent, tri-colored)
3. Abstraction away from Stack v. Heap without loss of speed
    - As noted directly above, you can think of every object as being on the Heap. However, the compiler will detect when an object is both small enough and short-lived enough to be able to be on the Stack
4. Interchangability between functions and public methods (including operator-overrides)
    - For instance, these are equivalent: myMatrix.det(); det(myMatrix);
    - In lieu of a toString() operator, conversion to strings is done by creating your own string constructor: str(myObj)
5. Unions with type checking
6. Currying
7. Strong typing - no auto-conversion.
8. Some Sugar:
    - Pointer assignment: myPtr <- myObj
    - Pointer comparsion: x is y is equivalent to &x == &yin C++
    - Use of "and", "or", and "not" instead of &&, ||, and !
    - More concise primitive int classes: int8, int16, int32, int64, int, uint8, uint 16, etc.
9. Large, but moduralized standard library. Because we will support all C++ libraries, we can incorporate a wide variety of Open Source libraries as part of our standard library. For instance, we plan to use wxWidgets to provide cross-platform UI buiding. Ultimately, we want to allow full-stack development after a single installation of our language.
