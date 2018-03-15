import os
import re

# Extract labels and start and end times from a TextGrid
def extract_word_alignments(tg_path):
    textgrid = open(tg_path)
    lines = textgrid.readlines()
    intervaltier_idx = []
    for i in range(0, len(lines)):
        if re.search('IntervalTier', lines[i]):
            intervaltier_idx.append(i)
    word_ali_lines = []
    for j in range(intervaltier_idx[0], intervaltier_idx[1]):
        if re.search('intervals \[', lines[j]):
            text = re.sub('[\t]*text = ', '', lines[j+3])
            if not text in ['"sil"\n', '"sp"\n', '""\n']:
                word_ali_lines.append(re.sub('[\t]*xmin = ', '', lines[j+1]))
                word_ali_lines.append(re.sub('[\t]*xmax = ', '', lines[j+2]))
                word_ali_lines.append(text)
    return word_ali_lines

# Create a folder with textfiles based on the TextGrids in the folder textgrid_path
def textfiles_from_tg_folder(textgrid_path, textfile_folder_path, model_name):
    # Check if the folder exists, otherwise create it
    if not os.path.exists(textfile_folder_path):
        os.makedirs(textfile_folder_path)
    # Loop over all TextGrids
    for tg in os.listdir(textgrid_path):
        tg_path = os.path.join(textgrid_path, tg)
        textgrid = open(tg_path)
        # Extract relevant information from the TextGrid
        align_info = extract_word_alignments(tg_path)
        # Save it in a textfile
        textfile_path = (os.path.join(textfile_folder_path, (tg + '.txt'))).replace('.TextGrid', '')
        textfile = open(textfile_path, 'w')
        for item in align_info:
            textfile.write(item)

# Create textfiles from all TextGrids structured the way MFA outputs them
def textfiles_from_folder_with_folders(textgrid_folder_path, textfile_folder_path, model_name):
    for folder in os.listdir(textgrid_folder_path):
        textgrid_path = os.path.join(textgrid_folder_path, folder)
        textfiles_from_tg_folder(textgrid_path, textfile_folder_path, model_name)

def main():
    # Example run
    # Path to the folder containing folders with TextGrids
    current_path = os.path.abspath(os.path.dirname(__file__))
    textgrid_folder_path = os.path.join(current_path, 'Textgrid_2_textfile_example', 'TextGrids')
    # Model name (for naming folders and files in a logical way)
    model_name = 'Example'
    # Path to the folder in which the textfiles should be saved
    textfile_folder_path = os.path.join('Textgrid_2_textfile_example', 'Textfiles', model_name)
    textfiles_from_folder_with_folders(textgrid_folder_path, textfile_folder_path, model_name)

if __name__ == "__main__":
    main()
