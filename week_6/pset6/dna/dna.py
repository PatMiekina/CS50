import csv
import sys


def main():

    # Ensure correct usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py file.csv file.txt")

    people = []

    # Open database
    with open(sys.argv[1], "r") as database:
        d_reader = csv.DictReader(database)
        strs = d_reader.fieldnames
        for row in d_reader:
            people.append(row)

    # print(people)

    # Open sequence
    with open(sys.argv[2], "r") as f:
        sequence = f.read()

    results = check_substrings(sequence, strs)

    check_matches(people, results)

# Check repeat of str in sequence


def check_substrings(sequence, strs):
    # remove 'name' from keys
    strs.pop(0)
    results = {}

    for j in strs:
        repeat = 0
        tmp = 0
        for i in range(len(sequence)):
            if sequence[i:(i + len(j))] == j:
                tmp += 1
                if tmp > repeat:
                    repeat = tmp
                i += len(j)
                if sequence[i:(i+len(j))] != j:
                    tmp = 0
        results[j] = str(repeat)
    # print(results)
    return results

# check if counts of strs in the sequence match any person's count of strs


def check_matches(people, results):
    for person in people:
        name = person["name"]
        del person['name']
        if person == results:
            print(name)
            sys.exit(0)

    print("No match")


if __name__ == "__main__":
    main()