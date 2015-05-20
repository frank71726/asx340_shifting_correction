#!/bin/sh
find . -name "*.h" -o -name "*.c" -o -name "*.cc" > cscope.files
cscope -bkqR -i cscope.files
ctags -R
