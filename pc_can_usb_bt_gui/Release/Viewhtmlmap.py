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
# ladd1 = "265 Hilltop Circle, Tullahoma, TN, 37388"
# print("Location address:",ladd1)
# location = geolocator.geocode(ladd1)
# print("Latitude and Longitude of the said address:")
# print((location.latitude, location.longitude))

filepath = r"C:\Users\c17264\OneDrive - Microchip Technology, Inc\Projects\Python\HFO\index.html"

df = pd.read_csv(r"C:\Users\c17264\OneDrive - Microchip Technology, Inc\Projects\Python\HFO\Homes List.csv")
df.head()

df['Fulladd'] = df['Address'].map(str) + ', ' + df['City'].map(str) + ', ' + df['State'].map(str)
df['Coordinates']= df['Fulladd'].apply(geolocator.geocode)
print(df)
df['latitude']=df['Coordinates'].apply(lambda x: x.latitude)
df['longitude']=df['Coordinates'].apply(lambda x: x.longitude)

print(df)
df.to_csv(r"C:\Users\c17264\OneDrive - Microchip Technology, Inc\Projects\Python\HFO\HomeCordList.csv")

m =  folium.Map(location=[38.228964, -85.764765],
                        #zoom_start=7)
                        zoom_start=7
                        ,tiles='Stamen Toner')
# tile = folium.TileLayer(
#         tiles = 'https://server.arcgisonline.com/ArcGIS/rest/services/World_Imagery/MapServer/tile/{z}/{y}/{x}',
#         attr = 'Esri',
#         name = 'Esri Satellite',
#         overlay = False,
#         control = True
#        ).add_to(m)

df.apply(lambda row:folium.CircleMarker(location=[row["latitude"], row["longitude"]]).add_to(m), axis=1)

m.save(filepath)

webbrowser.open('file://' + filepath)