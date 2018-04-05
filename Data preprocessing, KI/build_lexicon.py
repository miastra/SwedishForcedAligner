import os
from collections import defaultdict

basenames = set((os.path.join('DATA', f.split('.')[0]) for f in os.listdir('DATA')))

lexicon = defaultdict(list)

def split_xsampa(xsampa):
    pron = []
    for i in xsampa:
        if i in ('0', ':', '`', '\\', '\''):
            pron[-1] += i
        elif i in ('"', '$', '%'):
            continue
        else:
            pron.append(i)
    return ' '.join(pron)

for name in basenames:
    if False in map(os.path.exists, (name+'.LAB', name+'.FTARGET', name+'.FOBS')):
        print(name)
        continue
    ortowords = []
    targetprons = []
    obsprons = []
    with open(name + '.LAB', 'r') as lab:
        ortowords = lab.read().split()
    with open(name + '.FTARGET', 'r') as ftarget:
        targetprons = ftarget.read().split()
    with open(name + '.FOBS', 'r') as fobs:
        obsprons = fobs.read().split()

    for orto, target, obs in zip(ortowords, targetprons, obsprons):
        split_target = split_xsampa(target)
        split_obs = split_xsampa(obs)

        if split_target not in lexicon[orto]:
            lexicon[orto].append(split_target)
        
	# This can be uncommented to include the incorrect pronounciations
        #if split_target != split_obs:
            #lexicon[orto].append(split_obs)

with open('lexicon.DAT', 'w') as l_file:
    for orto in lexicon:
        for pron in lexicon[orto]:
            l_file.write('%s %s\n'%(orto, pron))
