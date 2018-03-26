import os

# Return a set with all files that that are not common for all folders
def differing_files(folders):
    num_folders = len(folders)
    files_per_folder = []
    all_files = set()
    common_files = set()
    # Add the files in all foders to the right sets and lists
    for i in range(num_folders):
        current_files = set()
        for filename in os.listdir(folders[i]):
            current_files.add(filename)
            all_files.add(filename)
        files_per_folder.append(current_files)
    # Intersection between the folders
    for filename in all_files:
        absent = False
        for i in range(num_folders):
            if not filename in files_per_folder[i]:
                absent = True
        if not absent:
            common_files.add(filename)
    # The differing files
    differing_files = all_files - common_files
    return differing_files

# Removes the differing files from all folders in folders
def remove_differing_files(differing_files, folders):
    for folder in folders:
        for filename in os.listdir(folder):
            if filename in differing_files:
                os.remove(os.path.join(folder, filename))

def main():
    # Example run
    # Paths to folders to be manipulated
    current_path = os.path.abspath(os.path.dirname(__file__))
    textgrid_folder_path = os.path.join(current_path, 'Textgrid_2_textfile_example', 'TextGrids')
    folder1 = os.path.join(current_path, 'Differing_file_functions_example', 'Folder1')
    folder2 = os.path.join(current_path, 'Differing_file_functions_example', 'Folder2')
    # A list of absolute paths to the folders
    folders = [folder1, folder2]
    # Function calls
    diffs = differing_files(folders)
    remove_differing_files(diffs, folders)

if __name__ == "__main__":
    main()
