gcc -c -Wall -Werror -fpic pluginsrc/$1.cpp
gcc -shared -o plugins/$1.so $1.o
