# Sprint
Compiler for New Language

I'm hoping to make a (as-yet-unnamed) language that is
1. as Elegant as Python
2. as Fast as C

Additionally, I'm hoping to add a large, cross-platform standard library to let people easily develop cross
-platform applications by writing code exactly once and simply compiling it on their different systems. This project is a complier for this language

I don't expect to work on this much prior to Nov 24th, 2015. I expect to be very productive in development between then and the new year. On an abstract level, this project has two fairly discrete parts:

1. Writing code that will take a set of files and compile it into C++ code.
2. Writing an IDE to aid in development. The IDE should include
  - Version Control
  - Highlighting
  - Autocomplete
  - Auto-updating
  - Language Documentation
  - Compiling - first from this language to C++ and then from C++ to assembly (using the OS's native compiler)
