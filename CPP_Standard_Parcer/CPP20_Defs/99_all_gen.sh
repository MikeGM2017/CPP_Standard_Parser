
export File_Log='99_all_run.log'
export File_Out='99_all_run.sh'
export File_Exec='./cpp_standard_parcer'

arr=(./*.txt)
for ((i=0; i < ${#arr[@]}; i++)); do

	if [ $i == "0" ]; then
		echo "" > "$File_Out"
	else
		echo "" >> "$File_Out"
	fi

	if [[ ${arr[$i]} =~ "V0" ]]; then
		echo "#Skip file '5${arr[$i]}'" >> "$File_Out"
	else

		if [ $i == "0" ]; then
			echo "echo '' > '$File_Log'" >> "$File_Out"
		else
			echo "echo '' >> '$File_Log'" >> "$File_Out"
		fi

		echo "echo 'File ${arr[$i]}' >> '$File_Log'" >> "$File_Out"
		echo "echo '' >> '$File_Log'" >> "$File_Out"
		echo "$File_Exec -e -f ${arr[$i]} >> '$File_Log'" >> "$File_Out"

	fi

done

chmod +x "$File_Out"
