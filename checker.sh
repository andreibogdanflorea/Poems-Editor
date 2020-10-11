#!/bin/bash

function check_task() {
	# @params: $TASK_NUMBER $OUTPUT $REFERENCE
	diff $1 $2 &> /dev/null
	return $?
}

TEST_POINTS=0

function check_test() {
	# @params: $INPUT $OUTPUT $REFERENCE
	INPUT=$1
	INPUT_ECHO=$(echo $INPUT | sed 's/tests\///g')
	OUTPUT=output/$INPUT_ECHO
	REFERENCE=ref/$INPUT_ECHO
	mkdir -p output/$(echo $INPUT_ECHO | cut -d '/' -f 1)
	./$EXE < $INPUT > $OUTPUT
	check_task $OUTPUT $REFERENCE
	if [ $? -eq 0 ]; then
		echo -e "Test file $INPUT_ECHO task: ok 2.5p"
		TEST_POINTS="2.5"
	else
		echo -e "Test file $INPUT_ECHO task: failed 0p"
		TEST_POINTS="0"
	fi
}

EXE=poems_editor
TESTING=checker
make clean
if [ $? -ne 0 ]; then
	exit 1
fi
make build
if [ $? -ne 0 ]; then
	exit 1
fi
if [ ! -f $EXE ]; then
	echo "The executable name must be $EXE."
	exit 1
fi

echo "=============="
echo "TEMA 2"
echo "=============="

TOTAL=0
mkdir -p output
for i in $(find "tests" | grep -E "test[0-9]" | sort); do
	check_test $i
	TOTAL=$(echo "$TOTAL + $TEST_POINTS" | bc -l)
done


echo "=============="
echo "TOTAL: "$TOTAL"/125"
echo "=============="
