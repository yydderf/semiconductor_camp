import sys
import json

f = open(sys.argv[1], 'r+', encoding='utf-8')
data = json.loads(f.read())


for line in sys.stdin:
    (name, lecturer) = line.split(',')
    lecturer = lecturer.strip()
    data[name][lecturer] = data[name][lecturer] + 1

f.seek(0)
json.dump(data, f, ensure_ascii=False, indent=4)
f.truncate()
f.close()
