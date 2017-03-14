import subprocess

filenames = ['ACRONYMS.TXT', 'COMMON.TXT', 'COMPOUND.TXT', 'CROSSWD.TXT', 'CRSWD-D.TXT', 'FICTION.TXT', 'FREQ.TXT', 'FREQ-INT.TXT', 'KJVFREQ.TXT', 'NAMES.TXT', 'NAMES-F.TXT', 'NAMES-M.TXT', 'OFTENMIS.TXT', 'PLACES.TXT', 'SINGLE.TXT', 'USACONST.TXT']
exe = '..\Release\StringSetPerfComparison.exe'

with open('alldata.csv', 'w') as datafile:
    # write the header
    datafile.write("insertfile,searchfile,insertwords,searchwords,setfound,uosfound,triefound,setinsert,uosinsert,trieinsert,setsearch,uossearch,triesearch\n")

    # Compare all files against all other files, in both orders, including against self.  Write data to the datafile
    for filename1 in filenames:
        for filename2 in filenames:
            subprocess.call([exe, "..\StringSetPerfComparison\\%s" % filename1, "..\StringSetPerfComparison\\%s" % filename2], stdout=datafile)

