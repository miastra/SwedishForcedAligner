import re

# Remove all non-phonemes and add spaces between all phonemes to fit MFA
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

# Remove everything but orthography and first transcription
def clean_up(line):
    new_line = re.sub(r'(;([^;]*;){10}){1}', " ", line, 1)
    new_line = re.sub(r';.*$', '', new_line)
    parts = new_line.split()
    parts[0] = parts[0].upper()
    parts[1] = split_xsampa(parts[1])
    new_line = ' '.join(parts) + "\n"
    return new_line

# Clean up NST lexicon and save to "NST_lexicon.txt"
new_lex = open("NST_lexicon.txt", "w")
with open("swe030224NST.pron") as old_lex:
    for line in old_lex:
        # Check that the entry is not marked as garbage
        if not 'GARB' in line:
            new_line = clean_up(line)
            new_lex.write(new_line)
new_lex.close()
old_lex.close()
