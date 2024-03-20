# this script to list out all mac address from website url.
# from the output i will import to imaster nce campus.

import requests
from bs4 import BeautifulSoup

def fetch_mac_prefixes(url):
    # Send a GET request to the URL
    response = requests.get(url)

    # Check if the request was successful (status code 200)
    if response.status_code == 200:
        # Parse the HTML content of the webpage
        soup = BeautifulSoup(response.content, "html.parser")
        
        # Find all <tr> tags containing MAC prefixes
        mac_prefixes = soup.find_all("tr")
        
        # Iterate over the <tr> tags and extract the MAC prefixes
        for row in mac_prefixes:
            # Extract the MAC prefix from the first <td> tag
            mac_prefix = row.find("td").text.strip()
            print(mac_prefix)
    else:
        print("Failed to fetch data from the website.")

# Prompt the user to enter the URL
print("eg : https://maclookup.app/vendors/espressif-inc")
url = input("Enter the URL of the webpage: ")

# Call the function with the input URL
fetch_mac_prefixes(url)
