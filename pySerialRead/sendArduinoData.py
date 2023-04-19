# importing the requests library
import requests
import os

def convert_data(file):
	f = open(file,'r')
	txt = f.read().strip()
	f.close()
	data = {'events':[]}
	lines = txt.split("\n")
	team_id = lines[0].split(";")[1]
	for line in lines[2:]:
		rw = line.split(";")
		data['events'].append({"name":rw[0], "time":rw[1], "type":rw[2]})

	return team_id, data

path = "./queue/"
dirs = os.listdir( path )

# api-endpoint
URL = "http://localhost:8000/arduino-data/"

for file in dirs:
	if (file.endswith(".txt")):
		team, data = convert_data(path+file)
		r = requests.post(url = URL+team, json=data)
		
		resp = r.text
		if resp.strip() == 'saved':
			os.rename(path+file,"./sent/"+file)
