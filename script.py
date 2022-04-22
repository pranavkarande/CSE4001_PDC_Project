import os
import subprocess
import sys
import platform

os.system('g++ main.cpp -o main -fopenmp')

output = []
single_thread_sum = 0.0
no_of_threads = os.cpu_count()
files = ['p2p-Gnutella08.txt', 'test.txt', 'soc-Epinions1.txt']

f = open('output.txt', 'w')
__file__ = "main"
file_path = os.path.dirname(os.path.realpath(__file__))+'/main'

for file_name in files:
    f.write("Time taken for "+file_name+':'+'\n')
    for _ in range(3):
        print('Running on 1 thread...', end='\r')
        p = subprocess.run([file_path, '1',
                            file_name], capture_output=True)
        x = p.stdout.decode("utf-8")
        splitting = x.split('h')
        single_thread_sum += float(splitting[1])/3
    output.append(single_thread_sum)

    for i in range(2, no_of_threads+1):
        print('Running on', str(i), 'threads...', end='\r')
        p = subprocess.run([file_path, str(i),
                            file_name], capture_output=True)
        x = p.stdout.decode("utf-8")
        splitting = x.split('h')
        output.append(float(splitting[1]))

    for i in range(16):
        element = output[i]
        element = f'{element:.3f}'
        f.write(str(i+1)+'. '+str(element)+'\n')
    
    f.write('\n')

f.close()
