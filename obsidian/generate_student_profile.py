import sys
import re

if len(sys.argv) < 2:
    print(f"Usage: {sys.argv[0]} [target file]")
    exit(1)

target_directory = "學員/"
id_regex_pattern = re.compile("T[12]-[0-9]{3}")

target_file = open(sys.argv[1], "r")
target_file.readline()
data = target_file.read().split("\n")

for line in data:
    if id_regex_pattern.search(line):
        info = line.split(',')
        student_data = {
            "ID": info[0],
            "性別": info[1],
            "房號": info[2],
            "出生日期": info[3],
            "身分證字號": info[4],
            "法定代理人姓名": info[5],
            "法定代理人聯絡電話": info[6],
            "法定代理人身分證字號": info[7],
            "法定代理人出生日期": info[8],
            "衣服": info[9],
            "就讀學校": info[10],
            "年級": info[11],
            "調查｜解散資訊": info[12],
            "調查｜飲食習慣": info[13],
            "調查｜實作課程相關能力經驗": info[14],
            "報名來源": info[15],
            "身份": info[16],
            "備審": info[17],
            "備審備註": info[18],
            "推薦信內文": info[19],
            "湘琪備註": info[20],
        }

    else:
        pass
