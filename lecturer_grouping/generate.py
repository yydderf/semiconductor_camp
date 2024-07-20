import random
import datetime
import numpy as np
import copy
import sys

if len(sys.argv) < 2:
    names_filename = "t1_names"
else:
    names_filename = sys.argv[1]
    print(names_filename)

lecturer_list = ["講者A", "講者B", "講者C", "講者E", "講者D", "講者F", "講者H", "講者G"]
probabilities = {
    "講者A": 100,
    "講者B": 80,
    "講者C": 60,
    "講者D": 50,
    "講者E": 50,
    "講者F": 50,
    "講者H": 45,
    "講者G": 45
}
num_values_to_select = 4
curr_datetime = datetime.datetime.now()

output = open("out.csv", "w")
names = open(names_filename, "r")
rps_list = ["剪刀 ✂️", "石頭 ✊", "布 🧻"]
output.write("時間戳記,您的姓名,講者志願序 [第一志願],講者志願序 [第二志願],講者志願序 [第三志願],講者志願序 [第四志願],講者志願序 [第五志願],講者志願序 [第六志願],講者志願序 [第七志願],講者志願序 [第八志願],在這個環節中，我們針對你填寫的講題志願，寫了一支精美程式來最好的分配結果\n")
id = 0

# for i in range(120):
while name := names.readline():
    tmp_list = copy.copy(lecturer_list)
    # output.write((f"{curr_datetime.strftime("%Y/%m/%d 下午 %H:%M:%S")},{str(id).zfill(3)},{",".join(lecturer_list)}\n"))
    ret_list = []
    while (tmp_list != []):
        tot_val = sum([probabilities[val] for val in tmp_list])
        tmp_prob = [probabilities[val]/tot_val for val in tmp_list]
        elem = np.random.choice(tmp_list, 1, p=tmp_prob)
        ret_list.append(elem[0])
        tmp_list.remove(elem[0])
    ret_list.append(random.choice(rps_list))
    output.write((f"{curr_datetime.strftime("%Y/%m/%d 下午 %H:%M:%S")},{name.strip()},{",".join(ret_list)}\n"))
    id += 1
    random_inc = random.randint(0, 2)
    curr_datetime = curr_datetime + datetime.timedelta(seconds=random_inc)

