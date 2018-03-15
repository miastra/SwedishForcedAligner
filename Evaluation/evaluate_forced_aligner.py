"""
evaluate_forced_aligner.py
~~~~~~~~~~
This script is based on a script by Yejin Cho (scarletcho@gmail.com),
available at https://github.com/scarletcho/evaluateFA.

It was modified for the project Swedish Forced Alignment in the course
DT2112 Speech Technology at KTH Royal Institute of Technology spring 2018.
Information about usage and output can be found in the attached readme file.
"""

import sys,os
import re
import operator

global non_matching_files
non_matching_files = []
global matching
matching = 0

# Extract lines containing alignment times & label info
def get_times_labels(text_file):
    time_init = []
    time_end = []
    labels = []
    for i in range(0, len(text_file)):
        if (i+1) % 3 == 1:
            time_init.append(text_file[i])
        if (i+1) % 3 == 2:
            time_end.append(text_file[i])
        if (i+1) % 3 == 0:
            labels.append(text_file[i])
    return time_init, time_end, labels

# Check if two sets of labels match. If they do not, add file path to non_matching_files
def matching_alingments(labels_fa, labels_target, time_end_fa, time_end_target, text_fa_fname):
    global non_matching_files
    if (labels_fa != labels_target) | (len(time_end_fa) != len(time_end_target)):
        non_matching_files.append(text_fa_fname)
        return False
    else:
        global matching
        matching += 1
        return True

# Calculate all absolute errors for an alignment file
def calculateError(time_init_fa, time_init_target, time_end_fa, time_end_target):
    times_fa = []
    times_target = []
    for i in range(0, len(time_end_fa)):
        # Remove non-digits & Convert into floating point numbers
        times_fa.append(float(re.sub(r'[^0-9.]', '', time_end_fa[i])) * 1000)
        times_target.append(float(re.sub(r'[^0-9.]', '', time_end_target[i])) * 1000)
    # Add init times that are not also end times
    for i in range(0, len(time_init_fa)):
        if ((time_init_fa[i] not in time_end_fa) or (time_init_target[i] not in time_end_target)):
            times_fa.append(float(re.sub(r'[^0-9.]', '', time_init_fa[i])) * 1000)
            times_target.append(float(re.sub(r'[^0-9.]', '', time_init_target[i])) * 1000)
    # Compute the errors
    error = list(map(operator.sub, times_fa, times_target))
    error_absolute = list(map(abs, error))
    return error_absolute

# Compute the accuracy give all absolute errors and a tolerance level in ms
def calculateAccPercent(error_absolute, tolerance_ms):
    within_thresh = 0
    for i in range(0, len(error_absolute)):
        if error_absolute[i] <= tolerance_ms:
            within_thresh += 1
    accuracy_percent = round(within_thresh / float(len(error_absolute)) * 100, 2)
    return accuracy_percent

# Check the number of fa labels overlapping the midpoint of the corresponding target labels
def check_num_overlap(labels_fa, labels_target, time_init_fa, time_init_target, time_end_fa, time_end_target):
    num_match = 0
    manual_midpoints = [(float(time_end_target[i])+float(time_init_target[i]))/2 for i in range(len(time_end_target))]
    for l in range(0, len(labels_fa)):
        # Check that the labels are equal
        if (labels_fa[l] == labels_target[l] and float(time_init_fa[l]) <= manual_midpoints[l] and manual_midpoints[l] <= float(time_end_fa[l])):
            num_match += 1
    return num_match

# Evaluate an alignment file
def evaluate_alignment(text_fa_fname, text_target_fname):
    # Open the files and read their contents
    text_fa = open(text_fa_fname)
    text_target = open(text_target_fname)
    lines_fa = text_fa.readlines()
    lines_target = text_target.readlines()
    # Extract labels and start and end times
    [time_init_fa, time_end_fa, labels_fa] = get_times_labels(lines_fa)
    [time_init_target, time_end_target, labels_target] = get_times_labels(lines_target)
    # Check that the labels match
    if matching_alingments(labels_fa,labels_target,time_end_fa,time_end_target, text_fa_fname):
        # Compute number of labels, how many of these that match, and absolute errors
        num_labels = len(labels_fa)
        num_match = check_num_overlap(labels_fa, labels_target,time_init_fa, time_init_target, time_end_fa, time_end_target)
        error_absolute = calculateError(time_init_fa, time_init_target, time_end_fa, time_end_target)
        return num_labels, num_match, error_absolute
    else:
        # Else return nothing
        return 0, 0, []

# Print all evaluation informatino in the terminal
def print_evaluation_results(tot_error, tot_num_bound,tot_num_labels, tot_num_match, tot_error_absolute, tolerances_ms):
    global non_matching_files
    global matching
    print('---------Evaluation results----------')
    print('Matching set of labels: ' + str(matching)) + ' files'
    print('Non-matching set of labels: ' + str(len(non_matching_files))) + ' files'
    print('Average boundary error: ' + str(tot_error/tot_num_bound))
    print('Total number of boundaries: ' + str(tot_num_bound))
    print('Total number of labels: ' + str(tot_num_labels))
    print('Total number of overlapping labels: ' + str(tot_num_match))
    percent_match = round(float(tot_num_match)/tot_num_labels*100, 2)
    print('%-match = ' + str(percent_match) + ' %')
    print('Accuracies: ')
    for tol in tolerances_ms:
        accuracy_percent = calculateAccPercent(tot_error_absolute, tol)
        print('Tolerance ' + str(tol) + ' ms => ' + str(accuracy_percent) + ' %')

# Compute evaluation metrics for all files in the fa-folder
def evaluate_aligner(tolerances_ms, fa_path, target_path):
    print('Aligner: ' + fa_path)
    print('Target: ' + target_path)
    tot_error = 0
    tot_num_bound = 0
    tot_num_match = 0
    tot_error_absolute = []
    tot_num_labels = 0
    for fa in os.listdir(fa_path):
        fa_file = os.path.join(fa_path,fa)
        target_file = os.path.join(target_path, fa)
        num_labels, num_match, error_absolute = evaluate_alignment(fa_file, target_file)
        tot_error+= sum(error_absolute)
        tot_num_bound += len(error_absolute)
        tot_num_labels += num_labels
        tot_num_match += num_match
        tot_error_absolute.extend(error_absolute)
    # Print results
    print_evaluation_results(tot_error, tot_num_bound,tot_num_labels, tot_num_match, tot_error_absolute, tolerances_ms)
    # Save paths to all files that were skipped to non_matching_files.txt
    nons = open('non_matching_files.txt', 'w')
    nons.writelines(["%s\n" % item  for item in non_matching_files])

def main():
    # Example run
    tolerances_ms = [10, 25, 50, 100, 250]
    current_path = os.path.abspath(os.path.dirname(__file__))
    fa_path = os.path.join(current_path, 'FA_example')
    target_path = os.path.join(current_path, 'Target_example')
    evaluate_aligner(tolerances_ms, fa_path, target_path)

if __name__ == "__main__":
    main()
