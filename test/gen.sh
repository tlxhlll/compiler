../build/spc -ir ./course/course.pas
llvm-as-9 ./course/course.ll
clang ./course/course.bc -o ./course/course.

../build/spc -ir ./matrix/matrix.pas
llvm-as-9 ./matrix/matrix.ll
clang ./matrix/matrix.bc -o ./matrix/matrix.

../build/spc -ir ./quicksort/quicksort.pas
llvm-as-9 ./quicksort/quicksort.ll
clang ./quicksort/quicksort.bc -o ./quicksort/quicksort.

../build/spc -ir ./Others/write.pas
../build/spc -ir ./Others/rec.pas
../build/spc -ir ./Others/fib.pas