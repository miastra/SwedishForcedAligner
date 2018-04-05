import os

WAXHOLM_DIR = '~/school/talteknologi/waxholmdata' # REPLACE WITH YOUR PATH

def process(fname):
    rows = []
    with open(fname, 'r') as f:
        for row in f:
            data = row.split('\t')
            if len(data) == 5 and 'X' in data[3]:
                # Skip "X-words"
                continue
            rows.append(data)

    triplets = []
    word = start = end = ''
    for row in rows:
        if len(row) == 5 and '' not in (word, start):
            if end == '':
                # Single phoneme word
                end = row[4].split()[0]
            triplets.append((word, start, end))
            word = start = end = ''

        if len(row) == 5:
            word = row[3][3:].lower().replace('|', 'ö').replace('{', 'ä').replace('}', 'å')
            start = row[4].split()[0]
        elif len(row) == 4:
            end = row[3].split()[0]
    
    return triplets

for directory in os.listdir('.'):
    for f in (f for f in os.listdir(directory) if 'lab' in f):
        path = os.path.join(WAXHOLM_DIR, directory, '.'.join(f.split('.')[:-1]) + '.smp.mix')
        outfile = os.path.join('translations', '.'.join(f.split('.')[:-1]) + '.txt')
        with open(outfile, 'w') as out:
            for word, start, end in process(path):
                out.write('%s\n%s\n"%s"\n'%(start, end, word))
                
