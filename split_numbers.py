# Open the file 'numbers.txt' in read mode
with open('numbers.txt', 'r') as file:
    # Read the entire content of the file
    content = file.read()

# Split the content into individual numbers using whitespace as the separator
numbers = content.split()

# Create a new list to store the separated numbers
separated_numbers = []

# Iterate over each number
for number in numbers:
    # Add each number to the separated_numbers list on a new line
    separated_numbers.append(number + '\n')

# Open a new file 'separated_numbers.txt' in write mode
with open('separated_numbers.txt', 'w') as file:
    # Write the separated numbers to the new file
    file.writelines(separated_numbers)