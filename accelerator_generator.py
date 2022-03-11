import re
import os
from pprint import pprint

new_acc="vector4"

# f=open("benchmarks/sys_validation/search/hw/search.c")
# rdlin = f.read()
# f.close()
# with open("benchmarks/sys_validation/search/hw/"+new_acc+".c","w") as f1:
#     f1.write(rdlin)
# f1.close()

# f=open("benchmarks/sys_validation/search/hw/search.ini")
# rdlin = f.read()
# f.close()
# with open("benchmarks/sys_validation/search/hw/"+new_acc+".ini","w") as f1:
#     f1.write(rdlin)
# f1.close()

# f=open("benchmarks/sys_validation/search/hw/Makefile","r")
# l=[]
# l=f.readlines()
# f.close()
# l[0]=l[0].replace("\n","")
# print(l)
# l[0]=l[0]+" "+new_acc+".ll\n"
# print(l)
# f=open("benchmarks/sys_validation/search/hw/Makefile","w")
# f.writelines(l)
# f.close()

# os.system('''
# cd benchmarks/sys_validation/search
# make clean; make
# cd ../../..
# ''')

f=open("benchmarks/sys_validation/search/config.yml")
# f=open("out1.txt")
l=f.readlines()
f.close()
print(l)
cnt=-1

f=open("benchmarks/sys_validation/search/config.yml","w")
f.write("")

cnt=-1
for lin in l:
    cnt+=1
    if(lin== 'hw_config:\n'):
        break
    f.write(lin)

s="  - Accelerator:\n\t- Name:"+new_acc+"\n\t\tIrPath: benchmarks/sys_validation/search/hw/"+new_acc+".ll\n\t\tConfigPath: benchmarks/sys_validation/search/hw/"+new_acc+".ini\n\t\tDebug: False\n\t\tInterruptNum: 68\n\t\tPIOSize: 1\n\t\tPIOMaster: LocalBus\n"
f.write(s)
j=0

for j in range(0,len(l)-1,1):
    if(j<=cnt):
        continue
    else:
        f.write(l[j])

nl=[]
for k in range(44,len(l),1):
    if(l[k]=='  vector1:\n'):
        break;
    nl.append(l[k])

# pprint(nl)

nl[0] = '  '+new_acc+':\n'

f.writelines(nl)
f.close()

# f=open("system_validation_2.sh")
# l1=[]

# l1=f.readlines()
# f.close()
# cnt1=0
# # print(l1)
# for k1 in range(0,len(l1),1):
#     if(l1[k1]=='# ${M5_PATH}/SALAM-Configurator/systembuilder.py --sysName $BENCH --benchDir "benchmarks/sys_validation/${BENCH}"\n'):
#         print("found")
#         cnt1=k1
#         l1[k1]='${M5_PATH}/SALAM-Configurator/systembuilder.py --sysName $BENCH --benchDir "benchmarks/sys_validation/${BENCH}"\n'

# f=open("system_validation_2.sh","w")
# f.writelines(l1)
# f.close()

# os.system("./batch_script.sh")

# f=open("system_validation_2.sh")
# l1=[]

# l1=f.readlines()
# f.close()
# cnt1=0
# # print(l1)
# for k1 in range(0,len(l1),1):
#     if(l1[k1]=='${M5_PATH}/SALAM-Configurator/systembuilder.py --sysName $BENCH --benchDir "benchmarks/sys_validation/${BENCH}"\n'):
#         print("found")
#         cnt1=k1
#         l1[k1]='# ${M5_PATH}/SALAM-Configurator/systembuilder.py --sysName $BENCH --benchDir "benchmarks/sys_validation/${BENCH}"\n'

# f=open("system_validation_2.sh","w")
# f.writelines(l1)
# f.close()

# f=open("configs/SALAM/generated/search.py")
# l1=[]

# l1=f.readlines()
# f.close()
# f=open("configs/SALAM/generated/search.py","w")
# # print(l1)

# for i in range(0,len(l1),1):
#     if(l1[i]=='\tclstr._connect_caches(system, options, l2coherent=False)\n'):
#         print("found")
#         cnt1=i
#         l1[i]='\tclstr._connect_caches(system, options, l2coherent=False, cache_size=\'1MB\')\n'
#     f.write(l1[i])
#     if(l1[i]=='\t# top Config\n'):
#         f.write("\tclstr.top.local = clstr.local_bus.cpu_side_ports\n")
#     if(l1[i]=='\t# search Config\n'):
#         f.write("\tclstr.search.acp = clstr.coherency_bus.cpu_side_ports\n")
#         f.write("\tclstr.search.local = clstr.local_bus.cpu_side_ports\n")
#     elif(l1[i]=='\t# vector1 Config\n'):
#         f.write("\tclstr.vector1.acp = clstr.coherency_bus.cpu_side_ports\n")
#         f.write("\tclstr.vector1.local = clstr.local_bus.cpu_side_ports\n")
#     elif(l1[i]=='\t# vector2 Config\n'):
#         f.write("\tclstr.vector2.acp = clstr.coherency_bus.cpu_side_ports\n")
#         f.write("\tclstr.vector2.local = clstr.local_bus.cpu_side_ports\n")
#     elif(l1[i]=='\t# vector3 Config\n'):
#         f.write("\tclstr.vector3.acp = clstr.coherency_bus.cpu_side_ports\n")
#         f.write("\tclstr.vector3.local = clstr.local_bus.cpu_side_ports\n")
#     else:
#         continue

# f.close()

# os.system("./batch_script.sh")

# f=open("BM_ARM_OUT/sys_validation/search/config.dot.pdf")