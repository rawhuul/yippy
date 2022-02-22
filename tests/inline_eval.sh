echo "*************Inline Evaluation TEST****************"

INLINE_EVAL="\"(+ 1 2 3 4 5 6 7 8 9 0)(- 1 2 3 4 5 6 7 8 9 0)(* 1 2 3 4 5 6 7 8 9 0)(/ 1 2 3 4 5 6 7 8 9 0)(print \"Inline Eval is WORKING!!\")\""
echo $INLINE_EVAL

${YIPPY_EXE} -e $INLINE_EVAL
