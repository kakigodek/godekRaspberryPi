#!/bin/bash

# Define the color code and corresponding values
declare -A color_code
color_code=(
    ["black"]=0
    ["brown"]=1
    ["red"]=2
    ["orange"]=3
    ["yellow"]=4
    ["green"]=5
    ["blue"]=6
    ["violet"]=7
    ["gray"]=8
    ["white"]=9
)

# Function to calculate the resistance value
calculate_resistance_value() {
    local first_band=${color_code[$1]}
    local second_band=${color_code[$2]}
    local multiplier_band=${color_code[$3]}

    if [[ -z $first_band || -z $second_band || -z $multiplier_band ]]; then
        echo "Invalid color(s) detected."
        exit 1
    fi

    local resistance=$((($first_band * 10 + $second_band) * (10 ** $multiplier_band)))

    if ((resistance >= 1000 && resistance < 1000000)); then
        resistance=$((resistance / 1000))
        echo "Resistance value: $resistance kΩ"
    elif ((resistance >= 1000000)); then
        resistance=$((resistance / 1000000))
        echo "Resistance value: $resistance MΩ"
    else
        echo "Resistance value: $resistance Ω"
    fi
}

# Display the list of color codes
echo "Color Code List:"
for color in "${!color_code[@]}"; do
    echo "$color: ${color_code[$color]}"
done

# Input the color bands
read -p "Enter the resistor color bands (space-separated, e.g., red brown green): " band1 band2 band3

# Calculate and display the resistance value
calculate_resistance_value "$band1" "$band2" "$band3"

