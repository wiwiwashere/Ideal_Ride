"""
This is the file I used to generates data and creates a CSV called random_rides_10000.csv in the same folder.
Each row has: Attraction, Latitude, Longitude
"""

import random
import csv

NUM_ROWS = 10_000
OUTPUT_FILE = "random_rides_10000.csv"

def random_coord(lat_range=(-90, 90), lon_range=(-180, 180)):
    """Return a (lat, lon) tuple with 6‑decimal precision."""
    lat = round(random.uniform(*lat_range), 6)
    lon = round(random.uniform(*lon_range), 6)
    return lat, lon

with open(OUTPUT_FILE, "w", newline="", encoding="utf-8") as f:
    writer = csv.writer(f)
    writer.writerow(["Attraction", "Latitude", "Longitude"])  # header row
    for i in range(1, NUM_ROWS + 1):
        name = f"Generated Ride {i:05d}"
        lat, lon = random_coord()
        writer.writerow([name, lat, lon])

print(f"✓ Created {OUTPUT_FILE} with {NUM_ROWS} rows.")
