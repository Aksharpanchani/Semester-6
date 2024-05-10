# command to generate all executables
all: vmsim_out vmgen_out vmstats_out

vmsim_out: vmsim.c engine.h
	gcc vmsim.c engine.h -o vmsim_out

vmgen_out: vmgen.c
	gcc vmgen.c -o vmgen_out

vmstats_out: vmstats.c engine.h
	gcc vmstats.c engine.h -o vmstats_out

# Command to delete all generated output files
clean:
	rm -f vmsim_out vmgen_out vmstats_out vmrates.dat vmrefs.dat