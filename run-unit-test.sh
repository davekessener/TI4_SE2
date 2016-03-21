#!/bin/bash

git_root=`git rev-parse --show-toplevel`
result=0

usage()
{
cat <<EOF
$0 [all/list/help/test [<test1>, <test2>, ...]]
	all	Run all tests that can be found recursively.
	list	Lists all found tests.
	help	Prints this message.
	test	Executes the specified test files.
	clean	Cleans the framework build.
EOF
}

make_testdir()
{
	testdir=`mktemp -d`

	trap cleanup_testdir EXIT
}

cleanup_testdir()
{
	rm -rf "$testdir"
}

print_padded()
{
	printf '%s%*.*s' "$1" 0 $((74 - ${#1} )) "`printf '%0.1s' " "{1..80}`"
}

compile_test()
{
	tcn="${2/%.test.o/}"
	tcn="${tcn// /_}"
	tcn="${tcn##*\/}"

	print_padded "Compiling '$of' "
	g++ -Wall -std=gnu++11 -I "$git_root/lib" -DTEST_CASE_NAME=$tcn -c "$1" -o "$2" || exit 1
	echo "[DONE]"
}

execute_tests()
{
	declare -a queued_tests=("${!1}")

	if [ ${#queued_tests[@]} -lt 1 ]; then
		echo "ERR: need to specify at least one test to run!"
		exit 1
	fi

	make_testdir

	for (( i=0 ; i<${#queued_tests[@]} ; ++i )); do
		of="${queued_tests[$i]/%.test.cc/}.test.cc"
		nf="${of//\//_}"

		if [ ! -f "$of" ]; then
			echo "ERR: file '$of' does not exist!"
			exit 1
		fi

		compile_test "$of" "$testdir/${nf/%.cc/.o}"
	done

	cd "$testdir"

	print_padded "Updating framework "
	make -C "$git_root/test" > /dev/null || exit 1
	echo "[DONE]"

	cp "$git_root/test/libut_framework.a" .
	
	print_padded "Assembling test executable "
	g++ -L. *.o -o exec_test -lut_framework || exit 1
	echo "[DONE]"

	echo "Running test executable"
	echo "================================================================================"
	if ./exec_test; then
		echo "SUCCESS"
		result=0
	else
		echo "-----"
		echo "FAILURE"
		result=1
	fi
	echo "================================================================================"
}

find_tests()
{
	local oifs=$IFS
	IFS=$'\n'
	test_list=($(find . -name "*.test.cc" | cut -c 3-))
	IFS=$oifs
}

list_all()
{
	find_tests

	for (( i=0 ; i<${#test_list[@]} ; ++i )); do
		echo "${test_list[$i]/%.test.cc/}"
	done
}

test_all()
{
	find_tests

	execute_tests test_list[@]
}

clean_framework()
{
	print_padded "Cleaning framework files ..."
	make -C "$git_root/test" clean > /dev/null
	echo "[DONE]"
}

if [ $# -lt 1 ]; then
	usage
else
	case $1 in
		"help")
			usage
			;;
		"list")
			list_all
			;;
		"all")
			test_all
			;;
		"test")
			shift
			a_arr=( "$@" )
			execute_tests a_arr[@]
			;;
		"clean")
			clean_framework
			;;
		*)
			echo "Unknown command."
			echo "Please try '$0 help'"
	esac
fi

exit $result

