# Sprint
Compiler for New Language

## PROGRESS
1. Tokenizer        - DONE
2. Parser           - DONE
3. Parse Rules      - IN PROGRESS as of Aug 5, 2016; expected completion is Aug 12.
4. Compiler         - TODO
5. Standard Library - TODO

## FEATURES
This language is generally similar to C++. Here are it's planned noteworthy features

1. Will support all C++ libraries.
2. Compiler-enforced garbage collection.
    - This is done by having two kinds of variables: owners and pointers. Pointers are like C++ pointers in that they can be null and can be reassigned. We do not allow pointer arithmetic. Owners can be thought of as pointers to objects on the Heap that are garbage collected once they exit scope. An owner can give its object away prior to exiting scope to maintain it's existence. This is, more-or-less, a compiler-verified version of the RAII paradigm, typically used in C++.
3. Abstraction away from Stack v. Heap without loss of speed
    - As noted directly above, you can think of every object as being on the Heap. However, the compiler will detect when an object is both small enough and short-lived enough to be able to be on the Stack.
4. Interchangability between functions and public methods (including operator-overrides)
    - For instance, these are equivalent: myMatrix.det(); det(myMatrix);
    - In lieu of a toString() operator, conversion to strings is done by creating your own string constructor: str(myObj)
5. The keyword "var" in lieu of void pointers.
    - Example: var x = int(42); type(x) == int;
    - The special "var" type is significantly slower than statically typed objects. However, it makes it much easer to implement a JSON parser, a DOM hierarchy, or a standard library print() function.
6. Currying
7. Strongly typed. The only auto-conversion is up the inheritance tree and for "var" objects.
8. Some Sugar:
    - Pointer assignment: myPtr := myObj
    - Pointer comparsion: x === y and x !== y are equivalent to &x == &y and &x != &y in C++
    - Use of "and", "or", and "not" instead of &&, ||, and !
    - Tuples can be used like any other types and have native suppor: (int, float) foo = (42, 3.14);
9. Large, but moduralized standard library. Because we will support all C++ libraries, we can incorporate a wide variety of Open Source libraries as part of our standard library. For instance, we plan to use wxWidgets to provide cross-platform UI buiding. Ultimately, we want to allow full-stack development after a single installation of our language.
