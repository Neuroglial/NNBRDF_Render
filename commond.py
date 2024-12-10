import requests

url = "https://www.shadertoy.com/view/lstSRS"

response = requests.get(url,verify=False)

print(response)