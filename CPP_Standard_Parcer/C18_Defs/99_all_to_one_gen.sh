
export File_Out='99_all_to_one.txt'
export File_Run='99_all_to_one_run.sh'
export File_Log='99_all_to_one_run.log'
export File_Dot='99_all_to_one.dot'
export File_Exec='./cpp_standard_parcer'

arr=(./*.txt)
for ((i=0; i < ${#arr[@]}; i++)); do

	if [ $i == "0" ]; then
		echo "" > "$File_Out"
	else
		echo "" >> "$File_Out"
	fi

	if ! [[ ${arr[$i]} =~ "V0" ]]; then

		if [ $i == "0" ]; then
			cat "${arr[$i]}" >> "$File_Out"
		else
			cat "${arr[$i]}" >> "$File_Out"
		fi

	fi

done

echo "$File_Exec -e -f '$File_Out' > '$File_Log'" > "$File_Run"
echo "$File_Exec -gen -f '$File_Out' > '$File_Dot'" >> "$File_Run"

chmod +x "$File_Run"
