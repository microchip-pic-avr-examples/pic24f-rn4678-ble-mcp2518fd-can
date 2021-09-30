#from geopy.geocoders import Nominatim
from geopy.geocoders import GoogleV3
import webbrowser
import folium
import pandas as pd

keys_file = open("keys.txt")
lines = keys_file.readlines()
GoogleAPI_key = lines[0].rstrip()

#geolocator = Nominatim(user_agent="Testexample")
geolocator = GoogleV3(api_key = GoogleAPI_key)
ladd1 = "265 Hilltop Circle, Tullahoma, TN, 37388"
print("Location address:",ladd1)
location = geolocator.geocode(ladd1)
print("Latitude and Longitude of the said address:")
print((location.latitude, location.longitude, location))

