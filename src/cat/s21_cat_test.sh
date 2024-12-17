test_files=("test1.txt" "test2.txt" "test3.txt")

echo -e "Hello\nWorld\n\nThis is a test file." > test1.txt
echo -e "\tIndented line\nAnother line\n\n\nEnd of file." > test2.txt
echo -e "Numbers:\n12345\n67890\n\nTabs:\tTest" > test3.txt

flags=(
    ""
    "-n"
    "-b"
    "-e"
    "-E"
    "-t"
    "-T"
    "-s"
    "--number"
    "--number-nonblank"
    "--squeeze-blank"
    "-n -b"
    "-e -s"
    "-t -E"
    "-b -s"
)

for file in "${test_files[@]}"; do
    for flag in "${flags[@]}"; do
        ./s21_cat $flag $file > my_output.txt

        cat $flag $file > original_output.txt

        if diff -q my_output.txt original_output.txt > /dev/null; then
            echo "Test PASSED for file: $file with flags: $flag"
        else
            echo "Test FAILED for file: $file with flags: $flag"
            echo "Differences:"
            diff my_output.txt original_output.txt
        fi
    done
done

rm my_output.txt original_output.txt test1.txt test2.txt test3.txt
