test_files=("test1.txt" "test2.txt" "test3.txt" "test4.txt")


echo -e "Hello\nWorld\n\nThis is a test file.\nAnd it will be nice, if my grep passes me through." > test1.txt
echo -e "\tIndented line\nAnother line\n\n\nEnd of file." > test2.txt
echo -e "Numbers:\n12345\n67890\n\nTabs:\tTest" > test3.txt
echo -e "Another test file\nwith some data\nand more lines" > test4.txt

flags=(            
    "another -i"           
    "some -v"           
    "data -n"           
    "Another -c "           
    "Another -l "                      
    "Another -h"           
    "-e Numbers" 
    "-e another" 
    "-e test -i" 
    "-f config.txt" 
    "-ni -f config.txt"          
    "-vn -f config.txt"          
    "-ni -e line" 
    "-cn -f config.txt"          
    "-lh -f config.txt"          
)

echo -e "Numbers\ntest\nsome\n12345\nAnother\nLine" > config.txt

for file in "${test_files[@]}"; do
    for flag in "${flags[@]}"; do
        ./s21_grep $flag $file > my_output.txt

        grep $flag $file > original_output.txt

        if diff -q my_output.txt original_output.txt > /dev/null; then
            echo "Test PASSED for file: $file with flags: $flag"
        else
            echo "Test FAILED for file: $file with flags: $flag"
            echo "Differences:"
            diff my_output.txt original_output.txt
			exit
        fi
    done
done

rm test1.txt test2.txt test3.txt test4.txt config.txt my_output.txt original_output.txt
