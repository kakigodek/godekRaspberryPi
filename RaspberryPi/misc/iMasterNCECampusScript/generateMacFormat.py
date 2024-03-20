# generate from input file to iMasterNCE Campus format.
# Admission / Admission Resources / User Management / MAC Account
import os

# Function to list all files inside the directory
def list_files_in_directory():
    print("Files in the current directory:")
    files = os.listdir()
    for file in files:
        print(file)

# List all files inside the directory
list_files_in_directory()

# Ask the user for input and output file names
input_file = input("Enter the input file name: ")
output_file = input("Enter the output file name: ")

# Define the replacement MAC address suffix
replacement_suffix = ":11:11:11"

# Read the contents of the input file
with open(input_file, "r") as file:
    lines = file.readlines()

# Modify the MAC addresses in the lines
modified_lines = []
for line in lines:
    mac_address = line.strip()
    if len(mac_address) == 8 and ":" in mac_address:
        # Append the replacement suffix to the MAC address
        modified_mac = mac_address + replacement_suffix
        # Add the additional process after adding :11:11:11
        modified_mac += ">" + mac_address + ":FF:FF:FF"
    else:
        # If the line doesn't contain a valid MAC address, use the replacement suffix alone
        modified_mac = replacement_suffix
    modified_lines.append(modified_mac + "\n")

# Write the modified lines to the output file
with open(output_file, "w") as file:
    file.writelines(modified_lines)

print("MAC addresses have been modified. Check the output file:", output_file)
