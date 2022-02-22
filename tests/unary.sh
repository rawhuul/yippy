echo "*************Unary Operations TEST****************"

for i in {1..50};
do

bit=$RANDOM

echo "Unary Plus Test"
echo "+$bit"
yippy_test=$(${YIPPY_EXE} -e "+ $bit")
py_test=$(python3 -c "print(+ $bit)")
if_ok "${yippy_test}" "${py_test}"

echo "Unary Negation Test"
echo "-$bit"
yippy_test=$(${YIPPY_EXE} -e "- $bit")
py_test=$(python3 -c "print(- $bit)")
if_ok "${yippy_test}" "${py_test}"

echo "Unary Negation Test"
echo "!$bit"
yippy_test=$(${YIPPY_EXE} -e "! $bit")
py_test=$(python3 -c "print(int(not $bit))")
if_ok "${yippy_test}" "${py_test}"

echo "Unary One's Complement Test"
echo "~$bit"
yippy_test=$(${YIPPY_EXE} -e "~ $bit")
py_test=$(python3 -c "print(~ $bit)")
if_ok "${yippy_test}" "${py_test}"

done
