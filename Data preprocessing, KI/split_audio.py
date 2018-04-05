import os
from collections import defaultdict

for f in os.listdir("Annotations"):
    utterance_dict = defaultdict(dict)
    fname = os.path.join("Annotations", f)
    wav_location = os.path.join('Wavs', f.split('_')[0] + '_INPUT1.WAV')
    name = f.split('_')[0]
    with open(fname, 'r') as annotations:
        for line in annotations:
            annotation = line.split('\t')
            start_time = annotation[2]
            category = annotation[0]
            
            if category in ('ortografi', 'fonologi_target', 'fonologi_obs'):
                utterance_dict[start_time][category] = {'end_time': annotation[3], 'duration': annotation[6], 'data': annotation[8]}
    for index, start_time in enumerate(utterance_dict.keys()):
        if 'ortografi' not in utterance_dict[start_time]:
            print '%s_%04d.LAB'%(name, index) + '\t MISSING'
            continue

        duration = utterance_dict[start_time]['ortografi']['duration']
        wavout = os.path.join('DATA', '%s_%04d.WAV'%(name, index))
        os.system('ffmpeg -v 0 -i %s -ss %s -t %s -acodec copy %s'%(wav_location, start_time, duration, wavout))
        with open(os.path.join('DATA', '%s_%04d.LAB'%(name, index)), 'w') as lab:
            lab.write(utterance_dict[start_time]['ortografi']['data'].upper())
        if 'fonologi_obs' in utterance_dict[start_time]:
            with open(os.path.join('DATA', '%s_%04d.FOBS'%(name, index)), 'w') as fobs:
                fobs.write(utterance_dict[start_time]['fonologi_obs']['data'])
        if 'fonologi_target' in utterance_dict[start_time]:
            with open(os.path.join('DATA', '%s_%04d.FTARGET'%(name, index)), 'w') as ftarget:
                ftarget.write(utterance_dict[start_time]['fonologi_target']['data'])
