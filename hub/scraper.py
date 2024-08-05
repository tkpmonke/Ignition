import requests

def get_releases():
    url = "https://api.github.com/repos/juice2011/Ignition/releases"

    response = requests.get(url)
    response.raise_for_status()

    releases = response.json()

    for release in releases:
        print(release.get('tag_name'))

get_releases()
        
