import json
import sys

big_dict = {}

names_file = sys.argv[1]
base_file_name = names_file.split('/')[-1].split('.')[0]
names = open(names_file, 'r')

for line in names:
    name = line.strip()
    big_dict[name] = {'0': 0, '1': 0, '2': 0, '3': 0, '4': 0, '5': 0, '6': 0, '7': 0}

with open(f'{base_file_name}.json', 'w', encoding='utf-8') as f:
    json.dump(big_dict, f, ensure_ascii=False, indent=4)
