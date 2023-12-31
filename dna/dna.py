import csv
import sys


def main():
    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py database sequence")

    # TODO: Read database file into a variable
    list = []
    with open(sys.argv[1]) as file:
        file_reader = csv.DictReader(file)
        counter = 0
        for person in file_reader:
            list += [person]
            for key in list[counter]:
                if not key == "name":
                    list[counter][key] = int(list[counter][key])
            counter += 1

    # TODO: Read DNA sequence file into a variable
    with open(sys.argv[2]) as file:
        text = file.read()

    # TODO: Find longest match of each STR in DNA sequence
    strObj = {}
    for c in list[0]:
        if not c == "name":
            strObj[c] = longest_match(text, c)

    # TODO: Check database for matching profiles
    for person in list:
        flag = True
        for key in person:
            if not key == "name":
                if not person[key] == strObj[key]:
                    flag = False
                    break
        if flag:
            print(person["name"])
            return

    print("No match")
    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):
        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:
            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
