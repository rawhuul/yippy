echo "*************Inline Evaluation TEST****************"

INLINE_EVAL='""'

res=$(${YIPPY_EXE} -e $INLINE_EVAL)

if [[ $? -ne 0 ]]
then
    echo -e "Failed. Make sure, you compiled the program correctly."
else
    echo -e "Working Fine."
fi
