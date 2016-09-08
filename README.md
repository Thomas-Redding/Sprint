# Sprint
Compiler for New Language

## PROGRESS
1. Tokenizer                            - DONE
2. Parser                               - DONE
3. Parse Rules                          - DONE
4. Verify Parsing                       - TODO: Sep 2016 (first half)
5. Type Checking and Variable Scoping   - TODO: Sep 2016 (first half)
6. Templating                           - TODO: Sep 2016 (second half)
7. Translate to C++                     - TODO: Oct 2016
8. Garbage Collection                   - TODO; Nov 2016
9. Output to C++ Files                  - TODO; expected to take a couple days
10. Allow Input From Multiples Files    - TODO; expected to take a couple days
11. Standard Library                    - TODO; Dec 2016 - Feb 2017 (with ongoing development)
12. Sample Projects                     - TODO; 2017

## FEATURES
This language is generally similar to C++. Here are it's planned noteworthy features

1. Will support all C++ libraries.
2. Garbage Collector (Mark-and-sweep, concurrent, tri-colored)
3. Abstraction away from Stack v. Heap without loss of speed
    - As noted directly above, you can think of every object as being on the Heap. However, the compiler will detect when an object is both small enough and short-lived enough to be able to be on the Stack
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
    - More concise primitive int classes: int8, int16, int32, int64, int, uint8, uint 16, etc.
9. Inheritance from primitives. For instance, once can say that Meter and Foot both inherit from Double, thereby allowing you to override the + operator to add these two different types properly. Alternatively, don't override the + operator to forbid the addition of two types, such as Meter and Kilogram.
10. Large, but moduralized standard library. Because we will support all C++ libraries, we can incorporate a wide variety of Open Source libraries as part of our standard library. For instance, we plan to use wxWidgets to provide cross-platform UI buiding. Ultimately, we want to allow full-stack development after a single installation of our language.
11. With a nod towards GUI app development, we plan to naitively support a JavaScript-style event loop. The benefit of this is that you can use callbacks without worrying about locking. This is dones using the "call" keyword. For instance, the line "call foo(5)" will cause your program to to call foo() with the given argument when it is doing nothing else. This is also how native event will works. While this is not true multi-threading (which is still supported with threads), it does allow for real-time GUI operations without worrying about object-locking, deadlock, and other common threading issues. For better or worse, this means we don't follow the idiom that C++ does, in which each program starts in main(). Instead, like JavaScript, any code you leave outside a function or class is executed by default. If you want to return with a code, simply call exit(), which takes an optional integer argument.
