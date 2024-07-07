#!/bin/bash

target_file=$1

if [ "$#" -ne 1 ]; then
    "Usage: {$0} [FILE]"
    exit 1
fi

mkdir "學員" &>/dev/null
mkdir "分組狀況" &>/dev/null

while read line; do
    IFS=, read -r name gender birth school grade <<< "$line"
    target="學員/${name}.md"
    if [[ "${name}" == "您的姓名" ]]; then
        continue
    fi
    echo "---" > "${target}"
    echo "性別: $gender" >> "${target}"
    echo "生日: $birth" >> "${target}"
    echo "學校: $school" >> "${target}"
    echo "年級: $grade" >> "${target}"
    echo "---" >> "${target}"
done < "${target_file}"

while read line; do
    target="分組狀況/${line}"
    mkdir "${target}" &>/dev/null
    touch "${target}/${line}.md"
done < groups.txt

echo "[+] Initialization Done"

echo "[+] Watching"

while true; do
    while read group; do
        for filename in 分組狀況/${group}/*.md; do
            if [[ ! -e "${filename}" || $(basename "${filename}" .md) == "${group}" ]]; then
                continue
            fi
            echo "${filename}"
            if ! grep -Fxq "[[${group}]]" "${filename}"; then
                sed -i '' -E -e "s/\[\[.*\]\]//" "${filename}"
                echo "[[${group}]]" >> "${filename}"
            fi
        done
    done < groups.txt
    sleep 1
done
