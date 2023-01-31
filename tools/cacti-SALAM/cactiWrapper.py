import subprocess
import sys
import os
import yaml
import csv

benchmark_config = str(sys.argv[1])
bench_name = str(sys.argv[2])
bench_subname = str(sys.argv[3])

yaml_file = open(benchmark_config, 'r')
yaml_data = yaml.load(yaml_file, Loader=yaml.FullLoader)
cluster_data = yaml_data['acc_cluster']
accelerators = list()
memobjects = list()
for acc in cluster_data:
    accelerators.append(acc)
    
for params in accelerators:
    for item in params.items():
        for param_type in item:
            if type(param_type) is list:
                for params_list in param_type:
                    if 'Var' in params_list.keys():  
                        for mem_object in params_list.items():
                            if mem_object[1][0]['Type'] == 'SPM':
                                memobjects.append(mem_object[1][0])
                            else:
                                print("Streaming Buffer Found - Dumping Parameters")
                                print(benchmark_config + " " + bench_name + " " + bench_subname)
                                print(yaml.dump(mem_object[1][0]))

process = subprocess.Popen(["./clearStats.sh"])
process.wait()

memobjectsList = []

for element in memobjects:
    memobjectsList.append(element['Name'])
    with open("results/stdout.txt","a") as out, open("results/stderr.txt","a") as err:
        # cacti has a lower limit of 2048 for size
        if (element['Size'] < 2048):
            element['Size'] = 2048
        process = subprocess.Popen(["./cactiStats.sh", str(element['Size']), str(element['Ports'])], stdout=out, stderr=err)
        process.wait()
        out.write(element["Name"] + '(above) \n')
        out.write("/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\ \n\n")

process = subprocess.Popen(["./getcactiResults.sh"])
process.wait()

results_csv = list()
with open('results/out.csv', 'r') as results_file:
    results = csv.reader(results_file)
    for row in results:
        results_csv.append(row)

results_csv[0].insert(0, "Benchmark")
results_csv[0].insert(1, "Config")
results_csv[0].insert(2, "Acc")

index = 0
for row in results_csv[1:]:
    row.insert(0, str(bench_name))
    row.insert(1, str(bench_subname))
    row.insert(2, str(memobjectsList[index]))
    index = index+1

if(not os.path.exists('results/SALAM-out.csv')):
    results_file = open('results/SALAM-out.csv', 'w+')
    writer = csv.writer(results_file)
    writer.writerow(results_csv[0])
    results_file.close()


with open('results/SALAM-out.csv', 'a') as results_file:
    writer = csv.writer(results_file)
    writer.writerows(results_csv[1:])

