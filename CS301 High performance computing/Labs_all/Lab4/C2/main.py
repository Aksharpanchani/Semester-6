from subprocess import Popen, PIPE
import os
import csv
import config

# Enter the name of problem and machine

# prob_name = "MERGE_SORT"
# machine_name= "CLUSTER"

prob_name = "MERGE_SORT"
machine_name= "LAB207"

process = Popen("lscpu", stdout=PIPE, stderr=PIPE, shell=True)
out,err = process.communicate()
out = out.strip()
# out = out.decode("utf-8")
res = list(out.split("\n"))
final_lscpu = {}
for r in res:
        r = r.strip()
        r = list(r.split(":"))
        final_lscpu[r[0]] = r[1].strip()

print(final_lscpu)

# removes a file if it is present
def removeFile(file):
    if file not in os.listdir(os.curdir):
        return "File "+file+" wasn't present: ignoring"

    out, err = Popen('rm '+file, stdout=PIPE, stderr=PIPE, shell=True).communicate()
    out = out.strip()
    err = err.strip()
    if err != '' or out != '':
        raise Exception(err+'\n'+out)
    
    return "File "+file+" was removed"


# returns only if file is compiled successfully
# provided the exec is absent when it is called
def compileFile(file):
    if file not in os.listdir(os.curdir):
        err = "File "+file+" is absent, can't proceed"
        raise Exception(err)

    file_name = file.split('.')[0]
    out, err = Popen('gcc -o '+file_name+' '+file+' -fopenmp', stdout=PIPE, stderr=PIPE, shell=True).communicate()
    out = out.strip()
    err = err.strip()
    if file_name not in os.listdir(os.curdir):
        print(out)
        print(err)
        err = "File "+file+" wasn't compiled"
        raise Exception(err)
            
    
    return "File "+file+" was compiled"



if __name__ == "__main__":
    # remove the previous execs and recompile
    try:
        print(removeFile('serial'))
        print(removeFile('parallel'))

        print(compileFile('serial.c'))
        print(compileFile('parallel.c'))
    except Exception as e:
        print(e, ":exiting")
        exit()

    print("*"*80)

    reuse = None
    probSizeIndx, procNumIndx, rid = None, None, None

    runs = None
    while type(runs) != int or runs <= 0:
        try:
            runs = int(input('Number of runs: '))
        except:
            pass

    print("Warning: if configs have changed, don't reuse!")
    while reuse != 'n' and reuse != 'y':
        reuse = raw_input('Reuse previous data? (y/n): ')

        if reuse == 'n':
            # don't reuse data, fresh start
            print("Removing data.txt")
            print(removeFile('data.txt'))
            probSizeIndx, procNumIndx, rid = 0, 0, 0
        elif reuse == 'y':
            if 'data.txt' not in os.listdir(os.curdir):
                print('data.txt is absent: ignoring')
                probSizeIndx, procNumIndx, rid = 0, 0, 0
            else:
                with open('data.txt', 'r') as f:
                    for line in f.readlines():
                        try:
                            n, p, r, a, e = line.split(',')
                            n, p, r = int(n), int(p), int(r)
                            probSizeIndx = config.prob.index(n)+1
                            procNumIndx = config.procs.index(p)
                            rid = r
                            if rid == runs:
                                print("data.txt has all the required runs, delete additional data")
                                exit()
                        except:
                            print("Configs are inconsistent with data.txt: exiting")
                            exit()
    avg_data_algo = [[0]*len(config.prob) for _ in range(len(config.procs))]
    avg_data_e2e = [[0]*len(config.prob) for _ in range(len(config.procs))]
    #print(avg_data_e2e)
    for runId in range(rid, runs):
        print("*"*80)
        print("Run #:", runId+1)
        for p in range(procNumIndx, len(config.procs)):
            print("*"*40)
            print("Processors:", config.procs[p])
            if config.procs[p] == 0:
                print("Serial Run")
            for n in range(probSizeIndx, len(config.prob)):
                print("Problem Size:", config.prob[n])

                if config.procs[p] == 0:
                    cmd = './serial '+str(config.prob[n])+' ' + str(config.procs[p])
                else:
                    cmd = './parallel '+str(config.prob[n])+' ' + str(config.procs[p])
                    
                process = Popen(cmd, stdout=PIPE, stderr=PIPE, shell=True)
                out, err = process.communicate()
                if process.returncode != 0:
                    print("Exit code:", process.returncode, "exiting")
                    print(err.strip())
                    print(out.strip())
                    exit()
                else:
                    out = out.strip()
                    algName,algApp,N,P,e2eX,e2eY,algX,algY = out.split(',')

                    # below code is from 'text_to_csv.py from original letshpc'
                    algTime = float(algX) + float(algY) / 1e9
                    e2eTime = float(e2eX) + float(e2eY) / 1e9
                    ###########################################################

                    avg_data_algo[p][n] += algTime
                    avg_data_e2e[p][n] += e2eTime

                    algTime = str(algTime)
                    e2eTime = str(e2eTime)
		#    print(avg_data_algo[p])
                    dataPoint = ','.join([N,P,str(runId),algTime,e2eTime])
            
            # next run should begin at 0s
            probSizeIndx = 0
        procNumIndx = 0
    final_data = []
    fields = ["PROB_NAME", "MACHINE", "PROB_SIZE","NUMBER_CORES", "ALG_TIME", "TOTAL_TIME"]
    for p in range(procNumIndx, len(config.procs)):
    	for n in range(probSizeIndx,len(config.prob)):
        	# with open('data.txt', 'a') as f:
       	 	#     dataPoint = ','.join([prob_name,machine_name,config.prob[p],str(avg_data_algo[p]/runs),str(avg_data_e2e[p]/runs)])
        	#     f.write(dataPoint+'\n')
        	dataPoint = [prob_name,final_lscpu["Model name"],config.prob[n],config.procs[p],(avg_data_algo[p][n]/runs),(avg_data_e2e[p][n]/runs)]
        	final_data.append(dataPoint)
    
    with open('data.csv', 'w') as f:
      
    # using csv.writer method from CSV package
        write = csv.writer(f)
        
        write.writerow(fields)
        write.writerows(final_data)




